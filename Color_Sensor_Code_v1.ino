/*
Color Sensor Calibration from DroneBotWorkshop (Lines #7 - #122)
To Calibrate the color sensor, comment out lines #115 - #279 ; Uncomment Lines #7 - #122
To Get Readings as outputs from the color sensor comment out lines #7 - #122 ; Uncomment Lines #115 - #279
*/

/*

  Color Sensor Calibration
  color-sensor-calib.ino
  Calibrate RGB Color Sensor output Pulse Widths
  Uses values obtained for RGB Sensor Demo sketch 

  DroneBot Workshop 2020
  https://dronebotworkshop.com


// Define color sensor pins

#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// Variables for Color Pulse Width Measurements

int redPW = 0;
int greenPW = 0;
int bluePW = 0;

void setup() {

  // Set S0 - S3 as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Set Sensor output as input
  pinMode(sensorOut, INPUT);
  
  // Set Pulse Width scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  // Setup Serial Monitor
  Serial.begin(9600);
}

void loop() {
  
  // Read Red Pulse Width
  redPW = getRedPW();
  // Delay to stabilize sensor
  delay(200);
  
  // Read Green Pulse Width
  greenPW = getGreenPW();
  // Delay to stabilize sensor
  delay(200);
  
  // Read Blue Pulse Width
  bluePW = getBluePW();
  // Delay to stabilize sensor
  delay(200);
  
  // Print output to Serial Monitor
  Serial.print("Red PW = ");
  Serial.print(redPW);
  Serial.print(" - Green PW = ");
  Serial.print(greenPW);
  Serial.print(" - Blue PW = ");
  Serial.println(bluePW);
  
}


// Function to read Red Pulse Widths
int getRedPW() {

  // Set sensor to read Red only
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}

// Function to read Green Pulse Widths
int getGreenPW() {

  // Set sensor to read Green only
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}

// Function to read Blue Pulse Widths
int getBluePW() {

  // Set sensor to read Blue only
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Library from https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
#include <Servo.h>
// Define color sensor pins
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo; // Servo object

#define S0 4 //
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

/* Calalibration Values
 *  Goto line # - # to see calibration sketch
 */
int redMin = 33; // Red minimum value
int redMax = 230; // Red maximum value
int greenMin = 28; // Green minimum value
int greenMax = 261; // Green maximum value
int blueMin = 24; // Blue minimum value
int blueMax = 181; // Blue maximum value
int pos = 100; // Variable to store the servo position, the "middle" point for us was an angle of 130, therefore it is set as default 
// (4/16/22) Changes Because of a new positon we moved the servo, the new middle point would be an angle of 100

// Color Pulse Width Measurements

int redPW = 0;
int greenPW = 0;
int bluePW = 0;

// Final Stored Values

int redValue;
int greenValue;
int blueValue;

void setup() {
  pinMode(S0, OUTPUT); // S pins as outputs
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(sensorOut, INPUT); //sensorOut as input
  
  digitalWrite(S0,HIGH); // 20% Frequency)
  digitalWrite(S1,LOW);
  
  Serial.begin(9600);

  myservo.attach(9); // Servo is attached to digitalPin 9 on Arduino
}

void loop() {
  
  redPW = getRedPW(); // Read Red value
  redValue = map(redPW, redMin,redMax,255,0); // Map to value from 0-255
  delay(200);// Delay to stabilize sensor
  
  greenPW = getGreenPW();// Read Green value
  greenValue = map(greenPW, greenMin,greenMax,255,0);
  delay(200);
  
  bluePW = getBluePW();// Read Blue value
  blueValue = map(bluePW, blueMin,blueMax,255,0);
  delay(200);
  
  /*
   * Prints out RGB value, in my case, I used these values to debug when I would insert a red checker and it would not read as red, finding the threshhold for the red. 
   * Same process for a black checker
   */
  Serial.print("Red = ");
  Serial.print(redValue);
  Serial.print(" - Green = ");
  Serial.print(greenValue);
  Serial.print(" - Blue = ");
  Serial.print(blueValue);

 
   if ( (redValue > 200 && redValue < 300) && ( greenValue > 50&& greenValue < 150) && (blueValue > 50 && blueValue < 150) ){ // Values fathered from redValue,greenValue,blueValue
    lcd.begin();
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("RED!");
    myservo.write(60); //Ideal position in our case was to change the servo angle from 130 -> 60 to release the red checker
    // (4/16/22) Changes Because of a new positon we moved the servo, the new angle moves fom 100 -> 60
    delay(100);
    Serial.println(" - The Color is Red! "); 
   }

   else if ( redValue < 100 && greenValue < 100 && blueValue < 100){
     lcd.begin();
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("BLACK!");
    myservo.write(150); //Ideal position in our case was to change the servo angle from 130 -> 180 to release the black checker
    // (4/16/22) Changes Because of a new positon we moved the servo, the new servo angle moves from 100 -> 150
    delay(100);
    Serial.println(" - The color is Black! "); 
   }

   else {
    lcd.begin();
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("None");
    myservo.write(pos); //Reset back to default, middle, position
    delay(100);
    Serial.println(" - No color was detected :( " );
   }
  
}


// Function to read Red Pulse Widths
int getRedPW() {

  // Set sensor to read Red only
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}

// Function to read Green Pulse Widths
int getGreenPW() {

  // Set sensor to read Green only
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}

// Function to read Blue Pulse Widths
int getBluePW() {

  // Set sensor to read Blue only
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}
