//
//  CameraManager.cpp
//  example
//
//  Created by Martial Geoffre-Rouland on 14/05/2018.
//

#include "CameraManager.hpp"
#include "ofxImgSizeUtils.h"
#include "Poco/URI.h"

void CameraManager::loadCameras() {
  
    ofLog(OF_LOG_NOTICE, "---------------Loading Streams---------------");
    
    vector < string > linesOfTheFile;
    ofBuffer buffer = ofBufferFromFile("urls.txt");
    
    int i=0;
    for (auto line : buffer.getLines()){
        
        std::string tag = "stream";
        
        IPCameraDef::AuthType authType = IPCameraDef::AuthType::NONE;
        string auth = "NONE";
        
        if (auth.compare("NONE") == 0)
        {
            authType = IPCameraDef::AuthType::NONE;
        }
        else if (auth.compare("BASIC") == 0)
        {
            authType = IPCameraDef::AuthType::BASIC;
        }
        else if (auth.compare("COOKIE") == 0)
        {
            authType = IPCameraDef::AuthType::COOKIE;
        }
        
        IPCameraDef def(line,
                        line,
                        "",
                        "",
                        authType);
        
        
        

      
        

        /*
        Poco::URI uri1(line);
        string host = uri1.getHost();
        
        ofLogNotice("cehck ip: ") << host;

        ofHttpResponse resp = ofLoadURL("http://geoip.nekudo.com/api/"+host);
        ofLogNotice("data: ") << resp.data;

        ofFile myTextFile;
        myTextFile.open("geoloc/"+ ofToString(i)+".json",ofFile::WriteOnly);
        myTextFile << resp.data;
        myTextFile.close();
        
        //ofxXmlSettings xml;
        //xml.loadFromBuffer(resp.data);
        //xml.save("geoloc/"+ ofToString(i)+".xml");
        ofLogNotice("saved: ");;
        
        
        bool bAddCam = false;
        
        ofxJSONElement result;
        result.openLocal("geoloc/"+ ofToString(i)+".json");
        string city = result["city"].asString();
        
        
        ofxJSONElement countryJson;
        
        if (!result["country"].isNull()) {
            
            countryJson = result["country"];
            string country = countryJson["name"].asString();
            
            if(country == "France" )
                bAddCam = true;
        }

        i++;
            
        if(bAddCam)
          */
         ipcams.push_back(def);
    }
        ofLogNotice("ipcams size ") << ipcams.size();
    nextCamera = ipcams.size();
}


void CameraManager::setup() {
    
    analyzedGrabber = NULL;
    pastAnalyzedGrabber = NULL;
    
    numOfAnalysedCameras = 3;
    
    
    // initialize connection
    for (std::size_t i = 0; i < NUM_CAMERAS; i++)
    {
        IPCameraDef& cam = getNextCamera();
        
        auto grabber = std::make_shared<Video::IPVideoGrabber>();
        
        if (cam.getAuthType() == IPCameraDef::AuthType::COOKIE)
        {
            grabber->setCookie("user", cam.getUsername());
            grabber->setCookie("password", cam.getPassword());
        }
        else if (!cam.getUsername().empty() || !cam.getPassword().empty())
        {
            grabber->setUsername(cam.getUsername());
            grabber->setPassword(cam.getPassword());
        }
        
        //grabber->setCameraName(cam.getName());
        
        grabber->setURI(cam.getURL());
        grabber->connect(); // connect immediately
        
        ofAddListener(grabber->videoResized, this, &CameraManager::videoResized);
        
        grabbers.push_back(grabber);
        
    }
    
    currentAnalyzedCamera = 0;
    analyseNextCamera();
    
    currentPrintedID = "";
    
}

void CameraManager::update() {
    
    // update the cameras
    for (std::size_t i = 0; i < grabbers.size(); ++i)
    {
        grabbers[i]->update();
    }
    
  
    analyzedGrabber->update();
    
    if(pastAnalyzedGrabber)
        pastAnalyzedGrabber->update();


    
}

void CameraManager::draw() {

    ofSetHexColor(0xffffff);
    
    int row = 0;
    int col = 0;
    
    int x = 0;
    int y = 0;
    
    int w = ofGetWidth() / NUM_COLS;
    int h = ofGetHeight() / NUM_ROWS;
    
    float totalKbps = 0;
    float totalFPS = 0;
    
    for (std::size_t i = 0; i < grabbers.size(); ++i)
    {
        x = col * w;
        y = row * h;
        
        // draw in a grid
        row = (row + 1) % NUM_ROWS;
        
        if (row == 0)
        {
            col = (col + 1) % NUM_COLS;
        }
        
        ofPushMatrix();
        ofTranslate(x,y);
        ofSetColor(255,255,255,255);
        grabbers[i]->draw(0,0,w,h); // draw the camera
  
        ofPopMatrix();
    }
    
    
}

