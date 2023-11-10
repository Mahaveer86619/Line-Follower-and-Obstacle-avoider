/*

The Original line detector and Obstacle avoider, 2 in 1, bot with bluetooth wireless feature built into it. 

There are many modes of the robot:
1. line follower
2. obstacle avoider
3. line follower and obstacle avoider
4. remote control through bluetooth (through app)
5. voice control through bluetooth (through app)

*/

#include <Servo.h>

int ir_r = 10;
int ir_l = 11;
int echo = 4;
int trig = 2;
int motorR1 = 7;
int motorR2 =8;
int enR = 6;
int motorL1 = 13;
int motorL2 = 12;
int enL =5; 

Servo servo;

bool state = false;
int mode = 0;

float hitDist = 45;

float duration, distance_L, distance_R, distance_F;

void setup() {
  Serial.begin(9600);

  pinMode(ir_r,INPUT);
  pinMode(ir_l,INPUT);

  pinMode(echo,INPUT);
  pinMode(trig,OUTPUT);

  pinMode(motorR1,OUTPUT);
  pinMode(motorR2,OUTPUT);

  pinMode(motorL1,OUTPUT);
  pinMode(motorL2,OUTPUT);

  pinMode(enR,OUTPUT);
  pinMode(enL,OUTPUT);

  servo.attach(3);

  analogWrite(enR, 250); // doesnot work in the ic
  analogWrite(enL, 250); // doesnot work in the ic
}

void loop() {

  while (Serial.available() > 0) {
    int data = Serial.read();

    if(data == '0'){
      state = false; // running = false
      mode = data;
      stop();
    }

    if(data == '1' || data == '2' || data == '3'){
      state = true; // running = true
      mode = data;
    }

    if (data = 'F') {
      forward();
    } else if (data == 'R') {
      steep_right();
    } else if (data == 'L') {
      steep_left();
    } else if (data == 'B') {
      back();
    }

  }

  if(state == false && mode == '0') {
    stop();
  }
  
  // line detector
  if(state == true && mode == '1') {
      
    lineFollower();

  }

  // obstacle avoider
  if(state == true && mode == '2') {

    obstacleAvoider();

  }

  // checking components
  if(state == true && mode == '3') {
    check_components();
  }

  // hybrid
  if(state == true && mode == '4') {
    hybrid();
  }
}

void check_components() {

  Serial.println("☐☐☐☐☐☐☐☐☐☐☐☐☐☐☐☐☐");
  stop();
  delay(100);
  for (int angle = 90; angle <= 180; angle += 5)  {
   servoPulse(angle);  
  }
  delay(300);
  distance_R = ultrasonic_read();
  Serial.println();
  Serial.print("Distance Right= ");
  Serial.println(distance_R);
  Serial.println();
  delay(100);
  for (int angle = 180; angle >= 0; angle -= 5) {
   servoPulse(angle);  
  }
  delay(500);
  distance_L = ultrasonic_read();
  Serial.println();
  Serial.print("Distance Left= ");
  Serial.println(distance_L);
  Serial.println();
  delay(100);
  for (int angle = 0; angle <= 90; angle += 5) {
   servoPulse(angle);  
  }
  delay(300);
  Serial.println("☐☐☐☐☐☐☐☐☐☐☐☐☐☐☐☐☐");

  forward();
  delay(500);
  steep_right();
  delay(500);
  steep_right();
  delay(500);
  back();
  delay(500);

}

void hybrid() {
  distance_F = ultrasonic_read();

  if (distance_F <= hitDist) {
    stop();
  }

  lineFollower();
}

void obstacleAvoider() {
  distance_F = ultrasonic_read();

  if (distance_F <= hitDist) {
    stop();
    check_side();
  }
  else {
    forward();
  }
}

void lineFollower() {
    if(digitalRead(ir_r)==0 && digitalRead(ir_l)==0){
      forward();
    }

    if(digitalRead(ir_r)==1 && digitalRead(ir_l)==0){
      right();
      delay(80);
    }

    if(digitalRead(ir_r)==0 && digitalRead(ir_l)==1){
      left();
      delay(80);
    }
    
    if(digitalRead(ir_r)==1 && digitalRead(ir_l)==1){
      stop();

      delay(100);
    }
}

