#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "Boid.h"


class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
    
    
	ofxKinect kinect;
	ofEasyCam easyCam;
	bool useProjective;
    bool kinect1;
    bool cloud1;
    
    
    vector<Boid*> boids;
    
    ofVec3f forepoint;
    ofVec3f smoothedForepoint;
  
    
    ofLight light;
    
};
