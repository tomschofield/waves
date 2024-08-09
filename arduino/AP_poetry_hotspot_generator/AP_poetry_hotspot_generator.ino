

#include <SPI.h>

#include  <WiFiNINA.h>


  ///////please enter your sensitive data in the Secret tab/arduino_secrets.h

  char ssid[] =
    "SECRET_SSIDaghlhglashglsadhgaslhg";  // your network SSID (name)

char pass[] =
  "SECRET_PASS";  // your network password (use for WPA, or use as key for WEP)

int keyIndex =
  0;  // your network key index number (needed only for WEP)





char ssid1[] =
  "cold air waves";

char ssid2[] =
  "or warm sinew";

char ssid3[] =
  "organ flesh apulse";

char ssid4[] =
  "your heart will melt";

char ssid5[] =
  "the surface of your flesh";

char ssid6[] =
  "porous to me";

char ssid7[] =
  "chiming subcutaenously";

char ssid8[] =
  "while the insulated cables";

char ssid9[] =
  "run into plaster or brickwork";

char ssid10[] = "will start to flow";

char ssid11[] =
  "having translated my oscillations";

char ssid12[] =
  "through veins & gritty marrow";

char ssid13[] =
  "and indiscriminate body parts";

char ssid14[] =
  "orgiastic in space";

char ssid15[] =
  "has melted tired iron";

char ssid16[] =
  "why can't I then";

char ssid17[] =
  "throughout this all";

char ssid18[] =
  "force you to bend";



char* ssids[] = { ssid1, ssid2, ssid3, ssid4, ssid5, ssid6, ssid7, ssid8, ssid9, ssid10, ssid11, ssid12, ssid13, ssid14, ssid15, ssid16, ssid17, ssid18 };

int ssid_index =
  0;

int ssids_length = 18;



int led = LED_BUILTIN;

int status = WL_IDLE_STATUS;

WiFiServer server(80);



void setup() {

  //Initialize serial and wait for port to open:

  Serial.begin(9600);

  while (!Serial) {

    ;  // wait for serial port to connect. Needed for native USB port only
  }



  Serial.println("Access Point Web Server");



  pinMode(led, OUTPUT);  // set the LED pin mode



  // check for the WiFi module:

  if (WiFi.status() == WL_NO_MODULE) {

    Serial.println("Communication with WiFi module failed!");

    // don't continue

    while (true)
      ;
  }



  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {

    Serial.println("Please upgrade the firmware");
  }



  // by default the local IP address will be 192.168.4.1

  // you can override it with the following:

  // WiFi.config(IPAddress(10, 0, 0, 1));



  // print the network name (SSID);

  startAP(ssids[ssid_index]);

  // wait 10 seconds for connection:

  delay(10000);



  // start the web server on port 80

  // server.begin();



  // you're connected now, so print out the status

  printWiFiStatus();
}





void loop() {

  // compare the previous status to the current status

  delay(5000);

  stopAP();

  delay(5000);

  ssid_index++;

  if (ssid_index >= ssids_length) {

    ssid_index = 0;
  }

  startAP(ssids[ssid_index]);





  if (status != WiFi.status()) {

    // it has changed update the variable

    status = WiFi.status();



    if (status == WL_AP_CONNECTED) {

      // a device has connected to the AP

      Serial.println("Device connected to AP");

    } else {

      // a device has disconnected from the AP, and we are back in listening mode

      Serial.println("Device disconnected from AP");
    }
  }


  // WiFiClient client = server.available(); // listen for incoming clients



  // if (client) { // if you get a client,

  // Serial.println("new client"); // print a message out the serial port

  // String currentLine = ""; // make a String to hold incoming data from the client

  // while (client.connected()) { // loop while the client's connected

  // delayMicroseconds(10); // This is required for the Arduino Nano RP2040 Connect - otherwise it will loop so fast that SPI will never be served.

  // if (client.available()) { // if there's bytes to read from the client,

  // char c = client.read(); // read a byte, then

  // Serial.write(c); // print it out to the serial monitor

  // if (c == '\n') { // if the byte is a newline character



  // // if the current line is blank, you got two newline characters in a row.

  // // that's the end of the client HTTP request, so send a response:

  // if (currentLine.length() == 0) {

  // // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)

  // // and a content-type so the client knows what's coming, then a blank line:

  // client.println("HTTP/1.1 200 OK");

  // client.println("Content-type:text/html");

  // client.println();



  // // the content of the HTTP response follows the header:

  // client.print("Click <a href=\"/H\">here</a> turn the LED on<br>");

  // client.print("Click <a href=\"/L\">here</a> turn the LED off<br>");



  // // The HTTP response ends with another blank line:

  // client.println();

  // // break out of the while loop:

  // break;

  // }

  // else { // if you got a newline, then clear currentLine:

  // currentLine = "";

  // }

  // }

  // else if (c != '\r') { // if you got anything else but a carriage return character,

  // currentLine += c; // add it to the end of the currentLine

  // }



  // // Check to see if the client request was "GET /H" or "GET /L":

  // if (currentLine.endsWith("GET /H")) {

  // digitalWrite(led, HIGH); // GET /H turns the LED on

  // }

  // if (currentLine.endsWith("GET /L")) {

  // digitalWrite(led, LOW); // GET /L turns the LED off

  // }

  // }

  // }

  // // close the connection:

  // client.stop();

  // Serial.println("client disconnected");

  // }
}

void startAP(char ssid[]) {

  Serial.print("Creating access point named: ");

  Serial.println(ssid);



  // Create open network. Change this line if you want to create an WEP network:

  status = WiFi.beginAP(ssid, pass);

  if (status != WL_AP_LISTENING) {

    Serial.println("Creating access point failed");

    // don't continue

    while (true)
      ;
  }
}

void stopAP() {

  WiFi.end();
}



void printWiFiStatus() {

  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());



  // print your WiFi shield's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);



  // print where to go in a browser:

  Serial.print("To see this page in action, open a browser to http://");

  Serial.println(ip);
}
