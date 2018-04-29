#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("JUMP! by Sayan Bhattacharjee");
	mySound_.load(SOUND_NAME);
	//mySound_.play();
	mySound_.setLoop(true);
	
	playerPartsParameters_ = currentPlayer_.getPerson();
	allPlatforms_.push_back(Platform(platformWidth_, platformHeight_));
	allPlatforms_.push_back(Platform(platformWidth_, platformHeight_, ofGetWindowHeight() * BORDER_WIDTH_FACTOR, ofGetWindowHeight() * BORDER_WIDTH_FACTOR * 2));
	carl_.load(CARL_IMAGE);
	regularFont_.load(REGULAR_FONT_NAME, REGULAR_FONT_SIZE);
	arrowFont_.load(ARROW_FONT_NAME, ARROW_FONT_SIZE);
}

//--------------------------------------------------------------
void ofApp::update(){
	if (allPlatforms_.back().getPlatform().intersects(ofRectangle(
				lastHeadPositionX_ - playerPartsParameters_[Person::kCircleRadiusIndex][0]
				, lastHeadPositionY_ - playerPartsParameters_[Person::kCircleRadiusIndex][0]
				, playerPartsParameters_[Person::kCircleRadiusIndex][0] * 2
				, playerPartsParameters_[Person::kCircleRadiusIndex][0] * 2))) {
		isChangingPlatforms_ = false;
		hasReached_ = true;
		platformIsMovingDown_ = true;
		mouseX_ = SENTIEL_MOUSE_POSITION;
		mouseY_ = SENTIEL_MOUSE_POSITION;
		allPlatforms_.push_back(Platform(platformWidth_, platformHeight_, ofGetWindowHeight() * 2 * BORDER_WIDTH_FACTOR - (ofGetWindowHeight() - allPlatforms_.back().getYPos()), ofGetWindowHeight() * BORDER_WIDTH_FACTOR * 3 - (ofGetWindowHeight() - allPlatforms_.back().getYPos())));
	}
	if ((allPlatforms_.back().getYPos() > lastHeadPositionY_ + playerPartsParameters_[Person::kCircleRadiusIndex][0]) || (lastHeadPositionY_ > ofGetWindowHeight())) {
		isDead_ = true;
		std::exit(0);
	}
	if (platformIsMovingDown_ && allPlatforms_[allPlatforms_.size() - 2].getYPos() > ofGetWindowHeight() * (1-BORDER_WIDTH_FACTOR)) {
		platformIsMovingDown_ = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (currentState_ == MENU) {
		drawMenu();
	} else if (currentState_ == IN_PROGRESS) {
		drawGamePlay();
	} else if (currentState_ == PAUSED) {
		drawPaused();
	} else if (currentState_ == FINISHED) {

	} else if (currentState_ == HIGH_SCORES) {
		drawHighScores();
	}
}

void ofApp::drawMenu() {
	ofSetColor(ofColor::black);
	std::ostringstream menuString;
	menuString << "Welcome to Jump!" << std::endl << std::endl;
	menuString << "Select One:" << std::endl;
	menuString << "1. Play Game" << std::endl;;
	menuString << "2. High Scores" << std::endl;
	std::string quitString = "Press q to exit game";
	regularFont_.drawString(menuString.str(), ofGetWindowWidth() * BORDER_WIDTH_FACTOR, ofGetWindowHeight() * BORDER_WIDTH_FACTOR);
	arrowFont_.drawString("f", ofGetWindowWidth() * BORDER_WIDTH_FACTOR - 1.5 * ARROW_FONT_SIZE, ofGetWindowHeight() * BORDER_WIDTH_FACTOR + REGULAR_FONT_SIZE * (4 + menuOptionHighlighted_));
	regularFont_.drawString(quitString, (ofGetWindowWidth() - REGULAR_FONT_SIZE * quitString.size()) / 2.0, ofGetWindowHeight() * BORDER_WIDTH_FACTOR + 8 * (REGULAR_FONT_SIZE + 1));
}

void ofApp::drawGamePlay() {
	ofSetColor(0, 0, 0);
	std::string scoreText = "Score: " + std::to_string(allPlatforms_.size() - 1);
	ofDrawBitmapString(scoreText, ofGetWindowWidth() * (1 - BORDER_WIDTH_FACTOR) - scoreText.size(), 15);
	ofSetColor(rValue_, gValue_, bValue_);
	if (clock() - startTime_ > 400) {
		rValue_ = rand() % MAX_RGB;
		gValue_ = rand() % MAX_RGB;
		bValue_ = rand() % MAX_RGB;
		startTime_ = clock();
	}
	ofSetColor(ofColor(rValue_, gValue_, bValue_));
	for (int index = 0; index < allPlatforms_.size(); ++index) {
		if (platformIsMovingDown_) {
			allPlatforms_[index].movePlatformDown(0.1);
		}
		ofDrawRectangle(allPlatforms_[index].getPlatform());
	}

	int lastX, lastY;

	if (!isChangingPlatforms_) {
		ofSetColor(ofColor(MAX_RGB - rValue_, MAX_RGB - gValue_, MAX_RGB - bValue_));
		int indexSecondToLast = allPlatforms_.size() - 2;
		lastY = allPlatforms_[indexSecondToLast].getYPos() - (allPlatforms_[indexSecondToLast].getHeight() / 2); //top of the platform
		lastX = allPlatforms_[indexSecondToLast].getXPos() + (allPlatforms_[indexSecondToLast].getLength() / 2); //middle of the platform

		drawLeg(lastX, lastY);
		drawTorso(lastX, lastY);

		if (mouseX_ != SENTIEL_MOUSE_POSITION && mouseY_ != SENTIEL_MOUSE_POSITION) {
			if (mouseX_ < lastX) {
				drawRightArm(lastX, lastY);
			}
			else {
				drawLeftArm(lastX, lastY);
			}
			ofSetColor(ofColor(255, 0, 0, 255 / 4.0));
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
	if ((!isChangingPlatforms_ || hasReached_) && (mouseX_ != SENTIEL_MOUSE_POSITION && mouseY_ != SENTIEL_MOUSE_POSITION)) {
		ofDrawCircle(mouseX_, mouseY_, 15);
	}

	ofSetColor(rValue_, gValue_, bValue_);
	drawHead();
}

void ofApp::drawPaused() {

}

void ofApp::drawHighScores() {

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

	if (isChangingPlatforms_) {
		if (abs(clickedLocations_.back()[0] - lastHeadPositionX_) < 1 && abs(clickedLocations_.back()[1] - lastHeadPositionY_) < 1) {
			hasReached_ = true;
		}

		ofDrawCircle(lastHeadPositionX_, lastHeadPositionY_ - playerPartsParameters_[Person::kCircleRadiusIndex][0], playerPartsParameters_[Person::kCircleRadiusIndex][0]);
		ofSetColor(ofColor::blueSteel);
		ofDrawCircle(lastHeadPositionX_, lastHeadPositionY_ - playerPartsParameters_[Person::kCircleRadiusIndex][0], playerPartsParameters_[Person::kCircleRadiusIndex][0] * kInnerToOuterCircleFactor);

		if (!hasReached_) {
			double differenceX = (clickedLocations_.back()[0] - lastHeadPositionX_) / 1500.0;
			double differenceY = (clickedLocations_.back()[1] - lastHeadPositionY_) / 1500.0;
			double angle = atan(differenceY / differenceX);
			if (differenceX < 0) {
				angle += PI;
			}


			if (abs(differenceX) > abs(0.3 * cos(angle))) {
				lastHeadPositionX_ += differenceX;
				lastHeadPositionY_ += differenceY;
			}
			else {
				lastHeadPositionX_ += 0.3 * cos(angle);
				lastHeadPositionY_ += 0.3 * sin(angle);
			}
		} else {
			lastHeadPositionY_ += 0.04;
		}
	} else {
		carl_.draw(lastHeadPositionX_ - 1.5 * playerPartsParameters_[Person::kCircleRadiusIndex][0], lastHeadPositionY_ - 3 * playerPartsParameters_[Person::kCircleRadiusIndex][0]
			, 3 * playerPartsParameters_[Person::kCircleRadiusIndex][0], 3 * playerPartsParameters_[Person::kCircleRadiusIndex][0]);
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
	std::cout << key << std::endl;
	
	int upperKey = toupper(key);
	if (upperKey == 'Q') {
		std::exit(0);
	}
	if (currentState_ == MENU) {
		if (upperKey == KEY_UP) {
			menuOptionHighlighted_ = menuOptionHighlighted_ == 1 ? 2 : 1;
		} else if (upperKey == KEY_DOWN) {
			menuOptionHighlighted_ = menuOptionHighlighted_ == 1 ? 2 : 1;
		} else if (upperKey == ENTER_KEY) {
			currentState_ = (menuOptionHighlighted_ == 1) ? IN_PROGRESS : HIGH_SCORES;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	if (((x < ofGetWindowWidth() * 0.1) || (x > ofGetWindowWidth() * 0.9)) && !platformIsMovingDown_) {
		mouseX_ = x;
		mouseY_ = y;
	} else {
		mouseX_ = SENTIEL_MOUSE_POSITION;
		mouseY_ = SENTIEL_MOUSE_POSITION;
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if ((!isChangingPlatforms_ || hasReached_ || !platformIsMovingDown_) && ((x < ofGetWindowWidth() * 0.1) || (x > ofGetWindowWidth() * 0.9))) {
		if (x < ofGetWindowWidth() * 0.1) {
			double yChangeFromHead = y - lastHeadPositionY_;
			double xChangeFromHead = x - lastHeadPositionX_;
			double actualChangeFromHead = playerPartsParameters_[Person::kCircleRadiusIndex][0] - lastHeadPositionX_;
			clickedLocations_.push_back({ (double) playerPartsParameters_[Person::kCircleRadiusIndex][0] , y + yChangeFromHead / (abs(yChangeFromHead)) *  actualChangeFromHead / xChangeFromHead});
		} else {
			double yChangeFromHead = y - lastHeadPositionY_;
			double xChangeFromHead = lastHeadPositionX_ - x;
			double actualChangeFromHead = ofGetWindowWidth() - playerPartsParameters_[Person::kCircleRadiusIndex][0] - lastHeadPositionX_;
			clickedLocations_.push_back({ (double)(ofGetWindowWidth() - playerPartsParameters_[Person::kCircleRadiusIndex][0]) , y + yChangeFromHead / (abs(yChangeFromHead)) *  actualChangeFromHead / xChangeFromHead });
		}
		isChangingPlatforms_ = true;
		hasReached_ = false;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
	mouseMoved(x, y);
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
