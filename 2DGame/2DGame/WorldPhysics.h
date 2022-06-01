#pragma once

#include <cstdint>
#include <stdlib.h>
#include <cmath>
#include "Character.h"

static float collectibles[5][4];
static int collectiblesIdx = 0;
static float entities[50][4];
static int entitiesIdx = 0;
static float menuButtons[30][4];
static int buttonsIdx = 0;

extern void initCollectibles();
extern void printCollectibles();
extern void addCollectibles(float[]);
extern void removeCollectibles();

extern void initEntities();
extern void printEntities();
extern void addEntities(float[]);
extern void removeEntities();

extern void initButtons();
extern void printButtons();
extern void addButtons(float[]);
extern void removeButtons();

extern bool collisionAABB(float, float, Character);
extern bool collisionAABBCollectibles(float, float, Character);
extern int mouseCollisionAABB(float, float);
extern bool collisionUp(float, float, Character);
extern bool collisionDown(float, float, Character);
extern bool collisionLeft(float, float, Character);
extern bool collisionRight(float, float, Character);
