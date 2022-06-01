#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <time.h>

#include "Object.h"
#include "Character.h"
#include "ImageLoader.h"
#include "lodepng.h"
#include "picopng.h"
#include "WorldPhysics.h"
#include "Platform.h"
#include "Collectible.h"

float movementX = 0.0;
float movementY = 0.0;
float spin = 0.0;
double red = 0.0;
double green = 0.0;
double blue = 0.0;

int screenWidth = 576*2;
int screenHeight = 324*2;

std::string currentMenu = "menu";
void menuPage_mouseHandler(int, int);
void characterSelection_mouseHandler(int, int);

Character player(48.0, 200.0, 200.0, 20.0, 150.0, 5.0, "biker"); // biker / punk / cyborg

Collectible money(100, 100);

/* CHARACTER STATS */
// { TOTALHEALTH, HEALTH, DAMAGE, JUMP, SPEED }
float bikerStats[5] = { 200.0, 200.0,  20.0, 175.0, 5.0 };
float punkStats[5] = { 130.0, 130.0,  25.0, 200.0, 7.0 };
float cyborgStats[5] = { 200.0, 200.0,  35.0, 150.0, 3.5 };

float startingY = 0.0;
void checkGround();
void checkCollectible();
//int sprites = 8;
//int currentSprite = 0;
void animationAttack1();
void animationAttack2();
void animationAttack3();
void animationClimb();
void animationDeath();
void animationDoublejump();
void animationHurt();
void animationIdle();
void animationJump();
void animationFalling();
void animationPunch();
void animationRun();
void animationRunAttack();

//HUD START
Object HUDStats_box;
Object HUDStats_box_detail;
Object HUDStats_character_box;
Character HUDStats_character;
Object HUDStats_health_box;
Object HUDStats_health_percent;

Object HUDScore_box;
Object HUDScore_box_detail;
Object HUDScore_score;
int score = 0;
//HUD END

//PLATFORMS START
Platform ground(-screenWidth / 2, -screenHeight / 2 + 32, screenWidth / 32);

Platform platformLeftLeft(-screenWidth / 2 + 100, -screenHeight / 2 + 32 + 125, 3);
Platform platformLeftRight(-screenWidth / 2 + 100 + 300, -screenHeight / 2 + 32 + 125, 3);

Platform platformRightLeft(screenWidth / 2 - 200 - 128 - 128 - 64, -screenHeight / 2 + 32 + 125*3, 3);
Platform platformRightCenter(screenWidth / 2 - 200 - 128, -screenHeight / 2 + 32 + 125*2, 3);
Platform platformRightRight(screenWidth / 2 - 200 + 64, -screenHeight / 2 + 32 + 125*1, 5);

Platform platformTopLeft(-screenWidth / 2, -screenHeight / 2 + 32 + 125 * 3, 10);
Platform platformTopRight(screenWidth / 2 - 200 - 64, -screenHeight / 2 + 32 + 125 * 4, 10);
//PLATFORMS END

//BACKGROUND START
Object bgPart1;
Object bgPart2;
Object bgPart3;
Object bgPart4;
Object bgPart5;
Object bgIllumination;
//BACKGROUND END

//OBJECTS ON THE MAP
Object fountain(72+200, -screenHeight/2+32, 0+200, -screenHeight / 2 + 32 + 72);

Object treeRight(screenWidth / 2 - 175 * 1 + 50, -screenHeight / 2 + 32 + 125*1, screenWidth / 2 + 50, -screenHeight / 2 + 32 + 125*1 + 190*1);

Object fountainTreeLeft(110 , -screenHeight / 2 + 32, 110 + 124, -screenHeight / 2 + 32 + 129 * 1);
Object fountainTreeRight(260, -screenHeight / 2 + 32, 260 + 62, -screenHeight / 2 + 32 + 103 * 1);

Object fence10(-screenWidth / 2 + 48 * 1, -screenHeight / 2 + 32 + 64 * 0, -screenWidth / 2 + 48 * 0, -screenHeight / 2 + 32 + 64 * 1);
Object fence11(-screenWidth / 2 + 48 * 1, -screenHeight / 2 + 32 + 64 * 1, -screenWidth / 2 + 48 * 0, -screenHeight / 2 + 32 + 64 * 2);
Object fence12(-screenWidth / 2 + 48 * 1, -screenHeight / 2 + 32 + 64 * 2, -screenWidth / 2 + 48 * 0, -screenHeight / 2 + 32 + 64 * 3);
Object fence13(-screenWidth / 2 + 48 * 1, -screenHeight / 2 + 32 + 64 * 3, -screenWidth / 2 + 48 * 0, -screenHeight / 2 + 32 + 64 * 4);

Object fence20(-screenWidth / 2 + 48 * 2, -screenHeight / 2 + 32 + 64 * 0, -screenWidth / 2 + 48 * 1, -screenHeight / 2 + 32 + 64 * 1);
Object fence21(-screenWidth / 2 + 48 * 2, -screenHeight / 2 + 32 + 64 * 1, -screenWidth / 2 + 48 * 1, -screenHeight / 2 + 32 + 64 * 2);
Object fence22(-screenWidth / 2 + 48 * 2, -screenHeight / 2 + 32 + 64 * 2, -screenWidth / 2 + 48 * 1, -screenHeight / 2 + 32 + 64 * 3);
Object fence23(-screenWidth / 2 + 48 * 2, -screenHeight / 2 + 32 + 64 * 3, -screenWidth / 2 + 48 * 1, -screenHeight / 2 + 32 + 64 * 4);

Object fence30(-screenWidth / 2 + 48 * 3, -screenHeight / 2 + 32 + 64 * 0, -screenWidth / 2 + 48 * 2, -screenHeight / 2 + 32 + 64 * 1);
Object fence31(-screenWidth / 2 + 48 * 3, -screenHeight / 2 + 32 + 64 * 1, -screenWidth / 2 + 48 * 2, -screenHeight / 2 + 32 + 64 * 2);
Object fence32(-screenWidth / 2 + 48 * 3, -screenHeight / 2 + 32 + 64 * 2, -screenWidth / 2 + 48 * 2, -screenHeight / 2 + 32 + 64 * 3);
Object fence33(-screenWidth / 2 + 48 * 3, -screenHeight / 2 + 32 + 64 * 3, -screenWidth / 2 + 48 * 2, -screenHeight / 2 + 32 + 64 * 4);

Object fence40(-screenWidth / 2 + 48 * 4, -screenHeight / 2 + 32 + 64 * 0, -screenWidth / 2 + 48 * 3, -screenHeight / 2 + 32 + 64 * 1);
Object fence41(-screenWidth / 2 + 48 * 4, -screenHeight / 2 + 32 + 64 * 1, -screenWidth / 2 + 48 * 3, -screenHeight / 2 + 32 + 64 * 2);
Object fence42(-screenWidth / 2 + 48 * 4, -screenHeight / 2 + 32 + 64 * 2, -screenWidth / 2 + 48 * 3, -screenHeight / 2 + 32 + 64 * 3);
Object fence43(-screenWidth / 2 + 48 * 4, -screenHeight / 2 + 32 + 64 * 3, -screenWidth / 2 + 48 * 3, -screenHeight / 2 + 32 + 64 * 4);

Object fence50(-screenWidth / 2 + 48 * 5, -screenHeight / 2 + 32 + 64 * 0, -screenWidth / 2 + 48 * 4, -screenHeight / 2 + 32 + 64 * 1);
Object fence51(-screenWidth / 2 + 48 * 5, -screenHeight / 2 + 32 + 64 * 1, -screenWidth / 2 + 48 * 4, -screenHeight / 2 + 32 + 64 * 2);
Object fence52(-screenWidth / 2 + 48 * 5, -screenHeight / 2 + 32 + 64 * 2, -screenWidth / 2 + 48 * 4, -screenHeight / 2 + 32 + 64 * 3);
Object fence53(-screenWidth / 2 + 48 * 5, -screenHeight / 2 + 32 + 64 * 3, -screenWidth / 2 + 48 * 4, -screenHeight / 2 + 32 + 64 * 4);

Object fence60(-screenWidth / 2 + 48 * 6, -screenHeight / 2 + 32 + 64 * 0, -screenWidth / 2 + 48 * 5, -screenHeight / 2 + 32 + 64 * 1);
Object fence61(-screenWidth / 2 + 48 * 6, -screenHeight / 2 + 32 + 64 * 1, -screenWidth / 2 + 48 * 5, -screenHeight / 2 + 32 + 64 * 2);
Object fence62(-screenWidth / 2 + 48 * 6, -screenHeight / 2 + 32 + 64 * 2, -screenWidth / 2 + 48 * 5, -screenHeight / 2 + 32 + 64 * 3);
Object fence63(-screenWidth / 2 + 48 * 6, -screenHeight / 2 + 32 + 64 * 3, -screenWidth / 2 + 48 * 5, -screenHeight / 2 + 32 + 64 * 4);

