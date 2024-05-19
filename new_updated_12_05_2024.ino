// <-------------------- include libraries -------------------->
#include <EEPROM.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

// <-------------------- rtc module data fetched -------------------->
RTC_DS3231 rtc;

// <-------------------- LCD pin configuration -------------------->
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// <-------------------- Store days name in 2D Array -------------------->
char daysOfTheWeek[7][4] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

// <-------------------- Initialize variables to store date & Time -------------------->
int Day;
int Month;
int Year;
int Secs;
int Minutes;
int Hours;
String dofweek;
String myDate;
String myTime;

// <-------------------- Initialize variables with pin no. to compare values & alarm system -------------------->
int buzzer = 13;
int buzzeroffpin = 8;
int setTime1pin = 9;
int setTime2pin = 10;
int setTime3pin = 11;
int buzzeroffvar = 0;
int setTime1var = 0;
int setTime2var = 0;
int setTime3var = 0;

// <-------------------- Initialize variables to store data in EEPROM memory -------------------->
String setTime1 = "";
String setTime2 = "";
String setTime3 = "";
char buffer1[9];  // Buffer is to hold the setTime value as an array of characters
char buffer2[9];
char buffer3[9];
String b1 = "";
String b2 = "";
String b3 = "";



// <-------------------- This section  will be execute only one time & also setup all pinmodes -------------------->
void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(buzzeroffpin, INPUT);
  pinMode(setTime1pin, INPUT);
  pinMode(setTime2pin, INPUT);
  pinMode(setTime3pin, INPUT);
  lcd.begin(16, 2);

  // <-------------------- check if rtc module is available or connected OR not -------------------->
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    // while (1)
      ;
  }
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //please upload the code 2 times - first time uncomment this line & set the time & then please comment this line again & upload code. It is necessary because everytime arduino power on it will set time from begining which is not correct if we do not upload code 2nd time after commented this line
  // rtc.adjust(DateTime(2021, 1, 21, 3, 0, 0));
  Serial.begin(9600);
}

// <-------------------- declaration of functions to setup alarm off & on -------------------->
void callAlarmOn() {
  digitalWrite(13, HIGH);
}

void callAlarmOff() {
  digitalWrite(13, LOW);
}

