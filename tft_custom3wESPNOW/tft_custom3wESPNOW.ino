/*
  The TFT_eSPI library incorporates an Adafruit_GFX compatible
  button handling class, this sketch is based on the Arduin-o-phone
  example.

  This example diplays a keypad where numbers can be entered and
  send to the Serial Monitor window.

  The sketch has been tested on the ESP8266 (which supports SPIFFS)

  The minimum screen size is 320 x 240 as that is the keypad size.
*/

// The SPIFFS (FLASH filing system) is used to hold touch screen
// calibration data

#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library
#include <iostream>

#include "TFT_SETUP.h" 
#include "TFT_dorianStuff.cpp"

#include <esp_now.h>
#include <WiFi.h>

#include "ledDictionary.h"
#include "basicTimer.cpp"


////////move to library
//#define ILI9341_DRIVER   in user seput. also pins are defined here
TFT_eSPI gTft = TFT_eSPI(); // Invoke custom library normally TFT_ESPI takes 2 arguments but you change sreen size in user setup


class TouchInfo {
public:
uint16_t t_x;
uint16_t t_y;
bool pressed;
uint16_t numOfKeys;
//pointer to function
TouchInfo(uint8_t numKeys){ numOfKeys = numKeys;}

};
//TouchInfo  touchInfo;  //  = 15; //doesn't work compile time error
#define NUMOFKEYS 3
TouchInfo touchInfo(NUMOFKEYS);

// Using two fonts since numbers are nice when bold
#define LABEL1_FONT &FreeSansBold12pt7b // Key label font 1
#define LABEL2_FONT &FreeSansBold12pt7b    // Key label font 2

// Create 15 keys for the keypad
DButton key[NUMOFKEYS]={DButton("Red"),DButton("Blue"),DButton("Green")};
int pink = gTft.color565(255, 50, 100);


// 1A 0x0C,0xB8,0x15,0xF7,0x3E,0x10
// 1B 0x24,0x6F,0x28,0xB2,0x47,0x5C

//bread Board previous  0x24,0x6F,0x28,0xB2,0x31,0x54 
uint8_t broadcastAddress[] = {0x24,0x6F,0x28,0xB2,0x47,0x5C}; 
String success;

//  uint8_t outRED; uint8_t outGRN; uint8_t outBLU;
  uint8_t incomingRED; uint8_t incomingGRN; uint8_t incomingBLU;


typedef struct struct_message {
  uint8_t RED; uint8_t GRN; uint8_t BLU;
} struct_message;

struct_message message;  struct_message incomingReadings;

esp_now_peer_info_t peerInfo;

bool colorHasChanged = false;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0) {
    success = "Delivery Success :)";
  } else {
    success = "Delivery Fail :(";
  }
}
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingRED = incomingReadings.RED;  incomingGRN = incomingReadings.GRN; incomingBLU = incomingReadings.BLU;
  Serial.println(incomingRED); Serial.println(incomingGRN);  Serial.println(incomingBLU);
}


SemaphoreHandle_t xBinarySemaphore;

TaskHandle_t Task1;
TaskHandle_t Task2;

//------------------------------------------------------------------------------------------

void setup() {
  // Use serial port
  Serial.begin(115200);

  // Initialise the TFT screen
  gTft.init();

  // Set the rotation before we calibrate
  gTft.setRotation(0);
// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
  touch_calibrate(gTft,false);  


  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

    esp_now_register_send_cb(OnDataSent);

 // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }


  // Register for a callback function that will be called when data is received
 esp_now_register_recv_cb(OnDataRecv);


  message.RED = 32; message.GRN = 32; message.BLU = 32;

  // Draw keypad
  drawKeypad();
}

//------------------------------------------------------------------------------------------
void loop(void) {

 runScreen(gTft);


  
//KNOWN MAC ADDRESS
static BasicTimer16 sendTimer(10000);
  
if(sendTimer.run(millis()) || colorHasChanged){
  colorHasChanged =false;
    Serial.print("every 10 seconds");  Serial.println(millis());

esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
   
//RANDOM MAC ADDRESS
  // Send message via ESP-NOW
  //esp_err_t result;
  //if(hasAddress)
 //  result = esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));

   if (result == ESP_OK) {
    Serial.println("Sent with success");
   } else {   Serial.println("Error sending the data");  }
  }
}

//------------------------------------------------------------------------------------------

