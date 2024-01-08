/*
 * Arduino solar panels station with an NPN anemometer for wind speed and two light
 * sensors for reading the light intensity. Depending on the intensity the arduino
 * controls a two relays that control an electric jack.
 */

/*
 * Anemometer initialization section
 * 
 * Read and count the pulses from a NPN anemometer.
 * 
 * I used this one from amazon :
 * https://www.amazon.fr/gp/product/B081RMGQRB/ref=ppx_yo_dt_b_asin_title_o07_s02?ie=UTF8&psc=1
 * 
 * Brown wire conncected to +5V
 * Black wire connceted to ground
 * Green wire connected to INPUT_PIN
 * 
 * According to the technical documentation the anemometer emits 20 pulses per rotation.
 * And 1 rotation per second is 1.75 m/s.
 * 
 * As in this sketch we count pulse changes form up to down and up to down, we have to
 * count 40 change state per turn.
 * 
 * Read this page for details: https://www.makerfabs.com/wiki/index.php?title=Anemometer
 */

// anemometer input pin
#define INPUT_PIN 5

// Variable declaration for anemomter
int counter = 0;
int pinState;
int pinLastState;

long previousMillis = 0;
float turnsPerSec;
int i;
unsigned long currentMillis;
float windSpeed;
float windSpeed_agv2m = 0.;
float windSpeedMph;
float windSpeedMph_agv2m = 0.;
float loopTime;
float loopMillis;


// The Relays pin numbers
#define RELAY1_PIN 7
#define RELAY2_PIN 6

// Variable declaration for ligth sensors
int sensorTop = 0;
int sensorBot = 0;
int diff = 0;
float avg = 0.;


#define WIND_SPEED_LIMIT 10.0

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

  // Init pin for the anemometer
  pinLastState = digitalRead(INPUT_PIN);

}

void loop() {
  // put your main code here, to run repeatedly:

  // Anemomter section
  currentMillis = millis();
  loopTime = 0.;
  windSpeed = 0.;
  counter = 0;
  
  // count the pulse changes in a loop for 1 second, so the wind speed is given every second.
  while (loopTime < 1000.) {
    pinState = digitalRead(INPUT_PIN); // Reads the "current" state of the INPUT_PIN

    // If the previous and the current state of the INPUT_PIN are different, that means a Pulse has occured
    if (pinState != pinLastState) {
        counter ++;
    }
    pinLastState = pinState; // Updates the previous state of the INPUT_PIN with the current state

    // add a small delay otherwise the loop wont work because it would be to fast.
    delay(10);
    loopMillis = millis();
    loopTime = loopMillis - currentMillis;
  }

  // wind speed in m/s
  windSpeed =  (counter / 40.) * 1.75;

  // A simple way to compute rolling average over n points ...
  // new_average = (old_average * (n-1) + new_value) / n

  // So we compute the average wind speed over 2 minutes, that is for n=120.
  windSpeed_agv2m = (windSpeed_agv2m * 119. + windSpeed )/ 120.;

  Serial.print("\nWind speed: \n");
  Serial.print(windSpeed);
  Serial.print("\nAvg Wind speed 2min: \n");
  Serial.print(windSpeed_agv2m);

  // Light sensor section
  sensorTop = analogRead(0);
  sensorBot = analogRead(1);

  diff = sensorTop - sensorBot;
  avg = (sensorTop + sensorBot) / 2.0;


  if (windSpeed_agv2m < WIND_SPEED_LIMIT) { 
    // The wind speed is acceptable
    if (((sensorTop < 900 ) && (sensorBot < 900 )) && (avg < 800) ){
      //diff = sensorTop - sensorBot;
      if (diff < -50) {
        // Case where more light in the bottom sensor
        // The jack should go downward
        Serial.print("\nRelay1 low");
        digitalWrite(RELAY2_PIN, HIGH);
        delay(100);
        digitalWrite(RELAY1_PIN, LOW);
      }
      else if ( diff > 50) {
        // Case where more light in the top sensor
        // The jack should go upward
        Serial.print("\nRelay2 low");
        digitalWrite(RELAY1_PIN, HIGH);
        delay(100);      
        digitalWrite(RELAY2_PIN, LOW);
      }
      else {
        Serial.print("\nEqual light. All relays high");
        digitalWrite(RELAY1_PIN, HIGH);
        digitalWrite(RELAY2_PIN, HIGH);
        delay(100);
      }
    }
    else {
      Serial.print("\nNo light. All relays high");
      digitalWrite(RELAY1_PIN, HIGH);
      digitalWrite(RELAY2_PIN, HIGH);
      }
    }
    else {
    // Case where the wind speed is too high
    // The jack should go upward to put the
    // solar panels in flat position
    Serial.print("\nWind speed too high");
    Serial.print("\nRelay2 low");
    digitalWrite(RELAY1_PIN, HIGH);
    delay(100);      
    digitalWrite(RELAY2_PIN, LOW);
    
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

//  delay(1000);
}
