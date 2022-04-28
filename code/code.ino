// Include the library for LCD1602 Display & I2C Serial Interface Adapter [M1]
#include <Wire.h>
#include <LCD_1602_RUS.h>
#include "LedControl.h"
#include <IRremote.h>
#include <Keypad.h> 

LCD_1602_RUS lcd(0x27, 20, 21); // Устанавливаем дисплей
LedControl led = LedControl(10, 8, 9, 1);
unsigned long delaytime = 250;
int RECV_PIN = 47;
IRrecv irrecv(RECV_PIN);
decode_results results;

//LED drawings
unsigned char questionMark[] = {0x3C, 0x7E, 0x66, 0x0C, 0x18, 0x00, 0x18, 0x18};
unsigned char wrench[] = {0x0C, 0x18, 0x19, 0x1F, 0x3E, 0x70, 0xE0, 0xC0};
unsigned char OK[] = {0xE9, 0xA9, 0xAA, 0xAC, 0xAC, 0xAA, 0xA9, 0xE9};
unsigned char speedClock[] = {0x00, 0x1E, 0x69, 0x29, 0xEF, 0x21, 0x1E, 0x00};

//Remote
unsigned char key_value = 0;
String key = "";
bool settingFlag = true;
bool settingGoing = false;
bool settingDone = false;


// Ultrasonic sensor
int trigPinRight = 39;    //
int echoPinRight = 41;    //

// 2 Ultrasonic sensor
int trigPinLeft = 43;    //
int echoPinLeft = 45;    //
long duration, cm, inches;

//Keypad
const byte ROWS = 4; // Количество рядов
const byte COLS = 4; // Количество строк

char keys[ROWS][COLS] =
{
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};

byte rowPins[ROWS] = {37, 35, 33, 31}; // Выводы, подключение к строкам
byte colPins[COLS] = {29, 27, 25, 23}; // Выводы, подключение к столбцам

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup() {
  Serial.begin(9600); // initialize the serial port:

  //Датчик пульта
  irrecv.enableIRIn();

  //LCD 1602
  lcd.init();
  lcd.backlight();// Включаем подсветку дисплея

  lcd.setCursor(0, 0);
  lcd.clear();

  //LED
  led.shutdown(0, false);
  /* Set the brightness to a medium values */
  led.setIntensity(0, 8);
  /* and clear the display */
  led.clearDisplay(0);

  //Ultrasonic sensor
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);

  // Left ultrasonic sensor
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);
  delay(1000);
}

bool printedDone = false;
int settingStartTime = 0;
int settingEndTime = 0;
String enteredSpeed = "";

void loop() {
  if (millis() - settingStartTime >= 5000 && !settingDone && settingGoing) {//for testing
    settingGoing = false;
    settingDone = true;
  }

  if (settingFlag) {
    printSetting();
    settingFlag = false;
  }

  if (getRemoteSignal() == "OK" && !settingGoing && !settingDone) {
    settingGoing = true;
    settingStartTime = millis();
    clearDisplays();
    printSettingOngoing();
  }

  if (settingGoing) {
    long distL = getDistanceLeft();
    long distR = getDistanceRight();
    lcd.setCursor(0, 1);
    //lcd.print("Лв:" + String(distL) + " Пр:" + String(distR));
    int timeSeconds = floor((millis() - settingStartTime) / 1000);
    //lcd.print("Время: " + String(timeSeconds) + "сек");
    lcd.setCursor(7, 1);
    lcd.print(timeSeconds);
    lcd.setCursor(9, 1);
    lcd.print("сек");
  }

  if (settingDone && !printedDone) {
    clearDisplays();
    printSettingDone();
    printedDone = true;
    settingEndTime = millis();
    //возвращение к начальному положению
  }

  if (millis() - settingEndTime >= 2000 && settingDone) {//когда вернулись к начальному положению после настройки. пока что, для тестирования, просто ждём 2сек
    String input = getRemoteSignal();
    if (isNumber(input)) {
      enteredSpeed += input;
    }
    printSpeedQuestion(inputToSpeed(enteredSpeed));
  }
}

//led.setChar(0,0,'a',false);
bool isNumber(String numberString) {
  if (numberString == "0" || numberString == "1" || numberString == "2" || 
      numberString == "3" || numberString == "4" || numberString == "5" || 
      numberString == "6" || numberString == "7" || numberString == "8" ||
      numberString == "9") {
        return true;
      }
  return false;
}

String getKeypadSignal() { //needs testing
  char key = keypad.getKey();
  String stringKey = "";
  if (key) {
    stringKey = String(key);
  }
  return stringKey;
}

