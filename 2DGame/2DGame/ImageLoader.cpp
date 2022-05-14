﻿#include "ImageLoader.h"
#include "picopng.h"

#include <assert.h>
#include <fstream>
#include <string>

using namespace std;
Image::Image(char* ps, unsigned int w, unsigned int h) : pixels(ps), width(w), height(h) {
}
Image::~Image() {
	delete[] pixels;
}
namespace {
	//Convertește un sir de patru caractere la un număr întreg
	int toInt(const char* bytes) {
		return (int)(((unsigned char)bytes[3] << 24) |
			((unsigned char)bytes[2] << 16) |
			((unsigned char)bytes[1] << 8) |
			(unsigned char)bytes[0]);
	}
	//Convertește un sir de patru caractere la un număr întreg
	short toShort(const char* bytes) {
		return (short)(((unsigned char)bytes[1] << 8) |
			(unsigned char)bytes[0]);
	}
	//Citește următorii octeți ca un întreg
	int readInt(ifstream& input) {
		char buffer[4];
		input.read(buffer, 4);
		return toInt(buffer);
	}
	//Citește următorii octeți
	short readShort(ifstream& input) {
		char buffer[2];
		input.read(buffer, 2);
		return toShort(buffer);
	}
	//Just like auto_ptr, but for arrays
	template<class T>
	class auto_array {
	private:
		T* array;
		mutable bool isReleased;
	public:
		explicit auto_array(T* array_ = NULL) :
			array(array_), isReleased(false) {
		}
		auto_array(const auto_array<T>& aarray) {
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}
		~auto_array() {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
		}
		T* get() const {
			return array;
		}
		T& operator*() const {
			return *array;
		}
		void operator=(const auto_array<T>& aarray) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}
		T* operator->() const {
			return array;
		}
		T* release() {
			isReleased = true;
			return array;
		}
		void reset(T* array_ = NULL) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = array_;
		}
		T* operator+(int i) {
			return array + i;
		}
		T& operator[](int i) {
			return array[i];
		}
	};
}

Image* loadBMP(const char* filename) {
	ifstream input;
	input.open(filename, ifstream::binary);
	assert(!input.fail() || !"Nu a gasit fisierul");
	char buffer[2];
	input.read(buffer, 2);
	assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Nu a gasit fisierul bitmap");
	input.ignore(8);
	int dataOffset = readInt(input);
	//Read the header
	int headerSize = readInt(input);
	int width;
	int height;
	switch (headerSize) {
	case 40:
		//V3
		width = readInt(input);
		height = readInt(input);
		input.ignore(2);
		assert(readShort(input) == 24 || !"Imaginea nu este de 24 bits per pixel");
			assert(readShort(input) == 0 || !"Imagine comprimata");
		break;
	case 12:
		//OS/2 V1
		width = readShort(input);
		height = readShort(input);
		input.ignore(2);
		assert(readShort(input) == 24 || !"Imaginea nu este de 24 bits per pixel");
			break;
	case 64:
		//OS/2 V2
		assert(!"Nu se poate incarca ");
		break;
	case 108:
		//Windows V4
		assert(!"Nu se poate incarca ");
		break;
	case 124:
		//Windows V5
		assert(!"Can't load Windows V5 bitmaps");
		break;
	default:
		assert(!"Unknown bitmap format");
	}
	//Citeste datele
	int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
	int size = bytesPerRow * height;
	auto_array<char> pixels(new char[size]);
	input.seekg(dataOffset, ios_base::beg);
	input.read(pixels.get(), size);
	//Obtine datele in formatul corect
	auto_array<char> pixels2(new char[width * height * 3]);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < 3; c++) {
				pixels2[3 * (width * y + x) + c] =
					pixels[bytesPerRow * y + 3 * x + (2 - c)];
			}
		}
	}
	input.close();
	return new Image(pixels2.release(), width, height);
}

GLTexture loadPNG(const char* filename, const std::string textureName, const int textureId) {
	GLTexture texture = {};

	vector<unsigned char> out;
	vector<unsigned char> in;
	unsigned long width, height;

	ifstream input;
	input.open(filename, ifstream::binary);

	while (!input.eof()) {
		in.push_back(input.get());
	}

	input.close();

	int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
	if (errorCode != 0) {
		assert(!"decodePNG failed with error: " + errorCode);
	}

	glGenTextures(1, &(texture.id));
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

	return GLTexture();
}

//void createTexture(const char* filename, const std::string textureName, uint8_t textureId) {
//	loadPNG(filename, textureName, textureId);
//	textures.insert(std::pair<std::string, int>(textureName, textureId));
//}
//
//void loadTextures() {
//	createTexture("./Resources/Characters/Cyborg/Cyborg_hurt.png", "cyborg", 1);
//	createTexture("./Resources/Characters/Biker/Biker_hurt.png", "biker", 2);
//	createTexture("./Resources/Characters/Punk/Punk_hurt.png", "punk", 3);
//}