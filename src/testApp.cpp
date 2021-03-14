#include "testApp.h" //this includes the testApp header

#define DOT_PRODUCT(p1, p2) ((p1.x*p2.x) + (p1.y*p2.y) + (p1.z*p2.z))

//--------------------------------------------------------------

void testApp::setup()
{
	ofBackground(0, 82, 136);
	hostAddress = "127.0.0.1";
	portNumber = 12345;
	sender.setup(hostAddress, portNumber);
//    ipToSender = &hostAddress;
//    portToSender = &portNumber;

	//normalToggle = FALSE;
	//absoluteToggle = FALSE;
	sender.setup(hostAddress, portNumber);
	// open an outgoing connection to HOST:PORT

	panel1 = new ofxUICanvas(20,20,250,105);		//ofxUICanvas(float x, float y, float width, float height)
	panel1->addWidgetDown(new ofxUILabel("GestOSC Sender", OFX_UI_FONT_LARGE));
	ofAddListener(panel1->newGUIEvent, this, &testApp::guiEvent);
	panel1->loadSettings("GUI/guiSettings.xml");
	panel1->addWidgetDown(new ofxUIToggle(15, 15, false, "Send absolute coordinates."));
	panel1->addWidgetDown(new ofxUIToggle(15, 15, false, "Send normalized coordinates."));
	panel1->loadSettings("GUI/guiSettings.xml");


	//panel2->setWidgetFontSize(OFX_UI_FONT_LARGE);
	panel2 = new ofxUICanvas(20,145,250,170);
	ofAddListener(panel2->newGUIEvent, this, &testApp::guiEvent);
	panel2->addWidgetDown(new ofxUILabel("Input host address", OFX_UI_FONT_MEDIUM));
	panel2->addTextInput("HOST INPUT", "Input host address", 10)->setAutoClear(false);
	panel2->addWidgetDown(new ofxUILabel("Input port number", OFX_UI_FONT_MEDIUM));
	panel2->addTextInput("PORT INPUT", "Input port number", 10)->setAutoClear(false);
	panel2->loadSettings("GUI/guiSettings.xml");

}

void testApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "Send absolute coordinates.")
    {
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		absoluteToggle = toggle->getValue();
    }

	if (e.widget->getName() == "Send normalized coordinates.")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		normalToggle = toggle->getValue();
	}

	if(e.widget->getName() == "HOST INPUT")
	{
		ofxUITextInput *stringy = (ofxUITextInput *) e.widget;
        if (Poco::Net::IPAddress::tryParse(stringy->getTextString(), result) == true)
            {
                //cout << hostAddress << endl;
//                normalToggle = FALSE;
//                absoluteToggle = FALSE;
						hostAddress = stringy->getTextString();

                sender.setup(hostAddress, portNumber);
				cout << "sender was set on IP " << hostAddress << " and port is " << portNumber << "." << endl;
                panel2->saveSettings("GUI/guiSettings.xml");

            }
	}

	if(e.widget->getName() == "PORT INPUT")
	{
		ofxUITextInput *integery = (ofxUITextInput *) e.widget;
		portName = integery->getTextString();
        portNumber = atoi(portName.c_str());
		//cout << portNumber << endl;

        if(portNumber>1023 && portNumber<=65535)
        {
//            normalToggle = FALSE;
//            absoluteToggle = FALSE;
            sender.setup(hostAddress, portNumber);
            cout << "sender was set on IP " << hostAddress << " and port is " << portNumber << "." << endl;
            panel2->saveSettings("GUI/guiSettings.xml");
        }
	}
}

void testApp::ProcessTrackerData()
{
  // Get the current tracker data
  int nSize = (int)MaxHands;
  m_NumHandsDetected = GestTrack3DHand_GetData(m_HandData, &m_TrackerVolume, &nSize);

  // Update whether or not we've received our first packet of tracker data yet.
  if (m_NumHandsDetected > 0)
  {
		m_DataHasBeenDetected = TRUE;
		//uncoment this if you want to print a console warning when it finds a hand.
	    //cout << "hand detected!" << endl;
  }
		else
		{
			m_DataHasBeenDetected = FALSE;
			//uncoment this if you want to print a console warning when there's no hand.
			//cout << "no hand" << endl;
		}
}

