#pragma once
#include "Object.h"
class Platform :
    public Object
{
public:
    float hitboxCoords[4] = { 0.0, 0.0, 0.0, 0.0 };
    int numberOfTiles = 0;
    GLuint texture[3] = { NULL };

    Platform() {};
    Platform(float x0, float y0, int tiles) {
        this->coords[0] = x0;
        this->coords[1] = y0;
        this->coords[2] = x0 + 32;
        this->coords[3] = y0 - 32;
        this->numberOfTiles = tiles;
    };

    void calcHitbox() {
        this->hitboxCoords[0] = this->coords[0];
        this->hitboxCoords[1] = this->coords[1];
        this->hitboxCoords[2] = this->coords[0] + ((numberOfTiles+1) * 32);
        this->hitboxCoords[3] = this->coords[1] - 32;
    }

    void render() {
        calcHitbox();
        glColor3f(this->color[0], this->color[1], this->color[2]);
        //glRectf(this->coords[0] - this->size / 2, this->coords[1] - this->size / 2, this->coords[2] + this->size / 2, this->coords[3] + this->size / 2);
        for (int i = 0; i < numberOfTiles; i++) {
            glBegin(GL_QUADS);
            glVertex2f(this->coords[0] + i * 32, this->coords[1]);
            glVertex2f(this->coords[2] + (i+1) * 32, this->coords[1]);
            glVertex2f(this->coords[2] + (i+1) * 32, this->coords[3]);
            glVertex2f(this->coords[0] + i * 32, this->coords[3]);
            glEnd();
        }
    }
    void renderTex() {
        glEnable(GL_TEXTURE_2D);
        //glBindTexture(GL_TEXTURE_2D, this->texture);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glColor3f(this->color[0], this->color[1], this->color[2]);
        //glRectf(this->coords[0] - this->size / 2, this->coords[1] - this->size / 2, this->coords[2] + this->size / 2, this->coords[3] + this->size / 2);
        for (int i = 0; i < numberOfTiles; i++) {

            if (numberOfTiles > 1) {
                if (i == 0) glBindTexture(GL_TEXTURE_2D, this->texture[0]);
                else if (i == numberOfTiles - 1) glBindTexture(GL_TEXTURE_2D, this->texture[2]);
                else glBindTexture(GL_TEXTURE_2D, this->texture[1]);
            }
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex2f(this->coords[0] + i * 32, this->coords[1]);
            glTexCoord2f(1.0, 0.0);
            glVertex2f(this->coords[2] + (i + 1) * 32, this->coords[1]);
            glTexCoord2f(1.0, 1.0);
            glVertex2f(this->coords[2] + (i + 1) * 32, this->coords[3]);
            glTexCoord2f(0.0, 1.0);
            glVertex2f(this->coords[0] + i * 32, this->coords[3]);
            glEnd();
        }
        glDisable(GL_TEXTURE_2D);
    }
};

