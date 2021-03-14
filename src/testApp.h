#pragma once //when compiling, iclude this header file only once

#include "ofMain.h" //ofMain.h includes all the core OF libraries
#include "ofxOsc.h" //includes the OF OSC add-on
#include "ofxUI.h"

#define HOST "localhost" //defines to where we're gonna send the OSC messages
#define PORT 12345 //defines the port through where we're gonna send the OSC messages

/*includes the GestTrack3D header with all of its 3 functions. 
This headers also sets all the comunication with the DLL.*/
#include "GestTrack3DHand.h"

/*every OF application is a class extension of the ofBaseApp.
in this this case, we are stating that everything that is public in the ofBaseApp class
will be public in it's extension, called testApp*/
class testApp : public ofBaseApp { 

	public: //the following class members are accessible from any function:

		
		void exit(); 
		void guiEvent(ofxUIEventArgs &e);
		ofxUICanvas *panel1;
		ofxUICanvas *panel2;

		enum CoordinateMode { CoordMode_Normalized=0, CoordMode_Absolute=1 };
		enum { MaxHands = 32 };
		
		GestTrack3DData   m_HandData[(int)MaxHands]; //creates m_HandData, which groups all the tracked data
		
		/*creates m_TrackerVolume, which groups the coordinates of the far and near planes that
		define the VOI (volume of interest)*/		
		GestTrack3DVolume m_TrackerVolume;

		int     m_NumHandsDetected; //integer to store the number of detected hands
		BOOL    m_DataHasBeenDetected; //boolean that will go true whenever there's at least one hand detected
		BOOL	absoluteToggle;
		BOOL	normalToggle;
		//string	hostAddress;
		//int portNumber;



		//prototype of the function that normalizes the absolute coordinates:
		void NormalizeCoordinate(float x, float y, float z, GestTrack3DVolume &volume, GestTrack3DPoint *pOutput);
		
		//prototype of the function that will figure out if the tracker is tracking any hand or not.
		void ProcessTrackerData(); 

		//these are basic OF functions:
		void setup();
		void update();
		void draw();

		ofTrueTypeFont font;
		ofxOscSender sender; //creates the sender object that will send the OSC messages.

};