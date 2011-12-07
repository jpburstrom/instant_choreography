#include "testApp.h"


//--------------------------------------------------------------
void instantChoreography::setup(){
	ofBackground(0,0,0);
	
	loopMoviesAt = 3513;
	fadeInTime = 5;
	fadeOutTime = 30;
	idleTime = 10;
	
	bFullscreen = false;
	
	fadePos = 255;
	fadeMode = 1; // start with a fade in, yes?
	
	currentIdleMovie = 9;
	
	for (int i=0; i < 9; i++) {
		vPos[i][0] = ((i % 3) * 260) + 20;
		vPos[i][1] = ((i/3) * 195) + 15;
		vPos[i][2]= 240;
		vPos[i][3] = 180;
	}

	ofSetFrameRate(25);
	
	updateDirOnLoad();
	receiver.setup(PORT);
	
}

void instantChoreography::updateDirOnLoad()
{
	cout << "------updateDir()----\n";

	directory.allowExt("mov");
	numFiles = directory.listDir("");
	cout << numFiles << "<-Numfiles\n";
	directory.sort();
	//G
	for (int i = numFiles; (i > (numFiles - 9)) && i > 0; i--) {		
		movie[numFiles - i].loadMovie(directory.getName(i-1), OFXQTVIDEOPLAYER_MODE_TEXTURE_ONLY);
		//movie[numFiles - i].setLoopState(false);
	}
	
	for (int i = 0; i < currentIdleMovie; i++) {

		//movie[i].play();
	}
	
	//currentIdleMovie = min(9, numFiles);
	
	//dirUpdated = true;
}
	
	
//--------------------------------------------------------------
void instantChoreography::update(){

	loop = ofGetFrameNum() % (idleTime + loopMoviesAt);
	
	
	
	while (receiver.hasWaitingMessages()) 
	{
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		if (m.getAddress() == "/filename") {
			fileName = m.getArgAsString(0);
			cout << "Received: " << fileName << "\n";
			dirUpdated = true;
		}
	}
	
	//cout << "FRAME " << ofGetFrameNum();

	
	if (loop == 0) {
		fadeMode = 1;
		if (dirUpdated) {
			// cout << "updated: " << i << "\n";
			movie[currentIdleMovie].loadMovie(fileName, OFXQTVIDEOPLAYER_MODE_TEXTURE_ONLY);
			//movie[currentIdleMovie].setLoopState(false);
			//movie[currentIdleMovie].play();
		
			currentIdleMovie = (currentIdleMovie - 1) % 10;
			movie[currentIdleMovie].stop();
			movie[currentIdleMovie].closeMovie();
			
			numFiles++;
			dirUpdated = false;
		}
		
	} else if (loop == fadeInTime) {
		fadeMode = 0;
	} else if (loop == (loopMoviesAt - fadeOutTime)) {
		fadeMode = 2;
	}

	for (int i=0; i < 10; i++) {
		movie[i].setFrame(loop);
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
		if (movie[z].isLoaded()) {
			movie[z].draw(vPos[i][0],vPos[i][1],vPos[i][2],vPos[i][3]);	
		}
	}	
	//cout << "\n";

	fade();


	
	//debugDraw();

	
}

void instantChoreography::debugDraw()
{
	for (int i = 0; i < 9; i++) {
		ofDrawBitmapString(ofToString(i), vPos[i][0], vPos[i][1] + 10);
	}
	ofSetColor(255);
	ofDrawBitmapString(ofToString(loop),700, 10);
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
	

	if (268 == key) { //F12
		ofToggleFullscreen();
		bFullscreen = !bFullscreen;
		if (bFullscreen) {
			ofHideCursor ();
		} else {
			ofShowCursor ();
		}
	}
	
	//DEBUG::
	//cout << "Next slot: " << numFiles % (currentIdleMovie + 1);
	//dirUpdated = true;
	
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
