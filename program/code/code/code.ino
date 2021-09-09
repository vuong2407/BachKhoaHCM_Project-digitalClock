#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
byte verticalLine[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};  

byte char2[8] = {
  B00000,
  B00000,
  B00000,
  B11100,
  B00100,
  B00100,
  B00100,
  B00100
};

byte char1[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00111,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};

byte char3[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte char4[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b11100,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte customCharChoose[] = {
  B00001,
  B00011,
  B00111,
  B01111,
  B00111,
  B00011,
  B00001,
  B00000
};

byte customCharArrow[] = {
  B00000,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000

};

String dataCurrentTime;
String oldDataCurrentTime;

bool onNormalScreen = true;
bool oldOnNormalScreen = false;
bool currentTimeModeScreen = false;

int btnChangeMode = 0;
int btnDone = 1;
int btnDec = 2;
int btnInc = 3;
unsigned long flatSecondTime;

int dataSecondCurrent = 30;
int dataMinuteCurrent = 30;
int dataHourCurrent = 16;


void setup() {
  pinMode(btnChangeMode, INPUT);
  pinMode(btnDone, INPUT);
  pinMode(btnDec, INPUT);
  pinMode(btnInc, INPUT);
  flatSecondTime = millis();
  lcd.init();
  lcd.backlight();
}

void NormalShowLCD(){
  if (onNormalScreen != oldOnNormalScreen){
      onNormalScreen = false;
      lcd.clear();
      createCustomCharacters();
      printFrame();
      oldOnNormalScreen = onNormalScreen;
  }
  UpdateCurrentTime(true);
}

void UpdateCurrentTime(bool flag) {
  if ((unsigned long)(millis() - flatSecondTime) > 840){
    flatSecondTime = millis();
    dataSecondCurrent = dataSecondCurrent + 1;
    dataCurrentTime = ValidTime(dataSecondCurrent, dataMinuteCurrent, dataHourCurrent);
    if (flag == true) {
      lcd.setCursor(6, 1);
      lcd.print(dataCurrentTime);
    }
  }
}

void ChangeMode(){
    lcd.clear();
    int tempChoose = 0;
    int oldStateBtnDec = digitalRead(btnDec);
    int oldStateBtnInc = digitalRead(btnInc);
    for (;;){ 
      UpdateCurrentTime(false);
      lcd.setCursor(0, 0);
      lcd.print("1: TIME CURRENT");
      lcd.createChar(0, customCharChoose);
      lcd.setCursor(0, 1);
      lcd.print("2: COUNTING");
      lcd.setCursor(0, 2);
      lcd.print("3: ALARM");
      lcd.setCursor(7, 3);
      lcd.print("Cancel");
      int stateBtnDec = digitalRead(btnDec);
      int stateBtnInc = digitalRead(btnInc);
      if ((stateBtnDec == LOW) && (oldStateBtnDec != stateBtnDec)){
        tempChoose = (tempChoose <= 2) ? tempChoose + 1 : tempChoose;
        for (int i = 0; i < tempChoose; i++){
          lcd.setCursor(19, i);
          lcd.print(" ");
        }
        oldStateBtnDec = stateBtnDec;
      }else oldStateBtnDec = stateBtnDec;
      if ((stateBtnInc == LOW) && (oldStateBtnInc != stateBtnInc)){
        tempChoose = (tempChoose >= 0) ? tempChoose - 1 : tempChoose;
        for (int i = 3; i > tempChoose; i--){
          lcd.setCursor(19, i);
          lcd.print(" ");
        }
        oldStateBtnInc = stateBtnInc;
      }else oldStateBtnInc = stateBtnInc;
      lcd.setCursor(19, tempChoose);
      lcd.write(byte(0));
      if (digitalRead(btnDone) == LOW) {
        switch(tempChoose){
          case 0:
            currentTimeModeScreen = true;
            delay(140);
            break;
          case 3:
            onNormalScreen = true;
            break;   
        }
        break;
      }
    }
}

void CurrentTimeMode(){
  if (currentTimeModeScreen == true){
    lcd.clear();
    lcd.setCursor(0, 3);
    lcd.print("PRESS DONE TO ACCEPT");
    lcd.createChar(0, customCharArrow);
    lcd.setCursor(7, 0);
    lcd.write(byte(0));

    int oldStateBtnDec = digitalRead(btnDec);
    int oldStateBtnInc = digitalRead(btnInc);
   
    int arrow = 7;
    int oldStateBtnChange = digitalRead(btnChangeMode);
    int stateBtnDec = digitalRead(btnDec);
    int stateBtnInc = digitalRead(btnInc);
    
    String tempDataCurrentTime = dataCurrentTime;
    int tempHour = dataHourCurrent;
    int tempMinute = dataMinuteCurrent;
    int tempSecond = dataSecondCurrent;
    
    for (;;){
      lcd.setCursor(6, 1);
      lcd.print(tempDataCurrentTime);
      int stateBtnChange = digitalRead(btnChangeMode);
      int stateBtnDec = digitalRead(btnDec);
      int stateBtnInc = digitalRead(btnInc);
      if ((stateBtnChange == LOW) && (oldStateBtnChange != stateBtnChange)){
        lcd.setCursor(arrow, 0);
        lcd.print(" ");
        arrow = (arrow <= 12) ? arrow += 3 : 7;
        oldStateBtnChange = stateBtnChange;
      }else oldStateBtnChange = stateBtnChange;
      lcd.setCursor(arrow, 0);
      lcd.write(byte(0));
      if ((stateBtnDec == LOW) && (oldStateBtnDec != stateBtnDec)){
        switch(arrow){
          case 7:
            tempHour = (tempHour == 0) ? tempHour : tempHour -= 1;
            tempDataCurrentTime = ValidTime(tempSecond, tempMinute, tempHour);
            break;
          case 10:
            tempMinute = (tempMinute == 0) ? tempMinute : tempMinute -= 1;
            tempDataCurrentTime = ValidTime(tempSecond, tempMinute, tempHour);
            break;
          case 13:
            tempSecond = (tempSecond == 0) ? tempSecond : tempSecond -= 1;
            tempDataCurrentTime = ValidTime(tempSecond, tempMinute, tempHour);
            break;
        }
        oldStateBtnDec = stateBtnDec;
      }else oldStateBtnDec = stateBtnDec;
      
      if ((stateBtnInc == LOW) && (oldStateBtnInc != stateBtnInc)){
        switch(arrow){
          case 7:
            tempHour += 1;
            tempDataCurrentTime = ValidTime(tempSecond, tempMinute, tempHour);
            break;
          case 10:
            tempMinute += 1;
            tempDataCurrentTime = ValidTime(tempSecond, tempMinute, tempHour);
            break;
          case 13:
            tempSecond += 1;
            tempDataCurrentTime = ValidTime(tempSecond, tempMinute, tempHour);
            break;
        }
        oldStateBtnInc = stateBtnInc;
      }else oldStateBtnInc = stateBtnInc;
      if (digitalRead(btnDone) == LOW) {
        onNormalScreen = true;
        currentTimeModeScreen = false;
        dataHourCurrent = tempHour;
        dataMinuteCurrent = tempMinute;
        dataSecondCurrent = tempSecond;
        break;
      }
    }
  }
}

void loop() {
  NormalShowLCD();
  if (digitalRead(btnChangeMode) == LOW) ChangeMode();
  CurrentTimeMode();
}

String ValidTime(int &second, int &minute, int &hour){
  if (second == 60){
    second = 0;
    minute += 1;
  }
  if (minute == 60){
    minute = 0;
    hour += 1;
  }
  if (hour == 24) hour = 0;
  
  String secondData = (String)((second < 10) ? "0" + (String)second : (String)second);
  String minuteData = (String)((minute < 10) ? "0" + (String)minute : (String)minute);
  String hourData = (String)((hour < 10) ? "0" + (String)hour : (String)hour);
  return hourData + ":" + minuteData + ":" + secondData;
}

void printFrame()
{
  lcd.setCursor(1,0);
  lcd.print("------------------");
  lcd.setCursor(1,3);
  lcd.print("------------------");
  lcd.setCursor(0,1);
  lcd.write(byte(0));
  lcd.setCursor(0,2);
  lcd.write(byte(0));
  lcd.setCursor(19,1);
  lcd.write(byte(0));
  lcd.setCursor(19,2);
  lcd.write(byte(0));
  lcd.setCursor(0,0);
  lcd.write(byte(1));
  lcd.setCursor(19,0);
  lcd.write(byte(2));
  lcd.setCursor(0,3);
  lcd.write(byte(3));
  lcd.setCursor(19,3);
  lcd.write(byte(4));
}

void createCustomCharacters()
{
  lcd.createChar(0, verticalLine);
  lcd.createChar(1, char1);
  lcd.createChar(2, char2);
  lcd.createChar(3, char3);
  lcd.createChar(4, char4);
}
