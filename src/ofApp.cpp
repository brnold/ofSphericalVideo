
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);

	
	fov = 40;
	precision = 2000;
	radius = 5000;
	
	// initial calculation of segment size
	//this->calculateFrustumSphereIntersects(fov, ratio, &latMin, &latMax, &longMin, &longMax);
	

#if STATIC_IMAGE	
    img.setUseTexture(true);
	img.loadImage("0.jpg");
	ratio = img.getHeight()/(double) img.getWidth();
	//ratio = 3 / (double) 4;
	// initial calculation of segment size
	this->calculateFrustumSphereIntersects(fov, ratio, &latMin, &latMax, &longMin, &longMax);
	this->createSegmentedMesh(ofVec3f(0,0,0), radius, precision, img.getWidth(), img.getHeight(), longMin, longMax, latMin, latMax);
#endif

#if VIDEO
	 vW = 1920;
	 vH = 1080;
	texture1.allocate(vW,vH,GL_RGB);
	//texture2.allocate(vW,vH,GL_RGB);

	client1.setup(10);
	client1.addVideoChannel(5000);
	//client2.setup(10);
	//client2.addVideoChannel(5001);
	client1.play();
	//client2.play();
	ratio = vH/ (double) vW;
	// initial calculation of segment size
	this->calculateFrustumSphereIntersects(fov, ratio, &latMin, &latMax, &longMin, &longMax);
	this->createSegmentedMesh(ofVec3f(0,0,0), radius, precision, vW, vH, longMin, longMax, latMin, latMax);
#endif	

	// generate segment
	//this->createSegmentedMesh(ofVec3f(0,0,0), radius, precision, width, height, longMin, longMax, latMin, latMax);
}

//--------------------------------------------------------------
void ofApp::update(){
#if VIDEO


	client1.update();
	//client2.update();
	
	if(!STATIC_IMAGE){
		if(client1.isFrameNewVideo()){
			texture1.loadData(client1.getPixelsVideo());
			//temp
			//texture2.loadData(client1.getPixelsVideo());
		}



		//if(client2.isFrameNewVideo()){
		//	texture2.loadData(client2.getPixelsVideo());
		//}
	}
#endif
}

//--------------------------------------------------------------
void ofApp::draw(){
    
	ofBackgroundGradient(ofColor(128), ofColor(0));
    
    cam.begin();
	ofEnableAlphaBlending();

	ofPushMatrix();
	ofRotateY(180);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	
	// bind texture and draw our segment
    ofSetColor(255, 150);
   
#if STATIC_IMAGE    
    if (img.isAllocated()) img.bind();
    mesh.draw();
    if (img.isAllocated()) img.unbind();
#endif

#if VIDEO
	texture1.bind();
	mesh.draw();
	texture1.unbind();
#endif	
	// draw transparent sphere as guide
	//ofSetColor(255, 25);
	//ofDrawSphere(0, 0, 250);
	
	//ofSetColor(200, 0, 0, 50);
	//ofDrawSphere(0, 0, 5);
	
	//this->drawFrustum();

	glDisable(GL_CULL_FACE);
	
	ofPopMatrix();
	
	ofDisableAlphaBlending();
    cam.end();

	// draw image
	ofSetColor(255, 255);
	//img.draw(0, 0, img.getWidth(), img.getHeight());
	
	// some info
	std::stringstream info;
	info << "FOV: " << ofToString(fov) << endl
	     << "RATIO: " << ofToString(ratio) << endl
		 << "DISTANCE: " << ofToString(cam.getDistance()) << endl
	     << "-> [ or ] to grow/shrink longitude" << endl
	     << "-> { or } to grow/shrink latitude" << endl
	     << "-> LEFT or RIGHT arrow to move segment longitudal" << endl
		 << "-> DOWN or RIGHT arrow to move segment latitudal" << endl
	     << "-> r or R to change ratio" << endl
	<< "-> SPACE to reset" << endl;
	
	ofDrawBitmapStringHighlight(info.str(), ofGetWidth()-400, 20);
}

