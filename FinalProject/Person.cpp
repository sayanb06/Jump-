#include "Person.h"

Person::Person() {
	//create array
	personPartsParameters_ = new int*[kNumberOfBodyParts];
	
	//circle
	personPartsParameters_[kCircleRadiusIndex] = new int[kcircleParamArrayLen];
	personPartsParameters_[kCircleRadiusIndex][0] = CIRCLE_RADIUS;

	//Torso
	personPartsParameters_[kTorsoIndex] = new int[kTorsoParamArrayLen];
	personPartsParameters_[kTorsoIndex][0] = 30;
	
	//Left Leg
	personPartsParameters_[kLeftLegIndex] = new int[kLegParamArrayLen];
	personPartsParameters_[kLeftLegIndex][0] = 20;
	personPartsParameters_[kLeftLegIndex][1] = 30;

	//Right Leg
	personPartsParameters_[kRightLegIndex] = new int[kLegParamArrayLen];
	personPartsParameters_[kRightLegIndex][0] = 20;
	personPartsParameters_[kRightLegIndex][1] = 30;

	//Right Arm
	personPartsParameters_[kRightArmIndex] = new int[kArmParamArrayLen];
	personPartsParameters_[kRightArmIndex][0] = 20;

	//Left Arm
	personPartsParameters_[kLeftArmIndex] = new int[kArmParamArrayLen];
	personPartsParameters_[kLeftArmIndex][0] = 20;
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
