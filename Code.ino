int trigPin       = PD5;  // Sensor Trip pin connected to Arduino pin D5
int echoPin       = PD6;  // Sensor Echo pin connected to Arduino pin D6
int redLED        = PD2;  // Red LED connected to pin D2
int yellowLED     = PD3;  // Yellow LED connected to pin D3
int greenLED      = PD4;  // Green LED connected to pin D4
int buzzer        = A0;   // Buzzer connected to Analogue pin A0
long TempDistance = 0;    // A variable to store the temporary distance
int counter       = 0;    // Counter value to check if the object has stopped moving

void setup() {
  
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);

  pinMode(buzzer, OUTPUT);

}

void loop() {

  long duration, Distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  Distance = (duration/2) / 74;     // Distance in Inches

  if(counter < 20){                             // Do the rest if the car is still moving
    if (Distance > 200) {                       // Nothing in the garrage
        turnThemAllOff();
    }
    if ((Distance > 55) && (Distance <= 200)) { // Turn on Green LED
        digitalWrite(greenLED, HIGH);
        digitalWrite(yellowLED, LOW);
        digitalWrite(redLED, LOW);
        noTone(buzzer);
    } 
    if ((Distance > 15) && (Distance <= 55)) {  // Turn on Yellow LED
        digitalWrite(yellowLED, HIGH);
        digitalWrite(redLED, LOW);
        digitalWrite(greenLED,LOW);
        noTone(buzzer);
    } 
    if (Distance <= 15) {                       // Turn on Red LED
        digitalWrite(redLED, HIGH);
        digitalWrite(greenLED,LOW);
        digitalWrite(yellowLED, LOW);
        noTone(buzzer);
    }
    if (Distance < 8) {                         // Item is way to close - start the buzzer
        tone(buzzer, 500);
    }
  }
   
  if ((Distance == TempDistance) || ((Distance+1) == TempDistance) || ((Distance-1) == TempDistance)){ 
     if(counter >= 20){   // Turn off the lights if the object hasn't moved for 20 cycles (no change in distance)
        Serial.println("No movement detected, turning off the lights");
        turnThemAllOff();
     } else {
        counter++;
     }
  } else {
     counter = 0;         // Reset counter if there is a movement
  }
  TempDistance = Distance;
  
  Serial.print(Distance);
  Serial.println(" inches");

  Serial.print("Counter : ");
  Serial.println(counter);
  delay(500);
}

// Function to turn the LEDs off
void turnThemAllOff(){
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED,LOW);
  digitalWrite(yellowLED, LOW);
  noTone(buzzer);
}

