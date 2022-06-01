#pragma once
#include "Object.h"
class Collectible :
    public Object
{
public :
    float size = 24.0;

    Collectible() {};
    Collectible(float x, float y) {
        this->coords[0] = x - size / 2;
        this->coords[1] = y + size / 2;
        this->coords[2] = x + size / 2;
        this->coords[3] = y - size / 2;
    }
    Collectible(float x, float y, GLuint texture) {
        this->coords[0] = x - size/2;
        this->coords[1] = y + size/2;
        this->coords[2] = x + size/2;
        this->coords[3] = y - size / 2;
        this->texture = texture;
    }

    void updateCoords(int x, int y) {
        this->coords[0] = x - size / 2;
        this->coords[1] = y + size / 2;
        this->coords[2] = x + size / 2;
        this->coords[3] = y - size / 2;
    }

    void renderTex() {

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glColor3f(this->color[0], this->color[1], this->color[2]);
        
        glBegin(GL_QUADS);
        glTexCoord2f((float)(1.0 / sprites * currentSprite), 0.0);
        glVertex2f(this->coords[0], this->coords[1]);
        glTexCoord2f((float)(1.0 / sprites * (currentSprite + 1)), 0.0);
        glVertex2f(this->coords[2], this->coords[1]);
        glTexCoord2f((float)(1.0 / sprites * (currentSprite + 1)), 1.0);
        glVertex2f(this->coords[2], this->coords[3]);
        glTexCoord2f((float)(1.0 / sprites * currentSprite), 1.0);
        glVertex2f(this->coords[0], this->coords[3]);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
};

