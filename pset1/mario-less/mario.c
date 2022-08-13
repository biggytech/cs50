#include <stdio.h>
#include <cs50.h>

int get_height(void);
void draw_two_pyramids(int height);
void print_spaces(int num);
void print_hashes(int num);

int main(void)
{
    int height = get_height();
    draw_two_pyramids(height);
}

int get_height(void)
{
    int height;

    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    return height;
}

void draw_two_pyramids(int height)
{
    for (int row = 1; row <= height; row++)
    {
        int spaces_in_pyramid = height - row;
        print_spaces(spaces_in_pyramid);
        print_hashes(row);
        printf("\n");
    }
}

void print_spaces(int num)
{
    for (int i = 0; i < num; i++)
    {
        printf(" ");
    }
}

void print_hashes(int num)
{
    for (int i = 0; i < num; i++)
    {
        printf("#");
    }
}