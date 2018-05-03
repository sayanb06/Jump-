#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("JUMP! by Sayan Bhattacharjee");

	//sound
	levelsSound_.load(SOUND_NAME);
	levelsSound_.play();
	levelsSound_.setLoop(true);
	
	//pictures
	evansPicture_.load(EVANS_IMAGE);
	background_.loadImage("oceanBackground.jpg");
	
	//fonts
	regularFont_.load(REGULAR_FONT_NAME, REGULAR_FONT_SIZE);
	arrowFont_.load(ARROW_FONT_NAME, ARROW_FONT_SIZE);
	regularFontSmall_.load(REGULAR_FONT_NAME, REGULAR_FONT_SMALL_SIZE);
	
	//high scores
	loadHighScores();

	player = new Person();
	playerPartsParameters_ = player->getPerson();
	ofSetLineWidth(BODY_WIDTH);
}

void ofApp::update(){
	if (currentState_ == IN_PROGRESS) {
		updateGamePlay();
	}
}


void ofApp::updateGamePlay() {
	if (headAndPlatformIntersects()) {
		movePlayerToNextPlatform();
	}

	//when the player has died
	//highest point of platform is lower than lowest point of circle head 
	//OR circle head is below the bottom of window
	if ((allPlatforms_.back().getYPos() 
			> lastHeadPositionY_ + playerPartsParameters_[Person::kCircleRadiusIndex][0])		
		|| (lastHeadPositionY_ > ofGetWindowHeight())) {																	
		isChangingPlatforms_ = false;
		platformsMovingDown_ = false;
		hasReachedBorder_ = false;

		addScoreUpdateHS(currentScore_);
		allPlatforms_.clear();
		currentState_ = GAME_OVER;
	}

	//defining when the platform has moved down enough
	//old platform inside the border zone
	if (platformsMovingDown_
		&& (allPlatforms_[allPlatforms_.size() - 2].getYPos() 
			> (ofGetWindowHeight() * (1 - BORDER_WIDTH_FACTOR)))) {			
		platformsMovingDown_ = false;
	}

	if (isChangingPlatforms_) {
		if (!hasReachedBorder_) {
			double differenceX = (lastClickedLocation_[0] - lastHeadPositionX_) / factorHeadMoving;
			double differenceY = (lastClickedLocation_[1] - lastHeadPositionY_) / factorHeadMoving;
			double angle = getAngle(differenceX, differenceY);
			
			//check if going by difference or the 
			//min head move distance will get the person there faster
			if (abs(differenceX) > abs(minHeadMoveDistance * cos(angle))) {
				lastHeadPositionX_ += differenceX;
				lastHeadPositionY_ += differenceY;
			} else {
				lastHeadPositionX_ += minHeadMoveDistance * cos(angle);
				lastHeadPositionY_ += minHeadMoveDistance * sin(angle);
			}
		} else {
			//moving down only in Y direction
			lastHeadPositionY_ += minHeadMoveDistance;
		}
	}

	//how much time before moving the platform again
	//given that 7 platforms has been reached
	//once satisfied, the platform will move kPlatDistanceChange units
	if ((allPlatforms_.size() > kNumPlatsBeforeMoving)
		&& ((clock() - platformMoveTime_) > kMillisPerMovingPlatform)) {
		allPlatforms_.back().moveXDirection(kPlatDistanceChange);
		platformMoveTime_ = clock();
	}

	//decrease platform width by the amount specified 
	if ((allPlatforms_.size() > kNumPlatsPerChanges)
		&& (allPlatforms_.size() % kNumPlatsPerChanges == 0)
		&& (platformWidth_ >= minPlatformWidth)
		&& (previousScoreWidth != allPlatforms_.size())) {
		platformWidth_ -= kPlatformWidthChange;
		previousScoreWidth = allPlatforms_.size();
	}

	//when to increase the min speed of the ball moving
	if ((allPlatforms_.size() > kNumPlatsSpeedIncreases)
		&& (allPlatforms_.size() % kNumPlatsSpeedIncreases == 0)
		&& (previousScoreDistance != allPlatforms_.size())) {
		minHeadMoveDistance += kHeadDistanceChange;
		previousScoreDistance = allPlatforms_.size();
	}
}


