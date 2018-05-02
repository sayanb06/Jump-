#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("JUMP! by Sayan Bhattacharjee");
	mySound_.load(SOUND_NAME);
	mySound_.play();
	mySound_.setLoop(true);
	player = new Person();
	playerPartsParameters_ = player->getPerson();
	allPlatforms_.push_back(Platform(platformWidth_, platformHeight_));
	allPlatforms_.push_back(Platform(platformWidth_, platformHeight_,
		ofGetWindowHeight() * BORDER_WIDTH_FACTOR * minPlatformHeightFactor, ofGetWindowHeight() * BORDER_WIDTH_FACTOR * maxPlatformHeightFactor));
	carl_.load(CARL_IMAGE);
	regularFont_.load(REGULAR_FONT_NAME, REGULAR_FONT_SIZE);
	arrowFont_.load(ARROW_FONT_NAME, ARROW_FONT_SIZE);
	loadHighScores();
}

//--------------------------------------------------------------
void ofApp::update(){
	if (currentState_ == IN_PROGRESS) {
		updateGamePlay();
	}
}

void ofApp::movePlayerToNextPlatform() {
	isChangingPlatforms_ = false;
	hasReachedBorder_ = true;
	platformsMovingDown_ = true;
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
		mySound_.setPaused(false);
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
	int numMenuOptions = 2;
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
	regularFont_.drawString(
		quitString
		, centeredQuitStringX
		, ofGetWindowHeight() * BORDER_WIDTH_FACTOR + numMenuOptions * newLineYFudgeFactor * (REGULAR_FONT_SIZE + 1));
}

void ofApp::drawGamePlay() {
	drawScore();
	setColor();
	drawAllPlatforms();
	if (!isChangingPlatforms_) {
		drawBody();
	}
	
	//mark where mouse is
	if ((!isChangingPlatforms_ || hasReachedBorder_) && (mouseX_ != SENTIEL_MOUSE_POSITION && mouseY_ != SENTIEL_MOUSE_POSITION)) {
		ofSetColor(MAX_RGB, 0, 0, MAX_RGB * quarterColorOpacity);
		ofDrawCircle(mouseX_, mouseY_, mouseMarkerRadius);
	}

	drawHead();
}

void ofApp::drawPaused() {
	drawGamePlay();
	mySound_.setPaused(true);
	std::string output = "Press P To Unpause";
	regularFont_.drawString(output, (ofGetWindowWidth() - REGULAR_FONT_SIZE * output.size()) / 2, (ofGetWindowHeight() - REGULAR_FONT_SIZE) / 2);
}

void ofApp::drawHighScores() {
	regularFont_.drawString("PRESS M TO RETURN TO MAIN MENU", 0, ofGetWindowHeight() * BORDER_WIDTH_FACTOR);
	regularFont_.drawString(getHighScoresString(), ofGetWindowWidth() * 0, ofGetWindowHeight() * BORDER_WIDTH_FACTOR + REGULAR_FONT_SIZE * 2);
}

void ofApp::drawGameOver() {
	std::string outputContents = "PRESS R TO RESTART\nPRESS M TO VIEW MENU\n" + getHighScoresString();
	regularFont_.drawString(outputContents,  ofGetWindowWidth() * BORDER_WIDTH_FACTOR, ofGetWindowHeight() * BORDER_WIDTH_FACTOR);
}

std::string ofApp::getHighScoresString() {
	if (highScores_.empty()) {
		return "There are no high scores yet. :(";
	}

	std::ostringstream highScoresOutput;
	highScoresOutput << "High Scores:" << std::endl;
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
		if (score >= 1) {
			highScores_.push_back(score);
		}
	}
	addScoreAndSort();
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
	ofDrawLine(lastX, lastY - (playerPartsParameters_[Person::kTorsoIndex][0] / 2.0)
		, lastX - playerPartsParameters_[Person::kArmIndex][0], lastY - (playerPartsParameters_[Person::kTorsoIndex][0] / 2.0));
}