void ofApp::createSegmentedMesh(const ofVec3f& center,
                                double radius,
                                int precision,
                                int textWidth,
                                int textHeight,
                                double theta1, double theta2,
                                double phi1, double phi2)
{
    /*
     Create a sphere centered at c, with radius r, and precision n
     Draw a point for zero radius spheres
     Use CCW facet ordering
     Partial spheres can be created using theta1->theta2, phi1->phi2
     in radians 0 < theta < 2pi, -pi/2 < phi < pi/2
     */
    int i,j;
    double t1,t2,t3;
    ofVec3f e,p;
    
    mesh.clear();

	/* Handle special cases */
    if (radius < 0)
        radius = -radius;
    if (precision < 0)
        precision = -precision;
    if (precision < 4 || radius <= 0) {
        mesh.addVertex(center);
        return;
    }
    
    for (j=0;j<precision/2;j++) {
        t1 = phi1 + j * (phi2 - phi1) / (precision/2);
        t2 = phi1 + (j + 1) * (phi2 - phi1) / (precision/2);

        mesh.setMode(OF_PRIMITIVE_POINTS );
        
        for (i=0;i<=precision;i++) {
            t3 = theta1 + i * (theta2 - theta1) / precision;
            
            e.x = cos(t1) * cos(t3);
            e.y = sin(t1);
            e.z = cos(t1) * sin(t3);
            p.x = center.x + radius * e.x;
            p.y = center.y + radius * e.y;
            p.z = center.z + radius * e.z;
            mesh.addNormal(e);
            mesh.addTexCoord(ofVec2f( (i/(double)precision) * textWidth,
                                      textHeight - (2*j/(double)precision) * textHeight));
            mesh.addVertex(p);
            
            e.x = cos(t2) * cos(t3);
            e.y = sin(t2);
            e.z = cos(t2) * sin(t3);
            p.x = center.x + radius * e.x;
            p.y = center.y + radius * e.y;
            p.z = center.z + radius * e.z;
            mesh.addNormal(e);
            mesh.addTexCoord(ofVec2f( (i/(double)precision) * textWidth,
                                      textHeight - (2*(j+1)/(double)precision) * textHeight));
            mesh.addVertex(p);
		}
    }
}

void ofApp::calculateFrustumSphereIntersects(double fov,
											 double ratio,
											 double * latMin,
											 double * latMax,
											 double * longMin,
											 double * longMax)
{
	*longMin = HALF_PI - (fov * DEG_TO_RAD / 2.0);
	*longMax = HALF_PI + (fov * DEG_TO_RAD / 2.0);
	
	*latMin = -(fov * DEG_TO_RAD / 2.0 * ratio);
	*latMax =  (fov * DEG_TO_RAD / 2.0 * ratio);
}
/*void ofApp::drawFrustum()
{
	ofSetColor(255, 100);
	ofPoint p;
	
	p.x = cos(latMin) * cos(longMin);
	p.y = sin(latMin);
	p.z = cos(latMin) * sin(longMin);
	ofDrawLine(ofPoint(0), p*radius);
	
	p.x = cos(latMax) * cos(longMin);
	p.y = sin(latMax);
	p.z = cos(latMax) * sin(longMin);
	ofDrawLine(ofPoint(0), p*radius);
	
	p.x = cos(latMin) * cos(longMax);
	p.y = sin(latMin);
	p.z = cos(latMin) * sin(longMax);
	ofDrawLine(ofPoint(0), p*radius);
	
	p.x = cos(latMax) * cos(longMax);
	p.y = sin(latMax);
	p.z = cos(latMax) * sin(longMax);
	ofDrawLine(ofPoint(0), p*radius);
}*/

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ']')
    {
        longMax = MIN( (longMax += 0.1), TWO_PI);
    }
    else if (key == '[')
    {
        longMax = MAX( (longMax -= 0.1), 0);
    }
    if (key == '}')
    {
        latMax = MIN( (latMax += 0.1), HALF_PI);
    }
    else if (key == '{')
    {
        latMax = MAX( (latMax -= 0.1), -HALF_PI);
    }
    else if (key == OF_KEY_LEFT)
    {
		double diff = longMax-longMin;
        longMin = MAX( (longMin -= 0.1), 0);
		longMax = longMin + diff;
    }
    else if (key == OF_KEY_RIGHT)
    {
		double diff = longMax - longMin;
		longMin = MIN( (longMin += 0.1), TWO_PI);
		longMax = longMin + diff;
    }
    else if (key == OF_KEY_DOWN)
    {
		double diff = latMax - latMin;
        latMin = MAX( (latMin -= 0.1), -HALF_PI);
		latMax = latMin + diff;
    }
    else if (key == OF_KEY_UP)
    {
		double diff = latMax - latMin;
		latMin = MIN( (latMin += 0.1), HALF_PI);
		latMax = latMin + diff;
    }
	else if (key == 'r')
	{
		if (fov > 1) --fov;
		this->calculateFrustumSphereIntersects(fov, ratio, &latMin, &latMax, &longMin, &longMax);
	}
	else if ( key == 'R')
	{
		if (fov < 180) ++fov;
		this->calculateFrustumSphereIntersects(fov, ratio, &latMin, &latMax, &longMin, &longMax);
	}
	else if (key == 'i')
	{
		cam.setDistance(1);
	}
	else if (key == ' ')
	{
		ratio = 3 / (double)4;
		fov = 60;
		this->calculateFrustumSphereIntersects(fov, ratio, &latMin, &latMax, &longMin, &longMax);
		cam.reset();
	}

#if STATIC_IMAGE	
	this->createSegmentedMesh(ofVec3f(0,0,0), radius, precision, img.getWidth(), img.getHeight(), longMin, longMax, latMin, latMax);
#endif

#if VIDEO
	this->createSegmentedMesh(ofVec3f(0,0,0), radius, precision, vW, vH, longMin, longMax, latMin, latMax);
#endif	
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'I')
	{
		cam.setDistance(700);
	}
}
