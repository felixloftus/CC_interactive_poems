/* This is an example of how to integrate maximilain into openFrameworks,
 including using audio received for input and audio requested for output.
 
 
 You can copy and paste this and use it as a starting example.
 
 */
// notes on progression
// have words separate from the underlying sound
// then just have the underlying sound slowly morphing
// then I only need to do a progression for words
// next to do is to set up the kinect2on with a different depth value
// another thing to do is to map the bar to the whole projected screen
// then have a camera at the bottom
//
//
//
#include "ofApp.h"
#include "time.h"
using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    
        
    ofSetLogLevel(OF_LOG_VERBOSE);

//     ___  __    ___  ________   _______   ________ _________
//    |\  \|\  \ |\  \|\   ___  \|\  ___ \ |\   ____\\___   ___\
//    \ \  \/  /|\ \  \ \  \\ \  \ \   __/|\ \  \___\|___ \  \_|
//     \ \   ___  \ \  \ \  \\ \  \ \  \_|/_\ \  \       \ \  \
//      \ \  \\ \  \ \  \ \  \\ \  \ \  \_|\ \ \  \____   \ \  \
//       \ \__\\ \__\ \__\ \__\\ \__\ \_______\ \_______\  \ \__\
//        \|__| \|__|\|__|\|__| \|__|\|_______|\|_______|   \|__|
//                                                   initialising
    
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    if (kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    
    depthImage.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    diff.allocate(kinect.width,kinect.height);
    
    angle = 0;
    kinect.setCameraTiltAngle(angle);
    
    //    ++++++++++++
    //    Triggers
    //    +++++++++++
        
        kinectOn=false;
        kinectOn2=false;
        kinectOn3=false;
        kinectOn4=false;
        triggerSample=false;
        triggerSample3=false;
        triggerSample4=false;
        reverse=false;
        reverse2=false;
        reverse3 = false;
        reverse4=false;
        delayLine=false;
        sampleTrigger=false;
//
//███████  ██████  █████  ███    ██ ███    ██ ██ ███    ██  ██████
//██      ██      ██   ██ ████   ██ ████   ██ ██ ████   ██ ██
//███████ ██      ███████ ██ ██  ██ ██ ██  ██ ██ ██ ██  ██ ██   ███
//     ██ ██      ██   ██ ██  ██ ██ ██  ██ ██ ██ ██  ██ ██ ██    ██
//███████  ██████ ██   ██ ██   ████ ██   ████ ██ ██   ████  ██████
//                                                          setup
        
        scanWidthX=5;
        scanWidthY=kinect.getHeight();
        scanStartX=100;
        scanStartY=0;
        scanEndY= scanStartY+scanWidthY;
    
        altScanStartY=100;
        altScanWidthX=kinect.getWidth();
        altScanStartX=0;
        altScanEndX=altScanStartX+altScanWidthX;
        nearScan=150;
        depthScanWidth=10;
        
        contourFinderCV.setMinAreaRadius(70);
        contourFinderCV.setMaxAreaRadius(1250);
        contourFinderCV.setThreshold(15);
        contourFinderCV.getTracker().setPersistence(15); /*wait for half a second before forgetting something */
        contourFinderCV.getTracker().setMaximumDistance(64); /* originally set to 32  */
        
        showLabels = true;
//     _____ ______   ________     ___    ___ ___  _____ ______
//    |\   _ \  _   \|\   __  \   |\  \  /  /|\  \|\   _ \  _   \
//    \ \  \\\__\ \  \ \  \|\  \  \ \  \/  / | \  \ \  \\\__\ \  \
//     \ \  \\|__| \  \ \   __  \  \ \    / / \ \  \ \  \\|__| \  \
//      \ \  \    \ \  \ \  \ \  \  /     \/   \ \  \ \  \    \ \  \
//       \ \__\    \ \__\ \__\ \__\/  /\   \    \ \__\ \__\    \ \__\
//        \|__|     \|__|\|__|\|__/__/ /\ __\    \|__|\|__|     \|__|
//                                |__|/ \|__|
//
    
    ofEnableAlphaBlending();
    ofSetupScreen();
    ofBackground(255);
    ofSetFrameRate(20);
    
    sampleRate     = 44100;
    bufferSize    = 512;
    
//    ++++++++++++++
//    samples
//    +++++++++++

    samp[0].load(ofToDataPath("allFiles/longBreathing.wav"));
    samp[1].load(ofToDataPath("allFiles/Inhale.wav"));
    samp[2].load(ofToDataPath("allFiles/Jewish.wav"));
    samp[3].load(ofToDataPath("allFiles/Jewish2.wav"));
    samp[4].load(ofToDataPath("allFiles/statues.wav"));
    samp[5].load(ofToDataPath("allFiles/alter.wav"));
    samp[6].load(ofToDataPath("allFiles/Inhale.wav"));
    samp[7].load(ofToDataPath("allFiles/i3.wav"));
    samp[8].load(ofToDataPath("allFiles/Breath1.wav"));
    samp[9].load(ofToDataPath("allFiles/AsI.wav"));
    samp[10].load(ofToDataPath("allFiles/Bushes.wav"));
    samp[11].load(ofToDataPath("allFiles/AsI.wav"));
    
//    +++++++++++++++
//    pitchStretch objects
//    +++++++++++
    
    int v0=ofRandom(11);
    int v1=ofRandom(11);
    int v2=ofRandom(11);
    int v3=ofRandom(11);
    int v4=ofRandom(11);
    int v5=ofRandom(11);
    int v6=ofRandom(11);
    int v7=ofRandom(11);
    int v8=ofRandom(11);
    int v9=ofRandom(11);
    int v10=ofRandom(11);
    int v11=ofRandom(11);
//    gran[0] = new maxiPitchStretch<grainPlayerWin>(&samp[v0]);
//    gran[1] = new maxiPitchStretch<grainPlayerWin>(&samp[v1]);
//    gran[2] = new maxiPitchStretch<grainPlayerWin>(&samp[v2]);
//    voc[0] = new maxiPitchStretch<grainPlayerWin>(&samp[v3]);
//    voc[1] = new maxiPitchStretch<grainPlayerWin>(&samp[v4]);
//    voc[2] = new maxiPitchStretch<grainPlayerWin>(&samp[v5]);
//    breath[0] = new maxiPitchStretch<grainPlayerWin>(&samp[v6]);
//    breath[1] = new maxiPitchStretch<grainPlayerWin>(&samp[v7]);
//    breath[2] = new maxiPitchStretch<grainPlayerWin>(&samp[v8]);
//    synths[0] = new maxiPitchStretch<grainPlayerWin>(&samp[v9]);
//    synths[1] = new maxiPitchStretch<grainPlayerWin>(&samp[v10]);
//    synths[2] = new maxiPitchStretch<grainPlayerWin>(&samp[v11]);
    
    gran[0] = new maxiPitchStretch<grainPlayerWin>(&samp[0]);
    gran[1] = new maxiPitchStretch<grainPlayerWin>(&samp[1]);
    gran[2] = new maxiPitchStretch<grainPlayerWin>(&samp[2]);
    voc[0] = new maxiPitchStretch<grainPlayerWin>(&samp[3]);
    voc[1] = new maxiPitchStretch<grainPlayerWin>(&samp[4]);
    voc[2] = new maxiPitchStretch<grainPlayerWin>(&samp[5]);
    breath[0] = new maxiPitchStretch<grainPlayerWin>(&samp[6]);
    breath[1] = new maxiPitchStretch<grainPlayerWin>(&samp[7]);
    breath[2] = new maxiPitchStretch<grainPlayerWin>(&samp[8]);
    synths[0] = new maxiPitchStretch<grainPlayerWin>(&samp[9]);
    synths[1] = new maxiPitchStretch<grainPlayerWin>(&samp[10]);
    synths[2] = new maxiPitchStretch<grainPlayerWin>(&samp[11]);
    
//    ++++++++++++++++++++++
//    push to three vectors
//    +++++++++++++++++
    
    mids.push_back(gran[0]);
    mids.push_back(gran[1]);
    mids.push_back(gran[2]);
    words_Vec.push_back(voc[0]);
    words_Vec.push_back(voc[1]);
    words_Vec.push_back(voc[2]);
    highs.push_back(breath[0]);
    highs.push_back(breath[1]);
    highs.push_back(breath[2]);
    layer1.push_back(voc[0]);
    layer1.push_back(voc[1]);
    layer1.push_back(voc[2]);
    synthsV.push_back(synths[0]);
    synthsV.push_back(synths[1]);
    synthsV.push_back(synths[2]);
    
//    ++++++++++++++++++
//    three settings [to start]
//    +++++++++++++++++
    
//    Modulation settings
    int midsTag = 0;
    speed[0] = 1.0;
    grainLength[0] = 0.05;
    pitch[0] = 1.0;
    overlaps[0] = 5;
    
//    bass
    speed[1] = 0.6;
    grainLength[1] = 0.5;
    pitch[1] = 0.5;
    overlaps[1] = 5;
    
//    highs
    speed[2] = 2.6;
    grainLength[2] = 0.1;
    pitch[2] = 4.2;
    overlaps[2] = 6;
    wordPitch=5;
    
    currentSynth=0;
    currentSetting=0;
    
//    eq's
    wordsEQ = 0.35;
    midsEQ = 0.9;
    breathEQ = 0.2;/*was 0.15*/
    
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    
    ofBackground(0,0,0);
    
    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4);
    
}

