import os
import time

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

db.execute("CREATE TABLE IF NOT EXISTS transactions (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER NOT NULL, stock_symbol TEXT NOT NULL, price NUMERIC NOT NULL, count INTEGER NOT NULL, is_bought INTEGER NOT NULL, timestamp INTEGER NOT NULL)")
db.execute("CREATE TABLE IF NOT EXISTS balances (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER NOT NULL, stock_symbol TEXT NOT NULL, count INTEGER NOT NULL)")

db.execute("CREATE UNIQUE INDEX IF NOT EXISTS user_id ON balances (user_id)")
db.execute("CREATE INDEX IF NOT EXISTS stock_symbol ON balances (stock_symbol)")
db.execute("CREATE INDEX IF NOT EXISTS count ON balances (count)")

db.execute("CREATE INDEX IF NOT EXISTS trans_user_id ON transactions (user_id)")
db.execute("CREATE INDEX IF NOT EXISTS timestamp ON transactions (timestamp)")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    user_id = session.get("user_id")
    stocks = []
    total = 0

    balances = db.execute("SELECT * FROM balances WHERE user_id = ? AND count > 0", user_id)

    for balance in balances:
        stock = lookup(balance["stock_symbol"])
        stock["count"] = balance["count"]
        stock["total"] = balance["count"] * stock["price"]
        stocks.append(stock)
        total += stock["total"]

    users = db.execute("SELECT * FROM users WHERE id = ?", user_id)

    total += users[0]["cash"]

    return render_template("index.html", stocks=stocks, total=total, cash=users[0]["cash"])


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("must provide symbol", 400)

        if not shares:
            return apology("must provide shares", 400)

        try:
            int(shares)
        except ValueError:
            return apology("shares must be positive integer", 400)

        if int(shares) <= 0:
            return apology("shares must be positive integer", 400)

        stock = lookup(symbol)

        if not stock:
            return apology("not found", 400)

        user_id = session.get("user_id")

        user = db.execute("SELECT * FROM users WHERE id = ?", user_id)

        transaction = stock["price"] * int(shares)

        if user[0]["cash"] < transaction:
            return apology("you don't have so much cash", 400)

        db.execute("INSERT INTO transactions (user_id, stock_symbol, price, count, is_bought, timestamp) VALUES (?, ?, ?, ?, ?, ?)",
                   user_id, symbol, stock["price"], shares, 1, int(time.time()))

        balance = db.execute("SELECT * FROM balances WHERE user_id = ? AND stock_symbol = ?", user_id, symbol)

        if balance:
            db.execute("UPDATE balances SET count = ? WHERE user_id = ? AND stock_symbol = ?",
                       balance[0]["count"] + int(shares), user_id, symbol)
        else:
            db.execute("INSERT INTO balances (user_id, stock_symbol, count) VALUES (?, ?, ?)", user_id, symbol, shares)

        db.execute("UPDATE users SET cash = ? WHERE id = ?", user[0]["cash"] - transaction, user_id)

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    user_id = session.get("user_id")

    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY timestamp DESC", user_id)

    for transaction in transactions:
        stock = lookup(transaction["stock_symbol"])
        transaction["name"] = stock["name"]
        transaction["symbol"] = stock["symbol"]
        transaction["is_bought"] = bool(transaction["is_bought"])
        transaction["total"] = transaction["price"] * transaction["count"]
        transaction["date"] = datetime.utcfromtimestamp(transaction["timestamp"]).strftime('%Y-%m-%d')
        transaction["time"] = datetime.utcfromtimestamp(transaction["timestamp"]).strftime('%H:%M:%S')

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":

        symbol = request.form.get("symbol")

        if not symbol:
            return apology("must provide symbol", 400)

        stock = lookup(symbol)

        if not stock:
            return apology("not found", 400)

        return render_template("quoted.html", stock=stock)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            return apology("must provide username", 400)

        if not password:
            return apology("must provide password", 400)

        if not confirmation:
            return apology("must provide confirmation", 400)

        if password != confirmation:
            return apology("password not equal confirmation", 400)

        if not any(char.isdigit() for char in password) or len(password) < 8:
            return apology("password must contain digits and be at least 8 characters long", 400)

        users = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(users) != 0:
            return apology("user exists", 400)

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("must provide symbol", 400)

        if not shares:
            return apology("must provide shares", 400)

        try:
            int(shares)
        except ValueError:
            return apology("shares must be positive integer", 400)

        if int(shares) <= 0:
            return apology("shares must be positive integer", 400)

        user_id = session.get("user_id")

        balances = db.execute("SELECT * FROM balances WHERE user_id = ? AND stock_symbol = ? AND count > 0", user_id, symbol)

        if len(balances) == 0:
            return apology("you don't have these stocks", 400)

        if balances[0]["count"] < int(shares):
            return apology("you don't have so much these stocks", 400)

        user = db.execute("SELECT * FROM users WHERE id = ?", user_id)

        stock = lookup(symbol)

        transaction = stock["price"] * int(shares)

        db.execute("INSERT INTO transactions (user_id, stock_symbol, price, count, is_bought, timestamp) VALUES (?, ?, ?, ?, ?, ?)",
                   user_id, symbol, stock["price"], shares, 0, int(time.time()))

        db.execute("UPDATE balances SET count = ? WHERE user_id = ? AND stock_symbol = ?",
                   balances[0]["count"] - int(shares), user_id, symbol)

        db.execute("UPDATE users SET cash = ? WHERE id = ?", user[0]["cash"] + transaction, user_id)

        return redirect("/")

    else:
        user_id = session.get("user_id")

        balances = db.execute("SELECT * FROM balances WHERE user_id = ? AND count > 0", user_id)

        stocks = []

        for balance in balances:
            stock = lookup(balance["stock_symbol"])
            stock["symbol"] = stock["symbol"].lower()
            stocks.append(stock)

        return render_template("sell.html", stocks=stocks)
