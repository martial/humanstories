#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    bAllBlack = false;
    
    cameraManager.loadCameras();
    cameraManager.setup();
    
    analyzer.setup();

    oscDarknet.setup(3333);
    senderDarknet.setup("localhost", 3334);

    ofAddListener(client.onOnline, this, &ofApp::onOnline);
    ofAddListener(client.onOffline, this, &ofApp::onOffline);
    ofAddListener(client.onMessage, this, &ofApp::onMessage);
    client.begin("localhost", 1883);
    client.connect("humanstories-server", "try", "try");
    
    numOfClients = 9;
    currentAnalysed = 0;
    
    macAdresses.push_back("b8:27:eb:bd:89:38");
    macAdresses.push_back("b8:27:eb:4d:6d:cb");
    macAdresses.push_back("b8:27:eb:17:43:d3");
    macAdresses.push_back("b8:27:eb:8c:36:62");
    macAdresses.push_back("b8:27:eb:e8:be:22");
    macAdresses.push_back("b8:27:eb:52:eb:23");
    macAdresses.push_back("b8:27:eb:e5:74:74");
    macAdresses.push_back("b8:27:eb:dc:96:52");
    macAdresses.push_back("b8:27:eb:45:03:a7");
    
    hadPeople = false;
    
    mode = RANDOM_SPLIT_MODE;
    
    timeElapsed = ofGetElapsedTimeMillis();
    
    int rdm = floor(ofRandom(cameraManager.categoriesJson.size()));
    currentCategory = rdm;
    
    ofLogNotice("setup") << cameraManager.categoriesJson.size() << " - " << rdm;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    int diff = ofGetElapsedTimeMillis() - timeElapsed;
    if(diff > 30000) {
        
        int rdm = floor(ofRandom(2));
        
        setRandomCameras();
        if (rdm == 0) {
            layoutMode = CENTER_LAYOUT;
            setLayout();
            
            
        }
        
        if (rdm == 1) {
            layoutMode = ALL_LAYOUT;
            setLayout();
          
            
        }
        
        ofLogNotice("change layout") << rdm;
        timeElapsed = ofGetElapsedTimeMillis();

        
    }
    
    client.update();
    
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
            ofJson categoriesJson;
            for(int i=0; i<splittedWords.size(); i++) {
                
                vector<string> splittedCat = ofSplitString(splittedWords[i], "/");
                r.objects.push_back(splittedCat[0]);
                
                categoriesJson.push_back(splittedCat[0]);
                
                cameraManager.analysisJson["cameras"][ofToInt(id)]["categories"].push_back(categoriesJson);
                
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
            
            if(mode == RANDOM_SPLIT_MODE) {

                if(bHasPeople) {
                    
                    timeElapsed = ofGetElapsedTimeMillis();

                    
                    int rdm = floor(ofRandom(3));
                    
                    if (rdm == 0) {
                        layoutMode = CENTER_LAYOUT;
                        setLayout();
                        for(int i=0; i<macAdresses.size(); i++) {
                            string str = ofToString(i) + "/" + ofToString(id);
                            client.publish("event-processed-id", str);
                            
                        }
                        
                    }
                    
                    if (rdm == 1) {
                        layoutMode = ALL_LAYOUT;
                        setLayout();
                        for(int i=0; i<macAdresses.size(); i++) {
                            string str = ofToString(i) + "/" + ofToString(id);
                            client.publish("event-processed-id", str);
                            
                        }
                        
                    }
                    
                    if (rdm == 2) {
                        layoutMode = ALL_LAYOUT;
                        setLayout();
                        for(int i=0; i<macAdresses.size(); i++) {
                            string str = ofToString(i) + "/" + ofToString(id);
                            client.publish("event-processed-id", str);
                            
                        }
                        
                    }

                    
                    
                } else {
                    
                    if(hadPeople) {
                        
                        int rdm = floor(ofRandom(cameraManager.categoriesJson.size()));
                        currentCategory = rdm;
                        
                        layoutMode = ALL_LAYOUT;
                        setLayout();
                        setRandomCameras();
                       
                        
                    } else {
                    
                   
                        
                    //string str = ofToString(currentAnalysed) + "/" +  ofToString(id);
                    //client.publish("event-processed-id", str);
                    //layoutMode = ALL_LAYOUT;
                    //setLayout();
                    setRandomCameras(currentAnalysed);
                    
                    string str = ofToString(currentAnalysed) + "/normal";
                    client.publish("layout", str);
                        
                    }
                    
                }
                hadPeople = bHasPeople;
                
            }

            r.getDetectedAsImages();
            
            cameraManager.currentCategory = currentCategory;
            cameraManager.analyseNextCamera(true);
            
           // ofLogNotice("check next camera") << currentCategory;
            
            //client.publish("event-processed", ofToString(id));
            

            
            currentAnalysed++;
            if(currentAnalysed >= numOfClients) {
                currentAnalysed = 0;
                
                int rdm = floor(ofRandom(cameraManager.categoriesJson.size()));
                currentCategory = rdm;
                
                
            }
            
        }
        
        cameraManager.analysisJson["cameras"][ofToInt(id)]["status"] = "Processed";
        ofSaveJson("result.json", cameraManager.analysisJson);
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

