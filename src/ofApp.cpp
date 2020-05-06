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
        triggerSample=false;
        reverse=false;
        reverse2=false;
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
//    samp[0].load(ofToDataPath("allFiles/longBreathing.wav"));
//    samp[1].load(ofToDataPath("allFiles/Inhale.wav"));
//    samp[2].load(ofToDataPath("allFiles/Jewish.wav"));
//    samp[3].load(ofToDataPath("allFiles/Jewish2.wav"));
//    samp[4].load(ofToDataPath("allFiles/statues.wav"));
//    samp[5].load(ofToDataPath("allFiles/alter.wav"));
//    samp[6].load(ofToDataPath("allFiles/Inhale.wav"));
//    samp[7].load(ofToDataPath("allFiles/Inhale2.wav"));
//    samp[8].load(ofToDataPath("allFiles/Breath1.wav"));
//    samp[9].load(ofToDataPath("allFiles/AsI.wav"));
//    samp[10].load(ofToDataPath("allFiles/Bushes.wav"));
//    samp[11].load(ofToDataPath("allFiles/Bushes.wav"));
    

    samp[0].load(ofToDataPath("allFiles/longBreathing.wav"));
    samp[1].load(ofToDataPath("allFiles/Inhale.wav"));
    samp[2].load(ofToDataPath("allFiles/Jewish.wav"));
    samp[3].load(ofToDataPath("allFiles/Jewish2.wav"));
    samp[4].load(ofToDataPath("allFiles/statues.wav"));
    samp[5].load(ofToDataPath("allFiles/alter.wav"));
    samp[6].load(ofToDataPath("allFiles/Inhale.wav"));
    samp[7].load(ofToDataPath("allFiles/Inhale2.wav"));
    samp[8].load(ofToDataPath("allFiles/Breath1.wav"));
    samp[9].load(ofToDataPath("allFiles/AsI.wav"));
    samp[10].load(ofToDataPath("allFiles/Bushes.wav"));
    samp[11].load(ofToDataPath("allFiles/Bushes.wav"));
    
//    +++++++++++++++
//    pitchStretch
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
    
//    mids
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
//
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
        //Store objects' centers
