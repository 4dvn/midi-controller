#include <MIDIUSB.h>

#define EXIT_SUCCESS 1
#define EXIT_FAILURE 0

#define numberRows 2
#define numberColumns 7

#define rowPin1 5
#define rowPin2 13

#define dataPin 2
#define clockPin 3
#define latchPin 4

bool keyPressed[numberColumns][numberRows];
int keyState[numberColumns][numberRows];
int keyNote[numberColumns][numberRows];

int scaleInterval[7] = {2, 2, 1, 2, 2, 2, 1};

void setup() {
  //C4 = 60
  assignNote = 60;

  for (int row = 0; row < numberRows; row++){
    for (int column = 0; column < numberColumns; column++){
      keyPressed[row][col] = false;
      keyState[row][col] = LOW;

      keyNote[row][col] = assignNote;

      assignNote += scaleInterval[col];
    }
  }
  
}

void loop() {
  for (int columnSelect = 0; columnSelect < 7; columnSelect++){
    scanColumn(columnSelect);
    int rowRead1 = digitalRead(rowPin1);
    int rowRead2 = digitalRead(rowPin2);

    if (rowRead1 == HIGH && !keyPressed[row][col]) {
      keyPressed[row][col] = 
    }
  }
}

int scanColumn(int col) {
  shiftBits = 1 << col;
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin,clockPin, MSBFIRST, shiftBits);
  digitalWrite(latchPin, HIGH);

  return EXIT_SUCCESS;
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
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

