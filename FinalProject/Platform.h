#pragma once
#include "ofMain.h"

#define BORDER_WIDTH_FACTOR 0.1

class Platform
{
private:
	//x position of the platform
	int xPos_;

	//y position of the platform
	int yPos_;

	//length of the platform
	int length_ = -1;

	//height of the platform
	int height_ = -1;
public:
	//initializer with random width, height and length determined by area
	Platform();
	~Platform();

	//platform
	ofRectangle getPlatform();

	int getLength(), getHeight(), getXPos(), getYPos();
};

