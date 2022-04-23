#include <stdint.h>


#define CONTINUE 0
#define FINISHED 1


typedef enum transformation{ROT_90, MIROIR_VERT} transformation;
void appliquer_transformation_base(uint8_t grille[3][3], transformation t);



void appliquer_transformation_base(uint8_t grille[3][3], transformation tr)
{
    uint8_t t;
    switch(tr)
    {
       

        case(ROT_90):
            t = grille[0][0];
            grille[0][0] = grille[2][0];
            grille[2][0]=grille[2][2];
            grille[2][2]=grille[0][2];
            grille[0][2]=t;

            t = grille[0][1];
            grille[0][1] = grille[1][0];
            grille[1][0]=grille[2][1];
            grille[2][1]=grille[1][2];
            grille[1][2]=t;
            break;


        case(MIROIR_VERT):
            t = grille[0][0];
            grille[0][0] = grille[0][2];
            grille[0][2] = t;

            t = grille[1][0];
            grille[1][0] = grille[1][2];
            grille[1][2] = t;

            t = grille[2][0];
            grille[2][0] = grille[2][2];
            grille[2][2] = t;
            break;

    }
}
void purger(char *chaine)
{
    char *p = strchr(chaine,'\n');
    if(p) *p=0;
    else
    {
        int c;
        while ((c=getchar())!='\n' && c!=EOF)
        {}
    }
}