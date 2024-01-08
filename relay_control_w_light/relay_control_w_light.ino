int sensorTop = 0;
int sensorBot = 0;
int diff = 0;
float avg = 0.;


// The Relays pin numbers
#define RELAY1_PIN 7
#define RELAY2_PIN 6


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Init Success");

  // Init relay 1
  pinMode(RELAY1_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, HIGH);

  // Init relay 2
  pinMode(RELAY2_PIN, OUTPUT);
  digitalWrite(RELAY2_PIN, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:

  sensorTop = analogRead(0);
  sensorBot = analogRead(1);
  diff = sensorTop - sensorBot;
  avg = (sensorTop + sensorBot) / 2.0;

  if (((sensorTop < 900 ) && (sensorBot < 900 )) && (avg < 800) ){
    //diff = sensorTop - sensorBot;
    if (diff < -50) {
      // Case where more light in the bottom sensor
      // The jack should go downward
      Serial.print("Relay1 low\n");
      digitalWrite(RELAY2_PIN, HIGH);
      delay(100);
      digitalWrite(RELAY1_PIN, LOW);
    }
    else if ( diff > 50) {
      // Case where more light in the top sensor
      // The jack should go upward
      Serial.print("Relay2 low\n");
      digitalWrite(RELAY1_PIN, HIGH);
      delay(100);      
      digitalWrite(RELAY2_PIN, LOW);
    }
    else {
      Serial.print("Equal light. All relays high\n");
      digitalWrite(RELAY1_PIN, HIGH);
      digitalWrite(RELAY2_PIN, HIGH);
      delay(100);
    }
  }
  else {
    Serial.print("No light. All relays high\n");
    digitalWrite(RELAY1_PIN, HIGH);
    digitalWrite(RELAY2_PIN, HIGH);
    }
  //}
//  Serial.print("A0: ");
//  Serial.print(sensorTop);
//  Serial.print(" - A1: ");
//  Serial.print(sensorBot);
//  Serial.print(" - avg: ");
//  Serial.print(avg);
//   Serial.print(" - diff: ");
//  Serial.print(diff);
//  Serial.print("\n");

  delay(1000);
}