Object fence70(-screenWidth / 2 + 48 * 7, -screenHeight / 2 + 32 + 64 * 0, -screenWidth / 2 + 48 * 6, -screenHeight / 2 + 32 + 64 * 1);
Object fence71(-screenWidth / 2 + 48 * 7, -screenHeight / 2 + 32 + 64 * 1, -screenWidth / 2 + 48 * 6, -screenHeight / 2 + 32 + 64 * 2);
Object fence72(-screenWidth / 2 + 48 * 7, -screenHeight / 2 + 32 + 64 * 2, -screenWidth / 2 + 48 * 6, -screenHeight / 2 + 32 + 64 * 3);
Object fence73(-screenWidth / 2 + 48 * 7, -screenHeight / 2 + 32 + 64 * 3, -screenWidth / 2 + 48 * 6, -screenHeight / 2 + 32 + 64 * 4);

Object fence80(-screenWidth / 2 + 48 * 8, -screenHeight / 2 + 32 + 64 * 0, -screenWidth / 2 + 48 * 7, -screenHeight / 2 + 32 + 64 * 1);
Object fence81(-screenWidth / 2 + 48 * 8, -screenHeight / 2 + 32 + 64 * 1, -screenWidth / 2 + 48 * 7, -screenHeight / 2 + 32 + 64 * 2);
Object fence82(-screenWidth / 2 + 48 * 8, -screenHeight / 2 + 32 + 64 * 2, -screenWidth / 2 + 48 * 7, -screenHeight / 2 + 32 + 64 * 3);
Object fence83(-screenWidth / 2 + 48 * 8, -screenHeight / 2 + 32 + 64 * 3, -screenWidth / 2 + 48 * 7, -screenHeight / 2 + 32 + 64 * 4);

Object fence90(-screenWidth / 2 + 48 * 9, -screenHeight / 2 + 32 + 64 * 0, -screenWidth / 2 + 48 * 8, -screenHeight / 2 + 32 + 64 * 1);
Object fence91(-screenWidth / 2 + 48 * 9, -screenHeight / 2 + 32 + 64 * 1, -screenWidth / 2 + 48 * 8, -screenHeight / 2 + 32 + 64 * 2);
Object fence92(-screenWidth / 2 + 48 * 9, -screenHeight / 2 + 32 + 64 * 2, -screenWidth / 2 + 48 * 8, -screenHeight / 2 + 32 + 64 * 3);
Object fence93(-screenWidth / 2 + 48 * 9, -screenHeight / 2 + 32 + 64 * 3, -screenWidth / 2 + 48 * 8, -screenHeight / 2 + 32 + 64 * 4);

Object fenceA0(-screenWidth / 2 + 48 * 10, -screenHeight / 2 + 32 + 64 * 0, -screenWidth / 2 + 48 * 9, -screenHeight / 2 + 32 + 64 * 1);
Object fenceA1(-screenWidth / 2 + 48 * 10, -screenHeight / 2 + 32 + 64 * 1, -screenWidth / 2 + 48 * 9, -screenHeight / 2 + 32 + 64 * 2);
Object fenceA2(-screenWidth / 2 + 48 * 10, -screenHeight / 2 + 32 + 64 * 2, -screenWidth / 2 + 48 * 9, -screenHeight / 2 + 32 + 64 * 3);
Object fenceA3(-screenWidth / 2 + 48 * 10, -screenHeight / 2 + 32 + 64 * 3, -screenWidth / 2 + 48 * 9, -screenHeight / 2 + 32 + 64 * 4);

Object fenceB0(-screenWidth / 2 + 48 * 11, -screenHeight / 2 + 32 + 64 * 0, -screenWidth / 2 + 48 * 10, -screenHeight / 2 + 32 + 64 * 1);
Object fenceB1(-screenWidth / 2 + 48 * 11, -screenHeight / 2 + 32 + 64 * 1, -screenWidth / 2 + 48 * 10, -screenHeight / 2 + 32 + 64 * 2);
Object fenceB2(-screenWidth / 2 + 48 * 11, -screenHeight / 2 + 32 + 64 * 2, -screenWidth / 2 + 48 * 10, -screenHeight / 2 + 32 + 64 * 3);
Object fenceB3(-screenWidth / 2 + 48 * 11, -screenHeight / 2 + 32 + 64 * 3, -screenWidth / 2 + 48 * 10, -screenHeight / 2 + 32 + 64 * 4);

Object fenceC0(-screenWidth / 2 + 48 * 12, -screenHeight / 2 + 32 + 64 * 0, -screenWidth / 2 + 48 * 11, -screenHeight / 2 + 32 + 64 * 1);
Object fenceC1(-screenWidth / 2 + 48 * 12, -screenHeight / 2 + 32 + 64 * 1, -screenWidth / 2 + 48 * 11, -screenHeight / 2 + 32 + 64 * 2);
Object fenceC2(-screenWidth / 2 + 48 * 12, -screenHeight / 2 + 32 + 64 * 2, -screenWidth / 2 + 48 * 11, -screenHeight / 2 + 32 + 64 * 3);
Object fenceC3(-screenWidth / 2 + 48 * 12, -screenHeight / 2 + 32 + 64 * 3, -screenWidth / 2 + 48 * 11, -screenHeight / 2 + 32 + 64 * 4);

//

// MENU START
float menuLeftAxis = 0;
float menuRightAxis = 0;
float menuCenterAxis = 0;

float buttonLeftMargin = 0;
float buttonRightMargin = 0;

float buttonSpacing = 0;
float buttonPadding = 0;

int buttonColors[3] = { 100, 100, 255 };

Object menuOption1Box;
Object menuOption2Box;
Object menuOption3Box;
Object menuOption4Box;
// MENU END

/* CHARACTER PAGE START
	1. Select character
	2. See character animations
	3. See character stats */

Object characterSelectButton_biker;
Object characterSelectButton_punk;
Object characterSelectButton_cyborg;

Object characterSelect_stats_health_background;
Object characterSelect_stats_damage_background;
Object characterSelect_stats_jumpPower_background;
Object characterSelect_stats_speed_background;

Object characterSelect_stats_health;
Object characterSelect_stats_damage;
Object characterSelect_stats_jumpPower;
Object characterSelect_stats_speed;

Object characterSelectButton_idle;
Object characterSelectButton_attack1;
Object characterSelectButton_attack2;
Object characterSelectButton_attack3;
Object characterSelectButton_climb;
Object characterSelectButton_death;
Object characterSelectButton_jump;
Object characterSelectButton_doubleJump;
Object characterSelectButton_hurt;
Object characterSelectButton_punch;
Object characterSelectButton_run;
Object characterSelectButton_runAttack;

/* CHARACTER PAGE END */

bool keyStates[256] = { 0 };

void initWindow(void);

void initMenu(void);
void displayMenu(void);

void initGame(void);
void displayGame(void);

void initCharacterSelection(void);
void displayCharacterSelection(void);

void reshape(int w, int h);
void spinDisplay(void);

void gravity();
float gravityForce = 3.0;
bool gravityActive = false;

void controls();
bool controlsActive = false;

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

void generate_randomCoords();

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow("Platformer");

	srand((unsigned)time(NULL));
	initWindow();
	initMenu();
	

	glutDisplayFunc(displayMenu);
	//glutDisplayFunc(displayGame);

	glutReshapeFunc(reshape);

	glutMouseFunc(mouse);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

	glutTimerFunc(1000/120, timer, 0);

	glutMainLoop();
	return 0;
}

void initWindow(void) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glPointSize(1.0);
	glShadeModel(GL_SMOOTH);
	gluOrtho2D(-screenWidth / 2, screenWidth / 2, -screenHeight / 2, screenHeight / 2);
}

void initMenu(void) {
	glClearColor(0.2, 0.2, 0.2, 1.0);

	initButtons();
	initEntities();

	player.size = 48.0;

	gravityActive = false;
	controlsActive = false;

	menuLeftAxis = -screenWidth / 6;
	menuRightAxis = screenWidth / 6;
	menuCenterAxis = abs(menuLeftAxis) - abs(menuRightAxis);

	buttonLeftMargin = -screenWidth / 9;
	buttonRightMargin = screenWidth / 9;

	buttonSpacing = 1.5;
	buttonPadding = buttonSpacing * 6;

	menuOption1Box.coords[0] = buttonLeftMargin;
	menuOption1Box.coords[1] = -0 * 18 * 2 * buttonSpacing - buttonPadding;
	menuOption1Box.coords[2] = buttonRightMargin;
	menuOption1Box.coords[3] = -0 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	menuOption1Box.color[0] = buttonColors[0];
	menuOption1Box.color[1] = buttonColors[1];
	menuOption1Box.color[2] = buttonColors[2];

	menuOption2Box.coords[0] = buttonLeftMargin;
	menuOption2Box.coords[1] = -1 * 18 * 2 * buttonSpacing - buttonPadding;
	menuOption2Box.coords[2] = buttonRightMargin;
	menuOption2Box.coords[3] = -1 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	menuOption2Box.color[0] = buttonColors[0];
	menuOption2Box.color[1] = buttonColors[1];
	menuOption2Box.color[2] = buttonColors[2];

	menuOption3Box.coords[0] = buttonLeftMargin;
	menuOption3Box.coords[1] = -2 * 18 * 2 * buttonSpacing - buttonPadding;
	menuOption3Box.coords[2] = buttonRightMargin;
	menuOption3Box.coords[3] = -2 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	menuOption3Box.color[0] = buttonColors[0];
	menuOption3Box.color[1] = buttonColors[1];
	menuOption3Box.color[2] = buttonColors[2];

	menuOption4Box.coords[0] = buttonLeftMargin;
	menuOption4Box.coords[1] = -3 * 18 * 2 * buttonSpacing - buttonPadding;
	menuOption4Box.coords[2] = buttonRightMargin;
	menuOption4Box.coords[3] = -3 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	menuOption4Box.color[0] = buttonColors[0];
	menuOption4Box.color[1] = buttonColors[1];
	menuOption4Box.color[2] = buttonColors[2];
	
	addButtons(menuOption1Box.coords);
	addButtons(menuOption2Box.coords);
	addButtons(menuOption3Box.coords);
	addButtons(menuOption4Box.coords);

	//printButtons();
}

