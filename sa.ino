#include <driver/ledc.h>
#include <WiFi.h> 

String header;

// ########## MOTOR CONTROL ##########
// Motor-1
const int motorPin1 = 27; 
const int motorPin2 = 26; 
const int enablePin = 14; // PWM Control Pins

// Motor-2 
const int motorPin3 = 17;
const int motorPin4 = 16;
const int enablePin2 = 4; // PWM Control Pins 


// Setting PWM properties
const int freq = 30000;
const int pwmChannel1 = enablePin;
const int pwmChannel2 = enablePin2;
const int resolution = 8;



// ########## Direction Functions ###########

void rigth(){
digitalWrite(motorPin1,HIGH);
digitalWrite(motorPin2,LOW); 
digitalWrite(motorPin3,LOW);
digitalWrite(motorPin4,LOW);
}

void left(){
digitalWrite(motorPin1,LOW);
digitalWrite(motorPin2,LOW);
digitalWrite(motorPin3,HIGH);
digitalWrite(motorPin4,LOW);
  
}

void forward(){
digitalWrite(motorPin1,HIGH);
digitalWrite(motorPin2,LOW);
digitalWrite(motorPin3,HIGH);
digitalWrite(motorPin4,LOW);
}

void backward(){
  digitalWrite(motorPin1,LOW);
digitalWrite(motorPin2,HIGH);
digitalWrite(motorPin3,LOW);
digitalWrite(motorPin4,HIGH);
}

void allLow(){
digitalWrite(motorPin1,LOW);
digitalWrite(motorPin2,LOW);
digitalWrite(motorPin3,LOW);
digitalWrite(motorPin4,LOW);
}

 // ########## Duty Cycle PWM ##########
int dutyCycle1 = 200;
int dutyCycle2 = 200;

unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// ########## WIFI CONFIGURATION #############

// Replace with your network credentials
const char* ssid = "Movsec 2";
const char* password = "movsec2022!";

// Set web server port number to 80
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  // sets the pins as outputs for mmotors
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(enablePin2, OUTPUT);

  digitalWrite(motorPin1,LOW);
  digitalWrite(motorPin2,LOW);
  digitalWrite(motorPin3,LOW);
  digitalWrite(motorPin4,LOW);
  // configure LED PWM functionalitites
  ledcSetup(enablePin, freq, resolution);
  ledcAttachPin(14, enablePin);
  ledcWrite(enablePin, dutyCycle1);

  // Initialize PWM for channel 2
  ledcSetup(enablePin2, freq, resolution);
  ledcAttachPin(4, enablePin2);
  ledcWrite(enablePin2, dutyCycle2);

  // #### WIFI SETUP ####### 

    // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print("Error");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

}
// ############## HTML Needs ############
String output26State = "off";
String output27State = "off";
String output28State = "off";
String output29State = "off";


  
char moves[100];
int moveIndex;
// ############### LOOP  ######################
void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients
  
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
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
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              moves[moveIndex] = 'r';

              // debug index
              Serial.print("index = ");
              Serial.println(String(moveIndex));
              
              moveIndex++;

              // rigth();
              
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              moves[moveIndex] = 's';

              // debug index
              Serial.print("index = ");
              Serial.println(String(moveIndex));
              
              moveIndex++;
              
              //allLow();
              
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              moves[moveIndex] = 'l';

              // debug index
              Serial.print("index = ");
              Serial.println(String(moveIndex));
              
              moveIndex++;
               //left();
               
              
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              moves[moveIndex] = 's';

              // debug index
              Serial.print("index = ");
              Serial.println(String(moveIndex));
              
              moveIndex++;
              //allLow();
              
            } else if (header.indexOf("GET /28/on") >= 0) {
              Serial.println("GPIO 28 on");
               output28State = "on";
              moves[moveIndex] = 'b';

              // debug index
              Serial.print("index = ");
              Serial.println(String(moveIndex));
              
              moveIndex++;
              //forward();
              
            } else if (header.indexOf("GET /28/off") >= 0) {
              Serial.println("GPIO 28 off");
               output28State = "off";
              moves[moveIndex] = 's';

              // debug index
              Serial.print("index = ");
              Serial.println(String(moveIndex));
              
              moveIndex++;
              //allLow();
              
            } else if (header.indexOf("GET /29/on") >= 0) {
              Serial.println("GPIO 29 on");
               output29State = "on";
              moves[moveIndex] = 'f';

              // debug index
              Serial.print("index = ");
              Serial.println(String(moveIndex));
              
              moveIndex++;
              //backward();
              
            } else if (header.indexOf("GET /29/off") >= 0) {
              Serial.println("GPIO 29 off");
               output29State = "off";
              moves[moveIndex] = 's';

              // debug index
              Serial.print("index = ");
              Serial.println(String(moveIndex));
              
              moveIndex++;
              //allLow();
            } else if (header.indexOf("GET /ok") >= 0) {
              Serial.println("Running moves!");
              for (int i = 0; i < 100; i++) {
                if (moves[i] == 'r') {
                  Serial.println(" move: r ");
                } else if (moves[i] == 's') {
                  Serial.println(" move: s ");
                } else if (moves[i] == 'b') {
                  Serial.println(" move: b ");
                } else if (moves[i] == 'f') {
                  Serial.println(" move: f ");
                }else if (moves[i] == 'l') {
                  Serial.println(" move: l ");
                } else {
                  continue;
                }
              }
              
              for (int i = 0; i < 100; i++) {
                if (moves[i] == 'r') {
                  rigth();
                } else if (moves[i] == 'l') {
                  left();
                } else if (moves[i] == 'b') {
                  backward();
                } else if (moves[i] == 'f') {
                  forward();
                } else if (moves[i] == 's') {
                  allLow();
                }else {
                  continue;
                }

                delay(2500);
              }
              moveIndex = 0;
              memset(moves, 0, sizeof(moves));
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">RIGHT</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2 button3 button4\">STOP</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button       
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">LEFT</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2 button3 button4\">STOP</button></a></p>");
            }


            // Display current state, and ON/OFF buttons for GPIO 28  
            client.println("<p>GPIO 28 - State " + output28State + "</p>");
            // If the output28State is off, it displays the ON button       
            if (output28State=="off") {
              client.println("<p><a href=\"/28/on\"><button class=\"button\">BACKWARD</button></a></p>");
            } else {
              client.println("<p><a href=\"/28/off\"><button class=\"button button2 button3 button4\">STOP</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 29  
            client.println("<p>GPIO 29 - State " + output29State + "</p>");
            // If the output29State is off, it displays the ON button       
            if (output29State=="off") {
              client.println("<p><a href=\"/29/on\"><button class=\"button\">FORWARD</button></a></p>");
            } else {
              client.println("<p><a href=\"/29/off\"><button class=\"button button2 button3 button4\">STOP</button></a></p>");
            }

            client.println("<p>ENTER</p>");
            client.println("<p><a href=\"/ok\"><button class=\"button\">ENTER</button></a></p>");
            
            client.println("</body></html>");
            
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
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
