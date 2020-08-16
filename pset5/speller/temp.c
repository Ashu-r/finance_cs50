#include <stdio.h>
#include <stdbool.h>

int main()
{
    FILE *f =fopen("dictionaries/small", "r");
    if(f==NULL)
        return 0;

    char word[45];
    while(true)
    {
        if(fscanf(f, "%s", word) == EOF)
            break;

        printf("%s\n",word);
    }

}