void initGame(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);

	score = 0;

	gravityActive = true;
	controlsActive = true;

	loadTextures();

	initButtons();
	initEntities();

	player.size = 48.0;
	player.health = player.totalHealth;
	player.move(0, 0);
	movementX = 0;
	movementY = 0;
	
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

	/* HUD STATS */
	HUDStats_box.coords[0] = -screenWidth / 2;
	HUDStats_box.coords[1] = screenHeight / 2;
	HUDStats_box.coords[2] = -screenWidth / 4;
	HUDStats_box.coords[3] = screenHeight / 2 - 42;
	HUDStats_box.color[0] = 43;
	HUDStats_box.color[1] = 43;
	HUDStats_box.color[2] = 43;

	HUDStats_character_box.coords[0] = HUDStats_box.coords[0] + 5;
	HUDStats_character_box.coords[1] = HUDStats_box.coords[1] - 5;
	HUDStats_character_box.coords[2] = HUDStats_character_box.coords[0] + 32;
	HUDStats_character_box.coords[3] = HUDStats_character_box.coords[1] - 32;
	HUDStats_character_box.color[0] = 0.2;
	HUDStats_character_box.color[1] = 0.2;
	HUDStats_character_box.color[2] = 0.2;

	HUDStats_character.size = 36;
	HUDStats_character.character = player.character;
	HUDStats_character.animation = "_idle";
	HUDStats_character.currentSprite = 0;
	HUDStats_character.sprites = 4;
	HUDStats_character.texture = textures[HUDStats_character.character + HUDStats_character.animation];
	HUDStats_character.move(-screenWidth / 2 + HUDStats_character.size/1.5 + 5, screenHeight / 2 - HUDStats_character.size / 2 + 1);

	HUDStats_health_box.coords[0] = HUDStats_character_box.coords[2] + 5;
	HUDStats_health_box.coords[1] = HUDStats_box.coords[1] - 5;
	HUDStats_health_box.coords[2] = HUDStats_box.coords[2] - 5;
	HUDStats_health_box.coords[3] = HUDStats_character_box.coords[1] - 32;
	HUDStats_health_box.color[0] = 0.2;
	HUDStats_health_box.color[1] = 0.2;
	HUDStats_health_box.color[2] = 0.2;

	HUDStats_health_percent.coords[0] = HUDStats_health_box.coords[0] + 5;
	HUDStats_health_percent.coords[1] = HUDStats_health_box.coords[1] - 5;
	HUDStats_health_percent.coords[2] = HUDStats_health_box.coords[2] - 5;
	HUDStats_health_percent.coords[3] = HUDStats_health_box.coords[3] + 5;

	/* HUD SCORE */
	HUDScore_box.coords[0] = screenWidth / 2;
	HUDScore_box.coords[1] = screenHeight / 2;
	HUDScore_box.coords[2] = screenWidth / 3;
	HUDScore_box.coords[3] = screenHeight / 2 - 42;
	HUDScore_box.color[0] = 43;
	HUDScore_box.color[1] = 43;
	HUDScore_box.color[2] = 43;

	HUDScore_score.coords[0] = HUDScore_box.coords[0] - 5;
	HUDScore_score.coords[1] = HUDScore_box.coords[1] - 5;
	HUDScore_score.coords[2] = HUDScore_box.coords[0] + (HUDScore_box.coords[1] - HUDScore_box.coords[0]) / 2;
	HUDScore_score.coords[3] = HUDScore_box.coords[3] + 5;
	HUDScore_score.color[0] = 0.2;
	HUDScore_score.color[1] = 0.2;
	HUDScore_score.color[2] = 0.2;

	/* GROUND */
	ground.texture[0] = textures["tileLeft"];
	ground.texture[1] = textures["tileMiddle"];
	ground.texture[2] = textures["tileRight"];
	ground.calcHitbox();
	addEntities(ground.hitboxCoords);

	/* PLATFORMS */
	platformLeftLeft.texture[0] = textures["tileLeft"];
	platformLeftLeft.texture[1] = textures["tileMiddle"];
	platformLeftLeft.texture[2] = textures["tileRight"];
	platformLeftLeft.calcHitbox();
	addEntities(platformLeftLeft.hitboxCoords);

	platformLeftRight.texture[0] = textures["tileLeft"];
	platformLeftRight.texture[1] = textures["tileMiddle"];
	platformLeftRight.texture[2] = textures["tileRight"];
	platformLeftRight.calcHitbox();
	addEntities(platformLeftRight.hitboxCoords);

	platformRightRight.texture[0] = textures["tileLeft"];
	platformRightRight.texture[1] = textures["tileMiddle"];
	platformRightRight.texture[2] = textures["tileRight"];
	platformRightRight.calcHitbox();
	addEntities(platformRightRight.hitboxCoords);

	platformRightLeft.texture[0] = textures["tileLeft"];
	platformRightLeft.texture[1] = textures["tileMiddle"];
	platformRightLeft.texture[2] = textures["tileRight"];
	platformRightLeft.calcHitbox();
	addEntities(platformRightLeft.hitboxCoords);

	platformRightCenter.texture[0] = textures["tileLeft"];
	platformRightCenter.texture[1] = textures["tileMiddle"];
	platformRightCenter.texture[2] = textures["tileRight"];
	platformRightCenter.calcHitbox();
	addEntities(platformRightCenter.hitboxCoords);

	platformTopLeft.texture[0] = textures["tileLeft"];
	platformTopLeft.texture[1] = textures["tileMiddle"];
	platformTopLeft.texture[2] = textures["tileRight"];
	platformTopLeft.calcHitbox();
	addEntities(platformTopLeft.hitboxCoords);

	platformTopRight.texture[0] = textures["tileLeft"];
	platformTopRight.texture[1] = textures["tileMiddle"];
	platformTopRight.texture[2] = textures["tileRight"];
	platformTopRight.calcHitbox();
	addEntities(platformTopRight.hitboxCoords);

	//OBJECTS ON THE MAP
	fountain.currentSprite = 0;
	fountain.sprites = 4;
	fountain.texture = textures["animated_fountain"];

	treeRight.texture = textures["tree_with_swing"];

	fountainTreeLeft.texture = textures["tree2"];
	fountainTreeRight.texture = textures["tree1"];

	fence10.texture = textures["fence"];
	fence11.texture = textures["fence"];
	fence12.texture = textures["fence"];
	fence13.texture = textures["fence"];

	fence20.texture = textures["fence"];
	fence21.texture = textures["fence"];
	fence22.texture = textures["fence"];
	fence23.texture = textures["fence"];

	fence30.texture = textures["fence"];
	fence31.texture = textures["fence"];
	fence32.texture = textures["fence"];
	fence33.texture = textures["fence"];

	fence40.texture = textures["fence"];
	fence41.texture = textures["fence"];
	fence42.texture = textures["fence"];
	fence43.texture = textures["fence"];

	fence50.texture = textures["fence"];
	fence51.texture = textures["fence"];
	fence52.texture = textures["fence"];
	fence53.texture = textures["fence"];

	fence60.texture = textures["fence"];
	fence61.texture = textures["fence"];
	fence62.texture = textures["fence"];
	fence63.texture = textures["fence"];

	fence70.texture = textures["fence"];
	fence71.texture = textures["fence"];
	fence72.texture = textures["fence"];
	fence73.texture = textures["fence"];

	fence80.texture = textures["fence"];
	fence81.texture = textures["fence"];
	fence82.texture = textures["fence"];
	fence83.texture = textures["fence"];

	fence90.texture = textures["fence"];
	fence91.texture = textures["fence"];
	fence92.texture = textures["fence"];
	fence93.texture = textures["fence"];

	fenceA0.texture = textures["fence"];
	fenceA1.texture = textures["fence"];
	fenceA2.texture = textures["fence"];
	fenceA3.texture = textures["fence"];

	fenceB0.texture = textures["fence"];
	fenceB1.texture = textures["fence"];
	fenceB2.texture = textures["fence"];
	fenceB3.texture = textures["fence"];

	fenceC0.texture = textures["fence"];
	fenceC1.texture = textures["fence"];
	fenceC2.texture = textures["fence"];
	fenceC3.texture = textures["fence"];

	//
	money.currentSprite = 0;
	money.sprites = 6;
	money.texture = textures["money"];
	addCollectibles(money.coords);

	//printEntities();
}

