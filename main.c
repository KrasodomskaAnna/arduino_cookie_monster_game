#include <Wire.h>
#include <stdlib.h>
#include <time.h>
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Make custom characters:
byte monster1[8] = {
    0b01010,
    0b11111,
    0b10101,
    0b11111,
    0b10001,
    0b11111,
    0b01010,
    0b11011
};
byte monster2[8] = {
    0b11111,
    0b10101,
    0b11111,
    0b10001,
    0b10001,
    0b11111,
    0b01010,
    0b11011
};
byte monster3[8] = {
    0b11111,
    0b10101,
    0b11111,
    0b10001,
    0b11110,
    0b01010,
    0b01010,
    0b11011
};
byte monster4[8] = {
    0b11111,
    0b10101,
    0b11111,
    0b10001,
    0b11110,
    0b01010,
    0b11011,
    0b00000
};

byte cookie[8] = {
	0b00000,
	0b01110,
	0b11101,
	0b10111,
	0b11101,
	0b11011,
	0b01110,
	0b00000
};

const int x_size = 16;
const int y_size = 2;

void setup() {
  // Specify the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // random initialize
  time_t t;
  srand((unsigned int) time(&t));
  
  // Create new characters:
  lcd.createChar(0, monster1);
  lcd.createChar(1, monster2);
  lcd.createChar(2, monster3);
  lcd.createChar(3, monster4);
  lcd.createChar(4, cookie);
  
  // Clears the LCD screen:
  lcd.clear();

  Serial.begin(9600);
  Serial.write("Hello!");
}

void loop() {
  static int x = 0;
  static int y = 1;
  static int anim = 0;

  // cookie logic
  static int x_cookie = 0;
  static int y_cookie = 0;
  static bool show_cookie = true;
  static int score = 0;

  if(!show_cookie) {
    do {
      x_cookie = rand() % x_size;
      y_cookie = rand() % y_size;
    } while(x != x_cookie && y != y_cookie);
    show_cookie = true;
  }

  if(x == x_cookie && y == y_cookie) {
    score++;
    show_cookie = false;
    Serial.write("\nCongratulations! You picked ");
    char score_text[10];
    itoa(score, score_text, 10);
    Serial.write(score_text);
    Serial.write(" cookies!");
  }

  if(show_cookie) {
    lcd.setCursor(x_cookie, y_cookie);
    lcd.write(uint8_t(4));
  }

  // Print all the custom characters:
  lcd.setCursor(x, y);
  delay(150);
  lcd.write(byte(anim));
  anim = ++anim %4;

  if(Serial.available()) {
    delay(100);

    lcd.clear();
    while(Serial.available() > 0) {
      char button = Serial.read();
      switch(button){
        case 'w':
          y = y == 0 ? y_size-1 : --y % y_size;
          break;
        case 's':
          y = ++y % y_size;
          break;

        case 'a':
          x = x == 0 ? x_size-1 : --x % x_size;
          break;
        case 'd':
          x = ++x % x_size;
          break;

        default:
          break;
      }
    }
  }
}