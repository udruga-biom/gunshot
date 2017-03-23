#include <SD.h>

/******************************************************************************
   sound_detector_demo.ino
   Sound detector sample sketch
   Byron Jacquot @ SparkFun Electronics
   February 19, 2014
   https://github.com/sparkfun/Sound_Detector

   This sketch demonstrates the use of the Sparkfun Sound Detector board.

   The Sound Detector is a small board that combines a microphone and some
   processing circuitry.  It provides not only an audio output, but also a
   binary indication of the presence of sound and an analog representation
   of it's amplitude.

   This sketch demonstrates two different modes of usage for the Sound
   Detector.  The gate output (a binary indication that is high when sound
   is present, and low when conditions are quiet) is used to fire a pin-change
   ISR, which lights an LED when the sound is present.  The envelope output
   (an analog voltage to rises to indicate the amplitude of the sound) is
   sampled in the loop(), and it prints an indication of the level to the
   serial terminal.

   For more details about the Sound Detector, please check the hookup guide.

   Connections:
   The Sound Detector is connected to the Adrduino as follows:
   (Sound Detector -> Arduino pin)
   GND → GND
   VCC → 5V
   Gate → Pin 2
   Envelope → A0

   Resources:
   Additional library requirements: none

   Development environment specifics:
   Using Arduino IDe 1.0.5
   Tested on Redboard, 3.3v/8MHz and 5v/16MHz ProMini hardware.

   This code is beerware; if you see me (or any other SparkFun employee) at the
   local, and you've found our code helpful, please buy us a round!

   Distributed as-is; no warranty is given.
 ******************************************************************************/

// Define hardware connections
#define PIN_GATE_IN 2
#define IRQ_GATE_IN 0
#define PIN_LED_OUT 4
#define PIN_ANALOG_IN A0

// Change to alter smoothness of output
const int numReadings = 10;
const int treshold = 15;
const int numDelay = 3;
const int eventDuration = 500;

const uint8_t BUFFER_SIZE = 25;
char fileName[] = "demoFile.txt"; // SD library only supports up to 8.3 names
char buff[BUFFER_SIZE] = "";  // Added two to allow a 2 char peek for EOF state
uint8_t index = 0;

const uint8_t chipSelect = 8;
const uint8_t cardDetect = 9;

enum states : uint8_t { NORMAL, E, EO };
uint8_t state = NORMAL;

bool alreadyBegan = false;  // SD.begin() misbehaves if not first call

int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;
unsigned long event = 0;

void setup()
{
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  Serial.begin(9600);

  //  Configure LED pin as output
  pinMode(PIN_LED_OUT, OUTPUT);

  // Display status
  Serial.println("Initialized");

  // Setup SD card
  pinMode(cardDetect, INPUT);
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop()
{
  int value;
  String valueString;

  // Check the envelope input
  value = analogRead(PIN_ANALOG_IN);
  //value = digitalRead(PIN_GATE_IN);

  // Smoother
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = value;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;

  if (value - average > treshold) {
    if (millis() - event > eventDuration) {
      event = millis();
      valueString = String(value) + " " + String(millis());
      sendToSD(valueString);
    }

  }
//  Serial.print(average);
//  Serial.print(" ");
//  Serial.println(value);

  if (millis() - event < eventDuration) {
    digitalWrite(PIN_LED_OUT, HIGH);
  } else {
    digitalWrite(PIN_LED_OUT, LOW);
  }
  
  // pause
  delay(numDelay);
}

void sendToSD(String outputString)
{
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(outputString);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

