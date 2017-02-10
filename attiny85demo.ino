/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// Pin 11 has the LED on Teensy 2.0
// Pin 6  has the LED on Teensy++ 2.0
// Pin 13 has the LED on Teensy 3.0
// give it a name:
const int led = 1;
const int buzzer = 4;
const int cds = 3;
const int SENSOR_SAMPLES = 5;
const int LIGHT_DELTA_THRESHOLD = 200;
const int SENSOR_SAMPLE_DELAY_MS = 5;

int light, prev_light;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  pinMode(buzzer, OUTPUT);   
  pinMode(cds, INPUT);
  light = prev_light = 0;  
}

// The sound producing function for chips without tone() support
void beep (unsigned char pin, int frequencyInHertz, long timeInMilliseconds) {
  // from http://web.media.mit.edu/~leah/LilyPad/07_sound_code.html
  int x;   
  long delayAmount = (long)(1000000/frequencyInHertz);
  long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2));
  for (x=0;x<loopTime;x++) {   
    digitalWrite(pin,HIGH);
    delayMicroseconds(delayAmount);
    digitalWrite(pin,LOW);
    delayMicroseconds(delayAmount);
  }  
}

int smooth(int array[], int len) {
  /**
  Return the average of the array without the highest and lowest values.
  **/
  int low = 9999;
  int high = -1;
  int total = 0;
  for (int s=0; s< len; s++) {
    total += array[s];
    low = min(array[s], low);
    high = max(array[s], high);
  }
  total -= low;
  total -= high;
  return total / (len -2);
}

int readSensor() {
  /***
  The multiple reads and delay are recommended to allow the shared ADC to properly
  read multiple pins in succession.
  ***/
  int samples[SENSOR_SAMPLES];
  
  for (int s=0; s<SENSOR_SAMPLES; s++) {
    samples[s] = analogRead(cds); delay(SENSOR_SAMPLE_DELAY_MS);
  }
  return smooth(samples, SENSOR_SAMPLES);
}

// the loop routine runs over and over again forever:
void loop() {
  prev_light = light;
  light = readSensor();
  analogWrite(led, 150);
  if ((light - prev_light) > LIGHT_DELTA_THRESHOLD) {
    beep(buzzer, 125, 50);
    analogWrite(led, 250);
  }
  if ((prev_light - light) > LIGHT_DELTA_THRESHOLD) {
    beep(buzzer, 125, 50);
    analogWrite(led, 50);
  }
  delay(1000);
}