//--------------------------------------------------------------
void ofApp::update(){
//       __    _              __
//      / /__ (_)__  ___ ____/ /_
//     /  '_// / _ \/ -_) __/ __/
//    /_/\_\/_/_//_/\__/\__/\__/
//      Updates Kinect and does background subtraction with the diff image
//       based on Theo Papatheodorou's code from Workshops in Creative Coding Course [Goldsmiths]
    
    kinect.update();
    if(kinect.isFrameNew()) {
        depthImage.setFromPixels(kinect.getDepthPixels());
        grayImage.setFromPixels(kinect.getDepthPixels());
        depthImage.flagImageChanged();
        grayImage.setFromPixels(kinect.getDepthPixels());
        blurred=grayImage;
        blurred.blurGaussian(3); /* check values for this as kinect has low pixel count*/
        //Store first frame to background image
        if ( !background.bAllocated ) {
            background = blurred;
        }
        diff = blurred; /* Find difference of the frame and background */
        diff.absDiff(blurred, background); /* get pixel difference*/
        contourFinderCV.findContours(diff); /* Find contours*/
    }
    
//    Main Triggers for change in progression
    if (triggerSample==true) {triggered1(true);}
    if (triggerSample2==true) {triggered2();}
    if (kinectOn2==true) {
        nearScan=200;
    }
//    optional other triggers - not currently used as my room is too small for lots of triggers
    if (triggerSample3==true) {triggered3(true);}
    if (triggerSample4==true) {triggered4();}
    if (kinectOn4==true) {

    }

//    sets the sections of the depth image that are scanned
    scanEndX=scanStartX+scanWidthX;
    altScanEndY=altScanStartY+scanWidthX;
    farScan=nearScan-depthScanWidth;
    
//    timer for some modulations
    timer+=0.5;
}
//--------------------------------------------------------------
void ofApp::draw(){
    w=depthImage.getWidth();
    h=depthImage.getHeight();
    
    ofSetBackgroundAuto(showLabels);
    RectTracker& tracker = contourFinderCV.getTracker(); /* for tracking audience members - based on ofxCV [Kyle McDonald example - contour tracking */
    
    
//       __    _              __
//      / /__ (_)__  ___ ____/ /_
//     /  '_// / _ \/ -_) __/ __/
//    /_/\_\/_/_//_/\__/\__/\__/
//    current turn on is left arrow - will change to be a trigger - could just be time ellapsed
    if (kinectOn) {kinectOnFun();}
    if(kinectOn2) {kinectOn2Fun();}
    if (kinectOn3) {kinectOn3Fun();}
    if(kinectOn4) {kinectOn4Fun();}

//    diff.draw(0,0,ofGetWidth(), ofGetHeight());
    if(showLabels) {
        ofSetColor(255);
        diff.draw(0, 0);
        contourFinderCV.draw();
        for(int i = 0; i < contourFinderCV.size(); i++) {
            ofPoint center = toOf(contourFinderCV.getCenter(i));
            ofPushMatrix();
            ofTranslate(center.x, center.y);
            int label = contourFinderCV.getLabel(i);
            string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
            ofDrawBitmapString(msg, 0, 0);
            ofVec2f velocity = toOf(contourFinderCV.getVelocity(i));
            ofScale(5, 5);
            ofDrawLine(0, 0, velocity.x, velocity.y);
            ofPopMatrix();
//            cout << msg << endl;
            if (tracker.getAge(i)>30){
            blob1 = toOf(contourFinderCV.getCenter(i));
            area = contourFinderCV.getContourArea(i);
            stationaryCheck = toOf(contourFinderCV.getVelocity(i));
            cout << stationaryCheck << endl;
        }
        }
    }
    
    //    scan scan scan
    //    main display for projector
        ofPushMatrix();
        ofSetColor(235+ofMap(sin(timer),-1,1,-20,20));
        ofScale(3,3);
        ofDrawRectangle(scanStartX+ofMap(sin(timer), -1,1,-30,30), scanStartY, scanEndX-scanStartX, scanEndY-scanStartY);
        ofPopMatrix();
        
    // small display for testing - commented out for projecting
        ofPushMatrix();
        ofSetColor(255);
        ofTranslate(0, 2.4*ofGetHeight()/4);
        ofScale(0.4, 0.4);
//        depthImage.draw(0,0,kinect.getWidth(),kinect.getHeight());
//        ofDrawRectangle(scanStartX, scanStartY, scanEndX-scanStartX, scanEndY-scanStartY);
    //        ofDrawRectangle(altScanStartX, altScanStartY, altScanEndX-altScanStartX, altScanEndY-altScanStartY);
        ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    timerS=ofGetFrameNum();
    double speed=0.8; /* local variable - could be attached to blob data instead*/
    
    for (int i = 0; i < bufferSize; i++){
//        ++++++++++++++
//        sitting still trigger
//        +++++++++++
        
            if (stationaryCheck.x==0) {sC_Counter=sC_Counter+1;}
            if(sC_Counter%23==0){sampleTrigger=!sampleTrigger;}

//        ++++++++++++++
//        effects + variables
//        +++++++++++
            double preLFO=osc[0].sinewave(0.01);
            LFO=ofMap(preLFO,-1,1,0.3,0.8); /* LFO - useful for modulation - could also be attached to blob data for more experimental results */
            
            float blobMapped = ofMap(blob1.x,0,640,0.1,0.9); /* direct mapping */
            float areaMapped = ofMap(area,20000,180000,1,0.3);/* inverse mapping so far away is loud*/
            double areaMappedLite = ofMap(area,20000,180000,0.9,1.1);/* inverse mapping so far away is loud*/
            bassEQ=areaMapped;
        
//       ++++++++++++++
//        setup synths
//       ++++++++++
        if (timer>ofGetFrameRate()*100) {
            speed+=0.01;
        }
            if(triggerSample==true){/* this is true when an audience member steps into a specific area in 3D space*/
            words =words_Vec[currentSynth]->play(wordPitch, 0.8, grainLength[currentSetting], overlaps[currentSetting]+5);
//        +++++++++++
//        bass
//        currently bass is triggered if one trigger is touched - this could be put outside trigger to make bass consistent
//        ++++++++++
            wave[3]=osc[4].sinewave(0.1); /* working as an LFO for the bass gain - connecting this to AreaMapped would change the speed of the bass pulse - currently set as consistent */
            wave[0]=  osc[1].sinewave(45.375); /* Colundi Tuning - from Aleksi Perälä*/
            }
        
            if(triggerSample2==true){ /* this is true when an audience member steps into a specific area in 3D space*/
//                mids[0]->setPosition(ofMap(sin(timer), -1, 1, 0.2, 0.6));
                if(sampleTrigger==true){ /* this is only triggered if an audience member stands still for a little while */
                    synthsV[2]->setPosition(LFO/10);
                    samplePlay=synthsV[2]->play(0.8, 0.8, 0.05, 3);
                }
                breaths=highs[currentSynth]->play(0.4, 0.4, grainLength[currentSetting], overlaps[currentSetting]);
                delayedBreaths = delayed.dl(breaths, 12000, 0.8); /* delay line of breaths */
            }
//            midLevel=mids[0]->play(0.75, ofMap(sin(timer),-1,1,0.9,0.6), 0.05, 9);
            midLevel=mids[0]->play(0.75, 0.8, 0.3, 9);
        
        
//        +++++++++++
//        synths
//        ++++++++++
        
            synthsV[0]->setPosition(ofMap(wave[3],-1, 1,areaMapped/20, 0.08));
            synthsV[1]->setPosition(ofMap(wave[3],-1, 1,0.01, 0.08));
            double bassAM = ofMap(wave[3],-1,1,0.01,0.2);
            wave[2]= synthsV[0]->play(1.0, areaMappedLite, areaMapped/2, 5);
            wave[4]= synthsV[1]->play(1.0, areaMappedLite, areaMapped/2, 5);
            delays[0] = delayed2.dl(wave[2], 1200, 0.5);
        
//        ++++++++++++
//        mixing
//        +++++++++++
        
            mymix.stereo(delayedBreaths, delays, 0.5); /*delays*/
            mymix.stereo(words*wordsEQ+(midLevel*midsEQ)+(breaths*breathEQ), outputs, 0.5); /*mids*/
            mymix.stereo(wave[0], bassTones, 0.5); /*bass */
            mymix.stereo(wave[2]+delays[0]+wave[4], altOutputs, blobMapped);
            mymix.stereo(samplePlay,sampleOutputs, 0.2);
        
//        ++++++++++
//        outputs
//        currently the output gain is attached to the overall blob area
//        +++++++++
        
            output[i*nChannels    ] = (outputs[0]*0.1)+(altOutputs[0]*0.012)+(delays[0]*0.005)+(bassTones[0]*bassAM)+(sampleOutputs[0]*0.25)*areaMapped/4;
            output[i*nChannels + 1] = (outputs[1]*0.1)+(altOutputs[1]*0.012)+(delays[0]*0.005)+(bassTones[1]*bassAM)+(sampleOutputs[1]*0.25)*areaMapped/4;
    }
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    for(int i = 0; i < bufferSize; i++){} /* left in for vocal input option - use with ofxSoundObjects to save audio clips as files*/
}

