#include <stdio.h>
#include <cs50.h>
#include <ctype.h>

float count_letters(string text);
float words(string text);
float sentense(string text);

int main(void)
{
    string text = get_string("Text: ");
    float count = count_letters(text);
    float word_count = words(text);
    float sen_count = sentense(text);
    float L = 100 * (count / words(text)) ;
    float S = 100 * (sen_count / word_count);
    float index = 0.0588 * L - 0.296 * S - 15.8;
    // printf ("The numbers of letters are %i and words are %i , sentense %i\n" ,count, word_count, sen_count);
    if (index <= 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.0f\n", index);

    }
}

float count_letters(string text)
{
    float count = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i]))
        {
            count++;
        }

    }
    return count;
}

float words(string text)
{
    float word = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == ' ')
        {
            word++;
        }
    }
    return ++word;
}

float sentense(string text)
{
    float sen = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sen++;
        }
    }
    return sen;
}