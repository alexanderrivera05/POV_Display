#define RES 360

#define CHARSTRETCH 5

volatile unsigned long frameCount = 0;
volatile int degree = 0;

volatile byte buf[2][RES]; //two buffers one to load and one to display
volatile byte activeBuf = 0, lastActiveBuf=0;

#include "characters.h"

  
void setup() {
  //setting outputs
  DDRD = (B11111100); //PG. 59 
  PORTD = (B00000000); //PG. 72 
  
  cli();
  //TIMER 0 STUFF
  TCCR0A = (1 << WGM01); //CTC Mode
  TCCR0B = (1 << CS01) | (1 << CS00); // 64 prescaler
  TIMSK0 = (1 << OCIE0A); //Trigger Interrupt on Compare Match 
  OCR0A=0xFF;
  
  //TIMER 1 STUFF
  TCCR1A = 0x00;
  TCCR1B = (1 << ICES1); //PG110 rising edge interrupt
  TCCR1B |= (1 << CS11) | (1 << CS10); //PG110 Set Prescaler to 64
  TIMSK1 = (1 << ICIE1); //PG112 enable input capture interrupt
  TCNT1 = 0;
  sei();  
}

void loop() {
  if(activeBuf != lastActiveBuf) { //keep track of each buffer
    lastActiveBuf=activeBuf;
    printString("ALEX ALEX", (frameCount%360));
  }
}

ISR(TIMER1_CAPT_vect){
  OCR0A = ICR1/RES;
  TCNT1 = 0;
  TCNT0 = 0;
  frameCount++;
  degree=0;
  activeBuf = !activeBuf;
}

ISR(TIMER0_COMPA_vect){
  if(degree < RES) {
    PORTD = buf[!activeBuf][degree];
    degree++;
  } 
}

void printString(String str, int start) {
  int pos=start;
  for(int x=0; x<RES; x++) //clear buffer previously used buffer
    buf[activeBuf][x]=0;
  for(byte x=0; x<str.length(); x++) { //enter data from characters.h to the loading buffer
    char currentChar=str.charAt(x);
    int charInd=-1;
    if(currentChar>='A' && currentChar<='Z') charInd=currentChar-'A'; //check if character is valid using ascii values
    else if(currentChar>='0' && currentChar<='9') charInd=currentChar-'0'+26; //assign charInd to value from characters array
    else if(currentChar == ' ') charInd=36; 
    else continue; // skip everything below if not A-Z or 1-9 or space
    if(charInd == 36){
        for(byte y=0; y<CHARSIZ; y++) { //fill every line into buffer
          buf[activeBuf][pos]=characters[charInd][y];
          pos++;
          if(pos>=RES) pos=0;
        }
      }
    else{
      for(byte y=0; y<CHARSIZ; y++) { //fill every line into buffer
        for(byte z=0; z<CHARSTRETCH; z++) { //stretch out each line to take 5 degrees
          buf[activeBuf][pos]=characters[charInd][y];
          pos++;
          if(pos>=RES) pos=0;
        }
      }
    }
    pos+=CHARSTRETCH*2; //space between letters 
    if(pos>=RES) pos-=RES;
  }
}
