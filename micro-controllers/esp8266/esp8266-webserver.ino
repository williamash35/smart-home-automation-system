#include <ESP8266WiFi.h>                                    // Load Wi-Fi library
const char* ssid     = "zzz";                               // Wifi UserName 
const char* password = "asdf12345";                         // Wifi Passwors
WiFiServer server(80);                                      // Set web server port number to 80
String header;                                              // Variable to store the HTTP request

String output5State = "off";                                // Auxiliar variables to store the current output state
String output4State = "off";
String output14State = "off";
String output12State = "off";

const int output5 = 5;                                      // Assign output variables to GPIO pins                                                        
const int output4 = 4;
const int output14 = 14;
const int output12 = 12;

void setup() {
  Serial.begin(115200);
  
  pinMode(output5, OUTPUT);                                 // Initialize the output variables as outputs
  pinMode(output4, OUTPUT);
  pinMode(output14, OUTPUT);
  pinMode(output12, OUTPUT);
  
  digitalWrite(output5, LOW);                               // Set outputs to LOW
  digitalWrite(output4, LOW);
  digitalWrite(output14, LOW);
  digitalWrite(output12, LOW);

  
  Serial.print("Connecting to ");                          // Connect to Wi-Fi network with SSID and password
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");                                     // Print local IP address and start web server
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();                 // Listen for incoming clients

  if (client) {                                           // If a new client connects,
    Serial.println("New Client.");                        // print a message out in the serial port
    String currentLine = "";                              // make a String to hold incoming data from the client
    while (client.connected()) {                          // loop while the client's connected
      if (client.available()) {                           // if there's bytes to read from the client,
        char c = client.read();                           // read a byte, then
        Serial.write(c);                                  // print it out the serial monitor
        header += c;
        if (c == '\n') {                                  // if the byte is a newline character
                                                          // if the current line is blank, you got two newline characters in a row.
                                                          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
                                                          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                                                          // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            
            if (header.indexOf("GET /5/on") >= 0) {                       // turns the GPIOs on and off
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            } else if (header.indexOf("GET /14/on") >= 0) {
              Serial.println("GPIO 14 on");
              output14State = "on";
              digitalWrite(output14, HIGH);
            } else if (header.indexOf("GET /14/off") >= 0) {
              Serial.println("GPIO 14 off");
              output14State = "off";
              digitalWrite(output14, LOW);
            } else if (header.indexOf("GET /12/on") >= 0) {
              Serial.println("GPIO 12 on");
              output12State = "on";
              digitalWrite(output12, HIGH);
            } else if (header.indexOf("GET /12/off") >= 0) {
              Serial.println("GPIO 12 off");
              output12State = "off";
              digitalWrite(output12, LOW);
            }
            
            // Dashboard Webpage (ESP local web server)
            client.println("<!DOCTYPE html><html>");                                                                                   // Display the HTML web page
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<title>uAutomate</title>");
            client.println("<link rel=\"stylesheet\" type=\"text/css\" href=\"https://cdnjs.cloudflare.com/ajax/libs/semantic-ui/2.4.1/semantic.min.css\">");

            // Styles                                                                                                                          // CSS to style the on/off buttons 
            client.println("<style> i.icon {font-size: 1.3em;padding-right: 1.5em;}");
            client.println(".item {font-size:1.05em;} </style></head>");
                
            // Pointing Menu
            client.println("<body><div class=\"ui fixed inverted menu borderless\"><div class=\"ui container\"><div class=\"item\"><a href=\"/\"><i class=\"code icon\"></i>üAutomate</a></div><a class=\"item\"href=\"/dashboard\">Dashboard</a><div class=\"right menu\"><a class=\"item\">Sign Up</a><a class=\"item\">Login</a></div></div></div>");
            
            // Switch 1
            client.println("<div class=\"ui main text container raised segment\"style=\"margin-top:7em\"><div class=\"ui items\"style=\"padding:2em;margin-top:0\"><h1 class=\"ui header\">Dashboard</h1><div class=\"ui divider\"style=\"margin-bottom:50px\"></div><div class=\"item\"><div class=\"ui small image\"><img src=\"https://images.unsplash.com/photo-1492539161849-b2b18e79c85f?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=500&q=60\"></div><divclass=\"middle aligned content\"><div class=\"header\">GPIO5</div><div class=\"description\"><p>Controls the Bulb</p></div><divclass=\"extra\">");if(output5State=="off"){client.println("<a href=\"/5/on\"><button class=\"ui button\">ON</button></a>");}else{client.println("<a href=\"/5/off\"><button class=\"ui button\">OFF</button></a>");}client.println("</div></div></div><div class=\"ui divider\"></div>");
            
            // Switch 2 
            client.println("<div class=\"item\"><div class=\"ui small image\"><img src=\"https://images.unsplash.com/photo-1523437345381-db5ee4df9c04?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=500&q=60\"></div><div class=\"middle aligned content\"><div class=\"header\">GPIO4</div><div class=\"description\"><p>Controls the Fan</p></div><div class=\"extra\">");if(output4State=="off"){client.println("<a href=\"/4/on\"><button class=\"ui button\">ON</button></a>");}else{client.println("<a href=\"/4/off\"><button class=\"ui button\">OFF</button></a>");}client.println("</div></div></div><div class=\"ui divider\"></div>");
            
            // Switch 3
            client.println("<div class=\"item\"><div class=\"ui small image\"><img src=\"https://images.unsplash.com/photo-1541480601022-2308c0f02487?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=500&q=60\"></div><div class=\"middle aligned content\"><div class=\"header\">GPIO14</div><div class=\"description\"><p>Controls the Alarm</p></div><div class=\"extra\">");if(output14State=="off"){client.println("<a href=\"/14/on\"><button class=\"ui button\">ON</button></a>");}else{client.println("<a href=\"/14/off\"><button class=\"ui button\">OFF</button></a>");}client.println("</div></div></div><div class=\"ui divider\"></div>");
            
            // Switch 4
            client.println("<div class=\"item\"><div class=\"ui small image\"><img src=\"https://images.unsplash.com/photo-1541519920349-237d9c325cac?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=500&q=60\"></div><div class=\"middle aligned content\"><div class=\"header\">GPIO12</div><div class=\"description\"><p>Controls the LEDs</p></div><div class=\"extra\">");if(output12State=="off"){client.println("<a href=\"/12/on\"><button class=\"ui button\">ON</button></a>");}else{client.println("<a href=\"/12/off\"><button class=\"ui button\">OFF</button></a>");}client.println("</div></div></div></div></div>");
            
                                                                                                                                
            /*if (output5State=="off") {                                                                                        // If the output5State is off, it displays the ON button  
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            
            client.println("<p>GPIO 4 - State " + output4State + "</p>");                                                     // Display current state, and ON/OFF buttons for GPIO 4       
            if (output4State=="off") {                                                                                        // If the output4State is off, it displays the ON button
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
  
            client.println("<p>GPIO 14 - State " + output14State + "</p>");                                                   // Display current state, and ON/OFF buttons for GPIO 14       
            if (output14State=="off") {                                                                                       // If the output14State is off, it displays the ON button
              client.println("<p><a href=\"/14/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/14/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
  
            client.println("<p>GPIO 12 - State " + output12State + "</p>");                                                   // Display current state, and ON/OFF buttons for GPIO 12       
            if (output12State=="off") {                                                                                       // If the output4State is off, it displays the ON button 
              client.println("<p><a href=\"/12/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/12/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            */
            client.println("</body></html>");
            
            client.println();                                                       // The HTTP response ends with another blank line
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {                                                    // if you got anything else but a carriage return character,
          currentLine += c;                                                        // add it to the end of the currentLine
        }
      }
    }
    header = "";                                                                  // Clear the header variable
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}