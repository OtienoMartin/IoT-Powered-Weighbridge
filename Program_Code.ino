#include <SoftwareSerial.h>
#include <Arduino.h>
#include "HX711.h"
#include <stdlib.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN = 13;

float weight; 
float calibration_factor = 229.2    ; // enter the calibration factor obtained earlier

//Pin wiring
int green=D0, red=D1, orange=D2;
int blue=D3, purple=D4, yellow=D5;
//blue=green, purple=red, yellow=orange.

HX711 scale;

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 Demo");
  Serial.println("Initializing the scale");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read()); // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20)); // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)
           
  scale.set_scale(229.2);
  //scale.set_scale(894.3571429);  // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale
            
  long zero_factor = scale.read_average(); //Get a baseline reading

  Serial.println("weight:");
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
}

void loop() {
  Serial.print("one reading:\t"); 
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  float weight=(scale.get_units(10), 5);     //reading of average used in led
  Serial.println(weight);
  weight = scale.get_units(5); //5

   if (weight<=75 && weight>20){   
    digitalWrite(D0, HIGH);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    Serial.println("OPTIMUM LOAD");
   }
   
   if (weight>75 && weight<=999){  
    digitalWrite(D0, LOW);
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, HIGH);
    digitalWrite(D5, LOW);
    Serial.println("OVERLOAD"); 
   }

   if (weight<=0 && weight<=20){    
    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    delay(500);
    digitalWrite(D2, LOW);
    delay(500);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    digitalWrite(D5, HIGH);
    delay(500);
    digitalWrite(D5, LOW);
    delay(500);
    Serial.println("NO LOAD"); 
   }

   
        
  scale.power_down();             // put the ADC in sleep mode
  delay(150);
  scale.power_up();
}
