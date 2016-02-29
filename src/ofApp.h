#pragma once

#include "ofMain.h"
#include "ofxGstRTPClient.h"
#include "ofxGstRTPServer.h"
#include "ofxOculusDK2.h"

#define STATIC_IMAGE 1
#define VIDEO 0
#define OCULUS_YAH 0

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    void createSegmentedMesh(const ofVec3f& center,
                            ofMesh &mesh,
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

    void createSegmentedMeshMine(const ofVec3f& center,
                                ofMesh &mesh,
                                double radius,
                               
                                int textWidth,
                                int textHeight);

    
    void drawFrustum();

    void keyPressed(int key);
    void keyReleased(int key);
    //for Gstreamer
    ofxGstRTPClient client1, client2;
    ofTexture texture1, texture2;
    //end for gstreamer

    //for Occulus
    void drawScene(int side);
    //void drawSceneVideo(int side);
    ofxOculusDK2        oculusRift;
    //end for occulus



    ofMesh mesh1, mesh2;
    ofEasyCam cam;
    
    double latMin, latMax;
    double longMin, longMax;
    
    double fov, ratio, precision, radius;
    int vW, vH;
    
    ofImage img, img2;
};
