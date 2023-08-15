//
// Simple example of time control with Arduino using interrupt
//

boolean s10Event = false;     // On seconds==0, 10, 20, 30, 40, 50 event
boolean secondEvent = false;  // On every second event
boolean e500msEvent = false;  // On every 500 ms event
boolean firsttime=true;       // Controls code in loop() to be executed once.
int minutes = 0;
int seconds = 0 ;  
int seconds2 = 0 ;


//
// ***** setup *****
//
void setup(){
  Serial.begin(115200);      // Initialize serial communication
//
// Set up interrupt system, timer 1, for interrupt every 0.5 second.
// timer1 (16 bits) counts from 0 to 65535 and is used for the PWM control of pins 9 and 10.
// It is also used by the Servo.h library

  cli();                     // Disable all interrupts
//set timer1 interrupt at 1Hz
  TCCR1A = 0;                // set entire timer 1 TCCR1A register to 0
  TCCR1B = 0;                // same for timer 1 TCCR1B
  TCNT1  = 0;                // initialize timer 1 counter value to 0
// set compare match register for 2hz increments
  OCR1A = 31249;             // 16 Mhz/512 - 1 = (16*10^6) / (2*256) - 1 (must be <65536)
// As example: How to set 1 second/1 Hz interrupt.
//  OCR1A = 15624;           // 16 Mhz/1024 = (16*10^6) / (1*1024) - 1 (must be <65536)
  TCCR1B |= (1 << WGM12);    // turn on CTC mode
  TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10); // Set CS12 and CS10 bits for 256 prescaler
// As example: How to set prescaler for 1 second/1 Hz interrupt.
//  TCCR1B |= (1 << CS12) | (1 << CS10); // Set CS12 and CS10 bits for 1024 prescaler
  TIMSK1 |= (1 << OCIE1A);   // enable timer compare interrupt
  sei();                     // Enable interrupts

  Serial.println("");
  Serial.println("setup done.");

} // End setup


//
// Interrupt handlers
//
ISR(TIMER1_COMPA_vect){ // Timer1 interrupt, 2Hz
  seconds2++;
  e500msEvent = true;
  if (seconds2<2) return;
  seconds2=0;
// Update second and minute counters.
  seconds++;
  if (seconds>59){
    seconds=0; minutes++;
    if (minutes>59) {minutes=0;}
  } // End of time count update

// Activate timer events
  if (10*floor(seconds/10)-seconds==0){s10Event = true;} // Every 10 second event
  secondEvent = true; // Every second event
} // End of interrupt handler


//
// ***** Main code, runs as infinite loop *****
//
void loop() {
  if (firsttime) {
// Do first time stuff.
// ...
    firsttime=false;
  }

// Handle every second event
  if (secondEvent){
    Serial.println("Handle every second event..."); 
    secondEvent = false;
  } // End secondEvent handler

//
// Do something every 10 seconds
//
  if (s10Event){
    Serial.println("Handle every ten second event..."); 
    s10Event = false;
  } // End s10Event handler

} // End of main loop().
