/*
 * Arduino wheather station with an NPN anemometer for wind spee and a BME280 
 * sensor for temperature and atmospheric pressure reading.
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


/*
 * BME280 initialization section
 */
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//if you need to read altitude,you need to know the sea level pressure
#define SEALEVELPRESSURE_HPA (1013.25)

//This Macro definition decide whether you use I2C or SPI
//When USEIIC is 1 means use I2C interface, When it is 0,use SPI interface
#define USEIIC 1

/*
This Demo is tested on UNO PLUS
SPI:
SPI_SCK:  D13
SPI_MISO: D12
SPI_MOSI: D11
SPI_CS:   D10

I2C:
I2C_SCL:  A5
I2C_SDA:  A4
the default I2C address is 0x77, you can change it in Adafruit_BME280.h
*/

#if(USEIIC)
  Adafruit_BME280 bme;
#else
  #define SPI_SCK 13
  #define SPI_MISO 12
  #define SPI_MOSI 11
  #define SPI_CS 10
  Adafruit_BME280 bme(SPI_CS, SPI_MOSI, SPI_MISO, SPI_SCK);
#endif

unsigned long delayTime;
float temp;
float pressure;
float humidity;
float alt;



void setup() {
    pinMode (INPUT_PIN, INPUT);
    Serial.begin(9600);
  
    bool rslt;
    rslt = bme.begin();  
    if (!rslt) {
        Serial.println("BME280 sensor init Fail, Please Check your address or the wire you connected!!!");
        while (1);
    }
  
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

    // Read data from the BME280
    temp = bme.readTemperature();
    pressure = bme.readPressure() / 100.0F;
    humidity = bme.readHumidity();

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
    Serial.print(", \"temp\" : ");
    Serial.print(temp);
    Serial.print(", \"pressure\" : ");
    Serial.print(pressure);
    Serial.print(", \"humidity\" : ");
    Serial.print(humidity);
    Serial.println(" }");
}
