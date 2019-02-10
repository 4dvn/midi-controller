#include <frequencyToNote.h>
#include <MIDIUSB.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>
#include <Key.h>

#define NUM_ROWS 2
#define NUM_COLS 7
#define NUM_CONTROLS 1

// Row input pins
const int row1Pin = 10;
const int row2Pin = 16;

// 74HC595 pins
const int dataPin = 5;
const int latchPin = 6;
const int clockPin = 7;

boolean keyPressed[NUM_ROWS][NUM_COLS];
byte keyToMidiMap[NUM_ROWS][NUM_COLS];
byte keyChannel[NUM_ROWS][NUM_COLS];

int scale_interval[] = {2, 2, 1, 2, 2, 2, 1};

// bitmasks for scanning columns
int bits[] =
{ 
  B00000001,
  B00000010,
  B00000100,
  B00001000,
  B00010000,
  B00100000,
  B01000000,
};

int ccValue;
const int ccChannel = 8;

const int ledPin = 9;

void setup()
{
  int note = 58;  //Key
  int count = 0;  //Note number - 1

  for(int rowCtr = 0; rowCtr < NUM_ROWS; ++rowCtr)
  {
    for(int colCtr = 0; colCtr < NUM_COLS; ++colCtr)
    {
      keyPressed[rowCtr][colCtr] = false;
      keyToMidiMap[rowCtr][colCtr] = note;
      keyChannel[rowCtr][colCtr] = (count < 7) ? 2 : 1;
      
      note += scale_interval[count%7];
      count++;
      }
    }

  // setup pins output/input mode
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  pinMode(row1Pin, INPUT);
  pinMode(row2Pin, INPUT);

  pinMode(ledPin, OUTPUT);
}

void loop()
{
  for (int colCtr = 0; colCtr < NUM_COLS; ++colCtr)
  {
    //scan next column
    scanColumn(colCtr);

    //get row values at this column
    int rowValue[NUM_ROWS];
    rowValue[0] = digitalRead(row1Pin);
    rowValue[1] = digitalRead(row2Pin);

    // process keys pressed
    for(int rowCtr=0; rowCtr<NUM_ROWS; ++rowCtr)
    {
      if(rowValue[rowCtr] != 0 && !keyPressed[rowCtr][colCtr])
      {
        keyPressed[rowCtr][colCtr] = true;
        int note = keyToMidiMap[rowCtr][colCtr];
        int channel = keyChannel[rowCtr][colCtr];
        noteOn(channel, note, 100);
        digitalWrite(ledPin, HIGH);
      }
    }

    // process keys released
    for(int rowCtr=0; rowCtr<NUM_ROWS; ++rowCtr)
    {
      if(rowValue[rowCtr] == 0 && keyPressed[rowCtr][colCtr])
      {
        keyPressed[rowCtr][colCtr] = false;
        int note = keyToMidiMap[rowCtr][colCtr];
        int channel = keyChannel[rowCtr][colCtr];
        noteOff(channel, note, 100);
        digitalWrite(ledPin, LOW);
      }
    }
  }

  int currentRead = map(analogRead(A3), 0, 1023, 0, 127);

  if (ccValue != currentRead) {
    ccValue = currentRead;
    controlChange(0, ccChannel, currentRead);
  }
  
}

void scanColumn(int colNum)
{
  digitalWrite(latchPin, LOW);
  
  shiftOut(dataPin, clockPin, MSBFIRST, bits[colNum]);

  digitalWrite(latchPin, HIGH);
}
