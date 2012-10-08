/*
 *  Boid.h
 *  boid
 *
 *  Created by Jeffrey Crouse on 3/29/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"


class Boid {
public:
	Boid(ofVec3f forepoint);
    
    void update();
    void applyForce(ofVec3f force);
    void seek(ofVec3f target);
    void display();
    
    
    ofVec3f acc;
    ofVec3f vel;
    ofVec3f loc;
    
    float r;
    float maxspeed;
    float maxforce;
    
    vector<ofVec3f> history;

    ofColor c;

};

