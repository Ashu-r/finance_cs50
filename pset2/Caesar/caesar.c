#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    bool right = true;

    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        // printf("%i\n%s\n",argc,argv[1]);
        for (int i = 0; argv[1][i] != '\0' ; i++)
        {
            if (! isdigit(argv[1][i]))
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }

    }
    if (true)
    {
        string original = get_string("plaintext: ");
//       printf("%s",argv[1]);
        int val = atoi(argv[1]);
        printf("ciphertext: ");
        int len = strlen(original);
        for (int j = 0; j < len; j++)
        {
            if (original[j] >= 'A' && original[j] <= 'Z')
            {
                original[j] -= 65;
                printf("%c", ((original[j] + val) % 26) + 65);

            }
            else if (original[j] >= 'a' && original[j] <= 'z')
            {
                original[j] -= 97;
                printf("%c", ((original[j] + val) % 26) + 97);

            }
            else
            {
                printf("%c", original[j]);
            }
            // neww [j] = original[j] + val;
            // neww [j] = (char)neww[j];
        }
        printf("\n");
    }
}