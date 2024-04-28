#include <Keypad.h>

const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad

int KeyTone = 0;

bool NewCode = false;

String CodeTry;

String Code = String("882426");


//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]= 
{
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {2,3,4,5}; //Rows 0 to 3
byte colPins[numCols]= {6,7,8,9}; //Columns 0 to 3

//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup(){
  Serial.begin(9600);
}

//If key is pressed, this key is stored in 'keypressed' variable
//If key is not equal to 'NO_KEY', then this key is printed out
//if count=17, then count is reset back to 0 (this means no key is pressed during the whole keypad scan process

void loop(){
  char keypressed = myKeypad.getKey();
  
  if (keypressed != NO_KEY){        //Tuşa basıldı

    if(NewCode){                    // # tuşuna basıldı ise yeni koda giriliyor
      CodeTry += keypressed;

      Serial.println(CodeTry);

      KeyTone = int(keypressed) * 40;
      tone(11, KeyTone, 60);
    }
    else{
      tone(11, 100, 10);
    }
    
    if (CodeTry.length() > 6){
      CodeTry = "";
      NewCode = false;
    }


    if (CodeTry == Code){
      CodeTry = "";
      NewCode = false;
      Serial.println("Correct Code");
      tone(11, 900, 100);
      delay(100);
      tone(11, 900, 100);
      delay(200);
      tone(11, 1500, 300);
    }

    if ( int(keypressed) == 35){
      KeyTone = int(keypressed) * 40;
      tone(11, KeyTone, 60);

      Serial.println("Passcode entry");
      NewCode = true;
      CodeTry = "";
    }

    delay(100);
  }

}

