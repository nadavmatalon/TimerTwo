//TimerTwo Example

#include <TimerTwo.h>

void setup() {
  pinMode(TIMER2_PIN, OUTPUT);

  Serial.begin(9600);
  delay(2);
  Serial.println("You can issue commands like:");
  Serial.println("  12345p - set period to 12345 microseconds");
  Serial.println("  o - turn on a simple counter as the overflow function");
  Serial.println("  n - turn off the overflow function");
  Serial.println("  b - print the counter from the oveflow function");
  Serial.println();
  TimerTwo::setPeriod(200);
  TimerTwo::enable();
}

// variables shared between interrupt context and main program
// context must be declared "volatile".
volatile unsigned long burpCount = 0;

void Burp(void) {
  burpCount++;
}

void loop() {
  static unsigned long v = 0;
  if ( Serial.available()) {
    char ch = Serial.read();
    switch(ch) {
      case '0'...'9':
        v = v * 10 + ch - '0';
        break;
      case 'p':
        TimerTwo::setPeriod(v);
        Serial.print("set ");
        Serial.print((long)v, DEC);
        Serial.print(" = ");
        Serial.print((long)TimerTwo::getPeriod(), DEC);
        Serial.println();
        v = 0;
        break;
      case 'r':
        Serial.print("period is ");
        Serial.println(TimerTwo::getPeriod());
        break;
      case 'e':
        TimerTwo::enable();
        break;
      case 'd':
        TimerTwo::disable();
        break;
      case 'o':
        TimerTwo::setOnOverflow(Burp);
        break;
      case 'n':
        TimerTwo::setOnOverflow(0);
        break;
      case 'b':
        unsigned long count;
        noInterrupts();     // disable interrupts while reading the count
        count = burpCount;  // so we don't accidentally read it while the
        interrupts();       // Burp() function is changing the value!
        Serial.println(count, DEC);
        break;
    }
  }
}

