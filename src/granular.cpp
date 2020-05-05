/* This is an example of how to integrate maximilain into openFrameworks,
 including using audio received for input and audio requested for output.
 
 
 You can copy and paste this and use it as a starting example.
 
 */

#include "ofApp.h"
//#include "maximilian.h"/* include the lib */
#include "time.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    /* This is stuff you always need.*/
    
    sender.setup(HOST, SENDPORT);
    receiver.setup(RECEIVEPORT);
    
    string path = "/data/allFiles";
//    dir.listDir("path");
    //samples
    samp.load(ofToDataPath("allFiles/TimeIsRunningOut.wav"));
    bedSample.load(ofToDataPath("allFiles/theTimeISrunningout.wav"));
    samp2.load(ofToDataPath("allFiles/i3.wav"));
    samp3.load(ofToDataPath("allFiles/u1.wav"));
    samp4.load(ofToDataPath("allFiles/emulsifying.wav"));
    samp5.load(ofToDataPath("allFiles/through.wav"));
    samp6.load(ofToDataPath("allFiles/diffusing.wav"));
//    pseudo code - for the size of the folder/directory samp.load(ofToDataPath("allFiles/
//    need to find a way to work this out as I can't seem to do for loops in Header and it seems that
//    I need to call
//    samp2.load(ofToDataPath("24620__anamorphosis__GMB_Kantilan_1.wav"));
//    samp3.load(ofToDataPath("26393__brfindla__Calango1berimbau.wav"));
//    samp4.load(ofToDataPath("68373__juskiddink__Cello_open_string_bowed.wav"));
//    samp5.load(ofToDataPath("71515__Microscopia__Wilhelm_Bruder_Sohne_Organ.wav"));
    //    samp5.load(ofToDataPath("sine1sec.wav"));
    
    
    ofEnableAlphaBlending();
    ofSetupScreen();
    ofBackground(0, 0, 0);
    ofSetFrameRate(10);
    
    sampleRate     = 44100; /* Sampling Rate */
    bufferSize    = 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */
    
    
    
    bed1 = new maxiPitchStretch<grainPlayerWin>(&samp);
    bed2 = new maxiPitchStretch<grainPlayerWin>(&bedSample);
    vocals1 = new maxiPitchStretch<grainPlayerWin>(&samp2);
    vocals2 = new maxiPitchStretch<grainPlayerWin>(&samp3);
    vocals3 = new maxiPitchStretch<grainPlayerWin>(&samp4);
    vocals4 = new maxiPitchStretch<grainPlayerWin>(&samp5);
    vocals5 = new maxiPitchStretch<grainPlayerWin>(&samp6);
    
    bed.push_back(bed1);
    bed.push_back(bed2);
    vocals.push_back(vocals1);
//    vocals.push_back(vocals2);
    vocals.push_back(vocals3);
    vocals.push_back(vocals4);
    vocals.push_back(vocals5);
    
    speed = 1;
    grainLength = 0.05;
    current=0;
    
    fft.setup(1024, 512, 256);
    oct.setup(44100, 1024, 10);
    
    int current = 0;
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    /* Anything that you would normally find/put in maximilian's setup() method needs to go here. For example, Sample loading.
     
     */
    
    ofSetSphereResolution(5);
    
    isTraining=true;
    
    ofBackground(0,0,0);
    
    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4); /* this has to happen at the end of setup - it switches on the DAC */
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(160,32,240, 150);
    ofDrawBitmapString(":: ofxMaxim Granular Timestretching Example ::", 10,20);
    ofDrawBitmapString("Move the mouse left to right to change playback speed and direction.", 10,40);
    ofDrawBitmapString("Move the mouse up and down to change the grain length.", 10,60);
    ofDrawBitmapString("Click to cycle through samples.", 10,80);
    
    stringstream s;
    s << "Speed: " << speed;
    ofDrawBitmapString(s.str(), 10,750);
    s.str("");
    s << "Grain length: " << round(grainLength*1000.0) << " ms";
    ofDrawBitmapString(s.str(), 180,750);
    
    ofNoFill();
