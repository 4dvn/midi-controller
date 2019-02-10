#include <MIDIUSB.h>

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  for (int i = 48; i < 73; i++){
    noteOn(0, i, 127);
    delay(250);
    noteOff(0, i, 127);
    delay(125);
  }

}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}
