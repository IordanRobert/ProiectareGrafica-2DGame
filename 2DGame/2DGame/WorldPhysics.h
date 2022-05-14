#pragma once

#include <cstdint>
#include <stdlib.h>
#include <cmath>
#include "Character.h"

static float entities[100][4];
static int entitiesIdx = 0;
extern void initEntities();
extern void addEntities(float XY[4]);
extern void removeEntities();

extern bool collisionAABB(float posX, float posY, Character entity);
extern bool collisionUp(float x, float y, Character entity);
extern bool collisionDown(float x, float y, Character entity);
extern bool collisionLeft(float x, float y, Character entity);
extern bool collisionRight(float x, float y, Character entity);
