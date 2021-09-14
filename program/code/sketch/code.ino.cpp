#include <Arduino.h>
#line 1 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <pitches.h>
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

byte customCharBell[] = {
  B00000,
  B00100,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000
};

byte customCharQuarterClock[] = {
  B00000,
  B01110,
  B11001,
  B11001,
  B11111,
  B11111,
  B01110,
  B00000
};

int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4 };

String dataCurrentTime;
String oldDataCurrentTime;

bool onNormalScreen = true;
bool oldOnNormalScreen = false;
bool currentTimeModeScreen = false;
bool alarmModeScreen = false;
bool countingModeScreen = false;
int btnChangeMode = 0;
int btnDone = 1;
int btnDec = 2;
int btnInc = 3;
int btnStop = 4;
unsigned long flatSecondTime, oldPressBtn, flagSecondCounting;

//data time current
int dataSecondCurrent = 55;
int dataMinuteCurrent = 30;
int dataHourCurrent = 12;

//data time alarm
int dataMinuteAlarm = 20;
int dataHourAlarm = 6;
int countVoice = 0;
bool alarmFlag = true;
int oldStateStop = HIGH;
bool statusAlarm = false;

//data time counting
int dataMinuteCounting = 0;
int dataSecondCounting = 0;
bool flagCounting = false;

#line 141 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void setup();
#line 152 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void AlarmVoice();
#line 160 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void NormalShowLCD();
#line 191 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void UpdateCurrentTime(bool flag);
#line 206 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void CountDown();
#line 217 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void ChangeMode();
#line 275 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void CurrentTimeMode();
#line 359 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void AlarmMode();
#line 433 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void CountingMode();
#line 516 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void ChangeStatusAlarm();
#line 530 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
String TimeToAlarm(int hourCurrent, int minuteCurrent, int hourAlarm, int minuteAlarm);
#line 548 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void loop();
#line 558 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void VibrationPushButton(int oldStateBtn, int numBtn);
#line 565 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
String ValidTime(int &second, int &minute, int &hour);
#line 582 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void printFrame();
#line 606 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void createCustomCharacters();
#line 141 "D:\\thiet_ke_he_thong_nhung\\btl\\program\\code\\code\\code.ino"
void setup() {
  pinMode(btnChangeMode, INPUT);
  pinMode(btnDone, INPUT);
  pinMode(btnDec, INPUT);
  pinMode(btnInc, INPUT);
  pinMode(btnStop, INPUT);
  flatSecondTime = millis();
  lcd.init();
  lcd.backlight();
}

void AlarmVoice() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);
  }
}


void NormalShowLCD() {
  if (onNormalScreen != oldOnNormalScreen) {
    onNormalScreen = false;
    lcd.clear();
    createCustomCharacters();
    printFrame();
    oldOnNormalScreen = onNormalScreen;
  }
  UpdateCurrentTime(true);
  String minuteData = (String)((dataMinuteAlarm < 10) ? "0" + (String)dataMinuteAlarm : (String)dataMinuteAlarm);
  String hourData = (String)((dataHourAlarm < 10) ? "0" + (String)dataHourAlarm : (String)dataHourAlarm);
  if (statusAlarm){
    lcd.createChar(5, customCharBell);
    lcd.setCursor(2, 2);
    lcd.write(byte(5));
    lcd.setCursor(3, 2);
    lcd.print(hourData + ":" + minuteData); 
    lcd.createChar(6, customCharQuarterClock);
    lcd.setCursor(12, 2);
    lcd.write(byte(6));
    lcd.setCursor(13, 2);
    lcd.print(TimeToAlarm(dataHourCurrent, dataMinuteCurrent, dataHourAlarm, dataMinuteAlarm));
  }
  else{
    lcd.setCursor(1, 2);
    lcd.print("       ");
    lcd.setCursor(13, 2);
    lcd.print("      ");
  }
}

