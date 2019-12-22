#include "LedControl.h"
#include<LiquidCrystal.h>
#include <EEPROM.h>

  const int rsPin = 8;
  const int ePin = 9;
  const int d4Pin = 5;
  const int d5Pin = 4;
  const int d6Pin = 3;
  const int d7Pin = 2;
  const int V0_PIN = 6;
  
  const int brightness = 1;
  const int dinPin = 12;
  const int clkPin = 11;
  const int loadPin = 10;
  const int noOfDrivers = 1;
  const int pinX = A0;
  const int pinY = A1;
  const int pinSW = A2;

  LedControl lc = LedControl(dinPin, clkPin, loadPin, noOfDrivers);
  LiquidCrystal lcd(rsPin,ePin, d4Pin, d5Pin, d6Pin, d7Pin);

  bool matrix[8][8]; 
    
  int xValue = 0;
  int yValue = 0;
  int minThreshold = 100;
  int maxThreshold = 900;
  bool joyMovedx = false;
  bool joyMovedy = false;
  bool joyMoved = false;

  int positionRow = 0;
  int lastPositionRow = 0;
  int positionCol = 0;
  int lastPositionCol = 0;

  bool gameOver = false;

  //array for smily face
  int arraySmilyX[16] = {1, 1, 1, 1, 2, 2, 2, 2, 4, 5, 6, 6, 6, 6, 5, 4};
  int arraySmilyY[16] = {1, 2, 6, 5, 1, 2, 6, 5, 0, 1, 2, 3, 4, 5, 6, 7};
  int numberSmily = 16;

  //array fost sad face when you lose one life
  int arraySadX[16] = {1, 1, 1, 1, 2, 2, 2, 2, 4, 4, 4, 4, 5, 5, 6, 6};
  int arraySadY[16] = {1, 2, 6, 5, 1, 2, 6, 5, 2, 3, 4, 5, 6, 1, 0, 7};
  int numberSad = 16;
    

   //level1
   int arrayPixelRowL1[25] = { 1, 1, 7, 2, 4, 2, 3, 3, 7, 0, 0, 5, 5, 4, 6, 0, 6, 6, 3, 7, 5, 1};
   int arrayPixelColL1[25] = { 0, 4, 5, 4, 5, 7, 6, 1, 7, 5, 7, 0, 6, 4, 4, 2, 3, 2, 7, 0, 3, 6};
   bool arrayPixelMissingL1[25];
   
   const int numberPixelL1 = 22;
   int numberPixelMissing = 0;

   //level2
   int arrayPixelRowL2[31] = { 1, 3, 7, 2, 2, 2, 3, 3, 7, 0, 0, 5, 5, 4, 6, 0, 6, 6, 3, 7, 5, 1 ,1, 1, 6, 2, 4, 7, 5, 4};
   int arrayPixelColL2[31] = { 0, 2, 5, 4, 5, 2, 6, 1, 7, 5, 7, 0, 6, 4, 4, 2, 3, 2, 7, 0, 3, 6, 3, 4, 6, 3, 3, 4, 7, 0};
   
   bool arrayPixelMissingL2[31];

   const int numberPixelL2 = 30;
   

   //level3
   int arrayPixelRowL3[13] = {0, 2, 5, 6, 6, 7, 2, 3, 0, 4, 6, 4, 7};
   int arrayPixelColL3[13] = {3, 2, 7, 0, 3, 7, 7, 0, 6, 4, 5, 1, 4};

   int arrayWallRowL3[19] = {2, 1, 1, 2, 2, 4, 4, 4, 6, 5, 5, 5, 5, 7, 5, 0, 1, 2, 2};
   int arrayWallColL3[19] = {0, 1, 2, 1, 3, 7, 6, 5, 1, 1, 2, 3, 5, 5, 4, 5, 5, 5, 6 };

   bool arrayPixelMissingL3[31];
   
   const int numberWallL3 = 19;
   const int numberPixelL3 = 13;
   bool findWallL3 = false;
   unsigned long timeBlinkEat = 0;
   bool off = false;

   //level4
   int arrayPixelRowL4[13] = {1, 2, 7, 2, 2, 6, 7, 5, 0, 4, 3, 4, 0};
   int arrayPixelColL4[13] = {1, 2, 2, 7, 5, 0, 7, 5, 4, 2, 0, 7, 7};

   int arrayWallRowL4[19] = {0, 1, 2, 3, 5, 6, 7, 1, 1, 2, 3, 3, 6, 5, 7, 6, 5};
   int arrayWallColL4[19] = {3, 3, 3, 3, 4, 4, 4, 6, 5, 6, 6, 7, 6, 6, 3, 2, 1};

   bool arrayPixelMissingL4[31];

   int arrayBombX[20] = {1, 6, 7, 3};
   int arrayBombY[20] = {2, 3, 6, 4};

   int numberWallL4 = 17;
   int numberPixelL4 = 13;
   int numberBomb = 4;

   //unsigned long timeEat = 0;
   unsigned long timeBlinkBomb = 0;
   bool findWallL4 = false;


