#include <Wire.h> //Needed for I2C to Qwiic MP3 Trigger
#include "SparkFun_Qwiic_MP3_Trigger_Arduino_Library.h" //http://librarymanager/All#SparkFun_MP3_Trigger

MP3TRIGGER mp3;
const int pir_pin = 12; // choose the input pin (for PIR sensor)
const int music_length_sec = 241;
const int music_length_msec = 1000 * music_length_sec;

void setup()
{
  pinMode(pir_pin, INPUT); // declare sensor as input
  pinMode(13, OUTPUT); // Trigger of built-in LED
  digitalWrite(13, HIGH);
  Serial.begin(115200);
  Wire.begin();

  if (!mp3.begin())
  {
    Serial.println("Qwiic MP3 failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while (1) { ; }
  }
  mp3.setVolume(0); //Volume can be 0 (off) to 31 (max)

  // Start MP3 card
  delay(20000);
}

void loop()
{
  digitalWrite(13, LOW);
  if (digitalRead(pir_pin) == HIGH)
  {
    digitalWrite(13, HIGH);
    mp3.setVolume(1);
    mp3.playTrack(1);
    delay(music_length_msec);
    mp3.stop();
  }
  delay(100);
}