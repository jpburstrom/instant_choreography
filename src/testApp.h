#pragma once

#include "ofMain.h"
#include "ofFileUtils.h"

#include "ofxQTKitVideoPlayer.h"

#include "ofQtVideoSaver.h"


class instantChoreography: public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
	void debugDraw();
	
	void updateDir();
	void startRecording();
	void doRecording();
	void stopRecording();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	void fade(int screen);
	
	int fadeMode[2]; //No fade, fade in, fade out, black
	int fadePos[2];
	int fadeInTime;
	int fadeOutTime;
	int idleTime; // Time to wait in black
	
	ofxQTKitVideoPlayer movie[10];
	ofVideoGrabber		grabber;
	ofQtVideoSaver		saver;
		
	int camWidth;
	int camHeight;
	int fileNumber;
	string fileName;
	bool recording;
	int recordingCounter;
	
	int vPos[10][4]; //[ID][x, y, w, h]
	
	int loopMoviesAt;
	int loop;
	
	ofDirectory directory;
	string fileNames[9];
	bool dirUpdated;
	
	int currentIdleMovie;

};