void initCharacterSelection() {

	initButtons();
	initEntities();

	loadTextures();

	animationIdle();

	gravityActive = false;
	controlsActive = false;

	player.size = player.size * 5;
	player.direction = 'r';
	glClearColor(0.2, 0.2, 0.2, 1.0);

	menuLeftAxis = 0;
	menuRightAxis = screenWidth / 2;
	menuCenterAxis = (abs(menuRightAxis) - abs(menuLeftAxis)) / 2;

	buttonLeftMargin = 0 + screenWidth / 9;
	buttonRightMargin = screenWidth / 2 - screenWidth / 9;

	buttonSpacing = 1.5;
	buttonPadding = buttonSpacing * 6;

	characterSelectButton_biker.coords[0] = buttonLeftMargin;
	characterSelectButton_biker.coords[1] = 3 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_biker.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 - 10;
	characterSelectButton_biker.coords[3] = 3 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_biker.color[0] = buttonColors[0];
	characterSelectButton_biker.color[1] = buttonColors[1];
	characterSelectButton_biker.color[2] = buttonColors[2];

	characterSelectButton_punk.coords[0] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 + 5;
	characterSelectButton_punk.coords[1] = 3 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_punk.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 2 - 5;
	characterSelectButton_punk.coords[3] = 3 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_punk.color[0] = buttonColors[0];
	characterSelectButton_punk.color[1] = buttonColors[1];
	characterSelectButton_punk.color[2] = buttonColors[2];

	characterSelectButton_cyborg.coords[0] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 2 + 10;
	characterSelectButton_cyborg.coords[1] = 3 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_cyborg.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 3;
	characterSelectButton_cyborg.coords[3] = 3 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_cyborg.color[0] = buttonColors[0];
	characterSelectButton_cyborg.color[1] = buttonColors[1];
	characterSelectButton_cyborg.color[2] = buttonColors[2];

	/* CHARACTER STATUS */
	// stats Background

	characterSelect_stats_health_background.coords[0] = buttonLeftMargin;
	characterSelect_stats_health_background.coords[1] = 2 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelect_stats_health_background.coords[2] = buttonRightMargin;
	characterSelect_stats_health_background.coords[3] = 2 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelect_stats_health_background.color[0] = 0.2;
	characterSelect_stats_health_background.color[1] = 0.2;
	characterSelect_stats_health_background.color[2] = 0.2;

	characterSelect_stats_damage_background.coords[0] = buttonLeftMargin;
	characterSelect_stats_damage_background.coords[1] = 1 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelect_stats_damage_background.coords[2] = buttonRightMargin;
	characterSelect_stats_damage_background.coords[3] = 1 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelect_stats_damage_background.color[0] = 0.2;
	characterSelect_stats_damage_background.color[1] = 0.2;
	characterSelect_stats_damage_background.color[2] = 0.2;

	characterSelect_stats_jumpPower_background.coords[0] = buttonLeftMargin;
	characterSelect_stats_jumpPower_background.coords[1] = 0 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelect_stats_jumpPower_background.coords[2] = buttonRightMargin;
	characterSelect_stats_jumpPower_background.coords[3] = 0 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelect_stats_jumpPower_background.color[0] = 0.2;
	characterSelect_stats_jumpPower_background.color[1] = 0.2;
	characterSelect_stats_jumpPower_background.color[2] = 0.2;

	characterSelect_stats_speed_background.coords[0] = buttonLeftMargin;
	characterSelect_stats_speed_background.coords[1] = -1 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelect_stats_speed_background.coords[2] = buttonRightMargin;
	characterSelect_stats_speed_background.coords[3] = -1 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelect_stats_speed_background.color[0] = 0.2;
	characterSelect_stats_speed_background.color[1] = 0.2;
	characterSelect_stats_speed_background.color[2] = 0.2;

	// Stats lines

	characterSelect_stats_health.coords[0] = buttonLeftMargin + 10;
	characterSelect_stats_health.coords[1] = 2 * 18 * 2 * buttonSpacing - buttonPadding + 10;
	characterSelect_stats_health.coords[2] = buttonRightMargin - 10;
	characterSelect_stats_health.coords[3] = 2 * 18 * 2 * buttonSpacing + 14 + buttonPadding - 10;
	characterSelect_stats_health.color[0] = 0;
	characterSelect_stats_health.color[1] = 255;
	characterSelect_stats_health.color[2] = 0;

	characterSelect_stats_damage.coords[0] = buttonLeftMargin + 10;
	characterSelect_stats_damage.coords[1] = 1 * 18 * 2 * buttonSpacing - buttonPadding + 10;
	characterSelect_stats_damage.coords[2] = buttonRightMargin - 10;
	characterSelect_stats_damage.coords[3] = 1 * 18 * 2 * buttonSpacing + 14 + buttonPadding - 10;
	characterSelect_stats_damage.color[0] = 255;
	characterSelect_stats_damage.color[1] = 0;
	characterSelect_stats_damage.color[2] = 0;

	characterSelect_stats_jumpPower.coords[0] = buttonLeftMargin + 10;
	characterSelect_stats_jumpPower.coords[1] = 0 * 18 * 2 * buttonSpacing - buttonPadding + 10; 
	characterSelect_stats_jumpPower.coords[2] = buttonRightMargin - 10;
	characterSelect_stats_jumpPower.coords[3] = 0 * 18 * 2 * buttonSpacing + 14 + buttonPadding - 10;
	characterSelect_stats_jumpPower.color[0] = 255;
	characterSelect_stats_jumpPower.color[1] = 0;
	characterSelect_stats_jumpPower.color[2] = 255;

	characterSelect_stats_speed.coords[0] = buttonLeftMargin + 10;
	characterSelect_stats_speed.coords[1] = -1 * 18 * 2 * buttonSpacing - buttonPadding + 10;
	characterSelect_stats_speed.coords[2] = buttonRightMargin - 10;
	characterSelect_stats_speed.coords[3] = -1 * 18 * 2 * buttonSpacing + 14 + buttonPadding - 10;
	characterSelect_stats_speed.color[0] = 255;
	characterSelect_stats_speed.color[1] = 255;
	characterSelect_stats_speed.color[2] = 0;

	/* ANIMATION BUTTONS BOXES */
	characterSelectButton_attack1.coords[0] = buttonLeftMargin;
	characterSelectButton_attack1.coords[1] = -2 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_attack1.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 - 10;
	characterSelectButton_attack1.coords[3] = -2 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_attack1.color[0] = buttonColors[0];
	characterSelectButton_attack1.color[1] = buttonColors[1];
	characterSelectButton_attack1.color[2] = buttonColors[2];

	characterSelectButton_attack2.coords[0] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 + 5;
	characterSelectButton_attack2.coords[1] = -2 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_attack2.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 2 - 5;
	characterSelectButton_attack2.coords[3] = -2 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_attack2.color[0] = buttonColors[0];
	characterSelectButton_attack2.color[1] = buttonColors[1];
	characterSelectButton_attack2.color[2] = buttonColors[2];

	characterSelectButton_attack3.coords[0] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 2 + 10;
	characterSelectButton_attack3.coords[1] = -2 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_attack3.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 3;
	characterSelectButton_attack3.coords[3] = -2 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_attack3.color[0] = buttonColors[0];
	characterSelectButton_attack3.color[1] = buttonColors[1];
	characterSelectButton_attack3.color[2] = buttonColors[2];


	//

	characterSelectButton_climb.coords[0] = buttonLeftMargin;
	characterSelectButton_climb.coords[1] = -3 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_climb.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 - 10;
	characterSelectButton_climb.coords[3] = -3 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_climb.color[0] = buttonColors[0];
	characterSelectButton_climb.color[1] = buttonColors[1];
	characterSelectButton_climb.color[2] = buttonColors[2];

	characterSelectButton_jump.coords[0] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 + 5;
	characterSelectButton_jump.coords[1] = -3 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_jump.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 2 - 5;
	characterSelectButton_jump.coords[3] = -3 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_jump.color[0] = buttonColors[0];
	characterSelectButton_jump.color[1] = buttonColors[1];
	characterSelectButton_jump.color[2] = buttonColors[2];

	characterSelectButton_doubleJump.coords[0] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 2 + 10;
	characterSelectButton_doubleJump.coords[1] = -3 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_doubleJump.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 3;
	characterSelectButton_doubleJump.coords[3] = -3 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_doubleJump.color[0] = buttonColors[0];
	characterSelectButton_doubleJump.color[1] = buttonColors[1];
	characterSelectButton_doubleJump.color[2] = buttonColors[2];

	//

	characterSelectButton_death.coords[0] = buttonLeftMargin;
	characterSelectButton_death.coords[1] = -4 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_death.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 - 10;
	characterSelectButton_death.coords[3] = -4 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_death.color[0] = buttonColors[0];
	characterSelectButton_death.color[1] = buttonColors[1];
	characterSelectButton_death.color[2] = buttonColors[2];

	characterSelectButton_idle.coords[0] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 + 5;
	characterSelectButton_idle.coords[1] = -4 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_idle.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 2 - 5;
	characterSelectButton_idle.coords[3] = -4 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_idle.color[0] = buttonColors[0];
	characterSelectButton_idle.color[1] = buttonColors[1];
	characterSelectButton_idle.color[2] = buttonColors[2];

	characterSelectButton_hurt.coords[0] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 2 + 10;
	characterSelectButton_hurt.coords[1] = -4 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_hurt.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 3;
	characterSelectButton_hurt.coords[3] = -4 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_hurt.color[0] = buttonColors[0];
	characterSelectButton_hurt.color[1] = buttonColors[1];
	characterSelectButton_hurt.color[2] = buttonColors[2];

	//

	characterSelectButton_punch.coords[0] = buttonLeftMargin;
	characterSelectButton_punch.coords[1] = -5 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_punch.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 - 10;
	characterSelectButton_punch.coords[3] = -5 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_punch.color[0] = buttonColors[0];
	characterSelectButton_punch.color[1] = buttonColors[1];
	characterSelectButton_punch.color[2] = buttonColors[2];

	characterSelectButton_run.coords[0] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 + 5;
	characterSelectButton_run.coords[1] = -5 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_run.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 2 - 5;
	characterSelectButton_run.coords[3] = -5 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_run.color[0] = buttonColors[0];
	characterSelectButton_run.color[1] = buttonColors[1];
	characterSelectButton_run.color[2] = buttonColors[2];

	characterSelectButton_runAttack.coords[0] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 2 + 10;
	characterSelectButton_runAttack.coords[1] = -5 * 18 * 2 * buttonSpacing - buttonPadding;
	characterSelectButton_runAttack.coords[2] = buttonLeftMargin + (buttonRightMargin - buttonLeftMargin) / 3 * 3;
	characterSelectButton_runAttack.coords[3] = -5 * 18 * 2 * buttonSpacing + 14 + buttonPadding;
	characterSelectButton_runAttack.color[0] = buttonColors[0];
	characterSelectButton_runAttack.color[1] = buttonColors[1];
	characterSelectButton_runAttack.color[2] = buttonColors[2];

	addButtons(characterSelectButton_biker.coords);
	addButtons(characterSelectButton_punk.coords);
	addButtons(characterSelectButton_cyborg.coords);

	addButtons(characterSelectButton_attack1.coords);
	addButtons(characterSelectButton_attack2.coords);
	addButtons(characterSelectButton_attack3.coords);

	addButtons(characterSelectButton_climb.coords);
	addButtons(characterSelectButton_jump.coords);
	addButtons(characterSelectButton_doubleJump.coords);

	addButtons(characterSelectButton_death.coords);
	addButtons(characterSelectButton_idle.coords);
	addButtons(characterSelectButton_hurt.coords);

	addButtons(characterSelectButton_punch.coords);
	addButtons(characterSelectButton_run.coords);
	addButtons(characterSelectButton_runAttack.coords);

	//printButtons();
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
	if (!collisionUp(movementX, movementY, player) && player.isJumping && !player.isFalling) {
		if (player.animation != "_jump") animationJump();
		for (float i = 0; i <= player.maxJump/(player.maxJump/5); i += 0.1) {
			if (!collisionUp(movementX, movementY, player)) {
				gravityActive = false;
				movementY += 0.1;
				player.move(movementX, movementY);
				//printf("Starting Jump at: %0.1f (jumping: %d / falling: %d / gravity: %d)\n", startingY, player.isJumping, player.isFalling, gravityActive);
				if (movementY >= startingY + player.maxJump) {
					player.currentSprite = 2;
					if (player.animation != "_jump") animationFalling();
					gravityActive = true;
					player.isJumping = false;
					player.isFalling = true;
				}
			}
			else {
				player.currentSprite = 2;
				if (player.animation != "_jump") animationFalling();
				gravityActive = true;
				player.isJumping = false;
				player.isFalling = true;
			}
		}
	}
}

