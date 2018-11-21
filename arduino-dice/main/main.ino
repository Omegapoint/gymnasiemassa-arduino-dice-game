/*
 * 
 * LIBRARY IMPORTS AND HARDWARE SETUP
 * 
*/

#include <Adafruit_NeoPixel.h>
#include <Adafruit_TiCoServo.h>
#include <Button.h>
#include "U8glib.h"

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE); 
const byte neoPin = 10;
const byte neoPixels = 24; // Declare and initialise global constant for number of pixels
byte neoBright = 1; // Declare and initialise variable for Neopixel brightness
Adafruit_NeoPixel ring = Adafruit_NeoPixel(neoPixels, neoPin, NEO_GRB);
Button btnRoll(13); // Create button object for  GPIO pin 13
Adafruit_TiCoServo servo; // Create new servo object



/*
 * 
 * MAIN GAME FUNCTIONALITY
 * 
*/

// Variables with game-related information
int currentScore = 0;
int winningScore = 18;
int currentNumberThrows = 0;
char line1[] = "Score:";

/*
 * Setup is run once in the beginning from each code upload.
*/
void setup() {

  // Initialize hardware components
  initializeHardware();

  // Move servo to position zero
  servo.write(1);

  // draw initial score
  drawScore(0);

  // Show steps for calibration
  for(int i = 1; i <= 6; i++){
    setDice(i);
     delay(1000);
  }
}

/*
 * Main game loop! This function runs over and over again throughout the game.
*/
void loop() {

  // Check for a winning score
  if (currentScore >= winningScore) {
    char messageToDraw[] = "Winner!!!!!";
    drawMessage(messageToDraw);
    winningCeremony();
    resetGame();

  // Else, check for a losing scenario
  } else if (currentNumberThrows >= 3) {
    char messageToDraw[] = "Loser!!!!!";
    drawMessage(messageToDraw);
    resetGame();
  }
  
  // Check if the button is pressed
  if(btnRoll.pressed()) {

    // Turn on the rainbow lights
    setUpRainbow();
    while(!btnRoll.released()){
      rainbowCycle();
    }
    turnOffRainbow();

    // Roll the dice
    upDown();
    int lowestTile = 1;
    int highestTile = 6;
    int thrownTile = randomNumber(lowestTile, highestTile);
    setDice(thrownTile);

    // Update the game state
    currentScore = currentScore + thrownTile;
    currentNumberThrows = currentNumberThrows +  1;
    drawScore(currentScore);
  }
  
}
