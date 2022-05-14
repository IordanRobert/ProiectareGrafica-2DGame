#pragma once
#ifndef IMAGE_LOADER_H_INCLUDED
#define IMAGE_LOADER_H_INCLUDED
//Reprezentarea unei imagini
class Image {
public:
	Image(char* ps, int w, int h);
	~Image();
	/* O matrice de forma (R1, G1, B1, R2, G2, B2, ...) indicând
	* Culoarea fiecarui pixel din imagine. Componente de culoare variază de la 0 la 255.
	* Matricea începe de la pixelul din stânga-jos, apoi se mută până la sfârșitul
	* rândului, apoi se mută pe la coloana următoare, și așa mai departe. Acesta este
	* Formatul în care OpenGL plaseaza imagini. */
	char* pixels;
	int width;
	int height;
};
//Citeste imaginea bmp din fisier.
Image* loadBMP(const char* filename);
#endif