/* groupe composé de :

    - Christopher
    - Idris
    - Karyl
*/


#include <iostream>
#include <stdio.h>
#include <string>
#include "OutilsCreationImage.h"
#include <math.h>

using namespace std;
/*________________________________________________________________________ les fonctions de base _______________________________________________________________*/

/*_____________________________pour la sphère_____________________________________________*/


double deg_vers_rad(double deg)// pour le contournement de la sphère du degré au radiant 
{
    const double PI = 3.14159265358979323846;// représentation mathématique de pi

    return deg * PI / 180.0;
}

double distanceEuclidienne(int i, int j, int x, int y)// pour la tache sur la sphère 
{
    return sqrt((i - y) * (i - y) + (j - x) * (j - x));
}


bool cercle_Ellipse(const double x, const double y, const double x0, const double y0, const double a, const double b, const double m1, const double coeffm, const double m2)
{
     // séléctionner les points appartenant à l'ellipse pour le disque 
    double dx = x - x0;
    double dy = y - y0;

    double f = m1 * dx * dx + 2 * coeffm * dx * dy + m2 * dy * dy - 1;

    // Sur l'ellipse (f(x,y) = 0) et dans l'ellipse (f(x,y) > 0)
    return f <= 0;
}

bool cercle_Disque(const double x, const double y, const double x0, const double y0, const double r)
{
    // séléctionner les points appartenant au disque pour la sphère
    double dx = x - x0;
    double dy = y - y0;

    double d = dx * dx + dy * dy; // distance au carré entre le point et le centre du disque

    return d <= r * r;
}



double distanceCarree(double x1, double y1, double x2, double y2)
{
	double x = x1 - x2;
	double y = y1 - y2;

	double d2 = x*x + y*y;

	return d2; 
} 
/* ____________________________fonction pour les anneaux_________________________________________*/

/* cette fonction  indiquera si un point quelconque est à l'intérieur d'une ellipse  x : xollaire y : y collaire ac et bc 
   les rayons de l'ellipse */

bool Interieur_ellipse (const double x, const double y, const double x0, const double y0, const double ac, const double bc) 
{
	double dx = x - x0;
	double dy = y - y0;

	dx /= ac ;
	dy /= bc ;

	double r = dx * dx  + 1 * dx * dy +  dy * dy  -0.5;

	return r <= 0;
}

/* cette fonction indique si le point est situé à l'intérieur de l'anneau elliptique */

bool Interieur_Anneau (const double x, const double y, const double x0, const double y0, const double ac1, const double bc1, const double ac2, const double bc2)
{
	return Interieur_ellipse( x, y, x0, y0, ac2, bc2) && ! Interieur_ellipse( x, y, x0, y0, ac1, bc1);
}

/* cette fonction indique si la fonction un point (x, y) est à l'intérieur d'un demi-anneau elliptique à gauche */

bool Interieur_demiAnneauGauche (const double x, const double y, const double x0, const double y0, const double ac1, const double bc1, const double ac2, const double bc2)
{
 
	return Interieur_Anneau( x, y, x0, y0, ac1,bc1, ac2, bc2) && x <= x0;
}


/* cette fonction indique si la fonction un point (x, y) est à l'intérieur d'un demi-anneau elliptique à droite */


bool Interieur_demiAnneauDroite (const double x, const double y, const double x0, const double y0, const double ac1, const double bc1, const double ac2, const double bc2)
{

	return Interieur_Anneau( x, y, x0, y0, ac1,bc1, ac2, bc2) && x >= x0;
}

/*______________________FONCTION POUR LE DÉGRADER DE COULEUR __________________________________*/


uint32_t AlphaBlending(uint32_t couleur1, uint32_t couleur2,uint8_t alpha)// pour les dégrader de couleur 
{
    // Séparation des composantes RGBA de chaque couleur
   uint8_t rouge1 = (couleur1 >> 24) & 0xFF;
   uint8_t vert1 = (couleur1 >> 16) & 0xFF;
   uint8_t bleu1 = (couleur1 >> 8) & 0xFF;
   uint8_t alpha1 = couleur1 & 0xFF;

   uint8_t rouge2 = (couleur2 >> 24) & 0xFF;
   uint8_t vert2 = (couleur2 >> 16) & 0xFF;
   uint8_t bleu2 = (couleur2 >> 8) & 0xFF;
   uint8_t alpha2 = couleur2 & 0xFF;

    // Mélange des composantes RGBA
   uint8_t rougeF = rouge1 * (255 - alpha) / 255 + rouge2 * alpha / 255;
   uint8_t vertF = vert1 * (255 - alpha) / 255 + vert2 * alpha / 255;
   uint8_t bleuF = bleu1 * (255 - alpha) / 255 + bleu2 * alpha / 255;
   uint8_t alphaF = alpha1 * (255 - alpha) / 255 + alpha2 * alpha / 255;

    // Assemblage de la couleur mélangée
    uint32_t couleurF = (rougeF << 24) | (vertF << 16) | (bleuF << 8) | alphaF;
    return couleurF;
}