void ofApp::movePlayerToNextPlatform() {
	//player has reached so reset variables
	isChangingPlatforms_ = false;
	hasReachedBorder_ = true;
	platformsMovingDown_ = true;
	mouseX_ = SENTIEL_MOUSE_POSITION;
	mouseY_ = SENTIEL_MOUSE_POSITION;
	
	//create new platform at a height where the current platform disappears
	double currentPlatformHeight = (ofGetWindowHeight() - allPlatforms_.back().getYPos());
	allPlatforms_.push_back(Platform(
			platformWidth_
			, kPlatformHeight_
			, ofGetWindowHeight() * kMinNewPlatformHeightFactor * BORDER_WIDTH_FACTOR - currentPlatformHeight
			, ofGetWindowHeight() * BORDER_WIDTH_FACTOR * kMaxNewPlatformHeightFactor - currentPlatformHeight));
}

void ofApp::draw(){
	ofSetColor(ofColor(MAX_RGB, MAX_RGB, MAX_RGB));
	background_.draw(0, 0);
	if (currentState_ == MENU) {
		drawMenu();
	} else if (currentState_ == IN_PROGRESS) {
		levelsSound_.setPaused(false);
		drawGamePlay();
	} else if (currentState_ == PAUSED) {
		drawPaused();
	} else if (currentState_ == GAME_OVER) {
		drawGameOver();
	} else if (currentState_ == HIGH_SCORES) {
		drawHighScores();
	}
}

void ofApp::drawMenu() {
	ofSetColor(ofColor::black);
	std::ostringstream menuOptions;
	int numMenuOptions = 2;
	
	menuOptions << "Welcome to Jump!" << std::endl << std::endl;
	menuOptions << "Select One:" << std::endl;
	menuOptions << "1. Play Game" << std::endl;;
	menuOptions << "2. High Scores" << std::endl;
	regularFont_.drawString(
		menuOptions.str()
		, ofGetWindowWidth() * BORDER_WIDTH_FACTOR
		, ofGetWindowHeight() * BORDER_WIDTH_FACTOR);
	
	//draw the arrow, "f" represents the certain arrow that I want
	double arrowXChangeFactor = 1.5;
	double newLineYFudgeFactor = 4.2;
	arrowFont_.drawString(
		"f"																			
		, ofGetWindowWidth() * BORDER_WIDTH_FACTOR - arrowXChangeFactor * ARROW_FONT_SIZE
		, ofGetWindowHeight() * BORDER_WIDTH_FACTOR 
			+ REGULAR_FONT_SIZE * (newLineYFudgeFactor + menuOptionHighlighted_));

	//tell users how to quit
	std::string quitString = "Press q to exit game";	
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
	
	//mark where mouse is if the position is valid and it is not moving from border to border
	if ((!isChangingPlatforms_ || hasReachedBorder_) 
	&& (mouseX_ != SENTIEL_MOUSE_POSITION && mouseY_ != SENTIEL_MOUSE_POSITION)) {
		ofSetColor(MAX_RGB, 0, 0, MAX_RGB * kQuarterColorOpacity);
		ofDrawCircle(mouseX_, mouseY_, kMouseMarkerRadius);
	}

	drawHead();
}

void ofApp::drawPaused() {
	drawGamePlay();
	ofSetColor(0, 0, 0);
	levelsSound_.setPaused(true);
	std::string output = "Press P To Unpause";
	//center the string
	regularFont_.drawString(output
		, (ofGetWindowWidth() - REGULAR_FONT_SIZE * output.size()) / 2.0
		, (ofGetWindowHeight() - REGULAR_FONT_SIZE) / 2.0);
}

