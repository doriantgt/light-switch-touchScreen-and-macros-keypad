#include <iostream>

struct _i{
const static uint8_t _=0;
const static uint8_t Rainbow=1;
const static uint8_t User=2;
const static uint8_t Save=3;
const static uint8_t Setting=4;
const static uint8_t Pallet=5;
const static uint8_t blend=6;
const static uint8_t chase=7;





};


const  char * _d[] = {   "", //0
                        "Rainbow ", //1
                         "User ", //2
                        "Save ", //3
                         "Setting ",//4
                         "zzz ", //5
                        
                         
                          "zzz "}; 


class tft_Screen{
    public:
uint8_t group; //M-mode S-settings U-user saved //this value not sent to strip  //all values below sent to strip
uint8_t name[3]; //C-chase B-blend R-Rainbow F-fade S-shooting P-pallet  //example PB pallet blend
uint8_t number; //maybe a number for multiple with same descriptors.


//tft_description
tft_Screen(uint8_t grp, uint8_t name0, uint8_t name1, uint8_t name2, uint8_t num ){
 group = grp;
 name[0] = name0;
 name[1] = name1;
 name[2] = name2;
 number = num;

}
};


tft_Screen screens[] = {  tft_Screen(_i::User ,_i::Rainbow ,_i::chase ,_i::_ ,0) };

