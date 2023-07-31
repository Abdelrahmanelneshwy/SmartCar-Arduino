#include <SoftwareSerial.h>
#include <Servo.h>


//IR Pins
#define IR_R 9
#define IR_L 8

// ultrasionic pins
#define Echo  6
#define Trigger  7

// Motors Pins
#define IN1_L  14
#define IN1_R  15
#define IN2_L  16
#define IN2_R  17

char msg = 0;
int Mode = 0;
int FrontDist=0, RightDist=0,LeftDist=0;

// Servo init
Servo servo;
int postion = 90;



int distance()
{
   long highPulseDuration;
   int calculatedDistanceCm;

   //Set the trigPin to low, before setting it to high for the pulse
   digitalWrite(Trigger, LOW);
   delayMicroseconds(5);

   // Create the 10 seconds pulse on the trig pin
   digitalWrite(Trigger, HIGH);
   delayMicroseconds(10);

   // Set the pin to low to end the pulse
   digitalWrite(Trigger, LOW);

   // Read the duration of the high pulse on the echo pin
   highPulseDuration = pulseIn(Echo, HIGH);

   // Calculating the distance
   calculatedDistanceCm = (highPulseDuration * 0.034) / 2; // Speed of sound wave divided by 2 (go and back)
  return calculatedDistanceCm;
}


void MoveForword(void)
{
  digitalWrite(IN1_L, LOW);
  digitalWrite(IN2_L, LOW);
  digitalWrite(IN1_R, HIGH);
  digitalWrite(IN2_R, HIGH);
}
void MoveRight(void)
{
  digitalWrite(IN1_L, HIGH);
  digitalWrite(IN2_L, LOW);
  digitalWrite(IN1_R, LOW);
  digitalWrite(IN2_R, HIGH);
}

void MoveLeft(void)
{
  digitalWrite(IN1_L, LOW);
  digitalWrite(IN2_L, HIGH);
  digitalWrite(IN1_R, HIGH);
  digitalWrite(IN2_R, LOW);
}

void MoveBackword(void)
{
  digitalWrite(IN1_L, HIGH);
  digitalWrite(IN2_L, HIGH);
  digitalWrite(IN1_R, LOW);
  digitalWrite(IN2_R, LOW);
}

void Stop(void)
{
  digitalWrite(IN1_L, LOW);
  digitalWrite(IN2_L, LOW);
  digitalWrite(IN1_R, LOW);
  digitalWrite(IN2_R, LOW);
}

void Mode_two(void)
{
  
  FrontDist=distance();
  Serial.println(FrontDist/2);
  if((FrontDist)<=30)
  {
    Stop();
    delay(300);
    MoveBackword();
    delay(300);
    Stop();
    delay(300);
     Serial.println("Check Right");
    servo.write(50);
    delay(300);
    RightDist=distance();
    servo.write(150);
    Serial.println("Check Left");
    delay(300);
    LeftDist=distance();
    
    if(RightDist >= LeftDist)
    {
      Serial.println("Move Right");
      MoveRight();
      delay(200);
      Stop();
      RightDist =0;
      LeftDist=0;
    }
    else {
      Serial.println("Move left");
      MoveLeft();
      delay(200);
      Stop();
      RightDist =0;
      LeftDist=0;
    }
    servo.write(90);
  }
  else{
      delay(300);
    MoveForword();

  }
  Serial.println("Forword");
  FrontDist = 0;
  
}

void Mode_three(void)
{
  if(digitalRead(IR_R) == 0 && digitalRead(IR_L) == 0)
  {
    Stop();
    MoveForword();
    Serial.println("Forword");
  }
  else if(digitalRead(IR_R) ==0 && digitalRead(IR_L) == 1 )
  {
    Stop();
    MoveRight();
    Serial.println("Right");
  }
  else if(digitalRead(IR_R) == 1 && digitalRead(IR_L) == 0)
  {
    Stop();
    MoveLeft();
    Serial.println("Left");
  }
  

}

void setup() {
  // init Serial monitor
  Serial.begin(9600);
  Serial.println("Start");
  // init Motors
  pinMode(IN1_L, OUTPUT);
  pinMode(IN1_R, OUTPUT);
  pinMode(IN2_L, OUTPUT);
  pinMode(IN2_R, OUTPUT);

  digitalWrite(IN1_L, LOW);
  digitalWrite(IN2_L, LOW);
  digitalWrite(IN1_R, LOW);
  digitalWrite(IN2_R, LOW);

  pinMode(3,OUTPUT);
  digitalWrite(3,LOW);
  // init Servo
  servo.attach(4);
  delay(400);
  servo.write(90);
  // init ultrasonic
  pinMode(Trigger, OUTPUT);       // Sets the trigPin as an OUTPUT
  pinMode(Echo, INPUT);          // Sets the echoPin as an INPUT
}

void loop() {
  // put your main code here, to run repeatedly:
    
  if (Serial.available() > 0) {
    //Stop();
    msg = Serial.read();
    delay(25);
    Serial.print(msg);
    Serial.print("\n");
  if(msg == 'F'){
    Serial.print("Foreword \n");
      MoveForword();
  }
   if(msg == 'R'){
    Serial.print("Right \n");
    MoveRight();
  }
  if(msg == 'L'){
    Serial.print("Left \n");
    MoveLeft();
  }
  if(msg == 'G'){
    Serial.print("Back \n");
    MoveBackword();
  }
  if(msg == 'S'){
    Serial.print("stop \n");
    Stop();
  }   
    
  if (msg == 'X' ) 
    {
    Mode =0;
    }
  if (msg == 'Y' ) 
    {
      Mode = 1;
      Serial.print("Mode Two\n");
    }
  if (msg == 'M' ){
      Mode = 2;
      Serial.print("Mode three\n");
    } 
  }

  switch (Mode) {
    case 1 :
      Serial.println("Avoiding obstcals");
      Mode_two();
    break;
    case 2 : 
      Serial.println("Line follower");
      Mode_three();
    break;
    default:
    //Serial.println("NO thing");
    break;
    
  }
}








