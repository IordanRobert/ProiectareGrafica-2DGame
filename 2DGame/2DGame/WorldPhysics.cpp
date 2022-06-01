#include "WorldPhysics.h"

//COLLECTIBLES
extern void initCollectibles() {
	collectiblesIdx = 0;
	for (float* collectible : collectibles) {
		collectible[0] = UINT16_MAX;
		collectible[1] = UINT16_MAX;
		collectible[2] = UINT16_MAX;
		collectible[3] = UINT16_MAX;
	}
}

extern void printCollectibles() {
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%lf\n", collectibles[i][j]);
		}
		printf("\n");
	}
}

extern void addCollectibles(float XY[]) {
	collectibles[collectiblesIdx][0] = XY[0];
	collectibles[collectiblesIdx][1] = XY[1];
	collectibles[collectiblesIdx][2] = XY[2];
	collectibles[collectiblesIdx][3] = XY[3];
	collectiblesIdx++;
}

//ENTITIES
extern void initEntities() {
	entitiesIdx = 0;
	for (float* entity : entities) {
		entity[0] = UINT16_MAX;
		entity[1] = UINT16_MAX;
		entity[2] = UINT16_MAX;
		entity[3] = UINT16_MAX;
	}
}

extern void printEntities() {
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%lf\n", entities[i][j]);
		}
		printf("\n");
	}
}

extern void addEntities(float XY[]) {
	entities[entitiesIdx][0] = XY[0];
	entities[entitiesIdx][1] = XY[1];
	entities[entitiesIdx][2] = XY[2];
	entities[entitiesIdx][3] = XY[3];
	entitiesIdx++;
}

// BUTTONS
extern void initButtons() {
	buttonsIdx = 0;
	for (float* button : menuButtons) {
		button[0] = UINT16_MAX;
		button[1] = UINT16_MAX;
		button[2] = UINT16_MAX;
		button[3] = UINT16_MAX;
	}
}

extern void printButtons() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%lf\n", menuButtons[i][j]);
		}
		printf("\n");
	}
}

extern void addButtons(float XY[]) {
	menuButtons[buttonsIdx][0] = XY[0];
	menuButtons[buttonsIdx][1] = XY[1];
	menuButtons[buttonsIdx][2] = XY[2];
	menuButtons[buttonsIdx][3] = XY[3];
	buttonsIdx++;
}

extern bool collisionAABB(float posX, float posY, Character player) {
	float playerCX = posX;
	float playerCY = posY;
	//printf("player: %f %f\n", playerCX, playerCY);

	for (float* entity : entities) {
		float entityX = fabs(*entity - *(entity + 2));
		float entityY = fabs(*(entity + 1) - *(entity + 3));
		float entityCX = *entity + (*(entity + 2) - *entity) / 2;
		float entityCY = *(entity + 1) + (*(entity + 3) - *(entity + 1)) / 2;

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

extern bool collisionAABBCollectibles(float posX, float posY, Character player) {
	float playerCX = posX;
	float playerCY = posY;
	//printf("player: %f %f\n", playerCX, playerCY);

	for (float* collectible : collectibles) {
		float collectibleX = fabs(*collectible - *(collectible + 2));
		float collectibleY = fabs(*(collectible + 1) - *(collectible + 3));
		float collectibleCX = *collectible + (*(collectible + 2) - *collectible) / 2;
		float collectibleCY = *(collectible + 1) + (*(collectible + 3) - *(collectible + 1)) / 2;

		//printf("collectible(cx, cy, x, y): %f %f %f %f\n", collectibleCX, collectibleCY, collectibleX, collectibleY);

		if (playerCX + player.size / 2 > collectibleCX - collectibleX / 2
			&& playerCX - player.size / 2 < collectibleCX + collectibleX / 2
			&& playerCY + player.size / 2 > collectibleCY - collectibleY / 2
			&& playerCY - player.size / 2 < collectibleCY + collectibleY / 2)
		{
			printf("Collected\n");
			return true;
		}
	}
	return false;
}

extern int mouseCollisionAABB(float posX, float posY) {
	float playerCX = posX;
	float playerCY = posY;
	float cursorDimension = 5;
	//printf("player: %f %f\n", playerCX, playerCY);

	int index = 0;

	for (float* button : menuButtons) {
		index++;
		float buttonX = fabs(*button - *(button + 2));
		float buttonY = fabs(*(button + 1) - *(button + 3));
		float buttonCX = *button + (*(button + 2) - *button) / 2;
		float buttonCY = *(button + 1) + (*(button + 3) - *(button + 1)) / 2;

		//printf("entity(cx, cy, x, y): %f %f %f %f\n", entityCX, entityCY, entityX, entityY);

		if (playerCX + cursorDimension > buttonCX - buttonX / 2
			&& playerCX - cursorDimension < buttonCX + buttonX / 2
			&& playerCY + cursorDimension > buttonCY - buttonY / 2
			&& playerCY - cursorDimension < buttonCY + buttonY / 2)
		{
			//std::cout << "Collision detected\n";
			return index;
		}
	}
	return 0;
}

extern bool collisionUp(float movementX, float movementY, Character entity) {
	float nextX = movementX, nextY = movementY + 0.1;
	return collisionAABB(nextX, nextY, entity);
}

extern bool collisionDown(float movementX, float movementY, Character entity) {
	float nextX = movementX, nextY = movementY - 0.1;
	return collisionAABB(nextX, nextY, entity);
}

extern bool collisionLeft(float movementX, float movementY, Character entity) {
	float nextX = movementX - 0.1, nextY = movementY;
	return collisionAABB(nextX, nextY, entity);
}

extern bool collisionRight(float movementX, float movementY, Character entity) {
	float nextX = movementX + 0.1, nextY = movementY;
	return collisionAABB(nextX, nextY, entity);
}