/*
  *Nathan Luong
  *22/10/2019
  *Routine 9: Persistance of Vision
  *At a 45 degree angle, the LED CUBE will display 1 single digit from 1 to 9
  *Press the button once to increase the digit by 1 and it will reset it self back to 1 when it hit 10
*/

//Variable declaration
int portPins[9] = {5, 6, 7, 0, 1, 2, 3, 4, 5}; //Store all the ports pin inside portPins array
int layerPins[3] = {2, 3, 4}; //Store all layer or Negative Pins inside layerPins array
int spiralPins[9] = {1, 2, 3, 6, 9, 8, 7, 4, 5}; //store the spiral patterns for routine 1
//Define button as analog pin 0
#define button A0

//Create a function that can light up an LED at a given (x,y) coordinate\


void single(int pin, byte layer){
  //Create 2 bytes of binary called "portD" and "portB"
  //Preset the value of the 3 negative pin to 1 (PD2, PD3, PD4)
  byte portD = B00011100;
  byte portB = B00000000;
  // If pin or the x coordinate is from 1 to 3 then:
  if(pin <= 3){
    // Make the bit of portD at position pin-1 of portPins to a 1
    // This will give power to the led at the given x coordinate
    bitWrite(portD, portPins[pin-1], 1);
  }
  // Else
  else if(pin > 3){
    //Make the bit of portB at position pin-1 of portPins to a 1
    // This will give power to the led at the given x coordinate
    bitWrite(portB, portPins[pin-1], 1);
  }
  // Set the negative pin of the layer to low to make a complete circuit while the rest of the layer remain HIGH
  bitWrite(portD, layerPins[layer-1], 0);
  //Send the digital signals to the pins
  PORTD = portD;
  PORTB = portB;
}

void setup(){
  // Set up PD2 to PD7 as OUTPUT
  DDRD = B11111100;
  // Set up PD0 to PD5 as OUTPUT
  DDRB = B00111111;
  // Set up the Serial communication between the desktop and the chip
  // Set the default speed to 9600 bits/second
  Serial.begin(9600);
  //Set up 
  pinMode(button, INPUT);  
}

// Initialize counter as 1 to keep track of what number we are counting
int counter = 1;

void loop(){
  //If the button is pressed  
  if(digitalRead(button) == 1){
    //Then increase counter by one
    counter++;
    // Turn all LEDs off and stop receiving signal for 500 milliseconds
    PORTB = B00000000;
    PORTD = B00000000;
    delay(500);
  }

  //Comparing counter to a value from 1 to 10
  switch(counter){
    case 1:
      //Use the scanning method to make a two appear with this combination of Leds
      single(1,3); single(3,3); single(4,3); single(5,3); single(7,3);
      single(3,2);
      single(3,1);
      break;
    case 2:
    //Use the scanning method to make a two appear with this combination of Leds
      single(3, 3); single(4, 3); single(6, 3); single(7, 3); single(8, 3); 
      single(2, 2); single(5, 2);
      single(2, 1); single(3, 1); single(6, 1); single(7, 1); 
      break;
    case 3:
    //Use the scanning method to make a three appear with this combination of Leds
      single(3, 3);single(4, 3); single(6, 3); single(7, 3); single(8, 3); 
      single(5, 2); single(6, 2);
      single(2, 1); single(3, 1); single(6, 1); single(7, 1); 
      break;
    case 4:
    //Use the scanning method to make a four appear with this combination of Leds
      single(1,3); single(2,3); single(3,3); single(4,3); single(5,3); single(6,3);single(7,3); single(9,3); 
      single(3,2);
      single(3,1);
      break;
    case 5:
    //Use the scanning method to make a five appear with this combination of Leds
      single(2, 3); single(3, 3); single(4, 3); single(7, 3); single(8, 3);  
      single(4, 2); single(5, 2); single(6, 2);  single(8, 2); 
      single(2, 1); single(3, 1); single(6, 1); single(7, 1);
      break;
    case 6:
    //Use the scanning method to make a six appear with this combination of Leds
      single(2, 3); single(3, 3); single(4, 3); single(7, 3); single(8, 3);  
      single(2, 2); single(4, 2); single(5, 2); single(6, 2); single(8, 2); 
      single(2, 1); single(3, 1); single(6, 1); single(7, 1);
      break;
    case 7:
    //Use the scanning method to make a seven appear with this combination of Leds
      single(4, 3); single(8, 3); single(9, 3);
      single(6, 2);
      single(3, 1);
      break;
    case 8:
    //Use the scanning method to make a eight appear with this combination of Leds
      single(2, 3); single(3, 3); single(4, 3); single(6, 3); single(7, 3); single(8, 3);  
      single(2, 2); single(4, 2); single(5, 2); single(6, 2); single(8, 2); 
      single(2, 1); single(3, 1); single(6, 1); single(7, 1); 
      break;
    case 9:
    //Use the scanning method to make a nine appear with this combination of Leds
      single(2, 3); single(3, 3); single(4, 3); single(6, 3); single(7, 3); single(8, 3); 
      single(4, 2); single(5, 2); single(6, 2); single(8, 2); 
      single(2, 1); single(3, 1); single(6, 1); single(7, 1);
      break;
    case 10:
    // if counter is 10 then reset it back to 1
      counter = 1;
      break;
  }
}
