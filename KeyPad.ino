#include <Servo.h>

#include <Keypad.h>

const byte numRows= 4;      //number of rows on the keypad
const byte numCols= 4;      //number of columns on the keypad

const int BuzzerPin= 11;    //Buzzer positive to pin 11

int KeyTone = 0;            //Default KeyTone is blank

bool NewCode = false;  //is true when # is pressed

String CodeTry;

String Code = String("882426");  //Create a static code

Servo LockServo;

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]= 
{
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

//Code that shows the the keypad connections to the arduino pins
byte rowPins[numRows] = {2,3,4,5};     // Rows 0 to 3
byte colPins[numCols]= {6,7,8,9};      // Columns 0 to 3

//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup(){
  Serial.begin(9600);
  LockServo.attach(10);                // Servo connected to pin 10
  LockServo.write(90);                 // Turn servo to 90 degrees to unlock
}

void loop(){
  char keypressed = myKeypad.getKey();
  
  if (keypressed != NO_KEY){          // Any key is pressed
    

    if(NewCode){                    // Let the user enter a code is "#" is pressed
      CodeTry += keypressed;        // Appends new key to the CodeTry string

      Serial.println(CodeTry);      // Debug print to see the current trial 

      KeyTone = int(keypressed) * 40;    // Play different tones based on key pressed
      tone(BuzzerPin, KeyTone, 60);
    }
    else{
      tone(BuzzerPin, 100, 10);      // If "#" is not pressed (at boot or after a wrong try)
    }
    
    if (CodeTry.length() > 6){       // After 6 digits are entered finish the new code
      CodeTry = "";                  // Clear the CodeTry variable for a new entry
      NewCode = false;
    }


    if (CodeTry == Code){            // If the entered code is the same with the static code

      LockServo.write(90);           // Unlock door
      CodeTry = "";                  // Clear the CodeTry variable for a new entry
      NewCode = false;                
      Serial.println("Correct Code");   // Debug print
      tone(BuzzerPin, 900, 100);        // Play success tone  
      delay(100);
      tone(BuzzerPin, 900, 100);
      delay(200);
      tone(BuzzerPin, 1500, 300);
    }

    if ( int(keypressed) == 35){         // If "#" key is pressed (turning the key into an integer then comparing to the corresponding number)
      LockServo.write(130);              // Lock the door!
      
      KeyTone = int(keypressed) * 40;    // Play the "#" keys tone
      tone(BuzzerPin, KeyTone, 60);

      Serial.println("Passcode entry");  // Debug print
      NewCode = true;                    // Start a new code
      CodeTry = "";                      // Clear the CodeTry variable for a new entry
    }

    delay(100);                  // Small delay for a more stable run
  }

}

