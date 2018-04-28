#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	mySound_.load(SOUND_NAME);
	//mySound_.play();
	mySound_.setLoop(true);
	
	playerPartsParameters_ = currentPlayer_.getPerson();
	allPlatforms.push_back(Platform());
	carl_.load(CARL_IMAGE);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	if (clock() - startTime_ > 400) {
		rValue_ = rand() % MAX_RGB;
		gValue_ = rand() % MAX_RGB;
		bValue_ = rand() % MAX_RGB;
		startTime_ = clock();
	}
	ofSetColor(ofColor(rValue_, gValue_, bValue_));
	for (int index = 0; index < allPlatforms.size(); ++index) {
		ofDrawRectangle(allPlatforms[index].getPlatform());
	}
	
	int lastX, lastY;
	
	if (!isMoving_) {
		ofSetColor(ofColor(MAX_RGB - rValue_, MAX_RGB - gValue_, MAX_RGB - bValue_));
		lastY = allPlatforms.back().getYPos() - (allPlatforms.back().getHeight() / 2); //top of the platform
		lastX = allPlatforms.back().getXPos() + (allPlatforms.back().getLength() / 2); //middle of the platform

		drawLeg(lastX, lastY);
		drawTorso(lastX, lastY);

		ofSetColor(ofColor(255, 0, 0, 255 / 4.0));
		if (mouseX_ != SENTIEL_MOUSE_POSITION && mouseY_ != SENTIEL_MOUSE_POSITION) {
			if (mouseX_ < lastX) {
				drawRightArm(lastX, lastY);
			}
			else {
				drawLeftArm(lastX, lastY);
			}
			ofDrawCircle(mouseX_, mouseY_, 15);
			drawDottedLine(lastX, lastY - playerPartsParameters_[Person::kTorsoIndex][0] / 2, mouseX_, mouseY_);
		}
		else {
			drawRightArm(lastX, lastY);
			drawLeftArm(lastX, lastY);
		}
		lastY -= playerPartsParameters_[Person::kTorsoIndex][0];
		lastHeadPositionX_ = lastX;
		lastHeadPositionY_ = lastY;
	}

	ofSetColor(rValue_, gValue_, bValue_);
	drawHead();
}

void ofApp::drawLeg(int &lastX, int &lastY) {
	//draw left leg
	ofDrawLine(lastX, lastY - playerPartsParameters_[Person::kLegIndex][1],
		lastX - playerPartsParameters_[Person::kLegIndex][0], lastY);
	//draw right leg
	ofDrawLine(lastX, lastY - playerPartsParameters_[Person::kLegIndex][1],
		lastX + playerPartsParameters_[Person::kLegIndex][0], lastY);
	
	//reset Y
	lastY -= max(playerPartsParameters_[Person::kLegIndex][1], playerPartsParameters_[Person::kLegIndex][1]);
}

void ofApp::drawTorso(int lastX, int lastY) {
	ofDrawLine(lastX, lastY, lastX, lastY - playerPartsParameters_[Person::kTorsoIndex][0]);
}

void ofApp::drawRightArm(int lastX, int lastY) {
	ofDrawLine(lastX, lastY - (playerPartsParameters_[Person::kTorsoIndex][0] / 2)
		, lastX + playerPartsParameters_[Person::kArmIndex][0], lastY - (playerPartsParameters_[Person::kTorsoIndex][0] / 2));
}

void ofApp::drawLeftArm(int lastX, int lastY) {
	ofDrawLine(lastX, lastY - (playerPartsParameters_[Person::kTorsoIndex][0] / 2)
		, lastX - playerPartsParameters_[Person::kArmIndex][0], lastY - (playerPartsParameters_[Person::kTorsoIndex][0] / 2));
}

void ofApp::drawHead() {
	ofDrawCircle(lastHeadPositionX_, lastHeadPositionY_ - playerPartsParameters_[Person::kCircleRadiusIndex][0], playerPartsParameters_[Person::kCircleRadiusIndex][0]);
	carl_.draw(lastHeadPositionX_ - 1.5 * playerPartsParameters_[Person::kCircleRadiusIndex][0], lastHeadPositionY_ - 3 * playerPartsParameters_[Person::kCircleRadiusIndex][0]
		, 3 * playerPartsParameters_[Person::kCircleRadiusIndex][0], 3 * playerPartsParameters_[Person::kCircleRadiusIndex][0]);

	if (isMoving_) {
		double differenceX = (clickedLocations_.back()[0] - lastHeadPositionX_) / 1000.0;
		double differenceY = (clickedLocations_.back()[1] - lastHeadPositionX_) / 1000.0;
		double angle = atan(differenceY / differenceX);
		if (differenceX < 0) {
			angle += PI;
		}

		if (abs(differenceX) > abs(3 * cos(angle))) {
			lastHeadPositionX_ += differenceX;
			lastHeadPositionY_ += differenceY;
		} else {
			lastHeadPositionX_ += 3 * cos(angle);
			lastHeadPositionY_ += 3 * sin(angle);
		}
	}
}

void ofApp::drawDottedLine(int startX, int startY, int endX, int endY) {
	double currentX = startX;
	double currentY = startY;
	double differenceX = (endX - startX) / (10.0 * kDottedNumSeparators);
	double differenceY = (endY - startY) / (10.0 * kDottedNumSeparators);
	double angle = atan(differenceY / differenceX);
	if (differenceX < 0) {
		angle += PI;
	}
	for (int dashNumber = 0; dashNumber < kDottedNumSeparators; ++dashNumber) {
		ofDrawCircle(currentX, currentY, 2);
		currentX += differenceX + 5 * cos(angle);
		currentY += differenceY + 5 * sin(angle);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	if ((x < ofGetWindowWidth() * 0.1) || (x > ofGetWindowWidth() * 0.9)) {
		mouseX_ = x;
		mouseY_ = y;
	} else {
		mouseX_ = SENTIEL_MOUSE_POSITION;
		mouseY_ = SENTIEL_MOUSE_POSITION;
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (!isMoving_ && (x < ofGetWindowWidth() * 0.1) || (x > ofGetWindowWidth() * 0.9)) {
		clickedLocations_.push_back({ x, y });
		isMoving_ = true;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
	if ((x < ofGetWindowWidth() * 0.1) || (x > ofGetWindowWidth() * 0.9)) {
		mouseX_ = x;
		mouseY_ = y;
	}
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
	mouseX_ = SENTIEL_MOUSE_POSITION;
	mouseY_ = SENTIEL_MOUSE_POSITION;
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
