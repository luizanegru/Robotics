#include<LiquidCrystal.h>

const int rsPin = 12;
const int ePin = 11;
const int d4Pin = 5;
const int d5Pin = 4;
const int d6Pin = 3;
const int d7Pin = 2;
const int V0_PIN = 9;

const int pinSW = 10; 
const int pinX = A0; 
const int pinY = A1;

int xValue = 0;
int yValue = 0;
int swValue = 0;
int minThreshold = 400;
int maxThreshold = 600;
bool joyMoved = false;
bool joyMovedx = false;

int menuPosition = 0;
const int numebrSturctures = 3;
bool lock = false;
bool lockScrolling = false;
bool creatMenu = false;

unsigned long dpTime = 0;
unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 50;
int lastSwState = 1;
int swState;

unsigned long timeGame = 0;
int lives = 2;
int level = 0;
int score = 0;
//0 - you are in StartGame
//1 - you are HighScore
//2 - you are in Settings
int here = -1;
String nameG;
String message = "Congratulations, you finished the game. Press button to exit";
bool gameDone = false;
int highScore = 0;
int startingLevel = 0;


byte arrowChar[] = {
  B10000,
  B01000,
  B00100,
  B00010,
  B00100,
  B01000,
  B10000,
  B00000
};

byte fullHeartChar[] = {
  B00000,
  B11011,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};

char menuStructure[numebrSturctures][10] = {
  "StartGame",
  "HighScore",
  "Settings "
};

LiquidCrystal lcd(rsPin,ePin, d4Pin, d5Pin, d6Pin, d7Pin);


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
    lcd.clear();
    //stop scrolling for the 3 menu structures
    lockScrolling = true;
    
    switch(menuPosition) {
    
      case 0:
        here = 0;

        timeGame = millis();
        
        lcd.setCursor(0, 0);
        lcd.print("Level:");
        lcd.setCursor(6, 0);
        lcd.print(level);
        
        lcd.setCursor(0, 1);
        lcd.print("Score");
        lcd.setCursor(6, 1);
        score = level * 3;
        lcd.print(score);

        lcd.setCursor(8, 0);
        lcd.print("Lives");

        //show a heart for every life
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
                
      break;

      case 1:
        here = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("HighScore");
        lcd.print(highScore);
       break;

       case 2:
        here = 2;  
        lcd.setCursor(0, 1);
        lcd.print("Starting level");
        //read the name of the player from Serial Monitor
        lcd.setCursor(0, 0);
         if (Serial.available()) {
             while (Serial.available() > 0) {
                lcd.write(Serial.read());
             }
          }
        break;
    }
  }
}

void returnToPrincipalMenu() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("*MENU GAME*");

  lcd.createChar(0, arrowChar);
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.print(menuStructure[0]); 
  
}

void changeTheMenu() {
   if (creatMenu == true) {
    switch (here) {
      case 0:
        
        if (millis() - timeGame == 5000) {
          
            ++level;
            lcd.setCursor(6, 0);
            lcd.print(level);
            score = level * 3;
            lcd.setCursor(6, 1);
            lcd.print(score);
            
         }

         if (millis() - timeGame == 10000) {
            lcd.clear(); 
            //lcd.setCursor(0,0);   
            lcd.print(message.substring(0, 32));
            lcd.setCursor(0,1);
            lcd.print(message.substring(16, 30));
            
            delay(1500);
            
            lcd.clear();
            lcd.print(message.substring(30, 45));
            lcd.setCursor(0,1);
            lcd.print(message.substring(46, 61));

            gameDone = true;
         }
         break;

         case 1:
            if (gameDone == true) {
              if (score > highScore) {
                highScore = score;
                lcd.setCursor(8, 0);
                lcd.print(" ");
                lcd.print(highScore);
              }
            }
          break;

          case 2:
             lcd.setCursor(0, 0);
              
             xValue = analogRead(pinX);
             
             if (xValue > maxThreshold && joyMovedx == false) {
              ++startingLevel;
              level = startingLevel;
   
              lcd.setCursor(14, 1);
              lcd.print(startingLevel);
              joyMovedx = true;
             }

             if (xValue < minThreshold && joyMovedx == false) {
              --startingLevel;
              level = startingLevel;

              //delete the characters if we move from a two-digit number to a one-digit number
              lcd.setCursor(14,1);
              lcd.print(" ");
              lcd.setCursor(15,1);
              lcd.print(" ");
              
              lcd.setCursor(14, 1);
              lcd.print(startingLevel);
              joyMovedx = true;
             }

             if (xValue >= minThreshold && xValue <= maxThreshold) {
                   joyMovedx = false;
              } 
            break;
         
    }
    if (here == 0 || here == 1 || here == 2) {
      buttonState();
         if (lock == false) {
          lockScrolling = false;
          creatMenu = false;
          returnToPrincipalMenu();
         } 
     }
   }
}
void setup() {
  
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

  Serial.begin(9600);
}

void loop() {
  
  scrollingMenu();
  creatMenuF();
  changeTheMenu();
 
}
