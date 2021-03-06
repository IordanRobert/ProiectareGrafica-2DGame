#pragma once

#include <vector>
#include "GLTexture.h"
#include <string>
#include <map>
#include <string>

#ifndef IMAGE_LOADER_H_INCLUDED
#define IMAGE_LOADER_H_INCLUDED

static std::map<std::string, int> textures;

//Reprezentarea unei imagini
class Image {
public:
	Image(char* ps, unsigned int w, unsigned int h);
	~Image();
	/* O matrice de forma (R1, G1, B1, R2, G2, B2, ...) indicând
	* Culoarea fiecarui pixel din imagine. Componente de culoare variază de la 0 la 255.
	* Matricea începe de la pixelul din stânga-jos, apoi se mută până la sfârșitul
	* rândului, apoi se mută pe la coloana următoare, și așa mai departe. Acesta este
	* Formatul în care OpenGL plaseaza imagini. */
	char* pixels;
	unsigned int width;
	unsigned int height;
};
//Citeste imaginea bmp din fisier.
Image* loadBMP(const char* filename);
GLTexture loadPNG(const char* filename, std::string textureName, const int textureId);

//void createTexture(const char* filename, const std::string textureName, uint8_t textureId);
//void loadTextures();

#endif