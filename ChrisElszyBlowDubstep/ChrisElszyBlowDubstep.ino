#include <Adafruit_CircuitPlayground.h>
#include <MIDIUSB.h>

//a modified version of an adafruit project I made last semester
//a midi controller that is controlled by the sound sensor

//however changing the light sensor variable ruins the program and
//makes it unuploadable
int value = CircuitPlayground.lightSensor();
int maxValue;
int fx;

bool overdrive = false;

void setup()
{
  Serial.begin(9600);

  setValue(value);

  CircuitPlayground.begin();

  overdrive = false;
}

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).
void noteOn(byte channel, byte pitch, byte velocity)
{
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) 
{
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

// Define MIDI function "controlChange"
void controlChange(byte channel, byte control, byte value)
{
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

//calibrate the max value for the light sensor
void setValue(int a)
{
  maxValue = a;
  Serial.print("a new value was set!");
}

//basically the update function
void loop() 
{
  //if we are in overdrive we will multiply the value by 2
  //else we just take the value as is
  if(overdrive)
  {
    value = CircuitPlayground.soundSensor() * 2;
  }
  else
  {
    value = CircuitPlayground.soundSensor();
  }

  //for testing purposes, reading the sensor values
  Serial.print("the max value is");
  Serial.println(maxValue);
  
  Serial.print("Sound Sensor: ");
  Serial.println(value);

  int MIDI_light = map(value, 1, 25, 64, 127);

  controlChange(0, 10, MIDI_light);
  MidiUSB.flush();

    //sets the max value incase light changes again
  if(CircuitPlayground.rightButton() == true)
  {
    setValue(value);
  }

  //puts the sensor into overdrive and makes it more sensitive
  if(CircuitPlayground.leftButton() == true)
  {
    toggleOverdrive();
  }

  delay(50);
}

//this function is called to set if the overdrive is on or not.
//also turns on all neo pixels to a different color
void toggleOverdrive()
{
  if(overdrive == false)
  {
    overdrive = true;
    Serial.println("Overdrive is on!");
    CircuitPlayground.setPixelColor(0, 0, 0, 1);
    CircuitPlayground.setPixelColor(1, 0, 0, 1);
    CircuitPlayground.setPixelColor(2, 0, 0, 1);
    CircuitPlayground.setPixelColor(3, 0, 0, 1);
    CircuitPlayground.setPixelColor(4, 0, 0, 1);
    CircuitPlayground.setPixelColor(5, 0, 0, 1);
    CircuitPlayground.setPixelColor(6, 0, 0, 1);
    CircuitPlayground.setPixelColor(7, 0, 0, 1);
    CircuitPlayground.setPixelColor(8, 0, 0, 1);
    CircuitPlayground.setPixelColor(9, 0, 0, 1);
    CircuitPlayground.setBrightness(255);
  }
  else
  {
    overdrive = false;
    Serial.println("Overdrive is off!");
    CircuitPlayground.setPixelColor(0, 1, 0, 0);
    CircuitPlayground.setPixelColor(1, 1, 0, 0);
    CircuitPlayground.setPixelColor(2, 1, 0, 0);
    CircuitPlayground.setPixelColor(3, 1, 0, 0);
    CircuitPlayground.setPixelColor(4, 1, 0, 0);
    CircuitPlayground.setPixelColor(5, 1, 0, 0);
    CircuitPlayground.setPixelColor(6, 1, 0, 0);
    CircuitPlayground.setPixelColor(7, 1, 0, 0);
    CircuitPlayground.setPixelColor(8, 1, 0, 0);
    CircuitPlayground.setPixelColor(9, 1, 0, 0);
    CircuitPlayground.setBrightness(255);
  }

  delay(300);
}
