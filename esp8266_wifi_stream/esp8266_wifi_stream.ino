#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <ESP_Google_Sheet_Client.h>

#define PROJECT_ID "milltronic"
#define CLIENT_EMAIL "esp-planilhas-google@milltronic.iam.gserviceaccount.com"
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCnEsTZ8N4Khtup\n4Itv3hbgmGg3wsIyYqdKT5R75pdVYUQUOb4BlzSSOloZL80ljy3yvos0HzH2s5J+\nIM0gX+FJQ8qP3PkQKDvB7YDQFR6rtOKFRpHvnhPES0efXIvcwDVqsiO4WxYdf8du\nC/4Be1At5PUd5W4WEN1jP1AwKi0i1YE52y45yl6e9yIzzKU53rx3y9I3zSrGoph0\n2owcj2GdJCMAhjUxKP3aScZtgq3ibLN7gjsDrgVNFZKQ74p8o5MPSXGS70jLv2x/\nHMirUqulVx2UvRsBnWcnUIVDdWa10qwW/BVs4GYiatfufDjKpU8kxjMaP+4+vkk6\n5NASl6ddAgMBAAECggEADBp6O86tC0avSax8dX95mU45OJbpoiRO+b1vpJU1L+iC\nudNgDjXxx0kmZ0409qxbeAsxQvailDx4l54RNi/z/m6EkW0A9xhvEdK+ovRwuyA4\ny85rupfxVyZb+zHFaRnclMzwdQXWQY9U94YzYdCY46ZDUqXhFoSkwTCgQsj/aZc/\ngXFWPOeU2pakRDEZ0ovTjrvO3I/XaLxQ0k9hikoOTnbA3n0Pqia5dRnsjG+nLxZb\ncVxM88Sda/N6VnL+32p4XAKVOVSTvPu8BzSZAVy1+MTpKeCXa0qwAoq70bnr3Ydx\nvRtOVB5OTeqY/hzE3JUVPXZhqJFN/bRZ/91y+Nvu2QKBgQDeKnQrkRRVs4H9fECv\nuWaf2G5YX752/BioqyTOWowm8OFxQJ0ne8WUSTWpNMXYMyC0GNILJs4bsvscbfRb\nrQvKFoB6estC/7NOzAWq5iYAX3mt8aqMdMvgzswg2rQmPz9r+0j57FzQyhe7OUJJ\n2crfTpAwjQFJT2mYJs5CINES9QKBgQDAhG5tnEBynw2EOAFlhtnIZVPD4i75pAcR\nrvtEGsKdMpmD1kOaub+VTjJlUhG9O4A3v1ZMaO3QT07Q426AP7ArS+f/SAhbm6De\nMr1hr1oh/jKiPTeA0fcoigC59LSaQbCOHVeAGChT1VgXWFIvdubt0azUjAiSkxTr\nhDDuJgeRyQKBgAWcPtjGziOn59V16NBHN088EPVMXav1i83enIxVfwqQtLC+d/mC\nR6gJ/T/rpD9kcyP5KHy9myDZ8Ttzj2MtSInu7YmqRi3H9Ug9ILuDGlySDKmW2762\nSFvB/SerPjdlC333l40wKdvf7p98v67LwDLJchy3wtuOFNWRzU2gNIdtAoGBAKAG\ndPRPyZJ9e3LISwDlkunssDBHNtzz55xfYyU9ST/563mXBSL7+7BUC7gE/BHb/7xX\nI5joL6XUBqPEKPoZ9opG0lV0vt3HA3UvoBQu+jD/Z39Me3JtFZFxBSGNswH2UJgQ\n88baTctBr7PYCpjBsTtF6u/SZt3PvLf4bePHYMthAoGAaWXSPO5Vf+q5Y8Ebdbzi\nybXXDG8gCRhzGifnDFZ7bAdQ5fTNrgw7L2BXhyZz+i6bRaEiXo39ei1kGJO/P4sI\ncaZiLuT6Uso1N8TCM8R8DIylGWb3GYZHBODm2usVy6oV7kXWCeAG7WNU9tOU8SQf\nXS8sq3t5q6EX6WKU7Jcn9WQ=\n-----END PRIVATE KEY-----\n";
const char SPREADSHEET_ID[] PROGMEM = "1AMiAq1DkiQGqhK0I29rX9Kg8Z9quhrj3iUAKV7SVkuE";
void tokenStatusCallback(TokenInfo info); // Callback para debug

const char* ssid = "LYKENET-Celia ";   // your network SSID (name)
const char* password = "19022000";   // your network password

WiFiClient  client;

int aux = 0;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

int positionX = 20;
int positionY = 45;

void setup() {
  //Inicializar serial
  Serial.begin(9600);

  //Inicializar WiFi
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //Inicializar Google Sheets
  Serial.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);
  GSheet.setTokenCallback(tokenStatusCallback); //Set the callback for Google API access token generation status (for debug only)
  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY); //Begin the access token generation for Google API authentication

  // Inicializar I2C
  Wire.begin(0x0A);
  Wire.onReceive(receiveEvent);
}

void loop() {
  bool ready = GSheet.ready();
  static unsigned long ms;
  static unsigned long interval = 11000;



  if ( ready && ((millis() > ms))) {
    ms = millis() + interval;
    FirebaseJson response;
    FirebaseJson valueRange;
    valueRange.add("majorDimension", "COLUMNS");
    valueRange.set("values/[0]/[0]", positionX);
    valueRange.set("values/[1]/[0]", positionY);

    if (aux == 0) {
      GSheet.values.clear(&response, SPREADSHEET_ID, "Log1!A:A"); // Limpar coluna A
      GSheet.values.clear(&response, SPREADSHEET_ID, "Log1!B:B"); // Limpar coluna A
      aux = 1;
      Serial.println("\nPlanilha limpa.");  
    }

    bool success = GSheet.values.append(&response, SPREADSHEET_ID, "Log1!A1", &valueRange);
    response.toString(Serial, true);
    Serial.println("\nInserindo valores na planilha...");
    success = GSheet.values.get(&response, SPREADSHEET_ID, "Log1!B3");
    response.toString(Serial, true);
    FirebaseJsonData result;
    response.get(result, "values/[0]/[0]");
    positionX++;
    positionY++;
  }
}

void tokenStatusCallback(TokenInfo info)
{
  if (info.status == esp_signer_token_status_error)
  {
    Serial.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    Serial.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  }
  else
  {
    Serial.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
  }
}

void receiveEvent(int numBytes) {
  positionX = Wire.read();
  Serial.print("X: ");
  Serial.println(positionX);

  positionY = Wire.read();
  Serial.print("Y: ");
  Serial.println(positionY);
}
