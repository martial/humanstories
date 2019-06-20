#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofLogNotice("setup") << "starting";

    configJson = ofLoadJson("config.json");
    
    ofSetFrameRate(15);
    ofSetVerticalSync(true);
    ofHideCursor();
    mainConfigUrl = configJson.value("config-url", "https://www.screen-club.com/humanstories/main-config.json");
    string serverIp = configJson.value("server-ip", "192.168.0.99");
    int mqttPort = configJson.value("mqtt-port", 1883);
    
    ofLogNotice("MQTT configuration") << serverIp << " " << mqttPort;
    raspiId = configJson.value("raspi-id", 0);
    
    ofLogNotice("setup") << "json parsed";
    
    ofLogNotice("setup") << "loading cameras";

    cameraManager.loadCameras();
    cameraManager.setup();

    macAdress = "undefined";
    ofLogNotice("setup") << "getting mac address";

#ifdef __linux__
    
    char mac[32]={0};
    getMacAddress (mac);
    macAdress = ofToString(mac);
    
#endif
    
    ofLogNotice("Mac address") << macAdress;
    
    ofAddListener(client.onOnline, this, &ofApp::onOnline);
    ofAddListener(client.onOffline, this, &ofApp::onOffline);
    ofAddListener(client.onMessage, this, &ofApp::onMessage);
    
    
    client.begin(serverIp, mqttPort);
    bool b = client.connect("humanstories-raspi-"+macAdress);
    
    if(b) {
        ofLogNotice("MQTT") << "seems connected";
        client.subscribe("mode");
        
        client.subscribe("command");

        client.subscribe("event");
        client.subscribe("event-processed");
        client.subscribe("event-processed-id");
        client.subscribe("id");

    }
    
  

   
    
    showAnalysis = true;
    
    ofRegisterURLNotification(this);
    
   
    
    //client.publish("human-stories", macAdress);
    
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    client.update();

    
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
    
    if(currentMode == "id") {
        
        ofSetColor(255,0,0);
        ofDrawBitmapString(raspiId, 20, 40);
        
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
    client.subscribe("mode");
    
    client.subscribe("event");
    client.subscribe("event-processed");
    
    bConnected = true;
    //client.subscribe("event");
    //client.subscribe("event-processed");

}
//--------------------------------------------------------------

void ofApp::onOffline(){
    ofLog() << "offline";
    bConnected = false;

}
//--------------------------------------------------------------

void ofApp::onMessage(ofxMQTTMessage &msg){
    
    ofLogNotice("MQTT message" ) << msg.topic  << " " << msg.payload;

    //ofLog() << "message: " << msg.topic << " - " << msg.payload;

    if(msg.topic == "event-processed" && msg.payload != "") {
        
        cameraManager.analyseNextCamera(ofToInt(msg.payload), false);
    }
    
    if(msg.topic == "event-processed-id" && msg.payload != "") {
        
        vector<string> splitted = ofSplitString(msg.payload, "/");
        int rId     = ofToInt(splitted[0]);
        int camId   = ofToInt(splitted[1]);
        
        ofLogNotice("event-processed-id") << rId << " - " << camId;

        
        if(rId == raspiId)
            cameraManager.analyseNextCamera(camId, false);
    }
    
    if(msg.topic == "mode") {
        
        currentMode = msg.payload;
    }
    
    if(msg.topic == "command") {
        ofSystem(msg.payload);
    }
    
    if(msg.topic == "id") {
        
        ofLogNotice("get id") << msg.payload;

        vector<string> splitted = ofSplitString(msg.payload, "=");
        int id= ofToInt(splitted[0]);
        string address= splitted[1];
        
        if(address == macAdress) {
            raspiId = id;
            ofLogNotice("set raspi id as") << raspiId;

        }
    }
    
}
