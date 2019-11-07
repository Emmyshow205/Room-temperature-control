#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_SI2C.h>

#include <SimpleDHT.h>
int pinDHT11 = 2;
int PWM = 3;
int sensorVal;
int PWMVal;

SimpleDHT11 dht11;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  pinMode(PWM, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  lcd.begin(16, 2);
  lcd.clear();
  
  // read without samples.
  byte temperature;
  byte humidity;
  dht11.read(pinDHT11, &temperature, &humidity, NULL);
  lcd.setCursor(0, 0);
  lcd.print("Temperature = "); lcd.print((int)temperature);
  lcd.setCursor(0, 1);
  lcd.print("Humidity = "); lcd.print((int)humidity);
  Serial.print("Temperature = "); Serial.print((int)temperature); Serial.println(" *C, "); 
  Serial.print("Humidity = "); Serial.print((int)humidity); Serial.println(" %");
  // DHT11 sampling rate is 1HZ.
  delay(1000);

  sensorVal = (int)temperature;
  if(sensorVal > 45){
    sensorVal = 45;
  }
  //map and assign pwm values to the fan output 0 to 255 corresponds to 0 to 100%
  PWMVal = map(sensorVal, 35, 45, 120, 255);
  
  //set 450 as out cutout or cut in limit where the fan switches from off to the lower PWM limit
  if(sensorVal <25){
    PWMVal = 0;
  }

  //write the PWM value to the pwm output pin
  analogWrite(PWM, PWMVal);
}
