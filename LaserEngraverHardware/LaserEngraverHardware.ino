// #include <WebSocketsServer.h>
// #include <Hash.h>
// #include <ESP8266WebServer.h>
// #include <ESP8266WiFi.h>
// Libraries for SD card
#include "FS.h"
#include "SD.h"
#include <WebServer.h>
#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HelperClasses.cpp"
#include "qrcode.h"
#include <WebSocketsServer.h>

// #include <SD.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const unsigned char PROGMEM arrow_back[]  = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x03, 0x00, 0x06, 0x00, 0x0c, 0x00, 0x1f, 0xf8, 
	0x1f, 0xf8, 0x0c, 0x00, 0x06, 0x00, 0x03, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const unsigned char PROGMEM settings[]  = {
	0x00, 0x00, 0x03, 0xc0, 0x03, 0xc0, 0x36, 0x6c, 0x3c, 0x3c, 0x60, 0x06, 0x73, 0xce, 0x12, 0x48, 
	0x12, 0x48, 0x73, 0xce, 0x60, 0x06, 0x3c, 0x3c, 0x36, 0x6c, 0x03, 0xc0, 0x03, 0xc0, 0x00, 0x00
};


const unsigned char PROGMEM history[]  = {
	0x00, 0x00, 0x00, 0x00, 0x27, 0xe0, 0x3c, 0x30, 0x38, 0x18, 0x3d, 0x8c, 0x01, 0x84, 0x01, 0x84, 
	0x20, 0x84, 0x20, 0x44, 0x30, 0x0c, 0x18, 0x18, 0x0c, 0x30, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM folder[]  = {
	0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x7f, 0xfc, 0x41, 0xfe, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 
	0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x7f, 0xfe, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00
};


const unsigned char PROGMEM list[]  = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0xfc, 0x07, 0xfc, 0x00, 0x00, 0x27, 0xfc, 
	0x27, 0xfc, 0x00, 0x00, 0x27, 0xfc, 0x27, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const unsigned char PROGMEM hotspot[]  = {
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x18, 0x18, 0x31, 0x8c, 0x27, 0xe4, 0x6c, 0x36, 0x49, 0x92, 
	0x4b, 0xd2, 0x49, 0x92, 0x4c, 0x32, 0x20, 0x06, 0x30, 0x0c, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00
};


const unsigned char PROGMEM info[] = {
	0x00, 0x00, 0x07, 0xe0, 0x0e, 0x70, 0x18, 0x18, 0x30, 0x0c, 0x61, 0x86, 0x60, 0x06, 0x40, 0x82, 
	0x41, 0x82, 0x61, 0x86, 0x61, 0x86, 0x30, 0x0c, 0x18, 0x18, 0x0e, 0x70, 0x07, 0xe0, 0x00, 0x00
};


const unsigned char PROGMEM point[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x07, 0xe0, 0x07, 0xe0, 
	0x07, 0xe0, 0x07, 0xe0, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const unsigned char PROGMEM toggle_on[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x60, 0x06, 0x40, 0x32, 0xc0, 0x3b, 
	0xc0, 0x3b, 0x40, 0x32, 0x60, 0x06, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const unsigned char PROGMEM toggle_off[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x60, 0x06, 0x4c, 0x02, 0xdc, 0x03, 
	0xdc, 0x03, 0x4c, 0x02, 0x60, 0x06, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const unsigned char PROGMEM backspace[] = {
	0x00, 0x1e, 0x22, 0x4e, 0x4e, 0x22, 0x1e, 0x00
};

const unsigned char PROGMEM shift[] = {
	0x08, 0x1c, 0x2a, 0x49, 0x08, 0x08, 0x08, 0x08
};

const unsigned char PROGMEM dots[] = {
	0x00, 0x00, 0x00, 0xdb, 0xdb, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM keyboard_back[] = {
	0x10, 0x20, 0x40, 0xff, 0x40, 0x20, 0x10, 0x00
};

const unsigned char PROGMEM keyboard_enter[]= {
	0x00, 0x04, 0x04, 0x24, 0x44, 0xfc, 0x40, 0x20
};

const unsigned char keyboard[] = {
  0x71, 0x77, 0x65, 0x72, 0x74, 0x79, 0x75, 0x69, 0x6f, 0x70,
  0x61, 0x73, 0x64, 0x66, 0x67, 0x68,	0x6a, 0x6b, 0x6c, 0x00,
  0x00, 0x7a, 0x78, 0x63, 0x5f, 0x76, 0x62, 0x6e, 0x6d, 0x00
};

// List of taken states
// 0,3,4,5,6,7,8,9,10,11,12,13

MenuItem Items[] = {
  MenuItem(0, "FOLDERS", folder, 0),
  MenuItem(30, "HISTORY", history, 0),
  MenuItem(60, "SETTINGS", settings, 3)
};
#define ItemsNo       3

MenuItem Settings[] = {
  MenuItem(0, "BACK", arrow_back, 0),
  MenuItem(30, "HOTSPOT", hotspot, 4),
  MenuItem(60, "PRINT PARAMS", list, 3),
  MenuItem(90, "ABOUT", info, 3)
};
#define SettingsNo    4


MenuItem HotSpotSettings[] = {
  MenuItem(0, "BACK", arrow_back, 3),
  MenuItem(30, "ON", toggle_on, 5),
  MenuItem(60, "NAME", point, 6),
  MenuItem(90, "PASSWORD", point, 7),
  MenuItem(120, "QR CODE", point, 11)
};
#define HSettingsNo   5



#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

#define Menu_Width    127
#define Menu_Height   25
#define Spacing       5
#define Icon_Spacing  4
#define After_Icon_Spacing  8

#define KeyBrdXpad    4
#define KeyBrdYpad    25

#define outputA 27 // prev D8
#define outputB 26 // prev D7
#define outputC 25 // prev D6

#define cs 5
// #define miso D3
// #define mosi D4
// #define sck D5

int listCentre = 20;
int dir = 1;

int counter = 0;
int item = 0; 
int aState;
int aLastState;
uint8_t selectState = 0;  
unsigned long lastInterruptTime = 0;
static unsigned long lastPress = 0;
bool hotspotON;
char keyboard_buffer[15] = "";
int keyboard_cursor = 0;
#define Keyboard_buffer_size   15
uint8_t keyboard_after_state = 0;
uint8_t keyboard_previous_state = 0;

QRCode qrcode;
unsigned char QR_CODE[128];
char *qrImportant = "WIFI:S:\0;T:WPA;P:\0;;";
char qrText[48];
bool displayInverted = false;

char ssid[15] = "RFAID";
char *password = "12345678";
uint8_t max_connections=8;//Maximum Connection Limit for AP
int current_stations=0, new_stations=0;
WebServer server(80);   //instantiate server at port 80 (http port)
WebSocketsServer  webSocket = WebSocketsServer(81);
bool webRunning = false;

HardwareSerial grbl(2);
char grblBuffer[600];

File GCodeFile;
bool compilingGCode = false;
bool readGcode = true;

hw_timer_t *My_timer = NULL;
void IRAM_ATTR onEncoderChange(){
  aState = digitalRead(outputA);
  if (digitalRead(outputB) != aState) { 
    counter ++;
  } 
  else {
    counter --;
  }
}

// void IRAM_ATTR CountTicks();
// void IRAM_ATTR ButtonPress();
void IRAM_ATTR CountTicks(){
  unsigned long interruptTime = millis();
  if(interruptTime - lastInterruptTime > 50){
    timerAlarmEnable(My_timer); //Just Enable
  }
  lastInterruptTime = interruptTime;
}

void IRAM_ATTR ButtonPress(){
  unsigned long pressTime = millis();
  if(pressTime - lastPress > 500){
    unsigned long pressTime = millis();
    if(selectState == 10) {ProcessKeyboardInput(); lastPress = pressTime; return;}
    if(item < 0) return;
    if(selectState == 0) selectState = Items[item].NewState;
    else if(selectState == 3) selectState = Settings[item].NewState;
    else if(selectState == 4) selectState = HotSpotSettings[item].NewState;
    else if(selectState == 8) {selectState = 12; lastPress = pressTime; return;}// flip display colors

  // reset counters
  counter = 0;
  item = 0;

  // show Icon when opened
  if(selectState == 3 || selectState == 4) counter =-2;
  }
  lastPress = pressTime;
}

void setup() {
  Serial.begin(9600);
  grbl.begin(115200);
  // Wire.begin(D4, D3);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  pinMode (outputA,INPUT);
  attachInterrupt(outputA, CountTicks, CHANGE);
  pinMode (outputC,INPUT);
  attachInterrupt(outputC, ButtonPress, HIGH);
  pinMode (outputB,INPUT);
  // Reads the initial state of the outputA
  // aLastState = digitalRead(outputA);  

  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &onEncoderChange, true);
  timerAlarmWrite(My_timer, 2000, false);

  display.clearDisplay();

  server.on("/", handleMain);
  server.on("/add.png", [](){ReadSend("/add.png");});
  server.on("/feedrate.png", [](){ReadSend("/feedrate.png");});
  server.on("/cut.png", [](){ReadSend("/cut.png");});
  server.on("/description.png", [](){ReadSend("/description.png");});
  server.on("/light.png", [](){ReadSend("/light.png");});
  server.on("/menu.png", [](){ReadSend("/menu.png");});
  server.on("/photo.png", [](){ReadSend("/photo.png");});
  server.on("/arrow.png", [](){ReadSend("/arrow.png");});
  server.on("/home.png", [](){ReadSend("/home.png");});
  server.on("/action.png", [](){ReadSend("/action.png");});
  server.on("/measure.png", [](){ReadSend("/measure.png");});
  server.on("/fupload",  HTTP_POST,[](){ server.send(200);}, handleFileUpload);
  server.onNotFound(handleNotFound);
  webSocket.onEvent(webSocketEvent);

  // SPI.begin(sck, miso, mosi, cs); 
  if (!SD.begin(cs)) {
    Serial.println("Initialization failed!");
  }

  // GCodeFile = SD.open("/test.txt", FILE_WRITE);
  // if (GCodeFile) {
  //   Serial.print("Writing to test.txt...");
  //   GCodeFile.println("testing 1, 2, 3.");
  //   // close the file:
  //   GCodeFile.close();
  //   Serial.println("done.");
  // } else {
  //   Serial.println("error opening test.txt");
  // }

  // uint8_t arrr[] = {(uint8_t)82, (uint8_t)83, (uint8_t)84, (uint8_t)85, (uint8_t)86};
  // GCodeFile = SD.open("/test2.txt", FILE_WRITE);
  // if (GCodeFile) {
  //   Serial.print("Writing to test.txt...");
  //   GCodeFile.printf("%s\n", arrr);
  //   // close the file:
  //   GCodeFile.close();
  //   Serial.println("done.");
  // } else {
  //   Serial.println("error opening test.txt");
  // }
  grblBuffer[599] = '\0';
  // selectState = 5;  //debug mode
}

void handleMain() {
  // server.send_P(200, "text/html", "<html><body><p>server testes</p></body></html>"  ); 
  ReadSend("/Laser Engraver.html");
}
void handleNotFound() {
  server.send(404,   "text/html", "<html><body><p>404 Error</p></body></html>" );
}
void ReadSend(String dir)
{
  File myFile = SD.open(dir);
  size_t sent = server.streamFile(myFile, getContentType(dir));
  myFile.close();
}

File UploadFile; 
void handleFileUpload(){ // upload a new file to the Filing system
  HTTPUpload& uploadfile = server.upload(); // See https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer/srcv
                                            // For further information on 'status' structure, there are other reasons such as a failed transfer that could be used
  if(uploadfile.status == UPLOAD_FILE_START)
  {
    String filename = uploadfile.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
    Serial.print("Upload File Name: "); Serial.println(filename);
    SD.remove(filename);                         // Remove a previous version, otherwise data is appended the file again
    UploadFile = SD.open(filename, FILE_WRITE);  // Open the file for writing in SPIFFS (create it, if doesn't exist)
    filename = String();
  }
  else if (uploadfile.status == UPLOAD_FILE_WRITE)
  {
    if(UploadFile) UploadFile.write(uploadfile.buf, uploadfile.currentSize); // Write the received bytes to the file
  } 
  else if (uploadfile.status == UPLOAD_FILE_END)
  {
    if(UploadFile)          // If the file was successfully created
    {                                    
      UploadFile.close();   // Close the file again
      Serial.print("Upload Size: "); Serial.println(uploadfile.totalSize);
      webSocket.broadcastTXT("received");
    } 
    // else
    // {
    //   ReportCouldNotCreateFile("upload");
    // }
  }
}

//This functions returns a String of content type
String getContentType(String filename) {
  if (server.hasArg("download")) { // check if the parameter "download" exists
    return "application/octet-stream";
  } else if (filename.endsWith(".htm")) { //check if the string filename ends with ".htm"
    return "text/html";
  } else if (filename.endsWith(".html")) {
    return "text/html";
  } else if (filename.endsWith(".css")) {
    return "text/css";
  } else if (filename.endsWith(".js")) {
    return "application/javascript";
  } else if (filename.endsWith(".png")) {
    return "image/png";
  } else if (filename.endsWith(".gif")) {
    return "image/gif";
  } else if (filename.endsWith(".jpg")) {
    return "image/jpeg";
  } else if (filename.endsWith(".ico")) {
    return "image/x-icon";
  } else if (filename.endsWith(".xml")) {
    return "text/xml";
  } else if (filename.endsWith(".pdf")) {
    return "application/x-pdf";
  } else if (filename.endsWith(".zip")) {
    return "application/x-zip";
  } else if (filename.endsWith(".gz")) {
    return "application/x-gzip";
  }
  return "text/plain";
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // send message to client
        webSocket.sendTXT(num, "0");
      }
      break;

    case WStype_TEXT:
      if(payload[0] == 'y'){
        compilingGCode = false;
        GCodeFile.close();
        Serial.println("Closing file");
        webSocket.broadcastTXT("received");
      }
      else if(compilingGCode && GCodeFile){
        GCodeFile.printf("%s\n", payload);
        Serial.printf("[%u] Gcode: %s\n", num, payload);
      }
      
      else if(payload[0] == 'z'){
        Serial.println("Creating file");
        GCodeFile = SD.open("/GCode1.txt", FILE_WRITE);
        compilingGCode = true;
      }

      else if(payload[0] == '1'){
        grbl.write(24);
      }

      else if(payload[0] == '$' || payload[0] == 'S' || payload[0] == 'G'){
        grbl.printf("%s\n", payload);
        Serial.printf("[%u] get Text: %s\n", num, payload);
      }

      else if(payload[0] == '2'){
        Serial.println("2 pressed");
        
        GCodeFile = SD.open("/GCode1.txt");
        // Engrave();
        // if (GCodeFile) {
        //   String str = GCodeFile.readStringUntil('\n');
        //   Serial.println(str);
        //   grbl.println(str);
        // }
        selectState = 13;
      }
      
      else{
        Serial.printf("[%u] get Text: %s\n", num, payload);
        grbl.printf("%s\n", payload);
      }

      // send message to client
      // webSocket.sendTXT(num, "message here");
      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
      
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      // hexdump(payload, length);
      // send message to client
      // webSocket.sendBIN(num, payload, length);
      break;
  }

}


void loop() {
  if(webRunning){
    ControlLoop();
  }

  switch(selectState){
    case 0:
      RenderMenu();
      break;
    case 3:
      RenderSettings();
      break;
    case 4:
      RenderHotspotSettings();
      break;
    case 5:   // turn on hotspot
      TurnOnHotspot();
      GenerateQRCode();
      selectState = 8;
      break;
    case 6:  // change hotspot name
      keyboard_after_state = 9;
      keyboard_previous_state = 4;
      selectState = 10;
      break;
    case 8:
      HotspotInfo();
      // ControlLoop();
      break;
    case 9:
      ChangeHotspotName();
      selectState = 4; // return to hotspot settings
    case 10:
      RenderKeyboard();
      break;
    case 11:
      OnQR_CODE_Clicked();
      break;
    case 12:
      ProcessHotspotInfoButtonPress();
      break;
    case 13:
      Engrave();
      break;
  }
}

void Engrave(){
  if (GCodeFile && readGcode) {
    String str = GCodeFile.readStringUntil('\n');
    Serial.println(str);
    grbl.println(str);
    readGcode = false;

    // close the file:
    if(GCodeFile.position() >= GCodeFile.size() - 1){
      GCodeFile.close();
      readGcode = true;
    }
  }
}

void ControlLoop(){
  webSocket.loop();
  server.handleClient();
  HandleGRBL();
}

void HandleGRBL(){
  //Check to see if anything is available in the serial receive buffer
  //  while (grbl.available() > 0){
  //    static unsigned int message_pos = 0;
  //    char inByte = Serial.read();
  //    if ( inByte != '\n' && (message_pos < 99) )
  //    {
  //      grblBuffer[message_pos] = inByte;
  //      message_pos++;
  //    }
  //    //Full message received...
  //    else
  //    {
  //      //Add null character to string
  //      grblBuffer[message_pos] = '\0';
  //      //Print the message (or do other things)
  //      webSocket.broadcastTXT(grblBuffer);
  //      //Reset for the next message
  //      message_pos = 0;
  //    }
  //  }
  if(grbl.available() > 0){
    grbl.readBytes(grblBuffer, 100);
    if (grblBuffer[0] == 'o' && grblBuffer[1] == 'k'){
      // GCodeFile.seek(GCodeFile.position()+1);
      Serial.println("ok found");
      readGcode = true;
    }
    Serial.println(grblBuffer);
    webSocket.broadcastTXT(grblBuffer);
  }
}

void RenderMenu(){
  // control scrolling behaviour
  RegularScrollUpdate(ItemsNo, 0);

  display.clearDisplay();
  for(int i = 0; i < ItemsNo; i++){
    if(i == item){
      DrawMenuItem(32 + (Items[i].Pos - listCentre), Items[i].Text, Items[i].bitMap, true);
    }
    else{
      DrawMenuItem(32 + (Items[i].Pos - listCentre), Items[i].Text, Items[i].bitMap);
    }
  }
  display.display();
}

void RenderSettings(){
  // control scrolling behaviour
  RegularScrollUpdate(SettingsNo, -1);

  display.clearDisplay();

  display.drawBitmap((128 - LOGO_WIDTH)/2, 32 + (-30 - listCentre) - (LOGO_HEIGHT/2), settings, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);

  for(int i = 0; i < SettingsNo; i++){
    if(i == item){
      DrawMenuItem(32 + (Settings[i].Pos - listCentre), Settings[i].Text, Settings[i].bitMap, true);
    }
    else{
      DrawMenuItem(32 + (Settings[i].Pos - listCentre), Settings[i].Text, Settings[i].bitMap);
    }
  }
  display.display();
}

void RenderHotspotSettings(){
  // control scrolling behaviour
  RegularScrollUpdate(HSettingsNo, -1);

  display.clearDisplay();

  display.drawBitmap((128 - LOGO_WIDTH)/2, 32 + (-30 - listCentre) - (LOGO_HEIGHT/2), hotspot, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);

  for(int i = 0; i < HSettingsNo; i++){
    if(i == item){
      DrawMenuItem(32 + (HotSpotSettings[i].Pos - listCentre), HotSpotSettings[i].Text, HotSpotSettings[i].bitMap, true);
    }
    else{
      DrawMenuItem(32 + (HotSpotSettings[i].Pos - listCentre), HotSpotSettings[i].Text, HotSpotSettings[i].bitMap);
    }
  }
  display.display();
}

void ChangeHotspotName(){
  for(int i = 0; i < 15; i++){
    if(i < keyboard_cursor) ssid[i] = keyboard_buffer[i];
    else ssid[i] = '\0';
  }
  GenerateQRCode();
  //clear buffer
  keyboard_cursor = 0;
}

// state 10
void RenderKeyboard(){
  if(counter > (29 * 2)+1) counter = -4;
  if(counter < -4 - 1) counter = 29 * 2;
  item = (counter/2);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int countx = 0;
  int county = 0;

  int index = 0;

  for(int j = 0; j < 3; j++){
    for(int i = 0; i < 10; i++){
      index = (j*10)+i;
      if(index == 19) DrawKeyboardIcon( KeyBrdXpad + (i * 12), KeyBrdYpad +(j * 14), backspace, index, item);
      else if(index == 20) DrawKeyboardIcon( KeyBrdXpad + (i * 12), KeyBrdYpad +(j * 14), shift, index, item);
      else if(index == 29) DrawKeyboardIcon( KeyBrdXpad + (i * 12), KeyBrdYpad +(j * 14), dots, index, item);
      else{
        DrawKeyboardChar(KeyBrdXpad + (i * 12), KeyBrdYpad +(j * 14), keyboard[index], index, item);
      }
    }
  }

  // draw back and enter buttons
  DrawKeyboardIcon( KeyBrdXpad + (0 * 12), KeyBrdYpad +(-1 * 14), keyboard_back, -2, item);
  DrawKeyboardIcon( KeyBrdXpad + (9 * 12), KeyBrdYpad +(-1 * 14), keyboard_enter, -1, item);

  // draw text screen
  display.fillRect(KeyBrdXpad + (1 * 12), KeyBrdYpad +(-1 * 14), (8 * 12)-2, 8, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(KeyBrdXpad + (1 * 12)+2, KeyBrdYpad +(-1 * 14));
  for(int i = 0; i < keyboard_cursor; i++){
    display.write(keyboard_buffer[i]);
  }

  display.display();
}

void DrawKeyboardChar(int xpos, int ypos, char character, int index, int selectedItem){
  if(selectedItem == index){
    display.fillRect(xpos-2, ypos, 8, 8, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(xpos, ypos);
    display.write(character);
  }
  else{
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(xpos, ypos);
    display.write(character);
  }
}

void DrawKeyboardIcon(int xpos, int ypos, const uint8_t *bitmap, int index, int selectedItem){
  if(selectedItem == index){
    display.fillRect(xpos, ypos, 8, 8, SSD1306_WHITE);
    display.drawBitmap( xpos, ypos, bitmap, 8, 8, SSD1306_BLACK);
  }
  else{
    display.drawBitmap( xpos, ypos, bitmap, 8, 8, SSD1306_WHITE);
  }
}

void ProcessKeyboardInput(){
  // back pressed
  if(item == -2){
    counter = -2;   // set to 4 to let cursor be on name after going back to hotspot setting
    selectState = keyboard_previous_state;   // return to previous setting
  }
  // enter pressed
  else if(item == -1){
    counter = -2;   // set to 4 to let cursor be on name after going back to hotspot setting
    selectState = keyboard_after_state;
  }
  // backspace pressed
  else if(item == 19){
    keyboard_cursor--;
  }
  // // shift pressed
  // else if(item == 20){

  // }
  // // numbers pressed
  // else if(item == 29){

  // }
  else{
    if(keyboard_cursor < Keyboard_buffer_size){
      keyboard_buffer[keyboard_cursor] = keyboard[item];
      keyboard_cursor++;
    }
  }
}

void TurnOnHotspot(){
  if(WiFi.softAP(ssid,password,1,false,max_connections)==true)
  {
    Serial.print("Access Point is Creadted with SSID: ");
    Serial.println(ssid);
    Serial.print("Max Connections Allowed: ");
    Serial.println(max_connections);
    Serial.print("Access Point IP: ");
    Serial.println(WiFi.softAPIP());
    server.begin();
    webSocket.begin();
    webRunning = true;
  }
  else
  {
    Serial.println("Unable to Create Access Point");
  }
}

void HotspotInfo(){
  RegularScrollUpdate(5, 0);
  display.clearDisplay();
  if(item == 0) DrawMenuItem(32 + (Settings[0].Pos - listCentre), Settings[0].Text, Settings[0].bitMap, true); // render back button
  else DrawMenuItem(32 + (Settings[0].Pos - listCentre), Settings[0].Text, Settings[0].bitMap);

  display.setCursor(0, 32 + 30 - listCentre);
  display.setTextColor(SSD1306_WHITE);  
  display.print("SSID: ");
  display.println(ssid);
  display.print("Max Connections: ");
  display.println(max_connections);
  display.print("IP: ");
  display.println(WiFi.softAPIP());
  display.print("Connected: ");
  display.println(current_stations);

  new_stations=WiFi.softAPgetStationNum();
   
  if(current_stations<new_stations)//Device is Connected
  {
    current_stations=new_stations;
    Serial.print("New Device Connected to SoftAP... Total Connections: ");
    Serial.println(current_stations);
  }
   
  if(current_stations>new_stations)//Device is Disconnected
  {
    current_stations=new_stations;
    Serial.print("Device disconnected from SoftAP... Total Connections: ");
    Serial.println(current_stations);
  }

  DisplayQRCode(35, 32 + 83 - listCentre);

  display.display();
}

void ProcessHotspotInfoButtonPress(){
  if (item == 0) {
    if(displayInverted){
      display.invertDisplay(false);
      displayInverted = false;
    }
    selectState = 4;
    counter = -2;
  }
  else{
    display.invertDisplay(!displayInverted);
    displayInverted = !displayInverted;
    selectState = 8;
  }
}


void GenerateQRCode(){
  int copied = 0;
  copied = CopyCharUntilStringTerminator(qrText, 0, qrImportant, 0);
  copied += CopyCharUntilStringTerminator(qrText, copied, ssid, 0);
  copied += CopyCharUntilStringTerminator(qrText, copied, qrImportant, 8);
  copied += CopyCharUntilStringTerminator(qrText, copied, password, 0);
  copied += CopyCharUntilStringTerminator(qrText, copied, qrImportant, 18);
  qrText[copied] = '\0';
  Serial.print("qr text: ");
  Serial.println(qrText);

  // clear QR CODE
  for(int i = 0; i < 128; i++){
    QR_CODE[i] = 0;
  }

  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, 0, qrText);

  Serial.print("QR size: ");
  Serial.println(qrcode.size);

  for (uint8_t y = 0; y < qrcode.size; y++) {
    // Each horizontal module
    for (uint8_t x = 0; x < qrcode.size; x++) {
        // Print each module (UTF-8 \u2588 is a solid block)
        if(qrcode_getModule(&qrcode, x, y)){ 
          QR_CODE[(y*4)+(x/8)] += pow(2, 7-(x%8));
        }
    }
  }
}

void DisplayQRCode(int xpos, int ypos){
  // display.clearDisplay();
  // display QR_Code
  for(int y = 0; y < 32; y++){
    for(int x = 0; x < 32; x++){
      if(bitRead(QR_CODE[(y*4)+(x/8)], 7 - (x % 8)) != 0){
        display.drawPixel((x * 2) + xpos, (y * 2) + ypos, SSD1306_WHITE);
        display.drawPixel((x * 2) + xpos, (y * 2) + 1 + ypos, SSD1306_WHITE);
        display.drawPixel((x * 2) + 1 + xpos, (y * 2) + ypos, SSD1306_WHITE);
        display.drawPixel((x * 2) + 1 + xpos, (y * 2) + 1 + ypos, SSD1306_WHITE);
      }
    }
  }
  // display.display();
}

void OnQR_CODE_Clicked(){
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);     
  display.println("Please turn on hotspot first");
  display.display();
}


void DrawMenuItem(int pos, String text, const uint8_t *bitmap){
  display.drawRect(0, pos - (Menu_Height/2), Menu_Width + 1, Menu_Height, SSD1306_WHITE);

  display.drawBitmap( Icon_Spacing, pos - (LOGO_HEIGHT/2), bitmap, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(Icon_Spacing + LOGO_WIDTH + After_Icon_Spacing, pos - (8/2));             // Start at top-left corner
  display.println(text);
}

void DrawMenuItem(int pos, String text, const uint8_t *bitmap, bool selected){
  display.fillRect(0, pos - (Menu_Height/2), Menu_Width + 1, Menu_Height, SSD1306_WHITE);

  display.drawBitmap( Icon_Spacing, pos - (LOGO_HEIGHT/2), bitmap, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_BLACK);

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_BLACK);        // Draw white text
  display.setCursor(Icon_Spacing + LOGO_WIDTH + After_Icon_Spacing, pos - (8/2));             // Start at top-left corner
  display.println(text);
}

float Lerp(float a, float b, float t){
  return a + ((b-a)*t);
}


//............utilities..........................................................................................
void RegularScrollUpdate(int itemCount, int negativeEnd){
  item = (counter/2)%itemCount;
  listCentre = Lerp(listCentre, item * 30, 0.1);

  //loop the counter on the negative
  if(counter < negativeEnd * 2){
    counter = (itemCount - 1)*2;
  }
}

void CopyCharFromArrayB(char A[], int startA, char B[], int startB, int length){
  for(int i = 0; i < length; i++){
    A[startA + i] = B[startB + i];
  }
}

int CopyCharUntilStringTerminator(char A[], int startA, char B[], int startB){
  int i = 0;
  while(B[startB + i] != '\0'){
    A[startA + i] = B[startB + i];
    i++;
  }
  return i;
}
