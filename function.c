//Author Name- Vinaysingh Pandurang Patil
//Mis-111903124
//Div-2
//Batch-S3

#include <stdio.h>
#include <stdlib.h>
#include "function.h"

// function for calculate  length of word
int codelenth(char* c)
{
    int i = 0;
    while (*(c + i) != '\0')//Travels to end of code word
        i++;
    return i;
}

// function to concatenate the words
void strconcat(char* str, char* pc, char A)//Also used for assign 0 and 1 to path from root to parent node
{
    int i = 0;
    while (*(pc + i) != '\0')//Traverse from parent code to end
    {
        *(str + i) = *(pc + i);
        i++;
    }
    if (A != '2')
    {
        str[i] = A;//either 0 or 1
        str[i + 1] = '\0';
    }
    else
        str[i] = '\0';
}

//function for find fibonacci number
int fibonacci(int n)
{
    if (n <= 1)         //this function used for find maximum length of code
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}
