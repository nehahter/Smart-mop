#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

Servo myservo;

String command;             // String to store app command state.
int speedCar = 800;         // Speed range: 400 - 1023.
int speed_Coeff = 3;

const char* ssid = "mana car";
const char* password = "hehehuhu";
ESP8266WebServer server(80);

// Motor and servo pins
#define ENA  14
#define ENB  12
#define IN_1 15
#define IN_2 13
#define IN_3  2
#define IN_4  0
#define MOP_PIN 5  // Assuming pin D1 for mop control

// Ultrasonic Sensor Pins
#define TRIG_PIN 10  // GPIO10
#define ECHO_PIN 9   // GPIO9

long duration;
int distance;

void servoControl() {
  for (int i = 0; i <= 180; i += 18) {
    myservo.write(i);
    delay(0.1);
  }

  for (int j = 180; j >= 0; j -= 18) {
    myservo.write(j);
    delay(0.1);
  }
}
 
void motorControl(int enPin, int in1Pin, int in2Pin, int speed, bool direction) {
  analogWrite(enPin, speed);
  digitalWrite(in1Pin, direction ? HIGH : LOW);
  digitalWrite(in2Pin, direction ? LOW : HIGH);
  
}

void goAhead() {
  motorControl(ENA, IN_1, IN_2, speedCar, false);
  motorControl(ENB, IN_3, IN_4, speedCar, false);
}

void goBack() {
  motorControl(ENA, IN_1, IN_2, speedCar, true);
  motorControl(ENB, IN_3, IN_4, speedCar, true);
}

void goRight() {
  motorControl(ENA, IN_1, IN_2, speedCar, true);
  motorControl(ENB, IN_3, IN_4, speedCar+200, false);
}

void goLeft() {
  motorControl(ENA, IN_1, IN_2, speedCar+200, false);
  motorControl(ENB, IN_3, IN_4, 0, true);
}

void goAheadRight() {
  goRight();
  goAhead();
}

void goAheadLeft() {
  goLeft();
  goAhead();
}

void goBackRight() {
  goRight();
  goBack();
}

void goBackLeft() {
  goLeft();
  goBack();
}

void stopRobot() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// Function to control the mop. Argument is true to start and false to stop.
void controlMop(bool start) {
  digitalWrite(MOP_PIN, start ? HIGH : LOW);
}

// Ultrasonic sensor function to measure distance
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

// void HTTP_handleRoot(void) {
//   if (server.hasArg("State")) {
//     Serial.println(server.arg("State"));
//   }
//   server.send(200, "text/html", "");
//   delay(1);
// }

void setup() {
  Serial.begin(115200);
  myservo.attach(D1);
  delay(100);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(MOP_PIN, OUTPUT);  // Mop control setup

  // Ultrasonic sensor setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Connecting WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Starting WEB-server
  server.on("/", HTTP_handleRoot);
  server.onNotFound(HTTP_handleRoot);
  server.begin();
}


void loop() {
  server.handleClient();

  command = server.arg("State");
  if (command == "F") goAhead();
  else if (command == "B") goBack();
  else if (command == "L") goLeft();
  else if (command == "R") goRight();
  else if (command == "I") goAheadRight();
  else if (command == "G") goAheadLeft();
  else if (command == "J") goBackRight();
  else if (command == "H") goBackLeft();
  else if (command == "0") speedCar = 400;
  else if (command == "1") speedCar = 470;
  else if (command == "2") speedCar = 540;
  else if (command == "3") speedCar = 610;
  else if (command == "4") speedCar = 680;
  else if (command == "5") speedCar = 750;
  else if (command == "6") speedCar = 820;
  else if (command == "7") speedCar = 890;
  else if (command == "8") speedCar = 960;
  else if (command == "9") speedCar = 1023;
  else if (command == "S") stopRobot();
  else stopRobot();
  servoControl();

  if (command == "M") { 
    controlMop(true);    // Start the mop
  } else if (command == "N") {  // Another command for stopping the mop
    controlMop(false);   // Stop the mop
  }

  // Example use of the ultrasonic sensor to stop the car if an obstacle is too close
  int currentDistance = getDistance();
  if (currentDistance < 20) {  // If an obstacle is closer than 20cm
    stopRobot();  // Assume stopRobot() stops the car
  }
  
}
