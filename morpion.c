#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "head.h"
#include <unistd.h>



/*void afficherTransformation(grid* g)
{
             uint8_t tableau[3][3]; 
             uint8_t tableau1[3][3]; 

            transformation tr;
            traduction_int_grid(g,tableau);
                printf("%d\n", g->decimalGrid);
            affiche(tableau);
             for (uint8_t i = 0; i < 8; i++)
            {
                 tr=0;
                if(i==3)
                {
                appliquer_transformation_base(tableau,tr);
                    continue;
                }
                if(i==4) tr=1;
                appliquer_transformation_base(tableau,tr);
                grid* g = traduction_grid_int(tableau);
                printf("%d\n", g->decimalGrid);
                traduction_int_grid(g,tableau1); 
                affiche(tableau1);
            }
}*/
//Fonction qui initialise chaque partie entre machine-user
void initPartie(listGrid *l)
{
   listGrid *lpartie = initListGrid();
    grid* g = initGrid(0);
    uint8_t bigin=1, gagnant;
    if(bigin == 0)
    {
    }
    else
    {
     uint8_t position[5]={0},increment=0;
     while (finPartie(g)!=0)
      {
          g=tourUser(g);
            gagnant=0;
          if(finPartie(g)==0)
          break;
        add_allumette2(lpartie,g);
        g=tourOrdinateur(l,g,position,increment++);
        gagnant=1;
      } 
      if(grilleGagnate(g)==1)
      {
          if (gagnant==1)
          {
          printf("****************la partie est terminee avec une victoire de la machine****************\n");
          add_bille(l,lpartie,position,3);
              
          }
          else
          {
            printf("****************la partie est terminee avec ta victoire****************\n");
            delete_bille(l,lpartie,position,1);
          }
      }
       else
      {
          printf("****************la partie est terminee avec un match nulle****************\n");
          add_bille(l,lpartie,position,1);

      }
       FILE *f=fopen("grille.txt", "w");
         exporter(l,f);
         fclose(f);
      viderListeGrille(lpartie);
        
    }
    printf("*****************FIN DE LA PARTIE********************* \n");
}
//Fonction qui initialise chaque partie entre machine-machine
void initPartieAnonyme(listGrid *l)
{
   listGrid *lpartie = initListGrid();
    grid* g = initGrid(0);
    uint8_t gagnant;
     uint8_t position[5]={0},increment=0;
     while (finPartie(g)!=0)
      {
          g=touranonyme(g);
            gagnant=0;
          if(finPartie(g)==0)
          break;
        add_allumette2(lpartie,g);
        g=tourOrdinateur(l,g,position,increment++);
        gagnant=1;
      } 
      if(grilleGagnate(g)==1)
      {
          if (gagnant==1)
          {
          printf("****************la partie est terminee avec une victoire de la machine****************\n");
          add_bille(l,lpartie,position,3);
              
          }
          else
          {
            printf("****************la partie est terminee avec la victoire du joueur anonyme****************\n");
            delete_bille(l,lpartie,position,1);
          }
      }
       else
      {
          printf("****************la partie est terminee avec un match nulle****************\n");
          add_bille(l,lpartie,position,1);

      }
       FILE *f=fopen("grille.txt", "w");
         exporter(l,f);
         fclose(f);
      viderListeGrille(lpartie);
    printf("*****************FIN DE LA PARTIE********************* \n");
}
grid* tourUser(grid* g)
{
    uint8_t tab[3][3];
    uint8_t i,j,saisie;
    traduction_int_grid(g,tab);
    affiche(tab);
    char *pc;
    do
    {
    char chaine[3];
            printf("\t **************Choisissez un numero de case vide**************\n");
            fgets(chaine, 3, stdin);
            purger(chaine);
            saisie = strtoul(chaine, &pc, 10);
        i=(saisie-1)/3,j = (saisie-1)%3;

    } while(tab[i][j]!=0 || saisie >9 || saisie==0);
    grid* gUser = jouerPartie(g,saisie,2);
    return gUser;
}
grid* touranonyme(grid* g)
{
      uint8_t *t = (uint8_t*) malloc(sizeof(uint8_t));
    uint8_t tab[3][3];
    traduction_int_grid(g,tab);
    affiche(tab);
    box* b= g->boxes->boxHead;
    uint8_t cas = 1;
    uint32_t i=0;
    while (b!=NULL)
    {
        if (b->pion==0)
        {
            t[i]=cas;
            i++;
            t = (uint8_t*) realloc(t,i*sizeof(uint8_t));
        }
        cas+=1;
        b=b->nextBox;
    }
    uint8_t ran = rand()%i;
        ran=t[ran]; 
        free(t);
    grid* gUser = jouerPartie(g,ran,2);
    return gUser;
}

