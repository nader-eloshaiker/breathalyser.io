#include <ESP8266WiFi.h>

<<<<<<< HEAD
char wifi_ssid[] = "NETGEAR63";
char wifi_password[] = "chummyroad556";
=======
#include <ESP8266WiFi.h>

char wifi_ssid[] = "NETGEAR63";
char wifi_password[] = "chummyroad556";


const int AOUTpin=0;//the AOUT pin of the alcohol sensor goes into analog pin A0 of the arduino
const int DOUTpin=8;//the DOUT pin of the alcohol sensor goes into digital pin D8 of the arduino
const int ledPin=13;//the anode of the LED connects to digital pin D13 of the arduino

int barValue;
>>>>>>> 4ac27fb209a636ca486ba48b02f4ac0c9ba37c2f

void setup() {
  Serial.begin(115200);

  WiFi.begin(wifi_ssid, wifi_password);
  delay(2000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
         
  Serial.print("WiFi connected to "); Serial.println(wifi_ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
  WiFi.printDiag(Serial);

  Serial.println();
  Serial.println("Setup complete");

  pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino
  pinMode(ledPin, OUTPUT);//sets the pin as an output of the arduino
}

void loop() {
  barValue = analogRead(AOUTpin);//reads the analaog value from the alcohol sensor's AOUT pin
  Serial.println(barValue);
 
  if (barValue < 200) {
      Serial.println("You are sober.");
  } else if (barValue >= 200 && barValue < 280) {
      Serial.println("Alcohol detected");
  } else if (barValue >= 280 && barValue < 350) {
      Serial.println("More than one drink going on here....");
  } else if (barValue >= 350 && barValue < 450) {
      Serial.println("Serious Booze here! ");
  } else if(barValue > 450) {
     Serial.println("You are drunk as fart!");
  }

}