int main(void)
{
	/*_______________________________________________________________________IINIALISATION DU FICHIER CONTENANT L'IMAGE__________________________________________________________________________________*/

	string chemin = "images_Crees";
	string nomFichierImage = chemin + "/" + "planète_saturnekaryl.bmp";

	printf("%s\n", nomFichierImage.c_str());
	printf("Création de la planète saturne.\n");

    /*______________________________________________________________________PROPORTION-IMAGE__________________________________________________________________________________*/

	const int LARGEUR = 400;		// largeur de l'image en pixels
	const int HAUTEUR = 320;		// hauteur de l'image en pixels

	int m = HAUTEUR-1;		// n° de la dernière ligne
	int n = LARGEUR-1;		// n° de la dernière colonne
	/*_____________________________________________________________________GESTION-COULEUR____________________________________________________________________________________*/

	uint32_t matricePixels[HAUTEUR][LARGEUR];  // Toutes les couleurs néssaicaire pour l'image 

	uint32_t noir = 0x0000007F;		
											   
	uint32_t orange = 0xFF6600FF;

	uint32_t blanc  = 0xFFFFFFFF;

    uint32_t transparent = 0x00000000;

    uint32_t Jaune =  0xFFFF99FF;

    uint32_t couleurSolCaliente = 0xFFB200FF;

    uint32_t couleurBleuNuit = 0x19197000; 	

    uint32_t Orombre = 0xA18200FF;
		
	const uint32_t couleurs[8] = {blanc,noir, orange,transparent,Jaune,couleurSolCaliente,couleurBleuNuit,Orombre};
    //                              0     1     2       3           4           5               6           7

	/*____________________________________________________________________DONNÉES-DES-OBJETS__________________________________________________________________________________*/

    /*_____________________________ CONCERNANT LA SPHÈRE__________________________________________*/

	double iCs = 0.5*m;			// les coordonnées de "iCs", "jCs" du centre "Cs" de saturne 
	double jCs = 0.5*n;			

	double RayonSaturn = 0.260*m;    // rayon de la planète qui determine la taille de la planète!
	double RayonSaturn2 = RayonSaturn*RayonSaturn; // rayon au carré !

	const double PI = 3.14159265358979323846;  //constante mathématiques pi


	//variable planète
	
	
	double iCT = iCs+100 ;
	double jCT = jCs-200;		// le centre de l'image a pour coordonnées (aCT, bCT)

	double K = 1.0e-4;		// ce coefficient déterminera la taille de notre tache, plus K est grand plus la tache est petite (K = (1.0)*10^-3)

    double x0 = iCs + 255;        // centre de l'ellipse en x
    double y0 = jCs - 195;          // centre de l'ellipse en y
    double a = 155;                   // rayon en x
    double b = 40;                    // rayon en y
    double theta = deg_vers_rad(-25); // angle en radians 

    // pour la sphère

    double m1 = (cos(theta) * cos(theta)) / (a * a) + (sin(theta) * sin(theta)) / (b * b);
    double coeffm = cos(theta) * sin(theta) * ((1 / (a * a)) - (1 / (b * b)));
    double m2 = (sin(theta) * sin(theta)) / (a * a) + (cos(theta) * cos(theta)) / (b * b);

    /*_____________________________ CONCERNANT LES ANNEAUX__________________________________________*/

    int MARGE1 = 7; // marge hoizontale entre les deux anneaux  en pixels 
    int MARGE2 = 50; // marge extèrieure hoizontale en pixels 

    

    double rayonSaturne = LARGEUR*0.2; 
    double ratioHorizontal1 = 1.6; // Hauteur des anneaux (les épaisseurs) 

    double rayon1Horizontal = rayonSaturne*ratioHorizontal1;
    double rayon4Horizontal = LARGEUR/2-MARGE2;

    double ratioHorizontal4 = rayon4Horizontal/rayon1Horizontal;

    double rayon1Vertical = rayonSaturne*0.35; 
    double rayon4Vertical = rayon1Vertical*ratioHorizontal4;

    double rayonMilieuHorizontal = 0.5*(rayon1Horizontal+rayon4Horizontal); 

    double rayon2Horizontal = rayonMilieuHorizontal-MARGE1;
    double rayon3Horizontal = rayonMilieuHorizontal+MARGE1;

    double ratioHorizontal2 = rayon2Horizontal/rayon1Horizontal;
    double ratioHorizontal3 = rayon3Horizontal/rayon1Horizontal;

    double rayon2Vertical = ratioHorizontal2*rayon1Vertical;
    double rayon3Vertical = ratioHorizontal3*rayon1Vertical;


    /*____________________________________________________________________IMPORT_IMAGE_DE_FOND________________________________________________________________________________*/
    int i, j; // indices pour parcourir les cases de la matrice
              // i parcourt les lignes et j les colonnes
	/*____________________________________________________________________CRÉATION-DE-SATURNE-LA-PLANÈTE_________________________________________________________________________________*/
    
    for (i = 0; i <= m; ++i)
    {
        for (j = 0; j <= n; ++j)
        {
            int k;

            //--------------- détermination de la couleur en fonction de (i,j) ---------------------------
            if (i < m / 3)
            {
                k = 3; // ciel
            }
            

            matricePixels[i][j] = couleurs[k];
        }
    }

    /*_______________________________________________________DESSIN DE LA DEMI-ANNEAU DE L'ARRIÈRE DU PLAN_______________________________________*/
    

    for ( i = 0 ; i <= m ; ++i )        // remplissage de la matrice de pixels
    for ( j = 0 ; j <= n ; ++j )        // dans cette boucle, il faut limiter les calculs car elle est répétée 128000 fois !
    {
        int k;
        double cX =  iCs;
        double cY = jCs;
        double distance = sqrt((i - cX) * (i - cX) + (j - cY) * (j - cY));
        double opacite = 255 / sqrt(0.075 * (distance)); // pour la nuance du dégrader 
        
        if ( Interieur_demiAnneauGauche( i, j, iCs, jCs,rayon1Vertical, rayon1Horizontal, rayon2Vertical, rayon2Horizontal) || Interieur_demiAnneauGauche( i, j, iCs, jCs, rayon3Vertical, rayon3Horizontal, rayon4Vertical, rayon4Horizontal))
            
            k = 5;
        else            // le pixel prend la couleur du bleu de la nuit
            k = 6;

                    matricePixels[i][j] = AlphaBlending(couleurs[k], couleurs[0], opacite);      // on inscrit la couleur dans le pixel (i,j)
    }
    
    
   
	/*______________________________________________________________________________________CRÉATION DE L'OMBRE_________________________________________________________________________________*/
     for (uint32_t i = 0; i <= HAUTEUR; i++)
     for (uint32_t j = 0; j <= LARGEUR; j++)
     {
        {

            if (cercle_Ellipse(j, i, x0, y0, a, b, m1, coeffm, m2)&& (Interieur_demiAnneauGauche( i, j, iCs, jCs,rayon1Vertical, rayon1Horizontal, rayon2Vertical, rayon2Horizontal) || Interieur_demiAnneauGauche( i, j, iCs, jCs, rayon3Vertical, rayon3Horizontal, rayon4Vertical, rayon4Horizontal)))
            { 
                matricePixels[i][j] = couleurs[7];
            }
            else if (cercle_Ellipse(j, i, x0, y0, a, b, m1, coeffm, m2))
            {
                matricePixels[i][j] = couleurs[1];
            }
            
            
        }
    }
	/*__________________________________________________________________CREATION-DE-LA-TACHE_________________________________________________________________________________*/
		
	for (int i = 0; i <= m; ++i)
    {
        for (int j = 0; j <= n; ++j)
        {
            double distance = distanceEuclidienne(i, j, iCT, jCT);

            double opacite = 255 / exp(distance * 0.025);
            if (cercle_Disque(i, j, iCs, jCs, RayonSaturn))
            {
                matricePixels[i][j] = AlphaBlending(couleurs[2], couleurs[0], opacite);
            }
        }
	}
    
    /*_________________________________________________ DESSIN DE LA DEMI-ANNEAU DU PREMIER PLAN______________________________________*/

    for ( i = 0 ; i <= m ; ++i )		// remplissage de la matrice de pixels
    for ( j = 0 ; j <= n ; ++j ) 
    {	// dans cette boucle, il faut limiter les calculs car elle est répété 128000 fois !

        double cX =  iCs;
        double cY = jCs;
        double distance = sqrt((i - cX) * (i - cX) + (j - cY) * (j - cY));


        double opacite = 255 / sqrt(0.075 * (distance)); // pour la nuance de dégradation

        if ( Interieur_demiAnneauDroite( i, j, iCs, jCs, rayon1Vertical, rayon1Horizontal, rayon2Vertical, rayon2Horizontal) || Interieur_demiAnneauDroite( i, j, iCs, jCs, rayon3Vertical, rayon3Horizontal, rayon4Vertical, rayon4Horizontal))
              matricePixels[i][j] = AlphaBlending(couleurs[5], couleurs[0],  opacite);		// on inscrit la couleur dans le pixel (i,j)
   
    }
    
    for (i = 0 ; i < m ; ++i)
    for (j = 0 ; j < n ; ++j)
    {
        
    }
	
	
	OutilsCreationImage::creeImage<LARGEUR>(nomFichierImage, matricePixels, HAUTEUR);

	printf("Image créée !\n");

	return 0;
}
