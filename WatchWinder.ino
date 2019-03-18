#include <Time.h>
#include <TimeLib.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include "DHT.h"

#define DHTPIN 4                  //DHT sensor pin
#define DHTTYPE DHT11             //DHT11 sensor type
#define buttonPin 2               // Pushbutton pin
#define ledPin 3                  // LED pin
#define stepsPerRevolution 50     // Change this to fit the number of steps per revolution
#define rolePerMinute 570         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm

int buttonState = 0;              // Variable for reading the pushbutton status
int directionState = 0;           // Direction of the stepper motor

//Variables for displaying time/date, not applicable right now
//int currHour = hour();
//int currMin = minute();
//int currSec = second();
//int currDay = day();
//int currMonth = month();
//char currTime[] = "00:00:00";
//char currDate[] = "00-00";

//Initialize LCD, stepper motor, DHT sensor 
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  myStepper.setSpeed(rolePerMinute); 
  pinMode(ledPin, OUTPUT);         // Initialize the LED pin as an output
  pinMode(buttonPin, INPUT);       // Initialize the pushbutton pin as an input
  lcd.begin(16, 2);                // Set up the LCD's number of columns and rows:
  dht.begin();
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
}

void loop() {
  buttonState = digitalRead(buttonPin);
//  currHour = hour();
//  currMin = minute();
//  currSec = second();
//  currDay = day();
//  currMonth = month();
  if (buttonState == HIGH)
  {
    if (directionState == LOW)
    {
      directionState = HIGH;
    }
    else
    {
      directionState = LOW;
    }
  }
  if (directionState == HIGH)
  {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    Serial.println("clockwise");
    myStepper.step(stepsPerRevolution);
  }
  else if (directionState == LOW)
  {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    Serial.println("counterclockwise");
    myStepper.step(-stepsPerRevolution);
  }
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) 
  {
    lcd.setCursor(0,0);
    lcd.print("Cannot read from");
    lcd.setCursor(0,1); 
    lcd.print("DHT sensor!");
  }
  else
  {
  //Displaying the humidity and temperature on the LCD  
  lcd.setCursor(10, 0);
  lcd.print(h);
  lcd.setCursor(14, 0);
  lcd.print("%");
  lcd.setCursor(6, 1);
  lcd.print(t);
  lcd.print((char)223);
  lcd.print("C");
//  Displaying the time/date on the LCD, not applicable right now
//  lcd.setCursor(0,0);
//  lcd.print(currHour);
//  lcd.print(":");
//  lcd.print(currMin);
//  lcd.print(":");
//  lcd.print(currSec);
//  sprintf(currTime, "%02i:%02i:%02i", currHour, currMin, currSec);
//  lcd.print(currTime);
//  lcd.setCursor(11,0);
//  sprintf(currDate, "%02i-%02i", currDay, currMonth);
//  lcd.print(currDate);
  }
}
