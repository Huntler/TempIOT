// DHT Sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>

#include <SPIFFS.h>
#include <WiFi.h>
#include <WiFiSettings.h>
#include <time.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <history.h>
#include <gui.h>
#include <html_parse.h>

#define DHTPIN 32       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT type

using namespace std;

DHT dht(DHTPIN, DHTTYPE);

History history = History();
Gui gui = Gui();
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  SPIFFS.begin(true);

  // Sensors and display
  dht.begin();
  gui.begin();

  // Start WiFi setup
  WiFiSettings.secure = true;
  WiFiSettings.hostname = "TempIOT-";
  WiFiSettings.password = "TempIOT-Setup";
  WiFiSettings.accentColor = "#c2185b";

  // WiFi handlers
  WiFiSettings.onPortalWaitLoop = []() {
    std::vector<String> setupTitle { "Setup" };

    gui.clear();
    gui.head(setupTitle);
    gui.setup(WiFiSettings.hostname, WiFiSettings.password);
    gui.refresh();
  };

  WiFiSettings.connect();

  // Configure NTP
  configTime(3600, 3600, "pool.ntp.org");

  // Set up web interface
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html;
    web::parseDashboard(history, gui, html);
    request->send(200, "text/html", html);
  });

  server.on("/post", HTTP_POST, [] (AsyncWebServerRequest *request) {
      bool state = false;
      if (request->hasParam("display", true)) {
        Serial.print(request->getParam("display", true)->value());
        state = request->getParam("display", true)->value() == "1";
      }

      gui.setState(state);

      String html;
      web::parseDashboard(history, gui, html);
      request->send(200, "text/html", html);
    });
  
  server.begin();
}

void loop() {
  // Read from sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Add to history
  history.add(temperature, humidity);

  // Display readings
  gui.clear();

  // Display current time and IP
  String strTime;
  web::localTime(strTime);
  vector<String> head = { strTime, WiFi.localIP().toString() }; 
  gui.head(head);

  gui.graph(history);

  // Show the display buffer
  gui.refresh();
}