//////////MENU///////////
char menuStructure[4][10] = {
  "StartGame",
  "HighScore",
  "Settings ",
  "Info     "
  };

  byte arrowChar[8] = {
    B10000,
    B01000,
    B00100,
    B00010,
    B00100,
    B01000,
    B10000,
    B00000
  };

  byte fullHeartChar[8] = {
    B00000,
    B11011,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000,
    B00000
  };

  int xValueMenu = 0;
  int yValueMenu = 0;
  int swValue = 0;
  
  int minThresholdMenu = 300;
  int maxThresholdMenu = 800;
  bool joyMovedyM = false;
  bool joyMovedxM = false;

  bool initializareL1 = false;
  bool initializareL2 = false;
  bool initializareL3 = false;
  bool initializareL4 = false;
  
  bool playGame = false;
  int menuPosition = 0;
  const int numebrSturctures = 4;
  bool creatMenu = false;
  bool lock = false;
  bool lockScrolling = false;
  int here = 0;
  bool inLevel = false;

  bool conditionToMessIntro = false;
  bool messageIntro = false;
  unsigned long timeMessIntro = 0;
  String nameG;
  unsigned long pause = 0;
  bool restartGame = false;

  unsigned long timeM = 0;
  int countTime = 11;
  unsigned long dpTime = 0;
  unsigned long lastDebounceTime = 0; 
  unsigned long debounceDelay = 50;
  int lastSwState = 1;
  int swState;
  unsigned long timeStartGame = 0;
  unsigned long timeGame = 0;
  int lives = 3;
  int level = 1;
  int score = 0;
  
  
  String message = "Sorry, You lose!";
  String messageStart = "Hi, welcome to Hungry Pixel!";
  bool gameDone = false;
  int highScore = 0;
  int startingLevel = 1;

  void setOffPixel(int posx, int posy) {
     matrix[posx][posy] = 0;
     lc.setLed(0, posx, posy, matrix[posx][posy]);
  }

  void setOnPixel(int posx, int posy) {
     matrix[posx][posy] = 1;
     lc.setLed(0, posx, posy, matrix[posx][posy]);
}

//at level 1 you can teleport from column 7 to column 1
void changeColumn(){
  xValue = analogRead(pinX);
  
  if (xValue > maxThreshold && joyMovedx == false && joyMovedy == false) {
    setOffPixel(positionRow, positionCol);
    
    //level1
    if (level == 1 || level == 2){
      if (positionCol < 7) {
         positionCol++;
      }
      else if (positionRow < 7 && level == 1) {
            positionCol = 0;
            positionRow++; 
      }
      else {
        positionCol = 7;
      }
      setOnPixel(positionRow, positionCol);
    }

    if (level == 3 || level == 4) {
      findWallL3 = false;
      findWallL4 = false;
    
      lastPositionCol = positionCol;
      lastPositionRow = positionRow;
    
      if (positionCol < 7) {
          positionCol++;
      }
      else {
          positionCol = 7;
      }
    
      if (level == 3){
          findWallL3 = colizWallsL3(); 
    
          if (findWallL3 == false) {
              setOnPixel(positionRow, positionCol);
          } 
       }
       else {
          findWallL4 = colizWallsL4(); 
    
          if (findWallL4 == false) {
              setOnPixel(positionRow, positionCol);
          } 
       }
    }
    
    joyMovedx = true;
 }

  if (xValue < minThreshold && joyMovedx == false && joyMovedy == false){
    setOffPixel(positionRow, positionCol);

    if (level == 1 || level == 2) {
      if (positionCol > 0) {
         positionCol--;
      }
      else if (positionRow > 0 && level == 1) {
             positionCol = 7;
             positionRow--;
       }
      else {
        positionCol = 0;
      }
      setOnPixel(positionRow, positionCol);
    }

    if (level == 3 || level == 4) {
      findWallL3 = false;
      lastPositionCol = positionCol;
      lastPositionRow = positionRow;
    
      if (positionCol > 0) {
          positionCol --;
      }
      else {
         positionCol = 0;
      }

      if (level == 3) {
          findWallL3 = colizWallsL3(); 
    
          if (findWallL3 == false) {
              setOnPixel(positionRow, positionCol);
          } 
       }
       else {
          findWallL4 = colizWallsL4(); 
    
          if (findWallL4 == false) {
              setOnPixel(positionRow, positionCol);
          } 
       }
    }
    joyMovedx = true;
  }
}

