#include <Stepper.h>

const int stepsPerRevolution = 200;
const int openSpeed = 20;
const int closeSpeed = 60;

const int numSamples = 100;
long samplesWritten = 0;
int samples[numSamples];
int normalizedSamples[numSamples];

long closedMillis = 0;
bool isOpen = false;
const long closedDelay = 5000;
const double threshold = 1.4;

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
  Serial.print(normalizedSamples[0]);
  Serial.print(" ");
  Serial.println(amplitude);

  long currentMillis = millis();
  if (amplitude > threshold) {
    close();
    closedMillis = currentMillis;
  }

  if (currentMillis - closedMillis > closedDelay) {
    open();
  }

  delayMicroseconds(100);
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
  int i=numSamples;
  while (i-- > 0) {
    samples[i] = samples[i-1];
  }

  samples[0] = analogRead(8);
  samplesWritten = min(samplesWritten+1, numSamples);
}

double getAmplitude() {
  normalizeSamples(samples, samplesWritten);

  return rmsValue(normalizedSamples, 10);
}

void open() {
  if (isOpen) {
    return;
  }
  isOpen = true;
  digitalWrite(LED_BUILTIN, HIGH);
  myStepper.setSpeed(openSpeed);
  myStepper.step(stepsPerRevolution);
}

void close() {
  if (!isOpen) {
    return;
  }
  isOpen = false;
  digitalWrite(LED_BUILTIN, LOW);
  myStepper.setSpeed(closeSpeed);
  myStepper.step(-stepsPerRevolution);
}

