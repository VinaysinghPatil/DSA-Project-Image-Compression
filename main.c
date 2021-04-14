//Author Name- Vinaysingh Pandurang Patil
//Mis-111903124
//Div-2
//Batch-S3

#include <stdio.h>
#include <stdlib.h>
#include "function.h"

int main()
{
    printf("***** DSA-PROJECT *****\n\n");
    printf("#NAME-VINAYSINGH PANDURANG PATIL\n");
    printf("#MIS- 111903124 \n");
    printf("#Div-2\n");
    printf("#BATCH-S3\n");
    printf("************************************************\n");
    printf("\n");

//Take BMP file as input and read it
    int i, j;
    char filename[] = "VINU.bmp";
    int data = 0, offset, bpp = 0 , column,row;
    long bs = 0, bd = 0;
    int** image;
    int t= 0;

    // Reading the BMP File
    FILE* image_file;

    image_file = fopen(filename, "rb");
    if (image_file == NULL)
    {
        printf("ERRORS IN OPEN FILE");
        exit(1);
    }
    else
    {
        offset = 0;
        offset = 2;//offset 2 means it contains size of BMP file
                    //size = 4 bytes

        fseek(image_file, offset, SEEK_SET);
        fread(&bs, 4, 1, image_file);// get size of BMP file

        offset = 10;//starting address where image data/pixel array starts
        fseek(image_file, offset, SEEK_SET);
        // Bitmap data offset
        fread(&bd, 4, 1, image_file);

        //For get height and width of image
        //offset= 18 for width of image
        //offset= 22 for height of image
        fseek(image_file, 18, SEEK_SET);
        fread(&column, 4, 1, image_file);
        fread(&row, 4, 1, image_file);

        // Number of bits per pixel
        fseek(image_file, 2, SEEK_CUR);//fseek function set file position SEEK_CUR to offset 2

        fread(&bpp, 2, 1, image_file);//fread 1 item from image file  and stored in bpp memory in 2 bytes offset

        fseek(image_file, bd, SEEK_SET);// Set offset to start of pixel data
                                        //here actual pixel data start

        // Creating Image array
        image = (int**)malloc(row * sizeof(int*));

        for (i = 0; i < row; i++)
        {
            image[i] = (int*)malloc(column * sizeof(int));
        }

        // no. of bytes in Image pixel
        int numbytes = (bs - bd) / 3; //for RED GREEN BLUE 3 colour

        // Reading BMP File into Image Array
        for (i = 0; i < row; i++)
        {
            for (j = 0; j < column; j++)
            {
                fread(&t, 3, 1, image_file);

                //for 24-bit BMP Image
                t = t & 0x0000FF;   //0000FF == 0 0 255 => Red pixel //we can use Blue or Green pixel
                image[i][j] = t; //pixel image
            }
        }
    }

// create  histogram of pixel intensity value
    int hist[256];
    for (i = 0; i < 256; i++)//Initialize as 0
        hist[i] = 0;
    for (i = 0; i < row; i++)
        for (j = 0; j < column; j++)
            hist[image[i][j]] += 1;

// Find out no. of non zero pixel intensity value
    int nodes = 0;
    for (i = 0; i < 256; i++)
        if (hist[i] != 0)
            nodes += 1;

//minimum probability of pixel intensity value
    float p = 1.0, m;
    for (i = 0; i < 256; i++)// pixel  intensity value from 0 to 255
    {
        m = (hist[i] / (float)(row  * column));//Total pixels = row * column
        if (m > 0 && m <= p)
            p = m;
    }

// Calculate max length of code word
    i = 0;
    while ((1 / p) > fibonacci(i))
        i++;
    int maxlenth = i - 3;


// create 2 Structures arrays => probability of pixel value array and huffman code array
    //Define structures
    struct pixfreq
    {
        int pix, lAL, rAL;
        float freq;
        struct pixfreq *left, *right;
        char code[maxlenth];
    };

    // Declaring structures
    struct pixfreq* pix_freq;
    struct huffcode* huffcodes;//For sorting

    int totalnodes = 2 * nodes - 1;
    pix_freq = (struct pixfreq*)malloc(sizeof(struct pixfreq) * totalnodes);
    huffcodes = (struct huffcode*)malloc(sizeof(struct huffcode) * nodes);

    // Initialize this 2 structures arrays
    j = 0;
    int totalpixel = row * column;
    float tp;
    for (i = 0; i < 256; i++)
    {
        if (hist[i] != 0)
        {

            // pixel intensity value
            huffcodes[j].pix = i;
            pix_freq[j].pix = i;

            //position of the node in pixel frequency array
            huffcodes[j].AL = j;

            // probability of pixel value
            tp = (float)hist[i] / (float)totalpixel;
            pix_freq[j].freq = tp;
            huffcodes[j].freq = tp;

            //declare child of leaf node as NULL
            pix_freq[j].left = NULL;
            pix_freq[j].right = NULL;

            // initializing code word as end of line
            pix_freq[j].code[0] = '\0';
            j++;
        }
    }


//sorting histogram accord to  probability of pixel values
    struct huffcode TH;
    for (i = 0; i < nodes; i++)
    {
        for (j = i + 1; j < nodes; j++)
        {
            if (huffcodes[i].freq < huffcodes[j].freq)//sorted huffcode array for creating huffman tree
            {
                TH = huffcodes[i];
                huffcodes[i] = huffcodes[j];//swapping
                huffcodes[j] = TH;
            }
        }
    }


//create Huffman tree

    float SP; //sum of probability
    int sumpix; //sum of pixel value
    int n = 0, k = 0;
    int nn = nodes; //next node

    //total number of nodes in Huffman Tree= 2*nodes-1
    while (n < nodes - 1)
    {
        // Add lowest two probabilities and pixel values
        SP = huffcodes[nodes - n - 1].freq + huffcodes[nodes - n - 2].freq;//add minimum probability at end
        sumpix = huffcodes[nodes - n - 1].pix + huffcodes[nodes - n - 2].pix;//add pixel intensity values

        // Append to the pixel frequency Array
        pix_freq[nn].pix = sumpix;
        pix_freq[nn].freq = SP;
        pix_freq[nn].left = &pix_freq[huffcodes[nodes - n - 2].AL];//left position
        pix_freq[nn].right = &pix_freq[huffcodes[nodes - n - 1].AL];//right position
        pix_freq[nn].code[0] = '\0';//sorted array initialize \0

        //now sorting huffcodes array after append
        i = 0;
        //new position of combined node
        while (SP <= huffcodes[i].freq)
              i++;

        // Inserting new node in  huffcodes array
        for (k = nodes; k >= 0; k--)
        {
            if (k == i)
            {
                huffcodes[k].pix = sumpix;;//insert sum of pixel intensity value
                huffcodes[k].freq = SP; ;//insert probabilities
                huffcodes[k].AL = nn; //next node position
            }
            else if (k > i)

                // Shift nodes by 1 below
                huffcodes[k] = huffcodes[k - 1]; //For insert new node at new position k

        }
        n = n + 1;
        nn = nn + 1;
    }

//backtrack from root to leaf nodes for assign path
    char left = '0';
    char right = '1';
    int index;
    for (i = totalnodes - 1; i >= nodes; i--)
    {
        //For left child
        if (pix_freq[i].left != NULL)
            strconcat(pix_freq[i].left->code, pix_freq[i].code, left); //use string assign function for
                                                                        //assign path as 0 or 1
        //For right child
        if (pix_freq[i].right != NULL)
            strconcat(pix_freq[i].right->code, pix_freq[i].code, right);
    }


//ENCODE IMAGE:-

    int pv; //pixel value
    int l;
    //convert Huffman encoded Image into text file
    FILE* IH = fopen("VINU.txt", "wb");
    for (i = 0; i < row; i++)
        for (j = 0; j < column; j++)
        {
            pv = image[i][j];
            for (l = 0; l < nodes; l++)
                if (pv == pix_freq[l].pix)
                    fprintf(IH, "%s", pix_freq[l].code);
        }

//Display Huffman encoded codes
    printf(" ## HUFFMAN CODES ##\n\n");
    printf("PIXELS VALUES ->        CODES\n\n");
    for (i = 0; i < nodes; i++) {
        if (snprintf(NULL, 0, "%d", pix_freq[i].pix) == 2)
            printf("    %d         -> %s\n", pix_freq[i].pix, pix_freq[i].code);
        else
            printf("    %d         -> %s\n", pix_freq[i].pix, pix_freq[i].code);
    }

// Calculate Average number of bits
    float ABN = 0;
    for (i = 0; i < nodes; i++)
        ABN += pix_freq[i].freq * codelenth(pix_freq[i].code);
    printf("Average number of bits:: %f", ABN);




}

