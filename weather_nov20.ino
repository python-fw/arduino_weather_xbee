// Based on http://cs.smith.edu/dftwiki/index.php/Tutorial:_Arduino_and_XBee_Communication

/*
  Xbee communication from Arduino to Raspberry Pi
  
  Arduino takes weather sensor readings and transmits via XBee wireless 
  to another XBee connected to Raspberry Pi.

  Arduino circuit: 
  * RX is digital pin 2 (connect to TX of XBee)
  * TX is digital pin 3 (connect to RX of XBee)
  * A4 connects to SDA of Barometer sensor (BMP085) 
  * A5 connects to SCL of Barometer sensor (CMP085)
  * Digital pin 7 connects pin 2 of DHT22 
  * A0 connects to photoresister
  
  
  
  Raspberry Pi circuit:
  * 3.3v to XBee 5V
  * Ground to XBee GND
  * UART RX to XBee TX
  * UART TX to XBee RX
 
*/

// import libraries
#include <SoftwareSerial.h>
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>

// Unique identifier for sensors
String uniqueID = "crakins001";

// Temperature Sensor DHT22 settings
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

// XBee settings
SoftwareSerial xbee(2, 3); // RX, TX

//instance of thermometer and baramoeter
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;

//photoresister settings
  int photoPin = 0;
  
void setup()  {
   Serial.begin(9600);
   if (!bmp.begin()) {
     Serial.println("Cannot connect to sensors");
     while (1) {}
   }
   // Serial.println( "Arduino sending data via XBee to Raspberry Pi" ); 
   //turn on DHT
   dht.begin();
     
   // set the data rate for the SoftwareSerial port
   xbee.begin( 9600 );
}

void loop()  {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float t = dht.readTemperature();
  float tb = bmp.readTemperature();
  float h = dht.readHumidity();
  float p = bmp.readPressure();
  float b = bmp.readAltitude();
  float photo = analogRead(photoPin);

  
  if (isnan(t) || isnan(tb) || isnan(h) || isnan(p) || isnan(b) || isnan(photo)){
   Serial.println("Error reading sensors");
   xbee.println("Error reading sensors");
  } else {
    // print to Serial Monitor
    Serial.print(t);
    Serial.print(",");
    Serial.print(tb);
    Serial.print(",");
    Serial.print(h);
    Serial.print(",");
    Serial.print(p);
    Serial.print(",");
    Serial.print(b);
    Serial.print(",");
    Serial.print(photo);
    Serial.print(",");
    Serial.print(uniqueID);
    Serial.println();
    
    //transmit via Xbee
    xbee.print(t);
    xbee.print(",");
    xbee.print(tb);
    xbee.print(",");
    xbee.print(h);
    xbee.print(",");
    xbee.print(p);
    xbee.print(",");
    xbee.print(b);
    xbee.print(",");
    xbee.print(photo);
    xbee.print(",");
    xbee.print(uniqueID);
    xbee.println();

    
    // delay sending another reading
    delay(2000);
  }
}
