/* Nathan Luong
 * 22/10/2019
 * Routine 7 : Snake
 * A legth 4 snake will spawn in the middle of the cube and randomly 
 move inside the 3d cube 
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
  //Set up the randomSeed for different types of random
  randomSeed(200);
}


// Initialize x and y coordinate of the snake
int x = 5;
int y = 2;
// create a direction list to store all calculation for a new coordinate
//Direction of the snake is relative to the arrow
//1 is Straight, 2 is Backward, 3 is Right, 4 is Left, 5 is Up, 6 is Down
int dirNum[6] = {1, -1, -3, 3, 1, -1};
// Save a place in memory for "curDir" which stands for current direction
int curDir;
// Initialize the previous direction of the snake as -1 which means none
// This variable will prevend the snake to run straight into its body
int preDir = -1;
// Create a 2d array to store the previous coordinate of the snake
// which is the tail of the snake
//Since the snake is in the length of 4, it have 3 leds as its tail
int preCoor[3][2] = {{0,0},{0,0},{0,0}};
// Initialize waitT as 1000
int waitT = 1000;

void loop() {
  // Create a list that store all moveable directions that a snake can go
  // Initialize all directions as false
  int moveable[6] = {false, false, false, false, false, false};
  
  //Step 1: Check for all available moves
  //Loop through the moveable array to store all valid move
  for(byte i=0; i < sizeof(moveable); i++){
    // first check the 2d valid moves of the snake(Straight, Reverse, Left and Right)
    if(i<=3){
      // See if the next move of the snake is outside a cube or not
      if(1 <= x+dirNum[i] && x+dirNum[i] <= 9){
        // If not then set that direction moveability to true
        moveable[i] = true;
        // If the snake is at position 4 or 7
        if(x == 4 || x== 7){
          // Then don't allow the snake to go on the "backward" direction
          // Cause it will re-spawn at the other end of the cube
          moveable[1]= false;
        }
        /// If the snake is at position 3 or 6
        if(x == 3 || x== 6){
          // Then don't allow the snake to go on the "straight" direction
          // Cause it will re-spawn at the other end of the cube
          moveable[0]= false;
        }
      }
    }
    // Second: Check the up and down validity of the snake
    else{
      // If the snake go up and down within the cube
      if(1 <= y+dirNum[i] && y+dirNum[i] <= 3){
        // Then set its up or down or both moveability to true
        moveable[i] = true;
      }
    }
  }

  // Print out the previous direction of the snake
  Serial.print("preDir: ");
  Serial.println(preDir);
  
  // Pick a random direction from 1 to 7
  // Create a while true loop and only break when it got a random direction
  while(true){
    // Randomly pick a direction
    curDir = random(1,7);
    // Check if that direction is moveable or not
    if (moveable[curDir-1] == true || moveable[curDir-1] == 1){
      // If true then keep checking if the direction just picked is opposite to the previous one
      if((curDir-preDir) == 1 || (curDir-preDir) == -1){
        //If all true then continue picking a random direction untill pass all of this check
         continue;
      }
      else{
        // If the random direction is all set
        //Then break out of a loop with one valid direction that the snake can move
        break;
      }
    }
  }
  // Print out the current direction afterwards
  Serial.print("curDir: ");
  Serial.println(curDir);
  
  //Move the snake to the direction that just picked
  if(1 <= curDir && curDir <= 4){
    // If the direction that just picked is responsible for moving the snake on the 2d plane
    // Then change its x coordinate
    x += dirNum[curDir-1];
  }
  else{
    //else if the direction that just picked is responsible for moving the snake up or down
    // Then change its y coordinate
    y += dirNum[curDir-1];
  }

  //Display the snake
  //Timing algorithm
  int startTime = millis();
  int endTime = startTime;
  while((endTime-startTime)<= waitT){
    // Light up the current position of the snake that just calculated from above
    single(x,y);
    // Light up the rest of the snake
    single(preCoor[0][0],preCoor[0][1]);
    single(preCoor[1][0],preCoor[1][1]);
    single(preCoor[2][0],preCoor[2][1]);
    endTime = millis();
  }

  //Save previous Direction and Coordinates of the snake
  // Shift its previous coordinate down 1 in the preCoor array of the snake
  preCoor[2][0] = preCoor[1][0]; preCoor[2][1] = preCoor[1][1];
  preCoor[1][0] = preCoor[0][0]; preCoor[1][1] = preCoor[0][1];
  preCoor[0][0] = x; preCoor[0][1] = y;
  preDir = curDir;
}
