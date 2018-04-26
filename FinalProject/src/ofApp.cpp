#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	mySound_.load(SOUND_NAME);
	mySound_.play();
	mySound_.setLoop(true);
	
	playerPartsParameters_ = currentPlayer_.getPerson();
	allPlatforms.push_back(Platform());
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	int RColor = rand() % MAX_RGB;
	int GColor = rand() % MAX_RGB;
	int BColor = rand() % MAX_RGB;
	ofSetColor(ofColor(RColor, GColor, BColor));
	for (int index = 0; index < allPlatforms.size(); ++index) {
		ofDrawRectangle(allPlatforms[index].getPlatform());
	}
	
	ofSetColor(ofColor(MAX_RGB - RColor, MAX_RGB - GColor, MAX_RGB - BColor));
	int lastY = allPlatforms.back().getYPos() - (allPlatforms.back().getHeight() / 2); //top of the platform
	int lastX = allPlatforms.back().getXPos() + (allPlatforms.back().getLength() / 2); //middle of the platform
	
	//draw left leg
	ofDrawLine(lastX, lastY - playerPartsParameters_[Person::kLeftLegIndex][1],
		lastX - playerPartsParameters_[Person::kLeftLegIndex][0], lastY);
	//draw right leg
	ofDrawLine(lastX, lastY - playerPartsParameters_[Person::kRightLegIndex][1],
		lastX + playerPartsParameters_[Person::kRightLegIndex][0], lastY);
	
	//reset Y
	lastY = lastY - max(playerPartsParameters_[Person::kLeftLegIndex][1], playerPartsParameters_[Person::kRightLegIndex][1]);
	
	//draw torso
	ofDrawLine(lastX, lastY, lastX, lastY - playerPartsParameters_[Person::kTorsoIndex][0]);

	//draw Left Arm
	ofDrawLine(lastX, lastY - (playerPartsParameters_[Person::kTorsoIndex][0] / 2)
		, lastX - playerPartsParameters_[Person::kLeftArmIndex][0], lastY - (playerPartsParameters_[Person::kTorsoIndex][0] / 2));

	//draw Right Arm
	ofDrawLine(lastX, lastY - (playerPartsParameters_[Person::kTorsoIndex][0] / 2)
		, lastX + playerPartsParameters_[Person::kRightArmIndex][0], lastY - (playerPartsParameters_[Person::kTorsoIndex][0] / 2));

	lastY -= playerPartsParameters_[Person::kTorsoIndex][0];
	//draw head
	ofDrawCircle(lastX, lastY - playerPartsParameters_[Person::kCircleRadiusIndex][0], playerPartsParameters_[Person::kCircleRadiusIndex][0]);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
