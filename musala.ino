#include <Wire.h> //Needed for I2C to Qwiic MP3 Trigger
#include "SparkFun_Qwiic_MP3_Trigger_Arduino_Library.h" //http://librarymanager/All#SparkFun_MP3_Trigger

MP3TRIGGER mp3;
const int pir_pin = 12; // choose the input pin (for PIR sensor)
const int led_pin = 13; // choose the input pin (for PIR sensor)
const int boot_time_sec = 70;
const int music_length_sec = 241;
const int music_volume = 20; // From 0 (off) to (and including) 31 (loudest)

void setup()
{
  pinMode(pir_pin, INPUT); // declare sensor as input
  pinMode(led_pin, OUTPUT); // Trigger of built-in LED
  digitalWrite(led_pin, HIGH);
  Serial.begin(115200);
  Wire.begin();

  delay(2000);
  if (!mp3.begin())
  {
    while (1)
    {
      Serial.println("Qwiic MP3 failed to respond. Freezing...");
      delay(100);
    }
  }

  // Warm up MP3 card, the second argument must 
  do_countdown_with_sound(boot_time_sec);

  // Ready to play
  mp3.setVolume(music_volume);
}

void loop()
{
  digitalWrite(led_pin, LOW);
  if (digitalRead(pir_pin) == HIGH)
  {
    digitalWrite(led_pin, HIGH);
    mp3.playTrack(1);
    do_countdown_without_sound(music_length_sec);
    mp3.stop();
  }
  delay(100);
}

void do_countdown_with_sound(const int t)
{
  do_countdown(t, true);
}

void do_countdown_without_sound(const int t)
{
  do_countdown(t, false);
}

/// Blinks the built-in LED longer and longer every second
/// The sound is essential: it makes sure the MP3 player
/// is started. One should start hearing it (every second!) after 30-70 seconds
void do_countdown(const int t, const bool use_sound)
{
  for (int i = 0; i != t; ++i)
  {
    const double t_on_sec  = static_cast<double>(i + 1) / t;
    const double t_off_sec = static_cast<double>(t - 1 - i) / t;

    digitalWrite(led_pin, HIGH);
    if (use_sound)
    {
      mp3.playTrack(1);
    }
    delay(t_on_sec * 1000);
    digitalWrite(led_pin, LOW);
    if (use_sound)
    {
      mp3.stop(); 
    }
    delay(t_off_sec * 1000);
  }
  digitalWrite(led_pin, LOW);
  delay(100);
  digitalWrite(led_pin, HIGH);
  delay(100);
  digitalWrite(led_pin, LOW);
}
