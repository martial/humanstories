#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    configJson = ofLoadJson("config.json");
    
    ofSetFrameRate(15);
    ofSetVerticalSync(true);
    
    mainConfigUrl = configJson.value("config-url", "https://www.screen-club.com/humanstories/main-config.json");
    string serverIp = configJson.value("server-ip", "192.168.0.99");
    int mqttPort = configJson.value("mqtt-port", 1883);
    
    ofLogNotice("MQTT configuration") << serverIp << " " << serverIp;
    raspiId = configJson.value("raspi-id", 0);

    client.begin(serverIp, mqttPort);
    client.connect("humanstories-raspi-"+ofToString(raspiId), "try", "try");
    
    ofAddListener(client.onOnline, this, &ofApp::onOnline);
    ofAddListener(client.onOffline, this, &ofApp::onOffline);
    ofAddListener(client.onMessage, this, &ofApp::onMessage);
    
    cameraManager.loadCameras();
    cameraManager.setup();
    
    showAnalysis = true;
    
    ofRegisterURLNotification(this);

    
#ifdef __linux__

    char mac[32]={0};
    getMacAddress (mac);
    macAdress = ofToString(mac);
    
    
    
    
#endif
    
    ofLogNotice("Mac address") << macAdress;
    client.publish("human-stories", macAdress);
    
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
  
    
    if(ofGetFrameNum() % 500 == 0) {
       
        ofLoadURLAsync(mainConfigUrl, "config-req");

    }
    
    cameraManager.update();
    
    if(cameraManager.analyzedGrabber->isConnected()) {
        
        ofPixels & pixels = cameraManager.analyzedGrabber->getPixels();
        
        if(pixels.isAllocated()) {
            
            string analyzedId = cameraManager.analyzedGrabber->getCameraName();
            
            if(!showAnalysis) {
                cameraManager.updateCameras();
                
            } else {
                //ofLogNotice("change camera");
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
    
    ofBackground(0);
    
    
    ofSetColor(255);

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

void ofApp::urlResponse(ofHttpResponse & response) {
    if (response.status==200 && response.request.name == "config-req") {
        
        configJson = nlohmann::json::parse(response.data);
        currentMode = configJson.value("mode", "live");
        ofLogNotice("Mode is") << currentMode;

        
    } else {
        cout << response.status << " " << response.error << endl;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
cameraManager.analyseNextCamera();
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
    ofLogNotice("MQTT") << "Online and suscribing";
    
    client.subscribe("event");
    client.subscribe("event-processed");

}
//--------------------------------------------------------------

void ofApp::onOffline(){
    ofLog() << "offline";
}
//--------------------------------------------------------------

void ofApp::onMessage(ofxMQTTMessage &msg){
    
    if(msg.topic == "event-processed") {
        
        cameraManager.analyseNextCamera(ofToInt(msg.payload), true);
    }
    
    ofLog() << "message: " << msg.topic << " - " << msg.payload;
}
