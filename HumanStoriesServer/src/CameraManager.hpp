//
//  CameraManager.hpp
//  example
//
//  Created by Martial Geoffre-Rouland on 14/05/2018.
//

#ifndef CameraManager_hpp
#define CameraManager_hpp

#include <stdio.h>
#include "ofMain.h"
#include "IPVideoGrabber.h"
using namespace ofx;

//#if defined(TARGET_OF_IPHONE) || defined(TARGET_ANDROID) || defined(TARGET_LINUX_ARM)

#define NUM_ROWS 4
#define NUM_COLS 3
//#else
//    #define NUM_CAMERAS 9
//    #define NUM_ROWS 3
//    #define NUM_COLS 3
//#endif


class IPCameraDef
{
public:
    enum AuthType {
        NONE,
        BASIC,
        COOKIE
    };
    
    IPCameraDef()
    {
    }
    
    IPCameraDef(const std::string& url): _url(url)
    {
    }
    
    IPCameraDef(const std::string& name,
                const std::string& url,
                const std::string& username,
                const std::string& password,
                const AuthType authType):
    _name(name),
    _url(url),
    _username(username),
    _password(password),
    _authType(authType)
    {
    }
    
    void setName(const std::string& name) { _name = name; }
    std::string getName() const { return _name; }
    
    void setURL(const std::string& url) { _url = url; }
    std::string getURL() const { return _url; }
    
    void setUsername(const std::string& username) { _username = username; }
    std::string getUsername() const { return _username; }
    
    void setPassword(const std::string& password) { _password = password; }
    std::string getPassword() const { return _password; }
    
    void setAuthType(AuthType authType) { _authType = authType; }
    AuthType getAuthType() const { return _authType; }
    
private:
    std::string _name;
    std::string _url;
    std::string _username;
    std::string _password;
    AuthType _authType = NONE;
};



class CameraManager {
  
public:
    
    void setup();
    void update();
    void draw();
    void drawAnalyzed();
    void drawResult();

    void loadCameras();
    
    void chooseRandomHumanCamera();
    
    std::shared_ptr<Video::IPVideoGrabber> analyzedGrabber, pastAnalyzedGrabber, currentDrawnCamera;
    void analyseNextCamera(bool bSkipOld = false);

    void updateCameras(bool bSkipOld = false);

    void loadCamerasById(vector<string> names);
    void printActual();

    std::vector<IPCameraDef> ipcams; // a list of IPCameras

    std::vector<std::shared_ptr<Video::IPVideoGrabber>> grabbers;

    ofJson analysisJson;
    vector<int> getIdFromCategoryId(int id);


private:
    
    int NUM_CAMERAS;
    int numOfAnalysedCameras;
    
    void nextVideo();
    
    int currentAnalyzedCamera;
    int analyzeTime, analyzeCicleTime;
    
    
    
    IPCameraDef& getNextCamera();
    std::size_t nextCamera;
    
    void videoResized(const void* sender, ofResizeEventArgs& arg);
    string currentPrintedID;
    
    vector<string> currentNames;
    
    int currentCategory;
    ofJson categoriesJson;

};

#endif /* CameraManager_hpp */
