/*Please Read "README" Before using this Program
P.S. Needs Servo.h library installed*/
#include <Servo.h>

////////////////////////// Pins Defined for Servo Motors /////////////////////////////
const int pins1 = 3; // Change as needed for angle of s1
const int pins2 = 5; // Change as needed for angle of s2
const int pins3 = 6; // Change as needed for angle of s3

Servo s1;
Servo s2;
Servo s3;

////////////////////////// Function Declarations /////////////////////////////////
float arctan(float temp); // Finds tan inverse in degrees
float arccos(float temp); // Finds cos inverse in degrees
float findsin(float ang); // Finds sin of an angle in degree
float findcos(float ang); // Finds cos of an angle in degree

void findangles1();
void findangles2();
void findangles3();

/////////////////// Variables Defined ////////////////////////
float defangles1 = 0; // Initial state of angle at start
float defangles2 = 90;
float defangles3 = 90;

float angles1;
float angles2;
float angles3;

float cs2; // Stores the value of cos(angles2)
float cs3; // Stores the value of cos(angles3)

float l1=1, l2=1, l3=1; // Length of the rods

float x=1, y=2.7320508076, z=0; // Coordinates of the position to reach
float calcx, calcy, calcz; // Calculated coordinates

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

  float a = ((l1 - y) * (l1 - y)) + (x * x) + (z * z);
  float b = (2 * y * l2) - (2 * (((l2 * l2) + (l3 * l3) - (((l1 - y) * (l1 - y)) + (x * x) + (z * z))) / (2 * l2 * l3)) * y * l3) - (2 * l1 * l2) + (2 * (((l2 * l2) + (l3 * l3) - (((l1 - y) * (l1 - y)) + (x * x) + (z * z))) / (2 * l2 * l3)) * l1 * l3);
  float c = ((((l2 * l2) + (l3 * l3) - (((l1 - y) * (l1 - y)) + (x * x) + (z * z))) / (2 * l2 * l3)) * (((l2 * l2) + (l3 * l3) - (((l1 - y) * (l1 - y)) + (x * x) + (z * z))) / (2 * l2 * l3)) * l3 * l3)- (l3 * l3)+ (l1 * l1) - (2 * y * l1)+(y * y);
  float dis = 0.02;//(b * b) - (4 * a * c); //problem with arduino in calculating discriminant due to its very little value (can try yourself with calculator)
  //I have calculated the discriminant outside of the program with the values of a,b and c given as output of this program
  float sol1 = (-b + sqrt(dis)) / (2 * a);
  float sol2 = (-b - sqrt(dis)) / (2 * a);
  cs2 = sol1; //switch between sol1 and sol2 for accurate results

  findangles1();
  findangles2();
  findangles3();
  

  Serial.print("Your X,Y,Z were: ");
  Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.println(z);
  calcx = (l2 * (findsin(angles2)) - l3 * (findsin(angles2 + angles3))) * (findcos(angles1));
  calcy = -l2 * (findcos(angles2)) + l1 + l3 * (findcos(angles2 + angles3));
  calcz = (l2 * (findsin(angles2)) - l3 * (findsin(angles2 + angles3))) * (findsin(angles1));
  Serial.print("Calculated X,Y,Z by this program are: ");
  Serial.print(calcx); Serial.print(", "); Serial.print(calcy); Serial.print(", "); Serial.println(calcz);

  /////////////////////////////////For Debugging//////////////////////////////
  Serial.print("Calculated a,b,c by this program are: ");
  Serial.print(a,6); Serial.print(", "); Serial.print(b,6); Serial.print(", "); Serial.println(c,6);
  Serial.print("Calculated angles by this program are: ");
  Serial.print(angles1); Serial.print(", "); Serial.print(angles2); Serial.print(", "); Serial.println(angles3);

  if (isnan(calcx) || isnan(calcy) || isnan(calcz) ) Serial.println("ERROR");
  else{
    Serial.println("10 second delay before execution");
    delay(10000);
    s1.write(angles1);
    s2.write(angles2);
    s3.write(angles3);
  }
}

void loop() {
 //abandoned the idea of user input interface due to lack of knowledge and time
}

/////////////////////////////////////////////////////////////////////////////////

float arctan(float temp) {
  float arctanRadians = atan(temp);
  float arctanDegrees = arctanRadians * (180.0 / PI);
  return arctanDegrees;
}

float arccos(float temp) {
  float arccosRadians = acos(temp);
  float arccosDegrees = arccosRadians * (180.0 / PI);
  return arccosDegrees;
}

float findsin(float ang) {
  float anginrad = ang * PI / 180.0;
  return sin(anginrad);
}

float findcos(float ang) {
  float anginrad = ang * PI / 180.0;
  return cos(anginrad);
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
  angles2 = arccos(cs2);
  
}

void findangles3() {
  cs3 = ((l2 * l2) + (l3 * l3) - (((l1 - y) * (l1 - y)) + (x * x) + (z * z))) / (2 * l2 * l3);
  angles3 = arccos(cs3);
}
