#include <ros.h>
#include <std_msgs/Int32.h>
#include <Servo.h>

ros::NodeHandle nh;

std_msgs::Int32 inputMessage;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
int c;

const int x_pin = A0;
const int y_pin = A1;

bool y_moved = false;
bool x_moved = false;

void setupServos() {
  servo1.attach(7); // Connect servo1 to pin 9
  servo2.attach(8); // Connect servo2 to pin 8
  servo3.attach(9); // Connect servo3 to pin 9
  servo4.attach(10); // Connect servo4 to pin 10
}

void callBackFunction(const std_msgs::Int32 &receivedMessage) {
  inputMessage = receivedMessage; // Store the received message
}

ros::Subscriber<std_msgs::Int32> sub("information", &callBackFunction);

void setup() {
  nh.initNode();
  nh.subscribe(sub);
  pinMode(LED_BUILTIN, OUTPUT); // Set the LED pin as output
  setupServos();
  
}

void moveServo(Servo &servo, bool &moved, int value, int minAngle, int maxAngle) {
  if (value > 600 && !moved) {
    int angle = servo.read();
    angle = min(angle + 5, maxAngle);
    delay(100);
    servo.write(angle);
    moved = true;
  } else if (value < 600 && value > 400) {
    moved = false;
  } else if (value < 400 && !moved) {
    int angle = servo.read();
    angle = max(angle - 5, minAngle);
    delay(100);
    servo.write(angle);
    moved = true;
  }
}

void manualControl() {
  int x_data = analogRead(x_pin);
  int y_data = analogRead(y_pin);

  moveServo(servo1, y_moved, y_data, 60, 180); // Servo1 controls the y-axis
  moveServo(servo2, x_moved, x_data, 0, 120);   // Servo2 controls the x-axis
  moveServo(servo3, y_moved, y_data, 0, 270); // Servo3 controls the y-axis
  moveServo(servo4, x_moved, x_data, 0, 180);   // Servo4 controls the x-axis
}

void loop() {
  nh.spinOnce(); // Process ROS communication
  
  if (inputMessage.data == 1) {
    manualControl(); 
    int angle1 = servo1.read();  // Read angle of servo 1
  int angle2 = servo2.read();  // Read angle of servo 2
  int angle3 = servo3.read();  // Read angle of servo 3
  int angle4 = servo4.read();  // Read angle of servo 4
  
  // Print angles to serial monitor
  Serial.print("Servo 1 Angle: ");
  Serial.println(angle1);
  Serial.print("Servo 2 Angle: ");
  Serial.println(angle2);
  Serial.print("Servo 3 Angle: ");
  Serial.println(angle3);
  Serial.print("Servo 4 Angle: ");
  Serial.println(angle4);
  
  delay(1000);// Manually control servos
  } else if (inputMessage.data == 2) {
    if (c==0){
    servo3.write(60);
    delay(400); // Move servo1 to a predefined position (e.g., 90 degrees)
    servo4.write(20);
    delay(400); // Move servo2 to a predefined position (e.g., 90 degrees)
    servo1.write(100);
    delay(400); // Move servo3 to a predefined position (e.g., 90 degrees)
    servo2.write(30); 
    delay(400);
    servo4.write(0);
    delay(400);
    servo2.write(0);
    delay(400);
    servo1.write(170);
    delay(400);
    c==1;}// Move servo4 to a predefined position (e.g., 90 degrees)
  } else if (inputMessage.data == 3) {
    servo1.write(45); // Move servo1 to another predefined position (e.g., 45 degrees)
    servo2.write(135); // Move servo2 to another predefined position (e.g., 135 degrees)
    servo3.write(45); // Move servo3 to another predefined position (e.g., 45 degrees)
    servo4.write(135); // Move servo4 to another predefined position (e.g., 135 degrees)
  }
  
  delay(100); // Add a short delay to avoid overwhelming the serial port
}
