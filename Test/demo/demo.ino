/*************************************************************
  This example shows how you can control a DFPlayer MP3 module
  on ESP8266 using Blynk (music player widget) and a Wifi connection

  Hardware:
    - ESP8266 based module e.g. NodeMCU
    - DFPlayer Mini MP3 player 

  App project setup:
  - Button widget attached to V4 :Play track button: OFF value=0, ON value=tracknr
  - Music Player widget attached to V5: shows current tracknr
  - Slider widget attached to V6 : Volume slider: range 0-30
  - Menu settings widget attached to V7: Equaliser selector: values
     1 Normal
     2 Pop
     3 Rock
     4 Jazz
     5 Classic
     6 Bass

   Necessary libraries
    - Blynk:   https://github.com/blynkkk/blynk-library/releases/latest
    - DFPlay:  https://github.com/rwpalmer/DFPlay

 
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

// D5 is RX of ESP8266, connect to TX of DFPlayer
// D6 is TX of ESP8266, connect to RX of DFPlayer module
SoftwareSerial dfPlaySerial(D5, D6);

#include "DFPlay.h"
DFPlay dfPlay;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "dd22a1f2e2b94d98ac723b383b62c9c6";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Hoang Trang";
char pass[] = "hoangtrang";

#define PLAY_MODE_OFF         0
#define PLAY_MODE_TRACK       1
#define PLAY_MODE_ALL         2
int playmode = PLAY_MODE_OFF;

#define DFPLAY_MEDIA_USB 1
#define DFPLAY_MEDIA_SD  2

#define VPIN_PLAYTRACK   V4
#define VPIN_MUSICPLAYER V5
#define VPIN_VOLUME      V6
#define VPIN_EQUALIZER   V7

int currentTrackCount = -1;

BLYNK_CONNECTED() {
  // Synchronize volume & equalizer of DFPlayer with Blynk widgets
  Blynk.syncVirtual(VPIN_VOLUME);
  Blynk.syncVirtual(VPIN_EQUALIZER);

  // Update music player widget (play/stop button and label) based on state DFPlayer
  if (currentTrackCount >= 0)
  {
    String trackcountString(currentTrackCount + 1); // Start with 1 instead of 0 in user interface
    Blynk.virtualWrite(VPIN_MUSICPLAYER, "play");
    Blynk.setProperty(VPIN_MUSICPLAYER, "label", trackcountString);
  }
  else
  {
    Blynk.virtualWrite(VPIN_MUSICPLAYER, "stop");
    Blynk.setProperty(VPIN_MUSICPLAYER, "label", " ");
  }
}

BLYNK_WRITE(VPIN_PLAYTRACK)
{
  int tracknr = param.asInt();
#ifdef BLYNK_PRINT
  BLYNK_PRINT.print("BLYNK_WRITE(VPIN_PLAYTRACK): ");
  BLYNK_PRINT.println(tracknr);
#endif

  if (tracknr > 0)
  {
    String trackcountString(tracknr);
    dfPlay.play(DFPLAY_MEDIA_SD, 0, tracknr); // Plays tracknr in root folder on SD-card
    playmode = PLAY_MODE_TRACK;
    currentTrackCount = -1;
    Blynk.setProperty(VPIN_MUSICPLAYER, "label", trackcountString);
    Blynk.virtualWrite(VPIN_MUSICPLAYER, "play");
  }
}


BLYNK_WRITE(VPIN_MUSICPLAYER)
{
  String action = param.asStr();
#ifdef BLYNK_PRINT
  BLYNK_PRINT.print("BLYNK_WRITE(VPIN_MUSICPLAYER): ");
  BLYNK_PRINT.println(action);
#endif

  if (action == "play") {
    dfPlay.play(DFPLAY_MEDIA_SD); // Plays all tracks in root folder of SD-card
    playmode = PLAY_MODE_ALL;
  }
  if (action == "stop") {
    dfPlay.stop();
  }
  if (action == "next") {
    dfPlay.skip();
  }
  if (action == "prev") {
    dfPlay.back();
  }
}

BLYNK_WRITE(VPIN_VOLUME)
{
  int paramVol = param.asInt();
  uint8_t volume = constrain(paramVol, 0, 30);
#ifdef BLYNK_PRINT
  BLYNK_PRINT.print("BLYNK_WRITE(VPIN_VOLUME): ");
  BLYNK_PRINT.println(paramVol);
#endif

  dfPlay.setVolume(volume);
}

BLYNK_WRITE(VPIN_EQUALIZER)
{
  int paramEq = param.asInt();
  uint8_t eq = constrain(paramEq, 1, 6) - 1; // Blynk starts with 1, setEqualizer starts with 0

#ifdef BLYNK_PRINT
  BLYNK_PRINT.print("BLYNK_WRITE(VPIN_EQUALIZER): ");
  BLYNK_PRINT.println(paramEq);
#endif

  dfPlay.setEqualizer(eq);
}

void updatePlayerState()
{
  int trackcount;

  if (dfPlay.isPlaying())
  {
    trackcount = (int)dfPlay.getTrackCount();
  }
  else
  {
    trackcount = -1;
    playmode = PLAY_MODE_OFF;
  }

//Mode play
  if (trackcount != currentTrackCount)
  {
    switch (playmode)
    {
      case PLAY_MODE_OFF:
        Blynk.setProperty(VPIN_MUSICPLAYER, "label", " ");
        Blynk.virtualWrite(VPIN_MUSICPLAYER, "stop");
        break;

      case PLAY_MODE_ALL:
        String trackcountString(trackcount + 1); // Start with 1 instead of 0 in user interface
        Blynk.setProperty(VPIN_MUSICPLAYER, "label", trackcountString);
        Blynk.virtualWrite(VPIN_MUSICPLAYER, "play");
        break;
    }

    currentTrackCount = trackcount;
  }
}

void setup()
{
  // Debug console
#ifdef BLYNK_PRINT
  BLYNK_PRINT.begin(115200);
  BLYNK_PRINT.println("Waiting for connections...");
#endif

  dfPlaySerial.begin(9600);
  dfPlay.begin(dfPlaySerial);          // Prepares DFPlay for execution

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

//Set timer
//Morning 

BLYNK_WRITE(V1)
{
  // You'll get HIGH/1 at startTime and LOW/0 at stopTime.
  // this method will be triggered every day
  // until you remove widget or stop project or
  // clean stop/start fields of widget
  int tracknr = param.asInt();
  Serial.println(param.asStr());
    if (param.asInt()){    
		    Blynk.notify(" >> Start to Music in morning. << ");
	  if (tracknr > 0) {
			String trackcountString(tracknr);
			dfPlay.play(DFPLAY_MEDIA_SD, 0, tracknr); // Plays tracknr in root folder on SD-card
			playmode = PLAY_MODE_TRACK;
			currentTrackCount = -1;
			Blynk.setProperty(VPIN_MUSICPLAYER, "label", trackcountString);
			Blynk.virtualWrite(VPIN_MUSICPLAYER, "play");
		}
	
    } else {
	  Blynk.notify(">> Finish to Music. <<");
      String trackcountString(tracknr);
      //dfPlay.play(DFPLAY_MEDIA_SD, 0, tracknr); // Plays tracknr in root folder on SD-card
      //playmode = PLAY_MODE_TRACK;
      currentTrackCount = -1;
		Blynk.setProperty(VPIN_MUSICPLAYER, "label", trackcountString);
		Blynk.virtualWrite(VPIN_MUSICPLAYER, "stop");
		
    }
}



void loop()
{
  Blynk.run();
  dfPlay.manageDevice(); // Sends requests to DFPlayer & processes responses.
  updatePlayerState();
}
