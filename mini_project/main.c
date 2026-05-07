#include <stdio.h>
#include <stdlib.h>
#include <time.h> /* la librairi pour le nombre aléatoire */

/*
    - L'ordinateur tire au sort un nombre entre 1 et 100.

    - Il vous demande de deviner le nombre. Vous entrez donc un nombre entre 1 et 100.

    - L'ordinateur compare le nombre que vous avez entré avec le nombre « mystère » qu'il a tiré au sort. Il vous dit si le
      nombre mystère est supérieur ou inférieur à celui que vous avez entré.

    - Puis l'ordinateur vous redemande le nombre.

    - … Et il vous indique si le nombre mystère est supérieur ou inférieur.
    
    - Et ainsi de suite, jusqu'à ce que vous trouviez le nombbre mystère.
*/

/* les variables globales */
int a;

/* un commentaire */
int MIN = 1; // le maximum 
int MAX = 100; // le minimum

int main (void){

    /* le nombre mystère générer */

    srand(time(NULL));
    int nombreMystere = (rand() % (MAX - MIN + 1)) + MIN;  
    
    /* au tour des utilisateurs */
    printf ("propose un nombre : ");
    scanf("%d", &a); 

    /* les 3 cas */

    do
    {

      if (a == nombreMystere){ /* quand tu trouve le nombre mystère */ 
      }
      else if (a > nombreMystere){ /* quand ton nombre est au-dessus */
        printf ("c'est moins !\n");
        printf ("propose un nombre : ");
        scanf("%d", &a); 
      }
      else /* quand ton nombre est en-dessous */
      { printf ("c'est plus !\n");
        printf ("propose un nombre : ");
        scanf("%d", &a); 
      }
    }while (a != nombreMystere); /* la boucle tourne du moment où le nombre mystère n'as pas été trouvé */

    printf("bravo c'est le nombre !!!\n"); 

    return 0;
}