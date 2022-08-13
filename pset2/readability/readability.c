#include <stdio.h>
#include <cs50.h>
#include <math.h>

int get_letters_count(string text);
int get_words_count(string text);
int get_sentences_count(string text);
int get_grade_level(int letters_count, int words_count, int sentences_count);

int main(void)
{
    string text = get_string("Text: ");

    int letters_count = get_letters_count(text);
    // printf("%i\n", letters_count);
    int words_count = get_words_count(text);
    // printf("%i\n", words_count);
    int sentences_count = get_sentences_count(text);
    // printf("%i\n", sentences_count);

    int grade = get_grade_level(letters_count, words_count, sentences_count);
    // printf("%i\n", grade);

    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int get_letters_count(string text)
{
    const int A_UPPER_CODE = 65;
    const int Z_UPPER_CODE = 90;
    const int A_LOWER_CODE = 97;
    const int Z_LOWER_CODE = 122;

    int letters_count = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] >= A_UPPER_CODE && text[i] <= Z_UPPER_CODE)
        {
            letters_count++;
        }
        else if (text[i] >= A_LOWER_CODE && text[i] <= Z_LOWER_CODE)
        {
            letters_count++;
        }
    }

    return letters_count;
}

int get_words_count(string text)
{
    const int SPACE_CODE = 32;

    int words_count = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == SPACE_CODE)
        {
            words_count++;
        }
    }

    if (words_count > 0)
    {
        words_count++;
    }

    return words_count;
}

int get_sentences_count(string text)
{
    const int PERIOD_CODE = 46;
    const int EXCLAMATION_POINT_CODE = 33;
    const int QUESTION_MARK_CODE = 63;

    int sentences_count = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == PERIOD_CODE || text[i] == EXCLAMATION_POINT_CODE || text[i] == QUESTION_MARK_CODE)
        {
            sentences_count++;
        }
    }

    return sentences_count;
}

int get_grade_level(int letters_count, int words_count, int sentences_count)
{
    float average_letters_count = ((float) letters_count / (float) words_count) * 100;
    float average_sentences_count = ((float) sentences_count / (float) words_count) * 100;

    int grade_level = round(0.0588 * average_letters_count - 0.296 * average_sentences_count - 15.8);

    return grade_level;
}