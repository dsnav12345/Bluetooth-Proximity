# Proximity Detection Using Bluetooth
This project uses an ESP32 as a BLE scanner to detect the presence of nearby BLE devices, such as a smartwatch. If the detected device's signal strength (RSSI) is below a threshold value, its status is updated on an IoT dashboard using the Ubidots platform. In this project there are two places office and lab and a known device sunch as smart watch whose location is detected based on its bluetooth signal. Its location (In office/In lab/Else where) is updated to dashboard and is same shown in a LCD screen connected to an ESP8266.
## Apparatus
1. XIAO_ESP32C3 modules (by seed studio) - 2

2. NodeMCU v1.0 -1

3. LCD Screen with I2C adapter - 1

4. RGB LEDs - 2

5. Jumper wires

6. A laptop/desktop installed with Arduino IDE (Procedure is given below to setup the PC)
## PC setup
### Arduino Installation
1. Go to the arduino downloads webpage using this [link](https://www.arduino.cc/en/software) 

2. Download arduino installer of your preference based on your PC's Operating System 

3. Install the Arduino IDE on your PC by using the downloaded installer.

### Board Manager Installation
1. Open Arduino IDE

2. Go to File > Preferences
3. Enter the text below into the "Additional Boards Manager URLs" field as shown in this [image](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2022/04/ESP32-URL-Arduino-IDE.png?w=828&quality=100&strip=all&ssl=1) 
    > https://arduino.esp8266.com/stable/package_esp8266com_index.json,https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

4. Go to Tools > Board > Boards manager
    1. Search for the word "esp8266" and install "esp8266 by ESP8266 Community"

    2. Search for the word "esp32" and install "esp32 by Espressif Systems"
### Libraries Installation
1. Go to Sketch > Include Library > Manage Libraries
    1. Search for the word "LiquidCrystal I2C" and install the library "LiquidCrystal I2C by Frank de Brabander"

    2. Search for the word "ArduinoJson" and install the library "ArduinoJson by Benoit Blanchon"

2. Ubidots Library
    1. Go to this [git repository](https://github.com/ubidots/ubidots-esp32) and download the zip file or use this [link](https://github.com/ubidots/ubidots-esp32/archive/refs/heads/main.zip) to download the zip library

    2. Go to Sketch > Include Library > Add .ZIP Library

    3. A prompt will be opened navigate to the download location and select the downloaded zip library

## Ubidots Platform Setup
1. Open a web browser and go to [ubidots.com/stem](https://ubidots.com/stem) website

2. Click on **CREATE FREE ACCOUNT**, fill in your details and signup

3. Login to your account in [stem.ubidots.com](https://stem.ubidots.com/accounts/signin/)

4. Go to Devices > Devices

5. Click on **Create Device** and select **Blank Device**

6. Enter a name and label for your device. you must remember the device label as it is required for future.

7. Click on the device created.

8. On left you can see some details of the device. Copy the token and save it for later.

9. Click **Add Variable** and name the variable as **Threshold**

10. Go to Data > Dashboard, click **Add new Dashboard** and click save

11. Click **Add new Widget** and Select Control > Slider

12. Click **ADD VARIABLES**, Select the device and Select Threshold

13. Set Minimum Value as ***-100*** and Maximum value as ***0*** and clikc save

14. Adjust the threshold to -50 using the slider.

## Hardware Connections
1. Connect the XIAO_ESP32C3s each with a RGB LED as shown in figure below
    ![XIAO_ESP32C3 connected to RGB LED](/Images/esp32_led.png "XIAO_ESP32C3 connected to RGB LED")
    1. **GND** pin of LED is connected to **GND** pin of XIAO_ESP32C3

    2. **R** pin of LED is connected to **D10** pin of XIAO_ESP32C3

    3. **B** pin of LED is connected to **D9** pin of XIAO_ESP32C3

    4. **G** pin of LED is connected to **D8** pin of XIAO_ESP32C3

2. Connect the NodeMCU v1.0 with LCD screen
    ![NodeMCU v1.0 connected to LCD screen](/Images/node_lcd.png "NodeMCU v1.0 connected to LCD Screen")
    1. **GND** pin of LCD Screen is connected to **GND** pin of NodeMCU v1.0

    2. **VCC** pin of LCD Screen is connected to **3V3** pin of NodeMCU v1.0

    3. **SDA** pin of LCD Screen is connected to **D2** pin of NodeMCU v1.0

    4. **SCL** pin of LCD Screen is connected to **D1** pin of NodeMCU v1.0
    
>If the LCD Screen is not working try rotating the screw inside bluebox on backside of the LCD Screen to adjust the display contrast
    
>Also try connecting the **VCC** pin of LCD Screen to **Vin** instead of **3V3** pin of NodeMCU

## Code Upload
Downlad / Clone the repository and save it in your PC
### I2C address of LCD Screen
1. Open Arduino IDE

2. Create new sketch and copy paste this code in the code area
    ```
    #include <Wire.h>

    void setup() {
        Serial.begin (9600);
        while (!Serial){}
    }

    void loop() {
        Serial.println ("Scanning ...");
        byte count = 0;

        Wire.begin();
        for (byte i = 0; i < 127; i++) {
            Wire.beginTransmission (i);
            if (Wire.endTransmission () == 0){
                Serial.print ("Found address: ");
                Serial.print (i, DEC);
                Serial.print (" (0x");
                Serial.print (i, HEX);
                Serial.println (")");
                count++;
                delay (1000);  
            }
        }
        Serial.println ("Done.");
    }

    ```

3. Connect the NodeMCU v1.0 to the laptop using micro USB cable.

4. Go to Tools > Board > esp8266 > NodeMCU 1.0 (ESP-12E Module)

5. Go to Tools > Port and select the port.

6. Go to Sketch > Upload

7. Observe the output area and wait until code is uploaded succesfully

8. Go to Tools > Serial Moniter

9. In the Serial Moniter you can observe the I2C address of LCD Screen being printed.

10. Note down the address and save it for later.

### MAC address of Smart Watch
1. On your mobile phone download and install the application **nRF Connect**

2. Open the application and give required bluetooth permissions

3. It will scan for nearby devices and display all BLE Devices.

4. Find your smart watch among the devices shown and note down its MAC address and save it for later.

### Office_Ubidots
1. Go to the repository downloaded in your PC and open ***Office_Ubidots.ino***

2. In the code from line 6 - 10 replace the variables with your values
    ```
    #define UBI_TOKEN "YOUR_UBIDOTS_TOKEN"
    #define UBI_DEVICE_LABEL "YOUR_UBIDOTS_DEVICE_LABEL"
    #define WIFI_SSID "YOUR_WIFI_SSID"
    #define WIFI_PSWD "YOUR_WIFI_PASSWORD"
    #define KNOWN_DEVICE "YOUR_BLE_DEVICE_MAC_ADDRESS"
    ```
    1. Replace ***YOUR_UBIDOTS_TOKEN*** with the token of the device created in Ubidots Platform

    2. Replace ***YOUR_UBIDOTS_DEVICE_LABEL*** with the label you have given in Ubidots Platform

    3. Replace ***YOUR_WIFI_SSID*** with the name of your WiFi at Location 1

    4. Replace ***YOUR_WIFI_PASSWORD*** with the password for your WiFi at Location 1

    5. Replace ***YOUR_BLE_DEVICE_MAC_ADDRESS*** with the MAC address of you smart watch displayed in nRF Connect application

3. Connect one of the two XIAO_ESP32C3 to the laptop with USB Cable

4. Go to Tools > Board > esp32 > XIAO_ESP32C3

5. Go to Tools > Port and select the port.

6. Go to Sketch > Upload

7. Observe the output area and wait until code is uploaded succesfully

### Lab_Ubidots
1. Go to the repository downloaded in your PC and open ***Lab_Ubidots.ino***

2. In the code from line 6 - 10 replace the variables with your values
    ```
    #define UBI_TOKEN "YOUR_UBIDOTS_TOKEN"
    #define UBI_DEVICE_LABEL "YOUR_UBIDOTS_DEVICE_LABEL"
    #define WIFI_SSID "YOUR_WIFI_SSID"
    #define WIFI_PSWD "YOUR_WIFI_PASSWORD"
    #define KNOWN_DEVICE "YOUR_BLE_DEVICE_MAC_ADDRESS"
    ```
    1. Replace ***YOUR_UBIDOTS_TOKEN*** with the token of the device created in Ubidots Platform

    2. Replace ***YOUR_UBIDOTS_DEVICE_LABEL*** with the label you have given in Ubidots Platform

    3. Replace ***YOUR_WIFI_SSID*** with the name of your WiFi at Location 2

    4. Replace ***YOUR_WIFI_PASSWORD*** with the password for your WiFi at Location 2

    5. Replace ***YOUR_BLE_DEVICE_MAC_ADDRESS*** with the MAC address of you smart watch displayed in nRF Connect application

3. Connect the other XIAO_ESP32C3 to the laptop with USB Cable

4. Go to Tools > Board > esp32 > XIAO_ESP32C3

5. Go to Tools > Port and select the port.

6. Go to Sketch > Upload

7. Observe the output area and wait until code is uploaded succesfully

### Display_Ubidots
1. Go to the repository downloaded in your PC and open ***Display_Ubidots.ino***

2. In the code from line 6 - 9 replace the variables with your values
    ```
    #define UBI_TOKEN "YOUR_UBIDOTS_TOKEN"
    #define UBI_DEVICE_LABEL "YOUR_UBIDOTS_DEVICE_LABEL"
    #define WIFI_SSID "YOUR_WIFI_SSID"
    #define WIFI_PSWD "YOUR_WIFI_PASSWORD"
    ```
    1. Replace ***YOUR_UBIDOTS_TOKEN*** with the token of the device created in Ubidots Platform

    2. Replace ***YOUR_UBIDOTS_DEVICE_LABEL*** with the label you have given in Ubidots Platform

    3. Replace ***YOUR_WIFI_SSID*** with the name of your WiFi at the location where this device will be used

    4. Replace ***YOUR_WIFI_PASSWORD*** with the password for the WiFi at the location

3. In the code on line 11 replace the ***0x27*** with the I2C address you found

4. Connect the NodeMCU v1.0 to the laptop using micro USB cable.

5. Go to Tools > Board > esp8266 > NodeMCU 1.0 (ESP-12E Module)

6. Go to Tools > Port and select the port.

7. Go to Sketch > Upload

8. Observe the output area and wait until code is uploaded succesfully


## Usage
1. Connect the XIAO_ESP32C3 which is uploaded with the code ***Office_Ubidots.ino*** to the power and place it at location 1

2. Connect the XIAO_ESP32C3 which is uploaded with the code ***Lab_Ubidots.ino*** to the power and place it at location 2

3. Connect the NodeMCU v1.0 to the power and place it at location of your desire.

4. When the smart watch is around XIAO_ESP32C3 at location 1 the LCD Display of NodeMCU will show ***R:Office***

5. When the smart watch is around XIAO_ESP32C3 at location 2 the LCD Display of NodeMCU will show ***R:Lab***

6. When the smart watch is not there at any location then the LCD Display of NodeMCU will show ***R:Somewhere***

7. Open a web browser and login to your Ubidots account

8. Go to Data > Dashboards

9. You can adjust the threshold to change the distance of detection between the XIAO_ESP32C3 and your smart watch.