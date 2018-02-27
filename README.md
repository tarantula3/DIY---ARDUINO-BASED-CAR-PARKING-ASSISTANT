# DIY - ARDUINO BASED CAR PARKING ASSISTANT

Introduction
----------------

Hi Friends,

I am back again with another Arduino based home automation project. This time I am trying to make my partner's life easy by installing a collision avoidance system in the garage to help her park the car safely without hitting the garage wall.

So, in this video, I am going to use an ultrasonic sensor to calculate the car's distance from the garage wall and display it using green, yellow and red LEDs. The color of LEDs indicates whether to keep moving, slow down, stop or go back.

The total cost of the project is around $20 - $25.

[![DIY - ARDUINO BASED CAR PARKING ASSISTANT](https://github.com/tarantula3/DIY---ARDUINO-BASED-CAR-PARKING-ASSISTANT/blob/master/qwe.JPG)](https://youtu.be/LQGhprwuHe0 "DIY - ARDUINO BASED CAR PARKING ASSISTANT")


Step 1: Logic
-----
![Logic](https://cdn.instructables.com/FIT/3SNU/JE4KJCZS/FIT3SNUJE4KJCZS.MEDIUM.jpg)

The project has 3 phases

Phase 1: Waiting for the car In this phase the device keeps looking for a moving object within the sensors proximity. If an object enters the proximity then one of the three LEDs turns on based on how far the moving object is. If the object is way too close, then a noise is made to make the moving object aware of the distance.

Phase 2: No car in the garage If there is no object in the proximity then turn off all the LEDs.

Phase 3: The car has stopped moving (Parked in the right spot) If the object has stopped moving and is still in the proximity wait for 20 CPU cycles and then turn off the LEDs.


Step 2: Hardware Requirement
----
![Hardware Requirement](https://cdn.instructables.com/F7F/JIZJ/JE4KJD1H/F7FJIZJJE4KJD1H.MEDIUM.jpg)

For this very simple project we need:
- A Perfboard
- An Arduino nano/uno (whatever is handy)
- A Red, Green and a Yellow LED (Light Emitting Diode)
- 3 x 220ohm resistor for the LEDs
- One HC-SRO4 Ultrasonic Sensor
- A Buzzer shield or A buzzer and a 100 ohm resistor
- A 220v AC to 5v DC Buck step-down module
- One Female Pin Header Strip
- An Ethernet cable
- Some connecting cables
- A USB cable to upload the code to the Arduino
- and general soldering equipments


Step 3: Assembly
------
![Assembly](https://cdn.instructables.com/F8A/8MKK/JE4KJD34/F8A8MKKJE4KJD34.MEDIUM.jpg)

Let start by connecting the LEDs to the board.

Connect the Red LED to pin D2, Yellow LED to D3 and the Green LED to D4 of the Arduino by putting in a 220ohm resistor between the Arduino board and the LEDs. Now lets connect the Buzzer to analogue pin A0. Next, connect the Trig pin of the Ultrasonic Sensor to D5 and the Echo pin to D6 of the Arduino. Once all the modules are connected to the Arduino board, its time for us to connect all the positive and negative pins together. Connect all the positive pins of the modules to the +5v supplied by the Buck Step-Down Module and the negative pins to the -ve terminal of the Module. That's it, we can now upload our sketch to the board.

In this assembly I am using 3 LEDs to display the distance, however you can replace the 3 LEDs with a RGB LED, or you can also use an array of LEDs like an audio level indicator to display the movement of the car.


Step 4: My Setup
-----
![Assembly](https://cdn.instructables.com/F1M/ZIA6/JE4KJD57/F1MZIA6JE4KJD57.MEDIUM.jpg)

OK now lets see what I have made.

I have installed the Arduino, buzzer, the ultrasonic sensor and the three 220 ohms resistors on one Perfboard. The 3 LEDs and the power module is installed on a second Perfboard. I will be covering the LEDs with a translucent cover to give it a nice look.

The 220v power supply will be connected to the screw terminal block. The base unit will then be connected to the LEDs and the power supply with an Ethernet cable.


Step 5: The Code
----
```
int trigPin = PD5; // Sensor Trip pin connected to Arduino pin D5
int echoPin = PD6; // Sensor Echo pin connected to Arduino pin D6
int redLED = PD2; // Red LED connected to pin D2
int yellowLED = PD3; // Yellow LED connected to pin D3
int greenLED = PD4; // Green LED connected to pin D4
int buzzer = A0; // Buzzer connected to Analogue pin A0
long TempDistance = 0; // A variable to store the temporary distance
int counter = 0; // Counter value to check if the object has stopped moving

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
  Distance = (duration/2) / 74; // Distance in Inches

  if(counter < 20){ // Do the rest if the car is still moving
    if (Distance > 200) { // Nothing in the garrage
      turnThemAllOff();
    }
    if ((Distance > 55) && (Distance <= 200)) { // Turn on Green LED
      digitalWrite(greenLED, HIGH);
      digitalWrite(yellowLED, LOW);
      digitalWrite(redLED, LOW);
      noTone(buzzer);
   }
   if ((Distance > 15) && (Distance <= 55)) { // Turn on Yellow LED
      digitalWrite(yellowLED, HIGH);
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED,LOW);
      noTone(buzzer);
   }
   if (Distance <= 15) { // Turn on Red LED
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED,LOW);
      digitalWrite(yellowLED, LOW);
      noTone(buzzer);
   }
   if (Distance < 8) { // Item is way to close - start the buzzer
      tone(buzzer, 500);
   }

  }

  if ((Distance == TempDistance) || ((Distance+1) == TempDistance) || ((Distance-1) == TempDistance)){
      if(counter >= 20){ // Turn off the lights if the object hasn't moved for 20 cycles (no change in distance)
       Serial.println("No movement detected, turning off the lights");
       turnThemAllOff();
      } else {
       counter++;
      }
  } else {
  counter = 0; // Reset counter if there is a movement
  }
  TempDistance = Distance;
  
  Serial.print(Distance);
  Serial.println(" inches");
  Serial.print("Counter : ");
  Serial.println(counter); delay(500); 
}

// Function to turn the LEDs off
void turnThemAllOff(){
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED,LOW);
  digitalWrite(yellowLED, LOW);
  noTone(buzzer);
}
```

Start the code by defining the constants and the global variables that will be used throughout the code.

Then define the pin modes in the setup section of the code.

Then create a function to turn off all the LEDs and the buzzer.

Now, calculate the "Distance" in inches by reading the values received from the Ultrasonic Sensor.

Then by checking the value of the "Distance" we will turn on and off the LEDs based on how far the object is. If the distance is greater than 200 then turn off all the LEDs and the buzzer as the object is out of range. Else if it is between 55 and 200 then turn on the green LED. If the object is between 15 and 55 then turn on the yellow LED, and if the object goes closer than 15 inches then turn on the red LED until it reaches 8 inches. When the distance becomes less than 8 start the buzzer along with the red LED.

Next bit of the code is to set the value of the counter based on the cars movement which then decides when to turn off the LEDs. It compares the value of "Distance" with the "TempDistance" and if the values are same (object hasn't moved) then increments the counter. If the object moves any-time during this process the counter is reset to 0. Finally the "TempDistance" is set to the value of "Distance".

Just before comparing the Distances we also need to check if the counter value has exceed 20. I am doing this to stop the below code from executing if the car is in a steady position.

Lastly we just need to add a small delay to our sketch to pause the code for a while.


Step 6: Quick Demo
-----
[![DIY - ARDUINO BASED CAR PARKING ASSISTANT](https://github.com/tarantula3/DIY---ARDUINO-BASED-CAR-PARKING-ASSISTANT/blob/master/qwe.JPG)](https://youtu.be/LQGhprwuHe0 "DIY - ARDUINO BASED CAR PARKING ASSISTANT")

So this is how I have installed the unit in my garage.

As I walk close to the sensor the light changes from green to yellow to red and ultimately the buzzer goes on when I am too close to the sensor. In my case I have installed the buzzer next to the Arduino however I will recommend you to install the buzzer along with the LEDs. If you want you can also flash the red LED when the buzzer goes on.

So now, my partner can park the car easily without making any assumptions. Doesn't matter how many times she fail her driving test she is not going to break my wall (even when she is drunk). Not that I am asking her to drive when she is drunk (just kidding).

Thanks again for watching this video! I hope it helps you. If you want to support me, you can subscribe to my channel and watch my other videos. Thanks, ca again in my next video.
