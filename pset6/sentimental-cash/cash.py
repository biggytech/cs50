from cs50 import get_float


def main():
    change = get_change()
    coins = calculate_coins(change)
    print(coins)


def get_change():
    while True:
        change = get_float("Change owed: ")
        if change >= 0:
            return change


def calculate_coins(money):
    # set money in cents
    money *= 100

    total_coins = 0
    available_coins = [25, 10, 5, 1]

    for coin in available_coins:
        while coin <= money:
            # print(f"Coin: {coin}, Money: {money}, Total coins: {total_coins}")
            money -= coin
            total_coins += 1

    return total_coins


main()