// void check_side_wheels(float distance_F) {
//   stop();
//   delay(100);

//   right();
//   delay(300);
//   stop();
//   delay(300);
//   distance_R = ultrasonic_read();
//   Serial.print("Distance Right= ");
//   Serial.println(distance_R);
//   left();
//   delay(600);
//   stop();
//   delay(300);
//   distance_L = ultrasonic_read();
//   Serial.print("Distance Left= ");
//   Serial.println(distance_L);
//   right();
//   delay(300);
//   stop();

//   if(distance_R > distance_L) {
//     steep_right();
//     delay(300);
//   } 
//   if (distance_R < distance_L) {
//     steep_left();
//     delay(300);
//   } 
//   if ((distance_R <= hitDist && distance_L <= hitDist) && distance_F <= hitDist) {
//     steep_right();
//     delay(1000);
//   }
// }

void check_side(){

  Serial.println("☐☐☐☐☐☐☐☐☐☐☐☐☐☐☐☐☐");
  stop();
  delay(100);
  for (int angle = 90; angle <= 180; angle += 5)  {
   servoPulse(angle);  
  }
  delay(300);
  distance_R = ultrasonic_read();
  Serial.println();
  Serial.print("Distance Right= ");
  Serial.println(distance_R);
  Serial.println();
  delay(100);
  for (int angle = 180; angle >= 0; angle -= 5) {
   servoPulse(angle);  
  }
  delay(500);
  distance_L = ultrasonic_read();
  Serial.println();
  Serial.print("Distance Left= ");
  Serial.println(distance_L);
  Serial.println();
  delay(100);
  for (int angle = 0; angle <= 90; angle += 5) {
   servoPulse(angle);  
  }
  delay(300);
  Serial.println("☐☐☐☐☐☐☐☐☐☐☐☐☐☐☐☐☐");

  if(distance_R > distance_L) {
    steep_right();
    delay(350); // right motor is more powerfull fo less delay
  } else if (distance_R < distance_L) {
    steep_left();
    delay(500);
  } else if (distance_R <= hitDist && distance_L <= hitDist){
    steep_left();
    delay(1000);
  } 

}

void servoPulse (int angle){
  servo.write(angle); //rotate arm of servo by given angle
  delay(50); // Refresh cycle of servo
}

float ultrasonic_read(){
  digitalWrite(trig, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trig, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trig, LOW); 

  float duration = pulseIn(echo, HIGH); // recieve signal
  float distance = (duration*.0343)/2;  // convert into cm by myltiplying bt speed of sound and dividing by 2

  return distance;
}

void forward(){
  digitalWrite(motorR1,HIGH);
  digitalWrite(motorR2,LOW);

  digitalWrite(motorL1,HIGH);
  digitalWrite(motorL2,LOW);
}

void right(){
  digitalWrite(motorR1,LOW);
  digitalWrite(motorR2,LOW);

  digitalWrite(motorL1,HIGH);
  digitalWrite(motorL2,LOW);
}

void steep_right() {
  digitalWrite(motorR1,LOW);
  digitalWrite(motorR2,HIGH);

  digitalWrite(motorL1,HIGH);
  digitalWrite(motorL2,LOW);
}

void left(){
  digitalWrite(motorR1,HIGH);
  digitalWrite(motorR2,LOW);

  digitalWrite(motorL1,LOW);
  digitalWrite(motorL2,LOW);
}

void steep_left() {
  digitalWrite(motorR1,HIGH);
  digitalWrite(motorR2,LOW);

  digitalWrite(motorL1,LOW);
  digitalWrite(motorL2,HIGH);
}

void stop(){
    digitalWrite(motorR1,LOW);
    digitalWrite(motorR2,LOW);

    digitalWrite(motorL1,LOW);
    digitalWrite(motorL2,LOW);
}

void back() {
  digitalWrite(motorR1,LOW);
  digitalWrite(motorR2,HIGH);

  digitalWrite(motorL1,LOW);
  digitalWrite(motorL2,HIGH);
}