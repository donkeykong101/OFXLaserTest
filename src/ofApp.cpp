#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
    
    laserWidth = 800;
    laserHeight = 800;
    laser.setup(laserWidth, laserHeight);
    
    laser.addProjector(dac);
    // replace this with the IP address of your etherdream (find it with the sitter diagnostic
    // tool at https://ether-dream.com/downloads.html )
    
    string dacIP = "169.254.110.60";
    dac.setup(dacIP);
    
    laser.initGui();
    currentLaserEffect = 0;
    numLaserEffects = 8;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float deltaTime = ofClamp(ofGetLastFrameTime(), 0, 0.2);
    elapsedTime+=deltaTime;
    
    // prepares laser manager to receive new points
    laser.update();
    
}





void ofApp::draw() {
    
    ofBackground(0);
    
    ofNoFill();
    ofSetLineWidth(1);
    ofDrawRectangle(0,0,laserWidth, laserHeight);
    
    int ypos = laserHeight+20;
    ofDrawBitmapString("Current Effect : "+ofToString(currentLaserEffect), 400, ypos+=30);
    ofDrawBitmapString("TAB to change view, F to toggle full screen", 400, ypos+=30);
    ofDrawBitmapString("Left and Right Arrows to change current effect", 400, ypos+=30);
    ofDrawBitmapString("Mouse to draw polylines, 'C' to clear", 400, ypos+=30);
    
    showLaserEffect(currentLaserEffect);
    
    // sends points to the DAC
    laser.send();
    
    laser.drawUI();
    
}


void ofApp :: showLaserEffect(int effectnum) {
    
    
    float left = laserWidth*0.1;
    float top = laserHeight*0.1;
    float right = laserWidth*0.9;
    float bottom = laserHeight*0.9;
    float width = laserWidth*0.8;
    float height = laserHeight*0.8;
    
    switch (currentLaserEffect) {
            
        case 1: {
            
            // LASER LINES
            int numlines = 10;
            
            for(int i = 0; i<numlines; i++) {
                
                float progress =(float)i/(float)(numlines-1);
                
                float xpos =left + (width*progress);
                
                laser.drawLine(ofPoint(xpos, top+height*0.1), ofPoint(xpos, top+height*0.4), ofColor(255));
                
                ofColor c = ofColor::white;
                //c.setHsb(progress*255, 255, 255);
                
                laser.drawLine(ofPoint(xpos, top+height*0.6), ofPoint(xpos, top+height*0.9), c);
                
            }
            
            break;
            
        }
            
            
        case 2: {
            
            // LASER LINES ANIMATING
            int numlines = 10;
            
            for(int i = 0; i<numlines; i++) {
                
                float progress =(float)i/(float)(numlines-1);
                
                float xpos =left + (width*progress) + (sin(elapsedTime*4+i*0.5)*width*0.05);
                
                laser.drawLine(ofPoint(xpos, top+height*0.1), ofPoint(xpos, top+height*0.4), ofColor(255));
                ofColor c;
                c.setHsb(progress*255, 255, 255);
                laser.drawLine(ofPoint(xpos, top+height*0.6), ofPoint(xpos, top+height*0.9), c);
                
            }
            
            break;
            
        }
            
            
        case 3: {
            
            // LASER CIRCLES
            int numCircles = 6;
            
            for(int i = 0; i<numCircles; i++) {
                
                float progress =(float)i/(float)(numCircles-1);
                
                float xpos =left + (width*progress);
                
                laser.drawCircle(ofPoint(xpos, top+height*0.3),30, ofColor(255));
                ofColor c;
                c.setHsb(progress*255, 255, 255);
                
                laser.drawCircle(ofPoint(xpos, top+height*0.7), 30, c);
                
            }
            
            break;
            
        }
            
        case 4: {
            
            // LASER CIRCLES ANIMATING
            int numCircles = 6;
            
            for(int i = 0; i<numCircles; i++) {
                
                float progress =(float)i/(float)(numCircles-1);
                
                float xpos =left + (width*progress) + (sin(elapsedTime*4+i*0.5)*width*0.05);
                
                laser.drawCircle(ofPoint(xpos, top+height*0.3), 30, ofColor::white);
                ofColor c = ofColor::white;
                //c.setHsb(progress*255, 255, 255);
                
                laser.drawCircle(ofPoint(xpos, top+height*0.7), 30, c);
                
            }
            
            break;
            
        }
            
        case 5: {
            
            // LASER PARTICLES
            int numParticles = 20;
            
            for(int i = 0; i<numParticles; i++) {
                
                float progress =(float)i/(float)(numParticles-1);
                
                float xpos =left + (width*progress) ;
                
                laser.drawDot(ofPoint(xpos, top+height*0.3), ofColor(255));
                ofColor c;
                c.setHsb(progress*255, 255, 255);
                laser.drawDot(ofPoint(xpos, top+height*0.7), c);
                
            }
            
            break;
            
        }
        case 6: {
            
            // LASER PARTICLES ANIMATING
            
            float speed = 1;
            for(int i = 0; i<30; i++) {
                
                ofColor c;
                c.setHsb(i*6,255,255);
                ofPoint p;
                float spread = ofMap(cos(elapsedTime*0.4),1,-1,0.01,0.1);
                p.x = sin((elapsedTime-((float)i*spread)) *1.83f * speed) * 300;
                p.y = sin((elapsedTime-((float)i*spread)) *2.71f *speed) * 300;
                p.x+=laserWidth/2;
                p.y+=laserHeight/2;
                
                laser.drawDot(p, c);
                
            }
            
            break;
            
        }
    }
    
    // LASER POLYLINES
    float t = 0.1 * ofGetElapsedTimef();
    
    float val = ofMap(t - (float)((int)t),0,1,0.2,0.75);
    ofSetColor(ofColor::white);
    ofDrawBitmapString("val: " + ofToString(val), 20,20);
    ofColor c;
    c.setHsb( val*255, 255, 255);
    for(int i = 0; i<polyLines.size(); i++) {
        laser.drawPoly(polyLines[i], c);
    }
    
    
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key =='c') {
        polyLines.clear();
    } else if (key == OF_KEY_LEFT) {
        currentLaserEffect--;
        if(currentLaserEffect<0) currentLaserEffect = numLaserEffects-1;
    } else if (key == OF_KEY_RIGHT) {
        currentLaserEffect++;
        if(currentLaserEffect>=numLaserEffects) currentLaserEffect = 0;
    }
    if(key=='f') {
        ofToggleFullscreen();
    }
    if(key==OF_KEY_TAB) {
        laser.nextProjector();
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(!drawingShape) return;
    
    ofPolyline &poly = polyLines.back();
    //if(poly.getofVec3f end =poly.getVertices().back();
    //if(ofDist(x, y, end.x, end.y) > 5) {
    //poly.simplify();
    poly.addVertex(x, y);
    
    //}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    polyLines.push_back(ofPolyline());
    drawingShape = true;
}

void ofApp::mouseReleased(int x, int y, int button) {
    if(drawingShape) {
        ofPolyline &poly = polyLines.back();
        poly = poly.getSmoothed(2);
        drawingShape = false;
    }
    // TODO add dot if the line is super short
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    laser.saveSettings();
    dac.close();
}
