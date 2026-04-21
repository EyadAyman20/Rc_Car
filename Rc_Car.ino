#include <LiquidCrystal.h>

// ====== Pins ======
#define dir4 12
#define dir3 11
#define en2 8
#define dir2 10
#define dir1 13
#define en1 9

#define buzz A5
#define echo A4
#define trig A3

#define rs 7
#define en 6
#define d4 5
#define d5 4
#define d6 3
#define d7 2

// ====== LCD ======
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// ====== Variables ======
bool is_working = false;
char state;
int speed = 255;

long duration;
int distance;

unsigned long lastBuzz = 0;

// ====== Setup =======
void setup() {
  Serial.begin(9600);

  pinMode(en1 , OUTPUT);
  pinMode(dir1 , OUTPUT);
  pinMode(dir2 , OUTPUT);
  pinMode(en2 , OUTPUT);
  pinMode(dir3 , OUTPUT);
  pinMode(dir4 , OUTPUT);

  pinMode(trig , OUTPUT);
  pinMode(echo , INPUT);
  pinMode(buzz , OUTPUT);

  // LCD
  lcd.begin(16,2);
  delay(50);
  lcd.print("Hello");
  delay(2000);
  lcd.clear();
}

// ====== Ultrasonic ======
int getDistance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  return duration * 0.034 / 2;
}

// ====== Loop ======
void loop() {

  distance = getDistance();
  if(distance != 1193) 
  {
  // LCD display
  lcd.setCursor(0,0);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm   ");
  }
  else 
  {
  lcd.setCursor(0,0);
  lcd.print("Out Of Range");  
  }
  delay (200);
  // ====== Safety ======
  if(distance <= 10) {
    Stop();
    digitalWrite(buzz, HIGH);
  }
  else if(distance <= 8) {
    int interval = map(distance, 5, 8, 100, 500);

    if(millis() - lastBuzz > interval) {
      digitalWrite(buzz, !digitalRead(buzz));
      lastBuzz = millis();
    }
  }
  else {
    digitalWrite(buzz, LOW);
  }

  // ====== Control ======
  if(Serial.available()){
    state = Serial.read();

    switch (state)
    {
      case '1': is_working = true; ON_OFF(true); break;
      case '2': is_working = false; ON_OFF(false); break;

      case '3': digitalWrite(buzz , HIGH); break;
      case '4': digitalWrite(buzz , LOW); break;

      case 'D': if(is_working && distance > 10) Forward(); break;
      case 'U': if(is_working) Backward(); break;
      case 'R': if(is_working) TurnRight(); break;
      case 'L': if(is_working) TurnLeft(); break;

      case 'T': if(is_working){ Forward(); TurnLeft(); } break;
      case 'F': if(is_working){ Forward(); TurnRight(); } break;
      case 'H': if(is_working){ Backward(); TurnLeft(); } break;
      case 'G': if(is_working){ Backward(); TurnRight(); } break;

      default: Stop();
    }
  }
}

// ====== Movement ======
void Forward() {
  digitalWrite(dir1 , HIGH);
  digitalWrite(dir3 , HIGH);
  digitalWrite(dir2 , LOW);
  digitalWrite(dir4 , LOW);
}

void Backward() {
  digitalWrite(dir2 , HIGH);
  digitalWrite(dir4 , HIGH);
  digitalWrite(dir3 , LOW);
  digitalWrite(dir1 , LOW);
}

void TurnRight () {
  digitalWrite(dir2 , HIGH);
  digitalWrite(dir3 , HIGH);
  digitalWrite(dir1 , LOW);
  digitalWrite(dir4 , LOW);
}

void TurnLeft () {
  digitalWrite(dir1 , HIGH);
  digitalWrite(dir4 , HIGH);
  digitalWrite(dir3 , LOW);
  digitalWrite(dir2 , LOW);
}

void Stop() {
  digitalWrite(dir1 , LOW);
  digitalWrite(dir2 , LOW);
  digitalWrite(dir3 , LOW);
  digitalWrite(dir4 , LOW);
}

// ====== Power ======
void ON_OFF(bool state) {
  if (state) {
    analogWrite(en1, 255);
    analogWrite(en2, 255);
  } else {
    digitalWrite(en1, LOW);
    digitalWrite(en2 , LOW);
  }
}