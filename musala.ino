
/*
  Controlling the Qwiic MP3 Trigger with I2C Commands
  By: Nathan Seidle
  SparkFun Electronics
  Date: January 12th, 2019
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  This example plays the first track on the SD card.

  Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/15165

  Hardware Connections:
  Plug in headphones
  Make sure the SD card is in the socket and has some MP3s in the root directory
  Don't have a USB cable connected to the Qwiic MP3 Trigger right now
  If needed, attach a Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the Qwiic device onto an available Qwiic port
  Open the serial monitor at 115200 baud
*/

#include <Wire.h> //Needed for I2C to Qwiic MP3 Trigger

#include "SparkFun_Qwiic_MP3_Trigger_Arduino_Library.h" //http://librarymanager/All#SparkFun_MP3_Trigger
MP3TRIGGER mp3;

const int PIRmotion = 12; // choose the input pin (for PIR sensor)
int PIRstate = LOW; // we start, assuming no motion detected
int PIRval = 0; // variable for reading the pin status

boolean is_playing_music = false;
int time_playing_music_sec = 0;
const int music_length_sec = 241;

void setup()
{
  Serial.println("Setup");
  pinMode(PIRmotion, INPUT); // declare sensor as input
  pinMode(13, OUTPUT); // LED on breadboard
  pinMode(3, OUTPUT); // Trigger of MP3 player

  // Turn on pin for 100 msec at startup to say hi
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);


  Serial.println("Setting up software serial");
  Serial.begin(115200);

  Wire.begin();
  delay(10000);
 

  //Check to see if Qwiic MP3 is present on the bus
  if (mp3.begin() == false)
  {
    Serial.println("Qwiic MP3 failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while (1)
      ;
  }


  Serial.println("Give MP3 card time to start, 2 sec");
  delay(2000);
  mp3.setVolume(25); //Volume can be 0 (off) to 31 (max)
  mp3.stop();
  //mp3.clearInterrupts();
  digitalWrite(3, HIGH); // ?????'When trigger pin is pulled to GND, the file is played'

  // Turn on pin for 200 msec at startup to say setup is done
  //digitalWrite(13, HIGH);
  //delay(200);
  //digitalWrite(13, LOW);
  //delay(200);

  Serial.println("Setup is done");
}

void loop()
{
  if (is_playing_music)
  {
    ++time_playing_music_sec;
    Serial.print("Jag spelar musik nu den ");
    Serial.print(time_playing_music_sec);
    Serial.println(":e second");
    digitalWrite(3, LOW); // Trigger music
    digitalWrite(13, HIGH);
  }
  else
  {
    const int PIRval = digitalRead(PIRmotion); // read input value
    if (PIRval == HIGH)
    {
      Serial.println("Jag startar musik nu ...");
      is_playing_music = true;
      time_playing_music_sec = 0;
      mp3.playTrack(1);
    }
    else
    {
      digitalWrite(3, HIGH); // Do not trigger music
    }
    digitalWrite(13, LOW);
  }
  if (time_playing_music_sec > music_length_sec)
  {
    Serial.println("Jag stoppar musik nu ...");
    is_playing_music = false;
    mp3.stop();
  }
  delay(1000);
}
