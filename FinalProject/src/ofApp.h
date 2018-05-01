#pragma once
#define SOUND_NAME "Levels.mp3"
#define MAX_RGB 256
#define CARL_IMAGE "Carl.png"
#define SENTIEL_MOUSE_POSITION -100
#define REGULAR_FONT_SIZE 25
#define REGULAR_FONT_NAME "Gameplay.ttf"
#define ARROW_FONT_SIZE 25
#define ARROW_FONT_NAME "Arrows tfb.ttf"
#define KEY_UP 357
#define KEY_DOWN 359
#define ENTER_KEY 13
#define NUM_DOTS 6
#define TOP_RIGHT_Y 15

#include "ofMain.h"
#include "../../../../../../../source/repos/FinalProject/FinalProject/Platform.h"
#include "../../../../../../../source/repos/FinalProject/FinalProject/Person.h"
#include <iostream>
#include <algorithm>
#include <vector> 

class ofApp : public ofBaseApp{
	// Enum to represent the current state of the game
	enum GameState {
		IN_PROGRESS = 0,
		PAUSED,
		FINISHED,
		MENU,
		HIGH_SCORES
	};
private:

	//elements used in almost every function
	std::vector<Platform> allPlatforms_;							//all platforms that have been created
	int rValue_, gValue_, bValue_;									//current RGB values
	int** playerPartsParameters_;									//parts of the player
	double lastHeadPositionX_ = ofGetWindowWidth(), lastHeadPositionY_ = ofGetWindowHeight(); //senteniel values for where the head was last
	int mouseX_ = SENTIEL_MOUSE_POSITION;							//mouse position X
	int mouseY_ = SENTIEL_MOUSE_POSITION;							//mouse position Y
	GameState currentState_ = MENU;									
	int menuOptionHighlighted_ = 1;									//know which element to highlight in menu
	double lastClickedLocation_[2];									//gets information from mousePressed event
	bool isChangingPlatforms_ = false;								//is on the way to the next platfor
	bool hasReached_ = false;										//has reached that platform_, not necessarily opposites
	bool platformIsMovingDown_ = false;								//moving to a different platform
	const double kInnerToOuterCircleFactor = 1 / 3.0;				//ratio of the outer head to inner head when player moving
	bool isDead_ = false;											//if the user has died
	int platformWidth_ = 50;
	const int platformHeight_ = 10;
	const double minPlatformHeightFactor = 2, maxPlatformHeightFactor = 3.5;//how far the platform should be from the top border
	const double mouseMarkerRadius = 15;

	//setup utilities
	ofSoundPlayer mySound_;											//sound that I'm going to be playing
	ofImage carl_;													//image of prof. evans
	Person* player;													//current player
	clock_t startTime_ = clock();									//current time
	ofTrueTypeFont regularFont_;
	ofTrueTypeFont arrowFont_;

	void clearAndExit();
	bool headAndPlatformIntersects();								//returns whether the head and platform have intersected
	void movePlayerToNextPlatform();
	void setColor();
	void drawAllPlatforms();										//drawing all the platforms if I want to do something with falling down
	void drawBody();
	void drawScore();
public:
	
	void setup();
	void update();
	void draw();

	//different states of the game
	void drawGamePlay();
	void drawMenu();
	void drawPaused();
	void drawHighScores();
	void drawGameOver();
	
	void loadHighScores();
	std::string getHighScoresString();
	std::vector<int> highScores_;

	//draw each part of the person
	void drawLeg(int &lastX, int &lastY);							//dereferenced so that the the lastX, lastY updated
	void drawTorso(int lastX, int lastY);
	void drawLeftArm(int lastX, int lastY);
	void drawRightArm(int lastX, int lastY);
	void drawHead();
	void drawDottedLine(int startX, int startY, int endX, int endY);	//draw dotted line from torso to mouse position


	//Events
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
		
};
