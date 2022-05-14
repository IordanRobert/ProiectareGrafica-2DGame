#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdint>
#include <iostream>

#include "Object.h"
#include "Character.h"
#include "ImageLoader.h"

float movementX = 0.0;
float movementY = 0.0;
float spin = 0.0;
double red = 0.0;
double green = 0.0;
double blue = 0.0;

int screenWidth = 800;
int screenHeight = 600;

float entities[10][4];
int entitiesIdx = 0;
void initEntities();
void addEntities(float XY[4]);
void removeEntities();

bool collisionAABB(float posX, float posY);
bool collisionUp();
bool collisionDown();
bool collisionLeft();
bool collisionRight();

Character player(48.0, 5.0, 6.0);
Object ground;
Object platformLeft;
Object platformRight;
Object platformCenter;

float gravityForce = 2.0;
bool gravityActive = true;

bool keyStates[256];

void init(void);
void display(void);
void reshape(int w, int h);
void spinDisplay(void);

void gravity();
void controls();

void mouse(int button, int state, int x, int y);
void keyUp(unsigned char key, int x, int y);
void keyDown(unsigned char key, int x, int y);
void timer(int);
void lazyJmp();

void movementKeys();
void specialKeys();

GLuint _textureId;
GLuint loadTexture(Image* image);
void initRendering(const char path[]);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow("Platformer");

	init();

	initRendering("sponge24bit.bmp");

	glutDisplayFunc(display);

	//glutReshapeFunc(reshape);

	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

	glutTimerFunc(1000/120, timer, 0);

	glutMainLoop();
	return 0;
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.5, 0.0);
	glPointSize(1.0);
	glShadeModel(GL_SMOOTH);
	gluOrtho2D(-screenWidth / 2, screenWidth / 2, -screenHeight / 2, screenHeight / 2);

	initEntities();
	/* GROUND */
	//glRectf(-screenWidth / 2, -screenHeight / 2, screenWidth / 2, -screenHeight / 2 + 20);
	ground.coords[0] = -screenWidth / 2.0;
	ground.coords[1] = -screenHeight / 2.0;
	ground.coords[2] = screenWidth / 2.0;
	ground.coords[3] = -screenHeight / 2.0 + 20;
	addEntities(ground.coords);
	
	ground.color[0] = 0.0;
	ground.color[1] = 0.63;
	ground.color[2] = 0.0;


	/* PLATFORMS */
	//glRectf(-screenWidth / 2 + 100, -screenHeight / 2 + 150, -screenWidth / 2 + 300, -screenHeight / 2 + 20 + 150);
	platformLeft.coords[0] = -screenWidth / 2.0 + 100;
	platformLeft.coords[1] = -screenHeight / 2.0 + 150;
	platformLeft.coords[2] = -screenWidth / 2.0 + 300;
	platformLeft.coords[3] = -screenHeight / 2.0 + 20 + 150;
	addEntities(platformLeft.coords);

	platformLeft.color[0] = 0.25;
	platformLeft.color[1] = 0.25;
	platformLeft.color[2] = 0.25;

	//glRectf(screenWidth / 2 - 100, -screenHeight / 2 + 150, screenWidth / 2 - 300, -screenHeight / 2 + 20 + 150);
	platformRight.coords[0] = screenWidth / 2.0 - 100;
	platformRight.coords[1] = -screenHeight / 2.0 + 150;
	platformRight.coords[2] = screenWidth / 2.0 - 300;
	platformRight.coords[3] = -screenHeight / 2.0 + 20 + 150;
	addEntities(platformRight.coords);

	platformRight.color[0] = 0.25;
	platformRight.color[1] = 0.25;
	platformRight.color[2] = 0.25;

	//glRectf(-screenWidth / 2 + 310, -screenHeight / 2 + 250, screenWidth / 2 - 310, -screenHeight / 2 + 20 + 250);
	platformCenter.coords[0] = -screenWidth / 2.0 + 310;
	platformCenter.coords[1] = -screenHeight / 2.0 + 250;
	platformCenter.coords[2] = screenWidth / 2.0 - 310;
	platformCenter.coords[3] = -screenHeight / 2.0 + 20 + 250;
	addEntities(platformCenter.coords);

	platformCenter.color[0] = 0.25;
	platformCenter.color[1] = 0.25;
	platformCenter.color[2] = 0.25;

}

void gravity() {
	if (!collisionDown() && gravityActive) {
		for (float i = 0; i <= gravityForce; i += 0.1) {
			if (!collisionDown()) {
				movementY -= 0.1;
				player.move(movementX, movementY);
			}
			else break;
		}
	}
}

void lazyJmp() {
	if (!collisionUp() && gravityActive && player.isInAir && !player.isFalling) {
		//std::cout << "here" << std::endl;
		for (float i = 0; i <= player.maxJump; i += 0.1) {
			if (!collisionUp()) {
				movementY += 0.1;
				player.move(movementX, movementY);
				//if (i = player.maxJump) player.isFalling = true;
			}
			else break;
		}
		player.isInAir = false;
	}
}

void display(void)
{ 
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();

	player.color[0] = 1.0;
	player.color[1] = 1.0;
	player.color[2] = 1.0;
	player.texture = _textureId;
	player.renderTex();

	/*float playerCX = movementX;
	float playerCY = movementY;

	glPointSize(10.0);
	glColor3f(1.0, 0.0, 0.0);
	glRectf(playerCX-2, playerCY-2, playerCX+2, playerCY+2);*/

	ground.render();
	platformLeft.render();
	platformRight.render();
	platformCenter.render();

	glutPostRedisplay();
	glPopMatrix();
	glutSwapBuffers();
}

