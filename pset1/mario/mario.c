#include <stdio.h>

#include <cs50.h>

int input(void);
int main(void)
{
    int height = input();
    for (int i = 1; i <= height; i++)
    {
        //first spaces
        for (int j = 1; j <= height - i; j++)
        {
            printf(" ");
        }
        //left side hashes
        for (int k = 1; k <= i; k++)
        {
            printf("#");
        }
        //middle space
        printf("  ");

        //right side hashes
        for (int k = 1; k <= i; k++)
        {
            printf("#");
        }
        //newline after every row.
        printf("\n");
    }
}

int input(void)
{
    //declare height first
    int height;
    do
    {
        height = get_int("Enter the height of the pyramid: ");
    }
    //asks until input is between 1-8
    while (height <= 0 || height > 8);
    return height;
}