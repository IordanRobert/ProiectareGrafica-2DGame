#pragma once

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

class Object
{
public:
	float coords[4] = { 0.0,0.0,0.0,0.0 };
	float color[3] = { 1.0,1.0,1.0 };
	GLuint texture = NULL;
	std::string animation = "_idle";
	int currentSprite = 0;
	int sprites = 1;
	int animationCounter = 0;
#define ANIMATION_DELAY 15
	Object() {
	};
	Object(float x0, float y0, float x1, float y1) {
		this->coords[0] = x0;
		this->coords[1] = y0;
		this->coords[2] = x1;
		this->coords[3] = y1;
	}
	Object(float x0, float y0, float x1, float y1, float red, float green, float blue) {
		this->coords[0] = x0;
		this->coords[1] = y0;
		this->coords[2] = x1;
		this->coords[3] = y1;
		this->color[0] = red;
		this->color[1] = green;
		this->color[2] = blue;
	}
	void render3f() {
		glColor3f(this->color[0], this->color[1], this->color[2]);
		//glRectf(this->coords[0], this->coords[1], this->coords[2], this->coords[3]);
		glBegin(GL_QUADS);
		glVertex2f(this->coords[0], this->coords[1]);
		glVertex2f(this->coords[2], this->coords[1]);
		glVertex2f(this->coords[2], this->coords[3]);
		glVertex2f(this->coords[0], this->coords[3]);
		glEnd();
	}
	void render3ub() {
		glColor3ub(this->color[0], this->color[1], this->color[2]);
		//glRectf(this->coords[0], this->coords[1], this->coords[2], this->coords[3]);
		glBegin(GL_QUADS);
		glVertex2f(this->coords[0], this->coords[1]);
		glVertex2f(this->coords[2], this->coords[1]);
		glVertex2f(this->coords[2], this->coords[3]);
		glVertex2f(this->coords[0], this->coords[3]);
		glEnd();
	}
	void renderTex() {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glColor3f(this->color[0], this->color[1], this->color[2]);
		glBegin(GL_QUADS);
		glTexCoord2f((float)(1.0 / sprites * currentSprite), 1.0);
		glVertex2f(this->coords[0], this->coords[1]);
		glTexCoord2f((float)(1.0 / sprites * (currentSprite + 1)), 1.0);
		glVertex2f(this->coords[2], this->coords[1]);
		glTexCoord2f((float)(1.0 / sprites * (currentSprite + 1)), 0.0);
		glVertex2f(this->coords[2], this->coords[3]);
		glTexCoord2f((float)(1.0 / sprites * currentSprite), 0.0);
		glVertex2f(this->coords[0], this->coords[3]);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	int animationRepeat() {
		if (animationCounter >= ANIMATION_DELAY) {
			animationCounter = 0;
			currentSprite >= sprites ? (currentSprite = 1) : currentSprite++;
			return 0;
		}
		return 1;
	}
	int animationDoOnce() {
		if (animationCounter >= ANIMATION_DELAY) {
			animationCounter = 0;
			if (currentSprite >= sprites - 1) return 0;
			else currentSprite++;
		}
		return 1;
	}
};

