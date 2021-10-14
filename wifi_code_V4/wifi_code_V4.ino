#include "SoftwareSerial.h"

SoftwareSerial ESP8266(2, 3);// RX, TX

//variables to be read from memory,these variables will be configured throught the website link
#define VthU 4.2
#define VthL 2.7

unsigned int UIDM;
unsigned int UIDS;
unsigned int CID;
unsigned int BID;

//variables to be uploaded on server if wifi is available else they will be stored in memory
unsigned int batteryCycle;
unsigned int packetNumber;
float V[13]={0};
float current;
float instPower;
float totalPower;
float timeStamp;
float avgSpeed;

//Server configuration
String server;
String openServerPort;
String joinAccessPoint;

void setup(){
  //verify its a punchline processor by reading the IDs from memory
  
  configureProcessor();
  configureServerStrings();
  configureESP();
}

void loop(){
  Serial.print("Starting server at http port...");
  startServerPort();
  Serial.print("         Sending data to server...");
  sendDataToServer();
  //store all the data to the memory if there is no wifi 
}
void sendDataToServer(){
  packetNumber++;
  //cmd =  "GET /live/data.php?string=asifNazeer12345 HTTP/1.1\r\n";
  String cmd =  "GET /live/data.php?string=BC"+String(batteryCycle)\
                                  +"PN"+String(packetNumber)\
                                  +"UIDM"+String(UIDM)\
                                  +"UIDS"+String(UIDS)\
                                  +"CID"+String(CID)\
                                  +"BID"+String(BID)\
                                  +"V1:"+String(V[0])\
                                  +"V2:"+String(V[1])\
                                  +"V3:"+String(V[2])\
                                  +"V4:"+String(V[3])\
                                  +"V5:"+String(V[4])\
                                  +"V6:"+String(V[5])\
                                  +"V7:"+String(V[6])\
                                  +"V8:"+String(V[7])\
                                  +"V9:"+String(V[8])\
                                  +"V10:"+String(V[9])\
                                  +"V11:"+String(V[10])\
                                  +"V12:"+String(V[11])\
                                  +"V13:"+String(V[12])\
                                  +"C"+String(current)\
                                  +"IP"+String(instPower)\
                                  +"TP"+String(totalPower)\
                                  +"T"+String(timeStamp)\
                                  +"SP"+String(avgSpeed)\
                                  +" HTTP/1.1\r\n"; 
  delay(500);
  cmd += "Host: pdpl.in\r\n\r\n";
  delay(500); 
  ESP8266.print("AT+CIPSEND=1,");
  delay(500);
  while(ESP8266.available()){ESP8266.read();}
  ESP8266.println(cmd.length()); 
  delay(2000);
  if(ESP8266.find(">")) Serial.print("OK....."); else Serial.print("Not OK......");
  while(ESP8266.available()){ESP8266.read();}
  ESP8266.println(cmd);
  delay(2000);
  if(ESP8266.find("SEND OK")) Serial.print("sent!......"); else Serial.print("Not sent!......");
  closeServer();
}
void closeServer(){
  while(ESP8266.available()){ESP8266.read();}
  ESP8266.println("AT+CIPCLOSE=1");
  delay(1000);
  if(ESP8266.find("OK")) Serial.println("Server closed!");else Serial.println("Server not Closed!");
  //else closeServer();
}
void configureServerStrings(){
  server="pdpl.in";
  openServerPort="AT+CIPSTART=1,\"TCP\",\""+String(server)+"\",80";
  //joinAccessPoint="AT+CWJAP=\"@sifNaz\",\"8 character\"";
  joinAccessPoint = "AT+CWJAP=\"JioFi3_6C0C0F\",\"Punchl1ne\"";
}
void startServerPort(){
  delay(2000);
  while(ESP8266.available()){ESP8266.read();}
  ESP8266.println(openServerPort);
  Serial.print(".");
  delay(5000);
  if(ESP8266.find("OK")) Serial.println("Started!"); else Serial.println("Not started");
  //else startServerPort();
}
void configureProcessor(){
  Serial.begin(115200);
  UIDM=1;
  UIDS=2;
  CID=1;
  BID=1;
  //variables to be uploaded on server if wifi is available else they will be stored in memory
  batteryCycle=0;
  packetNumber=0;
  current=2.2;
  instPower=1;
  totalPower=2;
  timeStamp=3;
  avgSpeed=4;
  Serial.println("Processor initialized!");
}
void configureESP(){
  ESP8266.begin(115200); //odpowiednia prędkość dla twojego modułu
  delay(2000);
  
  //if(ESP8266.find("ready")) Serial.println("ESP is ready"); else Serial.println("ESP is not ready");
  
  Serial.print("Resetting the ESP...");
  resetESP();
  
  ESP8266.println("AT+CWMODE=1");
  delay("1000");
  
  ESP8266.println("AT+CIPMUX=1");
  delay("1000");
  
  // Connecting to wifi
  Serial.print("Connecting ESP to wifi...");
  connectESPToWifi();
  Serial.println("ESP initialized");
}

void connectESPToWifi(){
  ESP8266.println(joinAccessPoint);
  delay(4000);
  Serial.print(".");
  if(ESP8266.find("OK")) Serial.println("Connected!");
  else connectESPToWifi();
}

void resetESP(){
  ESP8266.println("AT+RESTORE");
  delay(1000);
  Serial.print(".");
  if(ESP8266.find("OK")) Serial.println("Reset successful"); 
  else{
    resetESP();
  }
}
