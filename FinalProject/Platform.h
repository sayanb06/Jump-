#pragma once
#include "ofMain.h"

#define BORDER_WIDTH_FACTOR 0.1

class Platform
{
private:
	double xPos_;							//x position of the platform
	double yPos_;							//y position of the platform
	double length_ = -1;					//length of the platform
	double height_ = -1;					//height of the platform

public:

	//initializer with random width, height and length determined by area
	Platform(double width, double height);
	~Platform();
	Platform(double width, double height, double yPosMin, double yPosMax);
	bool movedLeftLast = false;
	//platform
	ofRectangle getPlatform();

	double getLength(), getHeight(), getXPos(), getYPos();

	void movePlatformDown(double yChange);

	void moveXDirection(double xChange);
};

