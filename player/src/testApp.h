#pragma once

#include "ofMain.h"
#include "ofFileUtils.h"
#include "ofxOsc.h"

#include "ofxQTKitVideoPlayer.h"

#define PORT 5556

class instantChoreography: public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
	void debugDraw();
	
	void updateDirOnLoad();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void fade();
	
	bool bFullscreen;
	
	int fadeMode; //No fade, fade in, fade out, black
	int fadePos;
	int fadeInTime;
	int fadeOutTime;
	int idleTime; // Time to wait in black
	
	int numFiles;
	
	ofxQTKitVideoPlayer movie[10];
	
	ofxOscReceiver receiver;
	
	int vPos[10][4]; //[ID][x, y, w, h]
	
	int loopMoviesAt;
	int loop;

	int currentIdleMovie;
	bool dirUpdated;
	
	string fileName;
	
	ofDirectory directory;
	string fileNames[9];
	//bool dirUpdated;
	
	
};



