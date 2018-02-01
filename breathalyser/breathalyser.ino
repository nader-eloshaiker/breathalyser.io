#include <ESP8266WiFi.h>

char wifi_ssid[] = "<SSID>";
char wifi_password[] = "<PASSWORD>";

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
  Serial.print("IP address: "); Serial.print(WiFi.localIP());
  WiFi.printDiag(Serial);

  Serial.println();
  Serial.println("Setup complete");
}

void loop() {
  // put your main code here, to run repeatedly:

}
