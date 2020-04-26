/* Meta data
 * Author: Valerii Marchuk
 * Department: Valmark Media Programming Centre
 * Project: AM-V1 Control Panel 1.5 Beta
*/

// Cycle 1 Library
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <stdio.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();

// Common
const int buttonMainPin = 10;  // Main control button
String app_type = "Beta";
String app_version = "v1.5";


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
//Calculator
long number1;
long number2;
char calSignal;
long result; // result of the calculation


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
    Serial.println("System wait your first request. It`s special function for testing connection");
    Serial.println("Input command");
    delay(2000);
}

void loop() {
    buttonMain = digitalRead(buttonMainPin);
    if (buttonMain == HIGH && Serial.available()) {
        while (1) {
            Serial.print("Input command: ");
            com = InputString();
            if (com.indexOf("test") >= 0) {
                if(Test()) Serial.println("Test complete");
            }
            else if (com.indexOf("led") >= 0) {
                LedControl();
            }
            else if (com.indexOf("temperature") >= 0 | com.indexOf("temp") >= 0) {
                Temperature();
            }
            else if (com.indexOf("Help") >= 0 | com.indexOf("help") >= 0 | com.indexOf("?") >= 0) {
                Help();
            }
            else if (com.indexOf("Migalka") >= 0 | com.indexOf("migalka") >= 0) {
                MigalkaMode();
            }
            else if (com.indexOf("calc") >= 0 | com.indexOf("calculator") >= 0) {
                Serial.print("This function is unstable for now, to continue enter 'ignore', to exit type ENTER");
                com = InputString();
                if (com.indexOf("ignore") >= 0) {
                    CalculatorMode();
                }
            }
            else Serial.println("Wrong command, try again... \r\n");
        }
    }
    else if (buttonMain == HIGH) {
        ErrorLed();
    }
    else if (buttonMain == LOW) {
        Migalka();
    }
    BlueOff();
}

// Mode function
void InDevelopment() {
    Serial.println("Sorry, but now function unavailble");
}
bool Test() {
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
    RedOff();
    delay(200);
    GreenOff();
    delay(200);
    BlueOff();
    delay(500);
    digitalWrite(blue1led, HIGH);
    digitalWrite(blue3led, HIGH);
    digitalWrite(red1led, HIGH);
    digitalWrite(red3led, HIGH);
    delay(200);
    BlueOff();
    RedOff();
    digitalWrite(blue2led, HIGH);
    digitalWrite(red2led, HIGH);
    delay(200);
    BlueOff();
    RedOff();
    delay(500);
    digitalWrite(blue1led, HIGH);
    digitalWrite(blue3led, HIGH);
    digitalWrite(red1led, HIGH);
    digitalWrite(red3led, HIGH);
    delay(200);
    BlueOff();
    RedOff();
    digitalWrite(blue2led, HIGH);
    digitalWrite(red2led, HIGH);
    delay(200);
    BlueOff();
    RedOff();
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
    ErrorLed();
    Serial.print("OK");
    // button indicate
    Serial.println(); Serial.print("Button position: ");
    buttonMain = digitalRead(buttonMainPin);
    if (buttonMain == HIGH) Serial.print("ON");
    else Serial.print("OFF");
    // input test
    Serial.println(); Serial.print("Input number: ");
    Input();
    Serial.println(); Serial.print("Input string: ");
    InputString();
    // Finish
    return true;
}
void LedControl() {
    while (1) {
        Serial.println();
        Serial.print("Please type led command number: ");
        switch (Input())
        {
        case 1: digitalWrite(blue1led, HIGH); break;
        case 2: digitalWrite(blue2led, HIGH); break;
        case 3: digitalWrite(blue3led, HIGH); break;
        case 4: GreenOn(); break;
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
        case 123: RedOn(); GreenOn(); BlueOn(); break;
        case 0: RedOff(); GreenOff(); BlueOff(); break;
        default:
            ErrorLed();
            break;
        }
        buttonMain = digitalRead(buttonMainPin);
        if (buttonMain == LOW) break;
    }
}
void Temperature() {
    float t = sht31.readTemperature();
    float h = sht31.readHumidity();
    if (!isnan(t)) {  // check if 'is not a number'
        Serial.print("Temperature *C = "); Serial.println(t, DEC);
        BlueOn();
    }
    else {
        Serial.println("Failed to read temperature, please check temperature module");
        ErrorLed();
    }
    if (!isnan(h)) {  // check if 'is not a number'
        Serial.print("Humidity % = "); Serial.println(h);
    }
    else {
        Serial.println("Failed to read humidity, please check temperature module");
        ErrorLed();
    }
    delay(300);
    BlueOff();
}
void Migalka() {
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
    BlueOn(); // blue ON
    delay(150); // wait
    BlueOff(); //blue OFF
    delay(50); // wait
    if (buttonState == LOW) {
        delay(200); // wait
        RedOn();  // red ON
        BlueOn(); // blue ON
        delay(150); // wait
        RedOff();  // red OFF
        BlueOff(); // blue OFF
        delay(50); // wait
        RedOn();  // red ON
        BlueOn(); // blue ON
        delay(250); // wait
        RedOff();  // red OFF
        BlueOff(); // blue OFF
        delay(250); // wait
    }
    else {
        RedOff(); // red OFF
        BlueOn(); // blue ON 
        delay(250); // wait
    }
    BlueOff();
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
    BlueOn();
    delay(250); // wait
    BlueOff();
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
        delay(250);  // wait
        RedOn();
        BlueOn();
        delay(150); // wait
        RedOff();
        BlueOff();
        delay(50); // wait
        RedOn();
        BlueOn();
        delay(250); // wait
        RedOff();
        BlueOff();
    }
    else {
        RedOff();
        delay(50); // wait
        BlueOn();
        delay(150); // wait
        BlueOff();
    }
    delay(600); // wait END
}
void MigalkaMode() {
    bool flag = true;
    Serial.println("Control: 'stop' to stop scenary, 'start' to continue, 'exit' return to menu");
    Serial.println("Input control command");
    while (1)
    {
        if (flag) Migalka();
        com = Serial.readString();
        if (com.indexOf("stop") >= 0 | com.indexOf("Stop") >= 0) { flag = false; Serial.println("Stop scenery"); }
        if (com.indexOf("start") >= 0 | com.indexOf("Start") >= 0) { flag = true; Serial.println("Start scenery"); }
        if (com.indexOf("exit") >= 0 | com.indexOf("Exit") >= 0) break;
    }
}
void Help() {
    Serial.println("Help list");
    Serial.println("Input 'test' to check the last modification of AM-V1 'Sendvich'");
    Serial.println("Input 'led' to control led system of AM-V1 'Sendvich'");
    Serial.println("Input 'temp' to check temperature");
    Serial.println("Input 'migalka' to start migalka scenary");
}