void ofApp::drawHighScores() {
	//return to main menu
	regularFont_.drawString("PRESS M TO RETURN TO MAIN MENU\nOR C TO CLEAR HIGH SCORES"
							, 0
							, ofGetWindowHeight() * BORDER_WIDTH_FACTOR);
	
	//draw the high scores
	double fontFudgeFactor = 4.5;
	regularFont_.drawString(getHighScoresString()
		, 0
		, ofGetWindowHeight() * BORDER_WIDTH_FACTOR + REGULAR_FONT_SIZE * fontFudgeFactor);
}

void ofApp::drawGameOver() {
	ofSetColor(ofColor::black);
	
	//set what needs to be drawn
	std::string outputContents = "PRESS R TO RESTART\n";
	outputContents += "PRESS M TO VIEW MENU\n\nYou scored: " + to_string(currentScore_) + "\n";
	outputContents += getHighScoresString();

	//actually draw it starting at borders
	regularFont_.drawString(outputContents,  
							ofGetWindowWidth() * BORDER_WIDTH_FACTOR
							, ofGetWindowHeight() * BORDER_WIDTH_FACTOR);
}

std::string ofApp::getHighScoresString() {
	if (highScores_.empty()) {
		return "There are no high scores yet. :(";
	}

	//concatenate all the high scores into one string 
	//(works with 0 high scores as well)
	std::ostringstream highScoresOutput;
	highScoresOutput << "High Scores:" << std::endl;
	for (int rank = 1; rank <= highScores_.size(); ++rank) {
		highScoresOutput << rank << ". " << highScores_[rank - 1] << "\n";
	}
	return highScoresOutput.str();
}

void ofApp::loadHighScores() {
	ifstream highScoresReader(HIGH_SCORES_FILE);

	//go to the end of the file and push high scores to highScores_
	while (!highScoresReader.fail() && !highScoresReader.eof()) {
		int score;
		highScoresReader >> score;
		//verify score is right
		if (score >= 1) {
			highScores_.push_back(score);
		}
	}
	addScoreUpdateHS();
}

void ofApp::drawBody() {
	int lastX, lastY;
	ofSetColor(ofColor::black);
	
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
		}  else {
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
	//torso length is specified at playerPartsParameters_[Person::kTorsoIndex][0]
	ofDrawLine(lastX, lastY, lastX, lastY - playerPartsParameters_[Person::kTorsoIndex][0]);
}

void ofApp::drawRightArm(int lastX, int lastY) {
	double middleTorsoY = lastY - (playerPartsParameters_[Person::kTorsoIndex][0] / 2);
	ofDrawLine(lastX, middleTorsoY, lastX + playerPartsParameters_[Person::kArmIndex][0], middleTorsoY);
}

void ofApp::drawLeftArm(int lastX, int lastY) {
	double middleTorsoY = lastY - (playerPartsParameters_[Person::kTorsoIndex][0] / 2);
	ofDrawLine(lastX, middleTorsoY, lastX - playerPartsParameters_[Person::kArmIndex][0], middleTorsoY);
}

void ofApp::drawHead() {
	ofSetColor(MAX_RGB - rValue_, MAX_RGB - gValue_, MAX_RGB - bValue_);
	
	//check if the head should be a picture or a ball, depending on if the person is moving
	if (isChangingPlatforms_) {
		//hasReached the border, check by making sure the locations are <1 px apart
		if (abs(lastClickedLocation_[0] - lastHeadPositionX_) < 1 
			&& abs(lastClickedLocation_[1] - lastHeadPositionY_) < 1) {
			hasReachedBorder_ = true;
		}

		//draw head as ball
		double centerY = lastHeadPositionY_ - playerPartsParameters_[Person::kCircleRadiusIndex][0];
		double radius = playerPartsParameters_[Person::kCircleRadiusIndex][0];
		ofDrawCircle(lastHeadPositionX_, centerY, radius);
		ofSetColor(ofColor::blueSteel);
		ofDrawCircle(lastHeadPositionX_, centerY, radius * kInnerToOuterCircleFactor);
	} else {		
		//draw head as picture
		ofSetColor(ofColor(MAX_RGB, MAX_RGB, MAX_RGB));
		double imageSideLength = 3 * playerPartsParameters_[Person::kCircleRadiusIndex][0];
		evansPicture_.draw(lastHeadPositionX_ - imageSideLength / 2
							, lastHeadPositionY_ - imageSideLength
							, imageSideLength
							, imageSideLength);
	}
}

