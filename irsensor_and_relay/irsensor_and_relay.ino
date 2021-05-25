
#include <LiquidCrystal.h>


void setup() {

  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  
}
  
void loop() {
   
    int sensorValue = analogRead(A0);
    float voltage = sensorValue * (5.0 / 1023.0);
    float distance = 24.65251/(voltage-0.1065759);

    if (distance < 55.) {
      digitalWrite(7, LOW);
    }
    else {
      digitalWrite(7, HIGH);
    }
    delay(2500);
    
    
}
