#include "CWLibrary.hpp"
/***   NOTES ON LIBRARY   ***/
/*
   - useful: https://github.com/bobboteck/CWLibrary/blob/master/README.md
   - supports uppercase letters A-Z (ASCII: 65-90) and numbers 0-9 (ASCII: 48-57)
   - supported special char's: { }  {"}  {'}  {(}  {)}  {,}  {-}  {.}  {/}  {:}  {=}  {?}  {@}
                   - in ASCII: {32} {34} {39} {40} {41} {44} {45} {46} {47} {58} {61} {63} {64}
   - allowed ASCII values: {32, 34, 39-41, 44-58, 61, 63-90}
*/

/***   CWLibrary Initialisation   ***/
void ActiveAction(void);
void noActiveAction(void);
int transmissionSpeed = 10;
CWLibrary cw = CWLibrary(transmissionSpeed, ActiveAction, noActiveAction); // SYNTAX of the function: CWLibrary(transmission speed, function
                                                                           // for presence of signal, function for absence of signal)

/***   Global Variable Initialisation   ***/
const int redLED = 2;
const int greenLED = 3;
const int piezo = 4;
const int beepLED = 5;
int numOfWords = 1;
int piezoFreq = 1000;


void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(piezo, OUTPUT);
  pinMode(beepLED, OUTPUT);

  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, HIGH);

  Serial.begin(9600);
  while (!Serial) {;}

  printWelcomeBanner();
}


void loop() {
  
  if (Serial.available() > 0) {

    String rawInput = "";

    rawInput.concat(Serial.readString());
    Serial.flush();
    rawInput.toUpperCase();

    char formattedInput[rawInput.length()+1];
    rawInput.toCharArray(formattedInput, sizeof(formattedInput));
    bool isValidChar = true;

    for (byte i=0; i<(sizeof(formattedInput) - 1); i++) {
      if (formattedInput[i] == 32 || formattedInput[i] == 34 || (39 <= formattedInput[i] && formattedInput[i] <= 41) || (44 <= formattedInput[i] && formattedInput[i] <= 58) || formattedInput[i] == 61 || (63 <= formattedInput[i] && formattedInput[i] <= 90)) {
        ;
      }
      else {
        Serial.println("At least 1 invalid character entered. Please try again.");
        Serial.println("-------------------------------------------------------\n");
        
        isValidChar = false;
        break;
      }
    }
    
    if (isValidChar) {
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, HIGH);
      delay(250);

      Serial.print("Word #");
      Serial.print(numOfWords);
      Serial.print(": '");
      Serial.print(formattedInput);
      Serial.println("'");
      for (byte i=0; i < ((sizeof(formattedInput)-1) + 11); i++) {
        Serial.print("-");
      }
      Serial.println("\n");

      cw.sendMessage(formattedInput);

      numOfWords++;
      delay(250);
      digitalWrite(greenLED, LOW);
      digitalWrite(redLED, HIGH);
    }
  }
}


/***   Simple Welcome Banner, During Initial Setup   ***/
void printWelcomeBanner() {
  delay(1000);
  
  Serial.println("\n\n\n");
  Serial.println("###################################################################");
  Serial.println("#                      Morse Code Translator                      #");
  Serial.println("#               ----===========================----               #");
  Serial.println("#   >Takes user input and translates it audibly and visibly to    #");
  Serial.println("#    Morse Code.                                                  #");
  Serial.println("#   >TO USE: Simply type your input into the serial moniter.      #");
  Serial.println("#   >Valid characters include only A-Z, 0-9, and special          #");
  Serial.println("#    characters enclosed in brackets: { } {\"} {'} {(} {)}         #"); // extra space needed to compensate for backslash in {\"}, which does not appear in Serial Moniter
  Serial.println("#    {,}  {-}  {.}  {/}  {:}  {=}  {?}  {@}.                      #");
  Serial.println("###################################################################");
  Serial.println("\n");
}


/***   User-defined Functions, from Morse (CW) library   ***/
void ActiveAction() {   //what will happen when a signal is PRESENT 
  tone(piezo, piezoFreq);
  digitalWrite(beepLED, HIGH);
}

void noActiveAction() {   //what will happen when a signal is ABSENT
  noTone(piezo);
  digitalWrite(beepLED, LOW);
}