void CameraManager::drawAnalyzed() {
    ofSetColor(255,255,255,255);
    if(pastAnalyzedGrabber) {
        ofRectangle rect = ofxImgSizeUtils::getCenteredRect(ofGetWidth(), ofGetHeight(), pastAnalyzedGrabber->getWidth(), pastAnalyzedGrabber->getHeight(), false);
        pastAnalyzedGrabber->draw(rect);
        currentDrawnCamera = pastAnalyzedGrabber;

    }
    
    unsigned char * pix = analyzedGrabber->getPixels().getData();
    
    float sumBrightness = 0.0;
    for(int i=0; i<analyzedGrabber->getPixels().size(); i++) {
        
        float c = pix[i];
        sumBrightness += c;
    }
    
    float medBrigthness = sumBrightness / (float)analyzedGrabber->getPixels().size();
    
    if(medBrigthness > 10) {
        
        currentDrawnCamera = analyzedGrabber;
        ofRectangle rect = ofxImgSizeUtils::getCenteredRect(ofGetWidth(), ofGetHeight(), analyzedGrabber->getWidth(), analyzedGrabber->getHeight(),false);
        analyzedGrabber->draw(rect); // draw the camera
        
        if(analyzedGrabber->getCameraName() != currentPrintedID) {
        //printer.print(analyzedGrabber->getPixels());
         
            currentPrintedID = analyzedGrabber->getCameraName();
        }
        
    }
}

void CameraManager::drawResult() {
    
    ofSetColor(255,255,255,255);
    ofRectangle rect = ofxImgSizeUtils::getCenteredRect(ofGetWidth(), ofGetHeight(), currentDrawnCamera->getWidth(), currentDrawnCamera->getHeight(), false);
    currentDrawnCamera->draw(rect);
    
}




void CameraManager::printActual() {
    
}


IPCameraDef& CameraManager::getNextCamera() {
    
    //nextCamera = (nextCamera + 1) % ipcams.size();
    nextCamera = floor(ofRandom(ipcams.size()));
    return ipcams[nextCamera];
    
}

void CameraManager::loadCamerasById(vector<string> names) {
    
    
    currentNames = names;
    NUM_CAMERAS = names.size();
    grabbers.clear();
    for(int i=0; i<names.size(); i++ ) {
        
        ofLogNotice("Find human and load at index ") << ofToInt(names[i]);
        
        auto c = std::make_shared<Video::IPVideoGrabber>();
        IPCameraDef& cam = ipcams[ofToInt(names[i])];
        c->setUsername(cam.getUsername());
        c->setPassword(cam.getPassword());
        c->setCameraName(names[i]);
        Poco::URI uri(cam.getURL());
        c->setURI(uri);
        c->connect();
        grabbers.push_back(c);
        
    }
    
}



void CameraManager::nextVideo() {
    
    // initialize connection
    for (std::size_t i = 0; i < NUM_CAMERAS; ++i)
    {
        ofRemoveListener(grabbers[i]->videoResized, this, &CameraManager::videoResized);
        auto c = std::make_shared<Video::IPVideoGrabber>();
        IPCameraDef& cam = getNextCamera();
        c->setUsername(cam.getUsername());
        c->setPassword(cam.getPassword());
        c->setCameraName(ofToString(nextCamera));
        
        Poco::URI uri(cam.getURL());
        c->setURI(uri);
        c->connect();
        
        grabbers[i] = c;
    }
    
}

void CameraManager::chooseRandomHumanCamera() {
    
    int rdm = floor(ofRandom(grabbers.size()));
    currentDrawnCamera = grabbers[rdm];
    
}

void CameraManager::updateCameras(bool bSkipOld) {
    
    
    if(analyzedGrabber && !bSkipOld)
        pastAnalyzedGrabber = analyzedGrabber;
    
    analyzedGrabber = currentDrawnCamera;
    
    analyzeTime = ofGetElapsedTimeMillis();
    
    ofLogNotice("Jump camera");
    
}



void CameraManager::analyseNextCamera(bool bSkipOld) {
    
    //currentAnalyzedCamera++;
    
    if(analyzedGrabber && !bSkipOld)
        pastAnalyzedGrabber = analyzedGrabber;
    
    currentAnalyzedCamera = floor(ofRandom(ipcams.size()));

    
    auto c = std::make_shared<Video::IPVideoGrabber>();
    IPCameraDef& cam = ipcams[currentAnalyzedCamera];
    c->setUsername(cam.getUsername());
    c->setPassword(cam.getPassword());
    c->setCameraName(ofToString(currentAnalyzedCamera));
    
    Poco::URI uri(cam.getURL());
    c->setURI(uri);
    c->connect();
    
    analyzedGrabber = c;
    
    analyzeTime = ofGetElapsedTimeMillis();
    
    
}

void CameraManager::videoResized(const void* sender, ofResizeEventArgs& arg)
{
    // Find the camera that sent the resize event changed.
    for (std::size_t i = 0; i < NUM_CAMERAS; ++i) {
        if (sender == grabbers[i].get()) {
           //
        }
    }
}