#pragma once

#include "ofMain.h"
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
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    void audioOut(float * output, int bufferSize, int nChannels);
    void audioIn(float * input, int bufferSize, int nChannels);
    
    int        bufferSize;
    
    
    int        initialBufferSize; /* buffer size */
    int        sampleRate;
    
    
    /* stick your maximilian stuff below */
    
    double wave,wave2,wave3,sample,outputs[2];
    double voc1,voc2,voc3,voc4;
    double preSamp2,preSamp3, you, I;
    double sample2;
    maxiSample samp, samp2, samp3, samp4, samp5, samp6, samp7, samp8, samp9;
    maxiSample bedSample;
    vector<maxiPitchStretch<grainPlayerWin>*> bed;
    vector<maxiPitchStretch<grainPlayerWin>*> vocals;
    maxiMix mymix;
    maxiPitchStretch<grainPlayerWin> *bed1, *bed2, *bed3, *bed4, *bed5, *bed6;
    maxiPitchStretch<grainPlayerWin> *vocals1, *vocals2, *vocals3, *vocals4, *vocals5, *vocals6;
    double speed, grainLength, pitch;
    maxiDelayline delayed, d2,d3,d4;
    maxiFreeVerb free;
    ofxMaxiFFT fft;
    ofxMaxiFFTOctaveAnalyzer oct;
    int current;
    double pos;
    
    //osc
    ofxOscSender sender;
    ofxOscReceiver receiver;
    
    ofxMaxiFilter myFilter, myFilter2;
    
    bool isTraining;
    
    int curXpos, curYpos;
    int prevXpos, prevYpos;
    ofDirectory directory1;
    vector<ofSoundPlayer>sounds;
    
//    sequencing
    int morebeats, beat, lastbeat;
    maxiOsc a,b,c,d,e,g,h,i,j,squarewave;
    int rhythm1[16]={1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,0};//another way of doing a rhythm
};
