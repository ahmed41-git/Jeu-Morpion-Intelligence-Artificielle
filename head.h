#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "rotation.h"



typedef struct grid grid;
typedef struct listGrid listGrid;
typedef struct box box;
typedef struct listBox listBox;

struct grid
{
   uint32_t decimalGrid; 
   
   struct listBox *boxes;
   struct grid *nextGrid;
};

struct box
{
    uint8_t pion;
    uint32_t bille;
    struct box *nextBox;
};

struct listGrid
{
    uint32_t taille;
    grid *gridHead;
    grid *gridTail;
};
struct listBox
{
    uint8_t taille;
    box *boxHead;
    box *boxTail;
};

grid *traduction_grid_int(uint8_t tab[][3]);
void traduction_int_grid(grid* grille,uint8_t out[][3]);
grid *initGrid(uint32_t  entier);
box  *initBox(uint8_t pion);
listGrid *initListGrid();
listBox *initListBox();
void verify_grid(uint32_t grille[3][3]);
void affiche(uint8_t tab[][3]);
_Bool compare(uint8_t a,uint8_t b, uint8_t c);
void add_boxes(grid* g);
grid* tourUser(grid* g);
grid* tourOrdinateur(listGrid* liste, grid* grille,uint8_t t[5], uint8_t increment0);
grid* touranonyme(grid* g);
uint8_t choice_pion();
char game();
void parcoursList();
void parcoursBoxes();
void exporter(listGrid *l, FILE *f);
listGrid* import(FILE* f);
listGrid* changement(listGrid *l, listGrid *old);
void morpion();
void initPartie(listGrid *l);
void initPartieAnonyme(listGrid *l);

void add_allumette2(listGrid *list, grid* g2);
void viderListeGrille(listGrid* l);
void viderListebox(grid* g);

grid *initGrid(uint32_t  entier)
{
    grid *g = (grid*) malloc(sizeof(grid));
    if(g==NULL)
        assert(0);
    g->decimalGrid=entier;
    add_boxes(g);
    return g;
}
box  *initBox(uint8_t p)
{
    box *b = (box*) malloc(sizeof(box));
    if(b==NULL)
        assert(0);
    b->pion=p;
    if(p==0)
      b->bille=1;
    else b->bille=0;
    return b;
}
listGrid *initListGrid()
{
    listGrid *lGrid = (listGrid*) malloc(sizeof(listGrid));
    if(lGrid==NULL)
        assert(0);
    lGrid->gridHead=NULL;
    lGrid->gridTail=NULL;
    lGrid->taille=0;
    return lGrid;
}
listBox *initListBox()
{
    listBox *lBox = (listBox*)  malloc(sizeof(listBox));
    if(lBox==NULL)
        assert(0);
    lBox->boxHead=NULL;
    lBox->boxTail=NULL;
    lBox->taille=0;
    return lBox;
}
void add_boxes(grid* g)
{
      
       listBox *lbox= initListBox();
        uint8_t tab[3][3];
    traduction_int_grid(g, tab);
    for (uint8_t i = 0; i < 3; i++)
    {
        for (uint8_t j = 0; j < 3; j++)
        {
           box *b= initBox(tab[i][j]);
           b->nextBox=NULL;
           if(lbox->taille>0)
                lbox->boxTail->nextBox=b;
            else
                lbox->boxHead=b;
            lbox->boxTail=b;
            lbox->taille+=1;
        }
    }
       g->boxes=lbox;
   
}
grid *traduction_grid_int(uint8_t tab[][3])
{
    grid *g;
    uint32_t somme=0;
    int8_t a=2,b=2;
    for(uint8_t i=0; i<9; i++)
    { 
        somme = somme + tab[a][b]*pow(3,i);
        b--;
        if(b==-1)
        { a--; b=2;}
    }
    g = initGrid(somme);
    return g;
}
void traduction_int_grid(grid*  grille, uint8_t tab[][3])
{
    uint32_t entier = grille->decimalGrid;
    for (int i = 0; i < 3; i++)
    {
        int j = 0;
    tab[i][j]=0;tab[i][j+1]=0,tab[i][j+2]=0;
    }
    uint32_t nbre;
    int8_t a=2,b=2;
    do
    {
        nbre = entier%3;
        tab[a][b]=nbre;
        b--;
        if(b==-1)
        { a--; b=2;}
        entier=entier/3;
    }while(entier>0);
}
//Fonction qui permet de jouer dans une case

grid* jouerPartie(grid* g, uint8_t cas,uint8_t pion)
{
    uint8_t tableau[3][3];
    traduction_int_grid(g,tableau);
    uint8_t i=(cas-1)/3,j = (cas-1)%3;
    tableau[i][j]= pion;
   grid* new1 = traduction_grid_int(tableau);
       
     return new1;
}

