#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

//functions prototype:-
int codelenth(char* c);
void strconcat(char* str, char* pc, char A);
int fibonacci(int n);

//huffman code array
struct huffcode     //this array used for sorting pixel values and probabilities
{
    int pix;
    int AL; //array location for only sorted
    float freq;
};

#endif // FUNCTION_H_INCLUDED
