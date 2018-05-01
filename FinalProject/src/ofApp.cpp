#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("JUMP! by Sayan Bhattacharjee");
	mySound_.load(SOUND_NAME);
	//mySound_.play();
	mySound_.setLoop(true);
	player = new Person();
	playerPartsParameters_ = player->getPerson();
	allPlatforms_.push_back(Platform(platformWidth_, platformHeight_));
	allPlatforms_.push_back(Platform(platformWidth_, platformHeight_, ofGetWindowHeight() * BORDER_WIDTH_FACTOR * 2, ofGetWindowHeight() * BORDER_WIDTH_FACTOR * 3));
	carl_.load(CARL_IMAGE);
	regularFont_.load(REGULAR_FONT_NAME, REGULAR_FONT_SIZE);
	arrowFont_.load(ARROW_FONT_NAME, ARROW_FONT_SIZE);
	loadHighScores();

}

//--------------------------------------------------------------
void ofApp::update(){
	if (headAndPlatformIntersects()) {
		movePlayerToNextPlatform();
	}

	//when the player has died
	if ((allPlatforms_.back().getYPos() > lastHeadPositionY_ + playerPartsParameters_[Person::kCircleRadiusIndex][0])		//highest point of platform is lower than lowest point of circle head
		|| (lastHeadPositionY_ > ofGetWindowHeight())) {																	//circle head is below the bottom of window
		isDead_ = true;
		currentState_ = FINISHED;
	}

	//defining when the platform has moved down enough
	if (platformIsMovingDown_ 
		&& (allPlatforms_[allPlatforms_.size() - 2].getYPos() > (ofGetWindowHeight() * (1-BORDER_WIDTH_FACTOR)))) {			//old platform inside the border zone
		platformIsMovingDown_ = false;
	}
}

void ofApp::movePlayerToNextPlatform() {
	isChangingPlatforms_ = false;
	hasReached_ = true;
	platformIsMovingDown_ = true;
	mouseX_ = SENTIEL_MOUSE_POSITION;
	mouseY_ = SENTIEL_MOUSE_POSITION;
	
	double currentPlatformHeight = (ofGetWindowHeight() - allPlatforms_.back().getYPos());
	allPlatforms_.push_back(Platform(
			platformWidth_
			, platformHeight_
			, ofGetWindowHeight() * minPlatformHeightFactor * BORDER_WIDTH_FACTOR - currentPlatformHeight
			, ofGetWindowHeight() * BORDER_WIDTH_FACTOR * maxPlatformHeightFactor - currentPlatformHeight));
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::beige, ofColor::aliceBlue);
	if (currentState_ == MENU) {
		drawMenu();
	} else if (currentState_ == IN_PROGRESS) {
		drawGamePlay();
	} else if (currentState_ == PAUSED) {
		drawPaused();
	} else if (currentState_ == FINISHED) {
		drawGameOver();
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
	
	regularFont_.drawString(
		menuString.str()
		, ofGetWindowWidth() * BORDER_WIDTH_FACTOR
		, ofGetWindowHeight() * BORDER_WIDTH_FACTOR);
	
	double arrowXChangeFactor = 1.5;
	double newLineYFudgeFactor = 4;
	arrowFont_.drawString(
		"f"																			//arrow
		, ofGetWindowWidth() * BORDER_WIDTH_FACTOR - arrowXChangeFactor * ARROW_FONT_SIZE
		, ofGetWindowHeight() * BORDER_WIDTH_FACTOR + REGULAR_FONT_SIZE * (newLineYFudgeFactor + menuOptionHighlighted_));
	
	double centeredQuitStringX = (ofGetWindowWidth() - REGULAR_FONT_SIZE * quitString.size()) / 2.0;
	int numOptions = 2;
	regularFont_.drawString(
		quitString
		, centeredQuitStringX
		, ofGetWindowHeight() * BORDER_WIDTH_FACTOR + numOptions * newLineYFudgeFactor * (REGULAR_FONT_SIZE + 1));
}

void ofApp::drawGamePlay() {
	drawScore();
	setColor();
	drawAllPlatforms();
	if (!isChangingPlatforms_) {
		drawBody();
	}
	//mark where mouse is
	if ((!isChangingPlatforms_ || hasReached_) && (mouseX_ != SENTIEL_MOUSE_POSITION && mouseY_ != SENTIEL_MOUSE_POSITION)) {
		ofDrawCircle(mouseX_, mouseY_, mouseMarkerRadius);
	}

	drawHead();
}

void ofApp::drawPaused() {

}

void ofApp::drawHighScores() {
	regularFont_.drawString(getHighScoresString(), ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0);
}

void ofApp::drawGameOver() {
	drawMenu();
}

std::string ofApp::getHighScoresString() {
	std::ostringstream highScoresOutput;
	for (int rank = 1; rank <= highScores_.size(); ++rank) {
		highScoresOutput << rank << ". " << highScores_[rank - 1] << "\n";
	}
	return highScoresOutput.str();
}

