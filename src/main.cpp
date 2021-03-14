#include "testApp.h" //this includes the testApp header

/*includes a header with various functions prototypes
for creating the windows with the OpenGL UtilitiesToolkit*/
#include "ofAppGlutWindow.h"

//========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){ //always the first thing to run, it kicks off testApp

	ofAppGlutWindow window; //creates the window object
	ofSetupOpenGL(&window, 290, 330, OF_WINDOW);         // <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:

	GestTrack3DHand_Start(); //starts the communication with the tracker!
	ofRunApp(new testApp()); //runs testApp
}
