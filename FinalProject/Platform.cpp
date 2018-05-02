#include "Platform.h"
#include <stdlib.h>

Platform::Platform(double width, double height) : Platform(width, height, ofGetWindowHeight() * (1 - BORDER_WIDTH_FACTOR), ofGetWindowHeight()) {}


Platform::~Platform() {
	xPos_ = 0;
	yPos_ = 0;
	length_ = 0;
	height_ = 0;
}

Platform::Platform(double width, double height, double yPosMin, double yPosMax) {
	xPos_ = rand() % (int)(ofGetWindowWidth() * (1 - BORDER_WIDTH_FACTOR - BORDER_WIDTH_FACTOR)) + ofGetWindowWidth() * BORDER_WIDTH_FACTOR;
	yPos_ = rand() % (int)(yPosMax - yPosMin) + yPosMin;
	length_ = width;
	height_ = height;
}

ofRectangle Platform::getPlatform() {
	if (length_ == -1 || height_ == -1) {
		return ofRectangle(0, 0, 10, 10);
	}

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
	bool inLeftBorder = xPos_ <= (ofGetWindowWidth() * BORDER_WIDTH_FACTOR);
	bool inRightBorder = xPos_ >= (ofGetWindowWidth() * (1 - BORDER_WIDTH_FACTOR));
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
