//
//  Analyzer.hpp
//  example
//
//  Created by Martial Geoffre-Rouland on 04/05/2018.
//

#ifndef Analyzer_hpp
#define Analyzer_hpp

#include <stdio.h>
#include "ofMain.h"

#include "ofxOpenCv.h"

using namespace cv;


class Result {
    
public:
    
    string id;
    float brightness;
    
    bool bHasCheckPedestrians, bHasSavedImage, bHasCheckedObjects, bHasPrinted, bIsLandscape;
    int numPedestrians;
    int numFaces;
    string caption;
    vector<string> humanKeys;
    vector<string> objects;
    vector<ofRectangle> bounds;
    
    ofImage img;
    vector<ofImage> detectedImages;
    
    void resetForUpdate() {
        
        bHasSavedImage = false;
        objects.clear();
        bounds.clear();
        humanKeys.clear();
        
    }
    
    bool getHumans() {
        
        if (std::find(objects.begin(), objects.end(), "people") != objects.end()) {
            return true;
        }
        
        
        if (std::find(objects.begin(), objects.end(), "person") != objects.end()) {
            return true;
        }
        
        return false;
        
    }

    void analyzeCaption() {
        
        vector<string> words;
        words.push_back("people");
        words.push_back("man");
        words.push_back("woman");

        for(int i=0; i<words.size(); i++) {
            
            if (caption.find(words[i]) != string::npos) {
                humanKeys.push_back(words[i]);
            }
        }
        
        if(humanKeys.size() > 0) {
            ofLogNotice("found people here");
        }
        
    }
    
    void getDetectedAsImages () {
        
        /*
        for(int i=0; i<bounds.size(); i++) {
            
            if(objects[i] == "person") {
                
                ofImage copyImg = img;
                copyImg.crop(bounds[i].x,bounds[i].y,bounds[i].width,bounds[i].height );
                detectedImages.push_back(copyImg);
            
                copyImg.save("detected/" + ofToString(id)+"_"+ofToString(i)+".jpg");
                
            }
            
        }
         */
        
    }
    
    string getLocation() {
        
        string result;
        if(city == "false") {
            return country;
        } else {
            return country + ", " + city;
        }
        
        return "";
    }
    
    void loadJson() {
        
        /*
        ofxJSONElement result;
        result.openLocal("geoloc/"+ ofToString(id)+".json");
        city = result["city"].asString();
        
        
        ofxJSONElement countryJson;
        
        if (!result["country"].isNull()) {
            
        countryJson = result["country"];
        country = countryJson["name"].asString();
        ofLogNotice("check: ") << country;
        ofLogNotice("check: ") << city << " - " << country;
            
        }
         
         */
        
    }
    
    int analyzedCount;
    string city;
    string country;
    
};

class Analyzer {
  
public:
    
    void setup();
    void checkIfImage(ofPixels & pixels, string id);
    void checkPedestrians(ofPixels & pixels, string id);
    void checkFaces(ofPixels & pixels, string id);
    void drawCCV();
    void drawClassify();
    
    Result & getResult(string id);
    int getResultIndex(string id);
    
    void debugHumans();
    vector<string> getResultWithHumans();
    
    vector<ofImage> images;
    vector<vector<double> > imagePoints;
    vector<vector<float> > encodings;
    
    ofImage gridImage;
    
    vector<Result> results;
    
};

#endif /* Analyzer_hpp */
