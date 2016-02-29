
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	#if OCULUS_YAH

	oculusRift.baseCamera = &cam; //attach to your camera
    //opens the device, an Oculus must be plugged in 
    //as it uses the params returned from the head set to configure 
    //the resolution settings
    oculusRift.setup();

    ofSetVerticalSync(true);
	
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	ofEnableDepthTest();

	//ofEnableNormalizedTexCoords();
	
	// this sets the camera's distance from the object
	cam.setDistance(5);


    ofSetFrameRate(75);

#endif

  
	
	fov = 180;
	precision = 1500;
	//radius = 5000;
	radius = 100;
	
	// initial calculation of segment size
	//this->calculateFrustumSphereIntersects(fov, ratio, &latMin, &latMax, &longMin, &longMax);
	

#if STATIC_IMAGE	
    img.setUseTexture(true);
	img.loadImage("0_r.jpg");
	ratio = img.getHeight()/(double) img.getWidth();
	// initial calculation of segment size
	this->calculateFrustumSphereIntersects(fov, ratio, &latMin, &latMax, &longMin, &longMax);
	//this->createSegmentedMesh(ofVec3f(0,0,0), radius, precision, img.getWidth(), img.getHeight(), longMin, longMax, latMin, latMax);
	this->createSegmentedMeshMine(ofVec3f(0,0,0), mesh1, radius, img.getWidth(), img.getHeight());
	
	img2.setUseTexture(true);
	img2.loadImage("0_r.jpg");
	ratio = img2.getHeight()/(double) img2.getWidth();


	this->createSegmentedMeshMine(ofVec3f(0,0,0), mesh2, radius, img2.getWidth(), img2.getHeight());
#endif

#if VIDEO
	vW = 1920;
	vH = 1080;
	texture1.allocate(vW,vH,GL_RGB);
	texture2.allocate(vW,vH,GL_RGB);

	client1.setup(10);
	client1.addVideoChannel(5000);
	client2.setup(10);
	client2.addVideoChannel(5001);
	client1.play();
	//client2.play();
	ratio = vH/ (double) vW;
	// initial calculation of segment size
	//this->calculateFrustumSphereIntersects(fov, ratio, &latMin, &latMax, &longMin, &longMax);
	this->createSegmentedMeshMine(ofVec3f(0,0,0), mesh1, radius, vW, vH);
	this->createSegmentedMeshMine(ofVec3f(0,0,0), mesh2, radius, vW, vH);
	//this->createSegmentedMesh(ofVec3f(0,0,0), mesh2, radius, precision, vW, vH, longMin, longMax, latMin, latMax);

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
			texture2.loadData(client1.getPixelsVideo());
		}



		//if(client2.isFrameNewVideo()){
		//	texture2.loadData(client2.getPixelsVideo());
		//}
	}
#endif
}

//--------------------------------------------------------------
void ofApp::draw(){
    
	//ofBackgroundGradient(ofColor(128), ofColor(50));
    
    cam.begin();
	
	// bind texture and draw our segment
    //ofSetColor(255, 150);
	//ofBackground(100,100,100);   
    
    if(OCULUS_YAH == 1){
	    oculusRift.beginLeftEye();
	    drawScene(0);
	    oculusRift.endLeftEye();

	    oculusRift.beginRightEye(); 
	    drawScene(1);  
	    oculusRift.endRightEye();
	}else{


	drawScene(1);
    }

	

    
    cam.end();

	// draw image
	//ofSetColor(255, 255);
	//img.draw(0, 0, img.getWidth(), img.getHeight());
	
	oculusRift.draw();
/*
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
	*/
}

