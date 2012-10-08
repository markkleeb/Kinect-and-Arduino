/*
 *  Boid.cpp
 *  boid
 *
 *  Created by Jeffrey Crouse on 3/29/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#include "Boid.h"
#include <iostream>

Boid::Boid(ofVec3f forepoint) {
    
    ofSetFrameRate(30);
    loc = forepoint;
	maxspeed = ofRandom(1,10);
    maxforce = ofRandom(0.01, 0.4);
	vel = ofVec3f(3, -3, 3);
	acc = ofVec3f(0,0,0);
    r = 4.0;
    
    c = ofColor(0, 0, ofRandom(100,255));
    
    
    
}

void Boid::update(){

    vel += acc;
    vel.x = ofClamp(vel.x, -maxspeed, maxspeed);
    vel.y = ofClamp(vel.y, -maxspeed, maxspeed);
    vel.z = ofClamp(vel.z, -maxspeed, maxspeed);
    
    loc += vel;
    
  
    
    acc *= 0;
    
    history.push_back(loc);
    
    if(history.size() > 100){
        history.erase(history.begin() + 0);
        
    }
    
    
    
    }


void Boid::applyForce(ofVec3f force){
 
    acc += force;
    
}

void Boid::seek(ofVec3f target){
    
    ofVec3f desired = target - loc;
    float d = target.distance(loc);
    desired /= d;
    desired *= maxspeed;
    
    ofVec3f steer = desired - vel;
    steer.x = ofClamp(steer.x, -maxforce, maxforce);
    steer.y = ofClamp(steer.y, -maxforce, maxforce);
    steer.z = ofClamp(steer.z, -maxforce, maxforce);
    
    applyForce(steer);
    
}

void Boid::display(){
    
    //draw trails
    
    ofMesh l;
    l.setMode(OF_PRIMITIVE_LINE_STRIP);
    ofEnableAlphaBlending();
    
    for(int i = 0; i < history.size(); i++){
        
        int alpha = ofMap(i, 0, 100, 0, 255);
        
        l.addColor(ofColor(c, alpha));
        ofSetLineWidth(4);
        l.addVertex(history[i]);
        
    }
    
    l.drawVertices();
    ofDisableAlphaBlending();
    
    //Draw boids
    
    
    if(loc.x == 0 || loc.x == ofGetWindowWidth()) vel.x *= -1;
    if(loc.y == 0 || loc.y == ofGetWindowHeight()) vel.y *= -1;
   
    
    
    
	float anglexy = (float)atan2(-vel.y, vel.x);
    float thetaxy =  -1.0*anglexy;
	float heading2Dxy = ofRadToDeg(thetaxy)+90;
  
    
    float angleyz = (float)atan2(-vel.y, vel.z);
    float thetayz =  1.0*angleyz;
	float heading2Dyz = ofRadToDeg(thetayz)+90;
    
    float anglexz = (float)atan2(vel.x, vel.z);
    float thetaxz =  1.0*anglexz;
	float heading2Dxz = ofRadToDeg(thetaxz)+90;
    
    
    

    ofSetColor(c);
    ofFill();
    ofPushMatrix();
    ofTranslate(loc.x, loc.y, loc.z);
    ofRotateZ(heading2Dxy);
    ofRotateY(-heading2Dxz);
    ofRotateX(-heading2Dyz);
    ofSetColor(c);
    ofCone(0, 0, 0, r, 4*r);
    ofSphere(0, 0, 0, r);
    ofPopMatrix();
	
    
    
}
    