//--------------------------------------------------------------
void ofApp::kinectOnFun(){ /* first set of scanning */
                if(wordsEQ<0.4) {wordsEQ+=0.1;}
                if(wordPitch>4){wordPitch-=0.15;}
                for(int y=scanStartY; y<scanEndY; y++){
                    for (int x=scanStartX; x<scanEndX; x++) {
                        ofColor colorXY =  depthImage.getPixels().getColor(x, y);
                        float brightXY = colorXY.getBrightness();
//                        cout << brightXY << endl;
                        if (brightXY>farScan && brightXY<nearScan) {
//                            cout << "triggered" << endl;
                            ofSetColor(255,0,0);
                            ofDrawBitmapString("triggered", ofGetWidth()/2, ofGetHeight()/2);
                            triggerSample=true;
                            kinectOn=false;
                        }
                    }
                }
}

//--------------------------------------------------------------
void ofApp::kinectOn2Fun(){  /* second set of scanning */
        check=false;
        delayLine=true;
        wordPitch+=0.1;
        wordsEQ-=0.01;
        if (wordsEQ<0.01) {wordsEQ=0;}
        if (wordPitch>6) {wordPitch=6;}
        speed[0]=1.2;
        for(int y=scanStartY; y<scanEndY; y++){
            for (int x=scanStartX; x<scanEndX; x++) {
                ofColor colorXY =  depthImage.getPixels().getColor(x, y);
                float brightXY = colorXY.getBrightness();
                if (brightXY>farScan && brightXY<nearScan) {
                    ofSetColor(255,0,0);
                    ofDrawBitmapString("triggered", ofGetWidth()/2, ofGetHeight()/2);
                    reverse2=false;
                    triggerSample2=true;
                    kinectOn2=false;
                }
            }
        }
}

