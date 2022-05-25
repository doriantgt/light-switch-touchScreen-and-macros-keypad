/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 16
#define DATA_PIN  4
#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];


#include <esp_now.h>
#include <WiFi.h>

//#include <Wire.h>
////#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>

//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

class BasicTimer16{
    public:
    uint16_t previousTime =0;
    uint16_t interval;
    
    BasicTimer16(uint16_t setInterval){
        interval = setInterval;
    }
    
    bool run(uint16_t currentTime){

       if((uint16_t)(currentTime - previousTime) > interval ){
            
            previousTime = currentTime;
            
            
       }else{return false;}
        return true;
    }
    
};

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)


// REPLACE WITH THE MAC Address of your receiver 
 //screen esp 80:7D:3A:A5:B0:94  // com 11
//this is esp com 4

// 1A 0x0C,0xB8,0x15,0xF7,0x3E,0x10
// 1B 0x24,0x6F,0x28,0xB2,0x47,0x5C
//1C 0x24,0xD7,0xEB,0x11,0xC8,0x20
//1D 0x24,0xD7,0xEB,0x11,0xAA,0x9C
//1e 0x94,0xB5,0x55,0xF9,0x0F,0xB0
//previous breadboard 0x80, 0x7D, 0x3A, 0xA5, 0xB0, 0x94

uint8_t broadcastAddress[] = {0x0C,0xB8,0x15,0xF7,0x3E,0x10};

//  uint8_t outRED; uint8_t outGRN; uint8_t outBLU;
  uint8_t incomingRED=32; uint8_t incomingGRN=32; uint8_t incomingBLU=32;


typedef struct struct_message {
  uint8_t RED=32; uint8_t GRN=32; uint8_t BLU=32;
} struct_message;

struct_message message;  struct_message incomingReadings;

// Variable to store if sending data was successful
String success;


esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  } else {
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
   incomingRED = incomingReadings.RED;  incomingGRN = incomingReadings.GRN; incomingBLU = incomingReadings.BLU;
  Serial.println(incomingRED); Serial.println(incomingGRN);  Serial.println(incomingBLU);


for(uint8_t i=0; i< NUM_LEDS; i++){
   leds[i] = CRGB(incomingRED,incomingGRN,incomingBLU);

}

   FastLED.show();
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
   FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical

 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
   

  // Set values to send
   for(uint8_t i=0; i< NUM_LEDS; i++){
   leds[i] = CRGB(incomingRED,incomingGRN,incomingBLU);

}
    FastLED.show();

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(10000);
}


