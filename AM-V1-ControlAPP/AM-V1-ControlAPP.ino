/*
 Name:		AM_V1_ControlAPP.ino
 Created:	4/13/2020 3:43:04 PM
 Author:	Valmark
*/

// Cycle 1 Library
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <stdio.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();

// Common
const int buttonMainPin = 10;  // Main control button
bool flag_menu = true;
String app_type = "BETA";
String app_version = "v0.1";


// Cycle 1 const
const int buttonPin = 10;     // button 2 input
const int mainledPin = 40;      // Main LED
const int blue1led = 28;
const int blue2led = 32;
const int blue3led = 36;
const int red1led = 44;
const int red2led = 48;
const int red3led = 53;

//// Cycle 1 variebles
String com = "";
bool flag = false;

//// Input convert varieble
int x = 0;
int y = 0;
int incomingByte = 0;
int secondIncomingByte = 0;

//// Cycle 2 variebles
int buttonState = 0;  // button 2 state
int button_mode = 0;
int buttonMain = 0;

//// arduino main function
void setup() {
    Serial.begin(9600);
    while (!Serial) delay(100);
    Serial.print("Valmark AM-V1 Control Panel "); Serial.print(app_type + " "); Serial.println(app_version); // Top of page
    pinMode(buttonMain, INPUT);
    buttonMain = digitalRead(buttonMainPin);
    pinMode(buttonPin, INPUT);
    // Cycle 2 Setup
    pinMode(blue1led, OUTPUT); // Blue 1 LED
    pinMode(blue2led, OUTPUT); // Blue 2 LED
    pinMode(blue3led, OUTPUT); // Blue 3 LED
    pinMode(mainledPin, OUTPUT); // Green LED
    pinMode(red1led, OUTPUT); // Red 1 LED
    pinMode(red2led, OUTPUT); // Red 2 LED
    pinMode(red3led, OUTPUT); // Red 3 LED
    // Cycle 1 Setup
    if (!sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
        Serial.println("Couldn't find SHT31");
    }
}

void loop() {
    buttonMain = digitalRead(buttonMainPin);
    if (buttonMain == HIGH) {
        while (1) {
            Serial.print("Input command: ");
            com = inputString();
            if (com.indexOf("test") >= 0) {
                if(test()) Serial.println("Test complete");
            }
            else if (com.indexOf("led") >= 0) {
                ledControl();
            }
            else if (com.indexOf("temperature") >= 0 | com.indexOf("temp") >= 0) {
                temperature();
            }
            else Serial.println("Wrong command, try again...");
            Serial.println();
        }
    }
    else if (buttonMain == LOW) {
        migalka();
    }
    blueOff();
}

