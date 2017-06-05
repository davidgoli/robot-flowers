#include <Stepper.h>

const int stepsPerRevolution = 200;
const int openSteps = 100;
const int openSpeed = 20;
const int closeSpeed = 60;

const int stepperPins[] = { 8, 9, 10, 11 };
const int micPin = 8;
const int potPin = 10;
const int ledPin = 7;

const int numSamples = 100;
long samplesWritten = 0;
int samples[numSamples];
int normalizedSamples[numSamples];

long closedMillis = 0;
bool isOpen = false;
const long closedDelay = 5000;
double threshold = 1.4;

Stepper myStepper(stepsPerRevolution, stepperPins[0], stepperPins[1], stepperPins[2], stepperPins[3]);

void setup() {
  initializeBuffer();
  myStepper.setSpeed(60);
  Serial.begin(9600);
  pinMode(micPin, INPUT);
  pinMode(potPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  sampleAudio();
  double amplitude = getAmplitude();

  analogWrite(ledPin, 255);
  long currentMillis = millis();
  if (amplitude > threshold) {
    close();
    closedMillis = currentMillis;
  }

  if (currentMillis - closedMillis > closedDelay) {
    open();
  }

  if (currentMillis % 10 == 0) {
    const int potValue = analogRead(potPin);
    threshold = (double)potValue * 0.005;
    Serial.print(amplitude);
    Serial.print(" ");
    Serial.print(isOpen ? 5 : 0);
    Serial.print(" ");
    Serial.println(threshold);
  }
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
  while (--i > 0) {
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
  myStepper.step(-openSteps);
  analogWrite(ledPin, 255);
}

void close() {
  if (!isOpen) {
    return;
  }
  isOpen = false;
  digitalWrite(LED_BUILTIN, LOW);
  //analogWrite(ledPin, 0);
  myStepper.setSpeed(closeSpeed);
  myStepper.step(openSteps);
}

