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
//#include "maximilian.h"/* include the lib */
#include "time.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
        
    ofSetLogLevel(OF_LOG_VERBOSE);
//    ofSetFrameRate(60); test frame rates

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
    
    angle = 0;
    kinect.setCameraTiltAngle(angle);
    
    //    ++++++++++++
    //    Triggers
    //    +++++++++++
        
        kinectOn=false;
        kinectOn2=false;
        triggerSample=false;
        reverse=false;
        reverse2=false;
        delayLine=false;
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
    ofSetFrameRate(10);
    
    sampleRate     = 44100;
    bufferSize    = 512;
    
//    ++++++++++++++
//    samples
//    +++++++++++
    //    could improve by changing names and loading with a function
        string path = "/data/allFiles";
    //    dir.listDir("path");
    samp[0].load(ofToDataPath("allFiles/Jewish2.wav"));
    samp[1].load(ofToDataPath("allFiles/Irish.wav"));
    samp[2].load(ofToDataPath("allFiles/Jewish.wav"));
    samp[3].load(ofToDataPath("allFiles/alter.wav"));
    samp[4].load(ofToDataPath("allFiles/StainedGlassKeeps.wav"));
    samp[5].load(ofToDataPath("allFiles/statues.wav"));
    samp[6].load(ofToDataPath("allFiles/Inhale.wav"));
    samp[7].load(ofToDataPath("allFiles/Inhale2.wav"));
    samp[8].load(ofToDataPath("allFiles/Breath1.wav"));
    
//    +++++++++++++++
//    pitchStretch
//    +++++++++++
    
    gran[0] = new maxiPitchStretch<grainPlayerWin>(&samp[0]);
    gran[1] = new maxiPitchStretch<grainPlayerWin>(&samp[1]);
    gran[2] = new maxiPitchStretch<grainPlayerWin>(&samp[2]);
    voc[0] = new maxiPitchStretch<grainPlayerWin>(&samp[3]);
    voc[1] = new maxiPitchStretch<grainPlayerWin>(&samp[4]);
    voc[2] = new maxiPitchStretch<grainPlayerWin>(&samp[5]);
    breath[0] = new maxiPitchStretch<grainPlayerWin>(&samp[6]);
    breath[1] = new maxiPitchStretch<grainPlayerWin>(&samp[7]);
    breath[2] = new maxiPitchStretch<grainPlayerWin>(&samp[8]);
    
//    ++++++++++++++++++++++
//    push to three vectors
//    +++++++++++++++++
    
    mids.push_back(gran[0]);
    mids.push_back(gran[1]);
    mids.push_back(gran[2]);
    bass.push_back(voc[0]);
    bass.push_back(voc[1]);
    bass.push_back(voc[2]);
    highs.push_back(breath[0]);
    highs.push_back(breath[1]);
    highs.push_back(breath[2]);
    
    
//    ++++++++++++++++++
//    three settings [to start]
//    +++++++++++++++++
    
//    mids
//    change to a smarter way of switching rather than going through 0-2
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
    wordPitch=4;
    
    currentSynth=0;
    currentSetting=0;
    
//    eq's
    eq[0]=0.2;
    eq[1]=0.7;
    eq[2]=0.5;
    
    
    fft.setup(1024, 512, 256);
    oct.setup(44100, 1024, 10);
    
    int current = 0;
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    
//    don't think these are needed
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    
    isTraining=true;
    
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
            depthImage.flagImageChanged();
    }
    
    if (triggerSample==true) {
        triggered1(eq[0], eq[1], wordPitch, true);
    }
    
    if (kinectOn2==true) {
        nearScan=200;
    }

    scanEndX=scanStartX+scanWidthX;
    farScan=nearScan-depthScanWidth;
    timer+=0.5;
}

