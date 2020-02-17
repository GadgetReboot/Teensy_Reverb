/* Teensy 3.6 Audio Freeverb Demo
   Takes audio from I2S Audio Shield
   Playback is on I2S Audio Shield left channel

   Gadget Reboot
*/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

AudioInputI2S            i2s1;
AudioEffectFreeverb      freeverb1;
AudioMixer4              mixer1;
AudioOutputI2S           i2s2;
AudioControlSGTL5000     sgtl5000_1;
AudioConnection          patchCord1(i2s1, 0, freeverb1, 0);
AudioConnection          patchCord2(i2s1, 0, mixer1, 2);
AudioConnection          patchCord3(freeverb1, 0, mixer1, 1);
AudioConnection          patchCord4(mixer1, 0, i2s2, 0);

// inputs
#define pot1            A12  // freeverb roomsize
#define pot2            A13  // freeverb damping
#define pot3            A14  // dry level

void setup() {
  Serial.begin(9600);

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);

  // I2S audio init
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);

  AudioInterrupts();
}

void loop() {

  // read pot levels
  float knob1 = (float)analogRead(pot1) / 1023.0;  // room size
  float knob2 = (float)analogRead(pot2) / 1023.0;  // damping
  float knob3 = (float)analogRead(pot3) / 1023.0;  // dry level

  mixer1.gain(0, 0);            // unused, gain 0
  mixer1.gain(1, 0.5);          // freeverb (wet) gain 
  mixer1.gain(2, knob3);        // dry gain 
  mixer1.gain(3, 0);            // unused, gain 0

  freeverb1.roomsize(knob1);
  freeverb1.damping(knob2);

  Serial.print("Room Size: ");
  Serial.print(knob1 * 100.0);
  Serial.print("%, Damping: ");
  Serial.print(knob2 * 100.0);
  Serial.print("%, Dry Level: ");
  Serial.print(knob3 * 100.0);
  Serial.print("% / ");

  // determine how much actual memory is needed for the audio processes
  Serial.print("Max audio blocks used: ");
  Serial.println(AudioMemoryUsageMax());
  Serial.println();
  delay(100);

}
