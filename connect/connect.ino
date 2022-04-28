// arduino modules list, 
//Stepper motor control module                                       || include => ✓, setup => ✓, loop=> X
//Angle sensor                                                       || include => ✓, setup => ✓, loop=> X
//Ultrasonic Sensor HC-SR04                                          || include => ✓, setup => ✓, loop=> X
//LCD1602 Display & I2C Serial Interface Adapter [M1]                || include => ✓, setup => ✓, loop=> ✓
//Membrane keyboard 4x4                                              || include => ✓, setup => X, loop=> X
// MAX7219 Dot led matrix MCU control LED Display module [M2]        || include => ✓, setup => ✓, loop=> X
//Infrared IR Wireless Remote Control [IR]                           || include => ✓, setup => ✓, loop=> X


// Include the Arduino Stepper Library
#include <Stepper.h>
const int stepsPerRevolution = 200; // Number of steps per output rotation
Stepper myStepper(stepsPerRevolution, 4, 5, 6, 7); // Create Instance of Stepper library


// Include the Library for LCD1602 Display & I2C Serial Interface Adapter [M1]
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,21);  // Устанавливаем дисплей


//Angle sensor
#define CLK 11
#define DT 12
#define SW 13
#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);  // для работы c кнопкой
// Варианты инициализации:
// Encoder enc;                  // не привязан к пину
// Encoder enc(пин CLK, пин DT);        // энкодер без кнопки (ускоренный опрос)
// Encoder enc(пин CLK, пин DT, пин SW);    // энкодер с кнопкой
// Encoder enc(пин CLK, пин DT, пин SW, тип);  // энкодер с кнопкой и указанием типа
// Encoder enc(пин CLK, пин DT, ENC_NO_BUTTON, тип);  // энкодер без кнопкой и с указанием типа


//#include Library for Membrane keyboard 4x4
#include <Keypad.h>
const byte ROWS = 4;   // rows number
const byte COLS = 4;   // cols number
char keys[ROWS][COLS] = 
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {23, 25, 27, 29}; // Выводы, подключение к строкам
byte colPins[COLS] = {31, 33, 35, 37}; // Выводы, подключение к столбцам


//MAX7219 Dot led matrix MCU control LED Display module [M2]
#include <LedControl.h>
int DIN = 10;
int CS =  9;
int CLK = 8;
byte e[8] =     {0x7C, 0x7C, 0x60, 0x7C, 0x7C, 0x60, 0x7C, 0x7C};
byte d[8] =     {0x78, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x78};
byte u[8] =     {0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7E, 0x7E};
byte c[8] =     {0x7E, 0x7E, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x7E};
byte eight[8] = {0x7E, 0x7E, 0x66, 0x7E, 0x7E, 0x66, 0x7E, 0x7E};
byte s[8] =     {0x7E, 0x7C, 0x60, 0x7C, 0x3E, 0x06, 0x3E, 0x7E};
byte dot[8] =   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18};
byte o[8] =     {0x7E, 0x7E, 0x66, 0x66, 0x66, 0x66, 0x7E, 0x7E};
byte m[8] =     {0xE7, 0xFF, 0xFF, 0xDB, 0xDB, 0xDB, 0xC3, 0xC3};

LedControl lc = LedControl(DIN, CLK, CS, 0);


//Infrared IR Wireless Remote Control [IR]
#include <IRremote.h>
int RECV_PIN = 47;
IRrecv irrecv(RECV_PIN);
decode_results results;


// Ultrasonic sensor
int trigPinRight = 39;    //
int echoPinRight = 41;    //


// 2 Ultrasonic sensor
int trigPinLeft = 43;    //
int echoPinLeft = 45;    //
long duration, cm, inches;

void setup() {
  Serial.begin(9600); // initialize the serial port:

  
  //LCD 1602
  lcd.begin();                     
  lcd.backlight();// Включаем подсветку дисплея
  lcd.print("iarduino.ru");
  lcd.setCursor(8, 1);
  lcd.print("LCD 1602");

  
  //Ultrasonic sensor
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);
  // Left ultrasonic sensor
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);

  
  //Angle sensor
  enc1.setType(TYPE2);
  
  
  //Arduino Stepper --------------
  myStepper.setSpeed(60);// set the speed at 60 rpm:


  
  //MAX7219 Dot led matrix MCU control LED Display module [M2] -----------
  lc.shutdown(0, false);      //The MAX72XX is in power-saving mode on startup
  lc.setIntensity(0, 15);     // Set the brightness to maximum value
  lc.clearDisplay(0);         // and clear the display


  
  //Infrared IR Wireless Remote Control [IR] ---------
  irrecv.enableIRIn();//receiver initialization
}

void loop() {
  //LCD1602
  // Устанавливаем курсор на вторую строку и нулевой символ.
  lcd.setCursor(0, 1);
  // Выводим на экран количество секунд с момента запуска ардуины
  lcd.print(millis()/1000);




  
 }

}
