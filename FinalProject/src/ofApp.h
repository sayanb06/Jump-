#pragma once
#define SOUND_NAME "Levels.mp3"
#define MAX_RGB 256

#include "ofMain.h"
#include "../../../../../../../source/repos/FinalProject/FinalProject/Platform.h"
#include "../../../../../../../source/repos/FinalProject/FinalProject/Person.h"

class ofApp : public ofBaseApp{
private:
	//platforms that have been initialized
	std::vector<Platform> allPlatforms;
	
	//sound I'm going to be playing
	ofSoundPlayer mySound_;

	Person currentPlayer_;

	int** playerPartsParameters_;

	int mouseX_ = -100;
	int mouseY_ = -100;

	int kDottedNumSeparators = 6;

public:
	void setup();
	void update();
	void draw();
	void drawDottedLine(int startX, int startY, int endX, int endY);

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