void ofApp::drawScene(int side){

	ofEnableAlphaBlending();

	

	ofPushMatrix();
	ofRotateX(90);
	ofRotateZ(180);
	ofTranslate(0, -75, 0);
	
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	
if(side == 1){ //right
	#if STATIC_IMAGE    
	    if (img.isAllocated()) img.bind();
	    mesh1.draw();
	    if (img.isAllocated()) img.unbind();
	#endif

	#if VIDEO
		texture1.bind();
		mesh1.draw();
		texture1.unbind();
	#endif	

} else{ //left
	#if STATIC_IMAGE    
	    if (img.isAllocated()) img2.bind();
	    mesh2.draw();
	    if (img.isAllocated()) img2.unbind();
	#endif

	#if VIDEO
		texture2.bind();
		mesh2.draw();
		texture2.unbind();
	#endif	
    
}

	ofPopMatrix();
	
	ofDisableAlphaBlending();

	//glDisable(GL_CULL_FACE);

}

/*//temp, brought over for debugging
void ofApp::drawSceneVideo(int side){
	//ofBackground(74, 88, 150);


    ofPushMatrix();
	ofRotate(90, 0, 0, -10);
    ofDrawGridPlane(100.0f, 2.0f, false );
	ofPopMatrix();


	ofPushMatrix();
	

	ofVec3f pos(
		0,
		0,
		0);
	
	float planeSize = 9000;
	
	
	ofTranslate(pos);
	ofRotateX(pos.x);
	ofRotateY(pos.y);
	ofRotateZ(pos.z);
	
	ofEnableDepthTest();

	if(side == 0){ // left eye
		texture1.bind();
		ofFill();
		//mesh.draw();
		ofDrawSphere(0,0,1080);
		texture1.unbind();

		//ofSetColor(255);
		//ofDrawSphere(0,0,0,planeSize);
		
		
	}else{ //right eye

	texture1.bind();
	ofFill();
	//ofSetColor(255);
	ofDrawBox(0,0,0,108,100, 0);
	//ofDrawSphere(0,0,1080);
	texture1.unbind();
		
	}	
	ofDisableDepthTest();	
	//ofNoFill();
	//ofSetColor(ofColor::fromHsb(sinf(t) * 128 + 128, 255, 255));
	//ofDrawBox(planeSize * 1.1f);
	
	ofPopMatrix();
}*/

