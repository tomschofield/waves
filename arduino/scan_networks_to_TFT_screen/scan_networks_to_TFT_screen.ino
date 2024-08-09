/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "SPI.h"
#include <Adafruit_GFX.h>  // Core graphics library
// #include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeMono9pt7b.h>

#include "Adafruit_ILI9341.h"
#include <WiFiNINA.h>
// For the Adafruit shield, these are the default.
#define TFT_DC 2
#define TFT_CS 5
// mosi is /DIN
#define TFT_MOSI 8
#define TFT_CLK 9
#define TFT_RST 6

//MISO is BLs
#define TFT_MISO 7

bool ambientSSIDS_are_logged = false;
String poem = "";

const int num_ssids = 15;
const int max_num_lines = 13;
String lines[max_num_lines] = { "", "", "","", "", "","", "", "","" , "", "","" };

String ambientSSIDs[num_ssids] = { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };
// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() {
  Serial.begin(9600);
  Serial.println("ILI9341 Test!");

  tft.begin();
  tft.setRotation(3);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(1);

  tft.setFont(&FreeMono9pt7b);
  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x");
  Serial.println(x, HEX);


  ///set up wifi
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

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  printMacAddress(mac);
}


void loop(void) {

  Serial.println("Scanning available networks...");
  // listNetworks();



  listNetworks();
  delay(5000);
}


void listNetworks() {
  // scan for nearby networks:
  Serial.println("** Scan Networks **");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("Couldn't get a WiFi connection");
    tft.println("Couldn't get a WiFi connection");
    // while (true)
    ;
  }

  // print the list of networks seen:
  Serial.print("number of available networks:");
  Serial.println(numSsid);
  if (numSsid == 0) tft.println("No WiFi networks visible");
  int yPos = 0;
  tft.fillScreen(ILI9341_BLACK);
  yield();
  tft.setCursor(0, 20);

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    // Serial.print(thisNet);
    // Serial.print(") ");
    // Serial.print(WiFi.SSID(thisNet));
    // Serial.print("\tSignal: ");
    // Serial.print(WiFi.RSSI(thisNet));
    // Serial.print(" dBm");
    // Serial.print("\tEncryption: ");
    // printEncryptionType(WiFi.encryptionType(thisNet));
    // tft.fillScreen(ILI9341_BLACK);
    unsigned long start = micros();
    // tft.setCursor(0, yPos);
    // tft.setTextColor(ILI9341_WHITE);
    // tft.setTextSize(1);
    String thisSSID = WiFi.SSID(thisNet);
    int index = 0;

    if (thisSSID == "Tom’s phone") {
      Serial.println("ambient SSID collections end");
      ambientSSIDS_are_logged = true;
    }


    if (inArray(thisSSID, ambientSSIDs, num_ssids) == false && ambientSSIDS_are_logged == false) {
      index = getFirstEmptyArrayIndex(thisSSID, ambientSSIDs, num_ssids);
      ambientSSIDs[index] = thisSSID;
       Serial.print("ambient SSIDS: ");
      printArray(ambientSSIDs, num_ssids);
    }
   

    if (ambientSSIDS_are_logged == false) {
      tft.setTextColor(ILI9341_RED);
      tft.println(WiFi.SSID(thisNet));
    } else {
      tft.setTextColor(ILI9341_WHITE);

      //first check this isn't an ambien ssid
      if (inArray(thisSSID, ambientSSIDs, num_ssids) == false) {
        
        //now check that we don't already have it on screen
        if (inArray(thisSSID, lines, max_num_lines)==false && thisSSID != "Tom’s phone") {
          Serial.print("got new SSID which is not an ambient one: ");
          Serial.println(thisSSID);
          // Serial.print("poem.length(): ");
          // Serial.println(poem.length());

          //for the first occassion
          if (poem.length() == 0) {
            Serial.println("got first poem line");
            lines[0] = thisSSID;
            
          } else {

            //count number of lines
            int num_lines = 0;

            for (int i = 0; i < max_num_lines; i++) {
              Serial.print("lines length at:");
              Serial.print(i);
              Serial.print(" : ");
              Serial.println(lines[i].length());
              if (lines[i].length() > 0) num_lines = i+1;
            }
            // for (int i = 0;i < poem.length(); i++) {
            //   String c(poem.charAt(i));
            //   if (c == "\n") {

            //     num_lines++;
            //   }

            // }
            Serial.print("num_lines : ");
            Serial.println(num_lines);


            //if the array is full and this is a new ssid
            if (num_lines >= max_num_lines ) {

              //shuffle array and insert new line at the beginning
              Serial.println("lines before shuffle");
              printArray(lines, max_num_lines);
              for (int i = max_num_lines-1; i >0 ; i--) {
                lines[i]=lines[i-1];
              }
              lines[0]=thisSSID;

              Serial.println("lines post shuffle");
              printArray(lines, max_num_lines);

              // Serial.print("removing the following section ");
              // int ind = poem.indexOf("\n");
              // String temp = poem.substring(ind, poem.length());
              // poem = thisSSID + temp;


            } else {
              int ind = 0;
              for (int i = 0; i < max_num_lines; i++) {
                if (lines[i].length() == 0) {
                  ind= i;
                  break;
                }
              }
              lines[ind]=thisSSID;
              
            }
          }
        }
      }
    }
  }
  poem = arrayToString(lines, max_num_lines);
  tft.println(poem);
  //delay(6000);
}
String arrayToString(String arr[], int arr_length){
  String str="";
if (arr_length > 0) {
    for (int i = 0; i < arr_length - 1; i++) {
      if (arr[i].length() > 0) {
        str+=arr[i]+"\n";
      }
    }
    str+=arr[arr_length - 1];
  }
  return str;
}
void printArray(String arr[], int arr_length) {
  if (arr_length > 0) {
    for (int i = 0; i < arr_length - 1; i++) {
      if (arr[i].length() > 0) {
        Serial.print(arr[i]);
        Serial.print(", ");
      }
    }
    Serial.println(arr[arr_length - 1]);
  }
}
int getFirstEmptyArrayIndex(String needle, String haystack[], int haystack_length) {
  int index = 0;
  for (int i = 0; i < haystack_length; i++) {
    if (haystack[i].length() == 0) index = i;
  }
  return index;
}
bool inArray(String needle, String haystack[], int haystack_length) {
  bool needle_exists = false;
  for (int i = 0; i < haystack_length; i++) {
    if (haystack[i] == needle) needle_exists = true;
  }
  return needle_exists;
}



void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.println("WEP");
      break;
    case ENC_TYPE_TKIP:
      Serial.println("WPA");
      break;
    case ENC_TYPE_CCMP:
      Serial.println("WPA2");
      break;
    case ENC_TYPE_NONE:
      Serial.println("None");
      break;
    case ENC_TYPE_AUTO:
      Serial.println("Auto");
      break;
    case ENC_TYPE_UNKNOWN:
    default:
      Serial.println("Unknown");
      break;
  }
}


void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}