// Led function
void BlueOn() {
    pinMode(blue1led, OUTPUT);
    pinMode(blue2led, OUTPUT);
    pinMode(blue3led, OUTPUT);
    digitalWrite(blue1led, HIGH); // Синий 1 ON
    digitalWrite(blue2led, HIGH); // Синий 2 ON
    digitalWrite(blue3led, HIGH); // Синий 3 ON
}
void BlueOff() {
    digitalWrite(blue1led, LOW); // Синий 1 OFF
    digitalWrite(blue2led, LOW); // Синий 2 OFF
    digitalWrite(blue3led, LOW); // Синий 3 OFF
}
void RedOn() {
    pinMode(red1led, OUTPUT);
    pinMode(red2led, OUTPUT);
    pinMode(red3led, OUTPUT);
    digitalWrite(red1led, HIGH); //Red 1 ON
    digitalWrite(red2led, HIGH); //Red 2 ON
    digitalWrite(red3led, HIGH); //Red 3 ON
}
void RedOff() {
    digitalWrite(red1led, LOW); //Red 1 OFF
    digitalWrite(red2led, LOW); //Red 2 OFF
    digitalWrite(red3led, LOW); //Red 3 OFF
}
void GreenOn() {
    pinMode(mainledPin, OUTPUT);
    digitalWrite(mainledPin, HIGH);
}
void GreenOff() {
    digitalWrite(mainledPin, LOW);
}

// Service
void ErrorLed() {
    pinMode(red1led, OUTPUT);
    pinMode(red3led, OUTPUT);
    digitalWrite(red1led, HIGH); //Red 1 ON
    digitalWrite(red3led, HIGH); //Red 3 ON
    delay(100);
    digitalWrite(red1led, LOW); //Red 1 OFF
    digitalWrite(red3led, LOW); //Red 3 OFF
}
void Pause() {
    int x = 0;
    int incomingByte = 0;
    int secondIncomingByte = 0;
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
void CalculatorMode() {
    Serial.println("Send a calculation: ");
    while (Serial.available() <= 0) {
        number1 = Serial.parseInt();
        calSignal = Serial.read();
        number2 = Serial.parseInt();
        Calculator();
        if (Serial.available() > 0) {
            Serial.println("Result = ");
            Serial.println(result);
        }
        Serial.println();
    }
}
void Calculator() {
    switch (calSignal) {
    case '+': result = number1 + number2; break;
    case '-': result = number1 - number2; break;
    case '*': result = number1 * number2; break;
    case '/': result = number1 / number2; // Warning! exception need
        break;
    default: Serial.println("Invalid input");
        Serial.println();
        result = 0;
    }
}

// Input
int Input() {
    int x = 0;
    int incomingByte = 0;
    int secondIncomingByte = 0;
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
String InputString() {
    String str = "";
    while (str == "") {
        str = Serial.readString();
    }
    Serial.print(str);
    return str;
}
// Test Block Warning!