String getRemoteSignal() {
  if (irrecv.decode(&results)) {
    
    if (results.value == 0XFFFFFFF)
      results.value = key_value;
    key = "";

    switch (results.value) {
      case 0xFF629D://up
        Serial.println("UP");
        key = "UP";
        break;
      case 0xFF22DD://left
        Serial.println("LEFT");
        key = "LEFT";
        break;
      case 0xFF02FD://ok
        Serial.println("OK");
        key = "OK";
        break ;
      case 0xFFC23D://right
        Serial.println("RIGHT");
        key = "RIGHT";
        break ;
      case 0xFFA857://down
        Serial.println("DOWN");
        key = "DOWN";
        break ;
      case 0xFF6897: //1
        Serial.println("1");
        key = "1";
        break ;
      case 0xFF9867://2
        Serial.println("2");
        key = "2";
        break ;
      case 0xFFB04F://3
        Serial.println("3");
        key = "3";
        break ;
      case 0xFF30CF://4
        Serial.println("4");
        key = "4";
        break ;
      case 0xFF18E7://5
        Serial.println("5");
        key = "5";
        break ;
      case 0xFF7A85://6
        Serial.println("6");
        key = "6";
        break ;
      case 0xFF10EF://7
        Serial.println("7");
        key = "7";
        break ;
      case 0xFF38C7://8
        Serial.println("8");
        key = "8";
        break ;
      case 0xFF5AA5://9
        Serial.println("9");
        key = "9";
        break ;
      case 0xFF42BD://*
        Serial.println("*");
        key = "*";
        break ;
      case 0xFF4AB5://0
        Serial.println("0");
        key = "0";
        break ;
      case 0xFF52AD://#
        Serial.println("#");
        key = "#";
        break ;
    }
    key_value = results.value;
    irrecv.resume();
  }
  return key;
}

void drawLED(char *byteArray) {
  for (int i = 0; i < 8; i++) {
    led.setRow(0, i, byteArray[i]);
  }
}

void clearDisplays() {
  lcd.clear();
  led.clearDisplay(0);
}

void printSettingOngoing() {
  lcd.setCursor(0, 0);
  lcd.print("Идёт настройка..");
  drawLED(wrench);
}

void printSetting() {
  lcd.setCursor(0, 0);
  lcd.print("Нужна настройка");
  lcd.setCursor(0, 1);
  lcd.print("Начать? [A] [OK]");
  drawLED(questionMark);
}

void printSettingDone() {
  lcd.setCursor(0, 0);
  lcd.print("НАСТРОЙКА");
  lcd.setCursor(0, 1);
  lcd.print("ОКОНЧЕНА");
  drawLED(OK);
}

void printRemoteSignal() {
  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX); //вывод результатов в монитор порта
    irrecv.resume(); // прием следующего значения
  }
}

long getDistanceLeft() {
  // датчик запускается импульсом HIGH продолжительностью 10 мкс
  // или более; но перед этим нужно подать короткий импульс LOW,
  // чтобы Trig в конце концов получил четкий импульс HIGH:
  digitalWrite(trigPinLeft, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPinLeft, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinLeft, LOW);

  // считываем сигнал от датчика; продолжительность импульса HIGH –
  // это время, прошедшее от отправки импульса
  // до получения эха от объекта:
  pinMode(echoPinLeft, INPUT);
  duration = pulseIn(echoPinLeft, HIGH);
  // конвертируем время в расстояние:
  cm = (duration / 2) / 29.1;
  Serial.print(cm);
  Serial.print("cm");  //  "сантиметров"
  Serial.println();

  return cm;
}

long getDistanceRight() {
  digitalWrite(trigPinRight, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPinRight, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinRight, LOW);

  pinMode(echoPinRight, INPUT);
  duration = pulseIn(echoPinRight, HIGH);
  cm = (duration / 2) / 29.1;
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  return cm;
}

void printSpeedQuestion(float speedMSec) {
  float sliderSpeed = speedMSec;
  //String speedString = String(float(sliderSpeed));
  lcd.setCursor(0, 0);
  lcd.print("Введите скорость");
  lcd.setCursor(0, 1);
  lcd.print(sliderSpeed);
  lcd.setCursor(4, 1);
  lcd.print("м/c [A][OK]");
  drawLED(speedClock);
}

float inputToSpeed(String keyInputs) {
  float transformedFloat = 0.00;
  if (keyInputs == "")
    return transformedFloat;
    
  for (int i = 0; i < 3; i++) {
    int inputLength = min(3, keyInputs.length());
    String oneInput = String(keyInputs[keyInputs.length()-(inputLength-i)]); //три последних числа в наборе цифер
    //String oneInput(1, keyInputs[keyInputs.length()-(3-i)]);
    if (i == 0) {
      transformedFloat += atof(oneInput.c_str()) / 100.0; 
    }
    else if (i == 1) {
      transformedFloat += atof(oneInput.c_str()) / 10.0;
    }
    else if (i > 1) {
      transformedFloat += atof(oneInput.c_str()) / 1.0;
    }
  }
  return transformedFloat;
}
