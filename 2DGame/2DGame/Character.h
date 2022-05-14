#pragma once
#include "Object.h"
class Character :
    public Object
{
public:
    float size = 20.0; 
    float maxSpeed = 1.0, jmpSpeed = 0.1; 
    float maxJump = 5.0;
    float acceleration = 0.1;
    bool isInAir = false;
    bool isFalling = false;

    Character() {};
    Character(float size, float speed, float jump) {
        this->size = size;
        this->maxSpeed = speed;
        this->maxJump = jump;
    };
    void move(float movementX, float movementY) {
        this->coords[0] = movementX;
        this->coords[1] = movementY;
        this->coords[2] = movementX;
        this->coords[3] = movementY;
        glutPostRedisplay();
    };
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
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glColor3f(this->color[0], this->color[1], this->color[2]);
        //glRectf(this->coords[0] - this->size / 2, this->coords[1] - this->size / 2, this->coords[2] + this->size / 2, this->coords[3] + this->size / 2);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0);
        glVertex2f(this->coords[0] - this->size / 2, this->coords[1] - this->size / 2);
        glTexCoord2f(0.5, 1.0);
        glVertex2f(this->coords[2] + this->size / 2, this->coords[1] - this->size / 2);
        glTexCoord2f(0.5, 0.0);
        glVertex2f(this->coords[2] + this->size / 2, this->coords[3] + this->size / 2);
        glTexCoord2f(0.0, 0.0);
        glVertex2f(this->coords[0] - this->size / 2, this->coords[3] + this->size / 2);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
};

