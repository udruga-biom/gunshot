# AF3 Gunshot detector workshop - instructions

## Useful links

### Microcontroller board

[https://learn.adafruit.com/introducting-itsy-bitsy-32u4?view=all]

### Real-time clock

[https://www.velleman.eu/downloads/29/vma301_a4v01.pdf]

[https://github.com/msparks/arduino-ds1302]

## Instructions

### Installing the Arduino IDE - Blinky lights

In order to get the microcontroller board talking to your computer over USB, you will need to install some software.

 1. Download and install the Arduino software by following the instructions from the following link:

[http://www.arduino.cc/en/Main/Software]

 2. Under Preferences > Additional Boards Manager URLs enter the address [https://adafruit.github.io/arduino-board-index/package_adafruit_index.json]

 3. Under Tools > Board > Boards manager, select Type > Contributed and install the 'Adafruit AVR Boards'

 4. Under Tools > Board, you should now be able to select the board "Adafruit ItsyBitsy 32u4 5V 16 MHz"

 5. After connecting your board via USB, a new port will appear under Tools > Port. Select this port.

 6. Load the `blink` example (File > Examples > 01.Basics > Blink), and press 'Upload'. If successful, the builtin LED on your board should repeatedly blink on for one second and off for one second.

The code for the `blink` example is well documented. Try reading it and figuring out what it does. Can you change the blinking period, e.g. to 0.5 seconds ON - 2 seconds OFF?

### Serial communication - The Propaganda Machine

 1. Load the file `serial_example.ino`. Upload it to your board.

 2. Open the serial monitor (Tools > Serial Monitor) and select 9600 as the baud rate.

 3. Check the serial messages coming from the board.

 4. Consider and discuss whether ants are, in fact, important.

 5. Change the message being output by the board to reflect your conclusions from step 4. Some suggestions:
    * "I can not comment on the importance of ants"
    * "Ants are colossally, massively important"

 6. Try changing the code so that it outputs different ant-related messages in sequence, every 2 seconds.

### Connecting the microphone

 1. Connect the sound detector board as shown in the handouts.

 2. Open the file `gunshot_serial_example.ino` and upload it to your board.

 3. Open the serial monitor and make some noise. How useful is the output?

 4. Open the serial plotter and make some noise. How useful is the output?