//--------------------------------------------------------------
void ofApp::triggered1(bool check){  /* first effect which is triggered if scans conditions are met */
    int startTime = ofGetElapsedTimeMillis();
    if (reverse==false){
//       cout << "playing" << endl;
        triggerSample2=false;
        ofDrawBitmapString("playing", ofGetWidth()/2, ofGetHeight()/2);
        wordPitch-=0.08;
        if (wordPitch<1.0) {
            wordPitch=1;
        }
    }
    if (wordPitch==1) { /* think this can go up into if statement*/
        scanStartX+=5;
        if (scanStartX>500) {
            scanStartX=500;
        }
    }
    if(scanStartX==500){
        kinectOn2=true;
        reverse=true;
//        reverse2=false;
    }
}
//--------------------------------------------------------------
void ofApp::triggered2(){/* second effect which is triggered if scans conditions are met */
    if (reverse2==false) {
      //        I'm trying to use this loop as the off + reset switch
              double time = 0;
              time++;
      //            turn off Granular
                  triggerSample=false;
      //            change to scanStart to another position
                  scanStartX=100;
      //            move to next sample
      //            could add in here removing samples
              if (currentSynth<words_Vec.size()-1) {
                  currentSynth=currentSynth+1;
              } else {currentSynth=0;}
      //            start the cycle again
                  kinectOn=true;
                  reverse=false;
                  reverse2=true;
          }
}

