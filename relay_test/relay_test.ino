// a simple program to test my relays

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

  Serial.print("Relay1 low - Relay 2 High\n");
  digitalWrite(RELAY2_PIN, HIGH);
  delay(100);
  digitalWrite(RELAY1_PIN, LOW);

  delay(1000);

  Serial.print("Relay1 High - Relay 2 Low\n");
  digitalWrite(RELAY1_PIN, HIGH);
  delay(100);      
  digitalWrite(RELAY2_PIN, LOW);

  delay(1000);

}