void ofApp::drawDottedLine(int startX, int startY, int endX, int endY) {
	double currentX = startX;
	double currentY = startY;
	double differenceX = (endX - startX) / NUM_DOTS * kDistanceToMouseFactor;
	double differenceY = (endY - startY) / NUM_DOTS * kDistanceToMouseFactor;

	//know which direction we're goin in 
	double angle = getAngle(differenceX, differenceY);
	
	//draw all the dots with spaces
	for (int dashNumber = 0; dashNumber < NUM_DOTS; ++dashNumber) {
		ofDrawCircle(currentX, currentY, BODY_WIDTH);
		currentX += differenceX + kDistanceBetweenDots * cos(angle);
		currentY += differenceY + kDistanceBetweenDots * sin(angle);
	}
}

bool ofApp::headAndPlatformIntersects() {
	Platform currentPlatform = allPlatforms_.back();
	double radius = playerPartsParameters_[Person::kCircleRadiusIndex][0];

	//defining x and y values of the current platform
	double topBorderY = currentPlatform.getYPos();
	double bottomBorderY = topBorderY + currentPlatform.getHeight();
	double leftBorderX = currentPlatform.getXPos();
	double rightBorderX = leftBorderX + currentPlatform.getLength();
	
	//check if at least one X and one Y match criteria
	bool centerYToTop = abs(topBorderY - lastHeadPositionY_) <= radius;
	bool centerYToBottom = abs(bottomBorderY - lastHeadPositionY_) <= radius;
	bool centerXToRight = abs(rightBorderX - lastHeadPositionX_) <= radius;
	bool centerXToLeft = abs(leftBorderX - lastHeadPositionX_) <= radius;
	if ((centerYToTop || centerYToBottom) && (centerXToLeft || centerXToRight)) { 
		//has intersected
		currentScore_ += 1;
		return true;
	}
	return false;	
}

void ofApp::setColor() {
	if (clock() - colorResetTime_ > kMillisPerColorReset) {
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
			allPlatforms_[index].movePlatformDown(kPlatformMoveDownRate);
		}
		ofDrawRectangle(allPlatforms_[index].getPlatform());
	}
}

void ofApp::drawScore() {
	ofSetColor(ofColor::black);
	double topRightX = ofGetWindowWidth() * (1 - BORDER_WIDTH_FACTOR);
	
	//get score and draw it
	std::string scoreText = "Score: " + std::to_string(currentScore_);
	regularFontSmall_.drawString(scoreText
					, topRightX - scoreText.size() * REGULAR_FONT_SMALL_SIZE
					, TOP_RIGHT_Y);
}

double ofApp::getAngle(double xChange, double yChange) {
	double angle = atan(yChange / xChange);
	if (xChange < 0) {
		angle += PI;
	}
	return angle;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	int upperKey = toupper(key);
	
	if (upperKey == 'Q') {
		clearAndExit();
	}
	
	//check which key is pressed during which state and switch states accordingly
	if (currentState_ == MENU) {
		//switch menu Option
		if (upperKey == KEY_UP) {
			menuOptionHighlighted_ = menuOptionHighlighted_ == 1 ? 2 : 1;
		} else if (upperKey == KEY_DOWN) {
			menuOptionHighlighted_ = menuOptionHighlighted_ == 1 ? 2 : 1;
		} else if (upperKey == ENTER_KEY) {
			//if currentState is play game, play, otherwise show high scores
			if (menuOptionHighlighted_ == 1) {
				currentState_ = IN_PROGRESS;
				resetGamePlay();
				resetGamePlay();
			} else {
				currentState_ = HIGH_SCORES;
			}
		}
	} else if (currentState_ == GAME_OVER) {
		if (upperKey == 'R') {
			resetGamePlay();
		} else if (upperKey == 'M') {
			currentState_ = MENU;
		}
	} else if (currentState_ == IN_PROGRESS || currentState_ == PAUSED) {
		if (upperKey == 'P') {
			currentState_ = currentState_ == PAUSED ? IN_PROGRESS : PAUSED;
		} else if (upperKey == 'R') {
			addScoreUpdateHS(currentScore_);
			resetGamePlay();
		}
	} else if (currentState_ == HIGH_SCORES) {
		if (upperKey == 'M') {
			currentState_ = MENU;
		} else if (upperKey == 'C') {
			highScores_.clear();
			updateHighScoresFile();
		}
	}
}

