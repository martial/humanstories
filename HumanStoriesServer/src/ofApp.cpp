#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    cameraManager.loadCameras();
    cameraManager.setup();
    
    analyzer.setup();

    oscDarknet.setup(3333);
    senderDarknet.setup("localhost", 3334);

    client.begin("localhost", 1883);
    client.connect("humanstories-server", "try", "try");
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //darknet
    while(oscDarknet.hasWaitingMessages()){
        ofxOscMessage m;
        oscDarknet.getNextMessage(m);
        
        ofLogNotice("darknet") << m.getArgAsString(0);
        
        string msg = m.getArgAsString(0);
        vector<string> splitedMsg = ofSplitString(msg, "_");
        string id = splitedMsg[0];
        
        Result & r = analyzer.getResult(id);
        
        if(splitedMsg.size() > 1 ) {
            vector<string> splittedWords = ofSplitString(splitedMsg[1], ",", true);
            
            bool bHasPeople = false;
            for(int i=0; i<splittedWords.size(); i++) {
                
                vector<string> splittedCat = ofSplitString(splittedWords[i], "/");
                r.objects.push_back(splittedCat[0]);
                ofLogNotice("category") << splittedCat[0];

                if(splittedCat[0] == "person")
                    bHasPeople = true;
                
                vector<string> splittedRect = ofSplitString(splittedCat[1], "-");
                ofRectangle rect;
                rect.x = ofToInt(splittedRect[0]);
                rect.y = ofToInt(splittedRect[1]);
                rect.width = ofToInt(splittedRect[2]);
                rect.height = ofToInt(splittedRect[3]);
                
                ofRectangle realRect;
                
                realRect.x = rect.x - rect.width / 2;
                realRect.y = rect.y - rect.height / 2;
                realRect.width = rect.width;
                realRect.height = rect.height,
                
                
                r.bounds.push_back(realRect);
                
               // ofLogNotice("word") << splittedWords[i];
               // ofLogNotice("rect") << rect;
                
             
                
                
            }
            
            if(bHasPeople) {
                ofLogNotice("PEOPLE") << id;
                client.publish("event", "people");
                
            }
            
            r.getDetectedAsImages();
            cameraManager.analyseNextCamera(true);
            client.publish("event-processed", ofToString(id));

            
        }
        
        r.bHasCheckedObjects = true;
        
        
        
        
    }
    
    cameraManager.update();
    
    if(cameraManager.analyzedGrabber->isConnected()) {
        
        ofPixels & pixels = cameraManager.analyzedGrabber->getPixels();
        
        if(pixels.isAllocated()) {
            
            string analyzedId = cameraManager.analyzedGrabber->getCameraName();
            Result & r = analyzer.getResult(analyzedId);
            analyzer.checkIfImage(pixels, analyzedId);
            
            if(r.brightness > 1.0) {
                
                if(r.bHasSavedImage) {
                    
                     sendImgToOsc(analyzedId);
                }
                
                
                if( (!r.caption.empty() && r.bHasCheckedObjects ) || r.analyzedCount > 2000 ) {
                    cameraManager.analyseNextCamera(true);

                    
                }
                
                
            } else {
                if( r.analyzedCount > 50) {
                    cameraManager.analyseNextCamera(true);
                    //ofLogNotice("change camera there");
                    
                }
            }
            
            
        }
        
    } else {
        
        if(cameraManager.analyzedGrabber->getReconnectCount() > 1 ) {
            cameraManager.analyseNextCamera(true);
            ofLogNotice("jump, reconected");
        }
        
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    ofBackground(0,0,0);
    cameraManager.drawAnalyzed();

}

void ofApp::sendImgToOsc(string id) {
    
    if(id != sentId ) {
        
        ofxOscMessage m;
        m.setAddress("/img");
        m.addStringArg(id);
        senderDarknet.sendMessage(m, false);
        
        ofLogNotice("send osc" );
        
    }
    
    sentId = id;
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 't')
        client.publish("event-processed", "key");
    
    if(key == 'm')
        client.publish("mode", "macadress");
    
    if(key == 'l')
        client.publish("mode", "live");

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
