#include <stdio.h>
#include <cs50.h>

long get_card_number(void);
string get_card_type(long number);
bool is_amex(long number);
bool is_mastercard(long number);
bool is_visa(long number);
bool is_valid_card_number(long number);

int main(void)
{
    long number = get_card_number();
    string type = get_card_type(number);
    printf("%s\n", type);
}

long get_card_number(void)
{
    long number = get_long("Number: ");
    return number;
}

string get_card_type(long number)
{
    if (is_amex(number) && is_valid_card_number(number))
    {
        return "AMEX";
    }
    else if (is_mastercard(number) && is_valid_card_number(number))
    {
        return "MASTERCARD";
    }
    else if (is_visa(number) && is_valid_card_number(number))
    {
        return "VISA";
    }
    else
    {
        return "INVALID";
    }
}

bool is_amex(long number)
{
    int number_start = number * 0.0000000000001;
    bool is_length_valid = (number != 0 && number / 1000000000000000 < 1 && number / 100000000000000 >= 1); // got 15 digits in it
    bool is_number_start_valid = number_start == 34 || number_start == 37;

    return is_length_valid && is_number_start_valid;
}

bool is_mastercard(long number)
{
    int number_start = number * 0.00000000000001;
    bool is_length_valid = (number != 0 && number / 10000000000000000 < 1 && number / 1000000000000000 >= 1); // got 16 digits in it
    bool is_number_start_valid = number_start >= 51 && number_start <= 55;

    return is_length_valid && is_number_start_valid;
}

bool is_visa(long number)
{
    int number_start;

    if (number != 0 && number / 10000000000000 < 1 && number / 1000000000000 >= 1) // got 13 digits in it
    {
        number_start = number * 0.000000000001;
        return number_start == 4;
    }
    else if (number != 0 && number / 10000000000000000 < 1 && number / 1000000000000000 >= 1) // got 16 digits in it
    {
        number_start = number * 0.000000000000001;
        return number_start == 4;
    }
    else
    {
        return false;
    }
}

bool is_valid_card_number(long number)
{
    int sum_of_multiplied_by_two = 0, others_sum = 0;

    for (int counter = 0; number > 0; counter++, number /= 10)
    {
        if (counter % 2 == 0)
        {
            others_sum += number % 10;
        }
        else
        {
            int multiplied_digit = (number % 10) * 2;
            while (multiplied_digit > 0)
            {
                sum_of_multiplied_by_two += multiplied_digit % 10;
                multiplied_digit /= 10;
            }
        }
    }

    return (sum_of_multiplied_by_two + others_sum) % 10 == 0;
}