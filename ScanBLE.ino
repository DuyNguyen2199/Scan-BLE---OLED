#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include <esp_task_wdt.h>


#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_MOSI   22
#define OLED_CLK   18
#define OLED_DC    16
#define OLED_CS    5
#define OLED_RESET 17

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

unsigned long previousMillis = 0;
const long interval = 1800000;

//String knownBLEAddresses[] = {"ac:0b:fb:67:fd:42"};
//int RSSI_THRESHOLD = -50;
//bool device_found;
int scanTime = 5; //In seconds
BLEScan* pBLEScan;
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {

      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());

      if (advertisedDevice.haveName())
      {
        display.clearDisplay();
        Serial.print("Device name: ");
        Serial.println(advertisedDevice.getName().c_str());
        Serial.println("");
        display.setTextSize(1.5); 
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(29, 5);
        display.println("Device Name ");
        display.setCursor(19, 17);
        display.println(F(advertisedDevice.getName().c_str()));
        display.display(); 

        BLEAddress devAddress = advertisedDevice.getAddress();
        Serial.print("ID Address: ");
        Serial.println(devAddress.toString().c_str());
        Serial.println("");
        display.setTextSize(1.5); 
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(30, 32);
        display.println("Mac Address ");
        display.setCursor(11, 42);
        display.println(F(devAddress.toString().c_str()));
        display.display(); 
        delay(2000);
        //esp_task_wdt_reset();
      }
      else {
        
        if (millis() > 20000 ){
        display.clearDisplay();
        display.setTextSize(1.5); 
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(30, 20);
        display.print("No scanning");
        display.setCursor(8, 35);
        display.print("device was found !!!");
        display.display(); 
        delay(2000);
        esp_task_wdt_reset(); 
        }
      }
/*
      for (int i = 0; i < (sizeof(knownBLEAddresses) / sizeof(knownBLEAddresses[0])); i++)
      {
        //Uncomment to Enable Debug Information
        //Serial.println("*************Start**************");
        //Serial.println(sizeof(knownBLEAddresses));
        //Serial.println(sizeof(knownBLEAddresses[0]));
        //Serial.println(sizeof(knownBLEAddresses)/sizeof(knownBLEAddresses[0]));
        //Serial.println(advertisedDevice.getAddress().toString().c_str());
        //Serial.println(knownBLEAddresses[i].c_str());
        //Serial.println("*************End**************");
        if (strcmp(advertisedDevice.getAddress().toString().c_str(), knownBLEAddresses[i].c_str()) == 0)
                        {
          device_found = true;
                          break;
                        }
        else
          device_found = false;
      }
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());*/
    }
};

void setup() {
  Serial.begin(115200); //Enable UART on ESP32
  Serial.println("Scanning..."); // Print Scanning
  //pinMode(2, OUTPUT); //make BUILTIN_LED pin as output
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.display(); 

  esp_task_wdt_init(20,true);
  esp_task_wdt_add(NULL);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); //Init Callback Function
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100); // set Scan interval
  pBLEScan->setWindow(99);  // less or equal setInterval value
}
void loop() {
  // put your main code here, to run repeatedly:
   unsigned long currentMillis = millis();
   if ( currentMillis >= interval ){ 
      // previousMillis = currentMillis; 
         // Added to repeatedly reset the Watch Dog Timer      
        }
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
 /* for (int i = 0; i < foundDevices.getCount(); i++)
  {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    int rssi = device.getRSSI();
    Serial.print("RSSI: ");
    Serial.println(rssi);
    if (rssi > RSSI_THRESHOLD && device_found == true){
      digitalWrite(2, HIGH);
      Serial.println("attack");}
    else
      digitalWrite(2, LOW);
  }*/
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(500);

}



