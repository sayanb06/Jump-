#include "Platform.h"
#include <stdlib.h>

Platform::Platform(int width, int height) : Platform(width, height, ofGetWindowHeight() * (1 - BORDER_WIDTH_FACTOR), ofGetWindowHeight()) {}


Platform::~Platform() {
	xPos_ = 0;
	yPos_ = 0;
	length_ = 0;
	height_ = 0;
}

Platform::Platform(int width, int height, int yPosMin, int yPosMax) {
	xPos_ = rand() % ofGetWindowWidth();
	yPos_ = rand() % (yPosMax - yPosMin) + yPosMin;
	length_ = width;
	height_ = height;
}

ofRectangle Platform::getPlatform() {
	if (length_ == -1 || height_ == -1) {
		return ofRectangle(0, 0, 10, 10);
	}

	return ofRectangle(xPos_ , yPos_, length_, height_);
}

int Platform::getHeight() {
	return height_;
}

int Platform::getLength() {
	return length_;
}

int Platform::getXPos() {
	return xPos_;
}

int Platform::getYPos() {
	return yPos_;
}

void Platform::movePlatformDown(double yChange) {
	yPos_ += yChange;
}