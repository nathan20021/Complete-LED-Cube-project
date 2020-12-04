/*Nathan Luong
 * 22/10/2019
 * Routine 8: Counter Rotating level
 * Each layer will have a snake of length 3
   the snakes will start in the middle and then spiral reversely.
   After doing their spiral pattern, they go around forever
*/

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!  A diagram and an explination file are included in the folder  !!
//!!          Take a look at it first before read the rest!         !!    
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// 2 list needed for the single function to work
int portPins[9] = {5, 6, 7, 0, 1, 2, 3, 4, 5}; //Store all the ports pin inside portPins array
int layerPins[3] = {2, 3, 4}; //Store all layer or Negative Pins inside layerPins array

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


void setup() {
  // Set up PD2 to PD7 as OUTPUT
  DDRD = B11111100;
  // Set up PD0 to PD5 as OUTPUT
  DDRB = B00111111;
  // Set up the Serial communication between the desktop and the chip
  // Set the default speed to 9600 bits/second
  Serial.begin(9600);
}


//Direction is relative to the arrow
//1 is Straight, 2 is Backward, 3 is Right, 4 is Left
int dirNum[4] = {1, -1, -3, 3};

//Since the first and third layer are moving ont the same direction, Use the same x coordinate
//Initialize x-coordiante of layer 1 and 3 as 5(middle led)
int xL1a3 = 5;
// Save a space for the currentDir of the snake on layer 1 and 3
int curDirL1a3;
// Create a list to store all the previous x-coordinate of layer 1 and 3 snake
int preCoorL1a3[2] = {0, 0};
// A list of pre-set spiral directions for the layer 1 and 3 snake to follow
int L1a3Dir[8] = {1, 3, 2, 2, 4, 4, 1, 1};
// A list of pre-set around pattern for the layer 1 and 3 snake to follow after finish spiraling 
int aroundL1a3[8] = {3, 3, 2, 2, 4, 4, 1, 1};

//Initialize x-coordiante of the second layer snake as 5(middle led)
int xL2 = 5;
// Save a space for the currentDir of the snake on the second layer
int curDirL2;
// Create a list to store all the previous x-coordinate of layer 2 snake
int preCoorL2[2] = {0,0};
// A list of pre-set spiral directions for the layer 2 snake to follow
int L2Dir[8] = {2, 4, 1, 1, 3, 3, 2, 2};
// A list of pre-set around pattern for the layer 2 snake to follow after finish spiraling 
int aroundL2[8] = {4, 4, 1, 1, 3, 3, 2, 2};

// initialize waitT as 300 to store the waiting time between multiple frame
int waitT = 300;

// Initialize 2 booleans of start and spiral as true
bool start = 1;
bool spiral = 1;
// Initialize counter =0 as the index of the spining around lists 
byte counter = 0;

void loop() {
  // Check if spiral is True or not
  if(spiral == 1){
    // If True then run the spiral code once only
    //Check if start is true or not
    if(start == 1){
      //if true then run the start code once only
      //The codes inside are just for displaying the spawn of the snakes which means 3 mid leds
      PORTD = B00000000;
      PORTB = B00000010;
      //Delay it for an amount of time
      delay(waitT);
      //set start to false so that the code can be run once only
      start = 0;
    }
    //Loop from 0 to 7
    for(byte i = 0; i<= 7; i++){
      // Set the directions of the snakes to the pre-set array of directions
      curDirL1a3 = L1a3Dir[i];
      curDirL2 = L2Dir[i];
      // Save the coordinates of the snakes and shift its last position to be its tail
      preCoorL2[1] = preCoorL2[0]; preCoorL1a3[1] = preCoorL1a3[0];
      preCoorL2[0] = xL2; preCoorL1a3[0] = xL1a3;
      //From the picked positions, caculate the next x coordinate of the snakes
      xL2 += dirNum[curDirL2-1];
      xL1a3 += dirNum[curDirL1a3-1];
      //The timing algorithms
      int startTime = millis();
      int endTime = startTime;
      while((endTime-startTime) <= waitT){
        //Display the snakes and its tail
        single(xL1a3, 1); single(preCoorL1a3[0], 1); single(preCoorL1a3[1], 1);
        single(xL2, 2); single(preCoorL2[0], 2); single(preCoorL2[1], 2);
        single(xL1a3, 3); single(preCoorL1a3[0], 3); single(preCoorL1a3[1], 3);
        endTime = millis();
      }
    }
  }
  //Set spiral to false so that the snake wont spiraling again
  spiral = 0;

  //The snake will be going around the cube for the rest
  //Set the directions of the snake to the follow pre-set spining directions
  curDirL1a3 = aroundL1a3[counter];
  curDirL2 = aroundL2[counter];
  // Save the previous coordinate of the snake and shifted down the list as its tail
  preCoorL2[1] = preCoorL2[0]; preCoorL1a3[1] = preCoorL1a3[0];
  preCoorL2[0] = xL2; preCoorL1a3[0] = xL1a3;
  // Caculate the x-coordinate of the snake due to the pre-set directions
  xL2 += dirNum[curDirL2-1];
  xL1a3 += dirNum[curDirL1a3-1];

  //Timing algorithm  
  int startTime = millis();
  int endTime = startTime;
  while((endTime-startTime) <= waitT){
    // Display the snakes and its tail
    single(xL1a3, 1); single(preCoorL1a3[0], 1); single(preCoorL1a3[1], 1);
    single(xL2, 2); single(preCoorL2[0], 2); single(preCoorL2[1], 2);
    single(xL1a3, 3); single(preCoorL1a3[0], 3); single(preCoorL1a3[1], 3);
    endTime = millis();
  }

  //Increase counter by 1 each time the loop run
  counter++;
  //Check if counter is 8
  if(counter == 8){
    // If true then reset it back to 0
    counter = 0;
    }
}
