import re


def main():
    number = get_card_number()
    card_type = get_card_type(number)
    print(card_type)


def get_card_number():
    number = input("Number: ")
    return number


def get_card_type(number):
    if is_card_valid(number):
        if is_amex(number):
            return "AMEX"
        elif is_mastercard(number):
            return "MASTERCARD"
        elif is_visa(number):
            return "VISA"

    return "INVALID"


def is_card_valid(card_number):
    digits_count = len(card_number)
    card_number = int(card_number)

    sum = 0

    for i in range(digits_count):
        digit = card_number % 10
        card_number //= 10

        if i % 2 == 0:
            sum += digit
        else:
            digit *= 2

            while digit > 0:
                sum += digit % 10
                digit //= 10

    return sum % 10 == 0


def is_amex(number):
    # should be "3[47]", but here it's ok since there is no commas in a provided card number
    return len(number) == 15 and re.compile("3[4,7]").match(number)


def is_mastercard(number):
    return len(number) == 16 and re.compile("5[1-5]").match(number)


def is_visa(number):
    number_length = len(number)

    return number_length in [13, 16] and re.compile("4").match(number)


main()
# print(is_card_valid("4003600000000014"))
# print(is_card_valid("4003600000000015"))