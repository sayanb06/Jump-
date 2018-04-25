#pragma once
#include "ofMain.h"

#define BORDER_WIDTH_FACTOR 0.9

class Platform
{
private:
	//x position of the platform
	int xPos;

	//y position of the platform
	int yPos;

	//length of the platform
	int length = -1;

	//height of the platform
	int height = -1;
public:
	//initializer with random width, height and length determined by area
	Platform();
	~Platform();

	//platform
	ofRectangle getPlatform();

	int getLength(), getHeight(), getXPos(), getYPos();
};