// Mode function
bool test() {
    Serial.println("TEST MODE ACTIVE");
    if (!sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
        Serial.println("Caution! Couldn't find SHT31!");
    }
    // Led
    Serial.print("Led test: ");
    digitalWrite(blue1led, HIGH);
    delay(200);
    digitalWrite(blue2led, HIGH);
    delay(200);
    digitalWrite(blue3led, HIGH);
    delay(200);
    digitalWrite(mainledPin, HIGH);
    delay(200);
    digitalWrite(red1led, HIGH);
    delay(200);
    digitalWrite(red2led, HIGH);
    delay(200);
    digitalWrite(red3led, HIGH);
    delay(200);
    redOff();
    delay(200);
    greenOff();
    delay(200);
    blueOff();
    delay(500);
    digitalWrite(blue1led, HIGH);
    digitalWrite(blue3led, HIGH);
    digitalWrite(red1led, HIGH);
    digitalWrite(red3led, HIGH);
    delay(200);
    blueOff();
    redOff();
    digitalWrite(blue2led, HIGH);
    digitalWrite(red2led, HIGH);
    delay(200);
    blueOff();
    redOff();
    delay(500);
    digitalWrite(blue1led, HIGH);
    digitalWrite(blue3led, HIGH);
    digitalWrite(red1led, HIGH);
    digitalWrite(red3led, HIGH);
    delay(200);
    blueOff();
    redOff();
    digitalWrite(blue2led, HIGH);
    digitalWrite(red2led, HIGH);
    delay(200);
    blueOff();
    redOff();
    delay(500);
    Serial.print("OK");
    // temp
    float t = sht31.readTemperature();
    float h = sht31.readHumidity();
    Serial.println(); Serial.print("Temperature module test: ");
    if (!isnan(t)) Serial.print("PASS");
    else Serial.print("Fail");
    Serial.println(); Serial.print("Humidity module test: ");
    if (!isnan(h)) Serial.print("PASS");
    else Serial.print("Fail");
    delay(200);
    // error indicate
    Serial.println(); Serial.print("Error indicate: ");
    errorLed();
    Serial.print("OK");
    // button indicate
    Serial.println(); Serial.print("Button position: ");
    buttonMain = digitalRead(buttonMainPin);
    if (buttonMain == HIGH) Serial.print("ON");
    else Serial.print("OFF");
    // input test
    Serial.println(); Serial.print("Input number: ");
    input();
    Serial.println(); Serial.print("Input string: ");
    inputString();
    // Finish
    return true;
}
void ledControl() {
    while (1) {
        Serial.println();
        Serial.print("Please type led command number: ");
        switch (input())
        {
        case 1: digitalWrite(blue1led, HIGH); break;
        case 2: digitalWrite(blue2led, HIGH); break;
        case 3: digitalWrite(blue3led, HIGH); break;
        case 4: greenOn(); break;
        case 5: digitalWrite(red1led, HIGH); break;
        case 6: digitalWrite(red2led, HIGH); break;
        case 7: digitalWrite(red3led, HIGH); break;
        case 11: {
            digitalWrite(blue1led, HIGH);
            digitalWrite(red3led, HIGH);
            break;
        }
        case 22: {
            digitalWrite(blue2led, HIGH);
            digitalWrite(red2led, HIGH);
            break;
        }
        case 33: {
            digitalWrite(blue3led, HIGH);
            digitalWrite(red1led, HIGH);
            break;
        }
        case 123: redOn(); greenOn(); blueOn(); break;
        case 0: redOff(); greenOff(); blueOff(); break;
        default:
            errorLed();
            break;
        }
        buttonMain = digitalRead(buttonMainPin);
        if (buttonMain == LOW) break;
    }
}
void temperature() {
    float t = sht31.readTemperature();
    float h = sht31.readHumidity();
    if (!isnan(t)) {  // check if 'is not a number'
        Serial.print("Temperature *C = "); Serial.println(t, DEC);
        blueOn();
    }
    else {
        Serial.println("Failed to read temperature, please check temperature module");
        errorLed();
    }
    if (!isnan(h)) {  // check if 'is not a number'
        Serial.print("Humidity % = "); Serial.println(h);
    }
    else {
        Serial.println("Failed to read humidity, please check temperature module");
        errorLed();
    }
    delay(300);
    blueOff();
}
void migalka() {
    // Cycle 2 Setup // Fix low voltage
    pinMode(blue1led, OUTPUT);
    pinMode(blue2led, OUTPUT);
    pinMode(blue3led, OUTPUT);
    pinMode(mainledPin, OUTPUT);
    pinMode(red1led, OUTPUT);
    pinMode(red2led, OUTPUT);
    pinMode(red3led, OUTPUT);
    // Cycle 2 loop
    buttonState = digitalRead(buttonPin);
    blueOn(); // blue ON
    delay(150); // wait
    blueOff(); //blue OFF
    delay(50); // wait
    if (buttonState == LOW) {
        delay(200); // wait
        redOn();  // red ON
        blueOn(); // blue ON
        delay(150); // wait
        redOff();  // red OFF
        blueOff(); // blue OFF
        delay(50); // wait
        redOn();  // red ON
        blueOn(); // blue ON
        delay(250); // wait
        redOff();  // red OFF
        blueOff(); // blue OFF
        delay(250); // wait
    }
    else {
        redOff(); // red OFF
        blueOn(); // blue ON 
        delay(250); // wait
    }
    blueOff();
    delay(150); // wait
    digitalWrite(blue1led, HIGH); // Blue 1 ON
    digitalWrite(blue3led, HIGH); // Blue 3 ON
    delay(250); // wait
    digitalWrite(blue1led, LOW);  // Blue 1 OFF
    digitalWrite(blue2led, HIGH); // Blue 2 ON
    digitalWrite(blue3led, LOW);  // Blue 3 OFF
    delay(250); // wait
    digitalWrite(blue1led, HIGH); // Blue 1 ON
    digitalWrite(blue2led, LOW);  // Blue 2 OFF
    digitalWrite(blue3led, HIGH); // Blue 3 ON
    delay(150); // wait
    digitalWrite(blue1led, LOW);  // Blue 1 OFF
    digitalWrite(blue2led, HIGH); // Blue 2 ON
    digitalWrite(blue3led, LOW);  // Blue 3 OFF
    delay(250); // wait
    blueOn();
    delay(250); // wait
    blueOff();
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
        delay(250);  // wait
        redOn();
        blueOn();
        delay(150); // wait
        redOff();
        blueOff();
        delay(50); // wait
        redOn();
        blueOn();
        delay(250); // wait
        redOff();
        blueOff();
    }
    else {
        redOff();
        delay(50); // wait
        blueOn();
        delay(150); // wait
        blueOff();
    }
    delay(600); // wait END
}

