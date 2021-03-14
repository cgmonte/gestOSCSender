#include "ofMain.h" //ofMain.h includes all the core OF libraries
#include "testApp.h" //this includes the testApp header

/*includes a header with various functions prototypes 
for creating the windows with the OpenGL UtilitiesToolkit*/
#include "ofAppGlutWindow.h" 

/*includes the GestTrack3D header with all of its 3 functions. 
This headers also sets all the comunication with the DLL.*/
#include "GestTrack3DHand.h"

//========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){ //always the first thing to run, it kicks off testApp 

	ofAppGlutWindow window; //creates the window object
	ofSetupOpenGL(&window, 290, 145, OF_WINDOW);         // <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:

	GestTrack3DHand_Start(); //starts the communication with the tracker!
	ofRunApp(new testApp()); //runs testApp

}