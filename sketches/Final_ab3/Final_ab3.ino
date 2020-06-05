/*
 * Allen Breyer III - 654748297
 * Final Lab - Weather-Monitoring Alarm Clock
 */

/* READ THIS!!!
 *  For the program to compile properly in regards to the DHT11 sensor, you need the correct libraries installed!
 *  Install the following libraries from the "Manage Libraries" manager:
 *  - Adafruit AM2320 sensor library
 *  - Adafruit Circuit Playground
 *  - Adafruit Unified Sensor
 *  - DHT sensor library
 */

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#include "DHT.h"
#include <TimeLib.h>

// DHT sensor using analog pin A0
#define DHTPIN A0
#define DHTTYPE DHT11

// used for temperature
float tempF;
// used for humidity
float humidity;

// Buzzer to arduino pin 8
const int BUZZER = 8;
int counter = 0;
int currTime = 0;

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  // Set up the serial communication
  Serial.begin(9600);
  // Sert up the buzzer
  pinMode(BUZZER, OUTPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  dht.begin();
}

void loop()
{
  // If it's not the current time
  if (currTime != now())
  {
    // Read temperature as Fahrenheit (isFahrenheit = true)
    tempF = dht.readTemperature(true);
    humidity = dht.readHumidity();
    counter = 0;
  
    // Error checking: Check if any reads failed and exit early (to try again).
    if (isnan(humidity))
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Failed to read");
      lcd.setCursor(0,1);
      lcd.print("from DHT sensor!");
      return;
    }

    //delay(100);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temp: ");
    lcd.print((int) tempF);
    lcd.setCursor(8,0);
    // degrees symbol
    lcd.print((char)223);
    lcd.setCursor(9,0);
    lcd.print("F");
    lcd.setCursor(0,1);
    lcd.print("Humidity: ");
    lcd.print((int) humidity);
    lcd.setCursor(12,1);
    lcd.print("%");
    // update the current time
    currTime = now();
  }

  // If a signal is being sent through serial communication
  if(Serial.available() > 0)
  {
    char input = Serial.read();
    Serial.println(input);
    // If the correct signal is read, sound the buzzer alarm
    if (input == 'y')
    {
      tone(BUZZER, 1000);
      delay(1000);
      noTone(BUZZER);
    }
  }
}
