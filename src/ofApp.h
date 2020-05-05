#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxMaxim.h"
#include "maxiGrains.h"
#include "maxiReverb.h"
#include "ofxOsc.h"
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
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void triggered1(double eq1, double eq2, double pitching, bool check);
    void triggered2(double eq1, double eq2, double pitching);
    
    ofxKinect kinect;
    ofxCvGrayscaleImage depthImage; //    This depth image is used to 'scan' the air
    int angle;
    
//    'Scanning' location and widths - global variables
    int farThresh,nearThresh;
    int scanStartY, scanEndY;
    int scanStartX, scanEndX;
    int scanWidthX, scanWidthY;
    int nearScan, farScan;
    int w,h;
    int depthScanWidth;
    float timer;
//    trigger for scanning - in some sections I only need the Kinect to work
//    for a short amount of time
    
    bool kinectOn,kinectOn2, check, reverse, reverse2, delayLine;
    bool triggerSample, triggerSample2;
    
//    {}[]][}{[}]][][
//    Audio
//    {}{}][]][]{}}{]
    
    void audioOut(float * output, int bufferSize, int nChannels);
    void audioIn(float * input, int bufferSize, int nChannels);
    
    int        bufferSize;
    int        initialBufferSize; /* buffer size */
    int        sampleRate;
    
//    plan is to start with 3 grainPlayers at once
//    one with the bass - pitched low and slowed down - heavily filtered
//    one with the main - mostly playing while slightly modulating
//    final with the highs - use modulo to only play every so often - add delay that changes
//    then can add other ambient breathing effects
//    the speech should be the highs - w/a delay on them - like w/religion - poptones
    
//    samples
    maxiSample samp[12];
    maxiSample bedSample[4];
    
//    PitchStretching [granular synths]
    
    maxiPitchStretch<grainPlayerWin> *gran[3], *voc[3], *breath[3]; /* change to array */
    maxiPitchStretch<grainPlayerWin> *vocals1, *vocals2, *vocals3, *vocals4, *vocals5, *vocals6;
    
//    vectors of Pitch Stretches [granular synths]
    vector<maxiPitchStretch<grainPlayerWin>*> bass;
    vector<maxiPitchStretch<grainPlayerWin>*> mids;
    vector<maxiPitchStretch<grainPlayerWin>*> highs;
//    test with layers of lows mid highs
    vector<maxiPitchStretch<grainPlayerWin>*> layer1;
    vector<maxiPitchStretch<grainPlayerWin>*> layer2;
    vector<maxiPitchStretch<grainPlayerWin>*> layer3;
    
//    granular settings
    double speed[3], grainLength[3], pitch[3], overlaps[3];
    double wordPitch;
    double pos;
    int currentSynth, currentSetting;
    
//    effects
    maxiDelayline delayed, d2,d3,d4;
    double delayedWords;
    maxiFreeVerb free;
    ofxMaxiFilter myFilter, myFilter2;
    
//    doubles
    double wave[3],outputs[2], eq[8], delays[2], breaths, words;
    double voc1,voc2,voc3,voc4;
    double preSamp2,preSamp3, you, I;
    
//    mixing
    maxiMix mymix;
    double pan;

    ofxMaxiFFT fft;
    ofxMaxiFFTOctaveAnalyzer oct;
    
    //osc
    ofxOscSender sender;
    ofxOscReceiver receiver;
    
    bool isTraining;
    
    int curXpos, curYpos;
    int prevXpos, prevYpos;
    ofDirectory directory1;
    vector<ofSoundPlayer>sounds;
    
};
