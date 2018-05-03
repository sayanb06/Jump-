#include "Platform.h"
#include <stdlib.h>

Platform::~Platform() {
	xPos_ = 0;
	yPos_ = 0;
	length_ = 0;
	height_ = 0;
}

Platform::Platform(double width, double height, double yPosMin, double yPosMax) {
	double ousideBorderZone = ofGetWindowWidth() * (1 - BORDER_WIDTH_FACTOR - BORDER_WIDTH_FACTOR);
	//only valid outside border zone so calculating that distance, and then moving it there
	xPos_ = rand() % (int)(ousideBorderZone) + ofGetWindowWidth() * BORDER_WIDTH_FACTOR;
	yPos_ = rand() % (int)(yPosMax - yPosMin) + yPosMin;
	length_ = width;
	height_ = height;
}

ofRectangle Platform::getPlatform() {
	return ofRectangle(xPos_ , yPos_, length_, height_);
}

double Platform::getHeight() {
	return height_;
}

double Platform::getLength() {
	return length_;
}

double Platform::getXPos() {
	return xPos_;
}

double Platform::getYPos() {
	return yPos_;
}

void Platform::movePlatformDown(double yChange) {
	yPos_ += yChange;
}

void Platform::moveXDirection(double xChange) {
	//check whether platform is already inside the border
	bool inLeftBorder = xPos_ <= (ofGetWindowWidth() * BORDER_WIDTH_FACTOR);
	bool inRightBorder = xPos_ >= (ofGetWindowWidth() * (1 - BORDER_WIDTH_FACTOR));

	//adjust accordingly
	if (inLeftBorder) {
		//move only right
		xPos_ += xChange;
		movedLeftLast = false;
	} else if (inRightBorder) {
		//move only left
		xPos_ -= xChange;
		movedLeftLast = true;
	} else {
		if (movedLeftLast) {
			xPos_ -= xChange;
		} else {
			xPos_ += xChange;
		}
	}
}
