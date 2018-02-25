// WEMOS Device
#include <ESP8266WiFi.h>

// OLED Device
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

//#if (SSD1306_LCDHEIGHT != 64)
//#error ("Height incorrect, please fix Adafruit_SSD1306.h!");
//#endif


// WiFi
//-----
char wifi_ssid[] = "NETGEAR63";
char wifi_password[] = "chummyroad556";
const char* host = "192.168.100.4";
const int   port = 8080;
const int   watchdog = 5000;
unsigned long previousMillis = millis();

// Pin Outs
//---------
const int AOUTpin=0;//the AOUT pin of the alcohol sensor goes into analog pin A0 of the arduino
const int DOUTpin=8;//the DOUT pin of the alcohol sensor goes into digital pin D8 of the arduino

int barValue;
int stateIndex;
bool stateChanged;

// Drunk State
//------------
#define LEVEL0 0
#define LEVEL1 1
#define LEVEL2 2
#define LEVEL3 3
#define LEVEL4 4


void setup() {
  Serial.begin(115200);

  //WiFi
  //----
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

  // Alchol Sensor
  //--------------
  //  pinMode(AOUTpin, INPUT);//sets the pin as an input to the arduino

  // OLED
  //-----
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  stateIndex = -1;
  stateChanged = false;
}

void loop() {

  // Take an alcohol reading and display it
  //---------------------------------------

  barValue = analogRead(AOUTpin);//reads the analaog value from the alcohol sensor's AOUT pin
  
  if (barValue < 200) {
    stateChanged = stateIndex != LEVEL0;
    stateIndex = LEVEL0;
    showMessage("Sober", stateChanged);
  } else if (barValue >= 200 && barValue < 280) {
    stateChanged = stateIndex != LEVEL1;
    stateIndex = LEVEL1;
    showMessage("Cheeky", stateChanged);
  } else if (barValue >= 280 && barValue < 350) {
    stateChanged = stateIndex != LEVEL2;
    stateIndex = LEVEL2;
    showMessage("Few Drinks", stateChanged);
  } else if (barValue >= 350 && barValue < 450) {
    stateChanged = stateIndex != LEVEL3;
    stateIndex = LEVEL3;
    showMessage("Boozed Up", stateChanged);
  } else if(barValue > 450) {
    stateChanged = stateIndex != LEVEL4;
    stateIndex = LEVEL4;
    showMessage("Flamable", stateChanged);
  }

  stateChanged = false;

  delay(1000);
}

void showMessage(char msg[], bool stateChanged) {
//  Serial.println("[debug]: stateChanged - " + String(stateChanged));
  Serial.println(barValue);
  Serial.println(msg);

  // text display tests
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(32,10);
  display.println(barValue);
  display.setCursor(32,20);
  display.println(msg);
  display.display();

  if (stateChanged) {
    tweet(msg);
  }
}

void tweet(char msg[]) {
  unsigned long currentMillis = millis();
  
  if ( currentMillis - previousMillis > watchdog ) {
    previousMillis = currentMillis;
    WiFiClient client;
 
    if (!client.connect(host, port)) {
      Serial.println("connection failed");
      return;
    }
    String url = "/tweet?bal=" + String(barValue) + "&msg=" + msg;
   
    //This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  }  
}

