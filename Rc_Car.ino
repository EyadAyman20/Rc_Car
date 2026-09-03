#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

bool is_working = false;
bool increment = true;

// ESP32 38-pin compatible pins (using safe GPIOs)
#define dir4 26   // Motor 2 direction
#define dir3 27   // Motor 1 direction
#define en2 14    // Motor 2 enable (PWM capable)
#define dir2 12   // Motor 2 direction
#define dir1 13   // Motor 1 direction
#define en1 15    // Motor 1 enable (PWM capable)
#define buzz 4    // Buzzer (GPIO4 is safe)

int speed = 0;
char state;
unsigned long lastCommandTime = 0;
const unsigned long timeoutDuration = 5000; // 5 seconds timeout

void Forward();
void Backward();
void TurnRight();
void TurnLeft();
void Horn();
void ON_OFF(bool state);
void speedModifing(bool state);
void Stop();
void emergencyStop();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SerialBT.begin("Eyad Car"); // Bluetooth device name
  
  // Configure pins
  pinMode(en1 , OUTPUT);
  pinMode(dir1 , OUTPUT);
  pinMode(en2 , OUTPUT);
  pinMode(dir2 , OUTPUT);
  pinMode(dir3 , OUTPUT);
  pinMode(dir4 , OUTPUT);
  pinMode(buzz, OUTPUT);
  
  // Initialize with safe state
  digitalWrite(en1, LOW);
  digitalWrite(en2, LOW);
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, LOW);
  digitalWrite(buzz, LOW);
  
  Serial.println("ESP32 38-Pin Bluetooth Car Ready!");
  Serial.println("Device name: ESP32_Car_38pin");
  Serial.println("Connect via Bluetooth and send commands:");
  Serial.println("1=ON, 2=OFF, 3=Speed+, 4=Speed-, U=Forward, D=Backward");
  Serial.println("R=Right, L=Left, S=Stop, H=Horn");
  
  // Initial beep to confirm startup
  tone(buzz, 1000, 200);
  delay(300);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Check for Bluetooth commands
  if(SerialBT.available()){
      state = SerialBT.read();
      lastCommandTime = millis(); // Reset timeout timer
      Serial.print("Command received: ");
      Serial.println(state); // For debugging
      
      switch (state)
  {
    case '1': 
        is_working = true;
        ON_OFF(true);
        break;
        
    case '2': 
        is_working = false;
        ON_OFF(false);
        break;
        
    case '3': 
        speedModifing(is_working);
        break;
        
    case '4': 
        speedModifing(is_working);
        break;
        
    case 'U': 
        if(is_working) {
          Forward();
          SerialBT.println("Moving Forward");
        }
        break;
        
    case 'D':
        if(is_working) {
          Backward();
          SerialBT.println("Moving Backward");
        }
        break;
        
    case 'R':
        if(is_working) {
          TurnRight();
          SerialBT.println("Turning Right");
        }
        break;
        
    case 'L':
        if(is_working) {
          TurnLeft();
          SerialBT.println("Turning Left");
        }
        break;
        
    case 'T': // Forward + Left
        if(is_working) {
          Forward();
          TurnLeft();
          SerialBT.println("Forward Left");
        }
        break;
        
    case 'F': // Forward + Right
        if(is_working) {
          Forward();
          TurnRight();
          SerialBT.println("Forward Right");
        }
        break;
        
    case 'H': // Backward + Left
        if(is_working) {
          Backward();
          TurnLeft();
          SerialBT.println("Backward Left");
        }
        break;
        
    case 'G': // Backward + Right
        if(is_working) {
          Backward();
          TurnRight();
          SerialBT.println("Backward Right");
        }
        break;
        
    case 'S':
        Stop();
        SerialBT.println("Stopped");
        break;
        
    case 'b': // Buzzer/Horn
        if(is_working) Horn();
        break;
        
    default: 
        SerialBT.println("Unknown command");
        break;
  }
  }
  
  // Emergency stop if no command received for timeout period
  if (is_working && (millis() - lastCommandTime > timeoutDuration)) {
    emergencyStop();
    SerialBT.println("Emergency Stop: Timeout");
    is_working = false;
  }
}

void Forward()
{
  digitalWrite(dir1 , HIGH);
  digitalWrite(dir3 , HIGH);
  digitalWrite(dir2 , LOW);
  digitalWrite(dir4 , LOW);
  analogWrite(en1, speed);
  analogWrite(en2, speed);
}

void Backward()
{
  digitalWrite(dir2 , HIGH);
  digitalWrite(dir4 , HIGH);
  digitalWrite(dir3 , LOW);
  digitalWrite(dir1 , LOW);
  analogWrite(en1, speed);
  analogWrite(en2, speed);
}

void TurnRight()
{
  digitalWrite(dir2 , HIGH);
  digitalWrite(dir3 , HIGH);
  digitalWrite(dir1 , LOW);
  digitalWrite(dir4 , LOW);
  analogWrite(en1, speed);
  analogWrite(en2, speed);
}

void TurnLeft()
{
  digitalWrite(dir1 , HIGH);
  digitalWrite(dir4 , HIGH);
  digitalWrite(dir3 , LOW);
  digitalWrite(dir2 , LOW);
  analogWrite(en1, speed);
  analogWrite(en2, speed);
}

void Horn()
{
  digitalWrite(buzz, HIGH);
  delay(200);
  digitalWrite(buzz, LOW);
  delay(100);
  digitalWrite(buzz, HIGH);
  delay(200);
  digitalWrite(buzz, LOW);
}

void speedModifing(bool state)
{
  if(state)
  {
    if (increment)
    {
      speed += 85;
      if (speed >= 255) {
        speed = 255;
        increment = false;
      }
    }
    else
    {
      speed -= 85;
      if (speed <= 0) {
        speed = 0;
        increment = true;
      }
    }
  
    analogWrite(en1, speed);
    analogWrite(en2, speed);
    SerialBT.println("Speed: " + String(speed));
  } else {
    SerialBT.println("Car is OFF, turn on first!");
  }
}

void ON_OFF(bool state)
{
  if (state)
  {
    if (speed == 0) speed = 85; // Start with medium speed if not set
    analogWrite(en1, speed);
    analogWrite(en2, speed);
    SerialBT.println("Car ON - Speed: " + String(speed));
    // Beep to confirm ON
    tone(buzz, 1000, 100);
  }
  else
  {
    digitalWrite(en1, LOW);
    digitalWrite(en2, LOW);
    speed = 0;
    increment = true;
    SerialBT.println("Car OFF");
    // Double beep to confirm OFF
    tone(buzz, 1000, 50);
    delay(100);
    tone(buzz, 1000, 50);
  }
}

void Stop()
{
  digitalWrite(dir1 , LOW);
  digitalWrite(dir2 , LOW);
  digitalWrite(dir3 , LOW);
  digitalWrite(dir4 , LOW);
  analogWrite(en1, 0);
  analogWrite(en2, 0);
}

void emergencyStop()
{
  Stop();
  // Alert with buzzer
  for(int i = 0; i < 3; i++) {
    tone(buzz, 800, 100);
    delay(150);
  }
  digitalWrite(buzz, LOW);
}