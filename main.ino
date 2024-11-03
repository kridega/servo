/*Please Read "README" Before using this Program
P.S. Needs Servo.h library installed*/
#include <Servo.h>

int Mode = 0;

#define ON 0
#define XYZ 1
#define EXECUTE 2
#define ERROR 3

////////////////////////// Pins Defined for Servo Motors /////////////////////////////
const int pins1 = 3; // Change as needed for angle of s1
const int pins2 = 5; // Change as needed for angle of s2
const int pins3 = 6; // Change as needed for angle of s3

Servo s1;
Servo s2;
Servo s3;

////////////////////////// Function Declarations /////////////////////////////////
double quadsolve(double a, double b, double c, double& sol1, double& sol2); // Solves quadratic equation
double arctan(double temp); // Finds tan inverse in degrees
double arccos(double temp); // Finds cos inverse in degrees
double findsin(double ang); // Finds sin of an angle in degree
double findcos(double ang); // Finds cos of an angle in degree
double mod(double input);

void findangles1();
void findangles2();
void findangles3();

int testcalculation();

int everythingOK();

/////////////////// Variables Defined ////////////////////////
double defangles1 = 0; // Initial state of angle at start
double defangles2 = 90;
double defangles3 = 90;

double angles1;
double angles2;
double angles3;

double cs2; // Stores the value of cos(angles2)
double cs3; // Stores the value of cos(angles3)

double l1, l2, l3; // Length of the rods

double x, y, z; // Coordinates of the position to reach
double calcx, calcy, calcz; // Calculated coordinates

int nerror = 0; // Counts the number of errors occurred

void setup() {
  Serial.begin(9600);
  
  s1.attach(pins1);
  s2.attach(pins2);
  s3.attach(pins3);

  Serial.println("Power On");

  s1.write(defangles1);
  s2.write(defangles2);
  s3.write(defangles3);
}

void loop() {
  Serial.println("Mode Set to ON as Default");
  Serial.println("-----------------------------------------\nSelect Mode (Type 1 to Start) ");
  while (Serial.available() == 0) {} // Wait for input
  Mode = Serial.parseInt();
  
  if (Mode == XYZ) {
    Serial.println("-----------------------------------------");
    
    Serial.print("Enter Value of X coordinate: ");
    while (Serial.available() == 0) {}
    x = Serial.parseFloat();
    
    Serial.print("Enter Value of Y coordinate: ");
    while (Serial.available() == 0) {}
    y = Serial.parseFloat();
    
    Serial.print("Enter Value of Z coordinate: ");
    while (Serial.available() == 0) {}
    z = Serial.parseFloat();

    Serial.println("---------------------------------------");

    Serial.println("Running Tests");
    
    if (everythingOK() == 0) Mode = EXECUTE;
    else {
      Serial.println("An Error Occurred. If You Wish to Overwrite, Type Y. Otherwise, press any key and Enter");
      while (Serial.available() == 0) {}
      char command = Serial.read();
      if (command == 'Y') Mode = EXECUTE;
      else Mode = ERROR;
    }
  } else if (Mode == EXECUTE) {
    findangles1();
    findangles2();
    findangles3();
    Serial.print("Your X,Y,Z were: ");
    Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.println(z);
    Serial.print("Calculated X,Y,Z by this program are: ");
    Serial.print(calcx); Serial.print(", "); Serial.print(calcy); Serial.print(", "); Serial.println(calcz);
    
    Serial.println("Are You SURE You Want To EXECUTE (Y)");
    while (Serial.available() == 0) {}
    char response = Serial.read();
    if (response == 'Y' || response == 'y') {
      s1.write(angles1);
      s2.write(angles2);
      s3.write(angles3);
      Mode = ON;
      Serial.println("---------------------------------\nEXECUTED SUCCESSFULLY");
    } else {
      Mode = ON;
    }
  } else if (Mode == ERROR) {
    Serial.println("An Unknown Error Occurred"); // Can update this field to specify error type
    Serial.print("Exiting with ");
    Serial.print(nerror);
    Serial.println(" Errors");
    Mode = ON;
  }
}

/////////////////////////////////////////////////////////////////////////////////
double quadsolve(double a, double b, double c, double& sol1, double& sol2) {
  double dis = b * b - 4 * a * c;
  if (dis < 0) return -1; // Return error for no real solution
  sol1 = (-b + sqrt(dis)) / (2 * a);
  sol2 = (-b - sqrt(dis)) / (2 * a);
  return 0;
}

double arctan(double temp) {
  double arctanRadians = atan(temp);
  double arctanDegrees = arctanRadians * (180.0 / PI);
  return arctanDegrees;
}

double arccos(double temp) {
  double arccosRadians = acos(temp);
  double arccosDegrees = arccosRadians * (180.0 / PI);
  return arccosDegrees;
}

double findsin(double ang) {
  double anginrad = ang * PI / 180.0;
  return sin(anginrad);
}

double findcos(double ang) {
  double anginrad = ang * PI / 180.0;
  return cos(anginrad);
}

double mod(double input) {
  return input < 0 ? -input : input;
}

void findangles1() {
  if (x > 0 && z > 0) {
    angles1 = arctan(x / z);
  } else if (x < 0 && z > 0) {
    angles1 = 180 + arctan(x / z);
  } else if (x < 0 && z < 0) {
    angles1 = 180 + arctan(x / z);
  } else if (x > 0 && z < 0) {
    angles1 = 360 + arctan(x / z);
  } else {
    nerror++;
  }
}

void findangles2() {
  double a, b, c;
  a = ((l1 - y) * (l1 - y)) + (x * x) + (z * z);
  b = (2 * y * l2) - (2 * cs3 * y * l3) - (2 * l1 * l2) + (2 * cs3 * l1 * l3);
  c = (y * y) + (l1 * l1) - (2 * y * l1) - (l3 * l3) + (cs3 * cs3 * l3 * l3);
  
  double sol1, sol2;
  if (quadsolve(a, b, c, sol1, sol2) == 0) {
    if (sol1 >= -1 && sol1 <= 1) cs2 = sol1;
    else if (sol2 >= -1 && sol2 <= 1) cs2 = sol2;
    else nerror++;
    angles2 = arccos(cs2);
  } else {
    nerror++;
  }
}

void findangles3() {
  cs3 = ((l2 * l2) + (l3 * l3) - (((l1 - y) * (l1 - y)) + (x * x) + (z * z))) / (2 * l2 * l3);
  angles3 = arccos(cs3);
}

int testcalculation() {
  calcx = (l2 * (findsin(angles1)) - l3 * (findsin(angles2 + angles3))) * (findcos(angles1));
  calcy = -l2 * (findcos(angles2)) + l1 + l3 * (findcos(angles2 + angles3));
  calcz = (l2 * (findsin(angles1)) - l3 * (findsin(angles2 + angles3))) * (findsin(angles1));
  
  if (mod(x - calcx) < 0.1 && mod(y - calcy) < 0.1 && mod(z - calcz) < 0.1) return 0;
  else return 1;
}

int everythingOK() {
  findangles1();
  findangles2();
  findangles3();
  return testcalculation();
}
