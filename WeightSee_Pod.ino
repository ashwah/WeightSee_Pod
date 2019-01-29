/* 
  WeightSee pod.

  Sketch works on Arduino Uno or Nano.
  
  Device acts as a slave on a I2C "group".
 
  Monitors weight data using a HX711 module.
*/

// Include libraries.
#include <Wire.h>
#include <WsGroup.h>

// Declare a WsGroup object to provide the required i2c function.
WsGroup ws_group;

void setup() {

  // Start a serial monitor.
  Serial.begin(9600);
  Serial.println("starting");
  
  // Run a group scan so we have an awareness of the address available.
  ws_group.runScan();

  // After the group has been scanned, it can attempt to issue an addres.
  int address = ws_group.issueAddress();
  Serial.print("Setting address: ");
  Serial.println(address);

  // TODO Some fail-over behavior if there are no addresses.

  // We can join the I2C bus with the issued address from the group.
  Wire.begin(address);

  // Do a final check to see that this address hasn't been duplicated on
  // this group. If it has, repeat the above process of scanning and issuing 
  // an address.
  Serial.print("Checking address:");
  Serial.println(address);  
  while (ws_group.queryAddress(address)) {
    ws_group.runScan();
    address = ws_group.issueAddress();    
    Serial.print("Checking address:");
    Serial.println(address);
  }

  // Register the 'request event' which is used to transmit info from this
  // device to the master.
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void loop() {
  
  delay(100);
}

// Function that executes whenever data is requested by master.
//
// This function is registered as an event, see setup().
//
// Here we broadcase various messages to the master:
// - I am alive.
// - I have new weight data.
// - I require a device ID.
void requestEvent() {
  Serial.println("request");
  Wire.write("hello "); // respond with message of 6 bytes
  // as expected by master
}

// Function that executes whenever data is received from master
// this function is registered as an event, see setup().
//
// Here we receive incoming data. E.g. a device ID.
void receiveEvent(int howMany) {
  Serial.println("receive");
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}
