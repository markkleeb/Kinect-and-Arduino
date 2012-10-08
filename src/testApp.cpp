#include "testApp.h"

const float FovX = 1.0144686707507438;
const float FovY = 0.78980943449644714;
const float XtoZ = tanf(FovX / 2) * 2;
const float YtoZ = tanf(FovY / 2) * 2;
const unsigned int Xres = 640;
const unsigned int Yres = 480;

ofVec3f ConvertProjectiveToRealWorld(float x, float y, float z) {
	return ofVec3f((x / Xres - .5f) * z * XtoZ,
                   (y / Yres - .5f) * z * YtoZ,
                   z);
}


void testApp::setup() {
    
    
   

    
	ofSetVerticalSync(true);
	kinect.init(false, false);  // disable infrared/rgb video iamge (faster fps)
	kinect.open();
	useProjective = false;
    kinect1 = false;
    cloud1 = false;
    
    light.setPosition(0,0, -200);
    light.setDiffuseColor(ofColor(255));
    light.setAmbientColor(ofColor(150));
    light.enable();
    
    for(int i = 0; i < 100; i++){
        boids.push_back(new Boid(ofVec3f(forepoint)));
        
    }
    
    ofEnableSmoothing();
    
}

void testApp::update() {
	kinect.update();
    
    for(int i = 0; i < boids.size(); i ++){
    
        boids[i]->seek(forepoint);
        boids[i]->update();
        
    }
}

void testApp::draw() {
	ofBackground(255);
    
    if(kinect1){
	kinect.drawDepth(0, 600, 400, 300);
	}
	easyCam.begin();
	int width = kinect.getWidth();
	int height = kinect.getHeight();
	ofScale(-1, -1, -1); // orient the point cloud properly
	if(useProjective) {
       
		ofTranslate(-width / 2, -height / 2);   // center the projective point cloud
	} else {
    
		ofTranslate(0, 0, -1000); // rotate about z = 1500 mm
	}
	float* distancePixels = kinect.getDistancePixels(); // distance in millimeters
	ofMesh cloud;
    cloud.setMode(OF_PRIMITIVE_POINTS);
    
    forepoint.z = numeric_limits<float>::max();
	int forex, forey;        
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
          
			int i = y * width + x;
			float z = distancePixels[i];
            
            if(z!=0){
                
                
                if(useProjective) {
					cloud.addVertex(ofVec3f(x, y, z));
                    if(z < forepoint.z){
                        forepoint = ofVec3f(x, y, z);
                    }
				} else {
					cloud.addVertex(ConvertProjectiveToRealWorld(x, y, z));
                    if(z < forepoint.z){
                        forex = x, forey = y;
                        forepoint = ConvertProjectiveToRealWorld(x,y,z);
                    }
				}
                
            
		}
        }
    }
    
    
        
    int searchRadius = 128;
    ofVec3f sum;
    int count = 0;
    
  
    int maxSearchDistance = 100;

    for(int y = -searchRadius/2; y < +searchRadius/2; y++){
        for(int x = -searchRadius/2; x < +searchRadius/2;x++){
            int curx = x + forex;
            int cury = y + forey;
            
            
            if(curx > 0 && curx < width && cury > 0 && cury < height){
                int i = cury*width + curx;
                
                float curz = distancePixels[i];
                if(curz !=0 && abs(curz - forepoint.z) < maxSearchDistance){
                    
                    sum.z += curz;
                    sum.x += curx;
                    sum.y += cury;
                    count++;
                }
                
            }
        }
    }
    

    if(count > 0){
        
        ofVec3f avg = sum / count;
        
        
        
        avg = ConvertProjectiveToRealWorld(avg.x, avg.y, avg.z);
        
       /* 
        ofSetColor(0,0,255);
        ofFill();
        ofSphere(avg, 10);    
        */
        if(smoothedForepoint == ofVec3f()){
            smoothedForepoint = avg;
        } else{
            smoothedForepoint.interpolate(avg, 0.1);
        }
        
    }
    ofEnableAlphaBlending();	
    
    if(cloud1){
        ofSetColor(0);
        cloud.drawVertices();
    }
  
    
    for(int i= 0; i < boids.size(); i++){
        boids[i]->display();
	}
    
    
    
    
        
    ofSetColor(0, 255, 0);
    ofSphere(smoothedForepoint.x, smoothedForepoint.y, smoothedForepoint.z, 10);
	
    /*
    ofSetColor(255, 0, 0);
    ofSphere(forepoint.x, forepoint.y, forepoint.z, 10);
    */
    easyCam.end();
    
    
	
    ofDisableAlphaBlending();
	ofSetColor(255);
	ofDrawBitmapString(useProjective ? "projective point cloud" : "real world point cloud", 10, 20);
    
  
    
    
   
}

void testApp::exit() {
	kinect.close();
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		
        kinect1 = !kinect1;
	}
    if(key == 'p'){
     useProjective = !useProjective;    
    }
    
    if(key == 'c'){
        
        cloud1 = !cloud1;
    }
    
}