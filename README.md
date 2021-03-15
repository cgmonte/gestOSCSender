*This repository is for archive purpose only.*

# gestOSCSender
An Open Sound Control (OSC) server written in C++ to connect GestureTek's GestTrack3D SDK to OSC-enabled applications.

As a proof of concept, I have developed [Gestdrums](https://github.com/cgmonte/gestDrums), an experiment in gesture-controlled drums made with Processing that uses GestTrack3D through gestOSCSender. Watch a video about Gestdrums [here](https://youtu.be/qLQKYlK8aWg).

gestOSCSender depends on [openFrameworkd](https://github.com/openframeworks/openFrameworks) for interface and OSC, [POCO C++ LIbrary](https://github.com/pocoproject/poco) for address validation and [GestTrack3DHand](http://gesturetek.com/oldhome/gestpoint/introduction.php) for MS Kinekt tracking. Many thanks to all the developers of these libraries and frameworks.

#### Example client implementation in Processing using oscP5 library

```processing
void oscEvent(OscMessage theOscMessage) {
  /* check if theOscMessage has the address pattern we are looking for. */
  if (theOscMessage.checkAddrPattern("/normalized/hand0")==true) {
    /* check if the typetag is the right one. */
    if (theOscMessage.checkTypetag("fff")) {
      /* parse theOscMessage and extract the values from the osc message arguments. */
      float firstValueX = theOscMessage.get(0).floatValue();  
      float secondValueY = theOscMessage.get(1).floatValue();
      float thirdValueZ = theOscMessage.get(2).floatValue();
      hand0x = firstValueX;
      hand0y = secondValueY;
      hand0z = thirdValueZ;
      //println("hand0 x: "+firstValueX+", y: "+secondValueY+", z: "+thirdValueZ);
      return;
    }
  } 
  if (theOscMessage.checkAddrPattern("/normalized/hand1")==true) {
    /* check if the typetag is the right one. */
    if (theOscMessage.checkTypetag("fff")) {
      /* parse theOscMessage and extract the values from the osc message arguments. */
      float firstValueX = theOscMessage.get(0).floatValue();  
      float secondValueY = theOscMessage.get(1).floatValue();
      float thirdValueZ = theOscMessage.get(2).floatValue();
      hand1x = firstValueX;
      hand1y = secondValueY;
      hand1z = thirdValueZ;
      //println("hand1 x: "+firstValueX+", y: "+secondValueY+", z: "+thirdValueZ);
      return;
    }
  } 
}
```
