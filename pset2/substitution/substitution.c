#include <stdio.h>
#include <cs50.h>
#include <ctype.h>

bool is_valid_length(string text, int expected_count);
bool is_valid_key(string key);
void convert_to_uppercase(string text);
void cipher(string text, string key);

const int A_UPPER_CODE = 65;
const int Z_UPPER_CODE = 90;
const int A_LOWER_CODE = 97;
const int Z_LOWER_CODE = 122;

int main(int argc, string argv[])
{
    string key;

    if (argc != 2)
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }
    else if (!is_valid_length(argv[1], 26))
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    key = argv[1];
    convert_to_uppercase(key);

    if (!is_valid_key(key))
    {
        printf("Invalid key\n");
        return 1;
    }

    string text = get_string("plaintext: ");
    cipher(text, key);
    printf("ciphertext: %s\n", text);

    return 0;
}

bool is_valid_length(string text, int expected_count)
{
    int counted_letters = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        counted_letters++;
    }

    return counted_letters == expected_count;
}

bool is_valid_key(string key)
{
    bool chars_codes_existence[26] = { false };

    for (int i = 0; key[i] != '\0'; i++)
    {
        if (!(key[i] >= A_UPPER_CODE && key[i] <= Z_UPPER_CODE) && !(key[i] >= A_LOWER_CODE && key[i] <= Z_LOWER_CODE))
        {
            return false;
        }

        int char_code = key[i] - A_UPPER_CODE;
        bool is_char_repeated = chars_codes_existence[char_code];

        // repeated char with this code
        if (is_char_repeated)
        {
            return false;
        }

        chars_codes_existence[char_code] = true;
    }

    return true;
}

void convert_to_uppercase(string text)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        text[i] = toupper(text[i]);
    }
}

void cipher(string text, string key)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] >= A_UPPER_CODE && text[i] <= Z_UPPER_CODE)
        {
            int key_position = text[i] - A_UPPER_CODE;
            text[i] = key[key_position];
        }
        else if (text[i] >= A_LOWER_CODE && text[i] <= Z_LOWER_CODE)
        {
            int key_position = text[i] - A_LOWER_CODE;
            text[i] = tolower(key[key_position]);
        }
    }
}