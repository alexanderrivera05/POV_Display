int compVal = 62500; //(16 MHz/((256)(1s)))

void setup() {
  // put your setup code here, to run once:
  //setting up output
  DDRD = B11111100;
  PORTD = B00000000;

  TCCR1A = 0;

  //Set Prescaler
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);
  TCCR1B &= ~(1 << WGM13);
  TCCR1B |= (1 << WGM12);

  TCNT1 = 0;
  OCR1A = compVal;

  TIMSK1 = (1 << OCIE1A);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
}

ISR(TIMER1_COMPA_vect){
    PORTD ^= B11111100;
  }