grid* tourOrdinateur(listGrid* liste, grid* grille,uint8_t t[5], uint8_t increment)
{
    uint8_t tableau[3][3]; 
    traduction_int_grid(grille,tableau);
    _Bool trouve=0;
    grid* grilleListe = liste->gridHead;
    while(grilleListe!= NULL && trouve==0)
    {  
        if(grilleListe->decimalGrid!=grille->decimalGrid)
        {
            transformation tr;
             for (uint8_t i = 0; i < 9; i++)
            {
                tr=0;
                if(i==3)
                {
                appliquer_transformation_base(tableau,tr);
                    continue;
                }
                if(i==4) tr=1;
                appliquer_transformation_base(tableau,tr);
                grid* g = traduction_grid_int(tableau);
                if(grilleListe->decimalGrid==g->decimalGrid)
                {
                    trouve=1;
                    break;
                }
            
            }
            if(trouve==1)   
                 break;
        }
        else{
            trouve=1;
            break;
        }
        grilleListe=grilleListe->nextGrid;
    }
        
    grid* newGrid;
    uint8_t choice;
    if(grilleListe==NULL)
    {
       
        choice = choice_pion(grille);
        newGrid = jouerPartie (grille, choice,1);
        add_allumette2(liste, grille);
    }
    else{
        choice = choice_pion(grilleListe);
        newGrid = jouerPartie (grilleListe, choice,1);
    }
    printf("************ j'ai joue a la case : %d ***************\n",choice);
    t[increment]=choice;
    return newGrid;
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
   morpion();
}


void exporter(listGrid *l, FILE* f)
{
    grid* g= l->gridHead;
    while (g!=NULL)
    {
    fprintf(f, "%u ", g->decimalGrid);
        box* b= g->boxes->boxHead;
        while (b!=NULL)
        {
            fprintf(f, " %u", b->bille);
            b=b->nextBox;
        }
            fprintf(f, "\n");
        g=g->nextGrid;
    }
}

listGrid* import(FILE* f)
{
    listGrid *l = initListGrid();
        uint32_t bo[10];
     while(fscanf(f, "%d  %d %d %d %d %d %d %d %d %d\n",&bo[0],
     &bo[1],&bo[2],&bo[3],&bo[4],&bo[5],&bo[6],&bo[7],&bo[8],&bo[9])==10)
     {
        add_allumette(l,bo[0]);
        box* b= l->gridTail->boxes->boxHead;
        uint8_t i=1;
        while (b!=NULL)
        {
            b->bille=bo[i];
            i++;
            b=b->nextBox;
        }
     }
     return l;
}


void morpion()
{
    
    char choix = game();
    if(choix=='1')
    {
        printf("jeu machine - user\n");
       char chaine[3];
        uint8_t c;
        do
        {
            listGrid* listeFichier;
             FILE *f=fopen("grille.txt", "r+");
                if(f!=NULL)
                {
                listeFichier=import(f);
                }
             fclose(f);
              initPartie(listeFichier);
                printf(" \t Cliquez sur 1 pour CONTINUER autre pour QUITTER\n");
                fgets(chaine, 3, stdin);
                purger(chaine);    
                c=atoi(chaine) ; 
      viderListeGrille(listeFichier);

        } while(c==1);
            
        printf("**************Merci A BIENTOT bientot\n");
        
    }
    else
    {
        printf("jeu machine - machine\n");
            char chaine[3];
    uint32_t c;
         do
            {
                printf(" *******************ENTRER LE NOMBRE DE PARTIES\n");
                fgets(chaine, 3, stdin);
                purger(chaine);    
                c=atoi(chaine) ;  
              } while( c<0);
      for (uint32_t i = 0; i < 5; i++)
      {
           listGrid* listeFichier ;
             FILE *f=fopen("grille.txt", "r+");
                if(f!=NULL)
                {
                listeFichier=import(f);
                }
                fclose(f);
              initPartieAnonyme(listeFichier);
                viderListeGrille(listeFichier);

             // sleep(5);
      }
      
    }
}