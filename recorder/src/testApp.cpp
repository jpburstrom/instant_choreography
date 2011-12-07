#include "testApp.h"


//--------------------------------------------------------------
void instantChoreography::setup(){
	
	loopMoviesAt = 66;//3513;
	fadeInTime = 10;
	fadeOutTime = 30;
	idleTime = 10;
	
	countdown = false;
	recording = false;
	thankyou = false;
	idle = true;
	recordingCounter = 0;
	
	saver.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_HIGH);  
	
	fadePos = 255;
	fadeMode = 0; // start with a fade in, yes?
	
	fileNumber = 1;
	camWidth = 640;
	camHeight = 480;
	
	grabber.setDeviceID(0);
	grabber.initGrabber(camWidth,camHeight);
	


	sender.setup(HOST, PORT);
	ofxOscMessage m;
	m.setAddress( "/starting" );
	m.addStringArg( "hello");
	sender.sendMessage( m );
	
	
	ofSetFrameRate(25);
	
	vPos[0] = 20;
	vPos[1] = 15;
	vPos[2] = 760;
	vPos[3] = 570;
	updateDir();
	
	font.loadFont("fonts/GillSans-Bold.ttf", 360, true, false);
	font2.loadFont("fonts/GillSans-Bold.ttf", 72, true, false);
	
	ofBackground(0,0,0);
	
}

void instantChoreography::updateDir()
{
	//cout << "------updateDir()----\n";

	directory.allowExt("mov");
	int numFiles = directory.listDir("");
	//cout << numFiles;
	directory.sort();
	
	//cout << "path:" << directory.path();
	
	playFileName = directory.getName(numFiles - 1);
	//cout << "playfile:" << playFileName << "\n";
	vector<string> parts = ofSplitString(playFileName, ".");
	
	fileNumber = ofToInt(parts[0]) + 1;
	//int fileNumber = atoi(parts[0].c_str());
	sprintf(fileName, "%06d.mov", fileNumber);
	//cout << "New file name: " << fileName << "\n";
	
}
	
	
//--------------------------------------------------------------
void instantChoreography::update(){
	
	
	if (countdown && countdownCounter == 0 && recordingCounter != 0) {
		startRecording();
	}
	if (countdown) {
		idle = false;
		recordingCounter++;
		countdownCounter = 3 - (recordingCounter/25);
		//cout << countdownCounter;

	} 
	if (recording){
		idle = false;
		if (recordingCounter == 0) {
			movie.setPosition(0.0);
			fadeMode = 1;
		} else if (recordingCounter == fadeInTime) {
			fadeMode = 0;
		} else if (recordingCounter == (loopMoviesAt - fadeOutTime)) {
			//fadeMode = 2;
		}
		doRecording();
		movie.update();
	} 
	if (thankyou) {
		//cout << "-------------THANKYOU ----------------\n";
		idle = false;
		if (200 == recordingCounter) {
			thankyou = false;
			idle = true;
		}
		if (recordingCounter == 0) {
			//cout << "fade 1";
			fadeMode = 3;
		} else if (recordingCounter == 30) {
			fadeMode = 0;
			fadePos = 0;
		} else if (recordingCounter == (120 - fadeOutTime)) {
			fadeMode = 2;
		}
		recordingCounter++;

	}
		
}

//--------------------------------------------------------------
void instantChoreography::draw(){



	ofSetHexColor(0xFFFFFF);

	if (recording) {
		movie.draw(vPos[0],vPos[1],vPos[2],vPos[3]);
	} else if (thankyou) {
		//cout << countdownCounter;
		ofSetColor(255);
		drawCenteredString(font2, "Thank you");
	}	
	
	fade();

	if (countdown && countdownCounter != 0) {
		//cout << countdownCounter;
		ofSetColor(255);
		drawCenteredString(font, ofToString(countdownCounter));
	} 
	
	if (idle) {
		ofSetColor(255);
		drawCenteredString(font2, "Press any key");
	} 
	
		

	
}