void ofApp::drawHead() {
	ofSetColor(MAX_RGB - rValue_, MAX_RGB - gValue_, MAX_RGB - bValue_);
	
	//check if the head should be a picture or a ball, depending on if the person is moving
	if (isChangingPlatforms_) {
		//hasReached the border
		if (abs(lastClickedLocation_[0] - lastHeadPositionX_) < 1 
			&& abs(lastClickedLocation_[1] - lastHeadPositionY_) < 1) {
			hasReachedBorder_ = true;

		}

		//draw head as ball
		ofDrawCircle(lastHeadPositionX_, lastHeadPositionY_ - playerPartsParameters_[Person::kCircleRadiusIndex][0], playerPartsParameters_[Person::kCircleRadiusIndex][0]);
		ofSetColor(ofColor::blueSteel);
		ofDrawCircle(lastHeadPositionX_, lastHeadPositionY_ - playerPartsParameters_[Person::kCircleRadiusIndex][0], playerPartsParameters_[Person::kCircleRadiusIndex][0] * kInnerToOuterCircleFactor);
	} else {
		
		//draw head as picture
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
	Platform currentPlatform = allPlatforms_.back();
	double topBorderY = currentPlatform.getYPos();
	double bottomBorderY = topBorderY + currentPlatform.getHeight();
	double leftBorderX = currentPlatform.getXPos();
	double rightBorderX = leftBorderX + currentPlatform.getLength();
	double circleRightEndpt = lastHeadPositionX_ + playerPartsParameters_[Person::kCircleRadiusIndex][0];
	double circleLeftEndpt = lastHeadPositionX_ - playerPartsParameters_[Person::kCircleRadiusIndex][0];
	double circleUpEndpt = lastHeadPositionY_ + playerPartsParameters_[Person::kCircleRadiusIndex][0];
	double circleDownEndpt = lastHeadPositionY_ - playerPartsParameters_[Person::kCircleRadiusIndex][0];

	if ((circleRightEndpt <= rightBorderX && circleRightEndpt >= leftBorderX
		|| circleLeftEndpt <= rightBorderX && circleLeftEndpt >= leftBorderX)
		&& (circleUpEndpt <= bottomBorderY && circleUpEndpt >= topBorderY
		|| circleDownEndpt <= bottomBorderY && circleDownEndpt >= topBorderY)) {
		return true;
	}
	return false;
	
	//check if the rectangle and inscribed circle intersect
	
}

void ofApp::setColor() {
	if (clock() - colorResetTime_ > 400) {
		rValue_ = rand() % MAX_RGB;
		gValue_ = rand() % MAX_RGB;
		bValue_ = rand() % MAX_RGB;
		colorResetTime_ = clock();
	}
	ofSetColor(ofColor(rValue_, gValue_, bValue_));
}

void ofApp::drawAllPlatforms() {
	for (int index = 0; index < allPlatforms_.size(); ++index) {
		//moving them down if necessary
		if (platformsMovingDown_ && currentState_ == IN_PROGRESS) {
			allPlatforms_[index].movePlatformDown(platformMoveDownRate);
		}
		ofDrawRectangle(allPlatforms_[index].getPlatform());
	}
}

void ofApp::drawBody() {
	int lastX, lastY;
	ofSetColor(ofColor(MAX_RGB - rValue_, MAX_RGB - gValue_, MAX_RGB - bValue_));
	int olderPlatformIndex = allPlatforms_.size() - 2;
	//top of the platform
	lastY = allPlatforms_[olderPlatformIndex].getYPos() - (allPlatforms_[olderPlatformIndex].getHeight() / 2);
	//middle of the platform
	lastX = allPlatforms_[olderPlatformIndex].getXPos() + (allPlatforms_[olderPlatformIndex].getLength() / 2);

	//draw bottom half of body
	drawLeg(lastX, lastY);
	drawTorso(lastX, lastY);
	
	//check if the mouse is within the border and if dotted line needs to be drawn
	if (mouseX_ != SENTIEL_MOUSE_POSITION && mouseY_ != SENTIEL_MOUSE_POSITION) {
		//choose which arm to draw	
		if (mouseX_ < lastX) {
			drawRightArm(lastX, lastY);
		} else {
			drawLeftArm(lastX, lastY);
		}
		
		//draw dotted line as other arm
		drawDottedLine(lastX, lastY - playerPartsParameters_[Person::kTorsoIndex][0] / 2, mouseX_, mouseY_);
	} else {
		drawRightArm(lastX, lastY);
		drawLeftArm(lastX, lastY);
	}
	
	//update head position for the rest of the program
	lastY -= playerPartsParameters_[Person::kTorsoIndex][0];
	lastHeadPositionX_ = lastX;
	lastHeadPositionY_ = lastY;
}

void ofApp::drawScore() {
	ofSetColor(0, 0, 0);
	std::string scoreText = "Score: " + std::to_string(allPlatforms_.size() - 1);
	ofDrawBitmapString(scoreText, ofGetWindowWidth() * (1 - BORDER_WIDTH_FACTOR) - scoreText.size(), TOP_RIGHT_Y);
}

void ofApp::updateGamePlay() {
	if (headAndPlatformIntersects()) {
		movePlayerToNextPlatform();
	}

	//when the player has died
	if ((allPlatforms_.back().getYPos() > lastHeadPositionY_ + playerPartsParameters_[Person::kCircleRadiusIndex][0])		//highest point of platform is lower than lowest point of circle head
		|| (lastHeadPositionY_ > ofGetWindowHeight())) {																	//circle head is below the bottom of window
		isChangingPlatforms_ = false;
		platformsMovingDown_ = false;
		hasReachedBorder_ = false;

		addScoreAndSort(allPlatforms_.size() - 1);
		allPlatforms_.clear();
		currentState_ = FINISHED;
	}

	//defining when the platform has moved down enough
	if (platformsMovingDown_
		&& (allPlatforms_[allPlatforms_.size() - 2].getYPos() > (ofGetWindowHeight() * (1 - BORDER_WIDTH_FACTOR)))) {			//old platform inside the border zone
		platformsMovingDown_ = false;
	}

	if (isChangingPlatforms_) {
		if (!hasReachedBorder_) {
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
	}

	if ((allPlatforms_.size() > numPlatsBeforeRandom) && ((clock() - platformMoveTime_) > 50)) {
		allPlatforms_.back().moveXDirection(6);
		platformMoveTime_ = clock();
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	int upperKey = toupper(key);
	
	if (upperKey == 'Q') {
		clearAndExit();
	}
	
	if (currentState_ == MENU) {
		//switch menu Option
		if (upperKey == KEY_UP) {
			menuOptionHighlighted_ = menuOptionHighlighted_ == 1 ? 2 : 1;
		} else if (upperKey == KEY_DOWN) {
			menuOptionHighlighted_ = menuOptionHighlighted_ == 1 ? 2 : 1;
		} else if (upperKey == ENTER_KEY) {
			//if currentState is play game, play, otherwise show high scores
			currentState_ = (menuOptionHighlighted_ == 1) ? IN_PROGRESS : HIGH_SCORES;
		}
	} else if (currentState_ == FINISHED) {
		if (upperKey == 'R') {
			resetGamePlay();
		} else if (upperKey == 'M') {
			currentState_ = MENU;
		}
	} else if (currentState_ == IN_PROGRESS || currentState_ == PAUSED) {
		if (upperKey == 'P') {
			currentState_ = currentState_ == PAUSED ? IN_PROGRESS : PAUSED;
		} else if (upperKey == 'R') {
			addScoreAndSort(allPlatforms_.size() - 1);
			resetGamePlay();
		}
	} else if (currentState_ == HIGH_SCORES) {
		if (upperKey == 'M') {
			currentState_ = MENU;
		}
	}
}

void ofApp::resetGamePlay() {
	allPlatforms_.clear();
	allPlatforms_.push_back(Platform(platformWidth_, platformHeight_));
	allPlatforms_.push_back(Platform(platformWidth_
		, platformHeight_,
		ofGetWindowHeight() * BORDER_WIDTH_FACTOR * minPlatformHeightFactor
		, ofGetWindowHeight() * BORDER_WIDTH_FACTOR * maxPlatformHeightFactor));
	drawBody();
	platformWidth_ = SENTIEL_PLATFORM_WIDTH;
	currentState_ = IN_PROGRESS;
}

void ofApp::addScoreAndSort(int score) {
	if (score != 0) {
		highScores_.push_back(score);
	}
	//sort the high scores
	std::sort(highScores_.begin(), highScores_.end());
	std::reverse(highScores_.begin(), highScores_.end());
	if (highScores_.size() > 10) {
		highScores_.pop_back();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	bool withinBorders = ((x < ofGetWindowWidth() * BORDER_WIDTH_FACTOR)
		|| (x > ofGetWindowWidth() * (1 - BORDER_WIDTH_FACTOR)));
	bool notMovingAlready = (!isChangingPlatforms_ || hasReachedBorder_);

	if (withinBorders && notMovingAlready && !platformsMovingDown_) {
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
	bool withinBorders = ((x < ofGetWindowWidth() * BORDER_WIDTH_FACTOR) 
						|| (x > ofGetWindowWidth() * (1 - BORDER_WIDTH_FACTOR)));
	bool notMovingAlready = (!isChangingPlatforms_ || hasReachedBorder_);
	
	if (withinBorders && notMovingAlready && !platformsMovingDown_ && currentState_ == IN_PROGRESS) {
		double newXLocation, newYLocation;
		if (x < ofGetWindowWidth() * BORDER_WIDTH_FACTOR) {
			double yChangeFromHead = y - lastHeadPositionY_;
			double xChangeFromHead = x - lastHeadPositionX_;
			double actualChangeFromHead = playerPartsParameters_[Person::kCircleRadiusIndex][0] - lastHeadPositionX_;
			newXLocation = (double)playerPartsParameters_[Person::kCircleRadiusIndex][0];
			newYLocation = y + yChangeFromHead / (abs(yChangeFromHead)) *  actualChangeFromHead / xChangeFromHead;
		} else {
			double yChangeFromHead = y - lastHeadPositionY_;
			double xChangeFromHead = lastHeadPositionX_ - x;
			double actualChangeFromHead = ofGetWindowWidth() - playerPartsParameters_[Person::kCircleRadiusIndex][0] - lastHeadPositionX_;
			newXLocation = (double)(ofGetWindowWidth() - playerPartsParameters_[Person::kCircleRadiusIndex][0]);
			newYLocation = y + yChangeFromHead / (abs(yChangeFromHead)) *  actualChangeFromHead / xChangeFromHead;
		}

		lastClickedLocation_[0] = newXLocation;
		lastClickedLocation_[1] = newYLocation;
		//indicates the user has started to move
		isChangingPlatforms_ = true;
		hasReachedBorder_ = false;
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
	ofstream updateHighScores("HighScores.txt");
	updateHighScores.clear();
	for (auto orderedHighScore = highScores_.begin(); orderedHighScore != highScores_.end(); ++orderedHighScore) {
		updateHighScores << *orderedHighScore << std::endl;
	}
	std::exit(0);
}