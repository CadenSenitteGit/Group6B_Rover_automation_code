#include <WiFi.h>
#include <SoftwareSerial.h>
#include <PololuQik.h>

//Defines
#define forward_speed 80        //Set forward speed

#define backward_speed -80      //Set backward speed

#define turn_speed 60           //Set turn speed

#define turn_speed_b -60        //Set turn speed

#define degStep 5

//unsigned long myTime; //Watchdog Time Variable
//#define stopTime 200

// Replace with your network credentials
const char* ssid     = "Group6B-Rover";
const char* password = "123456789";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//Declares Pololu Qik motors for wheel and tracks
PololuQik2s9v1 qik(2, 22, 4);

//Declare starting bucket degree and starting base degree of servos
int bucketDeg = 90;
int baseDeg = 45;

//Setup and Initialization of Pololu Qik motors
void setup() {
  Serial.begin(115200);
  Serial.println("qik 2s9v1 dual serial motor controller");
  qik.init();

  //Sets wifi address and password for ESP wifi connection
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  //start server
  server.begin();

  //Servo logic
  ledcSetup(1,50,9);
  ledcSetup(2,50,9);
  ledcSetup(3,50,9);
  ledcSetup(4,50,9);
  ledcAttachPin(14, 1);
  ledcAttachPin(27, 2);
  ledcAttachPin(26, 3);
  ledcAttachPin(25, 4);
  updatePos();
}

void loop(){
  /*if (myTime + stopTime < millis()){
    delay(100);
    qik.setM0Speed(0);
    qik.setM1Speed(0);
    delay(300);
  }*/
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            
            //Watchdog
            //myTime = millis();

            //Controls speed and direction of motors/rover
            
            //forward
            if (header.indexOf("GET /g") >= 0) {
              qik.init();
              qik.setM0Speed(forward_speed);
              qik.setM1Speed(forward_speed);
           
            } 
            
            //backward
            else if (header.indexOf("GET /b") >= 0) {
             qik.init();
             qik.setM0Speed(backward_speed);
             qik.setM1Speed(backward_speed);
            } 

            //right
            else if (header.indexOf("GET /r") >= 0) {
              qik.init();
              qik.setM0Speed(turn_speed);
              qik.setM1Speed(turn_speed_b);
            } 
            
            //left
            else if (header.indexOf("GET /l") >= 0){
              qik.init();
              qik.setM0Speed(turn_speed_b);
              qik.setM1Speed(turn_speed);
            } 

            //stop
            else if (header.indexOf("GET /s") >= 0) {
              delay(100);
              qik.init();
              qik.setM0Speed(0);
              qik.setM1Speed(0);
            } 

            //Controls Speed and Degree of Servo Motors
            
            //arm up
            else if (header.indexOf("GET /u") >= 0) {
              //qik.setM0Speed(0);
              //qik.setM1Speed(0);
              if(baseDeg + degStep <=180){
                baseDeg = baseDeg + degStep;
              }
              updatePos();
            } 

            //arm down
            else if (header.indexOf("GET /d") >= 0) {
              //qik.setM0Speed(0);
              //qik.setM1Speed(0);
              if(baseDeg - degStep >= 0){
                baseDeg = baseDeg - degStep;
              }
              updatePos();
            }

            //scoop up
            else if (header.indexOf("GET /o") >= 0) {
              //qik.setM0Speed(0);
              //qik.setM1Speed(0);
              if(bucketDeg + degStep <=180){
                bucketDeg = bucketDeg + degStep;
              }
              updatePos();
            } 

            //scoop down
            else if (header.indexOf("GET /k") >= 0) {
              //qik.setM0Speed(0);
              //qik.setM1Speed(0);
              if(bucketDeg - degStep >= 0){
                bucketDeg = bucketDeg - degStep;
              }
              updatePos();
            }
          
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
  }
}

//PWM to degree conversion
void toDeg(int deg, int c){
  ledcWrite(c, (deg/3.4)+10);
}

//Servo degree updating
void updatePos(void){
  toDeg(baseDeg,1);
  toDeg(193-baseDeg,2);
  toDeg(bucketDeg,3);
  toDeg(180-bucketDeg,4);
}
