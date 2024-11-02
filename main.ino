/*Please Read "README" Before using this Program
P.S. Needs Servo.h library installed*/
int Mode = 0;

#include<Servo.h>

#define ON 0
#define XYZ 1
#define EXECUTE 2
#define ERROR 3

////////////////////////// Pins Defined for Servo Motors/////////////////////////////
const int pins1 = 3; //change as per need
const int pins2 = 5; //change as per need
const int pins3 = 6; //change as per need

Servo s1;
Servo s2;
Servo s3;

////////////////////////// Functions Defined /////////////////////////////////
long quadsolve(); //solves quadratic equation of form ax^2 + bx + c = 0

long findx(); //finds the value of X coordinate
long findy(); //finds the value of X coordinate
long findz(); //finds the value of X coordinate

int everythingOK(); //checks if everything is OK or not

/////////////////// Variables Defined ////////////////////////
double defangles1 = 0; //Define initial state angle at start
double defangles2 = 90; //Define initial state angle at start
double defangles3 = 90; //Define initial state angle at start

long angles1;
long angles2;
long angles3;

double l1,l2,l3; //Length of the rods

double x,y,z; //Coordinates of the position to reach


/////////////////////////////////////////////////////////////
void setup(){
  Serial.begin(9600);
  
  s1.attach(pins1);
  s2.attach(pins2);
  s3.attach(pins3);

  Serial.println("Power On");
}

void loop(){
  Serial.println("Mode Set to ON as Default");
  Serial.println("-----------------------------------------\nSelect Mode (Type 1 to Start) ");
  Mode = Serial.read();
  
  if (Mode == XYZ){
    Serial.println("-----------------------------------------");
    
    Serial.print("Enter Value of X coordinate ");
    x = Serial.read();
    Serial.print("Enter Value of Y coordinate ");
    y = Serial.read();
    Serial.print("Enter Value of Z coordinate ");
    z = Serial.read();

    if (everythingOK == 0) Mode = EXECUTE;
    else Mode = ERROR;
  }
    
  else if (Mode == EXECUTE){
    Serial.println("Are You SURE You Want To EXECUTE (Y)");
    char response = Serial.read();
    if (response == "Y" || response == "y" || response == "YES" || response == "yes" || response == "Yes"){
      s1.write(angles1);
      s2.write(angles2);
      s3.write(angles3);
      Mode = ON;
      Serial.println("---------------------------------\nEXECUTED SUCCESFULLY");
    }
    else Mode = ON;
  }
    
  else if (Mode == ERROR){
    Serial.println("An Unknown Error Occured"); //Can update this field to find out the type of error
    Mode = ON;
  }
}

/////////////////////////////////////////////////////////////////////////////////
long quadsolve(long a,long b, long c){
  long dis = b*b-4*a*c;
  long sol1 = (-b + sqrt(dis))/(2*a);
  long sol2 = (-b - sqrt(dis))/(2*a);
  return sol1,sol2;
}