void UpdateCurrentTime(bool flag) {
  if ((unsigned long)(millis() - flatSecondTime) > 1000) {
    flatSecondTime = millis();
    dataSecondCurrent = dataSecondCurrent + 1;
    dataCurrentTime = ValidTime(dataSecondCurrent, dataMinuteCurrent, dataHourCurrent);
    if (flag == true) {
      lcd.setCursor(6, 1);
      lcd.print(dataCurrentTime);
    }
    if ((dataHourCurrent == dataHourAlarm) && (dataMinuteCurrent == dataMinuteAlarm) && (dataSecondCurrent <= 59) && alarmFlag && statusAlarm) {
      AlarmVoice();
    }
  }
}

void CountDown(){
  if ((unsigned long)(millis() - flagSecondCounting) > 1000) {
    flagSecondCounting = millis();
    dataSecondCounting -= 1;
    if (dataSecondCounting < 0){
      dataSecondCounting = 59;
      dataMinuteCounting = (dataMinuteCounting == 0) ? 0 : dataMinuteCounting - 1;
    }
  }
}

void ChangeMode() {
  lcd.clear();
  int tempChoose = 0;
  int oldStateBtnDec = digitalRead(btnDec);
  int oldStateBtnInc = digitalRead(btnInc);
  for (;;) {
    UpdateCurrentTime(false);
    if ((digitalRead(btnStop) == LOW) && (digitalRead(8) == HIGH)) alarmFlag = false;
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
    if ((stateBtnDec == LOW) && (oldStateBtnDec != stateBtnDec)) {
      tempChoose = (tempChoose <= 2) ? tempChoose + 1 : tempChoose;
      for (int i = 0; i < tempChoose; i++) {
        lcd.setCursor(19, i);
        lcd.print(" ");
      }
      oldStateBtnDec = stateBtnDec;
    } else oldStateBtnDec = stateBtnDec;
    if ((stateBtnInc == LOW) && (oldStateBtnInc != stateBtnInc)) {
      tempChoose = (tempChoose > 0) ? tempChoose - 1 : tempChoose;
      for (int i = 3; i > tempChoose; i--) {
        lcd.setCursor(19, i);
        lcd.print(" ");
      }
      oldStateBtnInc = stateBtnInc;
    } else oldStateBtnInc = stateBtnInc;
    lcd.setCursor(19, tempChoose);
    lcd.write(byte(0));
    if (digitalRead(btnDone) == LOW) {
      switch (tempChoose) {
        case 0:
          currentTimeModeScreen = true;
          break;
        case 1:
          countingModeScreen = true;
          break;
        case 2:
          alarmModeScreen = true;
          break;
        case 3:
          onNormalScreen = true;
          break;
      }
      VibrationPushButton(LOW, btnDone);
      break;
    }
  }
}