void spinDisplay(void)
{
	spin = spin + 2.0;
	if (spin > 360.0)
		spin = spin - 360.0;
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(spinDisplay);
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

void keyDown(unsigned char key, int x, int y) {
	keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
	keyStates[key] = false;
}

void controls() {
	movementKeys();
	specialKeys();
}

void movementKeys() {
	if (keyStates['w'] || keyStates['W'])
			if (!collisionUp() && !player.isInAir) {
					player.isInAir = true;
					/*for (float i = 0; i <= player.maxJump; i += 0.1) {
						if (!collisionUp()) {
							movementY += 0.1;
							player.move(movementX, movementY);
						}
						else break;
					}*/
			}
	//printf("X: %f\n Y: %f\n", movementX, movementY);
		
	if (keyStates['s'] || keyStates['S'])
		if (!collisionDown()) {
			for (float i = 0; i <= player.maxSpeed; i += 0.1) {
				if (!collisionDown()) {
					movementY -= 0.1;
					player.move(movementX, movementY);
				}
				else break;
			}
		}
	//printf("X: %f\n Y: %f\n", movementX, movementY);

	if (keyStates['a'] || keyStates['A'])
		if (-screenWidth / 2 + player.size/2 < movementX) 
			if (!collisionLeft()) {
				for (float i = 0; i <= player.maxSpeed; i += 0.1) {
					if (!collisionLeft()) {
						movementX -= 0.1;
						player.move(movementX, movementY);
					}
					else break;
				}
			}
	//printf("X: %f\n Y: %f\n", movementX, movementY);

	if(keyStates['d'] || keyStates['D'])
		if (screenWidth / 2 - player.size/2 > movementX) 
			if (!collisionRight()) {
				for (float i = 0; i <= player.maxSpeed; i += 0.1) {
					if (!collisionRight()) {
						movementX += 0.1;
						player.move(movementX, movementY);
					}
					else break;
				}
			}
	//printf("X: %f\n Y: %f\n", movementX, movementY);

}

void specialKeys() {
	if (keyStates['q'] || keyStates['Q'])
	{
		spin += 3;
		if (spin < -360.0)
			spin = spin + 360.0;
	}
	if (keyStates['e'] || keyStates['E'])
	{
		spin -= 3;
		if (spin > 360.0)
			spin = spin - 360.0;
	}

	if (keyStates['1'])
	{
		red += 1.0 / 255 * 2;
		if (red > 1.0) red = 0.0;
	}
	if (keyStates['2'])
	{
		green += 1.0 / 255 * 2;
		if (green > 1.0) green = 0.0;
	}
	if (keyStates['3'])
	{
		blue += 1.0 / 255 * 2;
		if (blue > 1.0) blue = 0.0;
	}
	if (keyStates[27]) {
		exit(0);
	}
}

void initEntities() {
	for (float* entity : entities) {
		entity[0] = UINT16_MAX;
		entity[1] = UINT16_MAX;
		entity[2] = UINT16_MAX;
		entity[3] = UINT16_MAX;
	}
}

void addEntities(float XY[]) {
	entities[entitiesIdx][0] = XY[0];
	entities[entitiesIdx][1] = XY[1];
	entities[entitiesIdx][2] = XY[2];
	entities[entitiesIdx][3] = XY[3];
	entitiesIdx++;
}

bool collisionAABB(float posX, float posY) {
	float playerCX = posX;
	float playerCY = posY;
	//printf("player: %f %f\n", playerCX, playerCY);

	for (float *entity : entities) {
		float entityX = abs(*entity - *(entity + 2));
		float entityY = abs(*(entity + 1) - *(entity + 3));
		float entityCX = *entity + (*(entity+2) - *entity)/2;
		float entityCY = *(entity+1) + (*(entity+3) - *(entity+1))/2;

		//printf("entity(cx, cy, x, y): %f %f %f %f\n", entityCX, entityCY, entityX, entityY);

		if (playerCX + player.size / 2 > entityCX - entityX / 2
			&& playerCX - player.size / 2 < entityCX + entityX / 2
			&& playerCY + player.size / 2 > entityCY - entityY / 2
			&& playerCY - player.size / 2 < entityCY + entityY / 2)
		{
			//std::cout << "Collision detected\n";
			return true;
		}
	}
	return false;
}

bool collisionUp() {
	float nextX = movementX, nextY = movementY + 0.1;
	return collisionAABB(nextX, nextY);
}

bool collisionDown() {
	float nextX = movementX, nextY = movementY - 0.1;
	if (collisionAABB(nextX, nextY)) {
		player.isInAir = false;
		player.isFalling = false;
	}
	return collisionAABB(nextX, nextY);
}

bool collisionLeft() {
	float nextX = movementX - 0.1, nextY = movementY;
	return collisionAABB(nextX, nextY);
}

bool collisionRight() {
	float nextX = movementX + 0.1, nextY = movementY;
	return collisionAABB(nextX, nextY);
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
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, //numere fara semn
		image->pixels); //Datele pentru pixeli
	return textureId; //Returneaza id-ul texturii
}

void initRendering(const char path[]) {
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	_textureId = loadTexture(loadBMP(path));
}

void timer(int cadrucurent) {
	lazyJmp();
	gravity();
	controls();
	glutTimerFunc(1000 / 120, timer, cadrucurent);
}