//--------------------------------------------------------------
void ofApp::kinectOn3Fun(){
//                if(wordsEQ<0.4) {wordsEQ+=0.1;}
//                if(wordPitch>4){wordPitch-=0.15;}
                for(int y=altScanStartY; y<altScanEndY; y++){
                    for (int x=altScanStartX; x<altScanEndX; x++) {
                        ofColor colorXY =  depthImage.getPixels().getColor(x, y);
                        float brightXY = colorXY.getBrightness();
//                        cout << brightXY << endl;
                        if (brightXY>farScan && brightXY<nearScan) {
                            cout << "triggered" << endl;
                            ofSetColor(255,0,0);
                            ofDrawBitmapString("triggered", ofGetWidth()/2, ofGetHeight()/2);
                            triggerSample3=true;
                            kinectOn3=false;
                        }
                    }
                }
}

//--------------------------------------------------------------
void ofApp::kinectOn4Fun(){
        check2=false;
//        delayLine=true;
//        wordPitch+=0.1;
//        wordsEQ-=0.01;
//        if (wordsEQ<0.01) {wordsEQ=0;}
//        if (wordPitch>6) {wordPitch=6;}
//        speed[0]=1.2;
        for(int y=altScanStartY; y<altScanEndY; y++){
            for (int x=altScanStartX; x<altScanEndX; x++) {
                ofColor colorXY =  depthImage.getPixels().getColor(x, y);
                float brightXY = colorXY.getBrightness();
                if (brightXY>farScan && brightXY<nearScan) {
                    ofSetColor(255,0,0);
                    ofDrawBitmapString("triggered", ofGetWidth()/2, ofGetHeight()/2);
                    reverse4=false;
                    triggerSample4=true;
                    kinectOn4=false;
                }
            }
        }
}