void CurrentTimeMode() {
  if (currentTimeModeScreen == true) {
    lcd.clear();
    lcd.setCursor(0, 3);
    lcd.print("PRESS DONE TO ACCEPT");
    lcd.createChar(0, customCharArrow);
    lcd.setCursor(7, 0);
    lcd.write(byte(0));

    bool flagBtnDone = true;

    int oldStateBtnDec = digitalRead(btnDec);
    int oldStateBtnInc = digitalRead(btnInc);

    int arrow = 7;
    int oldStateBtnChange = digitalRead(btnChangeMode);

    String tempDataCurrentTime = dataCurrentTime;
    int tempHour = dataHourCurrent;
    int tempMinute = dataMinuteCurrent;
    int tempSecond = dataSecondCurrent;

    for (;;) {
      if ((digitalRead(btnStop) == LOW) && (digitalRead(8) == HIGH)) alarmFlag = false;
      lcd.setCursor(6, 1);
      lcd.print(tempDataCurrentTime);
      int stateBtnChange = digitalRead(btnChangeMode);
      int stateBtnDec = digitalRead(btnDec);
      int stateBtnInc = digitalRead(btnInc);
      if ((stateBtnChange == LOW) && (oldStateBtnChange != stateBtnChange)) {
        lcd.setCursor(arrow, 0);
        lcd.print(" ");
        arrow = (arrow <= 12) ? arrow += 3 : 7;
        oldStateBtnChange = stateBtnChange;
      } else oldStateBtnChange = stateBtnChange;
      lcd.setCursor(arrow, 0);
      lcd.write(byte(0));
      if ((stateBtnDec == LOW) && (oldStateBtnDec != stateBtnDec)) {
        switch (arrow) {
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
      } else oldStateBtnDec = stateBtnDec;

      if ((stateBtnInc == LOW) && (oldStateBtnInc != stateBtnInc)) {
        switch (arrow) {
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
      } else oldStateBtnInc = stateBtnInc;
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

void AlarmMode() {
  if (alarmModeScreen == true) {
    lcd.clear();
    lcd.createChar(0, customCharArrow);
    lcd.setCursor(9, 0);
    lcd.write(byte(0));
    lcd.setCursor(0, 3);
    lcd.print("PRESS DONE TO ACCEPT");
    int arrow = 9;
    int oldStateBtnChange = digitalRead(btnChangeMode);
    int oldStateBtnDec = digitalRead(btnDec);
    int oldStateBtnInc = digitalRead(btnInc);

    int tempHour = dataHourAlarm;
    int tempMinute = dataMinuteAlarm;
    String minuteData = (String)((dataMinuteAlarm < 10) ? "0" + (String)dataMinuteAlarm : (String)dataMinuteAlarm);
    String hourData = (String)((dataHourAlarm < 10) ? "0" + (String)dataHourAlarm : (String)dataHourAlarm);
    for (;;) {
      UpdateCurrentTime(false);
      if ((digitalRead(btnStop) == LOW) && (digitalRead(8) == HIGH)) alarmFlag = false;
      int stateBtnChange = digitalRead(btnChangeMode);
      int stateBtnDec = digitalRead(btnDec);
      int stateBtnInc = digitalRead(btnInc);
      if ((stateBtnChange == LOW) && (oldStateBtnChange != stateBtnChange)) {
        lcd.setCursor(arrow, 0);
        lcd.print(" ");
        arrow = (arrow <= 9) ? arrow += 3 : 9;
        oldStateBtnChange = stateBtnChange;
      } else oldStateBtnChange = stateBtnChange;
      lcd.setCursor(arrow, 0);
      lcd.write(byte(0));
      lcd.setCursor(8, 1);
      lcd.print(hourData + ":" + minuteData);

      if ((stateBtnDec == LOW) && (oldStateBtnDec != stateBtnDec)) {
        switch (arrow) {
          case 9:
            tempHour = (tempHour == 0) ? tempHour : tempHour -= 1;
            hourData = (String)((tempHour < 10) ? "0" + (String)tempHour : (String)tempHour);
            break;
          case 12:
            tempMinute = (tempMinute == 0) ? tempMinute : tempMinute -= 1;
            minuteData = (String)((tempMinute < 10) ? "0" + (String)tempMinute : (String)tempMinute);
            break;
        }
        oldStateBtnDec = stateBtnDec;
      } else oldStateBtnDec = stateBtnDec;
      if ((stateBtnInc == LOW) && (oldStateBtnInc != stateBtnInc)) {
        switch (arrow) {
          case 9:
            tempHour = (tempHour == 23) ? 0 : tempHour + 1;
            hourData = (String)((tempHour < 10) ? "0" + (String)tempHour : (String)tempHour);
            break;
          case 12:
            tempMinute = (tempMinute == 60) ? 0 : tempMinute + 1;
            minuteData = (String)((tempMinute < 10) ? "0" + (String)tempMinute : (String)tempMinute);
            break;
        }
        oldStateBtnInc = stateBtnInc;
      } else oldStateBtnInc = stateBtnInc;
      if (digitalRead(btnDone) == LOW) {
        statusAlarm = true;
        onNormalScreen = true;
        alarmModeScreen = false;
        dataHourAlarm = tempHour;
        dataMinuteAlarm = tempMinute;
        // kiem tra xem luc bat bao thuc co trung vs gio hien tai neu co thi k cho chuong bao thuc keu
        if ((dataHourCurrent == dataHourAlarm) && (dataMinuteCurrent == dataMinuteAlarm)) alarmFlag = false; else alarmFlag = true;
        break;
      }
    }
  }
}

void CountingMode() {
  if (countingModeScreen == true) {
    lcd.clear();
    lcd.createChar(0, customCharArrow);
    lcd.setCursor(9, 0);
    lcd.write(byte(0));
    lcd.setCursor(0, 3);
    lcd.print("PRESS DONE TO ACCEPT");
    int arrow = 9;
    int oldStateBtnChange = digitalRead(btnChangeMode);
    int oldStateBtnDec = digitalRead(btnDec);
    int oldStateBtnInc = digitalRead(btnInc);

    String minuteData = (String)((dataMinuteCounting < 10) ? "0" + (String)dataMinuteCounting : (String)dataMinuteCounting);
    String secondData = (String)((dataSecondCounting < 10) ? "0" + (String)dataSecondCounting : (String)dataSecondCounting);
    for (;;) {
      UpdateCurrentTime(false);
      if (flagCounting){
        CountDown();
        minuteData = (String)((dataMinuteCounting < 10) ? "0" + (String)dataMinuteCounting : (String)dataMinuteCounting);
        secondData = (String)((dataSecondCounting < 10) ? "0" + (String)dataSecondCounting : (String)dataSecondCounting);
        if ((dataMinuteCounting == 0) && (dataSecondCounting == 0)) flagCounting = false;
      }
      int stateBtnChange = digitalRead(btnChangeMode);
      int stateBtnDec = digitalRead(btnDec);
      int stateBtnInc = digitalRead(btnInc);
      if ((stateBtnChange == LOW) && (oldStateBtnChange != stateBtnChange)) {
        lcd.setCursor(arrow, 0);
        lcd.print(" ");
        arrow = (arrow <= 9) ? arrow += 3 : 9;
        oldStateBtnChange = stateBtnChange;
      } else oldStateBtnChange = stateBtnChange;
      if (flagCounting){
        lcd.setCursor(arrow, 0);
        lcd.print("  ");
      }else{
        lcd.setCursor(arrow, 0);
        lcd.write(byte(0)); 
      }
      lcd.setCursor(8, 1);
      lcd.print(minuteData + ":" + secondData);

      if ((stateBtnDec == LOW) && (oldStateBtnDec != stateBtnDec)) {
        switch (arrow) {
          case 9:
            dataMinuteCounting = (dataMinuteCounting == 0) ? dataMinuteCounting : dataMinuteCounting -= 1;
            minuteData = (String)((dataMinuteCounting < 10) ? "0" + (String)dataMinuteCounting : (String)dataMinuteCounting);
            break;
          case 12:
            dataSecondCounting = (dataSecondCounting == 0) ? dataSecondCounting : dataSecondCounting -= 1;
            secondData = (String)((dataSecondCounting < 10) ? "0" + (String)dataSecondCounting : (String)dataSecondCounting);
            break;
        }
        oldStateBtnDec = stateBtnDec;
      } else oldStateBtnDec = stateBtnDec;
      if ((stateBtnInc == LOW) && (oldStateBtnInc != stateBtnInc)) {
        switch (arrow) {
          case 9:
            dataMinuteCounting = (dataMinuteCounting == 99) ? 0 : dataMinuteCounting + 1;
            minuteData = (String)((dataMinuteCounting < 10) ? "0" + (String)dataMinuteCounting : (String)dataMinuteCounting);
            break;
          case 12:
            dataSecondCounting = (dataSecondCounting == 60) ? 0 : dataSecondCounting + 1;
            secondData = (String)((dataSecondCounting < 10) ? "0" + (String)dataSecondCounting : (String)dataSecondCounting);
            break;
        }
        oldStateBtnInc = stateBtnInc;
      } else oldStateBtnInc = stateBtnInc;
      if (digitalRead(btnDone) == LOW) {
        if ((dataSecondCounting != 0) || (dataMinuteCounting != 0)){
          flagCounting = true;
          flagSecondCounting = millis();
        }else{
          statusAlarm = true;
          onNormalScreen = true;
          countingModeScreen = false;
          break; 
        }
      }
    }
  }
}

void ChangeStatusAlarm() {
  int newStateStop = digitalRead(btnStop);
  if (newStateStop != oldStateStop) {
    oldPressBtn = millis();
    oldStateStop = newStateStop;
  }
  if (((millis() - oldPressBtn) > 2000) && (oldPressBtn != 0) && (newStateStop) == LOW) {
    statusAlarm = !statusAlarm;
    // kiem tra xem luc bat bao thuc co trung vs gio hien tai neu co thi k cho chuong bao thuc keu
    if ((dataHourCurrent == dataHourAlarm) && (dataMinuteCurrent == dataMinuteAlarm)) alarmFlag = false; else alarmFlag = true;
    oldPressBtn = 0;
  }
}

String TimeToAlarm(int hourCurrent, int minuteCurrent, int hourAlarm, int minuteAlarm){
  int minute;
  int hour = ((hourAlarm - hourCurrent) < 0) ? 24 + hourAlarm - hourCurrent : hourAlarm - hourCurrent;
  if ((hourAlarm - hourCurrent) == 0){
     if ((minuteAlarm - minuteCurrent) > 0) hour = 0; 
     else if ((minuteAlarm - minuteCurrent) == 0) hour = 24;
     else if (((minuteAlarm - minuteCurrent) < 0)) hour = 23;
  }
  if ((minuteAlarm - minuteCurrent) < 0) {
    minute = 60 + minuteAlarm - minuteCurrent;
    hour -= 1;
  } else minute = minuteAlarm - minuteCurrent;
    
  String dataMinute = (String)((minute < 10) ? "0" + (String)minute : (String)minute);
  String dataHour = (String)((hour < 10) ? "0" + (String)hour : (String)hour);
  return dataHour + "h" + dataMinute + "m";
}

void loop() {
  if (digitalRead(btnChangeMode) == LOW) ChangeMode();
  CurrentTimeMode();
  if ((digitalRead(btnStop) == LOW) && (digitalRead(8) == HIGH)) alarmFlag = false;
  AlarmMode();
  NormalShowLCD();
  CountingMode();
  ChangeStatusAlarm();
}

void VibrationPushButton(int oldStateBtn, int numBtn) {
  for(;;){
    int newStateBtn = digitalRead(numBtn);
    if (newStateBtn != oldStateBtn) break;
  }
}

String ValidTime(int &second, int &minute, int &hour) {
  if (second == 60) {
    second = 0;
    minute += 1;
  }
  if (minute == 60) {
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
  lcd.setCursor(1, 0);
  lcd.print("------------------");
  lcd.setCursor(1, 3);
  lcd.print("------------------");
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.setCursor(0, 2);
  lcd.write(byte(0));
  lcd.setCursor(19, 1);
  lcd.write(byte(0));
  lcd.setCursor(19, 2);
  lcd.write(byte(0));
  lcd.setCursor(0, 0);
  lcd.write(byte(1));
  lcd.setCursor(19, 0);
  lcd.write(byte(2));
  lcd.setCursor(0, 3);  
  lcd.write(byte(3));
  lcd.setCursor(19, 3);
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

