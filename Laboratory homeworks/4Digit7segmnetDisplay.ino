const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int pinSW = A2; // digital pin connected to switch output
const int pinX = A1; // A1 - analog pin connected to X output
const int pinY = A0; // A0 - analog pin connected to Y output
int switchValue = 1;
int xValue = 0;
int yValue = 0;
int swValue = 0;
int lastSwState = 1;
int swState;

int minThreshold = 400;
int maxThreshold = 600;
bool lock = false;
bool joyMoved = false;
bool joyMovedy = false;
int newPosition = 0;
bool dpBlink = false;

unsigned long dpTime = 0;
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50; // the debounce time; increase if the output flickers

const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;

int dpState = LOW;
int location = 0;


int segments[segSize] = {
pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

// digits array, to switch between them easily
int digits[noOfDisplays] = {
pinD1, pinD2, pinD3, pinD4
};

int digitsValues[noOfDisplays] = {
0, 0 ,0, 0
};

byte digitMatrix[noOfDigits][segSize - 1] = {
// a b c d e f g
{1, 1, 1, 1, 1, 1, 0}, // 0
{0, 1, 1, 0, 0, 0, 0}, // 1
{1, 1, 0, 1, 1, 0, 1}, // 2
{1, 1, 1, 1, 0, 0, 1}, // 3
{0, 1, 1, 0, 0, 1, 1}, // 4
{1, 0, 1, 1, 0, 1, 1}, // 5
{1, 0, 1, 1, 1, 1, 1}, // 6
{1, 1, 1, 0, 0, 0, 0}, // 7
{1, 1, 1, 1, 1, 1, 1}, // 8
{1, 1, 1, 1, 0, 1, 1} // 9
};

void displayNumber(byte digit, byte decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }

  // write the decimalPoint to DP pin
  digitalWrite(segments[segSize - 1], decimalPoint);
}

// activate the display no. received as param
void showDigit(int num) {
  for (int i = 0; i < noOfDisplays; i++) {
    digitalWrite(digits[i], HIGH);
  }

  digitalWrite(digits[num], LOW);
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
      if (swState == 1) {
        lock = !lock;
      }
    }
  }
  lastSwState = swValue;
}

void changeDigit(){
  xValue = analogRead(pinX);
  
  if (xValue > maxThreshold && lock == true && joyMoved == false){
    newPosition--;
    if (newPosition == -1){
      newPosition = noOfDisplays - 1;
    }
    showDigit(newPosition);
    displayNumber(digitsValues[newPosition], HIGH);
    joyMoved = true;
  }

  if (xValue < minThreshold && lock == true && joyMoved == false){
    newPosition++;
    if (newPosition == noOfDisplays){
      newPosition = 0;
    }
    showDigit(newPosition);
    displayNumber(digitsValues[newPosition], HIGH);
    joyMoved = true;
  }

  if (xValue >= minThreshold && xValue <= maxThreshold) {
    joyMoved = false;
  } 
}

void changeValueDigit(){
  yValue = analogRead(pinY);
  //blink the DP

  if (yValue > maxThreshold && lock == false && joyMovedy == false){
    if(digitsValues[newPosition] < 9){
      digitsValues[newPosition]++;
    }
    else{
      digitsValues[newPosition] = 0;
    }
    showDigit(newPosition);
    displayNumber(digitsValues[newPosition], HIGH);
    joyMovedy = true;
  }

  if (yValue < minThreshold && lock == false && joyMovedy == false){
    if(digitsValues[newPosition] > 0){
      digitsValues[newPosition]--;
    }
    else{
      digitsValues[newPosition] = 9;
    }
    showDigit(newPosition);
    displayNumber(digitsValues[newPosition], HIGH);
    joyMovedy = true;
    //Serial.println(digitsValues[newPosition]);
    
  }

  if (yValue >= minThreshold && yValue <= maxThreshold) {
    joyMovedy = false;
  } 
}

void setValueDigits(){
  //assigning values
  for (int i = 0; i < 4; i++) {
    showDigit(i);
    if (i == newPosition){
      displayNumber(digitsValues[i], HIGH);
    }
    else{
      displayNumber(digitsValues[i], LOW);
    }
    
    delay(5);
  }
}

void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  
  for (int i = 0; i < segSize; i++){
    pinMode(segments[i], OUTPUT);
  }

  for (int i = 0; i < noOfDisplays; i++){
    pinMode(digits[i], OUTPUT);
  }

  Serial.begin(9600);
}

void loop(){
  dpTime = millis();
  
  setValueDigits();
  
  buttonState();

  changeDigit();

  changeValueDigit();

  if (millis() - dpTime == 2000){
    Serial.println("A");
    if (dpBlink == false){
      showDigit(newPosition);
      displayNumber(digitsValues[newPosition], HIGH);
      dpBlink = true;
    }
    else{
      showDigit(newPosition);
      displayNumber(digitsValues[newPosition], LOW);
      dpBlink = false;
    }
  }
}
