#include <AFMotor.h>
#include <SPI.h>
#include <SoftwareSerial.h>

#define MIN_SPEED 20    // Speeds below this are equivalent to 0
#define MAX_STEERING        255 // From Master code

// TODO: Check if motor numbers match with other code
AF_DCMotor motor2(4, MOTOR12_2KHZ); // for Steering
AF_DCMotor motor(2, MOTOR12_2KHZ); // Transmission motor
SoftwareSerial BTSerial(14, 15);    // RX, TX

int leaderSpeed;    // Used by SLAVE BT
uint8_t direction;

// From BT Tutorial
char command;
String string;

void setup() {

  Serial.begin(9600);
  Serial.print("Starting...\n");

//  BTSerial.begin(38400);  // Master
  BTSerial.begin(9600);   // Slave
  motor.run(FORWARD);    // FOR BLUETOOTH TESTING
}


void loop() {  
  string = "";  // Clear string
  
  while(BTSerial.available() > 0)
   {
    command = ((byte)BTSerial.read());
    
    if(command == ':')
    {
      break;
    }
    
    else
    {
      string += command;
    }
    
    delay(1); 
  }
  
  Serial.print("Cmd: ");
  Serial.println(string);

  control_commands(string);

}

void control_commands(String command) {
  
  if (command == "AF1") {
    motor.run(FORWARD);
    motor.setSpeed(200);
    Serial.println("Accel: FORWARD");
  }
  else if (command == "AB1") {
    motor.run(BACKWARD);
    motor.setSpeed(200);
    Serial.println("Accel: BACKWARD");
  }
  else if (command == "AF0" || command == "AB0") {
    motor.setSpeed(0);
    Serial.println("Accel: Stopping");
  }
  
  else if (command == "TL1") {
    motor2.run(BACKWARD);
    motor2.setSpeed(MAX_STEERING);
    Serial.println("Turn: LEFT");
  }
  else if (command == "TR1") {
    motor2.run(FORWARD);
    motor2.setSpeed(MAX_STEERING);
    Serial.println("Turn: RIGHT");
  }
  else if (command == "TL0" || command == "TR0") {
    motor2.setSpeed(0);
    Serial.println("Turn: Straight");
  }

  // DEPRECATED
  else if (command == "BRAKE") {
    // reset both motors 
    motor2.setSpeed(0);
    motor.setSpeed(0);
    Serial.println("Bing - Vehicle Stopped");
  }

}

// establish connection with slave RC cars and send commands
void broadcast_commands(String command) {
  // find connection
  // send command to RC cars
  control_commands(command);
}
