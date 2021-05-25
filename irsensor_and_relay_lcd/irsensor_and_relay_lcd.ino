//#include <SharpIR.h>

#include <LiquidCrystal.h>


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//SharpIR sensorName("GP2Y0A21YK0F", A0);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD
  //lcd.print("hello, world!");

  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  
}
  
void loop() {
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0,1);

    // print the number of seconds since reset:
//    lcd.print("Counter:");
//    lcd.print(millis() / 1000);


//    lcd.print("Distance:");
//    lcd.print(sensorName.getDistance());
//    
    int sensorValue = analogRead(A0);
    float voltage = sensorValue * (5.0 / 1023.0);
    float distance = 24.65251/(voltage-0.1065759);

    lcd.setCursor(0,0);
    lcd.print("Volatge:");
    lcd.print(voltage);
    lcd.setCursor(0,1);
    lcd.print("Distance:");
    lcd.print(distance);
    if (distance < 55.) {
      digitalWrite(7, LOW);
    }
    else {
      digitalWrite(7, HIGH);
    }
    delay(2500);

}
