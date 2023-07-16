/*
 * A simple program to test push buttons
 */

#define SWITCH_PIN 8
int buttonState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(SWITCH_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  buttonState = digitalRead(SWITCH_PIN);
  if (buttonState == HIGH) {
    Serial.print("Button is high\n");
    } else {
      Serial.print("Button is Low\n");
    }
    delay(1000);
}
