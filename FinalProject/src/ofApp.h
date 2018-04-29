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

#include "ofMain.h"
#include "../../../../../../../source/repos/FinalProject/FinalProject/Platform.h"
#include "../../../../../../../source/repos/FinalProject/FinalProject/Person.h"

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
	GameState currentState_ = MENU;
	//platforms that have been initialized
	std::vector<Platform> allPlatforms_;
	
	//sound I'm going to be playing
	ofSoundPlayer mySound_;

	Person currentPlayer_;

	int** playerPartsParameters_;

	int mouseX_ = -100;
	int mouseY_ = -100;

	int kDottedNumSeparators = 6;

	ofImage carl_;

	clock_t startTime_ = clock();
	int rValue_, gValue_, bValue_;

	std::list<std::array<double, 2>> clickedLocations_;

	double lastHeadPositionX_ = ofGetWindowWidth(), lastHeadPositionY_ = ofGetWindowHeight();
	bool isChangingPlatforms_ = false;								//is on the way to the next platfor
	bool hasReached_ = false;										//has reached that platform_, not necessarily opposites
	bool platformIsMovingDown_ = false;
	const double kInnerToOuterCircleFactor = 1 / 3.0;
	bool isDead_ = false;
	int platformWidth_ = 50;
	const int platformHeight_ = 10;
	ofTrueTypeFont regularFont_;
	ofTrueTypeFont arrowFont_;
	int menuOptionHighlighted_ = 1;
public:
	
	void setup();
	void update();
	void draw();

	//different states of the game
	void drawGamePlay();
	void drawMenu();
	void drawPaused();
	void drawHighScores();

	//draw each part of the person
	void drawLeg(int &lastX, int &lastY);							//dereferenced so that the the lastX, lastY updated
	void drawTorso(int lastX, int lastY);
	void drawLeftArm(int lastX, int lastY);
	void drawRightArm(int lastX, int lastY);
	void drawHead();
	void drawDottedLine(int startX, int startY, int endX, int endY);//draw dotted line from person to where the user is pointing


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
