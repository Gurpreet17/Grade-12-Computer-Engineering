/* 
Author: Gurpreet Singh
Date:2021-04-17
Description: This circuit counts up from 0 to 9 on the 7 segment display
*/
 
int segment_d = 2;
/* Segment_d is an example of a global variable. Any function within the code can access it.Declare the other pin numbers used in the setup function below as variables
here.  Example: instead of pinMode(2, OUTPUT), pinMode(segment_d, OUTPUT was used instead.  Do this for pins 3 - 9
*/
 
byte seven_seg_digits[10][7] = //Array that stores the output of each pin for a specific number to be shown on display
{ 
{ 0,0,0,0,0,0,1 }, //Displays 0
{ 1,0,0,1,1,1,1 }, //Displays 1 
{ 0,0,1,0,0,1,0 }, //Displays 2 
{ 0,0,0,0,1,1,0 }, //Displays 3
{ 1,0,0,1,1,0,0 }, //Displays 4 
{ 0,1,0,0,1,0,0 }, //Displays 5 
{ 0,1,0,0,0,0,0 }, //Displays 6 
{ 1,0,0,0,1,1,1 }, //Displays 7 
{ 0,0,0,0,0,0,0 }, //Displays 8 
{ 1,0,0,0,1,0,0 }  //Displays 9
};
void setup() 
{
Serial.begin(9600); //Speed of the program (bits/seconds)
Serial.println("The program is starting");
//Initializes all segment pins to "OUTPUT"
pinMode(segment_d, OUTPUT);
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
pinMode(6, OUTPUT);
pinMode(7, OUTPUT);
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
//Keeps all the segments off in the beginning of the program (prevents 8 from displaying)
digitalWrite(2,HIGH);
digitalWrite(3,HIGH);
digitalWrite(4,HIGH);
digitalWrite(5,HIGH);
digitalWrite(6,HIGH);
digitalWrite(7,HIGH);
digitalWrite(8,HIGH);
digitalWrite(9,HIGH);
writeDot(1);
}
//This function receives either 0/1 and outputs LOW (0) OR high(1)for DP
void writeDot(byte dot){
digitalWrite(9, dot);
}
//This function recieves a row number and reads each column for that specific row in the seven_seg_digits array
void sevenSegWrite(byte digit) {
byte pin = 2;
//Reads each column of a specific row in order to send HIGH to the appropriate pin
for (byte segCount = 0; segCount < 7; ++segCount) {
digitalWrite(pin, seven_seg_digits[digit][segCount]);
++pin;
}
}
 
void loop() 
{
Serial.println("This is the top of the loop function");
//This for loop keeps track of the rows in the seven_seg_digits array
for (byte count = 0; count < 10; ++count) {
  delay(1000); //Delays for 1000 millisecond 
  sevenSegWrite(count); //Sends count to the sevenSegWrite function
}
delay(4000); //Delays for 4000 milliseconds
} 


