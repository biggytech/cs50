#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>

bool is_positive_integer(string text);
void cipher(string plaintext, int key);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }
    else if (!is_positive_integer(argv[1]))
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }

    int key = atoi(argv[1]);
    string text = get_string("plaintext: ");
    cipher(text, key);
    printf("ciphertext: %s\n", text);

    return 0;
}

bool is_positive_integer(string text)
{
    const int ZERO_CODE = 48;
    const int NINE_CODE = 57;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] < ZERO_CODE || text[i] > NINE_CODE)
        {
            return false;
        }
    }

    return true;
}

void cipher(string text, int key)
{
    const int A_UPPER_CODE = 65;
    const int Z_UPPER_CODE = 90;
    const int A_LOWER_CODE = 97;
    const int Z_LOWER_CODE = 122;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] >= A_UPPER_CODE && text[i] <= Z_UPPER_CODE)
        {
            int new_code = text[i] + key % 26;
            if (new_code > Z_UPPER_CODE)
            {
                new_code -= 26;
            }
            text[i] = new_code;
        }
        else if (text[i] >= A_LOWER_CODE && text[i] <= Z_LOWER_CODE)
        {
            int new_code = text[i] + key % 26;
            if (new_code > Z_LOWER_CODE)
            {
                new_code -= 26;
            }
            text[i] = new_code;
        }
    }
}