//--------------------------------------------------------------
void ofApp::triggered1(double eq1, double eq2, double pitching, bool check){
//the timing to bring this back up to background noise isn't working
//    this now works - the trigger sample was always on after the first kinect and so was always
//    sending in these values
//    for final - adjust the timing
//    also i feel like all of these changes could be in one - by using a vector that counts how many
//    also I'm unsure why i have to specify eq[0] and 1 in this even though they're specified above
//    maybe I'm thinking about objects not functions
    int startTime = ofGetElapsedTimeMillis();
    if (reverse==false){
       cout << "playing" << endl;
            
        ofDrawBitmapString("playing", ofGetWidth()/2, ofGetHeight()/2);
//        eq[0]-=0.01;
//        eq[1]-=0.01;
//        if (eq[0]<0.2||eq[1]<0.2) {
//            eq[0]=0.2;
//            eq[1]=0.2;
//        }
        wordPitch-=0.08;
        if (wordPitch<1.0) {
            wordPitch=1;
        }
    }
    if (wordPitch==1) { /* think this can go up into if statement*/
        scanStartX+=10;
        if (scanStartX>500) {
            scanStartX=500;
        }
    }
    if(scanStartX==500){
        kinectOn2=true;
//        kinectOn=false;/*not doing anything*/
        reverse=true;
    }
   
}
//--------------------------------------------------------------
void ofApp::triggered2(double eq1, double eq2, double pitching){
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    
    stringstream s;
    s << "Speed: " << speed;
    ofDrawBitmapString(s.str(), 10,750);
    s.str("");
    s << "Grain length: " << round(grainLength[0]*1000.0) << " ms";
    ofDrawBitmapString(s.str(), 180,750);
    
//    scan scan scan

    ofPushMatrix();
    ofSetColor(235+ofMap(sin(timer),-1,1,-20,20));
    ofTranslate(ofGetWidth()/4,0);
    ofDrawRectangle(scanStartX+ofMap(sin(timer), -1,1,-30,30), scanStartY, scanEndX-scanStartX, scanEndY-scanStartY);
    ofPopMatrix();

    
    ofPushMatrix();
    ofSetColor(255);
    ofTranslate(0, 2.4*ofGetHeight()/4);
    ofScale(0.4, 0.4);
    depthImage.draw(0,0,kinect.getWidth(),kinect.getHeight());
    ofDrawRectangle(scanStartX, scanStartY, scanEndX-scanStartX, scanEndY-scanStartY);
    ofPopMatrix();
    
    w=depthImage.getWidth();
    h=depthImage.getHeight();
    
//       __    _              __
//      / /__ (_)__  ___ ____/ /_
//     /  '_// / _ \/ -_) __/ __/
//    /_/\_\/_/_//_/\__/\__/\__/
//    current turn on is left arrow - will change to be a trigger - could just be time ellapsed
//
    
//    if(ofGetElapsedTimeMillis()>5000){kinectOn=true;} /* this line could be problematic because its turning kinecton despite it being off*/
        if (kinectOn) {
            for(int y=scanStartY; y<scanEndY; y++){
                for (int x=scanStartX; x<scanEndX; x++) {
                    ofColor colorXY =  depthImage.getPixels().getColor(x, y);
                    float brightXY = colorXY.getBrightness();
                    cout << brightXY << endl;
                    if (brightXY>farScan && brightXY<nearScan) {
                        cout << "triggered" << endl;
                        ofSetColor(255,0,0);
                        ofDrawBitmapString("triggered", ofGetWidth()/2, ofGetHeight()/2);
                        triggerSample=true;
//                        if(ofGetFrameNum()%13==0){
                        kinectOn=false;
//                        }
                    }
                }
            }
        }
        if (kinectOn2) {
            check=false;
            delayLine=true;
            wordPitch+=0.1;
            eq[2]-=0.01;
            if (eq[2]<0.01) {
                eq[2]=0;
            }
            if (wordPitch>6) {
                wordPitch=6;
            }
            speed[0]=1.2;
            for(int y=scanStartY; y<scanEndY; y++){
                for (int x=scanStartX; x<scanEndX; x++) {
                    ofColor colorXY =  depthImage.getPixels().getColor(x, y);
                    float brightXY = colorXY.getBrightness();
//                    cout << brightXY << endl;
                    if (brightXY>farScan && brightXY<nearScan) {
//                        cout << "triggered" << endl;
                        ofSetColor(255,0,0);
                        ofDrawBitmapString("triggered", ofGetWidth()/2, ofGetHeight()/2);
                        triggerSample2=true;
//                        if(ofGetFrameNum()%13==0){
                        kinectOn2=false;
//                        }
                    }
                }
            }
        }
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    for (int i = 0; i < bufferSize; i++){

        maxiOsc panWave;
        double prePan=panWave.sinewave(0.5);
        pan=ofMap(prePan,-1,1,0,1);
        
//       ++++++++++++++
//        setup synths
//       ++++++++++
        
        double midLevel=mids[currentSynth]->play(pitch[currentSetting], speed[currentSetting]-0.8, grainLength[currentSetting], overlaps[currentSetting]);
        maxiOsc gate;
        float gateD = gate.square(0.5);
        cout << gateD << endl;
            breaths=highs[currentSynth]->play(pitch[currentSetting]-0.8, speed[currentSetting]-0.8, grainLength[currentSetting], overlaps[currentSetting]);
            words =bass[currentSynth]->play(wordPitch, wordPitch, grainLength[currentSetting], overlaps[currentSetting]+5);

//        +++++++++++
//        triggers
//        ++++++++++
//        if (delayLine) {
//            delayedWords = delayed.dl(words, 2000, 0.4);
//            eq[2]-=0.001;
//            if(eq[2]<0.1){
//                eq[2]=0;
//            }
//        }
//        ++++++++++++
//        mixing
//        +++++++++++
        mymix.stereo(delayedWords, delays, 0.5);
        mymix.stereo((midLevel*eq[1])+(breaths*eq[0])+(words*eq[2]), outputs, 0.5);
        
//        ++++++++++
//        outputs
//        +++++++++
//        check errors for volume too high - maybe try limiting here
        output[i*nChannels    ] = (outputs[0]+(delays[0]*0.5))*0.05;
        output[i*nChannels + 1] = (outputs[1]+(delays[1]*0.5))*0.05;
    }
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    // samples are "interleaved"
    for(int i = 0; i < bufferSize; i++){
        
    }
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
            case OF_KEY_UP:
                angle++;
                if(angle>30) angle=30;
                kinect.setCameraTiltAngle(angle);
                break;
                
            case OF_KEY_DOWN:
                angle--;
                if(angle<-30) angle=-30;
                kinect.setCameraTiltAngle(angle);
                break;
            case OF_KEY_LEFT:
                kinectOn=!kinectOn;
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