//--------------------------------------------------------------
void ofApp::triggered3(bool check){
    int startTime = ofGetElapsedTimeMillis();
    if (reverse3==false){
       cout << "playing" << endl;
        triggerSample4=false;
        ofDrawBitmapString("playing", ofGetWidth()/2, ofGetHeight()/2);
//        wordPitch-=0.08;
//        if (wordPitch<1.0) {
//            wordPitch=1;
//        }
    }
//    if (wordPitch==1) { /* think this can go up into if statement*/
        altScanStartY+=5;
        if (altScanStartY>400) {
            altScanStartY=400;
        }
//    }
    if(altScanStartY==400){
        kinectOn4=true;
        reverse3=true;
    }
}
//--------------------------------------------------------------
void ofApp::triggered4(){
    if (reverse4==false) {
      //        I'm trying to use this loop as the off + reset switch
              double time = 0;
              time++;
      //            turn off Granular
                  triggerSample3=false;
      //            change to scanStart to another position
//        if(timer>200){
                  altScanStartY=100;
//        }
      //            move to next sample
      //            could add in here removing samples
//              if (currentSynth<words_Vec.size()-1) {
//                  currentSynth=currentSynth+1;
//              } else {currentSynth=0;}
      //            start the cycle again
                  kinectOn3=true;
                  reverse3=false;
                  reverse4=true;
          }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
            case OF_KEY_UP:
                angle++;
                if(angle>30) angle=30;
                kinect.setCameraTiltAngle(angle);
//                showLabels = !showLabels;
                break;
                
            case OF_KEY_DOWN:
                angle--;
                if(angle<-30) angle=-30;
                kinect.setCameraTiltAngle(angle);
                break;
            case OF_KEY_LEFT:
                kinectOn=!kinectOn; /* currently turning system on and setting background with left button - could easily be changed to happen after a specific amount of time*/
                kinectOn3=!kinectOn3;
                background = blurred;
                break;
            case OF_KEY_RIGHT:
                if (++currentSynth > mids.size()-1) currentSynth = 0;
                break;
            case OF_KEY_BACKSPACE:
                if (++currentSetting > mids.size()-1) currentSetting = 0;
                break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::exit() {
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close(); /* finish the program by closing the kinect and the soundStream*/
    ofSoundStreamClose();
}
