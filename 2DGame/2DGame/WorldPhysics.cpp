#include "WorldPhysics.h"

extern void initEntities() {
	for (float* entity : entities) {
		entity[0] = UINT16_MAX;
		entity[1] = UINT16_MAX;
		entity[2] = UINT16_MAX;
		entity[3] = UINT16_MAX;
	}
}

extern void addEntities(float XY[]) {
	entities[entitiesIdx][0] = XY[0];
	entities[entitiesIdx][1] = XY[1];
	entities[entitiesIdx][2] = XY[2];
	entities[entitiesIdx][3] = XY[3];
	entitiesIdx++;
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