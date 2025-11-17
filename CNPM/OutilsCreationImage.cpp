/*
 * OutilsCreationImage.cpp
 *
 *  Created on: 26 juil. 2020
 *      Author: Dom
 */
#include <cstdio>
#include <cstdint>
#include "OutilsCreationImage.h"

/*static*/ const uint32_t OutilsCreationImage::TAILLE_HEADER = 14;
/*static*/ const uint32_t OutilsCreationImage::TAILLE_DIB = 56; // correspond � BITMAPV3INFOHEADER
/*static*/ const uint32_t OutilsCreationImage::BI_CHAMPS_BITS = 3; // m�thode de compression : pas de compression car les pixels sont cod�s sur 32 bits

//--------------------------------------------------------------------------------------------------------

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
/*static*/ void OutilsCreationImage::creeBMPDIBHeader(iostream & f,
							 const uint32_t largeurEnPixels,
							 const uint32_t hauteurEnPixels,
							 const uint32_t tailleTableauPixels,
							 const uint32_t densiteHorizontale,
							 const uint32_t densiteVerticale)
{
ecritLittleEndian(f,TAILLE_DIB);

ecritLittleEndian(f,largeurEnPixels);

ecritLittleEndian(f,hauteurEnPixels);

const unsigned short nombrePlansCouleur = 1;
ecritLittleEndian(f,nombrePlansCouleur);

const unsigned short profondeurPixel = 32; // 32 bits pour coder un pixel
ecritLittleEndian(f,profondeurPixel);

const uint32_t methodeCompression = BI_CHAMPS_BITS; // 3; // correspond � BI_CHAMPS_BITS,
											// pas de compression car profondeur = 32
ecritLittleEndian(f,methodeCompression);

ecritLittleEndian(f, tailleTableauPixels);

ecritLittleEndian(f,densiteHorizontale);
ecritLittleEndian(f,densiteVerticale);

const uint32_t vide = 0; // 4 octets contenant 0

ecritLittleEndian(f,vide);  // nombre de couleurs dans la palette
ecritLittleEndian(f,vide);	// toutes les couleurs sont importantes

//-------------- masques pour les champs  R G B A -------------------

// masques pour R G B A dans cet ordre qui est d�fini par BI_BITFILEDS

const int L = 4;
int m = L-1;
const uint32_t masquesRGBA[L] = { 0xFF000000,
									   0x00FF0000,
									   0x0000FF00,
									   0x000000FF};

const uint32_t * p;

int i;

for ( i = 0, p = masquesRGBA; i <= m; ++i, ++p) ecritLittleEndian(f,*p);

}

//------------------------------------------------------------------------------------------------------

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
/*static*/ void OutilsCreationImage::creeTableauPixels1( iostream & f, const uint32_t pixels[],
							   const uint32_t nombreLignes, const uint32_t nombreColonnes)
{
uint32_t i;
const uint32_t * p;
for ( i = 0, p = pixels+(nombreLignes-1)*nombreColonnes; i < nombreLignes; ++i, p-=nombreColonnes) // on parcourt l'image de bas en haut
	{
	uint32_t j;
	const uint32_t * q;
	for ( j = 0, q = p; j < nombreColonnes; ++j, ++q)	// puis de la gauche vers la droite
		ecritLittleEndian(f,*q);
	}
}

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
/*template <int NOMBRE_COLONNES>
static void OutilsCreationImage::creeTableauPixels( iostream & f, const uint32_t (*pixels)[NOMBRE_COLONNES],
							   const uint32_t nombreLignes)*/

