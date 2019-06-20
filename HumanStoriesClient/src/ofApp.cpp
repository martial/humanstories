#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofLogNotice("setup") << "starting";

    configJson = ofLoadJson("config.json");
    
    ofSetFrameRate(15);
    ofSetVerticalSync(true);
    
    mainConfigUrl = configJson.value("config-url", "https://www.screen-club.com/humanstories/main-config.json");
    string serverIp = configJson.value("server-ip", "192.168.0.99");
    int mqttPort = configJson.value("mqtt-port", 1883);
    
    ofLogNotice("MQTT configuration") << serverIp << " " << mqttPort;
    raspiId = configJson.value("raspi-id", 0);
    
    ofLogNotice("setup") << "json parsed";
    
    
    ofLogNotice("setup") << "loading cameras";

    cameraManager.loadCameras();
    cameraManager.setup();

    ofAddListener(client.onOnline, this, &ofApp::onOnline);
    ofAddListener(client.onOffline, this, &ofApp::onOffline);
    ofAddListener(client.onMessage, this, &ofApp::onMessage);
    
    client.begin(serverIp, mqttPort);
    bool b = client.connect("humanstories-raspi-"+ofToString(raspiId));
    
    if(b) {
        ofLogNotice("MQTT") << "seems connected";
        client.subscribe("mode");

        client.subscribe("event");
        client.subscribe("event-processed");
    }
    
   

   
    
    showAnalysis = true;
    
    ofRegisterURLNotification(this);
    
    macAdress = "undefined";
    ofLogNotice("setup") << "getting mac address";

#ifdef __linux__

    char mac[32]={0};
    getMacAddress (mac);
    macAdress = ofToString(mac);
    

    
    
    
#endif
    
    ofLogNotice("Mac address") << macAdress;
    //client.publish("human-stories", macAdress);
    
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    try {
        client.update();

    } catch (...) {
        ofLogNotice("Error updating") << "";

    }
    cameraManager.update();
    
    
    
    
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
    
    if(currentMode == "macadress") {
        
        ofSetColor(255,0,0);
        ofDrawBitmapString(macAdress, 20, 40);
        
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
    cameraManager.analyseNextCamera(false);
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
    
    //client.subscribe("event");
    //client.subscribe("event-processed");

}
//--------------------------------------------------------------

void ofApp::onOffline(){
    ofLog() << "offline";
}
//--------------------------------------------------------------

void ofApp::onMessage(ofxMQTTMessage &msg){
    
    if(msg.topic == "event-processed") {
        
        cameraManager.analyseNextCamera(ofToInt(msg.payload), false);
    }
    
    if(msg.topic == "mode") {
        
        currentMode = msg.payload;
    }
    
    ofLog() << "message: " << msg.topic << " - " << msg.payload;
}
