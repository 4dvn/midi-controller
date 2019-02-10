#include <MIDIUSB.h>

#define MIDI_CHANNEL 0
#define MIDI_CC_START 14

#define ANALOG_NUM_CHANNELS 4

//Holds current MIDI control values
int ccValue[ANALOG_NUM_CHANNELS];

//Stores pin numbers of analog channels
byte analogChannelPin[ANALOG_NUM_CHANNELS];

boolean controlChanged;

void setup(){
  pinMode(9, OUTPUT);
  
  analogChannelPin[0] = A0;
  analogChannelPin[1] = A1;
  analogChannelPin[2] = A2;
  analogChannelPin[3] = A3;
}

void loop(){
  //Loop through all analog channels
  for (byte analogChannel = 0; analogChannel < ANALOG_NUM_CHANNELS; analogChannel++){
    
    //the rest are rotary pots
    int midiValue = map(analogRead(analogChannelPin[analogChannel]), 0, 1023, 0, 127);

    if (ccValue[analogChannel] != midiValue){
      ccValue[analogChannel] = midiValue;
      controlChange(MIDI_CHANNEL, MIDI_CC_START + analogChannel, midiValue);
      controlChanged = 1;
    }
  }

  if (controlChanged) {
    digitalWrite(9, HIGH);
    controlChanged = 0;
  }
  else {
    digitalWrite(9, LOW);
  }
}
void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

