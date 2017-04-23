/*
  Analog input, 

 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.

 The circuit:
 * two cny70 next to each other taped to a translucent tube between the ant nest and the feeding area.
 created 
 modified 
 by N.Mohr

 This example code is in the public domain.

 */

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPinA = A1;  // Analog input pin that the Sensor is attached to
const int analogInPinB = A2; 
const int LedPinA = 2;
const int LedPinB = 3; 
const int smoothReadings = 30;


long A[smoothReadings],B[smoothReadings];
int readIndexsmooth = 0;              // the index of the current reading
int Atotal,Btotal = 0;                  // the running total
int Aaverage, Baverage, counts, antsout, antsin, antsoutside, maxantsoutside= 0;                // the average
int thresh = 8;  // set this one to detect an ant!
int zeitA = 0;
int zeitB = 0;
boolean Adetected = false; // was there a detection on sensor A
boolean Bdetected = false;
boolean Acooldown = true; //wait for normal
boolean Bcooldown = true; //wait for normal

void ausgabe()
{

 if (antsoutside>=maxantsoutside) 
       {
        maxantsoutside=antsoutside;
        }
    if (antsoutside<0) 
       {
        maxantsoutside++;
        antsoutside=0;       
       }
  /*
   debugging information:
  Serial.print("smoothedA = ");
  Serial.print(sensorValueA/10);
  Serial.print(" smoothedB = ");
  Serial.print (sensorValueB/10);
  Serial.print(" sensorA = ");
  Serial.print (A[readIndexsmooth]/10);
  Serial.print(" sensorB = ");
  Serial.print (B[readIndexsmooth]/10);
  */
  Serial.print("Time: ");
  Serial.print(millis());
  Serial.print(" Ant walked out = ");
  Serial.print(antsout);
  Serial.print(" Ants walked in = ");
  Serial.print(antsin);
  Serial.print(" Ants outside = ");
  Serial.print(antsoutside);
  Serial.print(" max Ants outside = ");
  Serial.print(maxantsoutside);
  // more debug
  Serial.print(" Adet = ");
  Serial.print(Adetected); 
  Serial.print(" Bdet = ");
  Serial.print(Bdetected); 
  Serial.print(" Acooldown = ");
  Serial.print(Acooldown); 
  Serial.print(" Bcooldown = ");
  Serial.println(Bcooldown); 
}
  
void einlesen()
  {
   // Smoothing of average signal (calibration)
   Atotal = Atotal - A[readIndexsmooth];
   Btotal = Btotal - B[readIndexsmooth];
   A[readIndexsmooth] = analogRead(analogInPinA);
   B[readIndexsmooth] = analogRead(analogInPinB);
   Atotal = Atotal + A[readIndexsmooth];
   Btotal = Btotal + B[readIndexsmooth];

   // Ant detection
   if ((abs(A[readIndexsmooth]-Aaverage)>=thresh)&&(!Acooldown)) //sensor A ist größer als thresh und nicht cooldown
    {
     Adetected = true;
     Acooldown=true;
     ausgabe(); // just for debugging
     zeitA=millis();
     if (Bdetected) //is this the second sensor?
      {
       antsin++;
       antsoutside--;
       Adetected=false;
       Bdetected=false; 
       Bcooldown=true;
       zeitA=0;
       ausgabe();
      }
    }

    if ((abs(B[readIndexsmooth]-Baverage)>=thresh)&&(!Bcooldown)) //sensor B ist größer als thresh und nicht cooldown
    {
     Bdetected = true;
     Bcooldown=true;
     ausgabe(); // just for debugging
     zeitB=millis();
     if (Adetected) //is this the second sensor?r
      {
       antsout++;
       antsoutside++;
       Adetected=false;
       Bdetected=false; 
       Acooldown=true;
       Bcooldown=true;
       zeitB=0;
       ausgabe();
      }
    }
    // set sensors active after "cooldown"
    if ((abs(A[readIndexsmooth]-Aaverage)<=thresh/2)&&(Acooldown)) //wait until sensor below 1/2 tresh 
    {
      Acooldown=false;
      ausgabe(); // just for debugging

    }
    if ((abs(B[readIndexsmooth]-Baverage)<=thresh/2)&&(Bcooldown)) //wait until sensor below 1/2 tresh 
    {
      Bcooldown=false;
      ausgabe(); // just for debugging
    }

    if (zeitA>2000) // reset after wrong measure or returning ant between the sensors
     {
      zeitA=0;
      Adetected=false;
      Acooldown=false;
      ausgabe(); // just for debugging
     }
     if (zeitB>2000) // reset after wrong measure or returning ant between the sensors
     {
      zeitB=0;
      Bdetected=false;
      Bcooldown=false;
      ausgabe(); // just for debugging
     }
  
   readIndexsmooth = readIndexsmooth + 1;
   if (readIndexsmooth >= smoothReadings) 
    {
    // ...wrap around to the beginning:
    readIndexsmooth = 0;
    }
    Aaverage = Atotal / smoothReadings; 
    Baverage = Btotal / smoothReadings;
     
   }
    
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(LedPinA, OUTPUT);
  pinMode(LedPinB, OUTPUT);
  digitalWrite(LedPinA, HIGH);
  digitalWrite(LedPinB, HIGH);
  delay (1000); //led richtig anlaufen lassen
  for (int thisReading = 0; thisReading < smoothReadings*2; thisReading++) 
    {einlesen();
  // alle Werte im Array zweimal einlesen;
    }
  ausgabe();
  }

void loop() {
  einlesen();
  delay(50);
}
