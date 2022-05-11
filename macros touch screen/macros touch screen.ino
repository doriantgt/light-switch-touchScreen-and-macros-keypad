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
#include "TFT_SETUP.h"  //TFT_eSPI gTft = TFT_eSPI(); // Invoke custom libraryALL THE COMMENTED OUT FILES MOVED TO THIS FILE
#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library
#include "ledDictionary.h"
#include <iostream>
#include "TFT_dorianStuff.cpp"

#include <BleKeyboard.h>
BleKeyboard bleKeyboard;

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
#define NUMOFKEYS 15
TouchInfo touchInfo(NUMOFKEYS);


//lets build a popup

uint16_t testValue;


// Using two fonts since numbers are nice when bold
#define LABEL1_FONT &FreeSansBold12pt7b // Key label font 1
#define LABEL2_FONT &FreeSansBold12pt7b    // Key label font 2

// Numeric display box size and location

int pink = gTft.color565(255, 50, 100);


// Create 15 keys for the keypad
uint16_t keyColor[NUMOFKEYS] = {TFT_RED, TFT_DARKGREY, TFT_DARKGREEN   };

// Invoke the TFT_eSPI button class and create all the button objects


//------------------------------------------------------------------------------------------

void setup() {
  // Use serial port
  Serial.begin(115200);
  bleKeyboard.begin();

  // Initialise the TFT screen
  gTft.init();

  // Set the rotation before we calibrate
  gTft.setRotation(0);
// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
//#define REPEAT_CAL TRUE  //CHANGED TO FUNCTION ARGUMENT
  // Calibrate the touch screen and retrieve the scaling factors
  touch_calibrate(gTft,false);  

  // Clear the screen
 // gTft.fillScreen(TFT_BLACK);

  // Draw keypad background
  gTft.fillRect(0, 0, 240, 320, gTft.color565(64, 0, 0));


  // Draw keypad
  drawKeypad();
}

//------------------------------------------------------------------------------------------
void loop(void) {

 runScreen(gTft);
  
}

//------------------------------------------------------------------------------------------
DButton key[NUMOFKEYS];//{DButton("for i"),DButton("for j"),DButton("for k")};

