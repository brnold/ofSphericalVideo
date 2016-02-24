#pragma once

#include "ofMain.h"
#include "ofxGstRTPClient.h"
#include "ofxGstRTPServer.h"

#define STATIC_IMAGE 0
#define VIDEO 1

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    void createSegmentedMesh(const ofVec3f& center,
                             double radius,
                             int precision,
                             int textWidth,
                             int textHeight,
                             double theta1, double theta2,
                             double phi1, double phi2);
    
    void createSphere(ofVboMesh *vbo,
                             float radius,
                             unsigned int rings,
                             unsigned int sectors);

    
    void calculateFrustumSphereIntersects(double fov,
                                          double ratio,
                                          double * latMin,
                                          double * latMax,
                                          double * longMin,
                                          double * longMax);
    
    void drawFrustum();

    void keyPressed(int key);
    void keyReleased(int key);
    //for Gstreamer
    ofxGstRTPClient client1;
    ofTexture texture1;
    //end for gstreamer



    ofMesh mesh;
    ofEasyCam cam;
    
    double latMin, latMax;
    double longMin, longMax;
    
    double fov, ratio, precision, radius;
    int vW, vH;
    
    ofImage img;
};
