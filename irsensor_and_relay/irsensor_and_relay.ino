
/*
 * A simple sketch to control a relay based on the distance measured with
 * an IR sensor. In this case an Sharp GP2Y0A21YK0F (10-80cm range).
 * https://www.amazon.fr/gp/product/B07MY27W7T/ref=ppx_yo_dt_b_asin_title_o07_s00?ie=UTF8&psc=1
 * 
 * The relation between the tension and the distance is given with ZSharpIR :
 * https://github.com/zoubworldArduino/ZSharpIR
 * 
 */

// The distance above which set relay to LOW
#define DISTANCE 60.

// The Relay pin number
#define RELAY_PIN 7


float distanceAvg = 80.;

void setup() {

  Serial.begin(9600);
  Serial.println("Init Success");

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  
}
  
void loop() {
   
    int sensorValue = analogRead(A0);
    float voltage = sensorValue * (5.0 / 1023.0);
    float distance = 29.988 * pow(voltage , -1.173);

    // set bounds...
    if (distance > 100. ) {
      distance = 100.;
    }

    if (distance < 10. ) {
      distance = 10.;
    }

    if (distanceAvg < DISTANCE) {
      digitalWrite(RELAY_PIN, LOW);
    }
    else {
      digitalWrite(RELAY_PIN, HIGH);
    }

    // Compute the average over the last n=5 values with this formula:
    // new_average = (old_average * (n-1) + new_value) / n
    distanceAvg =  (distanceAvg * 4. + distance) / 5.;
    Serial.print("Voltage : ");
    Serial.print(voltage);
    Serial.print("V Distance : ");
    Serial.print(distance);
    Serial.print("cm  Avg Distance : ");
    Serial.print(distanceAvg);
    Serial.println("cm ");

    // wait for a second
    delay(1000);
    
}