// <-------------------- This section  will be execute in loop -------------------->
void loop() {
  lcd.clear();
  // <-------------------- storing date & time values in corresponding variables-------------------->
  DateTime now = rtc.now();
  Day = now.day();
  Month = now.month();
  Year = now.year();
  Secs = now.second();
  Hours = now.hour();
  Minutes = now.minute();
  dofweek = daysOfTheWeek[now.dayOfTheWeek()];

  // <-------------------- storing date & time values as string -------------------->
  myDate = dofweek + " " + Day + "/" + Month + "/" + Year;
  myTime = String((Hours < 10 ? "0" : "") + String(Hours)) + ":" + String((Minutes < 10 ? "0" : "") + String(Minutes)) + ":" + String((Secs < 10 ? "0" : "") + String(Secs));

  // <-------------------- display date & time values as string -------------------->
  Serial.println("module time - " + myDate);
  Serial.println("module time - " + myTime);
  lcd.setCursor(0, 0);  //set coloumn, row value to display data in different rows
  lcd.print(myDate);
  lcd.setCursor(0, 1);  //set coloumn, row value to display data in different rows
  lcd.print(myTime);
  delay(100);
  lcd.clear();


  lcd.setCursor(0, 0);
  lcd.print("Now Press one");
  lcd.setCursor(0, 1);
  lcd.print("switch to set time");

  // <-------------------- read status of input pins & store data to variables -------------------->
  buzzeroffvar = digitalRead(buzzeroffpin);
  setTime1var = digitalRead(setTime1pin);
  setTime2var = digitalRead(setTime2pin);
  setTime3var = digitalRead(setTime3pin);

  // <-------------------- set time if 1st or 2nd or 3rd  switch is on -------------------->
  if (setTime1var == HIGH && setTime2var == LOW && setTime3var == LOW) {
    setTime1 = myTime;
    setTime1.toCharArray(buffer1, 9);             // Convert String 'a' to a null-terminated char array
    for (int i = 0; i < setTime1.length(); i++)  // Write each character of the char array to EEPROM
    {
      EEPROM.update(i, buffer1[i]);
    }
    EEPROM.update(setTime1.length(), '\0');  // Add null terminator to the end of the string in EEPROM
    Serial.println("EEPROM memory1 updated !!!!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time 1 set");
    lcd.setCursor(0, 1);
    lcd.print("Successfully");
  }
  
  else if (setTime2var == HIGH && setTime1var == LOW && setTime3var == LOW) {
    setTime2 = myTime;
    setTime2.toCharArray(buffer2, 9);             // Convert String 'a' to a null-terminated char array
    for (int i = 0; i < setTime2.length(); i++)  // Write each character of the char array to EEPROM
    {
      EEPROM.update(11 + i, buffer2[i]);
    }
    EEPROM.update(11 + setTime2.length(), '\0');  // Add null terminator to the end of the string in EEPROM
    Serial.println("EEPROM memory2 updated !!!!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time 2 set");
    lcd.setCursor(0, 1);
    lcd.print("Successfully");
  }

  else if (setTime3var == HIGH && setTime1var == LOW && setTime2var == LOW) {
    setTime3 = myTime;
    setTime3.toCharArray(buffer3, 9);             // Convert String 'a' to a null-terminated char array
    for (int i = 0; i < setTime3.length(); i++)  // Write each character of the char array to EEPROM
    {
      EEPROM.update(22 + i, buffer3[i]);
    }
    EEPROM.update(22 + setTime3.length(), '\0');  // Add null terminator to the end of the string in EEPROM
    Serial.println("EEPROM memory3 updated !!!!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time 3 set");
    lcd.setCursor(0, 1);
    lcd.print("Successfully");
  }

  else if (setTime1var == HIGH && setTime2var == HIGH && setTime3var == HIGH || setTime1var == HIGH && setTime2var == HIGH || setTime2var == HIGH && setTime3var == HIGH || setTime1var == HIGH && setTime3var == HIGH) {
    Serial.println("Do not press more than one timeSet switch while setting time !!!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press button");
    lcd.setCursor(0, 1);
    lcd.print("once a time");
  }
  Serial.println("setTime1 value - " + setTime1);
  Serial.println("setTime2 value - " + setTime2);
  Serial.println("setTime3 value - " + setTime3);

  // <-------------------- Read characters from EEPROM and reconstruct the String -------------------->
  for (int i = 0; i < EEPROM.length(); i++) {
    char character = EEPROM.read(i);
    if (character == '\0') {
      break;  // break loop before it store null value into b string
    }
    b1 += character;
  }

  for (int i = 0; i < EEPROM.length(); i++) {
    char character = EEPROM.read(11 + i);
    if (character == '\0') {
      break;  // break loop before it store null value into b string
    }
    b2 += character;
  }

  for (int i = 0; i < EEPROM.length(); i++) {
    char character = EEPROM.read(22 + i);
    if (character == '\0') {
      break;  // break loop before it store null value into b string
    }
    b3 += character;
  }

  // <-------------------- compare EEPROM value in each loop with current time to check if it's matching or not -------------------->
  Serial.println("b1 status before check if condition- " + b1);
  Serial.println("b2 status before check if condition- " + b2);
  Serial.println("b3 status before check if condition- " + b3);
  
  if (strcmp(myTime.c_str(), b1.c_str()) == 0 || strcmp(myTime.c_str(), b2.c_str()) == 0 || strcmp(myTime.c_str(), b3.c_str()) == 0) {
    Serial.println("inside if block");
    if (buzzeroffvar == 0) {
      callAlarmOn();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Take Your");
      lcd.setCursor(0, 1);
      lcd.print("Medicine");
    }
    else {
      callAlarmOff();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Take Your");
      lcd.setCursor(0, 1);
      lcd.print("Medicine");
    }
  } else {
    Serial.println("inside else block");
    if (buzzeroffvar == 1) {
      callAlarmOff();
    }
  }

  // <-------------------- clearing all strings because every time at the end we need fresh empty string to start next operation in next loop -------------------->
  myDate = "";
  myTime = "";
  setTime1 = "";
  setTime2 = "";
  setTime3 = "";
  b1 = "";
  b2 = "";
  b3 = "";
  Serial.println("b1 status check in the last - " + b1);
  Serial.println("b2 status check in the last - " + b2);
  Serial.println("b3 status check in the last - " + b3);
  Serial.println("");

  delay(500);
}
