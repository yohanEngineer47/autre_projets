/*
 * OutilsCreationImage.h
 *
 *  Created on: 26 juil. 2020
 *      Author: Dom
 */

#ifndef OUTILSCREATIONIMAGE_H_
#define OUTILSCREATIONIMAGE_H_

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>

using namespace std;

/**
 *
 * Cr�e au format BMP  des images o� chaque pixel est cod� sur 4 octets RGBA
 *
 * les nombres entiers sont tous non sign�s et inscrits dans l'ordre little endian
 *
 * */
class OutilsCreationImage
{
public:
static const uint32_t TAILLE_HEADER ; // en octets = 14;
static const uint32_t TAILLE_DIB; // en octets = 56; //  correspond � BITMAPV3INFOHEADER
static const uint32_t BI_CHAMPS_BITS; // = 3; // m�thode de compression : pas de compression car les pixels sont cod�s sur 32 bits

//-------------------------------------------------------------------------------------------

/**
 * HYPOTHESES :
 * on suppose que f est ouvert en mode binaire en �criture
 * on suppose que T = unsigned short ou T = uint32_t
 * TACHE : �crit x en little endian sur f
 * DONNEES : f, x
 * RESULTATS : f modifi�
 * */
template <class T>
static void ecritLittleEndian( iostream & f, const T & x, const int nombreOctets = sizeof(T))
{
const char * s = (const char *)&x;
f.write(s, nombreOctets);
}

//-------------------------------------------------------------------------------------------

/**
 * HYPOTHESES :
 * on suppose que f est ouvert en mode binaire en lecture
 * on suppose que T = unsigned short ou T = uint32_t
 * TACHE : lit un �l�ment x de type T en little endian sur f
 * DONNEES : f
 * RESULTATS : f modifi� et x par return
 *
 * d�j� �crit dans OutilsChargementImage
 *
 * */
/*
template <class T>
static T litLittleEndian( iostream & f,  const int nombreOctets = sizeof(T))
{
T x;
f.read((char *)&x, nombreOctets);
return x;
}
*/

//-------------------------------------------------------------------------------------------

/**
 * Ecrit sur f l'ent�te Bitmap file header
 * On suppose que f est ouvert en �criture en mode binaire
 *
 * �crit 14 octets en tout sur f
 *
 * prend l'ent�te DIB BITMAPV3INFOHEADER (pour disposer du canal alpha (= opacit�))
 *
 * DONNEES : tailleFichierCompletNombreOctets : mesur� en octets
 * */
static void creeBMPFileHeader(iostream & f, const uint32_t tailleFichierCompletNombreOctets)
{
const char * nom = "BM";
f.write(nom,2); // �crit sur f les 2 caract�res du mot "BM"

ecritLittleEndian(f,tailleFichierCompletNombreOctets);
// �crit en mode little endian les 4 octets du nombre tailleFichierCompletNombreOctets

const uint32_t vide = 0;
ecritLittleEndian(f,vide); // �crit 0 sur 4 octets cons�cutifs

uint32_t offsetTableauPixel = TAILLE_HEADER + TAILLE_DIB; //0x46; // = 70 = 14 + 56 (d�cimal) car on prend l'ent�te DIB BITMAPV3INFOHEADER
										//adresse o� commmence le tableau de pixels (compt�e � partir du d�but du fichier), (multiple de 4 ?)

ecritLittleEndian(f,offsetTableauPixel);
// �crit en mode little endian offsetTableauPixel sur 4 octets cons�cutifs
}

//-------------------------------------------------------------------------------------------

/**
 * Ecrit sur f l'ent�te DIB Header
 * �crit 56 octets en tout
 *
 * DONNEES :
 * largeurEnPixels : largeur de l'image, mesur�e en pixels
 * hauteurEnPixels : hauteur de l'image, mesur�e en pixels
 * tailleTableauPixels : taille du tableau de pixels, mesur�e en octets
 * densiteHorizontale : mesur�e en pixels/m
 * densiteVerticale : mesur�e en pixels/m
 * */
static void creeBMPDIBHeader(iostream & f,
							 const uint32_t largeurEnPixels,
							 const uint32_t hauteurEnPixels,
							 const uint32_t tailleTableauPixels,
							 const uint32_t densiteHorizontale = 100,
							 const uint32_t densiteVerticale = 100);

//-------------------------------------------------------------------------------------------

/**
 * HYPOTHESES : On suppose que f est ouvert en �criture en mode binaire
 *
 * TACHE : cr�e sur f le d�but du fichier au format BMP contenant une image. cr�e donc sur f le BMP HEADER puis le DIB HEADER.
 *
 * DONNEES :
 *            nombreColonnes : nombre de colonnes du tableau de pixels = largeur de l'image mesur�e en pixels
 *            nombreLignes : nombre de lignes du tableau de pixels = hauteur de l'image mesur�e en pixels
 *			  densiteHorizontale
 *			  densiteVerticale
 * densiteHorizontale  mesur�e en pixels/m  si densiteHorizontale = densiteVerticale = 100 <===> largeur pixel = hauteur pixel = 1 cm
 *
 * RESULTATS : f partiellement rempli au format BMP : uniquement les 2 headers BMP et DIB ont �t� inscrits.
 * Le curseur de f est positionn� en fin de fichier donc pr�t � remplir le tableau bidimensionnel de pixels
 *
 * OPTIONS FIXEES :
 *
 * le DIB choisi est BITMAPV3INFOHEADER
 *
 * et la m�thode de compression est BI_CHAMPS_BITS
 * la profondeur des pixels est 32 bits = 4 octets RGBA avec 1 octet par canal
 *
 * */
inline static void creeBMPFileHeaderEtBMPDIBHeader( iostream & f,
		               const uint32_t nombreColonnes, const uint32_t nombreLignes,
		               const uint32_t densiteHorizontale = 100, const uint32_t densiteVerticale = 100)
{
uint32_t tailleTableauOctets = nombreLignes*nombreColonnes*sizeof(long);
uint32_t tailleTotaleFichierOctets = TAILLE_HEADER + TAILLE_DIB + tailleTableauOctets; // en octets

creeBMPFileHeader( f, tailleTotaleFichierOctets);

creeBMPDIBHeader( f, nombreColonnes, nombreLignes, tailleTableauOctets, densiteHorizontale, densiteVerticale);
}

//-------------------------------------------------------------------------------------------

/**
 * TACHE : �crit sur f le tableau de pixels
 *
 * DONNEES : pixels : la matrice de pixels
 * 			 nombreLignes : nombre de lignes de la matrice = hauteur de l'image mesur�e en pixrels
 *           nombreColonnes : nombre de colonnes de la matric = largeur de l'image mesur�e en pixels
 *
 * RESULTATS : f modifi�
 *
 * CONVENTIONS :
 *
 * coin haut gauche = pixels[0]
 * coin haut droit  = pixels[nombreColonnes-1]
 * coin bas  gauche = pixels[nombreColonnes*(nombreLignes-1)]
 * coin bas  droit  = pixels[nombreColonnes*nombreLignes-1]
 *
 * NOTE : le tableau pixels est consid�r� comme bidimensionnel bien que stock� comme un tableau unidimensionnel.
 * en fait "pixels" est un pointeur vers le coin haut gauche de la matrice de pixels
 *
 * Attention !!! dans f on remplit de bas en haut et de gauche � droite
 * */
static void creeTableauPixels1( iostream & f, const uint32_t pixels[],
							   const uint32_t nombreLignes, const uint32_t nombreColonnes);

//-------------------------------------------------------------------------------------------

/**
 * TACHE : �crit sur f le tableau de pixels
 *
 * DONNEES : pixels : la matrice de pixels
 * 			 nombreLignes : nombre de lignes de la matrice = hauteur de l'image mesur�e en pixrels
 *           NOMBRE_COLONNES : nombre de colonnes de la matric = largeur de l'image mesur�e en pixels
 *
 * RESULTATS : f modifi�
 *
 * CONVENTIONS :
 *
 * coin haut gauche = pixels[0][0]
 * coin haut droit  = pixels[0][NOMBRE_COLONNES-1]
 * coin bas  gauche = pixels[nombreLignes-1][0]
 * coin bas  droit  = pixels[nombreLignes-1][NOMBRE_COLONNES-1]
 *
 * NOTE : le tableau pixels est  bidimensionnel.
 * en fait "pixels" est un pointeur vers le coin haut gauche de la matrice de pixels
 *
 * Attention !!! dans f on remplit de bas en haut et de gauche � droite
 * */
template <int NOMBRE_COLONNES>
static void creeTableauPixels( iostream & f, const uint32_t (*pixels)[NOMBRE_COLONNES],
							   const uint32_t nombreLignes)
{
uint32_t i;
const uint32_t (* p)[NOMBRE_COLONNES];
for ( i = 0, p = pixels + nombreLignes-1; i < nombreLignes; ++i, --p) // on parcourt l'image de bas en haut
	{
	uint32_t j;
	const uint32_t * q;
	for ( j = 0, q = (const uint32_t *)p; j < NOMBRE_COLONNES; ++j, ++q)	// puis de la gauche vers la droite
		ecritLittleEndian(f,*q);
	}
}

//-------------------------------------------------------------------------------------------

/**
 * HYPOTHESES : On suppose que f est ouvert en �criture en mode binaire
 *
 * TACHE : cr�e f le fichier au format BMP contenant l'image d�finie par le tableau de pixels "pixels"
 *
 * DONNEES : pixels : le tableau bidimensionnel de pixels repr�sentant l'image
 *           nombreLignes : nombre de lignes du tableau pixels = hauteur de l'image mesur�e en pixels
 *           nombreColonnes : nombre de colonnes du tableau pixels = largeur de l'image mesur�e en pixels
 *			 densiteHorizontale
 *			 densiteVerticale
 * densiteHorizontale  mesur�e en pixels/m  si denstit�Horizontale = densiteVerticale = 100 <===> largeur pixel = hauteur pixel = 1 cm
 *
 * RESULTATS : f rempli au format BMP
 *
 * OPTIONS FIXEES :
 *
 * le DIB choisi est BITMAPV3INFOHEADER
 *
 * et la m�thode de compression est BI_CHAMPS_BITS
 * la profondeur des pixels est 32 bits = 4 octets RGBA avec 1 octet par canal
 *
 * NOTE : le tableau pixels est consid�r� comme bidimensionnel bien que stock� comme un tableau unidimensionnel.
 * en fait "pixels" est un pointeur vers le coin haut gauche de la matrice de pixels
 * */
static void creeImage1( iostream & f, const uint32_t pixels[],
		               const uint32_t nombreLignes, const uint32_t nombreColonnes,
		               const uint32_t densiteHorizontale = 100, const uint32_t densiteVerticale = 100)
{
creeBMPFileHeaderEtBMPDIBHeader( f, nombreColonnes, nombreLignes, densiteHorizontale, densiteVerticale);
creeTableauPixels1( f, pixels, nombreLignes, nombreColonnes);
}

//-------------------------------------------------------------------------------------------

/**
 * HYPOTHESES : On suppose que f est ouvert en �criture en mode binaire
 *
 * TACHE : cr�e f le fichier au format BMP contenant l'image d�finie par le tableau de pixels "pixels"
 *
 * DONNEES : pixels : le tableau bidimensionnel de pixels repr�sentant l'image
 *           nombreLignes : nombre de lignes du tableau pixels = hauteur de l'image mesur�e en pixels
 *           NOMBRE_COLONNES : nombre de colonnes du tableau pixels = largeur de l'image mesur�e en pixels
 *			 densiteHorizontale
 *			 densiteVerticale
 * densiteHorizontale  mesur�e en pixels/m  si denstit�Horizontale = densiteVerticale = 100 <===> largeur pixel = hauteur pixel = 1 cm
 *
 * RESULTATS : f rempli au format BMP
 *
 * OPTIONS FIXEES :
 *
 * le DIB choisi est BITMAPV3INFOHEADER
 *
 * et la m�thode de compression est BI_CHAMPS_BITS
 * la profondeur des pixels est 32 bits = 4 octets RGBA avec 1 octet par canal
 *
 * NOTE : le tableau pixels est bidimensionnel.
 * en fait "pixels" est un pointeur vers le coin haut gauche de la matrice de pixels
 * */
template <int NOMBRE_COLONNES>
static void creeImage( iostream & f, const uint32_t (*pixels)[NOMBRE_COLONNES],
		               const uint32_t nombreLignes,
		               const uint32_t densiteHorizontale = 100, const uint32_t densiteVerticale = 100)
{
creeBMPFileHeaderEtBMPDIBHeader( f, NOMBRE_COLONNES, nombreLignes, densiteHorizontale, densiteVerticale);
creeTableauPixels( f, pixels, nombreLignes);
}

/*
{
uint32_t tailleTableau = nombreLignes*NOMBRE_COLONNES*sizeof(long);
uint32_t tailleTotaleFichier = TAILLE_HEADER + TAILLE_DIB + tailleTableau; // en octets
creeBMPFileHeader( f, tailleTotaleFichier);

creeBMPDIBHeader( f, NOMBRE_COLONNES, nombreLignes, tailleTableau, densiteHorizontale, densiteVerticale);
creeTableauPixels( f, pixels, nombreLignes);
}
*/


//-------------------------------------------------------------------------------------------

/**
 * HYPOTHESES : On suppose que nomFichierImage a une extension .bmp
 *
 * TACHE : cr�e  le fichier au format BMP contenant l'image d�finie par le tableau de pixels "pixels"
 *
 * DONNEES :
 * 			 nomFichierImage : nom du fichier disque qui va contenir la sauvegarde de l'image. doit avoir l'extension .bmp
 * 			 pixels : le tableau bidimensionnel de pixels repr�sentant l'image
 *           nombreLignes : nombre de lignes du tableau pixels = hauteur de l'image mesur�e en pixels
 *           NOMBRE_COLONNES : nombre de colonnes du tableau pixels = largeur de l'image mesur�e en pixels
 *			 densiteHorizontale
 *			 densiteVerticale
 * densiteHorizontale  mesur�e en pixels/m  si denstit�Horizontale = densiteVerticale = 100 <===> largeur pixel = hauteur pixel = 1 cm
 *
 * RESULTATS : le fichier de nom nomFichierImage rempli au format BMP
 *
 * OPTIONS FIXEES :
 *
 * le DIB choisi est BITMAPV3INFOHEADER
 *
 * et la m�thode de compression est BI_CHAMPS_BITS
 * la profondeur des pixels est 32 bits = 4 octets RGBA avec 1 octet par canal
 *
 * NOTE : le tableau pixels est bidimensionnel.
 * en fait "pixels" est un pointeur vers le coin haut gauche de la matrice de pixels
 * */
template <int NOMBRE_COLONNES>
static void creeImage( const string & nomFichierImage, const uint32_t (*pixels)[NOMBRE_COLONNES],
		               const uint32_t nombreLignes,
		               const uint32_t densiteHorizontale = 100, const uint32_t densiteVerticale = 100)
{
fstream f(nomFichierImage,ios::out|ios::binary); // ouverture en lecture et �criture seule et en mode binaire

OutilsCreationImage::creeImage( f, pixels, nombreLignes, densiteHorizontale, densiteVerticale);		// sauve l'image sur le fichier f

f.close();
}


};

#endif /* OUTILSCREATIONIMAGE_H_ */
