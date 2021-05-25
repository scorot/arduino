#include <Wire.h>
#include <SPI.h>

#include <LiquidCrystal.h>

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


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup() {
    Serial.begin(9600);
	
    bool rslt;
    rslt = bme.begin();  
    if (!rslt) {
        Serial.println("Init Fail,Please Check your address or the wire you connected!!!");
        while (1);
    }
	
    Serial.println("Init Success");
    Serial.println("Temperature           Pressure             Humidity           Atlitude");

    lcd.begin(16, 2);

    delayTime = 1000;
}

void loop() { 
    printSerialValues();
    printLcdValues();
    delay(delayTime);
}

void printSerialValues() {
    Serial.print("temperature:");
    Serial.print(bme.readTemperature());
    Serial.print("*C   ");

    Serial.print("pressure:");
    Serial.print(bme.readPressure()/100.0F);
    Serial.print("hPa   ");

    Serial.print("humidity:");
    Serial.print(bme.readHumidity());
    Serial.print("%   ");
	
    Serial.print("altitude:");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println("m");
}


void printLcdValues() {
    temp = bme.readTemperature();
    pressure = bme.readPressure() / 100.0F;
    humidity = bme.readHumidity();
    alt = bme.readAltitude(SEALEVELPRESSURE_HPA);

    lcd.setCursor(0, 0);
//    lcd.print("T ");
    lcd.print(temp, 1);
    lcd.print("*C ");
    lcd.print("Hum:");
    lcd.print(humidity, 1);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print(pressure,1);
    lcd.print("hPa ");
    lcd.print(alt, 1);
    lcd.print("m");
  
}
