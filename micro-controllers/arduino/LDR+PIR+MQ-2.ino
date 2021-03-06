// Relay pin is controlled with D8. We'll be using LED to represent it.
int relay = 8;
volatile byte relayState = LOW;

// Connect PIR sensor to D2.
int PIRInterrupt = 2;

// Connect LDR to A0.
int LDRPin = A0;
// Stores LDR Value 
int LDRReading;
// LDR Threshold value
int LDRThreshold = 1000;

// Timer Variables
long lastDebounceTime = 0;  
long debounceDelay = 10000;


int redLed = 12;
int greenLed = 11;
int buzzer = 10;
int smokeA0 = A5;
// MQ-2 Sensor Threshold Value.
int sensorThres = 400;

void setup() {
  // Pin for relay module set as output
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  // PIR motion sensor set as input
  pinMode(PIRInterrupt, INPUT);
  // If the conditions are met, Triggers detectMotion function on rising mode to turn the relay on.
  attachInterrupt(digitalPinToInterrupt(PIRInterrupt), detectMotion, RISING);
  // Serial communication for debugging purposes


  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // After 10 seconds, Turn relay OFF.
  if((millis() - lastDebounceTime) > debounceDelay && relayState == HIGH){
    digitalWrite(relay, HIGH);
    relayState = LOW;
    Serial.println("OFF");
  }
  delay(50);

  {
  int analogSensor = analogRead(smokeA0);

  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  // Check if Reading reached Threshold Value.
  if (analogSensor > sensorThres)
  {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    tone(buzzer, 1000, 200);
  }
  else
  {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    noTone(buzzer);
  }
  delay(100);
}
}

void detectMotion() {
  Serial.println("Motion");
  LDRReading = analogRead(LDRPin);
  Serial.println(LDRReading);
  // Print LDRReading value. (Callibration of LDR Threshold Value.
  // Only turns the Relay on if the LDR reading is higher than the LDRThreshold
  if(LDRReading > LDRThreshold){
    if(relayState == LOW){
      digitalWrite(relay, LOW);
    }
    relayState = HIGH;  
    Serial.println("ON");
    lastDebounceTime = millis();
  }
}
