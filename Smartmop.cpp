#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

Servo myservo;

String command;             // String to store app command state.
int speedCar = 800;         // 400 - 1023.
int speed_Coeff = 3;

const char* ssid = "mana car";
const char* password = "hehehuhu";
ESP8266WebServer server(80);

// Motor pins
#define ENA   14  // Enable/speed motors Right        GPIO14(D5)
#define ENB   12  // Enable/speed motors Left         GPIO12(D6)
#define IN_1  15  // L298N in1 motors Right           GPIO15(D8)
#define IN_2  13  // L298N in2 motors Right           GPIO13(D7)
#define IN_3   2  // L298N in3 motors Left            GPIO2(D4)
#define IN_4   0  // L298N in4 motors Left            GPIO0(D3)

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

void stopServo() {
  myservo.write(90);
}
 

void motorControlFront(int enPin, int in1Pin, int in2Pin, int speed) {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  analogWrite(enPin, speed);
}

void motorControlBack(int enPin, int in1Pin, int in2Pin, int speed) {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(enPin, speed);
}

void goAhead(){ 
      motorControlFront(ENA, IN_2, IN_1, speedCar);
      motorControlFront(ENB, IN_3, IN_4, speedCar);
  }

void goBack(){ 
      motorControlBack(ENA, IN_2, IN_1, speedCar);
      motorControlBack(ENB, IN_3, IN_4, speedCar);
  }

void goRight() {
  motorControlBack(ENA, IN_2, IN_1, speedCar - 400);
  motorControlFront(ENB, IN_3, IN_4, speedCar); // Increase speed for turning
}

void goLeft() {
  motorControlFront(ENA, IN_2, IN_1, speedCar); // Increase speed for turning
  motorControlBack(ENB, IN_3, IN_4, speedCar - 400 );
}

void goAheadLeft() {
  motorControlFront(ENA, IN_1, IN_2, speedCar / speed_Coeff);
  motorControlFront(ENB, IN_3, IN_4, speedCar + 200); // Increase speed for turning
}

void goAheadRight() {
  motorControlFront(ENA, IN_1, IN_2, speedCar + 200); // Increase speed for turning
  motorControlFront(ENB, IN_3, IN_4, speedCar / speed_Coeff);
}

void goBackLeft() {
  motorControlBack(ENA, IN_1, IN_2, speedCar / speed_Coeff);
  motorControlBack(ENB, IN_3, IN_4, speedCar + 200); // Increase speed for turning
}

void goBackRight() {
  motorControlBack(ENA, IN_1, IN_2, speedCar + 200); // Increase speed for turning
  motorControlBack(ENB, IN_3, IN_4, speedCar / speed_Coeff);
}

void stopRobot(){  

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
 }

void HTTP_handleRoot(void) {
  if (server.hasArg("State")) {
    Serial.println(server.arg("State"));
  }
  server.send(200, "text/html", "");
  delay(1);
}

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
  // Control the servo motor
  // delay(1000);    // Add a delay to give the servo some time to move
  // servoControl();
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
  else if (command == "0") speedCar = 800;
  else if (command == "1") speedCar = 800;
  else if (command == "2") speedCar = 800;
  else if (command == "3") speedCar = 800;
  else if (command == "4") speedCar = 800;
  else if (command == "5") speedCar = 800;
  else if (command == "6") speedCar = 800;
  else if (command == "7") speedCar = 800;
  else if (command == "8") speedCar = 800;
  else if (command == "9") speedCar = 800;
  else if (command == "S") stopRobot();
  else if (command == "M") servoControl();
  else if (command == "O") stopServo();
  else stopRobot();
  //servoControl();
  

}




