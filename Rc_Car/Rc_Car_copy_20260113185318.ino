bool is_working = false;
#define dir4 2
#define dir3 4
#define en2 5
#define dir2 13
#define dir1 8
#define en1 11
#define buzz 12
int speed = 0;
char state;
void Forward();
void Backward();
void TurnRight();
void TurnLeft();
void Horn();
void ON_OFF();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(en1 , OUTPUT);
  pinMode(dir1 , OUTPUT);
  pinMode(dir2 , OUTPUT);
  pinMode(en2 , OUTPUT);
  pinMode(dir3 , OUTPUT);
  pinMode(dir4 , OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
      state = Serial.read();
      switch (state)
  {
    case '1' : 
        is_working =  true;
        ON_OFF(1);
        break;
    case '2' : 
      is_working =  false;
      ON_OFF(0);
        break;
    case '3' : 
      speedModifing(is_working);
        break;
    case '4' : 
      speedModifing(is_working);
        break;
    case 'U' : 
    if(is_working) Forward();
    break;
    case 'D' :
    if(is_working) Backward();
    break;
    case 'R' :
    if(is_working) TurnRight();
    break;
    case 'L' :
    if(is_working) TurnLeft();
    break;
    case 'T' :
    if(is_working)
    {
    Forward();
    TurnLeft();
    }
    break;
    case 'F' :
    if(is_working)
    {
    Forward();
    TurnRight();
    }
    break;
    case 'H' :
    if(is_working)
    {
    Backward();
    TurnLeft();
    }
    break;
    case 'G' :
    if(is_working) 
    {
      Backward();
      TurnRight();
    }
    break;
    default: Stop();
  }
  }
  
}

void Forward()
{
  digitalWrite(dir1 , HIGH);
  digitalWrite(dir3 , HIGH);
  digitalWrite(dir2 , LOW);
  digitalWrite(dir4 , LOW);
}
void Backward()
{
  digitalWrite(dir2 , HIGH);
  digitalWrite(dir4 , HIGH);
  digitalWrite(dir3 , LOW);
  digitalWrite(dir1 , LOW);
}
void TurnRight ()
{
  digitalWrite(dir2 , HIGH);
  digitalWrite(dir3 , HIGH);
  digitalWrite(dir1 , LOW);
  digitalWrite(dir4 , LOW);
}

void TurnLeft ()
{
  digitalWrite(dir1 , HIGH);
  digitalWrite(dir4 , HIGH);
  digitalWrite(dir3 , LOW);
  digitalWrite(dir2 , LOW);

}
void speedModifing(bool state )
{
  bool increment = true;
  if(state)
  {
    if ( increment)
     {
     speed += 85;
      if (speed == 255)
        {
         increment = false;
        }
     }
    else if (!increment)
      {
        speed-=85;
         if (speed == 0)
            {
             increment = true;
            }
      }
  
    analogWrite(en1,speed);
    analogWrite(en2,speed);
  }
}
void ON_OFF(bool state)
{
  if (state)
  {
    analogWrite(en1, speed);
    analogWrite(en2 , speed);
  }
  else
  {
    digitalWrite(en1, 0);
    digitalWrite(en2 , 0);
  }
}
void Stop()
{
  digitalWrite(dir1 , LOW);
  digitalWrite(dir2 , LOW);
  digitalWrite(dir3 , LOW);
  digitalWrite(dir4 , LOW);
  
}