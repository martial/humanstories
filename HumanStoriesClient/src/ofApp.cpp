#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    client.begin("127.0.0.1", 1883);
    client.connect("humanstories-server", "try", "try");
    
    ofAddListener(client.onOnline, this, &ofApp::onOnline);
    ofAddListener(client.onOffline, this, &ofApp::onOffline);
    ofAddListener(client.onMessage, this, &ofApp::onMessage);
    
    cameraManager.loadCameras();
    cameraManager.setup();
    
    showAnalysis = true;

}

//--------------------------------------------------------------
void ofApp::update(){
    
    cameraManager.update();
    
    if(cameraManager.analyzedGrabber->isConnected()) {
        
        ofPixels & pixels = cameraManager.analyzedGrabber->getPixels();
        
        if(pixels.isAllocated()) {
            
            string analyzedId = cameraManager.analyzedGrabber->getCameraName();
            
            if(!showAnalysis) {
                cameraManager.updateCameras();
                
            } else {
                ofLogNotice("change camera");
                //cameraManager.analyseNextCamera();
                
            }
            
        }
        
    } else {
        
        if(cameraManager.analyzedGrabber->getReconnectCount() > 1 ) {
            cameraManager.analyseNextCamera(true);
            ofLogNotice("jump skip old");
        }
        
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(!showAnalysis)
        cameraManager.drawResult();
    else
        cameraManager.drawAnalyzed();
    
    if(cameraManager.currentDrawnCamera) {
        
        string analyzedId = cameraManager.currentDrawnCamera->getCameraName();
        //Result & r = analyzer.getResult(analyzedId);
        //number = r.getLocation();
        
    } else {
        number ="";
        
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

//--------------------------------------------------------------

void ofApp::onOnline(){
    ofLog() << "online";
    
    client.subscribe("hello");
}
//--------------------------------------------------------------

void ofApp::onOffline(){
    ofLog() << "offline";
}
//--------------------------------------------------------------

void ofApp::onMessage(ofxMQTTMessage &msg){
    ofLog() << "message: " << msg.topic << " - " << msg.payload;
}
