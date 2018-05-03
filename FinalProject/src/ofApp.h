#pragma once
#define SOUND_NAME "Levels.mp3" //Credit: Avicii
#define EVANS_IMAGE "Evans.png" //Credit: CS@Illinois
#define BACKGROUND_IMAGE_NAME "oceanBackground.jpg" //Credit: Pintrest
#define REGULAR_FONT_NAME "Gameplay.ttf" 
#define ARROW_FONT_NAME "Arrows tfb.ttf"
#define MAX_RGB 255
#define SENTIEL_MOUSE_POSITION -100
#define SENTIEL_PLATFORM_WIDTH 50
#define REGULAR_FONT_SMALL_SIZE 12
#define REGULAR_FONT_SIZE 22
#define ARROW_FONT_SIZE 25
#define KEY_UP 357
#define KEY_DOWN 359
#define ENTER_KEY 13
#define NUM_DOTS 6
#define TOP_RIGHT_Y 15
#define BODY_WIDTH 5
#define HIGH_SCORES_FILE "HighScores.txt"

#include "ofMain.h"
#include "../../../../../source/repos/FinalProject/FinalProject/Platform.h"
#include "../../../../../source/repos/FinalProject/FinalProject/Person.h"
#include <iostream>
#include <algorithm>
#include <vector> 

class ofApp : public ofBaseApp{
	// Enum to represent the current state of the game
	enum GameState {
		IN_PROGRESS = 0,
		PAUSED,
		GAME_OVER,
		MENU,
		HIGH_SCORES
	};

private:
	const int kNumMenuOptions = 2;							//current number of menu options we have
	const double kInnerToOuterCircleFactor = 1 / 3.0;		//ratio of the outer head to inner head when player moving
	const int kPlatformHeight_ = 10;						//height of each platform
	const double kMinNewPlatformHeightFactor = 2;			//how far the platform should be from the top border
	const double kMaxNewPlatformHeightFactor = 3.5;			//how far the platform should be from the top border
	const double kPlatformMoveDownRate = 0.1;				//how quickly the infinite scroll works	
	const double minPlatformWidth = 10;						//it would be insane for platform width to be less than this
	const double kDistanceToMouseFactor = 1 / 10.0;			// when drawing dotted line, don't want to draw whole line
	const double kDistanceBetweenDots = 5;					//distance between two adjacent dots when drawing dotted line
	const double factorHeadMoving = 1500.0;					//when the head needs to move to the mouse, we need to move it slowly so this will do that
	const int kMouseMarkerRadius = 15;						//radius of circle around mouse pointer when it is in the border zone
	const double kQuarterColorOpacity = 0.25;				//factor of opacity when displaying marker
	const double kHeadDistanceChange = 0.12;				//when the head is moving, how much the distance it covers in 1 frame will change
	const double kPlatformWidthChange = 2;					//how much the width of the platforms will change when (number of platforms) mod kNumPlatsWidthChanges = 0
	const double kPlatDistanceChange = 6;					//how much the platform will move once conditions are met
	const int kMillisPerMovingPlatform = 50;				//how much time before clock reset and platform moved
	const int kNumPlatsPerChanges = 4;					//how many platforms before the width of the following platforms will change
	const int kNumPlatsSpeedIncreases = 3;					//number of platforms before the speed of the head increases
	const int kNumPlatsBeforeMoving = 7;					//number of platforms before the consequent platforms start moving
	const int kMillisPerColorReset = 400;					//number of milliseconds before the color resets

	//elements used in almost every function
	GameState currentState_ = MENU;							//current state of the game
	int rValue_, gValue_, bValue_;							//current RGB values
	std::vector<Platform> allPlatforms_;					//all platforms that have been created
	bool isChangingPlatforms_;								//is on the way to the next platfor
	bool hasReachedBorder_;									//has reached that platform_, not necessarily opposites
	bool platformsMovingDown_;								//moving to a different platform
	int currentScore_ = 1;									//when game is in session, the current score
	int previousScoreWidth, previousScoreDistance;			//previous scores the distance and the width were changed at
	int platformWidth_;										//width of the new platforms
	int** playerPartsParameters_;							//parts of the player
	double lastHeadPositionX_, lastHeadPositionY_;			//X and Y coordinates of where the head was last
	double lastClickedLocation_[2];							//gets information from mousePressed event
	int mouseX_ = SENTIEL_MOUSE_POSITION;					//mouse position X
	int mouseY_ = SENTIEL_MOUSE_POSITION;					//mouse position Y
	int menuOptionHighlighted_ = 1;							//know which element to highlight in menu
	double minHeadMoveDistance;								//when the head is moving, how much is the minium the head will move?
	std::vector<int> highScores_;							//record of all high scores


	//setup utilities
	ofSoundPlayer levelsSound_;								//sound that I'm going to be playing
	ofImage evansPicture_;									//image of prof. evans
	ofImage background_;									//background image
	Person* player;											//current player
	clock_t colorResetTime_ = clock();						//current time
	clock_t platformMoveTime_ = clock();					//how much time it has been since platform moved last
	ofTrueTypeFont regularFont_;							//Font used for displaying game text								
	ofTrueTypeFont regularFontSmall_;						//Font used for displaying score
	ofTrueTypeFont arrowFont_;								//"Font" used for displaying a certain arrow by masking it as a letter

	void clearAndExit();									//clears memory on heap by deleting player parts pointer array
	bool headAndPlatformIntersects();						//returns whether the head and platform have intersected
	void movePlayerToNextPlatform();						//player and platform are close enough that the player can just climb on
	void setColor();										//if enough time has passed by, then reset color
	void drawAllPlatforms();								//drawing all the platforms if I want to do something with falling down
	void drawBody();										//draw Torso, Legs, and Arm(s)/DottedLines if necessary
	void drawScore();										//draw the score in the top right corner
	void updateGamePlay();									//when currentState_ == IN_PROGRESS, draw the game background, platforms, player, etc.
	void resetGamePlay();									//reset all game play variables to original and then play like new
	void addScoreUpdateHS(int score = 0);					//add the score the end of the list, sort (while keeping 10 elems) and output to file
	void loadHighScores();												//load high scores from file into highScores_
	std::string getHighScoresString();									//return string of high scores
	void updateHighScoresFile();										//update high scores with contents of highScores_

public:	
	void setup();
	void update();
	void draw();

	//different states of the game
	void drawGamePlay();												//currentState == IN_PROGRESS
	void drawMenu();
	void drawPaused();
	void drawHighScores();
	void drawGameOver();

	//draw each part of the person
	void drawLeg(int &lastX, int &lastY);								//dereferenced so that the the lastX, lastY updated
	void drawTorso(int lastX, int lastY);
	void drawLeftArm(int lastX, int lastY);
	void drawRightArm(int lastX, int lastY);
	void drawHead();
	void drawDottedLine(int startX, int startY, int endX, int endY);	//draw dotted line from torso to mouse position
	static double getAngle(double xChange, double yChange);				//angle on the unit circle for (xChange, yChange)

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
