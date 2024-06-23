#include "arduinoFFT.h"
#include <SoftwareSerial.h>

SoftwareSerial BTserial(0,1);
 
#define SAMPLES 128               //SAMPLES-pt FFT. Must   be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 4096   //Ts = Based on Nyquist, must be 2 times the highest expected frequency.
 
arduinoFFT FFT = arduinoFFT();
 
unsigned int samplingPeriod;
unsigned long microSeconds;
int val;
int total = 0;
int i=0;
   
double vReal[SAMPLES]; //create vector of size SAMPLES to hold real values
double vImag[SAMPLES]; //create vector of size SAMPLES to hold imaginary values
 
void   setup() 
{
    Serial.begin(9600); //Baud rate for the Serial Monitor
     samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY)); //Period in microseconds   
     BTserial.begin(9600);
}
 
void loop() 
{  
    /*Sample SAMPLES times*/
    for(int   i=0; i<SAMPLES; i++)
    {
      microSeconds = micros();    //Returns the   number of microseconds since the Arduino board began running the current script.   
      vReal[i] = analogRead(0);   //Reads the value from analog pin 0 (A0), quantize it and save it as a real term.
      vImag[i] = 0; //Makes   imaginary term 0 always
      /*remaining wait time between samples if necessary*/
      while(micros() < (microSeconds + samplingPeriod))
        {
           //do nothing
        }
    }
 
    /*Perform FFT on samples*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal,   vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    /*Find peak frequency and print peak*/
    double peak = FFT.MajorPeak(vReal,   SAMPLES, SAMPLING_FREQUENCY);
    //Serial.println(peak);     //Print out the most   dominant frequency.
    char ch;
    int arr[8]={0};
    if(i==8)
    {
      ch=total;
      Serial.println(ch); 
      i=0;
      total=0;
    }
    
    if(peak < 1500)
    {
      val = 0;
      i++;
    }

    else
    {
      val = 1;
      arr[i]=1;
      i++;
    }

    if(arr[0]==1)total=total+2*2*2*2*2*2*2;
    if(arr[1]==1)total=total+2*2*2*2*2*2;
    if(arr[2]==1)total=total+2*2*2*2*2;
    if(arr[3]==1)total=total+2*2*2*2;
    if(arr[4]==1)total=total+2*2*2;
    if(arr[5]==1)total=total+2*2;
    if(arr[6]==1)total=total+2;
    if(arr[7]==1)total=total+1;
    
    Serial.print(val);
    delay(1000);
    BTserial.print(val);
    /*Script stops here. Hardware reset required.*/
}
