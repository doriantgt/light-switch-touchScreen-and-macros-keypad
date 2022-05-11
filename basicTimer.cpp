
#ifndef basicTimer_cpp
#define basicTimer_cpp
#include <iostream>

class BasicTimer16 {
public:
  uint16_t previousTime = 0;
  uint16_t interval;

  BasicTimer16(uint16_t setInterval) {
    interval = setInterval;
  }

  bool run(uint16_t currentTime) {

    if ((uint16_t)(currentTime - previousTime) > interval) {

      previousTime = currentTime;


    } else {
      return false;
    }
    return true;
  }
};


#endif