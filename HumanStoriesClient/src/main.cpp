#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
#ifdef __linux__

	ofSetupOpenGL(480, 320,OF_FULLSCREEN);			// <-------- setup the GL context
#else
    ofSetupOpenGL(480, 320,OF_WINDOW);            // <-------- setup the GL context

#endif
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
