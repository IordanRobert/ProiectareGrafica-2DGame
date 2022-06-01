#pragma once
#include "Object.h"
#include <string>
class Character :
    public Object
{
public:
    float size = 48.0; 
    float totalHealth = 200.0;
    float health = 200.0;
    bool isHurt = false;
    float damage = 0.0;
    float maxSpeed = 1.0, jmpSpeed = 0.1; 
    float maxJump = 5.0;
    float acceleration = 0.1;
    char direction = 'r';
    std::string character = ""; // biker / punk / cyborg
    bool isJumping = false;
    bool isFalling = false;
    int isFallingCounter = 0;
    bool isAttacking = false;
    bool isMoving = false;

    Character() {};
    Character(float size, float totalHealth, float health, float damage, float jump, float speed, std::string character) {
        this->size = size;
        this->totalHealth = totalHealth;
        this->health = health;
        this->damage = damage;
        this->maxJump = jump;
        this->maxSpeed = speed;
        this->character = character;
    };
    void move(float movementX, float movementY) {
        this->coords[0] = movementX;
        this->coords[1] = movementY;
        this->coords[2] = movementX;
        this->coords[3] = movementY;
        glutPostRedisplay();
    };
    void setStats(float stats[]) {
        this->totalHealth = stats[0];
        this->health = stats[1];
        this->damage = stats[2];
        this->maxJump = stats[3];
        this->maxSpeed = stats[4];
    }
    void render() {
        glColor3f(this->color[0], this->color[1], this->color[2]);
        //glRectf(this->coords[0] - this->size / 2, this->coords[1] - this->size / 2, this->coords[2] + this->size / 2, this->coords[3] + this->size / 2);
        glBegin(GL_QUADS);
        glVertex2f(this->coords[0] - this->size/2, this->coords[1] - this->size / 2);
        glVertex2f(this->coords[2] + this->size / 2, this->coords[1] - this->size / 2);
        glVertex2f(this->coords[2] + this->size / 2, this->coords[3] + this->size / 2);
        glVertex2f(this->coords[0] - this->size / 2, this->coords[3] + this->size / 2);
        glEnd();
    }
    void renderTex() {
        float toleranceX = -0.001;
        float toleranceY = +0.01;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glColor3f(this->color[0], this->color[1], this->color[2]);
        //glRectf(this->coords[0] - this->size / 2, this->coords[1] - this->size / 2, this->coords[2] + this->size / 2, this->coords[3] + this->size / 2);
        glBegin(GL_QUADS);
        if (this->direction == 'r') {
            glTexCoord2f((float)(1.0/sprites*currentSprite + toleranceX), 1.0 + toleranceY);
            glVertex2f(this->coords[0] - this->size / 2, this->coords[1] - this->size / 2);
            glTexCoord2f((float)(1.0 / sprites * (currentSprite+1) + toleranceX), 1.0 + toleranceY);
            glVertex2f(this->coords[2] + this->size / 2, this->coords[1] - this->size / 2);
            glTexCoord2f((float)(1.0 / sprites * (currentSprite+1) + toleranceX), 0.0 + toleranceY);
            glVertex2f(this->coords[2] + this->size / 2, this->coords[3] + this->size / 2);
            glTexCoord2f((float)(1.0 / sprites * currentSprite + toleranceX), 0.0 + toleranceY);
            glVertex2f(this->coords[0] - this->size / 2, this->coords[3] + this->size / 2);
        }
        else if (this->direction == 'l') {
            glTexCoord2f((float)(1.0 / sprites * (currentSprite+1) + toleranceX), 1.0 + toleranceY);
            glVertex2f(this->coords[0] - this->size / 2, this->coords[1] - this->size / 2);
            glTexCoord2f((float)(1.0 / sprites * currentSprite + toleranceX), 1.0 + toleranceY);
            glVertex2f(this->coords[2] + this->size / 2, this->coords[1] - this->size / 2);
            glTexCoord2f((float)(1.0 / sprites * currentSprite + toleranceX), 0.0 + toleranceY);
            glVertex2f(this->coords[2] + this->size / 2, this->coords[3] + this->size / 2);
            glTexCoord2f((float)(1.0 / sprites * (currentSprite+1) + toleranceX), 0.0 + toleranceY);
            glVertex2f(this->coords[0] - this->size / 2, this->coords[3] + this->size / 2);
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
};

