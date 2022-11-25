#include <Keyboard.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

const int DELAY_FAST = 300;
bool LeftPressSensorState = false;
bool RightPressSensorState = false;
bool LeftFlexSensorState = false;
bool RightFlexSensorState = false;

SoftwareSerial mySoftwareSerial(0, 1); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup() {
  Keyboard.begin();
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
}

void loop() {

  int LeftPressSensor = analogRead(A1);
  int RightPressSensor = analogRead(A2);
  int LeftFlexSensor = analogRead(A3);
  int RightFlexSensor = analogRead(A4);

  //左耳を押したかどうか判定
  if (LeftPressSensor <= 700 && LeftPressSensorState == false) {
    LeftPressSensorState = 1;
    delay(10);
  }
  //左耳を押した状態で離したかどうか判定
  if (LeftPressSensor > 700 && LeftPressSensorState == true) {
    LeftPressSensorState = false;
    Keyboard.write(KEY_DOWN_ARROW);
    sound();
  }
  //右耳を押したかどうか判定
  if (RightPressSensor <= 700 && RightPressSensorState == false) {
    RightPressSensorState = true;
    delay(10);
  }
  //右耳を押した状態で離したかどうか判定
  if (RightPressSensor > 700 && RightPressSensorState == true) {
    RightPressSensorState = false;
    Keyboard.write(KEY_UP_ARROW);
  }
  //両耳を押したかどうか判定
  if (LeftPressSensorState == true && RightPressSensorState == true) {
    RightPressSensorState = false;
    LeftPressSensorState = false;
    Keyboard.press('m');
    Keyboard.release('m');
    delay(1000);
  }


  //左腕を曲げたかどうか判定
  if (LeftFlexSensor >= 1000 && LeftFlexSensorState == false) {
    LeftFlexSensorState = 1;
    delay(10);
  }
  //左腕を戻したかどうか判定
  if (LeftFlexSensor < 1000 && LeftFlexSensorState == true) {
    LeftFlexSensorState = false;
    Keyboard.write(KEY_LEFT_ARROW);
    sound();
  }
  //右腕を曲げたかどうか判定
  if (RightFlexSensor >= 1000 && RightFlexSensorState == false) {
    RightFlexSensorState = true;
    delay(10);
  }
  //右腕を戻したかどうか判定
  if (RightFlexSensor < 1000 && RightFlexSensorState == true) {
    RightFlexSensorState = false;
    Keyboard.write(KEY_RIGHT_ARROW);
    sound();
  }
  //両腕を曲げたかどうか判定
  if (LeftFlexSensorState == true && RightFlexSensorState == true) {
    RightFlexSensorState = false;
    LeftFlexSensorState = false;
    Keyboard.press(' ');
    Keyboard.release(' ');
    sound();
  }

  if (myDFPlayer.available()) {
  printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void sound(){
    myDFPlayer.play(2);
    delay(800);
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}