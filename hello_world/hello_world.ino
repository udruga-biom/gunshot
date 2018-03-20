#include <SD.h>

/******************************************************************************
  hello_world.ino
  2018-03-20
 ******************************************************************************/

// this part gets run only once after power on
void setup()
{
  // initialize the serial output
  // 9600 is the baud rate, i.e. speed of serial communication
  Serial.begin(9600);
}

// this part runs over and over for as long as there is power
void loop()
{
  String output = "Ants are important!";
  Serial.println(output);

  // pause for one second, i.e. 1000 milliseconds
  delay(1000);
}

