#include <Time.h>
#include <TimeLib.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include "DHT.h"

#define DHTPIN 4                  // DHT sensor pin
#define DHTTYPE DHT11             // DHT11 sensor type
#define DIRECTIONPIN 6            // Direction toggle button pin
#define LED1PIN 3                 // LED1 pin
#define LED2PIN 5

//Steps per revolution and rpm of the Stepper motor. ~34816 steps per min is the maximum, stay at around 30000 for best performance
//This choice of steps per revolution and rpm works best for the toggle button sensitivity, so we don't need to use a complicated button debouncer
const int stepsPerRevolution = 45;// Change this to fit the number of steps per revolution
const int rolePerMinute = 688;    // Adjustable range of 28BYJ-48 stepper is 0~17 rpm at 2048 steps (~34816 steps per min)

int buttonStateDir = 0;           // Variable for reading the pushbutton status (direction button)
int buttonStateStart = 0;         // Variable for reading the pushbutton status (start/stop button)
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
  pinMode(LED1PIN, OUTPUT);        // Initialize the LED1 pin as an output
  pinMode(LED2PIN, OUTPUT);        // Initialize the LED2 pin as an output
  pinMode(DIRECTIONPIN, INPUT);    // Initialize the pushbutton pin as an input
  lcd.begin(16, 2);                // Set up the LCD's number of columns and rows:
  dht.begin();                     // Set up the DHT11 sensor
}

void loop() {
  //  currHour = hour();
  //  currMin = minute();
  //  currSec = second();
  //  currDay = day();
  //  currMonth = month();
  buttonStateDir = digitalRead(DIRECTIONPIN);
  if (buttonStateDir == HIGH)
  {
    if (directionState == 0){directionState = 1;}
    else if (directionState == 1){directionState = 2;}
    else if (directionState == 2){directionState = 0;}
  }

  if (directionState == 0)
  {
    // Turn LED1 on, stepper motor goes counterclockwise:
    digitalWrite(LED1PIN, HIGH);
    digitalWrite(LED2PIN, LOW);
    myStepper.setSpeed(rolePerMinute);
    myStepper.step(stepsPerRevolution);
  }
  else if (directionState == 1)
  {
    // Turn LED2 on, stepper motor goes clockwise:
    digitalWrite(LED2PIN, HIGH);
    digitalWrite(LED1PIN, LOW);
    myStepper.setSpeed(rolePerMinute);
    myStepper.step(-stepsPerRevolution);
  }
  else if (directionState == 2)
  {
    // Turn off LEDs and stop the motor.
    digitalWrite(LED1PIN, LOW);
    digitalWrite(LED2PIN, LOW);
    myStepper.setSpeed(0);
    delay(100);
  }

  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) // If data cannot be read from DHT sensor
  {
    lcd.setCursor(0, 0);
    lcd.print("Cannot read from");
    lcd.setCursor(0, 1);
    lcd.print("DHT sensor!");
  }
  else
  {
    //Displaying the humidity and temperature on the LCD
    lcd.setCursor(15,0);
    lcd.print(" ");
    lcd.setCursor(0, 0);
    lcd.print("Humidity: ");
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.setCursor(10, 0);
    lcd.print(h);
    lcd.setCursor(14, 0);
    lcd.print("%");
    lcd.setCursor(6, 1);
    lcd.print(t);
    lcd.print((char)223);
    lcd.print("C");

    /*  Displaying the time/date on the LCD, not applicable right now
    lcd.setCursor(0,0);
    lcd.print(currHour);
    lcd.print(":");
    lcd.print(currMin);
    lcd.print(":");
    lcd.print(currSec);
    sprintf(currTime, "%02i:%02i:%02i", currHour, currMin, currSec);
    lcd.print(currTime);
    lcd.setCursor(11,0);
    sprintf(currDate, "%02i-%02i", currDay, currMonth);
    lcd.print(currDate);
    */
  }
}