void affiche(uint8_t tab[][3])
{
    for (int i = 0; i < 3; i++)
    {
        int j = 0;
    printf("*************\n");
    printf("* %d * %d * %d *\n",tab[i][j],tab[i][j+1],tab[i][j+2]);
    }
    printf("*************\n");
}
_Bool compare(uint8_t a,uint8_t b, uint8_t c)
{
    return ((a==b && a==c) && a!=0);
}
//Fonction qui verifie si une grille est celui d'une victoire
_Bool grilleGagnate(grid* grille)
{
    uint8_t tableau[3][3]; 
    traduction_int_grid(grille,tableau);
    _Bool over1 = 0, over2=0;
    uint8_t i=0;
    while((over1==0 && over2==0) && i<3){
      uint8_t j=0;
      over1=compare(tableau[i][j],tableau[i][j+1],tableau[i][j+2]);
      over2=compare(tableau[j][i],tableau[j+1][i],tableau[j+2][i]);
      i++;
    }
    
    if(over1==0 && over2==0)
    {
        _Bool diagonal1=compare(tableau[0][0],tableau[1][1],tableau[2][2]);
        _Bool diagonal2=compare(tableau[0][2],tableau[1][1],tableau[2][0]);
        if(diagonal1!=0 || diagonal2!=0)
        return 1;
        else return 0;
    }
    else return 1;
}
//Fonction qui verifie si une grille est celui d'un match nul
_Bool grilleNulle( grid* grille)
{
    uint8_t tableau[3][3]; 
    traduction_int_grid(grille,tableau);
    for (uint8_t i = 0; i < 3; i++)
    {
        uint8_t j=0;
        if(tableau[i][j]== 0 || tableau[i][j+1] == 0 || tableau[i][j+2] == 0)
        return 0;
    }
    return 1;
}
//Fonction qui verifie si une partie est finie
_Bool finPartie(grid* grille)
{
    if(grilleNulle(grille)!=0 || grilleGagnate(grille)!=0)
    return 0;
    else return 1;
}
//Ajouter une grille dans une liste
void add_allumette(listGrid *list, uint32_t decimal)
{
        grid* g = initGrid(decimal);
        g->nextGrid=NULL;
        if(list->taille>0)
            list->gridTail->nextGrid=g;
        else
            list->gridHead=g;
        list->gridTail=g;
        list->taille+=1;
   
}
void add_allumette2(listGrid *list, grid* g2)
{
        grid* g = g2; //initGrid(g2->decimalGrid);
        g->nextGrid=NULL;
        if(list->taille>0)
            list->gridTail->nextGrid=g;
        else
            list->gridHead=g;
        list->gridTail=g;
        list->taille+=1;
}
void add_bille(listGrid *l, listGrid *l2, uint8_t *t, uint8_t bonus)
{
        uint8_t i=0;
    grid* go= l2->gridHead;
    while( go!=NULL)
    {
        grid* gl= l->gridHead;
        while(gl!=NULL){
            if(gl->decimalGrid==go->decimalGrid)
            {
                box* b= gl->boxes->boxHead;
                uint8_t j;
                for (j = 1; j <t[i]; j++)
                {
                    b=b->nextBox;
                }
                b->bille+=bonus;
                i+=1;
            }
        gl=gl->nextGrid;
        }
        go=go->nextGrid;
    }
}
void delete_bille(listGrid *l, listGrid *l2, uint8_t *t, uint8_t bonus)
{
    grid* go= l2->gridHead;
        uint8_t i=0;
    while( go!=NULL)
    {
        grid* gl= l->gridHead;
        while(gl!=NULL){
            if(gl->decimalGrid==go->decimalGrid)
            {
                box* b= gl->boxes->boxHead;
                uint8_t j;
                for (j = 1; j <t[i]; j++)
                {
                    b=b->nextBox;
                }
                 if(b->bille>0)
                b->bille-=bonus;
                else b->bille=0;
                i+=1;
            }
        gl=gl->nextGrid;
        }
        go=go->nextGrid;
    }
}
char game()
{
    char chaine[3];
    uint8_t c;
    printf("\t******Bienvenue dans le GAME******\n");
    do
    {
            printf(" \t Cliquez sur 1 pour jouer avec la machine 2 pour laisser la machine jouer\n");
            fgets(chaine, 3, stdin);
            purger(chaine);    
            c=atoi(chaine) ;  
    } while( c<1 || c>2);
    return chaine[0];
}

uint8_t choice_pion(grid* g)
{
    uint8_t *t = (uint8_t*) malloc(sizeof(uint8_t));
    /*uint8_t tab[3][3];
    traduction_int_grid(g,tab);*/
    box* b= g->boxes->boxHead;
    uint8_t cas = 1;
    uint32_t i=0;
    while (b!=NULL)
    {
                uint32_t j = 0;
        while (j < b->bille && b->pion==0)
        {
            t[i]=cas;
            j++;
            i++;
            t = (uint8_t*) realloc(t,i*sizeof(uint8_t));
        }
        cas+=1;
        b=b->nextBox;
    }
    
    
        uint8_t ran = rand()%i;
        ran=t[ran];        
        free(t);
         return ran;
}
/*void parcoursList(listGrid* liste)
{
    if (liste!=NULL)
    {
        grid* g= liste->gridHead;
        uint32_t i=1;
        while (g!=NULL)
        {
            uint8_t t[3][3];
            traduction_int_grid(g,t);
            printf("grille n' %d ==> %d \n", i, g->decimalGrid);
            affiche(t);
            i+=1;
            parcoursBoxes(g->boxes);
            g=g->nextGrid;
        }
    }
    else printf("la liste est vide");
    
    
     
}*/
/*void parcoursBoxes(listBox* liste)
{
    box* b= liste->boxHead;
    uint32_t i=1;
    while (b!=NULL)
    {
        printf("pion n' %d ==> %d et nbre bille == %d\n", i, b->pion,  b->bille);
        i+=1;
        b=b->nextBox;
    }
     
}*/


void viderListeGrille(listGrid* l)
{
    while (l->gridHead!=NULL)
    {
        grid *g=l->gridHead;
        l->gridHead=l->gridHead->nextGrid;
        viderListebox(g);
        free(g);
        l->taille-=1;
        if(l->taille==0)
            l->gridTail=NULL;
    }
    free(l);
    
}
void viderListebox(grid* g)
{
    while (g->boxes->boxHead!=NULL)
    {
        box *b=g->boxes->boxHead;
        g->boxes->boxHead=g->boxes->boxHead->nextBox;
        free(b);
        g->boxes->taille-=1;
        if(g->boxes->taille==0)
            g->boxes->boxTail=NULL;
    }
    free(g->boxes);  
}