void displayMenu(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();

	glColor3ub(43, 43, 43);
	glBegin(GL_QUADS);
	glVertex2f(menuLeftAxis, -screenHeight / 2);
	glVertex2f(menuRightAxis, -screenHeight / 2);
	glVertex2f(menuRightAxis, screenHeight / 2);
	glVertex2f(menuLeftAxis, screenHeight / 2);
	glEnd();

	// MENU BUTTONS STRINGS
	std::string menuOption1 = "PLAY";
	float menuOption1Center = menuCenterAxis - 18 * (menuOption1.length() - 1) / 2 + 5;
	std::string menuOption2 = "CHARACTERS";
	float menuOption2Center = menuCenterAxis - 18 * (menuOption2.length() - 1) / 2 + 18;
	std::string menuOption3 = "SETTINGS";
	float menuOption3Center = menuCenterAxis - 18 * (menuOption3.length() - 1) / 2 + 18;
	std::string menuOption4 = "EXIT";
	float menuOption4Center = menuCenterAxis - 18 * (menuOption4.length() - 1) / 2 + 7;

	menuOption1Box.render3ub();
	menuOption2Box.render3ub();
	menuOption3Box.render3ub();
	menuOption4Box.render3ub();

	//RENDER MENU BUTTONS STRINGS
	glColor3ub(43, 43, 43);
	glRasterPos2i(menuOption1Center, 0);
	for (int i = 0; i < menuOption1.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, menuOption1[i]);
	}

	glRasterPos2i(menuOption2Center, -18 * 2 * buttonSpacing);
	for (int i = 0; i < menuOption2.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, menuOption2[i]);
	}

	glRasterPos2i(menuOption3Center, -18 * 4 * buttonSpacing);
	for (int i = 0; i < menuOption3.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, menuOption3[i]);
	}

	glRasterPos2i(menuOption4Center, -18 * 6 * buttonSpacing);
	for (int i = 0; i < menuOption4.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, menuOption4[i]);
	}

	glutPostRedisplay();
	glPopMatrix();
	glutSwapBuffers();

	//printf("X: %d / Y: %d\n", movementX, movementY);
}

void displayGame(void)
{ 
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();

	bgIllumination.renderTex();
	bgPart1.renderTex();
	bgPart2.renderTex();
	bgPart3.renderTex();
	bgPart4.renderTex();
	bgPart5.renderTex();

	HUDStats_box.render3ub();
	HUDStats_character_box.render3f();
	HUDStats_character.renderTex();

	if (HUDStats_character.animationRepeat()) HUDStats_character.animationCounter++;

	HUDStats_health_box.render3f();

	HUDStats_health_percent.coords[2] = HUDStats_health_box.coords[0] - 5 + (HUDStats_health_box.coords[2] - 5 - (HUDStats_health_box.coords[0] - 5)) / player.totalHealth * player.health;
	if (HUDStats_health_percent.coords[2] < HUDStats_health_percent.coords[0]) HUDStats_health_percent.coords[2] = HUDStats_health_percent.coords[0]+1;
	
	HUDStats_health_percent.color[0] = 255 - player.health / (player.totalHealth / 255);
	HUDStats_health_percent.color[1] = player.health / (player.totalHealth / 255);
	HUDStats_health_percent.color[2] = 0;

	HUDStats_health_percent.render3ub();

	std::string HUDStats_health_string = std::to_string((int)player.health) + "/" + std::to_string((int)player.totalHealth);
	glColor3ub(43, 43, 43);
	glRasterPos2i(HUDStats_health_box.coords[2] - (HUDStats_health_box.coords[2] - HUDStats_health_box.coords[0]) / 2 - 30, HUDStats_health_box.coords[3] + 10.5);
	for (int i = 0; i < HUDStats_health_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, HUDStats_health_string[i]);
	}

	HUDScore_box.render3ub();
	HUDScore_score.render3f();

	std::string HUDScore_string = "Score:";
	glColor3ub(250, 43, 120);
	glRasterPos2i(HUDScore_box.coords[2] + 5, HUDScore_box.coords[3] + 14);
	for (int i = 0; i < HUDScore_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, HUDScore_string[i]);
	}

	std::string HUDScore_score_string = std::to_string(score);
	glColor3ub(250, 43, 120);
	glRasterPos2i(HUDScore_score.coords[2] + 5, HUDScore_box.coords[3] + 14);
	for (int i = 0; i < HUDScore_score_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, HUDScore_score_string[i]);
	}

	if (player.health == 0) {
		std::string gameOver_string = "GAME OVER";
		glColor3ub(43, 43, 43);
		glRasterPos2i(-50, 0);
		for (int i = 0; i < gameOver_string.length(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, gameOver_string[i]);
		}
		controlsActive = false;
		if (player.animation != "_death") animationDeath();
		if (player.animationDoOnce()) player.animationCounter++;
	}
	else {
		if (!player.isMoving && !player.isAttacking && !player.isJumping && !player.isFalling && !player.isHurt) {
			if (player.animation != "_idle") animationIdle();
			if (player.animationRepeat()) player.animationCounter++;
			player.isMoving = false;
			player.isJumping = false;
			player.isFalling = false;
			player.isAttacking = false;
		}

		if (player.isAttacking) {
			if (player.animationDoOnce()) {
				//printf("\ncur:%d total:%d attacking:%d", player.currentSprite, player.sprites, player.isAttacking);
				controlsActive = false;
				player.animationCounter++;
			}
			else {
				player.currentSprite = 0;
				player.isAttacking = false;
				controlsActive = true;
			}
		}

		if (player.isMoving) {
			if (player.animationRepeat()) player.animationCounter++;
			else player.isMoving = false;
		}

		if (!player.isJumping && !collisionDown(movementX, movementY, player)) {
			if (player.animation != "_jump") animationFalling();
			player.isFalling = true;
			player.isFallingCounter++;
		}

		if (player.isHurt) {
			if (player.animation != "_hurt") animationHurt();
			if (player.animationDoOnce()) player.animationCounter++;
			else player.isHurt = false;
		}
	}

	ground.renderTex();
	platformLeftLeft.renderTex();
	platformLeftRight.renderTex();

	platformRightRight.renderTex();
	platformRightLeft.renderTex();
	platformRightCenter.renderTex();
	
	platformTopLeft.renderTex();
	platformTopRight.renderTex();

	treeRight.renderTex();

	fountainTreeLeft.renderTex();
	fountainTreeRight.renderTex();

	fountain.renderTex();
	if (fountain.animationRepeat()) fountain.animationCounter++;


	player.renderTex();

	money.renderTex();
	if (money.animationRepeat()) money.animationCounter++;

	fence10.renderTex();
	fence11.renderTex();
	fence12.renderTex();
	fence13.renderTex();

	fence20.renderTex();
	fence21.renderTex();
	fence22.renderTex();
	fence23.renderTex();

	fence30.renderTex();
	fence31.renderTex();
	fence32.renderTex();
	fence33.renderTex();

	fence40.renderTex();
	fence41.renderTex();
	fence42.renderTex();
	fence43.renderTex();

	fence50.renderTex();
	fence51.renderTex();
	fence52.renderTex();
	fence53.renderTex();

	fence60.renderTex();
	fence61.renderTex();
	fence62.renderTex();
	fence63.renderTex();

	fence70.renderTex();
	fence71.renderTex();
	fence72.renderTex();
	fence73.renderTex();

	fence80.renderTex();
	fence81.renderTex();
	fence82.renderTex();
	fence83.renderTex();

	fence90.renderTex();
	fence91.renderTex();
	fence92.renderTex();
	fence93.renderTex();

	fenceA0.renderTex();
	fenceA1.renderTex();
	fenceA2.renderTex();
	fenceA3.renderTex();

	fenceB0.renderTex();
	fenceB1.renderTex();
	fenceB2.renderTex();
	fenceB3.renderTex();

	fenceC0.renderTex();
	fenceC1.renderTex();
	fenceC2.renderTex();
	fenceC3.renderTex();


	glutPostRedisplay();
	glPopMatrix();
	glutSwapBuffers();

	//printf("X: %d / Y: %d\n", movementX, movementY);
}

