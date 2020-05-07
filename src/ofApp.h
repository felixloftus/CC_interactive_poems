#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxMaxim.h"
#include "maxiGrains.h"
#include "maxiReverb.h"
#include <sys/time.h>

#define HOST "localhost"
#define RECEIVEPORT 12000
#define SENDPORT 6448

typedef hannWinFunctor grainPlayerWin;


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);
    void keyReleased(int key);
    void triggered1( bool check);
    void triggered2();
    void triggered3( bool check2);
    void triggered4();
    void kinectOnFun();
    void kinectOn2Fun();
    void kinectOn3Fun();
    void kinectOn4Fun();

    
    ofxKinect kinect;
    
    ofxCvGrayscaleImage depthImage; //    This depth image is used to 'scan' the air
    ofxCvGrayscaleImage grayImage; /*these are for background subtraction*/
    ofxCvGrayscaleImage blurred;
    ofxCvGrayscaleImage background;
    ofxCvGrayscaleImage diff;
    ofxCv::ContourFinder contourFinderCV;
    ofPoint blob1;
    ofVec2f stationaryCheck;
    int sC_Counter;
    bool sampleTrigger;
    double area;
    bool showLabels;
    int angle;
    int timer;
    
//    'Scanning' location and widths - global variables
    int farThresh,nearThresh;
    int scanStartY, scanEndY;
    int scanStartX, scanEndX;
    int scanWidthX, scanWidthY;
    int altScanWidthX;
    int altScanEndX, altScanEndY;
    int altScanStartX, altScanStartY;
    int nearScan, farScan;
    int w,h;
    int depthScanWidth;
    float timerS;
//    trigger for scanning - in some sections I only need the Kinect to work
//    for a short amount of time
    
    bool kinectOn,kinectOn2, kinectOn3, kinectOn4, check, check2, reverse, reverse2, delayLine;
    bool reverse3, reverse4;
    bool triggerSample, triggerSample2, triggerSample3, triggerSample4;
    
//    {}[]][}{[}]][][
//    Core Audio setup
//    {}{}][]][]{}}{]
    
    void audioOut(float * output, int bufferSize, int nChannels);
    void audioIn(float * input, int bufferSize, int nChannels);
    
    int        bufferSize;
    int        initialBufferSize; /* buffer size */
    int        sampleRate;
    
//    samples
    maxiSample samp[12];
    
//    PitchStretching [granular synths]
//    Often these are just called like samples but having them in a
//    pitch stretch object provides more flexibility on modulation
    maxiPitchStretch<grainPlayerWin> *gran[3], *voc[3], *breath[3], *synths[3]; /* change to array */
    
//    vectors of Pitch Stretches [granular synths]
    vector<maxiPitchStretch<grainPlayerWin>*> words_Vec;
    vector<maxiPitchStretch<grainPlayerWin>*> mids;
    vector<maxiPitchStretch<grainPlayerWin>*> highs;
    vector<maxiPitchStretch<grainPlayerWin>*> layer1;
    vector<maxiPitchStretch<grainPlayerWin>*> synthsV;
    
//    granular settings
    double speed[3], grainLength[3], pitch[3], overlaps[3];
    double wordPitch;
    double pos;
    int currentSynth, currentSetting;
    
//    effects
    maxiDelayline delayed, delayed2;
    double delayedWords;
    maxiFreeVerb free;
    ofxMaxiFilter myFilter, myFilter2;
    maxiEnv envTest;
    maxiOsc osc[4];
    
//    doubles
    double wave[5],outputs[2], altOutputs[2], sampleOutputs[2];
    double delays[2], bassTones[2], breaths, words, midLevel, delayedBreaths;
    double bassFreq, samplePlay;
    
//    mixing
    maxiMix mymix;
    double LFO;
    double wordsEQ, midsEQ, breathEQ, bassEQ;
};
