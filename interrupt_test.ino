int state = 0;

void setup() {
  // put your setup code here, to run once:
  DDRD = B11111100;
  PORTD = B00000000;

  PCICR = (1 << PCIE0); //enabling pcint0 interrupt
  PCMSK0 = (1 << PCINT0);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

ISR(PCINT0_vect){
    if(PINB & B00000001){
        if(state == 0){
            PORTD = B11111100;
            state = !state;
          }else if(state == 1){
              PORTD = B00000000;
              state = !state;
            }
      }
  }
