/*
 * Read an count the pulses from a NPN anemometer.
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
 */

// anemometer input pin
#define INPUT_PIN 7

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


void setup() {
    pinMode (INPUT_PIN, INPUT);
    Serial.begin(9600);
 
    Serial.println("Init Success");

    pinLastState = digitalRead(INPUT_PIN);
}


void loop() {

    currentMillis = millis();
    loopTime = 0.;
    windSpeed = 0.;
    windSpeedMph = 0.;
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
    // wind speed in mph
    windSpeedMph = windSpeed * 2.23694;

    // A simple way to compute rolling average over n points ...
    // new_average = (old_average * (n-1) + new_value) / n

    // So we compute the average wind speed over 2 minutes, that is for n=120.
    windSpeed_agv2m = (windSpeed_agv2m * 119. + windSpeed )/ 120.;

    // Same for the wind speed in mph.
    windSpeedMph_agv2m = (windSpeedMph_agv2m * 119. + windSpeedMph )/ 120.;

    printSerialValues();
}

void printSerialValues() {

    // Print values the serial port
    // the format is similar to json format or python dictionnary for easier usage.
    Serial.print("{ \"wind_speed\" : ");
    Serial.print(windSpeed);
    Serial.print(", \"wind_speed_avg2m\" : ");
    Serial.print(windSpeed_agv2m);
    Serial.print(", \"windspeedmph\" : ");
    Serial.print(windSpeedMph);
    Serial.print(", \"windspeedmph_avg2m\" : ");
    Serial.print(windSpeedMph_agv2m);
    Serial.println(" }");
}
