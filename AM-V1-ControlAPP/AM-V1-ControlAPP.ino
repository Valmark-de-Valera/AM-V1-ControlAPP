/* Meta data
 * Author: Valerii Marchuk
 * Department: Valmark Media Programming Centre
 * Project: AM-V1 Control Panel 2.0 Beta
*/

// Cycle 1 Library
#include <SPI.h>
#include "SDFat.h"
#include "sdios.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <stdio.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();

// Common
const int buttonMainPin = 10;  // Main control button
String app_type = "Beta";
String app_version = "v2.0";


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

// File--------------------------------------------------------------------------
#define SD_CS_PIN 4
#define USE_SDIO 0
const int8_t DISABLE_CHIP_SELECT = -1; // to 10 to disable the Ethernet controller.
ArduinoOutStream cout(Serial);
uint32_t cardSize;
uint32_t eraseSize;
SdFat SD;
File serviceFile;
#define SDErrorMsg(msg) SD.errorPrint(F(msg));

// Calculator--------------------------------------------------------------------
long number1;
long number2;
char calSignal;
long result;

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
    Serial.print("Input command: ");
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
            else if (com.indexOf("sd") >= 0 | com.indexOf("SD") >= 0) {
                SDMode();
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

// Mode function-----------------------------------------------------------------
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
    Serial.print("Input control command: ");
    while (1)
    {
        if (flag) Migalka();
        com = Serial.readString();
        if (com.indexOf("stop") >= 0 | com.indexOf("Stop") >= 0) { flag = false; Serial.println("Stop scenery"); }
        if (com.indexOf("start") >= 0 | com.indexOf("Start") >= 0) { flag = true; Serial.println("Start scenery"); }
        if (com.indexOf("exit") >= 0 | com.indexOf("Exit") >= 0) break;
    }
}
void CalculatorMode() {
    Serial.print("Send a calculation: ");
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
void SDMode() {
    while (1)
    {
        Serial.println("Control: 'test' to test SD Card, 'wipe' to wipe SD Card, 'exit' return to menu");
        Serial.print("Input control command: ");
        com = InputString();
        if (com.indexOf("test") >= 0) {
            if (SDF2()) Serial.println("Test complete\n");
        }
        else if (com.indexOf("wipe") >= 0) {
            SDWipe();
        }
        else if (com.indexOf("exit") >= 0) {
            break;
        }
        else Serial.println("Wrong command, try again... \r\n");
    }
}
void Help() {
    Serial.println("Help list");
    Serial.println("Input 'test' to check the last modification of AM-V1 'Sendvich'");
    Serial.println("Input 'led' to control led system of AM-V1 'Sendvich'");
    Serial.println("Input 'temp' to check temperature");
    Serial.println("Input 'migalka' to start migalka scenary");
    Serial.println("Input 'sd' to start test SD card");
    //Serial.println("Input 'calc' to open calculator"); //Unstable for now
}

// Led function------------------------------------------------------------------
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

//// Service
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
void Calculator() {
    switch (calSignal) {
    case '+': result = number1 + number2; break;
    case '-': result = number1 - number2; break;
    case '*': result = number1 * number2; break;
    case '/': 
        if (number2 != 0) result = number1 / number2; 
        else Serial.println("Zero Division!");
        break;
    default: Serial.println("Invalid input");
        Serial.println();
        result = 0;
    }
}
// SD----------------------------------------------------------------------------
bool SDRWTest() {
    String file = "test.txt";
    Serial.print("Initializing SD card...");

    if (!SD.begin(4)) {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");
    serviceFile = SD.open(file, FILE_WRITE);
    if (serviceFile) {
        Serial.print("Writing to " + file + "...");
        serviceFile.println("testing 1, 2, 3.");
        serviceFile.close();
        Serial.println("done.");
    }
    else {
        Serial.println("error opening " + file);
    }
    serviceFile = SD.open(file);
    if (serviceFile) {
        Serial.println(file + ": ");
        while (serviceFile.available()) {
            Serial.write(serviceFile.read());
        }
        serviceFile.close();
        serviceFile = SD.open(file, FILE_WRITE);
        if (serviceFile.remove()) Serial.println("Test file delete confirmed!");
        else serviceFile.close();
        return true;
    }
    else {
        Serial.println("error opening " + file);
    }
    return;
}
void SDWipe() {
    int c;
    Serial.println("Type 'Y' to wipe all data.");
    while (!Serial.available()) {
        SysCall::yield();
    }
    c = Serial.read();
    if (c != 'Y') {
        SD.errorHalt("Quitting, you did not type 'Y'.");
    }
    if (!SD.begin(SD_CS_PIN, SD_SCK_MHZ(50))) {
        SD.initErrorHalt();
    }
    if (!SD.wipe(&Serial)) {
        SD.errorHalt("Wipe failed.");
    } 
    if (!SD.begin(SD_CS_PIN, SD_SCK_MHZ(50))) {
        SD.errorHalt("Second init failed.");
    }
    Serial.println("Done");
}
uint8_t cidDmp() {
    cid_t cid;
    if (!SD.card()->readCID(&cid)) {
        SDErrorMsg("readCID failed");
        return false;
    }
    cout << F("\nManufacturer ID: ");
    cout << hex << int(cid.mid) << dec << endl;
    cout << F("OEM ID: ") << cid.oid[0] << cid.oid[1] << endl;
    cout << F("Product: ");
    for (uint8_t i = 0; i < 5; i++) {
        cout << cid.pnm[i];
    }
    cout << F("\nVersion: ");
    cout << int(cid.prv_n) << '.' << int(cid.prv_m) << endl;
    cout << F("Serial number: ") << hex << cid.psn << dec << endl;
    cout << F("Manufacturing date: ");
    cout << int(cid.mdt_month) << '/';
    cout << (2000 + cid.mdt_year_low + 10 * cid.mdt_year_high) << endl;
    cout << endl;
    return true;
}
uint8_t csdDmp() {
    csd_t csd;
    uint8_t eraseSingleBlock;
    if (!SD.card()->readCSD(&csd)) {
        SDErrorMsg("readcsd failed");
        return false;
    }
    if (csd.v1.csd_ver == 0) {
        eraseSingleBlock = csd.v1.erase_blk_en;
        eraseSize = (csd.v1.sector_size_high << 1) | csd.v1.sector_size_low;
    }
    else if (csd.v2.csd_ver == 1) {
        eraseSingleBlock = csd.v2.erase_blk_en;
        eraseSize = (csd.v2.sector_size_high << 1) | csd.v2.sector_size_low;
    }
    else {
        cout << F("csd version error\n");
        return false;
    }
    eraseSize++;
    cout << F("cardSize: ") << 0.000512 * cardSize;
    cout << F(" MB (MB = 1,000,000 bytes)\n");

    cout << F("flashEraseSize: ") << int(eraseSize) << F(" blocks\n");
    cout << F("eraseSingleBlock: ");
    if (eraseSingleBlock) {
        cout << F("true\n");
    }
    else {
        cout << F("false\n");
    }
    return true;
}
uint8_t partDmp() {
    mbr_t mbr;
    if (!SD.card()->readBlock(0, (uint8_t*)&mbr)) {
        SDErrorMsg("read MBR failed");
        return false;
    }
    for (uint8_t ip = 1; ip < 5; ip++) {
        part_t* pt = &mbr.part[ip - 1];
        if ((pt->boot & 0X7F) != 0 || pt->firstSector > cardSize) {
            cout << F("\nNo MBR. Assuming Super Floppy format.\n");
            return true;
        }
    }
    cout << F("\nSD Partition Table\n");
    cout << F("part,boot,type,start,length\n");
    for (uint8_t ip = 1; ip < 5; ip++) {
        part_t* pt = &mbr.part[ip - 1];
        cout << int(ip) << ',' << hex << int(pt->boot) << ',' << int(pt->type);
        cout << dec << ',' << pt->firstSector << ',' << pt->totalSectors << endl;
    }
    return true;
}
void volDmp() {
    cout << F("\nVolume is FAT") << int(SD.vol()->fatType()) << endl;
    cout << F("blocksPerCluster: ") << int(SD.vol()->blocksPerCluster()) << endl;
    cout << F("clusterCount: ") << SD.vol()->clusterCount() << endl;
    cout << F("freeClusters: ");
    uint32_t volFree = SD.vol()->freeClusterCount();
    cout << volFree << endl;
    float fs = 0.000512 * volFree * SD.vol()->blocksPerCluster();
    cout << F("freeSpace: ") << fs << F(" MB (MB = 1,000,000 bytes)\n");
    cout << F("fatStartBlock: ") << SD.vol()->fatStartBlock() << endl;
    cout << F("fatCount: ") << int(SD.vol()->fatCount()) << endl;
    cout << F("blocksPerFat: ") << SD.vol()->blocksPerFat() << endl;
    cout << F("rootDirStart: ") << SD.vol()->rootDirStart() << endl;
    cout << F("dataStartBlock: ") << SD.vol()->dataStartBlock() << endl;
    if (SD.vol()->dataStartBlock() % eraseSize) {
        cout << F("Data area is not aligned on flash erase boundaries!\n");
        cout << F("Download and use formatter from www.SDcard.org!\n");
    }
}
void SDF1() {
    SDRWTest();
    // use uppercase in hex and use 0X base prefix
    cout << uppercase << showbase << endl;

    // F stores strings in flash to save RAM
    cout << F("SDFat version: ") << SD_FAT_VERSION << endl;
#if !USE_SDIO  
    if (DISABLE_CHIP_SELECT < 0) {
        cout << F(
            "\nAssuming the SD is the only SPI device.\n"
            "Edit DISABLE_CHIP_SELECT to disable another device.\n");
    }
    else {
        cout << F("\nDisabling SPI device on pin ");
        cout << int(DISABLE_CHIP_SELECT) << endl;
        pinMode(DISABLE_CHIP_SELECT, OUTPUT);
        digitalWrite(DISABLE_CHIP_SELECT, HIGH);
    }
    cout << F("\nAssuming the SD chip select pin is: ") << int(4);
    cout << F("\nEdit SD_CHIP_SELECT to change the SD chip select pin.\n");
#endif  // !USE_SDIO  
}
bool SDF2() {
    SDF1();
    uint32_t t = millis();
#if USE_SDIO
    if (!SD.cardBegin()) {
        SDErrorMsg("\ncardBegin failed");
        return false;
    }
#else
    if (!SD.cardBegin(4, SD_SCK_MHZ(50))) {
        SDErrorMsg("cardBegin failed");
        return false;
    }
#endif 
    t = millis() - t;

    cardSize = SD.card()->cardSize();
    if (cardSize == 0) {
        SDErrorMsg("cardSize failed");
        return false;
    }
    cout << F("\ninit time: ") << t << " ms" << endl;
    cout << F("\nCard type: ");
    switch (SD.card()->type()) {
    case SD_CARD_TYPE_SD1:
        cout << F("SD1\n");
        break;

    case SD_CARD_TYPE_SD2:
        cout << F("SD2\n");
        break;

    case SD_CARD_TYPE_SDHC:
        if (cardSize < 70000000) {
            cout << F("SDHC\n");
        }
        else {
            cout << F("SDXC\n");
        }
        break;

    default:
        cout << F("Unknown\n");
    }
    if (!cidDmp()) {
        return false;
    }
    if (!csdDmp()) {
        return false;
    }
    uint32_t ocr;
    if (!SD.card()->readOCR(&ocr)) {
        SDErrorMsg("\nreadOCR failed");
        return false;
    }
    cout << F("OCR: ") << hex << ocr << dec << endl;
    if (!partDmp()) {
        return false;
    }
    if (!SD.fsBegin()) {
        SDErrorMsg("\nFile System initialization failed.\n");
        return false;
    }
    volDmp();
    return true;
}
// Input-------------------------------------------------------------------------
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