//    for(int i=0; i < oct.nAverages; i++) {
//        ofSetColor(200 + ((int)(ofGetFrameNum() * 0.8) % 255),
//                   100 + ((int)(ofGetFrameNum() * 1.4) % 255),
//                   ofGetFrameNum() % 255,
//                   oct.averages[i] / 20.0 * 255.0);
//        glPushMatrix();
//        glTranslatef(ofGetWidth()/2,ofGetHeight()/2, 0);
//        glRotatef(0.01 * ofGetFrameNum() * speed * i, 0.01 * ofGetFrameNum() * speed * i,0.01 * ofGetFrameNum() * speed * i, 0);
//        ofDrawSphere(0, 0, i * 5);
//        glPopMatrix();
//    }
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    
    for (int i = 0; i < bufferSize; i++){
        
        beat=((int) c.phasor(8));//this oscillator is now a counter
        if (lastbeat!=beat) {

        }
        lastbeat=beat;//let's start again. It's a loop
//        bed[current]->setPosition(0.5);
//        if (ofGetFrameNum()%123==0) {
            
        if (ofRandom(10)>9.99) {
//                bed[current]->setPosition(ofRandom(0.4));
                bed1->setPosition(ofRandom(0.4));
                bed2->setPosition(ofRandom(0.4,0.9));
        }
        if (ofRandom(10)<0.001) {
//                bed[current]->setPosition(ofRandom(0.4,0.9));
                bed1->setPosition(ofRandom(0.4,0.9));
                bed2->setPosition(ofRandom(0.4));
        }
//        }
        if(ofGetFrameNum()%8==0){
            wave2=bed2->play(0.8, 0.01, 0.4, 5,0)*0.8;
        }
        if(ofGetFrameNum()%166==0){
            double prewave3=bed1->play(0.8, 0.01, 0.4, 5,0)*0.8;
            wave3 = free.play(prewave3, 3000, 0.6);
            samp5.setPosition(0);
            
        }
        if(ofGetFrameNum()%33==0){
            double preWav = bed2->play(0.7, 0.05, 0.4, 5,0)*0.9;
            wave2 = delayed.dl(preWav, 10000, 0.6);
            samp3.setPosition(0);
        }
        if(ofGetFrameNum()%54==0){
            double preWav2 = bed2->play(1.1, 0.05, 0.4, 5,0)*0.9;
            wave2 = delayed.dl(preWav2, 20000, 0.8);
            samp4.setPosition(0);
        }
        else{
            current=0;
        }
       double  preWave = bed[current]->play(0.8,0.01, grainLength=0.4, 4, 0);
        wave = myFilter.lores(preWave, ofMap(sin(ofGetElapsedTimef()), -1, 1, 300, 3000), 0.25);
        

//        if(ofGetFrameNum()>53){
//            preSamp2=samp4.playOnce();
//        }
//        double preSamp=samp3.playOnce();
//        preSamp3=samp5.playOnce();
//        double you=d2.dl(preSamp, 15000, 0.5);
//        double I=d3.dl(preSamp2, 19000, 0.5);
//        sample2=(I+you+preSamp3);
//
//        Vocals
//        attempting to play granulator then stop
        vocals1->setPosition(ofMap(a.phasor(8),-1,1,0,0.8));
        I = vocals1->play(1., 0.7, 0.4, 5);
        vocals3->setPosition(ofMap(b.phasor(24), -1, 1, 0, 0.9));
       you = vocals3->play(1.3, 1., 0.8, 5)*0.7;
        if (ofGetFrameNum()>300) {
//            you =vocals2->play(0.8,1.,0.4,5);
        }
//        wave3 = bed1->play(0.5, 0.02, 0.4, 4, 0)+0.7;
        if (fft.process(wave)) {
            oct.calculate(fft.magnitudes);
        }
        double sample=samp2.play(1*(1./16.),0,samp2.length);
        //play result
        mymix.stereo(((wave2+wave3)*0.5)+you+I, outputs, 0.125);
//        mymix.stereo(I, outputs, 0.125);
        output[i*nChannels    ] = outputs[0]; /* You may end up with lots of outputs. add them here */
        output[i*nChannels + 1] = outputs[1];
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
    if (++current > bed.size()-1) current = 0;
    
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
