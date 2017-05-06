#include <Stepper.h>

const int stepsPerRevolution = 200;
const int openSpeed = 20;
const int closeSpeed = 60;

const int numSamples = 1000;
int samples[numSamples];
int normalizedSamples[numSamples];

long closedMillis = 0;
bool isOpen = false;
const long closedDelay = 5000;
const double threshold = 1.1;

Stepper myStepper(stepsPerRevolution, 2, 3, 4, 5);

void setup() {
  initializeBuffer();
  myStepper.setSpeed(60);
  Serial.begin(9600);
  pinMode(8, INPUT);
}

void loop() {
  sampleAudio();
  double amplitude = getAmplitude();

  long currentMillis = millis();
  if (amplitude > threshold) {
    close();
    closedMillis = currentMillis;
  }

  if (!isOpen && currentMillis - closedMillis > closedDelay) {
    open();
  }

  delayMicroseconds(10);
}

void initializeBuffer() {
  for (int i=0; i<numSamples; i++) {
    samples[i] = 0;
  }
}

double rmsValue(int samples[], int numSamples) {
  long double totalValue = 0;
  for (int i=0; i<numSamples; i++) {
    totalValue += (double)sq(samples[i]);
  }

  return sqrt(totalValue / (double)numSamples);
}

void normalizeSamples(int samples[], int numSamples) {
  long totalValue = 0;
  for (int i=0; i<numSamples; i++) {
    totalValue += samples[i];
  }

  int avgValue = totalValue / numSamples;

  for (int i=0; i<numSamples; i++) {
    normalizedSamples[i] = samples[i] - avgValue;
  }
}

void sampleAudio() {
  for (int i=1; i<numSamples; i++) {
    samples[i] = samples[i-1];
  }

  samples[0] = analogRead(8);
}

double getAmplitude() {
  normalizeSamples(samples, numSamples);

  return rmsValue(normalizedSamples, 1000);
}

void open() {
  digitalWrite(LED_BUILTIN, HIGH);
  myStepper.setSpeed(openSpeed);
  myStepper.step(stepsPerRevolution);
  isOpen = true;
}

void close() {
  digitalWrite(LED_BUILTIN, LOW);
  myStepper.setSpeed(closeSpeed);
  myStepper.step(-stepsPerRevolution);
  isOpen = false;
}

