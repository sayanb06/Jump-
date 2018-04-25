#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
}

//--------------------------------------------------------------
void ofApp::update(){
	Platform newPlat;
	allPlatforms.push_back(newPlat);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(ofColor(rand() % 256, rand() % 256, rand() % 256));
	for (int index = 0; index < allPlatforms.size(); ++index) {
		ofDrawRectangle(allPlatforms[index].getPlatform());
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