void displayCharacterSelection(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();

	/* PLAYER ZONE */
	player.renderTex();
	//animationIdle();
	//animationDeath();
	if (player.animationRepeat()) ++player.animationCounter;

	player.move(-screenWidth / 4, 0);

	/* BUTTONS ZONE */
	glColor3ub(43, 43, 43);
	glBegin(GL_QUADS);
	glVertex2f(menuLeftAxis, -screenHeight / 2);
	glVertex2f(menuRightAxis, -screenHeight / 2);
	glVertex2f(menuRightAxis, screenHeight / 2);
	glVertex2f(menuLeftAxis, screenHeight / 2);
	glEnd();

	characterSelectButton_biker.render3ub();
	characterSelectButton_punk.render3ub();
	characterSelectButton_cyborg.render3ub();

	/* RENDER CHARACTER STATS */
	//stats background
	characterSelect_stats_health_background.render3f();
	characterSelect_stats_damage_background.render3f();
	characterSelect_stats_jumpPower_background.render3f();
	characterSelect_stats_speed_background.render3f();

	//stats
	float statsTotal = (buttonRightMargin - 10) - (buttonLeftMargin + 10);
	if (player.character == "biker") {
		characterSelect_stats_health.coords[2] = characterSelect_stats_health.coords[0] + statsTotal / 200 * bikerStats[1];
		characterSelect_stats_damage.coords[2] = characterSelect_stats_damage.coords[0] + statsTotal / 50 * bikerStats[2];
		characterSelect_stats_jumpPower.coords[2] = characterSelect_stats_jumpPower.coords[0] + statsTotal / 200 * bikerStats[3];
		characterSelect_stats_speed.coords[2] = characterSelect_stats_speed.coords[0] + statsTotal / 10 * bikerStats[4];
	} else if (player.character == "punk") {
		characterSelect_stats_health.coords[2] = characterSelect_stats_health.coords[0] + statsTotal / 200 * punkStats[1];
		characterSelect_stats_damage.coords[2] = characterSelect_stats_damage.coords[0] + statsTotal / 50 * punkStats[2];
		characterSelect_stats_jumpPower.coords[2] = characterSelect_stats_jumpPower.coords[0] + statsTotal / 200 * punkStats[3];
		characterSelect_stats_speed.coords[2] = characterSelect_stats_speed.coords[0] + statsTotal / 10 * punkStats[4];
	} else if (player.character == "cyborg") {
		characterSelect_stats_health.coords[2] = characterSelect_stats_health.coords[0] + statsTotal / 200 * cyborgStats[1];
		characterSelect_stats_damage.coords[2] = characterSelect_stats_damage.coords[0] + statsTotal / 50 * cyborgStats[2];
		characterSelect_stats_jumpPower.coords[2] = characterSelect_stats_jumpPower.coords[0] + statsTotal / 200 * cyborgStats[3];
		characterSelect_stats_speed.coords[2] = characterSelect_stats_speed.coords[0] + statsTotal / 10 * cyborgStats[4];
	}

	characterSelect_stats_health.render3ub();
	characterSelect_stats_damage.render3ub();
	characterSelect_stats_jumpPower.render3ub();
	characterSelect_stats_speed.render3ub();

	//

	characterSelectButton_attack1.render3ub();
	characterSelectButton_attack2.render3ub();
	characterSelectButton_attack3.render3ub();

	//

	characterSelectButton_climb.render3ub();
	characterSelectButton_jump.render3ub();
	characterSelectButton_doubleJump.render3ub();

	//

	characterSelectButton_death.render3ub();
	characterSelectButton_idle.render3ub();
	characterSelectButton_hurt.render3ub();

	//

	characterSelectButton_punch.render3ub();
	characterSelectButton_run.render3ub();
	characterSelectButton_runAttack.render3ub();

	//Character active button display
	if (player.character == "biker") {
		characterSelectButton_biker.color[0] = buttonColors[0] + 100;
		characterSelectButton_biker.color[1] = buttonColors[1];
		characterSelectButton_biker.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_biker.color[0] = buttonColors[0];
		characterSelectButton_biker.color[1] = buttonColors[1];
		characterSelectButton_biker.color[2] = buttonColors[2];
	}

	if (player.character == "punk") {
		characterSelectButton_punk.color[0] = buttonColors[0] + 100;
		characterSelectButton_punk.color[1] = buttonColors[1];
		characterSelectButton_punk.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_punk.color[0] = buttonColors[0];
		characterSelectButton_punk.color[1] = buttonColors[1];
		characterSelectButton_punk.color[2] = buttonColors[2];
	}

	if (player.character == "cyborg") {
		characterSelectButton_cyborg.color[0] = buttonColors[0] + 100;
		characterSelectButton_cyborg.color[1] = buttonColors[1];
		characterSelectButton_cyborg.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_cyborg.color[0] = buttonColors[0];
		characterSelectButton_cyborg.color[1] = buttonColors[1];
		characterSelectButton_cyborg.color[2] = buttonColors[2];
	}


	//Animation active button display
	if (player.animation == "_attack1") {
		characterSelectButton_attack1.color[0] = buttonColors[0]+100;
		characterSelectButton_attack1.color[1] = buttonColors[1];
		characterSelectButton_attack1.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_attack1.color[0] = buttonColors[0];
		characterSelectButton_attack1.color[1] = buttonColors[1];
		characterSelectButton_attack1.color[2] = buttonColors[2];
	}

	if (player.animation == "_attack2") {
		characterSelectButton_attack2.color[0] = buttonColors[0] + 100;
		characterSelectButton_attack2.color[1] = buttonColors[1];
		characterSelectButton_attack2.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_attack2.color[0] = buttonColors[0];
		characterSelectButton_attack2.color[1] = buttonColors[1];
		characterSelectButton_attack2.color[2] = buttonColors[2];
	}

	if (player.animation == "_attack3") {
		characterSelectButton_attack3.color[0] = buttonColors[0] + 100;
		characterSelectButton_attack3.color[1] = buttonColors[1];
		characterSelectButton_attack3.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_attack3.color[0] = buttonColors[0];
		characterSelectButton_attack3.color[1] = buttonColors[1];
		characterSelectButton_attack3.color[2] = buttonColors[2];
	}

	if (player.animation == "_death") {
		characterSelectButton_death.color[0] = buttonColors[0] + 100;
		characterSelectButton_death.color[1] = buttonColors[1];
		characterSelectButton_death.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_death.color[0] = buttonColors[0];
		characterSelectButton_death.color[1] = buttonColors[1];
		characterSelectButton_death.color[2] = buttonColors[2];
	}

	if (player.animation == "_idle") {
		characterSelectButton_idle.color[0] = buttonColors[0] + 100;
		characterSelectButton_idle.color[1] = buttonColors[1];
		characterSelectButton_idle.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_idle.color[0] = buttonColors[0];
		characterSelectButton_idle.color[1] = buttonColors[1];
		characterSelectButton_idle.color[2] = buttonColors[2];
	}

	if (player.animation == "_run") {
		characterSelectButton_run.color[0] = buttonColors[0] + 100;
		characterSelectButton_run.color[1] = buttonColors[1];
		characterSelectButton_run.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_run.color[0] = buttonColors[0];
		characterSelectButton_run.color[1] = buttonColors[1];
		characterSelectButton_run.color[2] = buttonColors[2];
	}

	if (player.animation == "_jump") {
		characterSelectButton_jump.color[0] = buttonColors[0] + 100;
		characterSelectButton_jump.color[1] = buttonColors[1];
		characterSelectButton_jump.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_jump.color[0] = buttonColors[0];
		characterSelectButton_jump.color[1] = buttonColors[1];
		characterSelectButton_jump.color[2] = buttonColors[2];
	}

	if (player.animation == "_hurt") {
		characterSelectButton_hurt.color[0] = buttonColors[0] + 100;
		characterSelectButton_hurt.color[1] = buttonColors[1];
		characterSelectButton_hurt.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_hurt.color[0] = buttonColors[0];
		characterSelectButton_hurt.color[1] = buttonColors[1];
		characterSelectButton_hurt.color[2] = buttonColors[2];
	}

	if (player.animation == "_doublejump") {
		characterSelectButton_doubleJump.color[0] = buttonColors[0] + 100;
		characterSelectButton_doubleJump.color[1] = buttonColors[1];
		characterSelectButton_doubleJump.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_doubleJump.color[0] = buttonColors[0];
		characterSelectButton_doubleJump.color[1] = buttonColors[1];
		characterSelectButton_doubleJump.color[2] = buttonColors[2];
	}

	if (player.animation == "_punch") {
		characterSelectButton_punch.color[0] = buttonColors[0] + 100;
		characterSelectButton_punch.color[1] = buttonColors[1];
		characterSelectButton_punch.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_punch.color[0] = buttonColors[0];
		characterSelectButton_punch.color[1] = buttonColors[1];
		characterSelectButton_punch.color[2] = buttonColors[2];
	}

	if (player.animation == "_climb") {
		characterSelectButton_climb.color[0] = buttonColors[0] + 100;
		characterSelectButton_climb.color[1] = buttonColors[1];
		characterSelectButton_climb.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_climb.color[0] = buttonColors[0];
		characterSelectButton_climb.color[1] = buttonColors[1];
		characterSelectButton_climb.color[2] = buttonColors[2];
	}

	if (player.animation == "_run_attack") {
		characterSelectButton_runAttack.color[0] = buttonColors[0] + 100;
		characterSelectButton_runAttack.color[1] = buttonColors[1];
		characterSelectButton_runAttack.color[2] = buttonColors[2];
	}
	else {
		characterSelectButton_runAttack.color[0] = buttonColors[0];
		characterSelectButton_runAttack.color[1] = buttonColors[1];
		characterSelectButton_runAttack.color[2] = buttonColors[2];
	}



	// MENU BUTTONS STRINGS
	std::string characterSelectButton_biker_string = "BIKER";
	float characterSelectButton_biker_string_center = characterSelectButton_biker.coords[0] + (characterSelectButton_biker.coords[2] - characterSelectButton_biker.coords[0]) / 2 - 25;
	std::string characterSelectButton_punk_string = "PUNK";
	float characterSelectButton_punk_string_center = characterSelectButton_punk.coords[0] + (characterSelectButton_punk.coords[2] - characterSelectButton_punk.coords[0]) / 2 - 25;
	std::string characterSelectButton_cyborg_string = "CYBORG";
	float characterSelectButton_cyborg_string_center = characterSelectButton_cyborg.coords[0] + (characterSelectButton_cyborg.coords[2] - characterSelectButton_cyborg.coords[0]) / 2 - 40;

	std::string characterSelectButton_attack1_string = "Attack1";
	float characterSelectButton_attack1_string_center = characterSelectButton_attack1.coords[0] + (characterSelectButton_attack1.coords[2] - characterSelectButton_attack1.coords[0]) / 2 - 40;
	std::string characterSelectButton_attack2_string = "Attack2";
	float characterSelectButton_attack2_string_center = characterSelectButton_attack2.coords[0] + (characterSelectButton_attack2.coords[2] - characterSelectButton_attack2.coords[0]) / 2 - 40;
	std::string characterSelectButton_attack3_string = "Attack3";
	float characterSelectButton_attack3_string_center = characterSelectButton_attack3.coords[0] + (characterSelectButton_attack3.coords[2] - characterSelectButton_attack3.coords[0]) / 2 - 40;

	//

	std::string characterSelectButton_climb_string = "Climb";
	float characterSelectButton_climb_string_center = characterSelectButton_climb.coords[0] + (characterSelectButton_climb.coords[2] - characterSelectButton_climb.coords[0]) / 2 - 40;
	std::string characterSelectButton_jump_string = "Jump";
	float characterSelectButton_jump_string_center = characterSelectButton_jump.coords[0] + (characterSelectButton_jump.coords[2] - characterSelectButton_jump.coords[0]) / 2 - 40;
	std::string characterSelectButton_doubleJump_string = "D Jump";
	float characterSelectButton_doubleJump_string_center = characterSelectButton_doubleJump.coords[0] + (characterSelectButton_doubleJump.coords[2] - characterSelectButton_doubleJump.coords[0]) / 2 - 40;

	//

	std::string characterSelectButton_death_string = "Death";
	float characterSelectButton_death_string_center = characterSelectButton_death.coords[0] + (characterSelectButton_death.coords[2] - characterSelectButton_death.coords[0]) / 2 - 40;
	std::string characterSelectButton_idle_string = "Idle";
	float characterSelectButton_idle_string_center = characterSelectButton_idle.coords[0] + (characterSelectButton_idle.coords[2] - characterSelectButton_idle.coords[0]) / 2 - 40;
	std::string characterSelectButton_hurt_string = "Hurt";
	float characterSelectButton_hurt_string_center = characterSelectButton_hurt.coords[0] + (characterSelectButton_hurt.coords[2] - characterSelectButton_hurt.coords[0]) / 2 - 40;

	//

	std::string characterSelectButton_punch_string = "Punch";
	float characterSelectButton_punch_string_center = characterSelectButton_punch.coords[0] + (characterSelectButton_punch.coords[2] - characterSelectButton_punch.coords[0]) / 2 - 40;
	std::string characterSelectButton_run_string = "Run";
	float characterSelectButton_run_string_center = characterSelectButton_run.coords[0] + (characterSelectButton_run.coords[2] - characterSelectButton_run.coords[0]) / 2 - 40;
	std::string characterSelectButton_runAttack_string = "Run Attack";
	float characterSelectButton_runAttack_string_center = characterSelectButton_runAttack.coords[0] + (characterSelectButton_runAttack.coords[2] - characterSelectButton_runAttack.coords[0]) / 2 - 40;

	/* RENDER STRINGS */
	glColor3ub(43, 43, 43);
	glRasterPos2i(characterSelectButton_biker_string_center, 3 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_biker_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_biker_string[i]);
	}

	glRasterPos2i(characterSelectButton_punk_string_center, 3 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_punk_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_punk_string[i]);
	}

	glRasterPos2i(characterSelectButton_cyborg_string_center, 3 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_cyborg_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_cyborg_string[i]);
	}

	//

	glRasterPos2i(characterSelectButton_attack1_string_center, -2 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_attack1_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_attack1_string[i]);
	}

	glRasterPos2i(characterSelectButton_attack2_string_center, -2 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_attack2_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_attack2_string[i]);
	}

	glRasterPos2i(characterSelectButton_attack3_string_center, -2 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_attack3_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_attack3_string[i]);
	}

	//

	glRasterPos2i(characterSelectButton_climb_string_center, -3 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_climb_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_climb_string[i]);
	}

	glRasterPos2i(characterSelectButton_jump_string_center, -3 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_jump_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_jump_string[i]);
	}

	glRasterPos2i(characterSelectButton_doubleJump_string_center, -3 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_doubleJump_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_doubleJump_string[i]);
	}

	//

	glRasterPos2i(characterSelectButton_death_string_center, -4 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_death_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_death_string[i]);
	}

	glRasterPos2i(characterSelectButton_idle_string_center, -4 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_idle_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_idle_string[i]);
	}

	glRasterPos2i(characterSelectButton_hurt_string_center, -4 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_hurt_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_hurt_string[i]);
	}

	//

	glRasterPos2i(characterSelectButton_punch_string_center, -5 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_punch_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_punch_string[i]);
	}

	glRasterPos2i(characterSelectButton_run_string_center, -5 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_run_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_run_string[i]);
	}

	glRasterPos2i(characterSelectButton_runAttack_string_center, -5 * 18 * 2 * buttonSpacing);
	for (int i = 0; i < characterSelectButton_runAttack_string.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, characterSelectButton_runAttack_string[i]);
	}

	glutPostRedisplay();
	glPopMatrix();
	glutSwapBuffers();

	//printf("X: %f / Y: %f\n", movementX, movementY);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-screenWidth / 2, screenWidth / 2, -screenHeight / 2, screenHeight / 2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			if (currentMenu == "menu") menuPage_mouseHandler(x, y);
			else if (currentMenu == "characterSelection") characterSelection_mouseHandler(x, y);
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

