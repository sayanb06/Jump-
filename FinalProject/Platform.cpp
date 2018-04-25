#include "Platform.h"
#include <stdlib.h>



Platform::Platform() {
	xPos = rand() % (int) (ofGetWindowWidth() * BORDER_WIDTH_FACTOR) + ofGetWindowWidth() * (1 - BORDER_WIDTH_FACTOR) / 2;
	yPos = rand() % (int) (ofGetWindowHeight() * BORDER_WIDTH_FACTOR) + ofGetWindowHeight() * (1 - BORDER_WIDTH_FACTOR) / 2;
	length = 50;
	height = 10;
}


Platform::~Platform() {

}

ofRectangle Platform::getPlatform() {
	if (length == -1 || height == -1) {
		return ofRectangle(0, 0, 10, 10);
	}

	return ofRectangle(xPos , yPos, length, height);
}

int Platform::getHeight() {
	return height;
}

int Platform::getLength() {
	return length;
}

int Platform::getXPos() {
	return xPos;
}

int Platform::getYPos() {
	return yPos;
}