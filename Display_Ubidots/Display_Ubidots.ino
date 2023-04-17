#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define UBI_TOKEN "YOUR_UBIDOTS_TOKEN"
#define UBI_DEVICE_LABEL "YOUR_UBIDOTS_DEVICE_LABEL"
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PSWD "YOUR_WIFI_PASSWORD"

#define DEVICE 2
#define I2C_ADDR 0x27

const char* ubidots_host = "industrial.api.ubidots.com";
const int ubidots_port = 80;

long long int ts1 = 0, ts2 = 0;
int c1 = 0, c2 = 0;

int status = -1;

LiquidCrystal_I2C lcd(I2C_ADDR, 16, 2);

void controlDevice(bool isHere) {
    if (isHere) {
        digitalWrite(DEVICE, LOW);
    } else {
        digitalWrite(DEVICE, HIGH);
    }
}

void display(String msg){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(msg);
}

void printStatus(bool inOffice, bool inLab){
    if(status == 2*inOffice + inLab){
        return;
    }
    controlDevice(inOffice);
    status = 2*inOffice + inLab;
    Serial.println(status);
    if(inOffice){
        display("R:Office");
    }
    else if(inLab){
        display("R:Lab");
    }
    else{
        display("R:Some Where"); 
    }
}

bool readFromUbidots(String variable_label, long long int &ts) {
    bool value = false;
    WiFiClient client;
    if (client.connect(ubidots_host, ubidots_port)) {
        client.print(String("GET /api/v1.6/devices/") + UBI_DEVICE_LABEL + "/" + variable_label + "/values/?token=" + UBI_TOKEN);
        client.println(" HTTP/1.1");
        client.println("Host: industrial.api.ubidots.com");
        client.println("Connection: close");
        client.println();

        while (client.connected()) {
            String line = client.readStringUntil('\n');
            if (line.indexOf("value") != -1) {
                DynamicJsonDocument doc(1024);
                deserializeJson(doc, line);
                value = doc["results"][0]["value"];
                ts = doc["results"][0]["timestamp"];
                Serial.println(ts);
                break;
            }
        }
    }

    return value;
}

void setup() {
    Serial.begin(115200);
    lcd.init();
    lcd.clear();
    lcd.backlight();
    display("WiFi ..");
    WiFi.begin(WIFI_SSID, WIFI_PSWD);
    pinMode(DEVICE, OUTPUT);
    digitalWrite(DEVICE, HIGH);
}

void loop() {
    Serial.println("------------------------------------------------------------");
    if(WiFi.status() != WL_CONNECTED){
        WiFi.begin(WIFI_SSID, WIFI_PSWD);
        status = -1;
        display("WiFi");
        for(int i=0;i<5;i++){
            lcd.print(".");
            delay(1000);
        }
    }
    else{
        long long int ts;
        bool inOffice = readFromUbidots("IN_OFFICE", ts);
        if(ts == ts1){
            c1++;
        }
        else{
            ts1 = ts;
            c1 = 0;
        }
        bool inLab = readFromUbidots("IN_LAB", ts);
        if(ts == ts2){
            c2++;
        }
        else{
            ts2 = ts;
            c2 = 0;
        }

        Serial.print("In Office: ");
        Serial.println(inOffice);
        Serial.print("In Lab: ");
        Serial.println(inLab);

        if(c1 > 20){
            inOffice = false;
        }

        if(c2 > 20){
            inLab = false;
        }

        if((c1<=20 && inOffice) || (inLab && c2<=20) || (c1<=20 && c2<=20)){
            printStatus(inOffice, inLab);
            delay(5000);
        }
        else{
            status = -1;
            display("Status");
            for(int i=0;i<5;i++){
                lcd.print(".");
                delay(1000);
            }
        }
    }
}
