#include <stdio.h>

#include <cs50.h>

#include <string.h>

bool Validity();
long int card;

int main(void)
{
    if (Validity())
    {
        char result[50];
        //convert number to string for simplicity
        sprintf(result, "%li", card);
        int length = strlen(result);
        //apply staring digits and length of the card number to find the ideal card
        if ((length == 13 || length == 16) && result[0] == '4')
        {
            printf("VISA\n");
        }
        else if (length == 15 && result[0] == '3' && (result[1] == '4' || result[1] == '7'))
        {
            printf("AMEX\n");
        }
        else if (length == 16 && result[0] == '5' && (result[1] == '1' || result[1] == '2' || result[1] == '3' || result[1] == '4'
                 || result[1] == '5'))
        {
            printf("MASTERCARD\n");
        }
        // card is not of above any of above so we take it as invalid
        else
        {
            printf("INVALID\n");
        }
    }
    //if validity formula is flase , card is invalid
    else
    {
        printf("INVALID\n");
    }
}

bool Validity()
{
    //function to check if card number is valid
    card = get_long("Enter the card number: ");
    long int copy = card;
    // add last number and every 2nd number from it
    long int Formula2 = 0;
    //Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together
    int Formula1 = 0;
    //formula implemented in while loop
    while (copy != 0)
    {
        int LastDigit = copy % 10;
        Formula2 += LastDigit;
        copy = copy / 10;
        int Every2ndNo = copy % 10;
        int Mulby2 = Every2ndNo * 2;
        if (Mulby2 < 10)
        {
            Formula1 += Mulby2;
        }
        else
        {
            int otherNo = Mulby2 % 10;
            Mulby2 /= 10;
            Formula1 += otherNo + Mulby2;
        }
        copy = copy / 10;

    }
    // printf("%li %li\n",Formula1,Formula2);
    long int FinalNumber = Formula1 + Formula2;

    if (FinalNumber % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}