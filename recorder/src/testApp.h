#pragma once

#include "ofMain.h"
#include "ofFileUtils.h"
#include "ofTrueTypeFont.h"

#include "ofxOsc.h"
#include "ofxQTKitVideoPlayer.h"

#include "ofQtVideoSaver.h"

#define HOST "localhost"
#define PORT 5556

class instantChoreography: public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	void drawCenteredString(ofTrueTypeFont f, string s);
	void debugDraw();
	
	
	
	void updateDir();
	void startCountdown();
	void startRecording();
	void doRecording();
	void stopRecording(bool del=false);
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void exit();
	
	void fade();
	
	bool bFullscreen;
	
	int fadeMode; //No fade, fade in, fade out, black
	int fadePos;
	int fadeInTime;
	int fadeOutTime;
	int idleTime; // Time to wait in black
	
	int vPos[4]; //[ID][x, y, w, h]
	
	int loopMoviesAt;


	//Recorder stuff
	
	ofxQTKitVideoPlayer movie;
	ofVideoGrabber		grabber;
	ofQtVideoSaver		saver;
	ofxOscSender		sender;
	ofTrueTypeFont		font;
	ofTrueTypeFont		font2;
	
	int camWidth;
	int camHeight;
	int fileNumber;
	string playFileName;
	char fileName[16];
	string filePath;
	bool idle;
	bool recording;
	bool countdown;
	bool thankyou;
	int recordingCounter;
	int countdownCounter;
	
	
	
	ofDirectory directory;
	ofFile		currentFile;
	string fileNames[9];
	//bool dirUpdated;
	
	
};