void drawKeypad()// args  tft, keys 
{

    tftElementDefiner def(gTft, 240,320);   //def.setScreen(gTft, 240,320);
    gTft.setFreeFont(LABEL1_FONT);

  
     tftStyler styler;
     
     //draw background///////////////////////////////////////////////////////////////////////////////
     gTft.fillScreen(gTft.color565(0, 0, 48));
    styler.colors(gTft.color565(0, 0, 0),  gTft.color565(64, 64, 0) , TFT_BLACK);
    styler.shape(3, -4,  -4);

    uint8_t rows = 160/10; uint8_t cols  = 120/8;

   for(int x=0; x<rows; x++){
   for(int y=0; y< cols; y++){
  int radius = -2;
   for(int i=0; i<3; i++){ //border thickness
     radius++;
      styler.shape(3, radius,  radius);

      def.createElipse(1  ,styler , rows, x , cols,y);
           }
   }}

     //end background///////////////////////////////////////////////////////////////////////////////

  uint8_t numRow =4; uint8_t numCol = 4;

    styler.colors(TFT_RED,  gTft.color565(0, 255, 255) , TFT_BLACK); //fill,border,text
    styler.shape(3, -20,  -20); //radius , -width, -height
    def.createButton( key[0] , styler , numCol,0, numRow, 1,   2,2); //key,styler, numOfColums, col#, numOfRow, row#, NumOfColWidth, NumOfRowHeight
         key[0].drawButton();   //


      styler.shape(20, -20,  -20);
    styler.colors(gTft.color565(0, 0, 255),  gTft.color565(0, 255, 255), TFT_BLACK);
  def.createButton( key[1] , styler , numCol,2 , numRow,1,  2,2);
        key[1].drawButton();


      styler.shape(30, -20,  -20);
    styler.colors(TFT_DARKGREEN,  gTft.color565(0, 255, 255), TFT_BLACK);
   //  def.createButton( key[2] , styler , numCol, 0, numRow,3,  4,1);//you could also draw the button this way by setting numCol = 1
        def.createButton( key[2] , styler , 1, 0, numRow,3);// this and the above row draw the same button.

        key[2].drawButton();



}



void hasTouchScreenBeenTouched(TFT_eSPI_Button *key,  TouchInfo touchInfo){

   // / Check if any key coordinate boxes contain the touch coordinates
  for (uint8_t b = 0; b < touchInfo.numOfKeys; b++) {
    if (touchInfo.pressed && key[b].contains(touchInfo.t_x, touchInfo.t_y)) {
      key[b].press(true);  // tell the button it is pressed
    } else {
      key[b].press(false);  // tell the button it is NOT pressed
    }
  }

}
void runScreen(TFT_eSPI &tft){ //args key, data // mutex?  chage gTft to tft

   tftElementDefiner def(gTft, 240,320);   //def.setScreen(gTft, 240,320);
   tftStyler textBoxStyler;

   //used for text box
 textBoxStyler.colors(gTft.color565(0, 32, 32),  gTft.color565(0, 255, 255) , pink);
   textBoxStyler.shape(3, -10,  -10);

 //uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
  // Pressed will be set true is there is a valid touch on the screen

  //these two lines should be changed
  touchInfo.pressed = tft.getTouch(&touchInfo.t_x, &touchInfo.t_y);
  hasTouchScreenBeenTouched(key, touchInfo);

  // Check if any key has changed state
tft.setFreeFont(LABEL1_FONT);

for(int b=0; b<3; b++){
         key[b].justReleasedDraw();
    if(  key[b].justPressedDraw(true, "CAT")){ //returns bool

      colorHasChanged= true;

       if (b == 0) {
        def.createRectangle(0,textBoxStyler, 1, 0, 5,0);
        def.createText18MC("Kitty", textBoxStyler, 1,0, 5,0);

  message.RED = 64; message.GRN = 0; message.BLU = 0;

      }
      if (b == 1) {
        def.createRectangle(0,textBoxStyler, 1, 0, 5,0);
        def.createText18MC("Meow", textBoxStyler, 1,0, 5,0);

  message.RED = 0; message.GRN = 0; message.BLU = 64;

      }

      if (b == 2) {
        def.createRectangle(0,textBoxStyler, 1, 0, 5,0);
        def.createText18MC("zzzzz", textBoxStyler, 1,0, 5,0);

    message.RED = 0; message.GRN = 64; message.BLU = 0;

      }
      
      delay(10); // UI debouncing
    }
  }// for
} 



//------------------------------------------------------------------------------------------

