#include "Platform.h"
#include <stdlib.h>

Platform::Platform() {
	xPos_ = rand() % (int) (ofGetWindowWidth() * BORDER_WIDTH_FACTOR) + ofGetWindowWidth() * (1 - BORDER_WIDTH_FACTOR) / 2;
	yPos_ = rand() % (int) (ofGetWindowHeight() * BORDER_WIDTH_FACTOR) + ofGetWindowHeight() * (1 - BORDER_WIDTH_FACTOR) / 2;
	length_ = 50;
	height_ = 10;
}


Platform::~Platform() {
	xPos_ = 0;
	yPos_ = 0;
	length_ = 0;
	height_ = 0;
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