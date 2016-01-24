#include <DHT.h>

#include <SPI.h>

#include <SD.h>

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#define DHTPIN 7     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
int CS_pin = 10;
void setup() {
  Serial.begin(9600);
  //Serial.println("DHTxx test!");
  //Serial.println("Initializing Card");
  pinMode(CS_pin, OUTPUT);
  if(!SD.begin(CS_pin)){
    //Serial.println("Card Failed");
    return;
  }
  //Serial.println("Card Ready");
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(3000);
  File dataFile = SD.open("dataLog.txt", FILE_WRITE);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  if(dataFile){
  //Serial.print("Time: ");
  int time = millis() / 1000;
  //prints time since program started
  //Serial.println(time);
  dataFile.print("Time: ");
  dataFile.println(time);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    //Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  /*
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  */
  dataFile.print("Humidity: ");
  dataFile.print(h);
  dataFile.print(" %\t");
  dataFile.print("Temperature: ");
  dataFile.print(t);
  dataFile.print(" *C ");
  dataFile.print(f);
  dataFile.print(" *F\t");
  dataFile.print("Heat index: ");
  dataFile.print(hic);
  dataFile.print(" *C ");
  dataFile.print(hif);
  dataFile.println(" *F");
  dataFile.close();
  }
  else{
    //Serial.println("Couldn't Access File");
    dataFile.close();
  }
}
