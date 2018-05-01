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

	const int kCircleParamArrayLen = 1;
	const int kTorsoParamArrayLen = 1;
	const int kLegParamArrayLen = 2;
	const int kArmParamArrayLen = 1;

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