// Led function
void blueOn() {
    pinMode(blue1led, OUTPUT);
    pinMode(blue2led, OUTPUT);
    pinMode(blue3led, OUTPUT);
    digitalWrite(blue1led, HIGH); // Синий 1 ON
    digitalWrite(blue2led, HIGH); // Синий 2 ON
    digitalWrite(blue3led, HIGH); // Синий 3 ON
}
void blueOff() {
    digitalWrite(blue1led, LOW); // Синий 1 OFF
    digitalWrite(blue2led, LOW); // Синий 2 OFF
    digitalWrite(blue3led, LOW); // Синий 3 OFF
}
void redOn() {
    pinMode(red1led, OUTPUT);
    pinMode(red2led, OUTPUT);
    pinMode(red3led, OUTPUT);
    digitalWrite(red1led, HIGH); //Red 1 ON
    digitalWrite(red2led, HIGH); //Red 2 ON
    digitalWrite(red3led, HIGH); //Red 3 ON
}
void redOff() {
    digitalWrite(red1led, LOW); //Red 1 OFF
    digitalWrite(red2led, LOW); //Red 2 OFF
    digitalWrite(red3led, LOW); //Red 3 OFF
}
void greenOn() {
    pinMode(mainledPin, OUTPUT);
    digitalWrite(mainledPin, HIGH);
}
void greenOff() {
    digitalWrite(mainledPin, LOW);
}

// Service
void errorLed() {
    pinMode(red1led, OUTPUT);
    pinMode(red3led, OUTPUT);
    digitalWrite(red1led, HIGH); //Red 1 ON
    digitalWrite(red3led, HIGH); //Red 3 ON
    delay(100);
    digitalWrite(red1led, LOW); //Red 1 OFF
    digitalWrite(red3led, LOW); //Red 3 OFF
}
void pause() {
    Serial.println("Please type ENTER to continue...");
    while (1) {
        if (Serial.available() > 0) { //Input
            incomingByte = Serial.read() - 48;
            if (incomingByte >= 0 & incomingByte <= 9) {
                x = x * 10 + incomingByte;
            }
            else break;
        }

    }
}

// Input
int input() {
    x = 0;
    while (1) {
        if (Serial.available() > 0) { //Input
            incomingByte = Serial.read() - 48;
            if (incomingByte >= 0 & incomingByte <= 9) {
                x = x * 10 + incomingByte;
            }
            else {
                Serial.print(x, DEC);
                break;
            }
        }

    }
    return x;
}
String inputString() {
    String str = "";
    while (str == "") {
        str = Serial.readString();
    }
    Serial.print(str);
    return str;
}

// Test Block Warning!