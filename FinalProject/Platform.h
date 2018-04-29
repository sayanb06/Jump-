#pragma once
#include "ofMain.h"

#define BORDER_WIDTH_FACTOR 0.1

class Platform
{
private:
	//x position of the platform
	double xPos_;

	//y position of the platform
	double yPos_;

	//length of the platform
	double length_ = -1;

	//height of the platform
	double height_ = -1;
public:
	//initializer with random width, height and length determined by area
	Platform(int width, int height);
	~Platform();
	Platform(int width, int height, int yPosMin, int yPosMax);

	//platform
	ofRectangle getPlatform();

	int getLength(), getHeight(), getXPos(), getYPos();

	void movePlatformDown(double yChange);
};