void instantChoreography::drawCenteredString(ofTrueTypeFont f, string s)
{
	f.drawString(s, (800 - f.stringWidth(s))/2, 600 - ((600 - f.stringHeight(s)) / 2));
}

void instantChoreography::debugDraw()
{
	if (saver.bAmSetupForRecording()){  
		ofSetColor(255);  
		ofDrawBitmapString("setup for recording, (press 's' to save)",100,300);  
		
		if (recording){  
			ofSetHexColor(0x00ff00);  
			ofDrawBitmapString("recording, (press 'r' to toggle)",100,320);  
		} else {  
			ofSetHexColor(0xff0000);  
			ofDrawBitmapString("not recording, (press 'r' to toggle)",100,320);  
		}  
	} else {  
		ofSetColor(128);  
		ofDrawBitmapString("not setup to record",100,320);  
	}
	ofSetColor(255);
	ofDrawBitmapString(ofToString(recordingCounter),700, 10);
}

void instantChoreography::startCountdown()
{
	//cout << "-------COUNTDOWN--------\n";
	countdown = true;
	recordingCounter = 0;
}

void instantChoreography::startRecording()
{
	countdown = false;
	saver.setup(camWidth,camHeight, fileName);
	//cout << playFileName << "<< Playfilename\n";
	movie.loadMovie(playFileName, OFXQTVIDEOPLAYER_MODE_TEXTURE_ONLY);
	movie.play();
	
	recording = true;
	recordingCounter = 0;
}
/**
 * Called every frame if recording
 */
void instantChoreography::doRecording()
{
	//cout << "Do Recording. \n";
	
	grabber.grabFrame();
	
	recordingCounter++;
	//cout << recordingCounter;
	if (recordingCounter == loopMoviesAt) {
		stopRecording();
	} else {
		saver.addFrame(grabber.getPixels(), 1.0f / 25.0f);   
	}
	
}

void instantChoreography::stopRecording(bool del)
{	
	//cout << "Stop Recording. \n";
	saver.finishMovie();
	recording = false;
	if (del) {
		//cout << "DELETE\n";
		currentFile.open(fileName);
		currentFile.remove(false);
		return;
	}
	//FIXME: set this in startRecording, maybe?
	updateDir();
	
	ofxOscMessage m;
	m.setAddress( "/filename" );
	m.addStringArg( filePath + playFileName );
	sender.sendMessage( m );
	
	recordingCounter = 0;
	thankyou = true;
}

void instantChoreography::fade()
{
	//cout << "FadeMode: " << fadeMode << "\n";
	if (fadeMode == 0) {
		return;
	} else if (fadeMode == 1) {
		//fade In
		fadePos = max(0, fadePos - (255 / fadeInTime));
		if (fadePos == 0)
			fadeMode = 0;
			
	} else if (fadeMode == 2) {
		//fadeout
		fadePos = min(255, fadePos + (255 / fadeOutTime));
		if (fadePos == 255)
			fadeMode = 3;
	} else { 
		// black
		fadePos = 255;
	}


	ofEnableAlphaBlending();
	ofSetColor(0,0,0,fadePos);
	//ofSetColor(255,0,0,127);
	
	ofRect(0, 0, 800, 600);
	
}

//--------------------------------------------------------------
void instantChoreography::keyPressed  (int key){
	
	//TODO: Check for other keys
	//cout << "KEY" << key << "\n";
	
	if (idle) {
		thankyou = false;
		startCountdown();

//		startRecording();
//		printf("Now recording\n");
	} else {
		stopRecording(true);

//		printf("Stopped recording\n");
	}

}


//--------------------------------------------------------------
void instantChoreography::keyReleased(int key){

}

//--------------------------------------------------------------
void instantChoreography::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void instantChoreography::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void instantChoreography::mousePressed(int x, int y,	int button){

}


//--------------------------------------------------------------
void instantChoreography::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void instantChoreography::windowResized(int w, int h){

}

//--------------------------------------------------------------
void instantChoreography::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void instantChoreography::dragEvent(ofDragInfo dragInfo){ 

}