//at level 1 you can teleport from one line to another below
void changeRow() {
  yValue = analogRead(pinY);

  if (yValue > maxThreshold && joyMovedy == false && joyMovedx == false) {
    setOffPixel(positionRow, positionCol);

    //level1
    if (level == 1 || level == 2){
       if (positionRow > 0) {
          positionRow --;
       }
       
       else if(positionCol > 0 && level == 1){
              positionRow = 7;
              positionCol --;
       }
       else {
          positionRow = 0;
       }
        setOnPixel(positionRow, positionCol);
    }

    if (level == 3 || level == 4) {
      findWallL3 = false;
      findWallL4 = false;
      lastPositionCol = positionCol;
      lastPositionRow = positionRow;
    
      if (positionRow > 0) {
          positionRow --;
      }
      else {
          positionRow = 0;
      }

      if (level == 3) {
          findWallL3 = colizWallsL3(); 
    
          if (findWallL3 == false) {
              setOnPixel(positionRow, positionCol);
          } 
       }
       else {
          findWallL4 = colizWallsL4(); 
    
          if (findWallL4 == false) {
              setOnPixel(positionRow, positionCol);
          } 
       }
    }
    joyMovedy = true;
  }

  if (yValue < minThreshold && joyMovedy == false && joyMovedx == false) {
    setOffPixel(positionRow, positionCol);

    if (level == 1 || level == 2){ 
       if (positionRow < 7) {
          positionRow ++;
       }
       else if(positionCol > 0 && level == 1){
              positionRow = 0;
              positionCol--;
       }
       else {
          positionRow = 7;
       }
        setOnPixel(positionRow, positionCol);
    }

    if (level == 3 || level == 4) {
      findWallL3 = false;
      findWallL4 = false;
      lastPositionCol = positionCol;
      lastPositionRow = positionRow;
    
      if (positionRow < 7) {
          positionRow ++;
      }
       else {
          positionRow = 7;
      }

      if (level == 3) {
          findWallL3 = colizWallsL3();
    
           if (findWallL3 == false) {
               setOnPixel(positionRow, positionCol);
           }
      }
      else {
          findWallL4 = colizWallsL4();
    
           if (findWallL4 == false) {
               setOnPixel(positionRow, positionCol);
           }
      }
      
    }
    
    joyMovedy = true;
  }
}
  void diagonal(){
    if (xValue >= minThreshold && xValue <= maxThreshold && joyMovedx == true) {
        joyMovedx = false; 

          if (level == 3) {
            eatPixel3();
          }
          else if (level == 4){
              eatPixel4();
          }
   } 

    if (yValue >= minThreshold && yValue <= maxThreshold && joyMovedy == true) {
        joyMovedy = false;
        
        if (level == 3) {
            eatPixel3();
          }
          else if (level == 4){
              eatPixel4();
          }
    } 
}

// initialize the matrix with the pixels to be eaten and the hungry pixel on column 0 line 0
void initLevel1() {
  positionRow = 0;
  positionCol = 0;
  for (int row = 0; row < 8; row++) {
     for (int col = 0; col < 8; col++) {
        matrix[row][col] = 0;
        lc.setLed(0, row, col, matrix[row][col]);
     }
  }
  setOnPixel(positionRow, positionCol);
  for (int i = 0; i < numberPixelL1; i++) {
    setOnPixel(arrayPixelRowL1[i], arrayPixelColL1[i]);
    arrayPixelMissingL1[i] = false;
   } 
   numberPixelMissing = 0;  
}

//count and mark if we have passed over a pixel that we can eat
void eatPixel() {
  for (int i = 0; i < numberPixelL1; i++) {
      if (positionRow == arrayPixelRowL1[i] && positionCol == arrayPixelColL1[i] && arrayPixelMissingL1[i] == false) {
        arrayPixelMissingL1[i] = true;
        numberPixelMissing = numberPixelMissing + 1;
        
      }
  }
}

void initLevel2() {
  positionRow = 0;
  positionCol = 0;
  for (int row = 0; row < 8; row++) {
     for (int col = 0; col < 8; col++) {
        matrix[row][col] = 0;
        lc.setLed(0, row, col, matrix[row][col]);
     }
  }
  setOnPixel(positionRow, positionCol);
  
  for (int i = 0; i < numberPixelL2; i++) {
    setOnPixel(arrayPixelRowL2[i], arrayPixelColL2[i]);
    arrayPixelMissingL2[i] = false;
   } 
   numberPixelMissing = 0; 
}

void eatPixel2() {
  for (int i = 0; i < numberPixelL2; i++) {
      if (positionRow == arrayPixelRowL2[i] && positionCol == arrayPixelColL2[i] && arrayPixelMissingL2[i] == false) {
        arrayPixelMissingL2[i] = true;
        numberPixelMissing++;
      }
  }
}

void initLevel3() {
  positionRow = 0;
  positionCol = 0;
  for (int row = 0; row < 8; row++) {
     for (int col = 0; col < 8; col++) {
        matrix[row][col] = 0;
        lc.setLed(0, row, col, matrix[row][col]);
     }
  }
  setOnPixel(positionRow, positionCol);
  for (int i = 0; i < numberWallL3; i++) {
      setOnPixel(arrayWallRowL3[i], arrayWallColL3[i]);
    }

  for (int i = 0; i < numberPixelL3; i++){
      setOnPixel(arrayPixelRowL3[i], arrayPixelColL3[i]);
      arrayPixelMissingL3[i] = false;
  }
    numberPixelMissing = 0; 
    timeBlinkEat = millis();  
    lastPositionRow = 0;
    lastPositionCol = 0;
}

void eatPixel3() {
  for (int i = 0; i < numberPixelL3; i++) {
      if (positionRow == arrayPixelRowL3[i] && positionCol == arrayPixelColL3[i] && arrayPixelMissingL3[i] == false) {
        arrayPixelMissingL3[i] = true;
        numberPixelMissing++; 
   }
  }
}

