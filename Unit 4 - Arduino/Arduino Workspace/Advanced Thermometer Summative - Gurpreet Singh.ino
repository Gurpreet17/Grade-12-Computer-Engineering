
#include <Servo.h>
Servo myServo;         //servo motor library
int angle;            //Angle of the servo motor

// Variables for mottor pin
const int motorPin = 5;   //Motor pin (5)
int motorspeed = 0;       //Motor speed


// Variables for temprature Sensor
const int sensorPin = A0; //Sensor Pin (A0)
int sensorVal = 0;        //Stores the sensor value
float voltage = 0;      //Stores current voltage
float temprature = 0;   //Stores current temperature on the sensor

// Variables for Max & Min led
int maxled = 4;           // Orange LED
int minled = 7;           // White LED

// Variables for RGB led
const int greenLed = 9;   //Stores RGB pin for Green
const int redLed = 11;    //Stores RGB pin for Red
const int blueLed = 10;   //Stores RGB pin for Blue
int redVal = 0;           //Stores the RGB colour code for Red
int greenVal = 0;         //Stores the RGB colour code for Green
int blueVal = 0;          //Stores the RGB colour code for Blue

// variables for reset button
const int resetb = 2;     //Stores reset Pin
boolean state = false;    //Stores the state of the reset push-button

//for OP amp
int op = 3;         //Stores the pin of 741 Operational Amplifier (3)

//for automatic
int automatic = 13;     //Stores the pin of Terminal 1 of the Mode slide-switch (13)
int autoState = 0;      //Stores the state value of the termainal 1 of the Mode slide-switch

//For manual
int dips = 12;        //Stores the pin of Fan DIP switch
int dipState = 0;         //Stores the state value of the Fan DIP switch

//initialize variables, pin modes and other libarires & functions
void setup() {
  Serial.begin(9600);
  myServo.attach(6);
  pinMode(minled, OUTPUT);
  pinMode(maxled, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(resetb, INPUT);
  pinMode(automatic, INPUT);
  pinMode(dips, INPUT);
  attachInterrupt(digitalPinToInterrupt(op), turnOn, RISING);
  attachInterrupt(digitalPinToInterrupt(resetb), reset, RISING);
}

//Interupts everytime the reset button is pressed turning off all the LED's & RGB's and rotating the servo motor.
void reset () {
  state = true;
  off ();
  analogWrite(motorPin, 0);
}

//Turns OFF all the LED's and RGB's
void off () {
  digitalWrite(minled, LOW);
  digitalWrite(maxled, LOW);
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, LOW);
}

//Calculates the voltage and temperature values
void turnOn() {
  int sensorVal = analogRead(sensorPin);
  voltage = (sensorVal / 1024.0) * 5.0;//Converting ACD to voltage
  temprature = map(((voltage - 0.5) * 100), -40, 125, -1, 42);//converting voltage to temprature in degrees
  motor(temprature);
}

//Turns the servo motor at a specific angles based on the sensor value.
void tempratureSensor(int temprature, int voltage ) {
  angle = map(temprature, 0, 40, 0, 180);
  myServo.write(angle);//Moving the servo motor

  //Outputs general information on the Serial Monitor
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print("\n");

  Serial.print("Degrees c: ");
  Serial.print(temprature);
  Serial.print("\n");

  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.println("\n");

  delay(500);
}

//Turns on the Max (40°C) and Min LED(0°C)
void MinMax(int temp) {
  if (temp == 0) {
    digitalWrite(minled, HIGH);
    digitalWrite(maxled, LOW);
  } else if (temp == 40) {
    digitalWrite(maxled, HIGH);
    digitalWrite(minled, LOW);
  } else {
    digitalWrite(maxled, LOW);
    digitalWrite(minled, LOW);
  }
}

//Turns on the RGB colour for a specific temperature (Red - 0°C-20°C, Pink - 20°C-25°C and Blue 25°C-40°C)
void RGB_color(int redVal, int greenVal, int blueVal)
{
  analogWrite(redLed, redVal);
  analogWrite(greenLed, greenVal);
  analogWrite(blueLed, blueVal);
}

//Runs the motor everytime the temprature sensor goes above 25°C
void motor(int temp) {
  if (temp > 25) {
    motorspeed = map(temp, 25, 41, 0, 250);
    analogWrite(motorPin, motorspeed);
  }
  else {
    analogWrite(motorPin, 0);
  }
}

//Main loop for the circuit. Runs only when the mode is AUTO
void loop() {
  if (state == true) {            //Checks if the MODE Slide-switch state is high
    off();
    analogWrite(motorPin, 0);
    myServo.write(0);
    delay(1000);
    myServo.write(180);
    delay(1000);
    state = false;
    delay(1000);
    tempratureSensor(temprature, voltage);
  }
  if (digitalRead(automatic) == HIGH) {
    autoState = HIGH;
    sensorVal = analogRead(sensorPin);
    voltage = (sensorVal / 1024.0) * 5.0;        //Converting ACD to voltage
    temprature = map(((voltage - 0.5) * 100), -40, 125, -1, 42);//converting voltage to temprature in degrees
    tempratureSensor(temprature, voltage);
    MinMax(temprature);

    if (temprature < 20) {
      RGB_color(255, 0, 0); // Red
      Serial.println("Heating is on");
      MinMax(temprature);
      analogWrite(motorPin, 0);
      delay(1000);
    } else if (temprature > 25) {
      RGB_color(0, 0, 255); // Blue
      Serial.println("cooling is on");
      MinMax(temprature);
      motor(temprature);
      delay(1000);
    } else {
      RGB_color(255, 0, 255); // Magenta
      analogWrite(motorPin, 0);
      Serial.println("Nothing is on");
    }
    delay(500);
  }else {
    autoState = LOW;
  }

  if (autoState == LOW) {       //Runs if mode is MANUAL
    dipState = digitalRead(dips);
    if (dipState == HIGH) {
      analogWrite(motorPin, 0);
    } else {
      analogWrite(motorPin, 250);
    }
    sensorVal = analogRead(sensorPin);
    voltage = (sensorVal / 1024.0) * 5.0;        //Converting ACD to voltage
    temprature = map(((voltage - 0.5) * 100), -40, 125, -1, 42);//converting voltage to temprature in degrees
    tempratureSensor(temprature, voltage);
    MinMax(temprature);

    if (temprature < 20) {
      RGB_color(255, 0, 0); // Red
      Serial.println("Heating is on");
      MinMax(temprature);
      delay(1000);
    } else if (temprature > 25) {
      RGB_color(0, 0, 255); // Blue
      Serial.println("cooling is on");
      MinMax(temprature);
      delay(1000);
    } else {
      RGB_color(255, 0, 255); // Magenta
      Serial.println("Nothing is on");
    }
    delay(500);
    dipState = digitalRead(dips);
    if (dipState == HIGH) {
      analogWrite(motorPin, 0);
    } else {
      analogWrite(motorPin, 250);
    }
  }


}