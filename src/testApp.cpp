#include "testApp.h"

void notice(string n)
{
	ofLog(OF_LOG_NOTICE, n);
}

//--------------------------------------------------------------
void instantChoreography::setup(){
	ofBackground(0,0,0);
	
	loopMoviesAt = 66;//3513;
	fadeInTime = 5;
	fadeOutTime = 30;
	idleTime = 10;
	
	fadePos[1] = 255;
	fadeMode[1] = 1; // start with a fade in, yes?
	
	fileNumber = 1;
	camWidth = 640;
	camHeight = 480;
	
	grabber.setDeviceID(0);
	grabber.initGrabber(camWidth,camHeight);
	
	string fileName = "test.mov";
	
	saver.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_NORMAL);  
    saver.setup(camWidth,camHeight,fileName);  

	for (int i=0; i < 10; i++) {
		movie[i].loadMovie("movies/fingers.mov");

	}
	
	for (int i=0; i < 10; i++) {
		movie[i].play();
	}
	
	currentIdleMovie = 9;
	
	for (int i=0; i < 9; i++) {
		vPos[i][0] = ((i % 3) * 200) + 800;
		vPos[i][1] = (i/3) * 150;
		vPos[i][2]= 160;
		vPos[i][3] = 120;
	}

	ofSetFrameRate(25);
	//updateDir();
}

void instantChoreography::updateDir()
{
	cout << "------updateDir()----\n";

	directory.allowExt("mov");
	int numFiles = directory.listDir("ic/");
	directory.sort();
	//G
	for (int i = numFiles; (i > (numFiles - 9)) && i >= 0; i--) {
		//cout << "\n LS: " << directory.getName(i - 1);
		fileNames[numFiles - i] = "ic/" + directory.getName(i - 1);
		//cout << numFiles - i << fileNames[numFiles - i] << "\n";
	}

	dirUpdated = true;
}
	
	
//--------------------------------------------------------------
void instantChoreography::update(){

	loop = ofGetFrameNum() % (loopMoviesAt + idleTime + fadeInTime + fadeOutTime);
	cout << loop;
	
	grabber.grabFrame();
	
	if (recording){
		doRecording();
	}				
	
	//cout << "FRAME " << ofGetFrameNum();
	
	//temp shit
	if (ofGetFrameNum() == 1) {
		updateDir();
	}
	
	int offset = ofGetFrameNum() % loopMoviesAt;
	
	if (loop == 0) {
		fadeMode[1] = 1;
		if (dirUpdated) {
				movie[currentIdleMovie].loadMovie(fileNames[0], OFXQTVIDEOPLAYER_MODE_TEXTURE_ONLY);
				movie[currentIdleMovie].play();
				currentIdleMovie = (currentIdleMovie - 1) % 10;
				//cout << currentIdleMovie;
				movie[currentIdleMovie].stop();
				movie[currentIdleMovie].closeMovie();
				dirUpdated = false;
		}
		for (int i=0; i < 9; i++) {
			movie[i].setPosition(0.0);
		}
		
	} else if (loop == fadeInTime) {
		fadeMode[1] = 0;
	} else if (loop == (loopMoviesAt + fadeInTime)) {
		fadeMode[1] = 2;
	}

	for (int i=0; i < 10; i++) {
		movie[i].update();
	}
}

//--------------------------------------------------------------
void instantChoreography::draw(){

	ofSetHexColor(0xFFFFFF);

	int x, y;
	for (int i=0; i < 9; i++) {
		int z = (i + currentIdleMovie + 1) % 10;
		//cout << z;//currentIdleMovie;		
		movie[z].draw(vPos[i][0],vPos[i][1],vPos[i][2],vPos[i][3]);	
	}	
	//cout << "\n";

	fade(1);


	ofSetColor(255);  
	
	debugDraw();

	
}

void instantChoreography::debugDraw()
{
	for (int i = 0; i < 9; i++) {
		ofDrawBitmapString(ofToString(i), vPos[i][0], vPos[i][1] + 10);
	}
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
	ofDrawBitmapString(ofToString(loop),700, 10);
}


void instantChoreography::startRecording()
{
	saver.setup(camWidth,camHeight, fileName);
	recording = true;
	recordingCounter = 0;
}
/**
 * Called every frame if recording
 */
void instantChoreography::doRecording()
{
	recordingCounter++;
	if (recordingCounter % loopMoviesAt == 0) {
		stopRecording();
	} else {
		saver.addFrame(grabber.getPixels(), 1.0f / 25.0f);   
	}
	
}

void instantChoreography::stopRecording()
{	
	saver.finishMovie();
	recording = false;
	//FIXME: set this in startRecording, maybe?
	fileNumber = ++fileNumber;
	fileName = "output_" + ofToString(fileNumber) + ".mov";
	updateDir();
}

void instantChoreography::fade(int screen)
{
	cout << "FadeMode: " << fadeMode[screen] << "\n";
	if (fadeMode[screen] == 0) {
		return;
	} else if (fadeMode[screen] == 1) {
		//fade In
		fadePos[screen] = max(0, fadePos[screen] - (255 / fadeInTime));
		if (fadePos[screen] == 0)
			fadeMode[screen] = 0;
			
	} else if (fadeMode[screen] == 2) {
		//fadeout
		fadePos[screen] = min(255, fadePos[screen] + (255 / fadeOutTime));
		if (fadePos[screen] == 255)
			fadeMode[screen] = 3;
	} else { 
		// black
		fadePos[screen] = 255;
	}


	ofEnableAlphaBlending();
	ofSetColor(0,0,0,fadePos[screen]);
	//ofSetColor(255,0,0,127);
	
	switch (screen) {
		case 0:
			ofRect(0, 0, 800, 600);
			break;
		case 1:
			ofRect(800,0,800, 600);
			break;
	}

}

//--------------------------------------------------------------
void instantChoreography::keyPressed  (int key){
	
	//TODO: Check for other keys
	
	if (!recording) {
		startRecording();
//		printf("Now recording\n");
	} else {
		stopRecording();

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

/*

void movieLoader::load(ofxQTKitVideoPlayer m, string f) {
	cout << "Loading new file " << f << "\n";
	movie.stop();
	movie.closeMovie();
	
	movie = m;
	filename = f;
	lock();  
	notice("Start thread");
	if(!isThreadRunning())  
		startThread(true, false);  
	unlock();
	
}

void movieLoader::threadedFunction() {
	movie.loadMovie(filename, OFXQTVIDEOPLAYER_MODE_TEXTURE_ONLY);
	stopThread(true);
}
*/