#pragma once
#include "ofMain.h"

#define BORDER_WIDTH_FACTOR 0.1

class Platform {

private:
	double xPos_;							//x position of the platform
	double yPos_;							//y position of the platform
	double length_ = -1;					//length of the platform
	double height_ = -1;					//height of the platform
	bool movedLeftLast = false;				//to simulate the platform moving

public:

	/*
	initializer with only width and height
	platform will be at bottom of screen
	*/
	Platform::Platform(double width, double height) : 
		Platform(width, height, ofGetWindowHeight() * (1 - BORDER_WIDTH_FACTOR), ofGetWindowHeight()) {}
	
	/*
	xPos is going to be random, yPos is going to be between yPosMin and yPosMax
	*/
	Platform(double width, double height, double yPosMin, double yPosMax);
	
	~Platform();
	
	/*
	return platform as rectangle
	*/
	ofRectangle getPlatform();
	
	double getLength(), getHeight(), getXPos(), getYPos();
	
	/*
	move yPos_ down, even if it goes off the screen
	*/
	void movePlatformDown(double yChange);
	
	/*
	make game harder by moving platform around
	*/
	void moveXDirection(double xChange);
};

