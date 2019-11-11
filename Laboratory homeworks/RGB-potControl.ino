int potRedValue = 0;
int potGreenValue = 0;
int potBlueValue = 0;

int ledRedValue = 0;
int ledGreenValue = 0;
int ledBlueValue = 0;

const int potPinRed = A0;
const int potPinGreen = A1;
const int potPinBlue = A2;

const int ledPinRed = 11;
const int ledPinGreen = 10;
const int ledPinBlue = 9;

void setup() {
  Serial.begin(9600);
}

void loop() {
  potRedValue = analogRead(potPinRed);
  potGreenValue = analogRead(potPinGreen);
  potBlueValue = analogRead(potPinBlue);

  ledRedValue = map(potRedValue, 0, 1023, 0, 255);
  ledGreenValue = map(potGreenValue, 0, 1023, 0, 255);
  ledBlueValue = map(potBlueValue, 0, 1023, 0, 255);
 
  analogWrite(ledPinRed, ledRedValue);
  analogWrite(ledPinGreen, ledGreenValue);
  analogWrite(ledPinBlue, ledBlueValue);
}
