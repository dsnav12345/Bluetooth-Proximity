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

## Procedure
### Hardware Connections
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
### Code Upload