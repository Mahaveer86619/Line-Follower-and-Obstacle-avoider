#include <Servo.h>

Servo servo;

const int trigPin = 2;  
const int echoPin = 4; 

const int enaB = 5;
const int enaA = 6;
const int motorB1 = 7;
const int motorB2 = 8;
const int motorA1 = 12;
const int motorA2 = 13;

const int ir_L = 10;
const int ir_R = 11;

int BTdata;

float duration, distance_L, distance_R, distance_F;  

int space = 15;


void setup() {

  Serial.begin(9600); // start serial communication at 9600bps

  pinMode(trigPin, OUTPUT); // ultra sonic triger (sender)
	pinMode(echoPin, INPUT); // ultra sonic echo (reciever)

  pinMode(enaA, OUTPUT); // to control the speed of motor A
  pinMode(enaB, OUTPUT); // to control the speed of motor B
  pinMode(motorA1, OUTPUT); // to control the motor A's direction
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT); // to control the motor B's direction
  pinMode(motorB2, OUTPUT);

  pinMode(ir_L, INPUT); // declare Left ir sensor as input
  pinMode(ir_R, INPUT); // declare Right ir sensor as input

  analogWrite(enaA, 200); // The Duty Cycle 0 to 255 Enable Pin A for Motor1 Speed 
  analogWrite(enaB, 200); // The Duty Cycle 0 to 255 Enable Pin B for Motor2 Speed 

  servo.attach(3); // servo motor control is in pin ~3

}

void loop() {

  distance_F = Ultrasonic_read();
  Serial.print("D F=");Serial.println(distance_F);
 
 
  //if Right Sensor and Left Sensor are at White color then it will call forword function
  if((digitalRead(ir_R) == 0)&&(digitalRead(ir_L) == 0)){
    if(distance_F > space){
      forword();
    } else {
      Check_side();
    }  
  }  
  
  //if Right Sensor is Black and Left Sensor is White then it will call turn Right function
  else if((digitalRead(ir_R) == 1)&&(digitalRead(ir_L) == 0)){
    turnRight();
  }  
  
  //if Right Sensor is White and Left Sensor is Black then it will call turn Left function
  else if((digitalRead(ir_R) == 0)&&(digitalRead(ir_L) == 1)){
    turnLeft();
  } 
     
  delay(10);

}

void servoPulse (int angle){
  // int pwm = (angle*11) + 500;      // Convert angle to microseconds
  // digitalWrite(pin, HIGH);
  // delayMicroseconds(pwm);
  // digitalWrite(pin, LOW);
  servo.write(angle); //rotate arm of servo by given angle
  delay(50); // Refresh cycle of servo
}

float Ultrasonic_read(){
  digitalWrite(trigPin, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin, LOW); 

  float duration = pulseIn(echoPin, HIGH); // recieve signal
  float distance = (duration*.0343)/2; // convert into cm by myltiplying bt speed of sound and dividing by 2

  return distance;
}

void compareDistance() { // Turns around the obstacle
  if(distance_L > distance_R){
  turnLeft();

  delay(500);

  forword();

  delay(600);

  turnRight();

  delay(500);

  forword();

  delay(600);

  turnRight();

  delay(400);
  } else {
  turnRight();

  delay(500);

  forword();

  delay(600);

  turnLeft();

  delay(500);

  forword();

  delay(600);  

  turnLeft();

  delay(400);
  }
}

void Check_side(){
  Stop();
  delay(100);
  for (int angle = 70; angle <= 140; angle += 5)  {
   servoPulse(angle);  
  }
  delay(300);
  distance_R = Ultrasonic_read();
  Serial.print("Distance Right= ");
  Serial.println(distance_R);
  delay(100);
  for (int angle = 140; angle >= 0; angle -= 5)  {
   servoPulse(angle);  
  }
  delay(500);
  distance_L = Ultrasonic_read();
  Serial.print("Distance Left= ");
  Serial.println(distance_L);
  delay(100);
  for (int angle = 0; angle <= 70; angle += 5)  {
   servoPulse(angle);  
  }
  delay(300);
  compareDistance();
}

void forword(){  //forword
  digitalWrite(motorA1, HIGH); //Left Motor forword Pin 
  digitalWrite(motorA2, LOW); //Left Motor backword Pin 
  digitalWrite(motorB1, HIGH); //Right Motor forword Pin 
  digitalWrite(motorB2, LOW); //Right Motor backword Pin 
}
 
void backword(){ //backword
  digitalWrite(motorA1, LOW); //Left Motor forword Pin 
  digitalWrite(motorA2, HIGH); //Left Motor backword Pin 
  digitalWrite(motorB1, LOW); //Right Motor forword Pin 
  digitalWrite(motorB2, HIGH); //Right Motor backword Pin 
}
 
void turnRight(){ //turnRight
  digitalWrite(motorA1, HIGH); //Left Motor forword Pin
  digitalWrite(motorA2, LOW); //Left Motor backword Pin  
  digitalWrite(motorB1, LOW); //Right Motor forword Pin 
  digitalWrite(motorB2, HIGH); //Right Motor backword Pin 
}
 
void turnLeft(){ //turnLeft
  digitalWrite(motorA1, LOW); //Left Motor forword Pin
  digitalWrite(motorA2, HIGH); //Left Motor backword Pin  
  digitalWrite(motorB1, HIGH); //Right Motor forword Pin 
  digitalWrite(motorB2, LOW); //Right Motor backword Pin 
}
 
void Stop(){ //stop
  digitalWrite(motorA1, LOW); //Left Motor forword Pin 
  digitalWrite(motorA2, LOW); //Left Motor backword Pin 
  digitalWrite(motorB1, LOW); //Right Motor forword Pin 
  digitalWrite(motorB2, LOW); //Right Motor backword Pin 
}
