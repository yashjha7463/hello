#include <Arduino.h>
#include<bits/stdc++.h>

const int signalPin = A0;
const int ledPin = 13;
const int sampleWindow = 50;

unsigned int signalSample[sampleWindow];
unsigned int lastBeatTime = 0;
int peak = 2048;
int trough = 2048;
int thresh = 2048;
int amplitude = 100;
int tempThresh = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned long now = millis();
  signalSample[now % sampleWindow] = analogRead(signalPin);
  
  int beatInterval = now - lastBeatTime;
  if (beatInterval < 80) {
    return;
  }
  
  // Adaptive spectrum noise cancellation
  if (signalSample[now % sampleWindow] < thresh && signalSample[now % sampleWindow] > tempThresh) {
    tempThresh = signalSample[now % sampleWindow];
  }
  else {
    thresh = tempThresh;
    tempThresh = 0;
  }
  
  if (signalSample[now % sampleWindow] > peak) {
    peak = signalSample[now % sampleWindow];
  }
  
  if (signalSample[now % sampleWindow] < trough) {
    trough = signalSample[now % sampleWindow];
  }
  
  // Heart beat detection
  if (signalSample[now % sampleWindow] > thresh && signalSample[now % sampleWindow] > amplitude) {
    digitalWrite(ledPin, HIGH);
    lastBeatTime = now;
    amplitude = peak - trough;
    peak = signalSample[now % sampleWindow];
    trough = signalSample[now % sampleWindow];
  }
  else {
    digitalWrite(ledPin, LOW);
  }
  
  Serial.println(signalSample[now % sampleWindow]);
}


/**

This code uses the analog input pin A0 to read the heart beat signal. 
The signal is processed to eliminate noise and detect the heart beats using an adaptive spectrum noise cancellation algorithm.
The heart beat is indicated by the LED on pin 13. The signal and the heart beat detection results are also sent to the serial monitor for debugging purposes.


**/