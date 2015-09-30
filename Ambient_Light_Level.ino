/* 
 * 
 * Using an 8-segment display, shows different numbers
 * different manner. Rather than using 8 pins we will use just three
 *
 */

int lightPin = 0;

// Segments
int upLeftSeg = 0;    // B00000001
int midSeg = 1;       // B00000010
int lowLeftSeg = 2;   // B00000100
int botSeg = 3;       // B00001000
int lowRightSeg = 4;  // B00010000
int decimalSeg = 5;   // B00100000
int upRightSeg = 6;   // B01000000
int topSeg = 7;       // B10000000

int cleared = B00000000;

// Character definitions
int seg0[] = {6, topSeg, upRightSeg, upLeftSeg, lowLeftSeg, lowRightSeg, botSeg};
int b0 = B11011101;

int seg1[] = {2, lowRightSeg, upRightSeg};
int b1 = B01010000;

int seg2[] = {5, topSeg, upRightSeg, midSeg, lowLeftSeg, botSeg};
int b2 = B11001110;

int seg3[] = {5, topSeg, upRightSeg, midSeg, lowRightSeg, botSeg};
int b3 = B11011010;

int seg4[] = {4, upLeftSeg, midSeg, upRightSeg, lowRightSeg};
int b4 = B01010011;

int seg5[] = {5, topSeg, upLeftSeg, midSeg, lowRightSeg, botSeg};
int b5 = B10011011;

int seg6[] = {6, topSeg, upLeftSeg, midSeg, lowLeftSeg, lowRightSeg, botSeg};
int b6 = B10011111;

int seg7[] = {3, topSeg, upRightSeg, lowRightSeg};
int b7 = B11010000;

int seg8[] = {7, topSeg, upRightSeg, upLeftSeg, midSeg, lowRightSeg, lowLeftSeg, botSeg};
int b8 = B11011111;

int seg9[] = {6, topSeg, upRightSeg, upLeftSeg, midSeg, lowRightSeg, botSeg};
int b9 = B11011011;

int segDecimal[] = {1, decimalSeg};

//Pin Definitions
//Pin Definitions
//The 74HC595 uses a serial communication 
//link which has three pins
int data = 2; 
int clock = 3;
int latch = 4;

//Used for single LED manipulation
int ledState = 0;
const int ON = HIGH;
const int OFF = LOW;
                        
/*
 * setup() - this function runs once when you turn your Arduino on
 * We set the three control pins to outputs
 */
void setup()
{
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);  
  pinMode(latch, OUTPUT);  

  // Serial.begin(9600);
}

/*
 * loop() - this function will start after setup finishes and then repeat
 * we set which LEDs we want on then call a routine which sends the states to the 74HC595
 */
void loop()                     // run over and over again
{
  int delayTime = 500; //the number of milliseconds to delay between LED updates

/*  updateLEDs(b9);
  delay(delayTime);
  updateLEDs(b8);
  delay(delayTime);
  updateLEDs(b7);
  delay(delayTime);
  updateLEDs(b6);
  delay(delayTime);
  updateLEDs(b5);
  delay(delayTime);
  updateLEDs(b4);
  delay(delayTime);
  updateLEDs(b3);
  delay(delayTime);
  updateLEDs(b2);
  delay(delayTime);
  updateLEDs(b1);
  delay(delayTime);
  updateLEDs(b0); 

  clearDisplay(); */

  int lightLevel = analogRead(lightPin); //Read the
                                        // lightlevel

  // Serial.println(lightLevel);

  // RadioShack sensor is not as granular as Adafruit sensor
  lightLevel = map(lightLevel, 0, 900, 0, 9);   // use 0 to 900 for Adafruit photoresistor
  // lightLevel = map(lightLevel, 50, 250, 0, 9); // use 100 to 250 for RadioShack photoresistor
         //adjust the value 0 to 900 to

  lightLevel = constrain(lightLevel, 0, 9);//make sure the 
                                           //value is betwween 
                                           //0 and 255

  lightLevel = 9 - lightLevel;

  switch(lightLevel) {
    case 0: updateLEDs(b0); break;
    case 1: updateLEDs(b1); break;
    case 2: updateLEDs(b2); break;
    case 3: updateLEDs(b3); break;
    case 4: updateLEDs(b4); break;
    case 5: updateLEDs(b5); break;
    case 6: updateLEDs(b6); break;
    case 7: updateLEDs(b7); break;
    case 8: updateLEDs(b8); break;
    case 9: updateLEDs(b9); break;
    default: updateLEDs(B11111111);
  }
  
  delay(delayTime);
}

/*
 * setDisplay(segments[], delayTime) - displays a particular pattern
 * segments[] - array int, segments to light with the first element as the number of segments
 * delayTime - int, milliseconds to set the display before clearing
 */
void setDisplay(int segments[]) {
  for (int i = 1; i < segments[0]+1; i++) {
    changeLED(segments[i], ON);
  }
}

/*
 * clearDisplay() - clears the display
 */
void clearDisplay() {
 for (int i = 0; i < 8; i++) {
  changeLED(i, OFF);
 }
} 

/*
 * updateLEDs() - sends the LED states set in ledStates to the 74HC595
 * sequence
 */
void updateLEDs(int value){
  digitalWrite(latch, LOW);     //Pulls the chips latch low
  shiftOut(data, clock, MSBFIRST, value); //Shifts out the 8 bits to the shift register
  digitalWrite(latch, HIGH);   //Pulls the latch high displaying the data
}

/*
 * updateLEDsLong() - sends the LED states set in ledStates to the 74HC595
 * sequence. Same as updateLEDs except the shifting out is done in software
 * so you can see what is happening.
 */ 
void updateLEDsLong(int value){
  digitalWrite(latch, LOW);    //Pulls the chips latch low
  for(int i = 0; i < 8; i++){  //Will repeat 8 times (once for each bit)
  int bit = value & B10000000; //We use a "bitmask" to select only the eighth 
                               //bit in our number (the one we are addressing this time through
  value = value << 1;          //we move our number up one bit value so next time bit 7 will be
                               //bit 8 and we will do our math on it
  if(bit == 128){digitalWrite(data, HIGH);} //if bit 8 is set then set our data pin high
  else{digitalWrite(data, LOW);}            //if bit 8 is unset then set the data pin low
  digitalWrite(clock, HIGH);                //the next three lines pulse the clock pin
  delay(1);
  digitalWrite(clock, LOW);
  }
  digitalWrite(latch, HIGH);  //pulls the latch high shifting our data into being displayed
}


//These are used in the bitwise math that we use to change individual LEDs
//For more details http://en.wikipedia.org/wiki/Bitwise_operation
int bits[] = {B00000001, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000};
int masks[] = {B11111110, B11111101, B11111011, B11110111, B11101111, B11011111, B10111111, B01111111};
/*
 * changeLED(int led, int state) - changes an individual LED 
 * LEDs are 0 to 7 and state is either 0 - OFF or 1 - ON
 */
 void changeLED(int led, int state){
   ledState = ledState & masks[led];  //clears ledState of the bit we are addressing
   if(state == ON){ledState = ledState | bits[led];} //if the bit is on we will add it to ledState
   updateLEDs(ledState);              //send the new LED state to the shift register
 }
