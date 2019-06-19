#pragma once

#include "ofMain.h"
#include "ofxMQTT.h"
#include "IPVideoGrabber.h"
#include "CameraManager.hpp"

#ifdef __linux__
#include "ofxPiTFT.h"
#endif

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofxMQTT client;
        void onOnline();
        void onOffline();
        void onMessage(ofxMQTTMessage &msg);
    
        ofJson configJson;
        int raspiId;
    
        string mainConfigUrl;
        ofJson mainConfigJson;

    
        CameraManager cameraManager;
        bool showAnalysis;
        string sentId;
        
        string label, labelMode, location, number;
    
        string macAdress;
    
        void urlResponse(ofHttpResponse & response);
    
        string currentMode;
    

#ifdef __linux__
    
    ofxPiTFT    piTft;

    void getMacAddress(char * uc_Mac)
    {
        int fd;
        
        struct ifreq ifr;
        char *iface = "eth0";
        char *mac;
        
        fd = socket(AF_INET, SOCK_DGRAM, 0);
        
        ifr.ifr_addr.sa_family = AF_INET;
        strncpy((char *)ifr.ifr_name , (const char *)iface , IFNAMSIZ-1);
        
        ioctl(fd, SIOCGIFHWADDR, &ifr);
        
        close(fd);
        
        mac = (char *)ifr.ifr_hwaddr.sa_data;
        
        //display mac address
        sprintf((char *)uc_Mac,(const char *)"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n" , mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        
    }

#endif

};
