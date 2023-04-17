#include <Ubidots.h>

#include <BLEDevice.h>
#include <BLEScan.h>

#define UBI_TOKEN "YOUR_UBIDOTS_TOKEN"
#define UBI_DEVICE_LABEL "YOUR_UBIDOTS_DEVICE_LABEL"
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PSWD "YOUR_WIFI_PASSWORD"
#define KNOWN_DEVICE "YOUR_BLE_DEVICE_MAC_ADDRESS"

#define GREEN D8
#define BLUE D9
#define RED D10

int rssi = 0;
int threshold = -50;
bool inOffice = 0;

BLEScan* pBLEScan;

Ubidots device(UBI_TOKEN, UBI_HTTP);

void updateStatus(bool isHere){
    if(isHere){
        digitalWrite(GREEN, HIGH);
        digitalWrite(RED, LOW);
    }
    else{
        digitalWrite(GREEN, LOW);
        digitalWrite(RED, HIGH);
    }
}

void scanForDevices() {
    BLEDevice::init("");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setActiveScan(true);
    BLEScanResults foundDevices = pBLEScan->start(4);

    for (int i = 0; i < foundDevices.getCount(); i++) {
            BLEAdvertisedDevice device = foundDevices.getDevice(i);
            if (device.getAddress().toString() == KNOWN_DEVICE) {
                rssi = device.getRSSI();
            }
    }
    pBLEScan->clearResults();
}


void setup() {
    pinMode(RED, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(GREEN, OUTPUT);

    digitalWrite(RED, LOW);
    digitalWrite(BLUE, HIGH);
    digitalWrite(GREEN, LOW);

    Serial.begin(115200);
    device.wifiConnect(WIFI_SSID, WIFI_PSWD);
    // device.setDebug(true);

    // digitalWrite(RED, LOW);
    digitalWrite(BLUE, LOW);
    // digitalWrite(GREEN, LOW);
}

void loop() {
    Serial.println("--------------------------------------");
    if(WiFi.status() == WL_CONNECTED){
        digitalWrite(BLUE, LOW);
        int tem = device.get(UBI_DEVICE_LABEL, "THRESHOLD");
        if(tem >= -100){
            threshold = tem;
        }
    }
    int avg = 0;
    for(int i=0;i<5;i++){
        rssi = -200;
        scanForDevices();
        Serial.println(rssi);
        avg += rssi;
    }

    avg /= 5;
    inOffice = avg >= threshold;

    updateStatus(inOffice);

    if(WiFi.status() == WL_CONNECTED){
        digitalWrite(BLUE, LOW);
        device.add("RSSI_OFFICE", avg);
        device.add("IN_OFFICE", inOffice);
        device.send(UBI_DEVICE_LABEL);
    }
    else{
        digitalWrite(BLUE, HIGH);
        WiFi.begin(WIFI_SSID, WIFI_PSWD);
    }
}
