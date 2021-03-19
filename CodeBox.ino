#include <Keypad.h> // Подключаем библиотеку 

#define BUZZER_PIN 12
#define RELE_PIN 3
#define RED_LED_PIN A1
#define GREEN_LED_PIN A2

const byte ROWS = 4; // 4 строки
const byte COLS = 4; // 4 столбца
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
}; 
byte rowPins[ROWS] = {11,10, 9, 8}; 
byte colPins[COLS] = {7, 6, 5, 4}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );

bool isLockClosed = true;
const byte SERIS_LENGTH = 8;
const byte SERIES[SERIS_LENGTH] = {9,1,7,5,3,4,6,6};
byte userSeries[SERIS_LENGTH];
byte userSeriesIndex = 0;

void setup(){
  configurePinMode();
  closeLock();
  makeStartSound();
}

void loop(){
  char key = keypad.getKey();
  if (key){
    onKeyPressed(key);
  }
}

void configurePinMode(){
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELE_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
}

void onKeyPressed(char key){
  makeShortSound(1200);
  if(key == '*'){
    onSequenceEntered();
  }
  else{
    addKey(key);
  }
}

void onSequenceEntered(){
  if(isSequenceCorrect()){
    onCorrectSequence();
  }
  else{
    onErrorSequence();
  }
  clearUserSequence();
}

void addKey(char key){
  userSeries[userSeriesIndex++] = (int)key - 48;
  if(userSeriesIndex == SERIS_LENGTH){
    userSeriesIndex = 0;
  }
}

bool isSequenceCorrect(){
  for(int i = 0; i < SERIS_LENGTH; i++){
    if(userSeries[i] != SERIES[i]){
      return false;
    }
  }
  return true;
}

void onCorrectSequence(){
  delay(500);
  makeFinishSound();
  for(;;){
    toggleLock();
    delay(4000);
  }
}

void onErrorSequence(){
  delay(500);
  makeErrorSound();
}

void clearUserSequence(){
  for(int i = 0; i < SERIS_LENGTH; i++){
    userSeries[i] = 0;
  }
  userSeriesIndex = 0;
}

void makeLongSound(int frequency){
  tone(BUZZER_PIN ,frequency, 100);
  delay(100);
}

void makeShortSound(int frequency){
  tone(BUZZER_PIN ,frequency, 50);
  delay(50);
}

void makeFinishSound(){
  makeLongSound(1200);
  delay(100);
  makeLongSound(1200);
  delay(100);
  makeLongSound(1200);
}

void makeErrorSound(){
  makeShortSound(600);
  delay(50);
  makeShortSound(600);
}

void makeStartSound(){
  makeLongSound(1000);
  delay(100);
  makeLongSound(1000);
  delay(100);
  makeLongSound(1000);
  delay(100);
  makeLongSound(1000);
  delay(100);
}

void openLock(){
  digitalWrite(RELE_PIN, LOW);
  isLockClosed = false;
  greenLedOn();
}

void closeLock(){
  digitalWrite(RELE_PIN, HIGH);
  isLockClosed = true;
  redLedOn();
}

void toggleLock(){
  if(isLockClosed){
    openLock();
  }
  else{
    closeLock();
  }
}

void greenLedOn(){
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
}

void redLedOn(){
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);
}