void drawKeypad()// args  tft, keys 
{

    tftElementDefiner def(gTft, 240,320);   //def.setScreen(gTft, 240,320);
    gTft.setFreeFont(LABEL1_FONT);

  
     tftStyler styler;
     
     //draw background///////////////////////////////////////////////////////////////////////////////
    styler.colors( gTft.color565(0, 0, 0),  gTft.color565(0, 100, 100) , TFT_BLACK);
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
   uint8_t keyNum =0;
  uint8_t numRow =4; uint8_t numCol = 3;
    styler.shape(5, -15,  -15);


  keyNum =2;
 key[keyNum].setLabel("u32");
    styler.colors(gTft.color565(0, 255, 255),  gTft.color565(0, 255, 0) , TFT_BLACK);
    def.createButton( key[keyNum] , styler , numCol,0, numRow, 0,   1,1);
         key[keyNum].drawButton();

  keyNum =3;
   key[keyNum].setLabel("u16");
    styler.colors(gTft.color565(0, 255, 255),  gTft.color565(0, 255, 0) , TFT_BLACK);
    def.createButton( key[keyNum] , styler , numCol,1, numRow, 0,   1,1);
         key[keyNum].drawButton();

 keyNum =4;
   key[keyNum].setLabel("u8");
    styler.colors(gTft.color565(0, 255, 255),  gTft.color565(0, 255, 0) , TFT_BLACK);
    def.createButton( key[keyNum] , styler , numCol,2, numRow, 0,   1,1);
         key[keyNum].drawButton();


         keyNum =9;
    key[keyNum].setLabel("-u");
    styler.colors(gTft.color565(255, 0, 0),  gTft.color565(0, 255, 255) , TFT_BLACK);
    def.createButton( key[keyNum] , styler , numCol ,  2, numRow, 1,   1,1);
         key[keyNum].drawButton();

          keyNum =1;
   key[keyNum].setLabel("while");
    styler.colors(gTft.color565(255, 0, 255),  gTft.color565(0, 255, 255) , TFT_BLACK);
    def.createButton( key[keyNum] , styler , numCol,0, numRow, 3,   1,1);
         key[keyNum].drawButton();

    keyNum = 5;
   key[keyNum].setLabel("if+el");
    styler.colors(gTft.color565(255, 255, 0),  gTft.color565(0, 255, 255) , TFT_BLACK);
    def.createButton( key[keyNum] , styler , numCol,0, numRow, 1,   1,1);
         key[keyNum].drawButton();

 keyNum =6;
   key[keyNum].setLabel("elif");
    styler.colors(gTft.color565(255, 255, 0),  gTft.color565(0, 255, 255) , TFT_BLACK);
    def.createButton( key[keyNum] , styler , numCol,1, numRow, 2,   1,1);
         key[keyNum].drawButton();

  keyNum =7;
  key[keyNum].setLabel("else");
    styler.colors(gTft.color565(255, 255, 0),  gTft.color565(0, 255, 255) , TFT_BLACK);
    def.createButton( key[keyNum] , styler , numCol,0, numRow, 2,   1,1);
         key[keyNum].drawButton();

 keyNum =8;
  key[keyNum].setLabel("if");
    styler.colors(gTft.color565(255, 255, 0),  gTft.color565(0, 255, 255) , TFT_BLACK);
    def.createButton( key[keyNum] , styler , numCol,2, numRow, 2,   1,1);
         key[keyNum].drawButton();

 keyNum =12;
    key[keyNum].setLabel("array");
    styler.colors(gTft.color565(255, 255, 255),  gTft.color565(0, 255, 255), TFT_BLACK);
  def.createButton( key[keyNum] , styler , numCol  ,1 , numRow,1,  1,1);
        key[keyNum].drawButton();


 keyNum =10;
    key[keyNum].setLabel("forXY");
    styler.colors(gTft.color565(0, 255, 0),  gTft.color565(0, 255, 255), TFT_BLACK);
  def.createButton( key[keyNum] , styler , 3  ,1 , numRow,3,  1,1);
        key[keyNum].drawButton();

 keyNum =11;
    key[keyNum].setLabel("for i");
    styler.colors(gTft.color565(0, 255, 0),  gTft.color565(0, 255, 255), TFT_BLACK);
     def.createButton( key[keyNum] , styler , 3, 2, numRow,3,1,1);
        key[keyNum].drawButton();




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

for(int b=0; b<NUMOFKEYS; b++){
         key[b].justReleasedDraw();
    if(  key[b].justPressedDraw(true, "CAT")){ //returns bool

      

       if (b == 2) {printBle("uint32_t ");  }
       if (b == 3) {printBle("uint16_t ");  }
       if (b == 4) {printBle("uint8_t ");  }
       if (b == 9) {  backArrow(8); keyBackspace(1); forwardArrow(8);}  //printBle("-u");  } //subtract u

        if (b == 1) {printReturnfirst("while(){}");curlyMacroEnding(6);  }

        if (b == 8) {   printReturnfirst("if(){}");curlyMacroEnding(3);}
        if (b == 7) {printBle("else {}");backArrow(1); keyReturn(1);    }
        if (b == 6) { printBle("else if(){}");backArrow(1);  retUPForward();forwardArrow(6);}

        if (b == 5) { printReturnfirst("if(){}");keyBackspace(1);
              keyReturn(2);
              printBle("}else{}");backArrow(1); keyReturn(2); 
                upArrow(3);forwardArrow(1);
                }
      if (b == 10) {
    printReturnfirst("for(int x=0; x<; x++){}");keyBackspace(1);
    printReturnfirst("for(int y=0; y<; y++){}");keyBackspace(1);
          keyReturn(2);    
          printBle("}");    keyReturn(1);printBle("}");

      }

      if (b == 11) { 
       printReturnfirst("for(int i=0; i<; i++){}");curlyMacroEnding(15);
       }
       if (b == 12) {printBle(" [] = {};");backArrow(7);   }


///0test area

 ///0test 
 
 


     
       delay(10); // UI debouncing
    }
  }// for
} 


//------------------------------------------------------------------------------------------

