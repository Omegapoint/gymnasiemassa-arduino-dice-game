char nr[4];

/*
 * 
 * ASSISTING FUNCTIONS
 * 
*/

void upDown() {
  // Move servo up and down
  setDice(6);
  delay(600);
  setDice(1);
  delay(600);
}

void setDice(int number){
  // Map scale
  int dicePointer = map(number, 1, 6, 10, 170); 

  // Move servo
  servo.write(dicePointer);
}

void rainbowCycle() {
  uint16_t i, j;

  for(j = 0; j < 256; j++) {
    for(i = 0; i < neoPixels; i++) {
      ring.setPixelColor(i, wheel(((i * 256 / neoPixels) + j) & 255));
    }
    ring.show();
  }
}

uint32_t wheel(byte wheelPos) {
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85) {
    return ring.Color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if(wheelPos < 170) {
    wheelPos -= 85;
    return ring.Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return ring.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

int randomNumber(int from, int to) {
  return random(from, to + 1);
}

void drawScore(int value) {
  oled.firstPage();  
  do {
    draw(value);
  } while(oled.nextPage());
}

void draw(int value) {
  
  // Set font to Helvetica size 10
  oled.setFont(u8g_font_helvB10);
  
  // Calculate line height
  int lineHeight = oled.getFontAscent()-oled.getFontDescent();  

  // Calculate starting position
  int line1x = (oled.getWidth() - oled.getStrWidth(line1)) / 2;
  int line2x = (oled.getWidth() - oled.getStrWidth(nr)) / 2;

  // calculate new value of finalVal
  sprintf (nr, "%02i", value);
  // Print text lines
  oled.drawStr(line1x, lineHeight * 2, line1);
  oled.drawStr(line2x, lineHeight * 3, nr);
}


void drawMessageHelper(char message[]) {
  
  // Set font to Helvetica size 10
  oled.setFont(u8g_font_helvB14);
  
  // Calculate line height
  int lineHeight = oled.getFontAscent()-oled.getFontDescent();  

  // Calculate starting position
  int line1x = (oled.getWidth() - oled.getStrWidth(message)) / 2;

  // Print text lines
  oled.drawStr(line1x, lineHeight * 2, message);
}

void drawMessage(char message[]) {
  oled.firstPage();  
  do {
    drawMessageHelper(message);
  } while(oled.nextPage());
}

void winningCeremony() {
  
  // Turn up brightness on Neopixel
  ring.setBrightness(neoBright);   
  ring.show(); 

  int counter = 0;
  while (counter < 10) {
    rainbowCycle();
    counter += 1;
  }

  // Turn down brightness Neopixel
  for(int i = neoBright; i >= 0; i--){
    ring.setBrightness(i);
    ring.show();
    delay(1);
  }
}

void resetGame() {
  currentScore = 0;
  currentNumberThrows = 0;
  delay(1000);
  drawScore(0);
  servo.write(1);
}

void setUpRainbow() {
  // Turn up brightness on Neopixel
  ring.setBrightness(neoBright);   
  ring.show(); 
}

void turnOffRainbow() {
  // Turn down brightness Neopixel
  for(int i = neoBright; i >= 0; i--){
    ring.setBrightness(i);
    ring.show();
    delay(1);
  }
}

void initializeHardware() {
  // Initialise the ring and turn off pixels
  ring.begin();
  ring.show();

  // Start button
  btnRoll.begin();
  
  // Attach the servo on pin 9 to the servo object
  servo.attach(9);

  // Initialise a pseudo-random number generator
  randomSeed(analogRead(0)); 
}