void ofApp::loadHighScores() {
	ifstream highScoresReader("HighScores.txt");
	while (!highScoresReader.fail() && !highScoresReader.eof()) {
		int score;
		highScoresReader >> score;
		highScores_.push_back(score);
	}
	std::sort(highScores_.begin(), highScores_.end());
	std::reverse(highScores_.begin(), highScores_.end());
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
	ofSetColor(MAX_RGB - rValue_, MAX_RGB - gValue_, MAX_RGB - bValue_);
	if (isChangingPlatforms_) {
		if (abs(lastClickedLocation_[0] - lastHeadPositionX_) < 1 && abs(lastClickedLocation_[1] - lastHeadPositionY_) < 1) {
			hasReached_ = true;
		}

		ofDrawCircle(lastHeadPositionX_, lastHeadPositionY_ - playerPartsParameters_[Person::kCircleRadiusIndex][0], playerPartsParameters_[Person::kCircleRadiusIndex][0]);
		ofSetColor(ofColor::blueSteel);
		ofDrawCircle(lastHeadPositionX_, lastHeadPositionY_ - playerPartsParameters_[Person::kCircleRadiusIndex][0], playerPartsParameters_[Person::kCircleRadiusIndex][0] * kInnerToOuterCircleFactor);

		if (!hasReached_) {
			double differenceX = (lastClickedLocation_[0] - lastHeadPositionX_) / 1500.0;
			double differenceY = (lastClickedLocation_[1] - lastHeadPositionY_) / 1500.0;
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
		ofSetColor(ofColor(MAX_RGB - rValue_, MAX_RGB - gValue_, MAX_RGB - bValue_));
		carl_.draw(lastHeadPositionX_ - 1.5 * playerPartsParameters_[Person::kCircleRadiusIndex][0], lastHeadPositionY_ - 3 * playerPartsParameters_[Person::kCircleRadiusIndex][0]
			, 3 * playerPartsParameters_[Person::kCircleRadiusIndex][0], 3 * playerPartsParameters_[Person::kCircleRadiusIndex][0]);
	}
}

void ofApp::drawDottedLine(int startX, int startY, int endX, int endY) {
	double currentX = startX;
	double currentY = startY;
	double differenceX = (endX - startX) / (10.0 * NUM_DOTS);
	double differenceY = (endY - startY) / (10.0 * NUM_DOTS);
	double angle = atan(differenceY / differenceX);
	if (differenceX < 0) {
		angle += PI;
	}
	for (int dashNumber = 0; dashNumber < NUM_DOTS; ++dashNumber) {
		ofDrawCircle(currentX, currentY, 2);
		currentX += differenceX + 5 * cos(angle);
		currentY += differenceY + 5 * sin(angle);
	}
}

bool ofApp::headAndPlatformIntersects() {
	ofRectangle headRectangleCircumscribed = ofRectangle(
		lastHeadPositionX_ - playerPartsParameters_[Person::kCircleRadiusIndex][0]
		, lastHeadPositionY_ - playerPartsParameters_[Person::kCircleRadiusIndex][0]
		, playerPartsParameters_[Person::kCircleRadiusIndex][0] * 2
		, playerPartsParameters_[Person::kCircleRadiusIndex][0] * 2);
	return allPlatforms_.back().getPlatform().intersects(headRectangleCircumscribed);
}

void ofApp::setColor() {
	if (clock() - startTime_ > 400) {
		rValue_ = rand() % MAX_RGB;
		gValue_ = rand() % MAX_RGB;
		bValue_ = rand() % MAX_RGB;
		startTime_ = clock();
	}
	ofSetColor(ofColor(rValue_, gValue_, bValue_));
}

void ofApp::drawAllPlatforms() {
	for (int index = 0; index < allPlatforms_.size(); ++index) {
		//moving them down if necessary
		if (platformIsMovingDown_) {
			allPlatforms_[index].movePlatformDown(0.1);							
		}
		ofDrawRectangle(allPlatforms_[index].getPlatform());
	}
}

void ofApp::drawBody() {
	int lastX, lastY;
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

void ofApp::drawScore() {
	ofSetColor(0, 0, 0);
	std::string scoreText = "Score: " + std::to_string(allPlatforms_.size() - 1);
	ofDrawBitmapString(scoreText, ofGetWindowWidth() * (1 - BORDER_WIDTH_FACTOR) - scoreText.size(), TOP_RIGHT_Y);
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	std::cout << key << std::endl;
	
	int upperKey = toupper(key);
	if (upperKey == 'Q') {
		clearAndExit();
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
		std::cout << !isChangingPlatforms_ << std::endl; 
		std::cout << hasReached_ << std::endl;
		std::cout << !platformIsMovingDown_ << std::endl;

		double newXLocation, newYLocation;
		if (x < ofGetWindowWidth() * 0.1) {
			double yChangeFromHead = y - lastHeadPositionY_;
			double xChangeFromHead = x - lastHeadPositionX_;
			double actualChangeFromHead = playerPartsParameters_[Person::kCircleRadiusIndex][0] - lastHeadPositionX_;
			newXLocation = (double)playerPartsParameters_[Person::kCircleRadiusIndex][0];
			newYLocation = y + yChangeFromHead / (abs(yChangeFromHead)) *  actualChangeFromHead / xChangeFromHead;
			lastClickedLocation_[0] = newXLocation;
			lastClickedLocation_[1] = newYLocation;
		} else {
			double yChangeFromHead = y - lastHeadPositionY_;
			double xChangeFromHead = lastHeadPositionX_ - x;
			double actualChangeFromHead = ofGetWindowWidth() - playerPartsParameters_[Person::kCircleRadiusIndex][0] - lastHeadPositionX_;
			newXLocation = (double)(ofGetWindowWidth() - playerPartsParameters_[Person::kCircleRadiusIndex][0]);
			newYLocation = y + yChangeFromHead / (abs(yChangeFromHead)) *  actualChangeFromHead / xChangeFromHead;
			lastClickedLocation_[0] = newXLocation;
			lastClickedLocation_[1] = newYLocation;
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

void ofApp::clearAndExit() {
	delete player;
	std::exit(0);
}