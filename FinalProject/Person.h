#pragma once
#include "ofMain.h"
#define CIRCLE_RADIUS 15

class Person {
private:
	/*
	[
		[CircleRadius],
		[yChangeTorso], //body
		[xChangeLeft, yChangeTopToBottom]	//left leg		
		[xChangeRight, yChangeTopToBottom]	//right leg
	]
	*/
	int** personPartsParameters_;
	
	const int kNumberOfBodyParts = 6;

	const int kcircleParamArrayLen = 1;
	const int kTorsoParamArrayLen = 1;
	const int kLegParamArrayLen = 2;
	const int kArmParamArrayLen = 1;

public:
	static const int kCircleRadiusIndex = 0;
	static const int kTorsoIndex = 1;
	static const int kLeftLegIndex = 2;
	static const int kRightLegIndex = 3; 
	static const int kLeftArmIndex = 4;
	static const int kRightArmIndex = 5;


	Person();
	~Person();

	//return person parts as array
	int** getPerson();
};

