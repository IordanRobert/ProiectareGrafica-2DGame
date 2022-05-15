#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <iostream>
#include <list>

#include "Object.h"
#include "Character.h"
#include "ImageLoader.h"
#include "lodepng.h"
#include "picopng.h"
#include "WorldPhysics.h"

float movementX = 0.0;
float movementY = 0.0;
float spin = 0.0;
double red = 0.0;
double green = 0.0;
double blue = 0.0;

int screenWidth = 576*2;
int screenHeight = 324*2;

Character player(48.0, 5.0, 6.0);
int sprites = 8;
int currentSprite = 0;
int animationCounter = 0;
#define ANIMATION_DELAY 20
void animationAttack1();
void animationAttack2();
void animationAttack3();
void animationClimb();
void animationDeath();
void animationDoublejump();
void animationHurt();
void animationIdle();
void animationJump();
void animationPunch();
void animationRun();
void animationRunAttack();

Object ground;
Object platformLeft;
Object platformRight;
Object platformCenter;

Object bgPart1;
Object bgPart2;
Object bgPart3;
Object bgPart4;
Object bgPart5;
Object bgIllumination;

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
void createTexture(const char* filename, std::string textureName, uint8_t textureId);
void loadTextures();

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow("Platformer");

	init();

	//initRendering("sponge24bit.bmp");

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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glPointSize(1.0);
	glShadeModel(GL_SMOOTH);
	gluOrtho2D(-screenWidth / 2, screenWidth / 2, -screenHeight / 2, screenHeight / 2);

	loadTextures();

	initEntities();
	
	/* BACKGROUND */
	bgPart1.coords[0] = -screenWidth / 2;
	bgPart1.coords[1] = -screenHeight / 2 + 32;
	bgPart1.coords[2] = screenWidth / 2;
	bgPart1.coords[3] = screenWidth / 2;
	bgPart1.texture = textures["DayPart1"];

	bgPart2.coords[0] = -screenWidth / 2;
	bgPart2.coords[1] = -screenHeight / 2 + 32;
	bgPart2.coords[2] = screenWidth / 2;
	bgPart2.coords[3] = screenWidth / 2;
	bgPart2.texture = textures["DayPart2"];

	bgPart3.coords[0] = -screenWidth / 2;
	bgPart3.coords[1] = -screenHeight / 2 + 32;
	bgPart3.coords[2] = screenWidth / 2;
	bgPart3.coords[3] = screenWidth / 2;
	bgPart3.texture = textures["DayPart3"];

	bgPart4.coords[0] = -screenWidth / 2;
	bgPart4.coords[1] = -screenHeight / 2 + 32;
	bgPart4.coords[2] = screenWidth / 2;
	bgPart4.coords[3] = screenWidth / 2;
	bgPart4.texture = textures["DayPart4"];

	bgPart5.coords[0] = -screenWidth / 2;
	bgPart5.coords[1] = -screenHeight / 2 + 32;
	bgPart5.coords[2] = screenWidth / 2;
	bgPart5.coords[3] = screenWidth / 2;
	bgPart5.texture = textures["DayPart5"];

	bgIllumination.coords[0] = -screenWidth / 2;
	bgIllumination.coords[1] = -screenHeight / 2 + 32;
	bgIllumination.coords[2] = screenWidth / 2;
	bgIllumination.coords[3] = screenWidth / 2;
	bgIllumination.texture = textures["Illumination"];

	/* GROUND */
	//glRectf(-screenWidth / 2, -screenHeight / 2, screenWidth / 2, -screenHeight / 2 + 20);
	ground.coords[0] = -screenWidth / 2.0;
	ground.coords[1] = -screenHeight / 2.0;
	ground.coords[2] = screenWidth / 2.0;
	ground.coords[3] = -screenHeight / 2.0 + 32;
	ground.texture = textures["tileTop"];
	addEntities(ground.coords);


	/* PLATFORMS */
	//glRectf(-screenWidth / 2 + 100, -screenHeight / 2 + 150, -screenWidth / 2 + 300, -screenHeight / 2 + 20 + 150);
	platformLeft.coords[0] = -screenWidth / 2.0 + 100;
	platformLeft.coords[1] = -screenHeight / 2.0 + 150;
	platformLeft.coords[2] = -screenWidth / 2.0 + 300;
	platformLeft.coords[3] = -screenHeight / 2.0 + 20 + 150;
	platformLeft.texture = textures["tileTop"];
	addEntities(platformLeft.coords);

	//glRectf(screenWidth / 2 - 100, -screenHeight / 2 + 150, screenWidth / 2 - 300, -screenHeight / 2 + 20 + 150);
	platformRight.coords[0] = screenWidth / 2.0 - 100;
	platformRight.coords[1] = -screenHeight / 2.0 + 150;
	platformRight.coords[2] = screenWidth / 2.0 - 300;
	platformRight.coords[3] = -screenHeight / 2.0 + 20 + 150;
	platformRight.texture = textures["tileTop"];
	addEntities(platformRight.coords);

	//glRectf(-screenWidth / 2 + 310, -screenHeight / 2 + 250, screenWidth / 2 - 310, -screenHeight / 2 + 20 + 250);
	platformCenter.coords[0] = -screenWidth / 2.0 + 310;
	platformCenter.coords[1] = -screenHeight / 2.0 + 250;
	platformCenter.coords[2] = screenWidth / 2.0 - 310;
	platformCenter.coords[3] = -screenHeight / 2.0 + 20 + 250;
	platformCenter.texture = textures["tileTop"];
	addEntities(platformCenter.coords);
	
}

