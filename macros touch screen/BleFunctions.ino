void printBle(String input){
    int delayAmount = 10;
 for(int i=0; i< input.length() ; i++){
                bleKeyboard.print(input.charAt(i));
                 delay(delayAmount);

     }
}
void retUPForward(){
      int delayAmount = 10;

  bleKeyboard.write(KEY_RETURN );delay(delayAmount); 
  bleKeyboard.write(KEY_UP_ARROW);delay(delayAmount); 
  bleKeyboard.write(KEY_RIGHT_ARROW);delay(delayAmount); 
}

void keyReturn(int numberOfbackArrows){
    int delayAmount = 10;

        for(int i=0; i<numberOfbackArrows; i++){
                 bleKeyboard.write(KEY_RETURN ); delay(delayAmount);
        }
}
void keyBackspace(int numberOfbackArrows){
    int delayAmount = 10;

        for(int i=0; i<numberOfbackArrows; i++){
                 bleKeyboard.write(KEY_BACKSPACE ); delay(delayAmount);
        }
}

void backArrow(int numberOfbackArrows){
    int delayAmount = 10;

    for(int i=0; i<numberOfbackArrows; i++){
        bleKeyboard.write(KEY_LEFT_ARROW);delay(delayAmount); 
    }
}

void forwardArrow(int numberOfforwardArrows){
    int delayAmount = 10;

    for(int i=0; i<numberOfforwardArrows; i++){
        bleKeyboard.write(KEY_RIGHT_ARROW);delay(delayAmount); 
    }
}
void upArrow(int numberOfUPArrows){
    int delayAmount = 10;

    for(int i=0; i<numberOfUPArrows; i++){
        bleKeyboard.write(KEY_UP_ARROW);delay(delayAmount); 
    }
}
void printReturnfirst(String input){
  int delayAmount = 10;
   bleKeyboard.write(KEY_RETURN ); delay(delayAmount);
   bleKeyboard.write(KEY_RETURN ); delay(delayAmount);
   bleKeyboard.write(KEY_UP_ARROW);delay(delayAmount); 

   for(int i=0; i< input.length() ; i++){
                bleKeyboard.print(input.charAt(i));
                 delay(delayAmount);

     }
   }



void curlyMacroEnding(int numberOfSpaces){
  int delayAmount = 10;

              //  bleKeyboard.write(KEY_LEFT_ARROW);delay(delayAmount); 
              bleKeyboard.write(KEY_BACKSPACE );delay(delayAmount);
              //  bleKeyboard.write(KEY_RETURN ); delay(delayAmount);
               bleKeyboard.write(KEY_DOWN_ARROW ); delay(delayAmount);               

               // bleKeyboard.write(KEY_RETURN ); delay(delayAmount);
               bleKeyboard.print("}"); delay(delayAmount);
                bleKeyboard.write(KEY_LEFT_ARROW);delay(delayAmount); 
                 bleKeyboard.write(KEY_RETURN ); delay(delayAmount);
               bleKeyboard.write(KEY_UP_ARROW ); delay(delayAmount);               
                bleKeyboard.write(KEY_UP_ARROW ); delay(delayAmount);

      
    for(int i=0; i<numberOfSpaces; i++){
              bleKeyboard.write(KEY_RIGHT_ARROW);
                    delay(delayAmount);

    }
  
}