//function that looks for if you are on a pixel that is a wall
bool colizWallsL3(){
  for (int i = 0; i < numberWallL3 && findWallL3 == false; i++) {
      if (positionRow == arrayWallRowL3[i] && positionCol == arrayWallColL3[i]) {
        findWallL3 = true;
        setOnPixel(lastPositionRow, lastPositionRow);
        positionCol = lastPositionCol;
        positionRow = lastPositionRow;
        
      }
    }
    return findWallL3;
}

//to differentiate the food on the wall, the food blinks
void blinkEatL3() {
  
  if (millis() - timeBlinkEat > 1500) {
    for(int i = 0; i < numberPixelL3; i++){
        if (arrayPixelMissingL3[i] == false) {
        matrix[arrayPixelRowL3[i]][arrayPixelColL3[i]] =! matrix[arrayPixelRowL3[i]][arrayPixelColL3[i]];
        lc.setLed(0, arrayPixelRowL3[i], arrayPixelColL3[i], matrix[arrayPixelRowL3[i]][arrayPixelColL3[i]]);
      }
    }
    timeBlinkEat = millis();
  }
}

void initLevel4() {
  positionRow = 0;
  positionCol = 0;
  for (int row = 0; row < 8; row++) {
     for (int col = 0; col < 8; col++) {
        matrix[row][col] = 0;
        lc.setLed(0, row, col, matrix[row][col]);
     }
  }
  setOnPixel(positionRow, positionCol);
  for (int i = 0; i < numberWallL4; i++) {
      setOnPixel(arrayWallRowL4[i], arrayWallColL4[i]);
    }

  for (int i = 0; i < numberPixelL4; i++){
    setOnPixel(arrayPixelRowL4[i], arrayPixelColL4[i]);
    arrayPixelMissingL4[i] = false;
  }
  for (int i = 0; i < numberBomb; i++) {
    setOnPixel(arrayBombX[i], arrayBombY[i]);
  }

  numberPixelMissing = 0; 
  timeBlinkEat = millis(); 
  timeBlinkBomb = millis(); 
  lastPositionRow = 0;
  lastPositionCol = 0;
}

bool colizWallsL4(){
  for (int i = 0; i < numberWallL4 && findWallL4 == false; i++) {
      if (positionRow == arrayWallRowL4[i] && positionCol == arrayWallColL4[i]) {
        findWallL4 = true;
        setOnPixel(lastPositionRow, lastPositionCol);
        positionCol = lastPositionCol;
        positionRow = lastPositionRow;
      }
    }
    return findWallL4;
}

void blinkEatL4() {
  
  if (millis() - timeBlinkEat > 1500) {
    for(int i = 0; i < numberPixelL4; i++){
        if (arrayPixelMissingL4[i] == false) {
        matrix[arrayPixelRowL4[i]][arrayPixelColL4[i]] =! matrix[arrayPixelRowL4[i]][arrayPixelColL4[i]];
        lc.setLed(0, arrayPixelRowL4[i], arrayPixelColL4[i], matrix[arrayPixelRowL4[i]][arrayPixelColL4[i]]);
      }
    }
      timeBlinkEat = millis();
  }
}

//to differentiate between wall and bomb food, bomb bliks
void blinkBomb() {
  if (millis() - timeBlinkBomb > 400) {
    for(int i = 0; i < numberBomb; i++){
      matrix[arrayBombX[i]][arrayBombY[i]] =! matrix[arrayBombX[i]][arrayBombY[i]];
      lc.setLed(0, arrayBombX[i], arrayBombY[i], matrix[arrayBombX[i]][arrayBombY[i]]);
    }
    timeBlinkBomb = millis();
  }
}

//check if the position we are in is the bomb
bool bombix = false;
bool isBomb() {
  for(int i = 0; i < numberBomb; i++){
      if (positionRow == arrayBombX[i] && positionCol == arrayBombY[i]){
        bombix = true;
      }
    }
    return bombix;
}

void eatPixel4() {
  for (int i = 0; i < numberPixelL4; i++) {
      if (positionRow == arrayPixelRowL4[i] && positionCol == arrayPixelColL4[i] && arrayPixelMissingL4[i] == false ) {
        arrayPixelMissingL4[i] = true;
        numberPixelMissing++;
   }
  }
}


//////Menu//////
void returnToPrincipalMenu() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("*MENU GAME*");

  lcd.createChar(0, arrowChar);
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.print(menuStructure[0]); 
  lock = false;
  lockScrolling = false;
  happyFace();
  lock = false;
  level = 1;
}

bool messageToStart(){
    if (millis() - timeMessIntro < 15000) {
      //lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(messageStart.substring(0, 14));
      lcd.setCursor(0, 1);
      lcd.print(messageStart.substring(14, 32));
      
    }
    else if(messageIntro == false){
      messageIntro = true;
      lcd.clear();
      returnToPrincipalMenu();
    }
    return messageIntro;
}

