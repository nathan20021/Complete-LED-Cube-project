/* Nathan Luong
 22/10/2019
 This is a file for all 6 routines in level 3 of the led cube assignment
 */
 
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!  A diagram and an explination file are included in the folder  !!
//!!          Take a look at it first before read the rest!         !!    
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//Variable declaration
int portPins[9] = {5, 6, 7, 0, 1, 2, 3, 4, 5}; //Store all the ports pin inside portPins array
int layerPins[3] = {2, 3, 4}; //Store all layer or Negative Pins inside layerPins array
int spiralPins[9] = {1, 2, 3, 6, 9, 8, 7, 4, 5}; //store the spiral patterns for routine 1

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

// Routine 1(Spiral 1 by 1) and pass in the waitTime as an argument
void routine1(int waitT){
  //Create a loop going from 1 to 3 for the y coordinate or layer number
  for(byte i=1; i<=3; i++){
    //Loop from 0 to 8 three times for the x coordinate
    for(byte j=0; j<=8; j++){
      //run the single function that pass in the spiral pattern
      single(spiralPins[j], i);
      //delay an amount of waitT (Suggest Time: 100 to 150)
      delay(waitT);
    }
  }
}

// Routine 2(Starlight Sparkle) and pass in the waitTime as an argument
//Suggested Time: 100 to 150
void routine2(int waitT){
  // Creat startTime and set it to the current time that arduino has run
  int startTime = millis();
  // Create endTime and set it equals to startTime
  int endTime = startTime;
  //Create a loop that make sure the random lighting of the Leds only happend in 30 seconds
  while((endTime- startTime) <= 30000){
    // Light up a random Led using the single function that passed in a random x and y coordinate
    single(random(1,10), random(1,4));
    // Print out the counting up time
    Serial.println(endTime- startTime);
    // Constantly update endTime with the run time of Arduino
    endTime = millis();
    //delay for a while before going back up the loop
    delay(waitT);
  }
  //Turn all Leds off and wait for 10 seconds
  PORTB = B00000000;
  PORTD = B00000000;
  delay(10000);
}

// Routine 3(Star Burst) in waitT for the delay between the frames
// Sugessted Time: 100,150
void routine3(int waitT){
  // Light up the middle Leds and wait for 2.5 times longler than the other frames
  single(5,2);
  delay(waitT*2.5);
  
  //First frame which is the 3d cross and run it for "waitT" seconds
  //The timing algorithm for percise time and scanning technique(expination File)
  word starttime = millis(); 
  word endtime = starttime;
  while((endtime-starttime)<=waitT){
    //Turn on all of the leds for the combinations of the 3d cross frame
    single(5, 3);
    single(5, 2);single(6, 2);single(4, 2);single(8, 2);single(2, 2);
    single(5, 1);
    endtime = millis();//Keep increasing when the board keep running
  }

  //The second frame which makes the whole cube light up but the most inside one
  //The timing algorithm for percise time and scanning technique(expination File)
  starttime = millis();
  endtime = starttime;
  while((endtime-starttime)<=waitT){
    //Loop through the whole cube and light them all up
    for(byte i=1; i <=3; i++){
      for(byte j=1; j<=9; j++){
        //Check if the current Led is the middle one or not
        if(i==2 && j==5){
          //If true than ignore this one and continue with the while loop
          continue;
        }
        // light all the Leds up
        single(j,i);
      }
    }
    endtime = millis();
  }
  
  //The last frame which make the whole cube light up without the 3d Cross
  //The timing algorithm for percise time and scanning technique(expination File)
  starttime = millis();
  endtime = starttime;
  while((endtime-starttime)<=waitT){
    //Create a nested loop to loop through the whole cube
    for(byte i=1; i <=3; i++){
      for(byte j=1; j<=9; j++){
        // Check if the leds are the combination of the 3d cross or not 
        if(j==5){
          //if True then continue and not lighting up those Leds
          continue;
        }
        if(i==2 && j%2==0){
          //if True then continue and not lighting up those Leds
          continue;
        }
        //Light up the rest of the Leds that are not the combination of the 3d cross
        single(j,i);
      }
    }
    endtime = millis();
  }
  // Turn the whole cube off for a waitT amount of time
  PORTB = B00000000;
  PORTD = B00000000;
  delay(waitT);
}

//Routine 4(Up and Down) and pass waitT in as an argument
//Suggested Time: 200
void routine4(int waitT) {
  //Loop from 1 to 3
  for(byte i = 1; i<=3;i++){
    //Create a byte of portD with all the pins up high including 3 negative pins
    byte portD = B11111100;
    // Set the negative pin of counted layer to LOW to complete the circuits
    bitWrite(portD, layerPins[i-1], 0);
    // Send out the digitals signals to the pins
    PORTD = portD;
    PORTB = B00111111;
    // Delay a "waitT" amount of time
    delay(waitT);
  }
}

//Routine 5(Side to Side) and pass waitT in as an argument
//Suggested Time: 200
void routine5(int waitT) {
  // Loop from 0 to 2
  for(byte i = 0; i<=2; i++){
    //Create 2 bytes called portD and portB and set all of the Ports to LOW
    byte portD = B00000000;
    byte portB = B00000000;
    // Make the 3 pins corresponding for the layer to light up to HIGH
    bitWrite(portD, portPins[i], 1);
    bitWrite(portB, portPins[i+3], 1);
    bitWrite(portB, portPins[i+6], 1); 
    // Send out the digitals signals to the pins
    PORTD = portD;
    PORTB = portB;
    // Delay a "waitT" amount of time
    delay(waitT);
  }
}

//Routine 6(rotatingWall) and pass waitT in as an argument
//Suggested Time: 250
void routine6(int waitT){
  // Create backward counter started at 4
  byte counter = 4;
  //Loop from 1 to 4
  for(byte i=1; i<= 4; i++){
    //Create 2 bytes called portD and portB and set all of the Ports to LOW
    //This include all the negative pins so that what ever combinations of OUTPUT pin just needed 
    byte portB = B00000000;
    byte portD = B00000000;
    //Check if this is the end of the loop or not
    if(i == 4){
      //if True then
      //Loop from 0 to 2
      for(byte j=0; j<=2; j++){
        //Turn pin 0 , 1 and 2 of portB on HIGH
        bitWrite(portB, j, 1);
      }
    }
    else{
      //Make these combination for the spinning Wall pattern
      bitWrite(portD, portPins[i-1], 1);
      bitWrite(portB, portPins[i-1+counter], 1);
      bitWrite(portB, portPins[i-1+(counter*2)], 1);
    }
    // Send out the digitals signals to the pins
    PORTD = portD;
    PORTB = portB;
    // Delay a "waitT" amount of time
    delay(waitT);
    // Decrease counter by 1
    counter--;  
  }
}

void setup() {
  // Set up PD2 to PD7 as OUTPUT
  DDRD = B11111100;
  // Set up PD0 to PD5 as OUTPUT
  DDRB = B00111111;
  // Set up the Serial communication between the desktop and the chip
  // Set the default speed to 9600 bits/second
  Serial.begin(9600);
}

void loop() {
  routine6(250);
}