void ofApp::createSegmentedMesh(const ofVec3f& center,
								ofMesh &mesh,
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
        //mesh.setMode( OF_PRIMITIVE_LINE_STRIP);
        
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

void ofApp::createSegmentedMeshMine(const ofVec3f& center,
								ofMesh &mesh,
                                double radius,
                                int textWidth,
                                int textHeight)
{

	int h, hTemp, w;
	double theta, phi, phi_1, limitH, limitW;
	ofVec3f p;

	mesh.clear();

	//Handle special cases 
    if (radius < 0)
        radius = -radius;
    if (precision < 0)
        precision = -precision;
    if (precision < 4 || radius <= 0) {
        mesh.addVertex(center);
    return;
    }

	//mesh.setupIndicesAuto();
	
	mesh.setMode( OF_PRIMITIVE_TRIANGLE_STRIP);

	limitH = 3.14 / (double) 3;
	limitW = limitH * (textWidth/(double)textHeight);

	for(hTemp = 0; hTemp < textHeight-1; hTemp = hTemp+2)
	{
		
		h = hTemp;
		//phi = (h * 3.141)/(double) textHeight;
		//phi_1 = ((h+1) * 3.141)/(double) textHeight;

		phi = ((h * limitH)/(double) textHeight) + (1.57079632679 - (limitH/ (double )2));
		phi_1 = (((h+1) * limitH)/(double) textHeight) + (1.57079632679 - (limitH/ (double )2));

		for(w = 0; w <= textWidth; w++) //count forward
		{
		
			theta = (limitW * w) / (double) textWidth + (1.57079632679 - (limitW/ (double )2));
			
           // p.x = radius * cos(theta);
            //p.y = h;
            //p.z = radius * sin(theta);

            p.x = radius*cos(theta)*sin(phi);
            p.y = radius* sin(theta)*sin(phi);
            p.z = radius*cos(phi);
   
            mesh.addTexCoord(ofVec2f(w, h));
            mesh.addVertex(p);

            //p.x = radius *cos(theta);
            //p.y = h+1;
            //p.z = radius * sin(theta);
             p.x = radius*cos(theta)*sin(phi_1);
            p.y = radius* sin(theta)*sin(phi_1);
            p.z = radius*cos(phi_1);

            mesh.addTexCoord(ofVec2f(w, h+1));
            mesh.addVertex(p);

		}
		
		h = hTemp+1;
		phi = ((h * limitH)/(double) textHeight) + (1.57079632679 - (limitH/ (double )2));
		phi_1 = (((h+1) * limitH)/(double) textHeight) + (1.57079632679 - (limitH/ (double )2));
		for(w = textWidth; w>=0; w--) //count backwards
		{

			theta = (limitW * w) / (double) textWidth + (1.57079632679 - (limitW/ (double )2));
		 	
		 	p.x = radius*cos(theta)*sin(phi);
            p.y = radius* sin(theta)*sin(phi);
            p.z = radius*cos(phi);
            mesh.addTexCoord(ofVec2f(w, h));
            mesh.addVertex(p);

             p.x = radius*cos(theta)*sin(phi_1);
            p.y = radius* sin(theta)*sin(phi_1);
            p.z = radius*cos(phi_1);
            mesh.addTexCoord(ofVec2f(w, h+1));
            mesh.addVertex(p);

		}
		
	}


    /*
     Create a sphere centered at c, with radius r, and precision n
     Draw a point for zero radius spheres
     Use CCW facet ordering
     Partial spheres can be created using theta1->theta2, phi1->phi2
     in radians 0 < theta < 2pi, -pi/2 < phi < pi/2
     
    int i,j;
    double t1,t2,t3;
    ofVec3f e,p;
    
    mesh.clear();

	//Handle special cases 
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

        //mesh.setMode(OF_PRIMITIVE_POINTS );
        mesh.setMode( OF_PRIMITIVE_LINE_STRIP);
        
        for (i=0;i<=precision;i++) {
            t3 = theta1 + i * (theta2 - theta1) / precision;
            
            e.x = cos(t1) * cos(t3);
            e.y = sin(t1);
            e.z = cos(t1) * sin(t3);
            p.x = center.x + radius * e.x;
            p.y = center.y + radius * e.y;
            p.z = center.z + radius * e.z;
            //mesh.addNormal(e);
            mesh.addTexCoord(ofVec2f( (i/(double)precision) * textWidth,
                                      textHeight - (2*j/(double)precision) * textHeight));
            mesh.addVertex(p);
            
            e.x = cos(t2) * cos(t3);
            e.y = sin(t2);
            e.z = cos(t2) * sin(t3);
            p.x = center.x + radius * e.x;
            p.y = center.y + radius * e.y;
            p.z = center.z + radius * e.z;
            //mesh.addNormal(e);
            mesh.addTexCoord(ofVec2f( (i/(double)precision) * textWidth,
                                      textHeight - (2*(j+1)/(double)precision) * textHeight));
            mesh.addVertex(p);
		}
    }*/
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
	else if (key == 'F' || key == 'f')
	{
		ofToggleFullscreen();
	}
	else if (key == 'Q' || key == 'q')
	{
		oculusRift.dismissSafetyWarning();
	}
#if STATIC_IMAGE	
	//this->createSegmentedMeshMine(ofVec3f(0,0,0), mesh1, radius, vW, vH);
	//this->createSegmentedMeshMine(ofVec3f(0,0,0), mesh2, radius, vW, vH);
#endif

#if VIDEO
	//this->createSegmentedMesh(ofVec3f(0,0,0), mesh1, radius, precision, vW, vH, longMin, longMax, latMin, latMax);
	//this->createSegmentedMesh(ofVec3f(0,0,0), mesh2, radius, precision, vW, vH, longMin, longMax, latMin, latMax);
    this->createSegmentedMeshMine(ofVec3f(0,0,0), mesh1, radius, vW, vH);
	this->createSegmentedMeshMine(ofVec3f(0,0,0), mesh2, radius, vW, vH);
#endif	
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'I')
	{
		cam.setDistance(700);
	}
}
