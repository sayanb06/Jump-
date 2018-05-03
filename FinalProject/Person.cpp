#include "Person.h"

Person::Person() {
	//create array
	personPartsParameters_ = new int*[kNumberOfBodyParts];
	
	//circle
	personPartsParameters_[kCircleRadiusIndex] = new int[kCircleParamArrayLen];
	personPartsParameters_[kCircleRadiusIndex][0] = CIRCLE_RADIUS;

	//Torso
	personPartsParameters_[kTorsoIndex] = new int[kTorsoParamArrayLen];
	personPartsParameters_[kTorsoIndex][0] = kTorsoHeight;
	
	//Both Legs
	personPartsParameters_[kLegIndex] = new int[kLegParamArrayLen];
	personPartsParameters_[kLegIndex][0] = kLegDeltaX;
	personPartsParameters_[kLegIndex][1] = kLegDeltaY;

	//Both Arms
	personPartsParameters_[kArmIndex] = new int[kArmParamArrayLen];
	personPartsParameters_[kArmIndex][0] = kArmSpan;
}


Person::~Person() {
	for (int i = 0; i < kNumberOfBodyParts; ++i) {
		delete[] personPartsParameters_[i];
		personPartsParameters_[i] = NULL;
	}

	delete[] personPartsParameters_;
	personPartsParameters_ = NULL;
}

int** Person::getPerson() {
	return personPartsParameters_;
}
