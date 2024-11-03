/*Please Read "README" Before using this Program
P.S. Needs Servo.h library installed*/
int Mode = 0;

#include<Servo.h>

#define ON 0
#define XYZ 1
#define EXECUTE 2
#define ERROR 3

////////////////////////// Pins Defined for Servo Motors/////////////////////////////
const int pins1 = 3; //change as per need for angle of s1
const int pins2 = 5; //change as per need for angle of s2
const int pins3 = 6; //change as per need for angle of s3

Servo s1;
Servo s2;
Servo s3;

////////////////////////// Functions Defined /////////////////////////////////
long quadsolve(); //solves quadratic equation of form ax^2 + bx + c = 0
long arctan(); //finds inverse of tan in degrees
long arccos(); //finds inverse of cos in degrees
long findsin(); //finds sin of an angle in degree
long findcos(); //finds cos of an angle in degree
double mod();

void findangles1(); //finds the value of angle1
void findangles2(); //finds the value of angle2
void findangles3(); //finds the value of angle3

int testcalculation(); //checks the calculation and returns accordingly

int everythingOK(); //checks if everything is OK or not

/////////////////// Variables Defined ////////////////////////
double defangles1 = 0; //Define initial state of angle at start
double defangles2 = 90; //Define initial state of angle at start
double defangles3 = 90; //Define initial state of angle at start

long angles1;
long angles2;
long angles3;

long cs2; //stores the value of cos(angles2)
long cs3; //stores the value of cos(angles3)

double l1,l2,l3; //Length of the rods

double x,y,z; //Coordinates of the position to reach
double calcx,calcy,calcz; //Calculated Coordinates after the calculations are done

int nerror; //counts the number of errors occured

/////////////////////////////////////////////////////////////
void setup(){
  Serial.begin(9600);
  
  s1.attach(pins1);
  s2.attach(pins2);
  s3.attach(pins3);

  Serial.println("Power On");

  s1.write(defangles1);
  s2.write(defangles2);
  s3.write(defangles3);
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

    Serial.println("---------------------------------------");

    Serial.println("Running Tests");
    

    if (everythingOK() == 0) Mode = EXECUTE;
    else {
      Serial.println("An Error Occured\n If You Wish to Overwrite Type Y else press any key and enter");
      char command = Serial.read();
      if (command == "Y") Mode = EXECUTE;
      else Mode = ERROR;
    }
  }
    
  else if (Mode == EXECUTE){
    findangles1();
    findangles2();
    findangles3();
    Serial.println("Your X,Y,Z were %d, %d, %d\nCalculated X,Y,Z by this program is %d, %d, %d",x,y,z,calcx,calcy,calcz);
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
    Serial.println("Exiting With %d Errors",nerror);
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

long arctan(long temp){
  long arctanRadians = atan(temp); // Calculate tan inverse in radians
  long arctanDegrees = arctanRadians * (180.0 / PI); // Convert radians to degrees
  return arctanDegrees;
}

long arccos(long temp){
  long arccosRadians = acos(temp); // Calculate cos inverse in radians
  long arccosDegrees = arccosRadians * (180.0 / PI); // Convert radians to degrees
  return arccosDegrees;
}

long findsin(long ang){
  long anginrad = ang * PI / 180;
  return sin(anginrad);
}

long findcos(long ang){
  long anginrad = ang * PI / 180;
  return cos(anginrad);
}

double mod(double input){
  if (input < 0) input = -input;
  return input;
}

void findangles1(){
  if (x>0 && z>0){
    angles1 = arctan(x/z);
  }
  else if (x<0 && z>0){
    angles1 = 180 + arctan(x/z);
  }
  else if (x<0 && z<0){
    angles1 = 180 + arctan(x/z);
  }
  else if (x>0 && z<0){
    angles1 = 360 + arctan(x/z);
  }
  else nerror+=1;
}

void findangles2(){
  long a,b,c; //variables for the equation of cs2 (refer notes or proof for the same)
  a = ((l1-y)*(l1-y)) + (x*x) + (z*z);
  b = (2*y*l2) - (2*cs3*y*l3) - (2*l1*l2) + (2*cs3*l1*l3);
  c = (y*y) + (l1*l1) - (2*y*l1) - (l3*l3) + (cs3*cs3*l3*l3);
  
  long sol1,sol2;
  sol1,sol2 = quadsolve(a,b,c);
  
  if (sol1 <=0 && sol1>=-1) cs2 = sol1;
  else if (sol2 <=0 && sol2>=-1) cs2 = sol2;
  else nerror+=1;

  angle2 = arccos(cs2);
}

void findangles3(){
  cs3 = ((l2*l2) + (l3*l3) - ( ((l1-y)*(l1-y)) + (x*x) + (z*z) )/ (2*l2*l3)
  angles3 = arccos(cs3); //derived from scratch
}

int testcalculation(){
  calcx = (l2*(findsin(angles1)) - l3*(findsin(angles2+angles3)))*(findcos(angles1));
  calcy = -l2*(findcos(angles2)) + l1 + l3*(findcos(angles2+angles3));
  calcz = (l2*(findsin(angles1)) - l3*(findsin(angles2+angles3)))*(findsin(angles1));
  if (mod(x-calcx)<0.1 && mod(y-calcy)<0.1 && mod(z-calcz)) return 0;
  else return 1;
}

int everythingOK();{
  if (testcalculation() == 0 && nerror == 0) return 0;
  else return 1;
}
