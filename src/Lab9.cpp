/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/evanh/OneDrive/Desktop/CTD/InternetOfThings/Lab9/src/Lab9.ino"
/*
 * Project Lab9
 * Description:
 * Author:
 * Date:
 */
void setup();
void loop();
void light(int blue, int green, int red);
void compareLight(unsigned int oneL, unsigned int twoL);
void tempDisplay();
void enableButtonsF();
void normalizeLights();
#line 7 "c:/Users/evanh/OneDrive/Desktop/CTD/InternetOfThings/Lab9/src/Lab9.ino"
SYSTEM_THREAD(ENABLED);

#include <Wire.h>
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include <blynk.h>

int firstBound = -1;
int secondBound = -1;

unsigned int lightLevelOne;
unsigned int lightLevelTwo;
VCNL4040 proximitySensor;

BlynkTimer timer;

boolean enableButtons = true;
boolean normalLights = true;

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.

  Blynk.begin("gs0c3Zq-ovbkpwP1G7paOHK_PHfRjNUs", IPAddress(167, 172, 234, 162), 9090);

  pinMode(A1, INPUT); //button
  pinMode(A5, INPUT); //potentiometer
  pinMode(A4, INPUT); //TMP

  pinMode(D3, OUTPUT); //blue
  pinMode(D5, OUTPUT); //green
  pinMode(D7, OUTPUT); //red

  Serial.begin(9600);
  proximitySensor.begin();
  proximitySensor.powerOnAmbient();

  timer.setInterval(1000, tempDisplay);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  Blynk.run();
  timer.run();

  // The core of your code will likely live here.
  unsigned int lightVal = proximitySensor.getAmbient();

  if (digitalRead(A1) == HIGH && enableButtons){
    if (firstBound == -1){
      firstBound = analogRead(A5);
      Serial.print("First point set to: ");
      Serial.println(firstBound);
      enableButtons = false;
      timer.setTimeout(500, enableButtonsF);
    }
    else if (secondBound == -1){
      secondBound = analogRead(A5);
      Serial.print("Second point set to: ");
      Serial.println(secondBound);
    }
  }
  if (normalLights){
    if (lightVal < firstBound && normalLights){
      light(HIGH, LOW, LOW);
    }
    else if(lightVal < secondBound && normalLights){
      light(LOW, HIGH, LOW);
    }
    else{
      light(LOW, LOW, HIGH );
    }
  }

  if (lightVal == firstBound || lightVal == secondBound){
    Blynk.notify("The light level has passed the set points");
  }
  lightLevelTwo = lightLevelOne;
  lightLevelOne = lightVal;
  compareLight(lightLevelOne, lightLevelTwo); 
}

//streamlining turning on the LEDSs
void light(int blue, int green, int red){
  digitalWrite(D3, blue);
  digitalWrite(D5, green);
  digitalWrite(D7, red);
}

void compareLight(unsigned int oneL, unsigned int twoL){
  if (oneL <= firstBound && twoL > firstBound){
    Blynk.notify("The light level has passed the set points");
  }
  else if(oneL <= secondBound && twoL > secondBound){
    Blynk.notify("The light level has passed the set points");
  }
  else if(oneL >= firstBound && twoL < firstBound){
    Blynk.notify("The light level has passed the set points");
  }
  else if (oneL >= secondBound && twoL < secondBound){
    Blynk.notify("The light level has passed the set points");
  }
}
void tempDisplay(){
  uint64_t reading = analogRead(A4);

  double voltage = (reading * 3.3) / 4095.0;

  double temperature = (voltage - 0.5) * 100;

  Serial.print(temperature);
  Serial.print(" C // ");
  Serial.print((temperature * 9/5) + 32);
  Serial.println(" F ");

  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, (temperature * 9/5) + 32);
  Blynk.syncVirtual(V1, V2);

}

void enableButtonsF(){
  enableButtons = true;
}

void normalizeLights(){
  normalLights = true;
}
BLYNK_WRITE(V3){
  light(HIGH, HIGH, HIGH);
  normalLights = false;
  timer.setTimeout(1000, normalizeLights);
}