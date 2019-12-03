//====================================================================================================================================================================//
//*-------------------------------------------------------- Collect data DS18B20 and POST to database  --------------------------------------------------------------*//
//====================================================================================================================================================================//
//============== Create on Tuesday ==============//
//--- date        : 11/09/2019                ---//
//--- programmer  : dhiki handika             ---// 
//--- email       : dhkkihandika36@gmail.com  ---//
//===============================================//

//====================================================================================================================================================================//
//------------------------------------------------------------------ Initialization program --------------------------------------------------------------------------//
//====================================================================================================================================================================//
#include <OneWire.h>                                                                  // Include library one wire data bus communication 
#include <DallasTemperature.h>                                                        // Include library Dallas Temperature

int ONE_WIRE_BUS = D7;                                                                // Declaration D2 its pin that use to collect data from ESP8266
OneWire oneWire(ONE_WIRE_BUS);                                                        // Setup a oneWire instance to communicate with any OneWire devices      
DallasTemperature sensors(&oneWire);                                                  // Pass our oneWire reference to Dallas Temperature sensor 


#include <ESP8266WiFi.h>                                                              // Library use the wifi on ESP8266 device
#include <ESP8266HTTPClient.h>                                                        // Library can make ESP8266 interact with Web Server
#include <ArduinoJson.h>                                                              // Include Library  version 6.x.x

const char* ssid = "Parametrik RnD";                                                  // Your SSID address
const char* pass = "tabassam";                                                        // Password your SSID


//====================================================================================================================================================================//
//------------------------------------------------------------ Wifi Setup connection Porocedure ----------------------------------------------------------------------//
//====================================================================================================================================================================//
void setup_wifi() {                                                                   // Procedure Setup_wifi ()
  delay(10);                                                                          // Delay 10 ms
  if (WiFi.status() != WL_CONNECTED) {                                                // Function if wifi status not equal connected
    delay(10);                                                                        // Delay 10 ms
    Serial.println("Wifi connection to the network, ");                               // Print on serial monitor "Wifi connection to the network"
    Serial.println();                                                                 //            --||--
    Serial.print ("Connection to ");                                                  //            --||--
    Serial.println(ssid);                                                             // Print on serial monitor variable SSID can be create
    WiFi.begin (ssid, pass);                                                          // Inisialisasi pengaturan WiFi (SSID dan Password)
  }

  while (WiFi.status () != WL_CONNECTED) {                                            // While running program until WiFi status has been Connected
    delay(500);                                                                       // Delay 500 ms
    Serial.print(".");                                                                // Print on serial monitor ".."
  }
  randomSeed(micros());                                                               // Intializes to generate random number on a computer
  Serial.println("Connected");                                                        // Print on serial monitor "Connected"
  Serial.println("IP Address");                                                       // Print on serial monitor "IP Address"
  Serial.println(WiFi.localIP());                                                     // Print on serial monitor character inside function WiFi.LocalIP();
  Serial.println("");                                                                 // print on serial monitor
  delay(2000);                                                                        // Delay 2000 ms
}


//====================================================================================================================================================================//
//------------------------------------------------------------------- Setup Beginning Program ------------------------------------------------------------------------//
//====================================================================================================================================================================//
void setup(void)
{
  Serial.begin(115200);                                                               // Start serial communication for debugging purposes
  sensors.begin();                                                                    // Start up the library sensor DS18B20

  setup_wifi();                                                                       // Call procedure Setup_wifi()
}


//====================================================================================================================================================================//
//--------------------------------------------------------------- Function to get data from MQ135 --------------------------------------------------------------------//
//====================================================================================================================================================================//
void loop(void){ 
  sensors.requestTemperatures();                                                      // Call sensors.requestTemperatures() to issue a global temperature and Requests 
                                                                                      // to all devices on the bus
                                                                                      
  // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  float tempC = sensors.getTempCByIndex(0);                                           // collect data temperature and convertion to satuan Celcsius
  float tempF = sensors.getTempFByIndex(0);                                           // collect data temperature and convertion to satuan Fahrenheit                                    
  
  Serial.print("Celsius temperature   : ");                                           // ------- Show on monitor ----------
  Serial.println(tempC);                                                              //
  Serial.print("Fahrenheit temperature: ");                                           //
  Serial.println(tempF);                                                              // ----------------------------------
  delay(200);                                                                         // Delay 200 ms

 String tempCelcsius = String(tempC);                                                 // If data can be POST to server must be String, you need to convert 
 String tempFahrenheit = String(tempF);                                               // Float data type "tempC" & "tempF" to String "tempCelcsius" & "tempFahrenheit" 
 


//====================================================================================================================================================================//
//------------------------------------------------------------------ POST json Data from ESP8266 --------------------------------------------------------------------//
//====================================================================================================================================================================//
   //ATENTION!!! 
   //POST json data we can choose library ArduinoJson version which will use (ArduinoJson Version 5.x.x or ArduinoJson 6.x.x)
   //In my program I'am use Library ArduinoJson Version 6.x.x
   
   //Check WiFi connection status
   //String sensorName = ("DS18B20");
   //String idDataSensor = ("PRND001");

   
   if (WiFi.status() == WL_CONNECTED) {                                               // Check WiFi connection status
 
    StaticJsonDocument<300> JSONbuffer;                                               // Declaring static json document memory 'JSONbuffer' pool in-place
    JsonObject JSONencoder = JSONbuffer.createNestedObject();                         // Creates an object and assigns it to the specified key. in program
                                                                                      // key itus = 'JSONencoder'
 
    JSONencoder["id"] = "PRNDD002";                                                   // Json format = "id":"PRNDD002",
    JSONencoder["valueC"] = tempCelcsius;                                             // Json format = "valueC":(value from variable "tempCelcsius"),
    JSONencoder["valueF"] = tempFahrenheit;                                           // Json format = "id":(value from variable "tempFahrenheit")
 
    //JsonArray values = JSONencoder.createNestedArray("valueC");                     // JSON array
    //values.add(20); //Add value to array
    //values.add(21); //Add value to array
    //values.add(23); //Add value to array
 
    //JsonArray timestamps = JSONencoder.createNestedArray("timestamps");             // JSON array
    //timestamps.add("10:10"); //Add value to array
    //timestamps.add("10:20"); //Add value to array
    //timestamps.add("10:30"); //Add value to array
 
    char JSONmessageBuffer[300];                                                      // Define variable char array 'JSONmessageBuffer' with maximum nember of bytes is 300
    serializeJson(JSONencoder, JSONmessageBuffer, sizeof(JSONmessageBuffer));         // Serializes a JsonDocument to create a minified JSON document
    Serial.println(JSONmessageBuffer);                                                // Print value from varible "JSONmessageDuffer"
 
    HTTPClient http;                                                                  // Declare object of class HTTPClient
 
    http.begin("http://192.168.1.9:8080/api/payload/post");                           // Specify request destination
    http.addHeader("Content-Type", "application/json");                               // Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);                                      // Send the POST request
    String payload = http.getString();                                                // Get the response payload
  
    Serial.print("POST status: ");
    Serial.println(httpCode);                                                         // Print HTTP return code (data POST status)
    Serial.println(payload);                                                          // Print request response payload (format json)
    Serial.println(" ");                                                              // Print 
 
    http.end();                                                                       // Close connection
 
  } else {
    Serial.println("Error in WiFi connection");                                       // Print "Error in WiFi connection"
  }
 
  delay(30000);                                                                       // Send a request every 30 seconds
}
