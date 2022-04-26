#include <DHT.h>
#include <Adafruit_Sensor.h>
#include "ThingSpeak.h"
#include "LiquidCrystal_I2C.h"
#include <Wire.h>                 //Thư viện giao tiếp I2C
#include "DHT.h"  // Including library for dht
#include <ESP8266WiFi.h>
LiquidCrystal_I2C lcd(0x27,16,2); //Thiết lập địa chỉ và loại LCD

String apiKey = "EYUMHDDPXGKM94UF";       // Enter your Write API key from ThingSpeak
const char* ssid = "Quan";           // Give your wifi network name
const char* password = "minhquan2000";   // Give your wifi network password
const char* server = "api.thingspeak.com";
#define DHTPIN 0      //pin D0 where the dht11 is connected
DHT dht(DHTPIN, DHT11);
WiFiClient client;

void setup() 
{
       Serial.begin(115200);
       delay(10);
       dht.begin();
       Serial.println("Connecting to ");
       Serial.println(ssid);
       WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");

      Wire.begin(D4,D5);               //Thiết lập chân kết nối I2C (SDA,SCL);
      lcd.init();                      //Khởi tạo LCD
      lcd.clear();                     //Xóa màn hình
      lcd.backlight();                 //Bật đèn nền

}

void loop() 
{
      float h = dht.readHumidity();
      float t = dht.readTemperature();
             /* if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                      delay(5000);
                 }
*/                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {    
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";

                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);

                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(h);
                             Serial.println("%. Send to Thingspeak.");
                        }
   
                 client.stop();

          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
          lcd.clear();
          lcd.setCursor(0,0);              //Đặt vị trí muốn hiển thị ô thứ 1 trên dòng 1
          lcd.print("Temp: ");                    //Ghi byte 0 ra vị trí ô thứ 1 trên dòng 1
          lcd.print(t, 1);
          lcd.print(char(223));
          lcd.print("C"); 
          lcd.setCursor(0,1);              //Đặt vị trí ở ô thứ 1 trên dòng 2
          lcd.print("Hum: ");   //Ghi đoạn text "E-smart Channel!" 
          lcd.print(int(h));
          lcd.print("%"); 
    
          lcd.display();

          delay(15000);
//          lcd.noDisplay();
//          delay(500);
}
