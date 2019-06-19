//
//  Analyzer.cpp
//  example
//
//  Created by Martial Geoffre-Rouland on 04/05/2018.
//

#include "Analyzer.hpp"


void Analyzer::setup() {
    
    
    
    ofSetLogLevel(OF_LOG_NOTICE);
   

    
}

int Analyzer::getResultIndex(string id) {
    
    // check if exist
    
    int resultID = -1;
    for(int i=0; i<results.size(); i++) {
        if(results[i].id == id)
            resultID = i;
    }
    
    if( resultID < 0 ) {
        
        Result r;
        r.id = id;
        r.analyzedCount = 0;
        r.numPedestrians = 0;
        r.numFaces = 0;
        r.bHasCheckPedestrians = false;
        r.bHasSavedImage = false;
        r.bHasCheckedObjects = false;
        r.bIsLandscape = false;
        r.loadJson();
        results.push_back(r);
        return ( results.size() - 1);
    } else {
        return resultID;
    }
    
}

Result & Analyzer::getResult(string id) {
    
    return results[getResultIndex(id)];
    
}


vector<string> Analyzer::getResultWithHumans() {
    
    vector<string> indexes;
    for(int i=0; i<results.size(); i++) {
        
        if(results[i].getHumans())
        //if(results[i].bIsLandscape)
            indexes.push_back(results[i].id);
        
    }
    
    return indexes;
    
    
}




void Analyzer::checkIfImage(ofPixels &  pixels, string id) {
    
    // we need to check if there is actually not only back image here
    //img.get
    
    
    unsigned char * pix = pixels.getData();
    
    float sumBrightness = 0.0;
    for(int i=0; i<pixels.size(); i++) {
        
        float c = pix[i];
        sumBrightness += c;
    }
    
    float medBrigthness = sumBrightness / (float)pixels.size();
    
    int index = getResultIndex(id);
    results[index].brightness = medBrigthness;
    results[index].analyzedCount++;
    
    if( medBrigthness > 10.0 && ! results[index].bHasSavedImage) {
        
        ofxCvColorImage            colorImg;
        colorImg.setFromPixels(pixels);
        
        ofImage img;
        img.setImageType(OF_IMAGE_COLOR);
        img.setFromPixels(colorImg.getPixels());
        img.setImageType(OF_IMAGE_COLOR);
        img.save("temp/"+id+".jpg");
        img.save("temp.jpg");

        results[index].img = img;
        
        results[index].bHasSavedImage = true;
        img.clear();
    }
    
}



void Analyzer::checkFaces(ofPixels & pixels, string id) {
    
   

}


void Analyzer::checkPedestrians(ofPixels & pixels, string id) {
    
    
     
     
 
    

   
}

void Analyzer::debugHumans() {
    
    int nFaces = 0;
    int nPedestrians = 0;
    int nKeys = 0;
    int hHumans = 0;

    for(int i=0; i<results.size(); i++) {
        
        if(results[i].bIsLandscape)
            nFaces ++;
        
        nPedestrians += results[i].numPedestrians;
        
        for(int j=0; j<results[i].humanKeys.size(); j++) {
            nKeys++;
        }

        
    }
    
    ofDrawBitmapString("nLandscapes " + ofToString(nFaces) , 20, 40);
    ofDrawBitmapString("nPedestrians " + ofToString(nPedestrians) , 20, 60);
    ofDrawBitmapString("nKeys " + ofToString(nKeys) , 20, 80);

    
}


void Analyzer::drawCCV() {
   
}

void Analyzer::drawClassify() {
    
    
    
}


