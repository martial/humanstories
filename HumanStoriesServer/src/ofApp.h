#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "CameraManager.hpp"
#include "Analyzer.hpp"
#include "ofxMQTT.h"

#define RANDOM_SPLIT_MODE 0
#define ALL_SPLIT_MODE 1
#define ALL_CROPPED_MODE 2

#define ALL_LAYOUT 0
#define CENTER_LAYOUT 1
#define TOP_LAYOUT 2
#define BOTTOM_LAYOUT 3


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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
    
        void sendImgToOsc(string id);

        ofxMQTT client;
        void onOnline();
        void onOffline();
        void onMessage(ofxMQTTMessage &msg);

        ofxOscReceiver oscDarknet;
        ofxOscSender senderDarknet;
        CameraManager cameraManager;
    
        Analyzer analyzer;
        bool showAnalysis;
        string sentId;
    
        vector<string> macAdresses;
        int numOfClients;
        int currentAnalysed;
    
        bool hadPeople;
    
        bool bAllBlack;
    
        int mode;
        int layoutMode;
    
        void setRandomCameras();
        void setRandomCameras(int id);

        void setAllSameCamera();
        void setLayout();
        int currentCategory;
    
        int timeElapsed;

};