//        blobs = contourFinderCV.getTracker();
//        int blobSize = blobs.size();
//        cout << blobSize << endl;
    }
    
    if (triggerSample==true) {triggered1(true);}
    if (triggerSample2==true) {triggered2();}

    if (kinectOn2==true) {
        nearScan=200;
    }

    scanEndX=scanStartX+scanWidthX;
    farScan=nearScan-depthScanWidth;
    timer+=0.5;
}
//--------------------------------------------------------------
void ofApp::draw(){
    w=depthImage.getWidth();
    h=depthImage.getHeight();
    ofSetBackgroundAuto(showLabels);
    RectTracker& tracker = contourFinderCV.getTracker();
    
//    scan scan scan
//    main display for testing
    ofPushMatrix();
    ofSetColor(235+ofMap(sin(timer),-1,1,-20,20));
    ofTranslate(ofGetWidth()/4,0);
    ofDrawRectangle(scanStartX+ofMap(sin(timer), -1,1,-30,30), scanStartY, scanEndX-scanStartX, scanEndY-scanStartY);
    ofPopMatrix();
    
// small display for projector - remove when projecting
    ofPushMatrix();
    ofSetColor(255);
    ofTranslate(0, 2.4*ofGetHeight()/4);
    ofScale(0.4, 0.4);
    depthImage.draw(0,0,kinect.getWidth(),kinect.getHeight());
    ofDrawRectangle(scanStartX, scanStartY, scanEndX-scanStartX, scanEndY-scanStartY);
    ofPopMatrix();
    
//       __    _              __
//      / /__ (_)__  ___ ____/ /_
//     /  '_// / _ \/ -_) __/ __/
//    /_/\_\/_/_//_/\__/\__/\__/
//    current turn on is left arrow - will change to be a trigger - could just be time ellapsed
    if(kinectOn2) {kinectOn2Fun();}
    if (kinectOn) {kinectOnFun();}
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
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
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
            LFO=ofMap(preLFO,-1,1,0.3,0.8); /* LFO */
            
            float blobMapped = ofMap(blob1.x,0,640,0.1,0.9); /* direct mapping */
            float areaMapped = ofMap(area,20000,180000,1,0.3);/* inverse mapping so far away is loud*/
            double areaMappedLite = ofMap(area,20000,180000,0.9,1.1);/* inverse mapping so far away is loud*/
            bassEQ=areaMapped;
        
//       ++++++++++++++
//        setup synths
//       ++++++++++
        
            if(triggerSample==true){
            words =words_Vec[currentSynth]->play(wordPitch, 0.8, grainLength[currentSetting], overlaps[currentSetting]+5);
//        +++++++++++
//        bass
//        ++++++++++
            wave[3]=osc[4].sinewave(0.1); /* working as an LFO */
            wave[0]=  osc[1].sinewave(45.375); /* Colundi Tuning - from Aleksi Perälä*/
            }
        
            if(triggerSample2==true){
//                mids[0]->setPosition(ofMap(sin(timer), -1, 1, 0.2, 0.6));
                if(sampleTrigger==true){
                    synthsV[2]->setPosition(0.1);
                    samplePlay=synthsV[2]->play(0.7, 1, 0.05, 3);
                }
                breaths=highs[currentSynth]->play(0.4, 0.4, grainLength[currentSetting], overlaps[currentSetting]); /* originally set to 0.2 for speed and pitch*/
                delayedBreaths = delayed.dl(breaths, 12000, 0.8);
            }
//            midLevel=mids[0]->play(0.75, ofMap(sin(timer),-1,1,0.9,0.6), 0.05, 9);
            midLevel=mids[0]->play(0.75, 0.8, 0.3, 9);
        
//        +++++++++++
//        bass
//        ++++++++++
//            wave[3]=osc[4].sinewave(0.1); /* working as an LFO */
//            wave[0]=  osc[1].sinewave(45.375); /* Colundi Tuning - from Aleksi Perälä*/
        
//        +++++++++++
//        synths
//        ++++++++++
            synthsV[0]->setPosition(ofMap(wave[3],-1, 1,0.01, 0.08));
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
//        +++++++++
//        output[i*nChannels    ] = ((outputs[0]*0.7)+(delays[0]*0.55)+(bassTones[0])+(altOutputs[0]*0.45))*0.25; /*standard eq*/
//        output[i*nChannels + 1] = ((outputs[0]*0.7)+(delays[0]*0.55)+(bassTones[0])+(altOutputs[0]*0.45))*0.25;
//        output[i*nChannels    ] = ((outputs[0]*0.7)+(delays[0]*0.55)+(bassTones[0])+(altOutputs[0]*0.45))*areaMapped/4;
//        output[i*nChannels + 1] = ((outputs[0]*0.7)+(delays[0]*0.55)+(bassTones[0])+(altOutputs[0]*0.45))*areaMapped/4;
            double altMasterEQ=0.1;
            output[i*nChannels    ] = (outputs[0]*0.1)+(altOutputs[0]*0.012)+(delays[0]*0.005)+(bassTones[0]*bassAM)+(sampleOutputs[0]*0.75)*altMasterEQ;
            output[i*nChannels + 1] = (outputs[1]*0.1)+(altOutputs[1]*0.012)+(delays[0]*0.005)+(bassTones[1]*bassAM)+(sampleOutputs[1]*0.75)*altMasterEQ;
    }
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    for(int i = 0; i < bufferSize; i++){} /* left in for vocal input option - use with ofxSoundObjects to save audio clips as files*/
}

//--------------------------------------------------------------
void ofApp::kinectOnFun(){
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
void ofApp::kinectOn2Fun(){
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
void ofApp::triggered1(bool check){
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
void ofApp::triggered2(){
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
                kinectOn=!kinectOn; /* currently turnign system on and setting background with left button*/
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
void ofApp::mouseMoved(int x, int y){
    
//    float x1 =  myFilter.lopass(x,0.25);
//    float y1 = myFilter2.lopass(y,0.25);
    
//    if (isTraining) {
        
//        speed = ((double ) x1 / ofGetWidth() * 4.0) - 2.0;
//        grainLength = ((double) y1 / ofGetHeight() * 0.1) + 0.001;
//        if (grainLength < 0.01 ) grainLength = 0.01;
//        if (grainLength > 0.4 ) grainLength = 0.4;
//        pos = ((double) x1 / ofGetWidth() * 2.0);
        //    cout << pos << endl;
//    }
    
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
//    use this code to cycle through samples + soundscapes
//    if (++current > bed.size()-1) current = 0;
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
//--------------------------------------------------------------
void ofApp::exit() {
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();
    ofSoundStreamClose();
}
