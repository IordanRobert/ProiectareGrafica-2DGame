#include <iostream>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "imageloader.h"
using namespace std;
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0);
	}
}
//Transforma imaginea într-o textură, și returnează ID-ul texturii
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Construieste camera pentru textura
	glBindTexture(GL_TEXTURE_2D, textureId); //Comunica OpenGL ce textura sa editeze
	//Harta imagine pentru textura
	glTexImage2D(GL_TEXTURE_2D, // GL_TEXTURE_2D
		0, //0 deocamdata
		GL_RGB, //Formatul OpenGL utilizat pentru imagine
		image->width, image->height, //Latimea si inaltimea
		0, //Cadrul imaginii
		GL_RGB, //GL_RGB,
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE,
		//numere fara semn
		image->pixels); //Datele pentru pixeli
	return textureId; //Returneaza id-ul texturii
}
GLuint _textureId; //id-ul texturii
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	Image* image = loadBMP("grasu.bmp");
	_textureId = loadTexture(image);
	delete image;
}
void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 1.0f, -6.0f);
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	GLfloat directedLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat directedLightPos[] = { -10.0f, 15.0f, 20.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	//Bottom
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.5f, -2.5f, 2.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.5f, -2.5f, 2.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.5f, -2.5f, -2.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.5f, -2.5f, -2.5f);
	glEnd();
	//Jos
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.5f, -2.5f, -2.5f);
	glTexCoord2f(5.0f, 5.0f);
	glVertex3f(0.0f, 2.5f, -2.5f);
	glTexCoord2f(10.0f, 0.0f);
	glVertex3f(2.5f, -2.5f, -2.5f);
	glEnd();
	//Stanga
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 0.7f, 0.3f);
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-2.5f, -2.5f, 2.5f);
	glVertex3f(-2.5f, -2.5f, -2.5f);
	glVertex3f(-2.5f, 2.5f, -2.5f);
	glVertex3f(-2.5f, 2.5f, 2.5f);
	glEnd();
	glutSwapBuffers();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Textura");
	initRendering();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMainLoop();
	return 0;
}