void ofApp::resetGamePlay() {
	//reset all values, and put game in motion
	platformWidth_ = SENTIEL_PLATFORM_WIDTH;
	lastClickedLocation_[0] = SENTIEL_MOUSE_POSITION;
	lastClickedLocation_[1] = SENTIEL_MOUSE_POSITION;
	lastHeadPositionX_ = ofGetWindowWidth();
	lastHeadPositionY_ = ofGetWindowHeight();
	isChangingPlatforms_ = false;
	hasReachedBorder_ = false;
	platformsMovingDown_ = false;
	platformWidth_ = SENTIEL_PLATFORM_WIDTH;
	minHeadMoveDistance = 0.15;
	allPlatforms_.clear();
	allPlatforms_.push_back(Platform(platformWidth_, kPlatformHeight_));
	allPlatforms_.push_back(Platform(platformWidth_
		, kPlatformHeight_,
		ofGetWindowHeight() * BORDER_WIDTH_FACTOR * kMinNewPlatformHeightFactor
		, ofGetWindowHeight() * BORDER_WIDTH_FACTOR * kMaxNewPlatformHeightFactor));
	drawBody();
	currentScore_ = 1;
	currentState_ = IN_PROGRESS;
}

void ofApp::addScoreUpdateHS(int score) {
	if (score != 0) {
		highScores_.push_back(score);
	}
	//sort the high scores
	std::sort(highScores_.begin(), highScores_.end());
	std::reverse(highScores_.begin(), highScores_.end());
	if (highScores_.size() > 10) {
		highScores_.pop_back();
	}
	updateHighScoresFile();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	if (allPlatforms_.empty()) {
		return;
	}
	
	//checking the mouse is in a correct position
	bool withinBorders = ((x < ofGetWindowWidth() * BORDER_WIDTH_FACTOR)
		|| (x > ofGetWindowWidth() * (1 - BORDER_WIDTH_FACTOR)));
	bool notMovingAlready = (!isChangingPlatforms_ || hasReachedBorder_);
	bool minYChangeMet = abs(y - allPlatforms_.back().getYPos()) > 5;
	
	//if conditions are met, set it to current position, or revert to sentienel
	if (withinBorders && notMovingAlready && !platformsMovingDown_ && minYChangeMet) {
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
	//conditions for registering click
	bool withinBorders = ((x < ofGetWindowWidth() * BORDER_WIDTH_FACTOR) 
						|| (x > ofGetWindowWidth() * (1 - BORDER_WIDTH_FACTOR)));
	bool notMovingAlready = (!isChangingPlatforms_ || hasReachedBorder_);
	if (allPlatforms_.empty()) {
		return;
	}
	bool minYChangeMet = abs(y - allPlatforms_.back().getYPos()) > 5;

	if (withinBorders 
		&& notMovingAlready 
		&& !platformsMovingDown_ 
		&& currentState_ == IN_PROGRESS
		&& minYChangeMet) {
		
		//I want to set the position closest to the border so I'm using using similar triangles
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
	//delete memory
	delete player;
	std::exit(0);
}

void ofApp::updateHighScoresFile() {
	ofstream updateHighScores(HIGH_SCORES_FILE);
	updateHighScores.clear();
	//go through the list and put in file
	for (auto orderedHighScore = highScores_.begin();
		orderedHighScore != highScores_.end(); 
		++orderedHighScore) {
		updateHighScores << *orderedHighScore << std::endl;
	}
}