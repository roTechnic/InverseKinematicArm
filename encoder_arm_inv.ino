
#include <Encoder.h>
#include <Servo.h>

#define BASESERVOPIN A0
#define ARM1SERVOPIN A1
#define ARM2SERVOPIN A2
#define GRIPSERVOPIN A3

#define GRIPBUTTONPIN 9
#define BASEBUTTONPIN 10
#define ARM1BUTTONPIN 11
#define ARM2BUTTONPIN 12

Encoder myEnc(A6, A7);
Servo arm1servo;
Servo arm2servo;
Servo baseservo;
Servo gripservo;

double x=75;
double y=0;
double z=75;
int gripAngle=67;


long oldPosition=0;

int angleToMicroseconds(double angle) {
  double val = 460.0 + (((2400.0 - 460.0) / 180.0) * angle);
  return (int)val;
}
void moveToAngle(double b, double a1, double a2, double g) {
  arm1servo.writeMicroseconds(angleToMicroseconds(188 - a1));
  arm2servo.writeMicroseconds(angleToMicroseconds(a2+101));
  baseservo.writeMicroseconds(angleToMicroseconds(b+90));
  gripservo.writeMicroseconds(angleToMicroseconds(g));
}

void moveToPos(double x, double y, double z, double g) {
  double b = atan2(y,x) * (180 / 3.1415);

  double l = sqrt(x*x + y*y) - 52; // x and y extension minus the gripper offset

  double h = sqrt (l*l + z*z);

  double theta = atan(z/l) * (180 / 3.1415);

  double phi = acos((h/2)/75) * (180 / 3.1415);
  

  double a1=theta + phi;
  double a2=theta - phi;

  
  //Serial.println((String)x + "," + (String)y + "," + (String)z + "   :   b=" +(String)b + " l=" +(String)l+ " h=" +(String)h+ " theta=" +(String)theta+ " phi=" +(String)phi + "   :   b=" +(String)b + " a1=" +(String)a1+ " a2=" +(String)a2+ " g=" +(String)gripAngle);
  //Serial.println((String)x + "," + (String)y + "," + (String)z);
  moveToAngle(b,a1,a2,g);
  //delay(15);
}

void setup() {
  // put your setup code here, to run once:
  
  baseservo.attach(BASESERVOPIN,460 ,2400);
  arm1servo.attach(ARM1SERVOPIN,460 ,2400);
  arm2servo.attach(ARM2SERVOPIN,460 ,2400);
  gripservo.attach(GRIPSERVOPIN,460 ,2400);
  
  pinMode(BASEBUTTONPIN,INPUT);
  pinMode(ARM1BUTTONPIN,INPUT);
  pinMode(ARM2BUTTONPIN,INPUT);
  pinMode(GRIPBUTTONPIN,INPUT);
  
  Serial.begin(115200);
  
    moveToPos(x,y,z,gripAngle);
}

void loop() {

//for (x=55; x<=130; x++) {
//  moveToPos(x,y,z,gripAngle);
//}
//
//for (x=130; x>55; x--) {
//  moveToPos(x,y,z,gripAngle);
//}


  int amount=0;
  long newPosition = myEnc.read();
  
  if (newPosition != oldPosition) {
    amount=newPosition-oldPosition;
    oldPosition = newPosition;
  }
   
  if (digitalRead(BASEBUTTONPIN) == HIGH && amount!=0) {
    x+=amount;
    moveToPos(x,y,z,gripAngle);
  }

  if (digitalRead(ARM1BUTTONPIN) == HIGH && amount!=0) {
    y+=amount;
    moveToPos(x,y,z,gripAngle);
  }

  if (digitalRead(ARM2BUTTONPIN) == HIGH && amount!=0) {
    z+=amount;
    moveToPos(x,y,z,gripAngle);
  }
  
  if (digitalRead(GRIPBUTTONPIN) == HIGH && amount!=0) {
    gripAngle+=amount;
    moveToPos(x,y,z,gripAngle);
  }

}