//--------------------------------------------------------------
void testApp::update()
{

	/*creates a GestTrack3DPoint struct called vecNormalized,
	which contains three double floats (see GestTrack3DHand.h)*/
	GestTrack3DPoint vecNormalized;

	std::ostringstream address; //creates a string buffer object called address.
	ofxOscMessage message; // creates an OSC message.

    ProcessTrackerData(); //calls this function to see if there are hands detected.

	if (m_DataHasBeenDetected == TRUE)//if there's at least one hand detected...
	{
		//call the function to normalize the coordinates:
		NormalizeCoordinate((float)m_HandData[0].m_Pos_Absolute.x, (float)m_HandData[0].m_Pos_Absolute.y, (float)m_HandData[0].m_Pos_Absolute.z, m_TrackerVolume, &vecNormalized);

		/*loop this until the index reaches the total number of hands minus 1.
		the reason for subtracting 1 is that the returned number of hands starts with 1
		and the hand indexes start with hand zero*/

		if (normalToggle == TRUE)
		{
			for (int i = 0; i <= m_NumHandsDetected-1; i++)
			{
				address << "/normalized/hand" << i; //add the current index to the string "/hand" and than add it to the string buffer

				//uncoment this if you wanna see the generated strings on the console window:
				//cout << address.str() << endl;

				message.setAddress(address.str()); //use the current string to set the address of the OSC control

				/* here we add the coordinate values to the OSC messages.
				the index is used here to get the data from exact the same hand we defined in the address*/
				message.addFloatArg(m_HandData[i].m_Pos_Normalized.x); //add the first value (x) to the OSC message
				message.addFloatArg(m_HandData[i].m_Pos_Normalized.y); //add the second value (y) to the OSC message
				message.addFloatArg(m_HandData[i].m_Pos_Normalized.z); //add the third value (z) to the OSC message

				sender.sendMessage(message); //sends the OSC message

				//clears everything before going to the next loop:
				message.clear();
				address.clear();
				address.str("");
			}
		}

		if (absoluteToggle == TRUE)
		{
			for (int i = 0; i <= m_NumHandsDetected-1; i++)
			{
				address << "/absolute/hand" << i; //add the current index to the string "/hand" and than add it to the string buffer

				//uncoment this if you wanna see the generated strings on the console window:
				//cout << address.str() << endl;

				message.setAddress(address.str()); //use the current string to set the address of the OSC control

				/* here we add the coordinate values to the OSC messages.
				the index is used here to get the data from exact the same hand we defined in the address*/
				message.addFloatArg(m_HandData[i].m_Pos_Absolute.x); //add the first value (x) to the OSC message
				message.addFloatArg(m_HandData[i].m_Pos_Absolute.y); //add the second value (y) to the OSC message
				message.addFloatArg(m_HandData[i].m_Pos_Absolute.z); //add the third value (z) to the OSC message

				sender.sendMessage(message); //sends the OSC message

				//clears everything before going to the next loop:
				message.clear();
				address.clear();
				address.str("");
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	// display instructions
	//ofDrawBitmapString("This an Open Sound Control (OSC) server for the GestTrack3D software.", 10, 20);
	//string buf;
	//buf = "Sending OSC messages to " + string(HOST) + " " + ofToString(PORT)+".";
	//ofDrawBitmapString(buf, 10, 50);
}

//mathmagic to normalize the absolute coordinates:
void testApp::NormalizeCoordinate(float x, float y, float z, GestTrack3DVolume &volume, GestTrack3DPoint *pOutput)
{
  GestTrack3DPoint vecRight, vecUp, vecNormal;
  GestTrack3DPoint vecInput;

  // Calculate the 3 normalized vectors defining the space of the tracker volume
  vecRight.x = volume.m_NearPlane[1].x - volume.m_NearPlane[0].x;
  vecRight.y = volume.m_NearPlane[1].y - volume.m_NearPlane[0].y;
  vecRight.z = volume.m_NearPlane[1].z - volume.m_NearPlane[0].z;
  float vecRightLen = (float)sqrt((vecRight.x*vecRight.x) + (vecRight.y*vecRight.y) + (vecRight.z*vecRight.z));
  if (vecRightLen > 0.0f)
  {
    vecRight.x /= vecRightLen;
    vecRight.y /= vecRightLen;
    vecRight.z /= vecRightLen;
  }
  else
  {
    vecRightLen = 1.0f;
  }

  vecUp.x = volume.m_NearPlane[3].x - volume.m_NearPlane[0].x;
  vecUp.y = volume.m_NearPlane[3].y - volume.m_NearPlane[0].y;
  vecUp.z = volume.m_NearPlane[3].z - volume.m_NearPlane[0].z;
  float vecUpLen = (float)sqrt((vecUp.x*vecUp.x) + (vecUp.y*vecUp.y) + (vecUp.z*vecUp.z));
  if (vecUpLen > 0.0f)
  {
    vecUp.x /= vecUpLen;
    vecUp.y /= vecUpLen;
    vecUp.z /= vecUpLen;
  }
  else
  {
    vecUpLen = 1.0f;
  }

  vecNormal.x = volume.m_FarPlane[0].x - volume.m_NearPlane[0].x;
  vecNormal.y = volume.m_FarPlane[0].y - volume.m_NearPlane[0].y;
  vecNormal.z = volume.m_FarPlane[0].z - volume.m_NearPlane[0].z;
  float vecNormalLen = (float)sqrt((vecNormal.x*vecNormal.x) + (vecNormal.y*vecNormal.y) + (vecNormal.z*vecNormal.z));
  if (vecNormalLen > 0.0f)
  {
    vecNormal.x /= vecNormalLen;
    vecNormal.y /= vecNormalLen;
    vecNormal.z /= vecNormalLen;
  }
  else
  {
    vecNormalLen = 1.0f;
  }


  // Project the input point along the 3 vectors to calculate its normalized position
  vecInput.x = x - volume.m_NearPlane[0].x;
  vecInput.y = y - volume.m_NearPlane[0].y;
  vecInput.z = z - volume.m_NearPlane[0].z;

  pOutput->x = DOT_PRODUCT(vecInput, vecRight) / vecRightLen;
  pOutput->y = DOT_PRODUCT(vecInput, vecUp) / vecUpLen;
  pOutput->z = DOT_PRODUCT(vecInput, vecNormal) / vecNormalLen;
}

void testApp::exit()
{
	panel1->saveSettings("GUI/guiSettings.xml");
    delete panel1;
    delete panel2;
}