void gravity() {
	if (!collisionDown(movementX, movementY, player) && gravityActive) {
		for (float i = 0; i <= gravityForce; i += 0.1) {
			if (!collisionDown(movementX, movementY, player)) {
				movementY -= 0.1;
				player.move(movementX, movementY);
			}
			else break;
		}
	}
}

void lazyJmp() {
	if (!collisionUp(movementX, movementY, player) && gravityActive && player.isInAir && !player.isFalling) {
		//std::cout << "here" << std::endl;
		for (float i = 0; i <= player.maxJump; i += 0.1) {
			if (!collisionUp(movementX, movementY, player)) {
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

	bgPart1.renderTex();
	bgPart2.renderTex();
	bgPart3.renderTex();
	bgPart4.renderTex();
	bgPart5.renderTex();
	//bgIllumination.renderTex();

	player.color[0] = 1.0;
	player.color[1] = 1.0;
	player.color[2] = 1.0;
	player.character = "biker";
	animationRunAttack();
	player.renderTex(currentSprite, sprites);
	if (animationCounter >= ANIMATION_DELAY) {
		animationCounter = 0;
		currentSprite > sprites ? (currentSprite = 0) : currentSprite++;
	}
	animationCounter++;
	/*float playerCX = movementX;
	float playerCY = movementY;

	glPointSize(10.0);
	glColor3f(1.0, 0.0, 0.0);
	glRectf(playerCX-2, playerCY-2, playerCX+2, playerCY+2);*/

	ground.renderTex();
	platformLeft.renderTex();
	platformRight.renderTex();
	platformCenter.renderTex();


	//Test
	/*Object test;
	test.coords[0] = 0;
	test.coords[1] = 0;
	test.coords[2] = 32;
	test.coords[3] = 32;
	test.color[0] = 1.0;
	test.color[1] = 1.0;
	test.color[2] = 1.0;
	test.texture = textures["tileTop"];
	test.renderTex();*/

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
			if (!collisionUp(movementX, movementY, player) && !player.isInAir) {
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
		if (!collisionDown(movementX, movementY, player)) {
			for (float i = 0; i <= player.maxSpeed; i += 0.1) {
				if (!collisionDown(movementX, movementY, player)) {
					movementY -= 0.1;
					player.move(movementX, movementY);
				}
				else break;
			}
		}
	//printf("X: %f\n Y: %f\n", movementX, movementY);

	if (keyStates['a'] || keyStates['A'])
		if (-screenWidth / 2 + player.size/2 < movementX) 
			if (!collisionLeft(movementX, movementY, player)) {
				for (float i = 0; i <= player.maxSpeed; i += 0.1) {
					if (!collisionLeft(movementX, movementY, player)) {
						movementX -= 0.1;
						player.move(movementX, movementY);
						player.direction = 'l';
					}
					else break;
				}
			}
	//printf("X: %f\n Y: %f\n", movementX, movementY);

	if(keyStates['d'] || keyStates['D'])
		if (screenWidth / 2 - player.size/2 > movementX) 
			if (!collisionRight(movementX, movementY, player)) {
				for (float i = 0; i <= player.maxSpeed; i += 0.1) {
					if (!collisionRight(movementX, movementY, player)) {
						movementX += 0.1;
						player.move(movementX, movementY);
						player.direction = 'r';
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

void createTexture(const char* filename, const std::string textureName, uint8_t textureId) {
	loadPNG(filename, textureName, textureId);
	textures.insert(std::pair<std::string, int>(textureName, textureId));
}

void loadTextures() {
	/* Characters */
	int i = 1;
	//Biker
	createTexture("./Resources/Characters/Biker/Biker_attack1.png", "biker_attack1", i++);
	createTexture("./Resources/Characters/Biker/Biker_attack2.png", "biker_attack2", i++);
	createTexture("./Resources/Characters/Biker/Biker_attack3.png", "biker_attack3", i++);
	createTexture("./Resources/Characters/Biker/Biker_climb.png", "biker_climb", i++);
	createTexture("./Resources/Characters/Biker/Biker_death.png", "biker_death", i++);
	createTexture("./Resources/Characters/Biker/Biker_doublejump.png", "biker_doublejump", i++);
	createTexture("./Resources/Characters/Biker/Biker_hurt.png", "biker_hurt", i++);
	createTexture("./Resources/Characters/Biker/Biker_idle.png", "biker_idle", i++);
	createTexture("./Resources/Characters/Biker/Biker_jump.png", "biker_jump", i++);
	createTexture("./Resources/Characters/Biker/Biker_punch.png", "biker_punch", i++);
	createTexture("./Resources/Characters/Biker/Biker_run.png", "biker_run", i++);
	createTexture("./Resources/Characters/Biker/Biker_run_attack.png", "biker_run_attack", i++);

	
	//Punk
	createTexture("./Resources/Characters/Punk/Punk_attack1.png", "punk_attack1", i++);
	createTexture("./Resources/Characters/Punk/Punk_attack2.png", "punk_attack2", i++);
	createTexture("./Resources/Characters/Punk/Punk_attack3.png", "punk_attack3", i++);
	createTexture("./Resources/Characters/Punk/Punk_climb.png", "punk_climb", i++);
	createTexture("./Resources/Characters/Punk/Punk_death.png", "punk_death", i++);
	createTexture("./Resources/Characters/Punk/Punk_doublejump.png", "punk_doublejump", i++);
	createTexture("./Resources/Characters/Punk/Punk_hurt.png", "punk_hurt", i++);
	createTexture("./Resources/Characters/Punk/Punk_idle.png", "punk_idle", i++);
	createTexture("./Resources/Characters/Punk/Punk_jump.png", "punk_jump", i++);
	createTexture("./Resources/Characters/Punk/Punk_punch.png", "punk_punch", i++);
	createTexture("./Resources/Characters/Punk/Punk_run.png", "punk_run", i++);
	createTexture("./Resources/Characters/Punk/Punk_run_attack.png", "punk_run_attack", i++);
	
	//Cyborg
	createTexture("./Resources/Characters/Cyborg/Cyborg_attack1.png", "cyborg_attack1", i++);
	createTexture("./Resources/Characters/Cyborg/Cyborg_attack2.png", "cyborg_attack2", i++);
	createTexture("./Resources/Characters/Cyborg/Cyborg_attack3.png", "cyborg_attack3", i++);
	createTexture("./Resources/Characters/Cyborg/Cyborg_climb.png", "cyborg_climb", i++);
	createTexture("./Resources/Characters/Cyborg/Cyborg_death.png", "cyborg_death", i++);
	createTexture("./Resources/Characters/Cyborg/Cyborg_doublejump.png", "cyborg_doublejump", i++);
	createTexture("./Resources/Characters/Cyborg/Cyborg_hurt.png", "cyborg_hurt", i++);
	createTexture("./Resources/Characters/Cyborg/Cyborg_idle.png", "cyborg_idle", i++);
	createTexture("./Resources/Characters/Cyborg/Cyborg_jump.png", "cyborg_jump", i++);
	createTexture("./Resources/Characters/Cyborg/Cyborg_punch.png", "cyborg_punch", i++);
	createTexture("./Resources/Characters/Cyborg/Cyborg_run.png", "cyborg_run", i++);
	createTexture("./Resources/Characters/Cyborg/Cyborg_run_attack.png", "cyborg_run_attack", i++);

	//Tiles
	createTexture("./Resources/Components/Tiles/Tile_05.png", "tileTop", i++);
	createTexture("./Resources/Components/Tiles/Tile_06.png", "tileLeft", i++);
	createTexture("./Resources/Components/Tiles/Tile_07.png", "tileMiddle", i++);
	createTexture("./Resources/Components/Tiles/Tile_08.png", "tileRight", i++);

	//Background Day
	createTexture("./Resources/Components/Background/Day/1.png", "DayPart1", i++);
	createTexture("./Resources/Components/Background/Day/2.png", "DayPart2", i++);
	createTexture("./Resources/Components/Background/Day/3.png", "DayPart3", i++);
	createTexture("./Resources/Components/Background/Day/4.png", "DayPart4", i++);
	createTexture("./Resources/Components/Background/Day/5.png", "DayPart5", i++);

	//Background Night
	createTexture("./Resources/Components/Background/Night/1.png","NightPart1", i++);
	createTexture("./Resources/Components/Background/Night/2.png","NightPart2", i++);
	createTexture("./Resources/Components/Background/Night/3.png","NightPart3", i++);
	createTexture("./Resources/Components/Background/Night/4.png","NightPart4", i++);
	createTexture("./Resources/Components/Background/Night/5.png","NightPart5", i++);

	//Background Illumination
	createTexture("./Resources/Components/Background/Overlay_illumination.png", "Illumination", 18);
}

void animationAttack1() {
	player.texture = textures[player.character + "_attack1"];
	sprites = 6;
	//currentSprite = 0;
}
void animationAttack2() {
	player.texture = textures[player.character + "_attack2"];
	sprites = 8;
	//currentSprite = 0;
}
void animationAttack3() {
	player.texture = textures[player.character + "_attack3"];
	sprites = 8;
	//currentSprite = 0;
}
void animationClimb() {
	player.texture = textures[player.character + "_climb"];
	sprites = 6;
	//currentSprite = 0;
}
void animationDeath() {
	player.texture = textures[player.character + "_death"];
	sprites = 6;
	//currentSprite = 0;
}
void animationDoublejump() {
	player.texture = textures[player.character + "_doublejump"];
	sprites = 6;
	//currentSprite = 0;
}
void animationHurt() {
	player.texture = textures[player.character + "_hurt"];
	sprites = 2;
	//currentSprite = 0;
}
void animationIdle() {
	player.texture = textures[player.character + "_idle"];
	sprites = 4;
	//currentSprite = 0;
}
void animationJump() {
	player.texture = textures[player.character + "_jump"];
	sprites = 4;
	//currentSprite = 0;
}
void animationPunch() {
	player.texture = textures[player.character + "_punch"];
	sprites = 6;
	//currentSprite = 0;
}
void animationRun() {
	player.texture = textures[player.character + "_run"];
	sprites = 6;
	//currentSprite = 0;
}
void animationRunAttack() {
	player.texture = textures[player.character + "_run_attack"];
	sprites = 6;
	//currentSprite = 0;
}
