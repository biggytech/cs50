-- Keep a log of any SQL queries you execute as you solve the mystery.

-- find the crime
select * from crime_scene_reports where day = 28 and month = 7 and year = 2021 and street = 'Humphrey Street';
-- id = 295
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time –
-- each of their interview transcripts mentions the bakery.

select * from interviews where day = 28 and month = 7 and year = 2021;
-- -------------------------------------------------------------------------------------------------------------------------------------+
-- | id  |  name   | year | month | day |                                                                                                                                                     transcript                                                                                                                                                      |
-- +-----+---------+------+-------+-----+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+                                                                                                                 |
-- | 161 | Ruth    | 2021 | 7     | 28  | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
-- | 162 | Eugene  | 2021 | 7     | 28  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
-- | 163 | Raymond | 2021 | 7     | 28  | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |                                                                       |
-- +-----+---------+------+-------+-----+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

-- Ruth:
-- within ten minutes of the theft
-- parking lot left car

-- Eugene:
-- Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street
-- and saw the thief there withdrawing some money

-- Raymond:
-- they called someone (a caller)
-- phone call - less than a minute - the earliest flight out of Fiftyville tomorrow (29 July).
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- TODO:
-- 1. check atm_transactions
-- 2. check phone_calls
-- 3. check flights
-- 4. check cars

-- 3. check flights
select * from flights where day = 29 and month = 7 and year = 2021 order by hour, minute limit 1;
-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+

-- Origin airport:
select * from airports where id = 8;
-- +----+--------------+-----------------------------+------------+
-- | id | abbreviation |          full_name          |    city    |
-- +----+--------------+-----------------------------+------------+
-- | 8  | CSF          | Fiftyville Regional Airport | Fiftyville |
-- +----+--------------+-----------------------------+------------+
-- Destination airport
select * from airports where id = 4;
-- +----+--------------+-------------------+---------------+
-- | id | abbreviation |     full_name     |     city      |
-- +----+--------------+-------------------+---------------+
-- | 4  | LGA          | LaGuardia Airport | New York City |
-- +----+--------------+-------------------+---------------+
-- ESCAPED TO?: New York City

-- 1. check atm_transactions
select name, phone_number, passport_number, license_plate from atm_transactions
join bank_accounts on atm_transactions.account_number = bank_accounts.account_number
join people on bank_accounts.person_id = people.id
where atm_transactions.day = 28
and atm_transactions.month = 7
and atm_transactions.year = 2021
and atm_transactions.atm_location = 'Leggett Street'
and atm_transactions.transaction_type = 'withdraw';
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+
-- | id  | account_number | year | month | day |  atm_location  | transaction_type | amount |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+
-- | 246 | 28500762       | 2021 | 7     | 28  | Leggett Street | withdraw         | 48     |
-- | 264 | 28296815       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
-- | 266 | 76054385       | 2021 | 7     | 28  | Leggett Street | withdraw         | 60     |
-- | 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     |
-- | 269 | 16153065       | 2021 | 7     | 28  | Leggett Street | withdraw         | 80     |
-- | 288 | 25506511       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
-- | 313 | 81061156       | 2021 | 7     | 28  | Leggett Street | withdraw         | 30     |
-- | 336 | 26013199       | 2021 | 7     | 28  | Leggett Street | withdraw         | 35     |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+

-- People from here ^
-- +---------+----------------+-----------------+---------------+
-- |  name   |  phone_number  | passport_number | license_plate |
-- +---------+----------------+-----------------+---------------+
-- | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       |
-- | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
-- | Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
-- +---------+----------------+-----------------+---------------+

-- 2. check phone_calls
select people.name from phone_calls
join people on phone_calls.receiver = people.phone_number
where phone_calls.day = 28 and phone_calls.month = 7 and phone_calls.year = 2021 and phone_calls.duration < 60;
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | id  |     caller     |    receiver    | year | month | day | duration |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | 221 | (130) 555-0289 | (996) 555-8899 | 2021 | 7     | 28  | 51       |
-- | 224 | (499) 555-9472 | (892) 555-8872 | 2021 | 7     | 28  | 36       |
-- | 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
-- | 251 | (499) 555-9472 | (717) 555-1342 | 2021 | 7     | 28  | 50       |
-- | 254 | (286) 555-6063 | (676) 555-6554 | 2021 | 7     | 28  | 43       |
-- | 255 | (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49       |
-- | 261 | (031) 555-6622 | (910) 555-3251 | 2021 | 7     | 28  | 38       |
-- | 279 | (826) 555-1652 | (066) 555-9701 | 2021 | 7     | 28  | 55       |
-- | 281 | (338) 555-6650 | (704) 555-2131 | 2021 | 7     | 28  | 54       |
-- +-----+----------------+----------------+------+-------+-----+----------+

-- Callers:
-- +---------+
-- |  name   |
-- +---------+
-- | Sofia   |
-- | Kelsey  |
-- | Bruce   | <- Match with atm transactions
-- | Kelsey  |
-- | Taylor  | <- Match with atm transactions
-- | Diana   | <- Match with atm transactions
-- | Carina  |
-- | Kenny   | <- Match with atm transactions
-- | Benista | <- Match with atm transactions
-- +---------+

-- Receivers:
-- +------------+
-- |    name    |
-- +------------+
-- | Jack       |
-- | Larry      |
-- | Robin      |
-- | Melissa    |
-- | James      |
-- | Philip     |
-- | Jacqueline |
-- | Doris      |
-- | Anna       |
-- +------------+

-- Possible thiefs:
-- Bruce (transactions, caller, car)
-- Taylor (transactions, caller)
-- Diana (transactions, caller, car)
-- Kenny (transactions, caller)
-- Benista (transactions, caller)

-- Possible accomps:
-- | Jack       | (receiver)
-- | Larry      | (receiver)
-- | Robin      | (receiver)
-- | Melissa    | (receiver)
-- | James      | (receiver)
-- | Philip     | (receiver)
-- | Jacqueline | (receiver)
-- | Doris      | (receiver)
-- | Anna       | (receiver)
-- +------------+

-- 4. check cars
select people.name from bakery_security_logs
join people on bakery_security_logs.license_plate = people.license_plate
where bakery_security_logs.day = 28 and bakery_security_logs.month = 7 and bakery_security_logs.year = 2021 and bakery_security_logs.hour = 10 and bakery_security_logs.minute >=15 and bakery_security_logs.minute <= 25;
-- +---------+
-- |  name   |
-- +---------+
-- | Vanessa |
-- | Bruce   | + car
-- | Barry   |
-- | Luca    |
-- | Sofia   |
-- | Iman    |
-- | Diana   | + car
-- | Kelsey  |
-- +---------+

-- Possible thiefs:
-- Bruce (transactions, caller, car, flight)
-- Taylor (transactions, caller, flight)
-- Diana (transactions, caller, car)
-- Kenny (transactions, caller, flight)
-- Benista (transactions, caller)

-- Check passengers
select people.name from passengers
join people on passengers.passport_number = people.passport_number
where passengers.flight_id = 36;
-- +--------+
-- |  name  |
-- +--------+
-- | Doris  |
-- | Sofia  |
-- | Bruce  | + flight
-- | Edward |
-- | Kelsey |
-- | Taylor |
-- | Kenny  |
-- | Luca   |
-- +--------+

-- Thief is Bruce

-- Accomp is:
select people.name from phone_calls
join people on phone_calls.receiver = people.phone_number
where phone_calls.day = 28 and phone_calls.month = 7 and phone_calls.year = 2021 and phone_calls.duration < 60
and phone_calls.caller = '(367) 555-5533';
-- Robin