void menuPage_mouseHandler(int x, int y) {
	int collisionStatus = 0;
	collisionStatus = mouseCollisionAABB(x - screenWidth / 2, screenHeight / 2 - y);
	if (collisionStatus > 0) {
		printf("%d %d collision detected at button %d\n", x - screenWidth / 2, screenHeight / 2 - y, collisionStatus);
		switch (collisionStatus) {
		case 1:
			currentMenu = "game";
			initGame();
			glutDisplayFunc(displayGame);
			break;
		case 2:
			currentMenu = "characterSelection";
			initCharacterSelection();
			glutDisplayFunc(displayCharacterSelection);
			break;
		case 3:
			break;
		case 4:
			exit(0);
			break;
		default:
			break;
		}
	}
	else printf("%d %d no collision detected\n", x - screenWidth / 2, screenHeight / 2 - y);
}

void characterSelection_mouseHandler(int x, int y) {
	int collisionStatus = 0;
	collisionStatus = mouseCollisionAABB(x - screenWidth / 2, screenHeight / 2 - y);
	if (collisionStatus > 0) {
		printf("%d %d collision detected at button %d\n", screenWidth / 2 - x, screenHeight / 2 - y, collisionStatus);
		switch (collisionStatus) {
		case 1:
			if (player.character != "biker") {
				player.character = "biker";
				player.setStats(bikerStats);
				player.texture = textures[player.character + player.animation];
			}
			break;
		case 2:
			if (player.character != "punk") {
				player.character = "punk";
				player.setStats(punkStats);
				player.texture = textures[player.character + player.animation];
			}
			break;
		case 3:
			if (player.character != "cyborg") {
				player.character = "cyborg";
				player.setStats(cyborgStats);
				player.texture = textures[player.character + player.animation];
			}
			break;
		case 4:
			animationAttack1();
			break;
		case 5:
			animationAttack2();
			break;
		case 6:
			animationAttack3();
			break;
		case 7:
			animationClimb();
			break;
		case 8:
			animationJump();
			break;
		case 9:
			animationDoublejump();
			break;
		case 10:
			animationDeath();
			break;
		case 11:
			animationIdle();
			break;
		case 12:
			animationHurt();
			break;
		case 13:
			animationPunch();
			break;
		case 14:
			animationRun();
			break;
		case 15:
			animationRunAttack();
			break;
		default:
			break;
		}
	}
	else printf("%d %d no collision detected\n", x - screenWidth / 2, screenHeight / 2 - y);
}

