#pragma once
#include "ofMain.h"
#define CIRCLE_RADIUS 15

class Person {
private:
	/*
	[
		[CircleRadius],						//head
		[yChangeTorso],						//body
		[xChange, yChangeTopToBottom],		//Both Legs
		[xChangeArm],						//Both Arms
	]
	*/
	int** personPartsParameters_;

	/*
	length of arrays
	*/
	const int kCircleParamArrayLen = 1;
	const int kTorsoParamArrayLen = 1;
	const int kLegParamArrayLen = 2;
	const int kArmParamArrayLen = 1;
	
	/*
	length of each body part
	*/
	const int kTorsoHeight = 30;
	const int kLegDeltaX = 20;
	const int kLegDeltaY = 30;
	const int kArmSpan = (int) sqrt(pow(kLegDeltaX, 2) + pow(kLegDeltaY, 2)); //arm is about as long as legs

public:
	static const int kNumberOfBodyParts = 4;
	static const int kCircleRadiusIndex = 0;
	static const int kTorsoIndex = 1;
	static const int kLegIndex = 2;
	static const int kArmIndex = 3;

	Person();
	~Person();

	//return person parts as array
	int** getPerson();
};