void scrollingMenu() {
  yValue = analogRead(pinY);

  if (yValue < minThreshold && joyMoved == false && lockScrolling == false) {
    if (menuPosition < numebrSturctures - 1) {
      menuPosition++;
    }
    else {
      menuPosition = 0;
    }
    
     lcd.createChar(0, arrowChar);
     lcd.setCursor(0, 1);
     lcd.write(byte(0));
     lcd.print(menuStructure[menuPosition]); 
     joyMoved = true;
  }

  if (yValue > maxThreshold && joyMoved == false && lockScrolling == false) {
    if (menuPosition > 0){
      menuPosition--;
    }
    else {
      menuPosition = numebrSturctures - 1;
    }
    lcd.createChar(0, arrowChar);
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    lcd.print(menuStructure[menuPosition]); 
    joyMoved = true;
  }

  if (yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  } 
}

void buttonState(){
  swValue = digitalRead(pinSW);
  
  if (swValue != lastSwState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
    if ((millis() - lastDebounceTime) > debounceDelay) {
    // if the button state has changed:
      if (swValue != swState) {
         swState = swValue;
         if (swState == 0) {
            lock = !lock;
         }
      }
    }
   lastSwState = swValue;
}

void creatMenuF() {
  swValue = digitalRead(pinSW);
  
  buttonState();
  if (lock == true && creatMenu == false) {
    creatMenu = true;
    playGame = true;
    lcd.clear();
    //stop scrolling for the 3 menu structures
    lockScrolling = true;
    
    switch(menuPosition) {
    
      case 0:
      
        here = 0;

        switch (level) {

          case 1:
            initializareL1 = false;
            lives = 3;
          break; 

           case 2:
            initializareL2 = false;
            lives = 3;
          break; 

          case 3:
            initializareL3 = false;
            lives = 3;          
          break; 

          case 4:
            initializareL4 = true;
            
          break; 
        }
  
      case 1:
        here = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Your HighScore, ");
        lcd.setCursor(0, 1);
        //read the name of the player from Serial Monitor
         if (Serial.available()) {
             while (Serial.available() > 0) {
                lcd.write(Serial.read());
             }
          }
          lcd.setCursor(6, 1);
        lcd.print(highScore);
       break;

       case 2:
        here = 2;  
        lcd.setCursor(0, 0);
        lcd.print("Starting level");
        lcd.setCursor(13, 0);
        lcd.print(startingLevel);
        break;

        case 3:
        here = 3;
        lcd.setCursor(0,0);
        lcd.print("Hungry Pixel");
        lcd.setCursor(0, 1);
        lcd.print("Negru Luiza");
    }
  }
}

void Menu() {
  
  switch (menuPosition) {
    
      case 0:
        switch (level) {
          case 1:
            //if now the level begins we print on lcd: the level we are at, how many lives we have, the score and the time left
            if (initializareL1 == false) {
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Level:");
              lcd.setCursor(6, 0);
              lcd.print(level);

              lcd.setCursor(8, 0);
              lcd.print("Lives");
              
              switch(lives){
                 case 3:
                  lcd.createChar(0, fullHeartChar);
                  lcd.setCursor(13, 0);
                  lcd.write(byte(0));
                  lcd.setCursor(14, 0);
                  lcd.write(byte(0));
                  lcd.setCursor(15, 0);
                  lcd.write(byte(0));
                 break;
        
                 case 2:
                   lcd.createChar(0, fullHeartChar);
                   lcd.setCursor(14, 0);
                   lcd.write(byte(0));
                   lcd.setCursor(15, 0);
                   lcd.write(byte(0));
                break;

                 case 1:
                   lcd.createChar(0, fullHeartChar);
                   lcd.setCursor(15, 0);
                   lcd.write(byte(0));
                break;
              }

              score = 0;
              lcd.setCursor(0, 1);
              lcd.print("Score");
              lcd.setCursor(6, 1);
              lcd.print(score);

              timeGame = 30;
              lcd.setCursor(8, 1);
              lcd.print("Time:");
              lcd.setCursor(13, 1);
              lcd.print(timeGame);

              countTime = 3;
              timeStartGame = millis();
              initLevel1();
              initializareL1 = true;
              level = 1;
            }

            if (gameOver == false && restartGame == false) {
              changeColumn();
              changeRow();
              diagonal();
              eatPixel();
              score = numberPixelMissing;
              lcd.setCursor(5, 1);
              lcd.print(" ");
              lcd.setCursor(6, 1);
              lcd.print(score);
            //time decreases and is displayed every one second
            if ((millis() - timeStartGame) / 1000 == countTime) {
              countTime++;
              timeGame--;
              lcd.setCursor(13, 1);
              lcd.print("  ");
              lcd.setCursor(13, 1);
              lcd.print(timeGame);
             }
              //if we ate all the pixels on the map the highscore is calculated
              if (numberPixelL1 == numberPixelMissing) {
                level++;
                initializareL2 = false;
                //HIGHSCORE
                score = score * (30 - timeGame);
                if (score > highScore){
                  highScore = score;
                }
                EEPROM.write(0, highScore);
              }
              pause = millis();
            }
            //if the time has reached 0, we check if we still have lives
            if (timeGame == 0) {
               
               sadFace();
               restartGame = true;
               if (millis() - pause > 5000){
                  timeGame = 30;
                  restartGame = false;
                  //if we have at least one life, we resume the game from the level we reached
                  if (lives > 1) {
                    lives--;
                    if (lives == 2) {
                    lcd.setCursor(13, 0);
                    lcd.print(" ");
                    lcd.setCursor(13, 0);
                    }
                  if (lives == 1) {
                     lcd.setCursor(13, 0);
                     lcd.print("  ");
                    lcd.setCursor(13, 0);
                  }
                    initializareL1 = false;
                  }
                  else {
                    gameOver = true;
                    pause = millis();
                  }                 
               }
               lcd.clear();
               lcd.setCursor(0,0);
               lcd.print("You still have:");
               lcd.setCursor(3,1);
               lcd.print(lives - 1);
               lcd.setCursor(5,1);
               lcd.print("live/s");
               
            }
            //if we do not have lives we return to the main menu
            if ( gameOver == true) {
              playGame = false;
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Sorry, You lose!");
              sadFace();
              if (millis() - pause > 5000){
                gameOver = false;
                happyFace();
                level = 1;
                creatMenu = false;
                //initializareL1 = false;
                returnToPrincipalMenu();
              }
            }
          //break level1
          break;
        

        //level2
        case 2:
            if (initializareL2 == false) {
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Level:");
              lcd.setCursor(6, 0);
              lcd.print(level);

              lcd.setCursor(8, 0);
              lcd.print("Lives");
              
              switch(lives){
                 case 3:
                  lcd.createChar(0, fullHeartChar);
                  lcd.setCursor(13, 0);
                  lcd.write(byte(0));
                  lcd.setCursor(14, 0);
                  lcd.write(byte(0));
                  lcd.setCursor(15, 0);
                  lcd.write(byte(0));
                 break;
        
                 case 2:
                   lcd.createChar(0, fullHeartChar);
                   lcd.setCursor(14, 0);
                   lcd.write(byte(0));
                   lcd.setCursor(15, 0);
                   lcd.write(byte(0));
                break;

                 case 1:
                   lcd.createChar(0, fullHeartChar);
                   lcd.setCursor(15, 0);
                   lcd.write(byte(0));
                break;
              }

              score = 0;
              lcd.setCursor(0, 1);
              lcd.print("Score");
              lcd.setCursor(6, 1);
              lcd.print(score);

              timeGame = 30;
              lcd.setCursor(8, 1);
              lcd.print("Time:");
              lcd.setCursor(13, 1);
              lcd.print(timeGame);

              countTime = 3;
              timeStartGame = millis();
              initLevel2();
              initializareL2 = true;
              level = 2;
            }

            if (gameOver == false && restartGame == false) {
              changeColumn();
              changeRow();
              diagonal();
              eatPixel2();

            if ((millis() - timeStartGame) / 1000 == countTime) {
              countTime++;
              timeGame--;
              lcd.setCursor(13, 1);
              lcd.print("  ");
              lcd.setCursor(13, 1);
              lcd.print(timeGame);

              score = numberPixelMissing;
              lcd.setCursor(5, 1);
              lcd.print(" ");
              lcd.setCursor(6, 1);
              lcd.print(score);
              
              }

              if (numberPixelL2 == numberPixelMissing) {
                level++;
                initializareL3 = false;
                score = score * (30 - timeGame);
                if (score > highScore){
                  highScore = score;
                }
                EEPROM.write(0, highScore);
              }
              pause = millis();
            }

            if (timeGame == 0) {
               
               sadFace();
               restartGame = true;
               if (millis() - pause > 5000){
                  timeGame = 30;
                  restartGame = false;
                  if (lives > 1) {
                    lives--;
                    if (lives == 2) {
                    lcd.setCursor(13, 0);
                    lcd.print(" ");
                    lcd.setCursor(13, 0);
                    }
                  if (lives == 1) {
                     lcd.setCursor(13, 0);
                     lcd.print("  ");
                    lcd.setCursor(13, 0);
                  }
                    initializareL2 = false;
                  }
                  else {
                    gameOver = true;
                    pause = millis();
                  }                 
               }
               lcd.clear();
               lcd.setCursor(0,0);
               lcd.print("You still have:");
               lcd.setCursor(3,1);
               lcd.print(lives - 1);
               lcd.setCursor(5,1);
               lcd.print("live/s");
               
            }
            if ( gameOver == true) {
              playGame = false;
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Sorry, You lose!");
              sadFace();
              if (millis() - pause > 5000){
                gameOver = false;
                happyFace();
                level = 1;
                creatMenu = false;
                lockScrolling = false;
                //initializareL1 = false;
                returnToPrincipalMenu();
              }
            }
  
          //break level2
          break;


         //level3
         case 3:
            if (initializareL3 == false) {
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Level:");
              lcd.setCursor(6, 0);
              lcd.print(level);

              lcd.setCursor(8, 0);
              lcd.print("Lives");
              
              switch(lives){
                 case 3:
                  lcd.createChar(0, fullHeartChar);
                  lcd.setCursor(13, 0);
                  lcd.write(byte(0));
                  lcd.setCursor(14, 0);
                  lcd.write(byte(0));
                  lcd.setCursor(15, 0);
                  lcd.write(byte(0));
                 break;
        
                 case 2:
                   lcd.createChar(0, fullHeartChar);
                   lcd.setCursor(14, 0);
                   lcd.write(byte(0));
                   lcd.setCursor(15, 0);
                   lcd.write(byte(0));
                break;

                 case 1:
                   lcd.createChar(0, fullHeartChar);
                   lcd.setCursor(15, 0);
                   lcd.write(byte(0));
                break;
              }

              score = 0;
              lcd.setCursor(0, 1);
              lcd.print("Score");
              lcd.setCursor(6, 1);
              lcd.print(score);

              timeGame = 30;
              lcd.setCursor(8, 1);
              lcd.print("Time:");
              lcd.setCursor(13, 1);
              lcd.print(timeGame);

              countTime = 3;
              timeStartGame = millis();
              initLevel3();
              initializareL3 = true;
            }

            if (gameOver == false && restartGame == false) {
              changeColumn();
              changeRow();
              diagonal();
              eatPixel3();
              blinkEatL3();

            if ((millis() - timeStartGame) / 1000 == countTime) {
              countTime++;
              timeGame--;
              lcd.setCursor(13, 1);
              lcd.print("  ");
              lcd.setCursor(13, 1);
              lcd.print(timeGame);

              score = numberPixelMissing;
              lcd.setCursor(5, 1);
              lcd.print(" ");
              lcd.setCursor(6, 1);
              lcd.print(score);
              
              }

              if (numberPixelL3 == numberPixelMissing) {
                level++;
                initializareL4 = false;
                score = score * (30 - timeGame);
                if (score > highScore){
                  highScore = score;
                }
                EEPROM.write(0, highScore);
              }
              pause = millis();
            }

            if (timeGame == 0) {
               
               sadFace();
               restartGame = true;
               if (millis() - pause > 5000){
                  timeGame = 30;
                  restartGame = false;
                  if (lives > 1) {
                    lives--;
                    timeGame = 30;
                    if (lives == 2) {
                    lcd.setCursor(13, 0);
                    lcd.print(" ");
                    lcd.setCursor(13, 0);
                    }
                  if (lives == 1) {
                     lcd.setCursor(13, 0);
                     lcd.print("  ");
                    lcd.setCursor(13, 0);
                  }
                    initializareL3 = false;
                  }
                  else {
                    gameOver = true;
                    pause = millis();
                  }                 
               }
               lcd.clear();
               lcd.setCursor(0,0);
               lcd.print("You still have:");
               lcd.setCursor(3,1);
               lcd.print(lives - 1);
               lcd.setCursor(5,1);
               lcd.print("live/s");
               
            }
            if ( gameOver == true) {
              playGame = false;
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Sorry, You lose!");
              sadFace();
              if (millis() - pause > 5000){
                gameOver = false;
                happyFace();
                level = 1;
                creatMenu = false;
                lockScrolling = false;
                //initializareL1 = false;
                returnToPrincipalMenu();
              }
            }
  
          //break level3
          break;

          //level4
         case 4:
            if (initializareL4 == false) {
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Level:");
              lcd.setCursor(6, 0);
              lcd.print(level);

              lcd.setCursor(8, 0);
              lcd.print("Lives");
              
              switch(lives){
                 case 3:
                  lcd.createChar(0, fullHeartChar);
                  lcd.setCursor(13, 0);
                  lcd.write(byte(0));
                  lcd.setCursor(14, 0);
                  lcd.write(byte(0));
                  lcd.setCursor(15, 0);
                  lcd.write(byte(0));
                 break;
        
                 case 2:
                   lcd.createChar(0, fullHeartChar);
                   lcd.setCursor(14, 0);
                   lcd.write(byte(0));
                   lcd.setCursor(15, 0);
                   lcd.write(byte(0));
                break;

                 case 1:
                   lcd.createChar(0, fullHeartChar);
                   lcd.setCursor(15, 0);
                   lcd.write(byte(0));
                break;
              }

              score = 0;
              lcd.setCursor(0, 1);
              lcd.print("Score");
              lcd.setCursor(6, 1);
              lcd.print(score);

              timeGame = 30;
              lcd.setCursor(8, 1);
              lcd.print("Time:");
              lcd.setCursor(13, 1);
              lcd.print(timeGame);

              countTime = 3;
              timeStartGame = millis();
              initLevel4();
              initializareL4 = true;
            }

            if (gameOver == false && restartGame == false) {
              changeColumn();
              changeRow();
              diagonal();
              eatPixel4();
              blinkEatL4();
              blinkBomb();
              bombix = isBomb();

            if ((millis() - timeStartGame) / 1000 == countTime) {
              countTime++;
              timeGame--;
              lcd.setCursor(13, 1);
              lcd.print("  ");
              lcd.setCursor(13, 1);
              lcd.print(timeGame);

              score = numberPixelMissing;
              lcd.setCursor(5, 1);
              lcd.print(" ");
              lcd.setCursor(6, 1);
              lcd.print(score);
              
              }

              if (numberPixelL4 == numberPixelMissing) {
                
                initializareL4 = false;
                score = score * (30 - timeGame);
                if (score > highScore){
                  highScore = score;
                }
                EEPROM.write(0, highScore);
                returnToPrincipalMenu();
              }
              pause = millis();
            }

            if (timeGame == 0 || bombix == true) {
               
               sadFace();
               restartGame = true;
               if (millis() - pause > 5000){
                  timeGame = 30;
                  bombix = false;
                  restartGame = false;
                  if (lives > 1) {
                    lives--;
                    timeGame = 30;
                    if (lives == 2) {
                    lcd.setCursor(13, 0);
                    lcd.print(" ");
                    lcd.setCursor(13, 0);
                    }
                  if (lives == 1) {
                     lcd.setCursor(13, 0);
                     lcd.print("  ");
                    lcd.setCursor(13, 0);
                  }
                    initializareL4 = false;
                  }
                  else {
                    gameOver = true;
                    pause = millis();
                  }                 
               }
               lcd.clear();
               lcd.setCursor(0, 0);
               lcd.print("You still have:");
               lcd.setCursor(3,1);
               lcd.print(lives - 1 );
               lcd.setCursor(5, 1);
               lcd.print("live/s");
               
            }
            if ( gameOver == true) {
              playGame = false;
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Sorry, You lose!");
              sadFace();
              if (millis() - pause > 5000){
                gameOver = false;
                happyFace();
                level = 1;
                creatMenu = false;
                lockScrolling = false;
                //initializareL1 = false;
                returnToPrincipalMenu();
              }
            }
  
          //break level4
          break;
        
        }
      case 1:
          //print name in highScore
          if (Serial.available() > 0) {
              // read the incoming byte:
              nameG = Serial.read();
              // say what you got:
              lcd.setCursor(0, 1);
              lcd.print(nameG);
          }
      break;
      case 2:
              //change the level in setting
              lcd.setCursor(7, 1);
              lcd.print(startingLevel);
             xValue = analogRead(pinX);
             
             if (xValue > maxThreshold && joyMovedx == false) {
              if (startingLevel < 4){
                ++startingLevel;
              }
              else {
                startingLevel = 1;
              }
              
              level = startingLevel;
   
              lcd.setCursor(7, 1);
              lcd.print(startingLevel);
              joyMovedx = true;
             }

             if (xValue < minThreshold && joyMovedx == false) {
              if (startingLevel > 1) {
                --startingLevel;
              }
              else {
                startingLevel = 4;
              }
              
              level = startingLevel;
              lcd.setCursor(7, 1);
              lcd.print(startingLevel);
              joyMovedx = true;
             }

             if (xValue >= minThreshold && xValue <= maxThreshold) {
                   joyMovedx = false;
                   level = startingLevel;
              } 
            break;
      //break Menu
      //break;
  }
    if (here == 0 || here == 1 || here == 2 || here == 3) {
      buttonState();
         if (lock == false) {
          lockScrolling = false;
          creatMenu = false;
          menuPosition = 0;
          lcd.clear();
          returnToPrincipalMenu();
         } 
     }
}



void sadFace() {
  for (int row = 0; row < 8; row++) {
     for (int col = 0; col < 8; col++) {
        matrix[row][col] = 0;
        lc.setLed(0, row, col, matrix[row][col]);
     }
  }
  for (int i = 0; i < 16; i++) {
    matrix[arraySadX[i]][arraySadY[i]] = 1;
    lc.setLed(0, arraySadX[i], arraySadY[i], matrix[arraySadX[i]][arraySadY[i]]);
  }
  
}

void happyFace() {
  for (int row = 0; row < 8; row++) {
     for (int col = 0; col < 8; col++) {
        matrix[row][col] = 0;
        lc.setLed(0, row, col, matrix[row][col]);
     }
  }
  
  for (int row = 0; row < 8; row++) {
     for (int col = 0; col < 8; col++) {
        lc.setLed(1, row, col, matrix[row][col]);
     }
  }
  
  for (int i = 0; i < 16; i++) {
    matrix[arraySmilyX[i]][arraySmilyY[i]] = 1;
    lc.setLed(0, arraySmilyX[i], arraySmilyY[i], matrix[arraySmilyX[i]][arraySmilyY[i]]);
  }

}
void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, brightness);
  lc.clearDisplay(0);
  for (int row = 0; row < 8; row++) {
     for (int col = 0; col < 8; col++) {
        lc.setLed(1, row, col, matrix[row][col]);
     }
  }
  
  for (int i = 0; i < 16; i++) {
    matrix[arraySmilyX[i]][arraySmilyY[i]] = 1;
    lc.setLed(0, arraySmilyX[i], arraySmilyY[i], matrix[arraySmilyX[i]][arraySmilyY[i]]);
  }

  pinMode(V0_PIN, OUTPUT); 
  analogWrite(V0_PIN, 140);

  pinMode(pinSW, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.setCursor(3, 0);
  lcd.print("*MENU GAME*");

  lcd.createChar(0, arrowChar);
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.print(menuStructure[0]); 

  highScore = EEPROM.read(0);
  EEPROM.write(0,0);

  Serial.begin(9600);
}


void loop() {

  conditionToMessIntro = messageToStart();
  if (conditionToMessIntro == false) {
      messageToStart();
  }
  
  scrollingMenu();
  creatMenuF();
  
  if (lockScrolling == 1) {
      Menu();    
  }
  
}