void ofApp::setRandomCameras() {
    
    vector<int> ips = cameraManager.getIdFromCategoryId(currentCategory);
    std::random_shuffle(ips.begin(), ips.end());
    
    for(int i=0; i<macAdresses.size(); i++) {
        string str = ofToString(i) + "/" +  ofToString(ips[i]);
        client.publish("event-processed-id", str);
    }
    
}

void ofApp::setRandomCameras(int id) {
    
   
    vector<int> ips = cameraManager.getIdFromCategoryId(currentCategory);
    std::random_shuffle(ips.begin(), ips.end());
    
    string str = ofToString(id) + "/" +  ofToString(ips[0]);
    client.publish("event-processed-id", str);
    
    
    
}

void ofApp::setAllSameCamera() {
    
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
    
    
    if(key == 'c') {
        
        vector<int> ips = cameraManager.getIdFromCategoryId(1);
        std::random_shuffle(ips.begin(), ips.end());

        for(int i=0; i<macAdresses.size(); i++) {
            string str = ofToString(i) + "/" +  ofToString(ips[i]);
            client.publish("event-processed-id", str);
        }
        //client.publish("command", "sudo reboot");
        
    }
    
    if(key == 'r')
        client.publish("command", "sudo reboot");
    
    if(key == 't')
        client.publish("event-processed", "key");
    
    if(key == 'm')
        client.publish("mode", "macadress");
    
    if(key == 'l')
        client.publish("mode", "live");
    
    if(key == 'i')
        client.publish("mode", "id");
    
    if(key == 'o') {
        
        bAllBlack = !bAllBlack;
        for(int i=0; i<macAdresses.size(); i++) {
            
            float a = 0;
            if(bAllBlack) {
               
                string str = ofToString(i) + "/off";
                client.publish("layout", str);
                
            } else {
                string str = ofToString(i) + "/normal";
                client.publish("layout", str);
            }
            
            
           
                        
        }
        
    }
    
    if(key == 'c'){
        
        layoutMode = CENTER_LAYOUT;
        setLayout();
    }
    
    if(key == ' ') {
        
        for(int i=0; i<macAdresses.size(); i++) {
            
            string str = ofToString(i) + "=" + macAdresses[i];
            client.publish("id", str);
            ofLogNotice("send id") << str;
            
        }
        
        
    }
    
}

void ofApp::setLayout() {
    
    if(layoutMode == ALL_LAYOUT) {
        
        for(int i=0; i<macAdresses.size(); i++) {
            string str = ofToString(i) + "/normal";
            client.publish("mode", str);
        }
        
    }
    
    if(layoutMode == CENTER_LAYOUT) {
        
        for(int i=0; i<macAdresses.size(); i++) {
            
            if(i!=4) {
                string str = ofToString(i) + "/off";
                client.publish("mode", str);
            } else {
                string str = ofToString(i) + "/normal";
                client.publish("mode", str);
            }
        }
        
    }
    
    if(layoutMode == TOP_LAYOUT) {
        
        for(int i=0; i<macAdresses.size(); i++) {
            
            if(i<3) {
                string str = ofToString(i) + "/off";
                client.publish("mode", str);
            } else {
                string str = ofToString(i) + "/normal";
                client.publish("mode", str);
            }
        }
        
    }
    
    if(layoutMode == BOTTOM_LAYOUT) {
        
        for(int i=0; i<macAdresses.size(); i++) {
            
            if(i>5) {
                string str = ofToString(i) + "/off";
                client.publish("mode", str);
            } else {
                string str = ofToString(i) + "/normal";
                client.publish("mode", str);
            }
        }
        
    }
    
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
    ofLogNotice("MQTT") << "Online and sending ids";
    
    for(int i=0; i<macAdresses.size(); i++) {
        
        string str = ofToString(i) + "=" + macAdresses[i];
        client.publish("id", str);
        ofLogNotice("send id") << str;
        
    }
  
    
}
//--------------------------------------------------------------

void ofApp::onOffline(){
    ofLog() << "offline";
    
}
//--------------------------------------------------------------

void ofApp::onMessage(ofxMQTTMessage &msg){
    
   
    
}