void keyDown(unsigned char key, int x, int y) {
	keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
	keyStates[key] = false;
}

void controls() {
	specialKeys();
	if (controlsActive) {
		movementKeys();
	}
}

void movementKeys() {
	if (keyStates['w'] || keyStates['W'])
			if (!collisionUp(movementX, movementY, player) && !player.isJumping && !player.isFalling && !player.isAttacking && !player.isHurt) {
					player.isJumping = true;
					startingY = movementY;
					if (player.animation != "_jump")animationJump();
			}
	//printf("X: %f\n Y: %f\n", movementX, movementY);
		
	if (keyStates['s'] || keyStates['S'])
		if (!collisionDown(movementX, movementY, player) && !player.isAttacking && !player.isHurt) {
			for (float i = 0; i <= player.maxSpeed; i += 0.1) {
				if (!collisionDown(movementX, movementY, player)) {
					movementY -= 0.1;
					player.move(movementX, movementY);
					player.isJumping = false;
					gravityActive = true;
				}
				else break;
			}
		}
	//printf("X: %f\n Y: %f\n", movementX, movementY);

	if (keyStates['a'] || keyStates['A'])
		if (-screenWidth / 2 + player.size/2 < movementX) 
			if (!collisionLeft(movementX, movementY, player) && !player.isAttacking && !player.isHurt) {
				for (float i = 0; i <= player.maxSpeed; i += 0.1) {
					if (!collisionLeft(movementX, movementY, player)) {
						movementX -= 0.1;
						player.move(movementX, movementY);
						player.direction = 'l';
						if (!player.isJumping && !player.isFalling) {
							player.isMoving = true;
							if (player.animation != "_run") animationRun();
						}
					}
					else break;
				}
			}
	//printf("X: %f\n Y: %f\n", movementX, movementY);

	if(keyStates['d'] || keyStates['D'])
		if (screenWidth / 2 - player.size/2 > movementX) 
			if (!collisionRight(movementX, movementY, player) && !player.isAttacking && !player.isHurt) {
				for (float i = 0; i <= player.maxSpeed; i += 0.1) {
					if (!collisionRight(movementX, movementY, player)) {
						movementX += 0.1;
						player.move(movementX, movementY);
						player.direction = 'r';
						if (!player.isJumping && !player.isFalling) {
							player.isMoving = true;
							if (player.animation != "_run") animationRun();
						}
					}
					else break;
				}
			}
	//printf("X: %f\n Y: %f\n", movementX, movementY);

}

void specialKeys() {
	if (currentMenu == "game") {
		if ((keyStates['j'] || keyStates['J']) && !player.isAttacking && !player.isHurt)
		{
			if (player.animation != "_attack1") animationAttack1();
			player.isAttacking = true;
		}
		if ((keyStates['k'] || keyStates['K']) && !player.isAttacking && !player.isHurt)
		{
			if (player.animation != "_attack2") animationAttack2();
			player.isAttacking = true;
		}
		if ((keyStates['l'] || keyStates['L']) && !player.isAttacking && !player.isHurt)
		{
			if (player.animation != "_attack3")animationAttack3();
			player.isAttacking = true;
		}
	}

	if (keyStates[27]) {
		if (currentMenu == "menu") exit(0);
		else {
			currentMenu = "menu";
			initMenu();
			glutDisplayFunc(displayMenu);
			keyStates[27] = false;
		}
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
	checkGround();
	checkCollectible();
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
	createTexture("./Resources/Components/Background/Overlay_illumination.png", "Illumination", i++);

	//Money sprite
	createTexture("./Resources/Components/Animated_objects/Money.png", "money", i++);

	//Objects on the map
	createTexture("./Resources/Components/Animated_objects/Fountain.png", "animated_fountain", i++);
	createTexture("./Resources/Components/Objects/Fence/6.png", "fence", i++);
	createTexture("./Resources/Components/Objects/Other/Tree1.png", "tree1", i++);
	createTexture("./Resources/Components/Objects/Other/Tree2.png", "tree2", i++);
	createTexture("./Resources/Components/Objects/Other/Tree4.png", "tree_with_swing", i++);
}

void animationAttack1() {
	player.animation = "_attack1";
	player.texture = textures[player.character + player.animation];
	player.currentSprite = 0;
	player.sprites = 6;
	//currentSprite = 0;
}
void animationAttack2() {
	player.animation = "_attack2";
	player.texture = textures[player.character + player.animation];
	player.currentSprite = 0;
	player.sprites = 8;
	//currentSprite = 0;
}
void animationAttack3() {
	player.animation = "_attack3";
	player.texture = textures[player.character + player.animation];
	player.currentSprite = 0;
	player.sprites = 8;
	//currentSprite = 0;
}
void animationClimb() {
	player.animation = "_climb";
	player.texture = textures[player.character + player.animation];
	player.currentSprite = 0;
	player.sprites = 6;
	//currentSprite = 0;
}
void animationDeath() {
	player.animation = "_death";
	player.texture = textures[player.character + player.animation];
	player.currentSprite = 0;
	player.sprites = 6;
	//currentSprite = 0;
}
void animationDoublejump() {
	player.animation = "_doublejump";
	player.texture = textures[player.character + player.animation];
	player.currentSprite = 0;
	player.sprites = 6;
	//currentSprite = 0;
}
void animationHurt() {
	player.animation = "_hurt";
	player.texture = textures[player.character + player.animation];
	player.currentSprite = 0;
	player.sprites = 2;
	//currentSprite = 0;
}
void animationIdle() {
	player.animation = "_idle";
	player.texture = textures[player.character + player.animation];
	player.currentSprite = 0;
	player.sprites = 4;
	//currentSprite = 0;
}
void animationJump() {
	player.animation = "_jump";
	player.texture = textures[player.character + player.animation];
	player.currentSprite = 0;
	player.sprites = 4;
	//currentSprite = 0;
}
void animationFalling() {
	player.animation = "_jump";
	player.texture = textures[player.character + player.animation];
	player.currentSprite = 0;
	player.sprites = 4;
	//currentSprite = 0;
}
void animationPunch() {
	player.animation = "_punch";
	player.texture = textures[player.character + player.animation];
	player.currentSprite = 0;
	player.sprites = 6;
	//currentSprite = 0;
}
void animationRun() {
	player.animation = "_run";
	player.texture = textures[player.character + player.animation];
	player.currentSprite = 0;
	player.sprites = 6;
	//currentSprite = 0;
}
void animationRunAttack() {
	player.animation = "_run_attack";
	player.texture = textures[player.character + player.animation];
	player.currentSprite = 0;
	player.sprites = 6;
	//currentSprite = 0;
}

void checkGround() {
	if (collisionDown(movementX, movementY, player)) {
		player.isFalling = false;
		if (player.isFallingCounter >= 100) {
			printf("Falling damage: %d\n", player.isFallingCounter/10);
			if (player.health > 0) {
				player.health -= player.isFallingCounter / 10;
				player.isHurt = true;
			}
			if (player.health < 0) player.health = 0;
		}
		player.isFallingCounter = 0;
		player.isJumping = false;
	}
}

void checkCollectible() {
	if (collisionAABBCollectibles(movementX, movementY, player)) {
		score += 100;
		generate_randomCoords();
	}
	else score += 0;
}

void generate_randomCoords() {
	int x = rand()%(screenWidth-24) - (screenWidth/2 - 24), y = -rand()%(screenHeight/2) + 32+12;

	money.updateCoords(x, y);

	initCollectibles();
	addCollectibles(money.coords);

	//printf("X: %d, Y:%d\n", collectible.coords[0], collectible.coords[1]);
}