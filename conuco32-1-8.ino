// A partir de esta versión se precisa PCB v4.

#define CONUCO           // usado para pinout PCB
#define PCBV5            // versión de PCB
#define versinst 2053    // versión Firmware 
boolean INITFAB=false;     // si true, se resetea a fábrica, si false no se hace nada
boolean MARKTIME=false;

#include "commontexts.h"              
#include <IoTtweetESP32.h>            // Local
#include "defines.h"                  // include
#include "variables.h"                // include 

#include <math.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Update.h>
#include <ESP8266FtpServer.h>
#include "Time.h"                     // Local 
#include "TimeLib.h"                  // Local
#include "FS.h"
#include "SPIFFS.h"
#include <HTTPClient.h>
#include "RCSwitch.h"                 // Local
#include "OneWire.h"                  // Local
#include "DallasTemperature.h"        // Local
#include "DHTesp.h"                   // Local
#include <Adafruit_BMP085.h>          // Local
#include "Free_Fonts.h"               // Include the header file attached to this sketch
#include <TFT_eSPI.h>                 // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include "Adafruit_GFX.h"
#include <ESP32httpUpdate.h>          // Local
#include "LiquidCrystal_I2C.h"        // Local
#include <PubSubClient.h>
#include <ModbusMaster.h>             // Local
#include <WiFiUdp.h>
#include <NTPClient.h>                // Local
#include <RemoteDebug.h>
//#include <uMQTTBroker.h>

//#include <ResponsiveAnalogRead.h>
///////////////////////////////
#include <Thermistor.h>
#include <NTC_Thermistor.h>
//#define SENSOR_PIN             A1
//#define SENSOR_PIN             ADC0
//#define REFERENCE_RESISTANCE   8000
//#define NOMINAL_RESISTANCE     100000
//#define NOMINAL_TEMPERATURE    25
//#define B_VALUE                3950
Thermistor *thermistor;
//////////////////////////////////////////
//#include <AccelStepper.h>

FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial
WebServer server(conf.webPort);
IoTtweetESP32 myiot;       //naming your devices
RCSwitch mySwitch = RCSwitch();
DHTesp dht[maxDHT];
Adafruit_BMP085 bmp085;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Display LCD 2x16 azul
WiFiClient espClient;
PubSubClient PSclient(espClient);
ModbusMaster MBnode;
WiFiUDP ntpUDP;
//ResponsiveAnalogRead analog0(4, true);
//ResponsiveAnalogRead analog1(27, true);
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org");
OneWire owire(W0);
DallasTemperature sensors0(&owire);
TFT_eSPI tft=TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

TFT_eSPI_Button btSD[maxSD];        // botones salidas digitale
TFT_eSPI_Button btED[maxED];        // botones entradas digitales
TFT_eSPI_Button btTE[maxTemp];      // botones temperaturas
TFT_eSPI_Button btIP[4];            // botones pantalla IP
TFT_eSPI_Button btGPIO[maxgpiovar]; // botones señales GPIOs
TFT_eSPI_Button btST[4];            // botones barra estado
TFT_eSPI_Button btSET[4];           // botones acciones: Reset, Setup,WiFi,xxxx
TFT_eSPI_Button btKEY[10];          // botones números
TFT_eSPI_Button btBC[16];           // botones bomba de calor
TFT_eSPI_Button btDEM[3];        // botones demanda para prueba
RemoteDebug Debug;

#include "basicfunctions.h"            // include
#include "ajaxcode.h"                  // include
#include "htmlFunctions.h"             // include
#include "tftfunctions.h"              // include
#include "conuco32.h"                  // include
#include "jsonfunctions.h"             // include
#include "main.h"                      // include
#include "bombacalor.h"                // include

void initSPIFSS(boolean testfiles, boolean format) {
	if(SPIFFS.begin(format)) { Serial.println("SPIFSS OK"); }	else  { Serial.println("SPIFSS ERROR"); }
	if(testfiles) {
		File dir=SPIFFS.open(barra);
		File file=dir.openNextFile();
		while(file) { Serial.print(file.name()); Serial.print(b); Serial.println(file.size()); file=dir.openNextFile(); }
	}
}

void initDS18B20() {
	sensors0.begin();
  conf.prectemp=12;
	sensors0.setResolution(conf.prectemp);
	nTemp=sensors0.getDeviceCount();
	if(nTemp>maxTemp) { nTemp=maxTemp; }
//  conf.lang=0;
  Serial.print("D conf.lang:"); Serial.println(conf.lang);
	dPrint(t(sondastemp));	dPrint(dp);
	dPrintI(nTemp);	dPrint(crlf);	
	dPrint(b);	dPrint(t(Modo));	dPrint(dp);
	dPrint((sensors0.isParasitePowerMode())?c(tparasite):c(tpower));	dPrint(crlf);
  for(byte i=0; i<maxTemp; i++)       {   // busca sondas conectadas
    if (sensors0.getAddress(addr1Wire[i], i))    {
      dPrint(b);
      for(uint8_t j=0; j<8; j++) { if(addr1Wire[i][j]<16) { dPrint(cero); } Serial.print(addr1Wire[i][j], HEX); }
      dPrint(crlf);
      }
  }
  leevaloresOW();
}

void initEntDig() { for(byte i=0; i<maxED; i++) { pinMode(edPin[i], INPUT_PULLUP); }  }

void initSalDig() {
	for(byte i=0; i<maxSD; i++) {
		pinMode(sdPin[i],OUTPUT);
		digitalWrite(sdPin[i],valorpin[conf.valinic[i]==2?getbit8(conf.MbC8,i):conf.valinic[i]]);
	}
}

void initbmp085() {
  if (conf.I2Cenabled==1)
    {
    if(bmp085.begin()) {bmp085enabled=true; Serial.println(c(BMP085OK));}
    else { Serial.print(b);  Serial.println(c(BMP085notfound));  }
    }
}

void initGpios() {
	for(byte i=0; i<maxgpiovar; i++) {
		if(gpiovis(i)) 
		  {
      if(conf.gpiosensortype[i]==0)      { pinMode(listgpiovar[i], INPUT_PULLUP); } // input
			else if(conf.gpiosensortype[i]==1) { pinMode(listgpiovar[i], OUTPUT); digitalWrite(listgpiovar[i],0);}       // output
			else if(conf.gpiosensortype[i]==2) { pinMode(listgpiovar[i], INPUT); }        // ADC
			else if(conf.gpiosensortype[i]==3) { pinMode(listgpiovar[i], OUTPUT);  digitalWrite(listgpiovar[i],0);}       // DAC
      else if(conf.gpiosensortype[i]==4) { pinMode(listgpiovar[i], INPUT); dht[i].setup(listgpiovar[i], DHTesp::DHT11); }  // DHT
      else if(conf.gpiosensortype[i]==5) { pinMode(listgpiovar[i], INPUT); }        // PT1000
      else if(conf.gpiosensortype[i]==6) { pinMode(listgpiovar[i], INPUT); }        // NTC
      else if(conf.gpiosensortype[i]==7) { pinMode(listgpiovar[i], INPUT); }        // ACS712
			else { pinMode(listgpiovar[i], OUTPUT);  digitalWrite(listgpiovar[i],0); }   // valor defecto
		  }
	}
}

void initWiFi() {
	// Init WiFi
	if(conf.wifimode>2) { conf.wifimode=2; }
	if(conf.wifimode==0) { WiFi.mode(WIFI_STA); }
	else if(conf.wifimode==1) { WiFi.mode(WIFI_AP); }
	else if(conf.wifimode==2) { WiFi.mode(WIFI_AP_STA); }

	WiFi.setAutoConnect(true);
	WiFi.setAutoReconnect(true);
	dPrint(t(wifimodet));	dPrint(dp);	dPrintI(conf.wifimode);	dPrint(crlf);
	dPrint(c(MAC));	dPrint(dp);	dPrint(WiFi.softAPmacAddress());	dPrint(crlf);
	for(byte i=0; i<6; i++) {
		WiFi.softAPmacAddress().substring(i*3,i*3+2).toCharArray(conf.EEmac[i], 3);
		strcat(mac, conf.EEmac[i]);
	}

	if((conf.wifimode==1) || (conf.wifimode==2) || (conf.wifimode==12)) { // AP o AP+STA
		WiFi.channel(conf.canalAP);
		WiFi.softAP(conf.ssidAP, conf.passAP, conf.canalAP, false);
		dPrint(t(canal));	dPrint(dp);	dPrintI(WiFi.channel());	dPrint(crlf);
		dPrint(c(tIP));	dPrint(dp);	Serial.print(WiFi.softAPIP());	dPrint(crlf);
    dPrint(c(tport)); dPrint(dp); Serial.print(conf.webPort); dPrint(crlf);
	}
	if((conf.wifimode==0) || (conf.wifimode==2) || (conf.wifimode==12)) { // STA o AP+STA
		dPrint(t(staticip)); dPrint(dp);	dPrint(conf.staticIP?t(SI):t(NO)); dPrint(coma);
		if(conf.staticIP == 1) {
			WiFi.config(conf.EEip, conf.EEgw, conf.EEmask, conf.EEdns, conf.EEdns2);
		  }
		for(byte i=0; i<4; i++) {Serial.print(conf.EEdns[i]); Serial.print(".");}
		dPrint(crlf);
		WiFi.begin(conf.ssidSTA, conf.passSTA, true);
		byte cont = 0;
		dPrint(t(conectando)); dPrint(b);
		dPrint(conf.ssidSTA);	dPrint(barra);
		dPrint(conf.passSTA);	dPrint(b);
		while((!WiFi.isConnected()) && (cont++ < 20))  { delay(500); dPrint(punto); }
    marktime("  conectando WiFi");
		dPrint(crlf);
		dPrint(t(tconectado));	dPrint(b);	dPrint(WiFi.isConnected()?ok:c(terror));	dPrint(crlf);
		dPrint(c(tIP));	dPrint(dp);	Serial.print(WiFi.localIP());	dPrint(crlf);
		dPrint(c(tport));	dPrint(dp);	Serial.print(conf.webPort);	dPrint(crlf);
		Serial.print("ESP Mac Address: ");	Serial.println(WiFi.macAddress());
		Serial.print("Subnet Mask: ");	Serial.println(WiFi.subnetMask());
		Serial.print("Gateway IP: ");	Serial.println(WiFi.gatewayIP());
		Serial.print("DNS: ");	Serial.println(WiFi.dnsIP());
	}
}

void initTime() {
	///// Time
	timeClient.begin();
  timeClient.setTimeOffset(3600);
  if(timeClient.update()==1)
 	  {countfaulttime=0; setTime(timeClient.getEpochTime()); }
	else {
	  Serial.print("timeclient.update:");		Serial.println(timeClient.update());
		}
}

void initFTP() {ftpSrv.begin(admin,admin);  }  //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
void initSerial() {	Serial.begin(115200); }
void initWebserver() { server.begin();  Serial.println("HTTP server started"); }
void initLCD() {  if(conf.I2Cenabled==1) { lcd.init();  lcd.backlight(); lcdshowstatus(); } }

void init433() {
	if(conf.RX433enabled==1) {
		pinMode(RX433,INPUT_PULLUP);
		//mySwitch.enableReceive(RX433);  // Receiver on interrupt 32 => that is pin #32,
	  }
}

void leerConf() { 
	if(readconf()<sizeof(conf)) { saveconf(); }
	if((conf.netseg==0) || (conf.netseg==255)) { conf.netseg=1; }       // provisional
}

void initIFTTT() {
  strcpy(auxdesc,itoa(WiFi.localIP()[0],buff,10));
	for(byte i=1; i<=3; i++) { strcat(auxdesc,"."); strcat(auxdesc,itoa(WiFi.localIP()[i],buff,10)); }
	ifttttrigger(conucochar, conf.iftttkey, conf.aliasdevice, auxdesc, conf.myippub);
}

void initDebug()        //  REMOTE DEBUG
{
    String hostNameWifi = HOST_NAME;
    hostNameWifi.concat(".local");
#ifdef USE_MDNS  // Use the MDNS ?
    if (MDNS.begin(HOST_NAME)) {
        Serial.print("* MDNS responder started. Hostname -> ");
        Serial.println(HOST_NAME);
    }
    MDNS.addService("telnet", "tcp", 23);
#endif
  // Initialize RemoteDebug
  Debug.begin(HOST_NAME);         // Initialize the WiFi server
  Debug.setResetCmdEnabled(true); // Enable the reset command
  Debug.showProfiler(true);       // Profiler (Good to measure times, to optimize codes)
  Debug.showColors(true);         // Colors
}

void initPubSub()
{
  PSclient.setServer(conf.mqttserver, 1883); 
  PSclient.setCallback(mqttcallback); 
}

void ICACHE_FLASH_ATTR setup(void) {  
  MARKTIME=true;                   
	initSerial(); marktime("initSerial");
  Serial.print("vers. "); Serial.println(versinst);
  if (INITFAB) { initFab(); marktime("initFab");}     
	initSPIFSS(false,true); marktime("initSPIFSS");
	leerConf(); marktime("leerConf");  
	initDS18B20(); marktime("initDS18B20");
	initEntDig(); marktime("initEntDig");
	initSalDig(); marktime("initSalDig");
	initGpios(); marktime("initGpios");
  if (conf.TFTenabled)  { initTFT(); marktime("initTFT"); }
	initWiFi(); marktime("initWiFi");
  initFTP(); marktime("initFTP");
  initHTML(); marktime("initHTML");
  initWebserver(); marktime("initWebserver");
  initPubSub();
  initDebug(); marktime("initDebug");
  internetON=(checkInternet()==200); marktime("checkInternet");
  Serial.println(internetON?"Internet OK":"NO Internet");
  if (internetON)
    {
    initTime(); marktime("initTime");
    checkMyIP(); marktime("checkMyIP");
    dPrint(t(ippublica)); dPrint(dp); dPrint(conf.myippub); dPrint(crlf);
    if(conf.iftttenabled) { initIFTTT(); marktime("initIFTTT");  }
    }
  initLCD(); marktime("initLCD");
  if (conf.TFTenabled)   
    { 
    clearTFT();
    drawIP();    delay(1000);
    clearTFT();
    drawTFT();
    } 
  if (conf.modofi==0)
    {
    //  initbmp085();
    //  init433();
    //  thermistor = new NTC_Thermistor(SENSOR_PIN,REFERENCE_RESISTANCE,NOMINAL_RESISTANCE,NOMINAL_TEMPERATURE,B_VALUE);
    }
  else if (conf.modofi==1)
    {
    //  initbmp085();
    //  init433();
    //  thermistor = new NTC_Thermistor(SENSOR_PIN,REFERENCE_RESISTANCE,NOMINAL_RESISTANCE,NOMINAL_TEMPERATURE,B_VALUE);
    }
  for (byte i=0;i<maxsalrem;i++) lasttimeremote[i]=(millis()/1000);
  MARKTIME=false;                   
  Serial.println("END SETUP");  Serial.println("----------------------------");
  Serial.println("Type 'help' to help");  Serial.println("----------------------------");
}

void checkbtPar1(int x, int y)
{
  for (byte i=0;i<4;i++)  // parámetros T1 a T4
    { if (btPar1[i].contains(x,y)) 
      { 
      long auxL=getNumberTFT(conf.T[i],2,"");  // retorna con OK o Cancel
      if (auxL!=-1) { conf.T[i]=auxL; saveconf(); }
      } 
    }
  if (btPar1[4].contains(x,y)) // CC
    { 
    long auxL=getNumberTFT(conf.CC,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.CC=auxL; saveconf(); }
    drawTFT();  
    } 
  if (btPar1[5].contains(x,y)) // CCC
    { 
    long auxL=getNumberTFT(conf.CCC,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.CCC=auxL; saveconf(); }
    drawTFT();  
    } 
  if (btPar1[6].contains(x,y)) // FRO
    { 
    long auxL=getNumberTFT(conf.FRO,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.FRO=auxL; saveconf(); }
    drawTFT();  
    } 
  if (btPar1[7].contains(x,y)) // ECS
    { 
    long auxL=getNumberTFT(conf.ECS,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.ECS=auxL; saveconf(); }
    drawTFT();  
    } 
  if (btPar1[8].contains(x,y)) // PIS
    { 
    long auxL=getNumberTFT(conf.PIS,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.PIS=auxL; saveconf(); }
    drawTFT();  
    } 
  if (btPar1[9].contains(x,y)) // RES
    { 
    long auxL=getNumberTFT(conf.RES,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.RES=auxL; saveconf(); }
    drawTFT();  
    } 
}

void checkbtPar2(int x, int y)
{
  if (btPar2[0].contains(x,y)) 
    { 
    long auxL=getNumberTFT(conf.HYC,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.HYC=auxL; saveconf(); }
    } 
  if (btPar2[1].contains(x,y)) 
    { 
    long auxL=getNumberTFT(conf.HYE,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.HYE=auxL; saveconf(); }
    } 
  if (btPar2[2].contains(x,y)) 
    { 
    long auxL=getNumberTFT(conf.HYF,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.HYF=auxL; saveconf(); }
    } 
  if (btPar2[3].contains(x,y)) 
    { 
    long auxL=getNumberTFT(conf.HYr,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.HYr=auxL; saveconf(); }
    } 
  if (btPar2[4].contains(x,y)) // Cr1
    { 
    long auxL=getNumberTFT(conf.Cr1,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.Cr1=auxL; saveconf(); }
    drawTFT();  
    } 
  if (btPar2[5].contains(x,y)) // Cr2
    { 
    long auxL=getNumberTFT(conf.Cr2,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.Cr2=auxL; saveconf(); }
    drawTFT();  
    } 
  if (btPar2[6].contains(x,y)) // AH2
    { 
    long auxL=getNumberTFT(conf.AH2,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.AH2=auxL; saveconf(); }
    drawTFT();  
    } 
  if (btPar2[7].contains(x,y)) // AH4
    { 
    long auxL=getNumberTFT(conf.AH4,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.AH4=auxL; saveconf(); }
    drawTFT();  
    } 
  if (btPar2[8].contains(x,y)) // Ab4
    { 
    long auxL=getNumberTFT(conf.Ab4,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.Ab4=auxL; saveconf(); }
    drawTFT();  
    } 
  if (btPar2[9].contains(x,y)) // tLP
    { 
    long auxL=getNumberTFT(conf.tLP,2,"");  // retorna con OK o Cancel
    if (auxL!=-1) { conf.tLP=auxL; saveconf(); }
    drawTFT();  
    } 
}

void checkbtST(int x, int y)
{
  for (byte i=0;i<4;i++)    // botones página atrás/adelante, en cualquier página
    {
    if (btST[i].contains(x,y))  
      {
      if (i==0) if (tftpage>0) { tftpage--; } else { tftpage=maxtftpages; }
      if (i==3) if (tftpage<maxtftpages) { tftpage++; } else { tftpage=0; }
      clearTFT();
      drawTFT(); 
      }
    }
}

void checkbtSD(int x, int y)
{
  for (byte i=0;i<maxSD;i++)  // botones salidas digitales 
    {
    if (btSD[i].contains(x,y)) 
      { 
      if ((i>=0) && (i<=maxSD)) 
        { pinVAL(i, getbit8(conf.MbC8,i)==0?1:0,conf.iddevice); drawTFT(); } 
      }
    }
}

void checkbtED(int x, int y)
{
  for (byte i=0;i<maxED;i++)  // botones entradas digitales 
    { if (btED[i].contains(x,y)) 
      { resetcontador(i); drawTFT();  } }
}

void checkbtGPIOS(int x, int y)
{
  for (byte i=0;i<maxgpiovar;i++)  
    {
    if (gpiovis(i))
      {
      if (btGPIO[i].contains(x,y))   
        {
        if (conf.gpiosensortype[i]==0) { resetcontador(i+10); drawTFT(); }  // GPIO como Input
        else if (conf.gpiosensortype[i]==1) 
          { if ((i>=0) && (i<maxgpiovar)) 
            { pinVAL(i+10, getbit8(conf.MbC8gpio,i)==0?1:0,conf.iddevice);  }               }
        drawTFT(); 
        }
      }
    }
}

void checkbtNET(int x, int y)
{
  for (byte i=0;i<4;i++)  // botones IP: SCAN, ...
    { 
    if (btIP[i].contains(x,y))  
      { 
      if (i==0) { selectapTFT();}
      else if (i==1) { setidTFT(); strcpy(conf.passSTA, auxdesc); }  
      }
    }
}

void checkbtSYS(int x, int y)
{
  for (byte i=0;i<4;i++)  // botones comandos sistema (reset, ...)
    { 
    if (btSET[i].contains(x,y)) 
      { 
      if (i==0) { ESP.restart(); }    
      }  
    }
}

void checkbtAP(int x, int y)
{
  for (byte i=0;i<4;i++)  // botones comandos
    { 
    if (btST[i].contains(x,y))  
      { 
      if (i==0) { tftapactual--; if (tftapactual<0) tftapactual=nAP-1; drawAP(tftapactual);}
      else if (i==1) { tftpage=4; }
      else if (i==2) { WiFi.SSID(tftapactual).toCharArray(conf.ssidSTA, 20); saveconf(); tftpage=5; }
      else if (i==3) { tftapactual++; if (tftapactual>nAP-1) tftapactual=0; drawAP(tftapactual);}
      }  
    }
}

void checkbtBC(int x, int y)
{
  if (btBC[0].contains(x,y)) { conf.Cal=conf.Cal==0?1:0; if (conf.Cal==1) conf.Ref=0; drawTFT(); }
  if (btBC[1].contains(x,y)) { conf.Ref=conf.Ref==0?1:0; if (conf.Ref==1) conf.Cal=0; drawTFT(); }
  if (btBC[2].contains(x,y)) { conf.ACS=conf.ACS==0?1:0; drawTFT(); }
  // ARRANQUE MANUAL de bombas y compresor
  if (conf.Cal+conf.Ref+conf.ACS==0)
    {
    if (btBC[6].contains(x,y)) { pinVAL(2,getbit8(conf.MbC8,2)==0?1:0,conf.iddevice); drawTFT(); } 
    if (btBC[7].contains(x,y)) { pinVAL(3,getbit8(conf.MbC8,3)==0?1:0,conf.iddevice); drawTFT(); } 
    if (btBC[8].contains(x,y)) { pinVAL(4,getbit8(conf.MbC8,4)==0?1:0,conf.iddevice); drawTFT(); } 
    if (btBC[9].contains(x,y)) { pinVAL(5,getbit8(conf.MbC8,5)==0?1:0,conf.iddevice); drawTFT(); } 
    if (btBC[10].contains(x,y)) { pinVAL(0,getbit8(conf.MbC8,0)==0?1:0,conf.iddevice); drawTFT(); } 
    } 
  // PRUEBA
}

void checkbtAL(int x, int y)
{
  for (byte i=0;i<3;i++)  // botones ok,esc,backspace
    { 
    if (btYN[i].contains(x,y)) 
      {
      Serial.println("checkbtAL");
      estalarma[tipoalarma1]=2;    // reconocer alarma
      tftpage=0;
      delay(50);
      clearTFT();
      drawTFT();
      }  
    }
}

void handletfttouch()
{
  uint16_t x, y;
  if (tft.getTouch(&x, &y))
    {
    delay(50);
    if (tft.getRotation()==3) { x=tft.width()-x; y=tft.height()-y;}
    if (conf.modofi==0)
      {
      if (tftpage<7) { checkbtST(x,y); }           // check botones navegacion por páginas
      if (tftpage==0) { checkbtSD(x,y); }          // check botones salidas digitales
      else if (tftpage==1) { checkbtED(x,y); }     // check botones entradas digitales
      else if (tftpage==3) { checkbtGPIOS(x,y); }  // check botones GPIOs
      else if (tftpage==4) { checkbtNET(x,y); }    // check botones WiFi
      else if (tftpage==5) { checkbtSYS(x,y); }    // check comandos sistema (reset, ...)
      else if (tftpage==7) { checkbtAP(x,y);  }    // check botones select AP
      else if (tftpage==21)      // readtext()
        {
        for (byte i=0;i<4;i++)  // botones comandos
          { 
          if (btST[i].contains(x,y))  
            { 
            delay(100);
            if (i==0) { if (strlen(auxtft)>0) auxtft[strlen(auxtft)-1]=0x00; readtextTFT(); }
            if (i==1) { tftpage=4; }
            else if (i==2) { strcpy(conf.passSTA,auxtft); saveconf(); tftpage=5;  }
            }  
          }
        for (byte i=0;i<10;i++)  // botones números
          { 
          if (strlen(auxtft)<20)
            if (btKEY[i].contains(x,y))  
              { 
              delay(100);
              strcat(auxtft,itoa(i,buff,10));  
              readtextTFT();
              }
          }          
        }
      }
    else if (conf.modofi==1)     // bomba de calor    
      {
      if (tftpage<7) { checkbtST(x,y); }     // check botones navegacion por páginas
      if (tftpage==0) { checkbtBC(x,y); }    // botones CAL, REF, ACS 
      else if (tftpage==1) { checkbtPar1(x,y); }   // parámetros 1
      else if (tftpage==2) { checkbtPar2(x,y); }   // parámetros 2
      else if (tftpage==3) { checkbtGPIOS(x,y); }  // check botones GPIOs
      else if (tftpage==4) { checkbtNET(x,y); }    // check botones WiFi
      else if (tftpage==5) { checkbtSYS(x,y); }    // check comandos sistema (reset, ...)
      else if (tftpage==21)      // readtext()
        {
        for (byte i=0;i<4;i++)  // botones comandos
          { 
          if (btST[i].contains(x,y))  
            { 
            delay(100);
            if (i==0) { if (strlen(auxtft)>0) auxtft[strlen(auxtft)-1]=0x00; readtextTFT(); }
            if (i==1) { tftpage=4; }
            else if (i==2) { strcpy(conf.passSTA,auxtft); saveconf(); tftpage=5;  }
            }  
          }
        for (byte i=0;i<10;i++)  // botones números
          { 
          if (strlen(auxtft)<20)
            if (btKEY[i].contains(x,y))  
              { 
              delay(100);
              strcat(auxtft,itoa(i,buff,10));  
              readtextTFT();
              }
          }          
        }
      else if (tftpage==22) 
        { 
        checkbtAL(x,y); 
        }    // check botón reconocer alarma
      }
    } 
}

void testChange() {
	if(statusChange) {
		Serial.println("Change ON");
		if (conf.TFTenabled==1) 
		  {
		  lcdshowstatus();
      drawTFT();
 	  }
		if(conf.mododweet==1) { postdweet(mac); }
		if(conf.iottweetenable==1) { postIoTweet(); }
	}

	if((iftttchange[0]>0) || (iftttchange[1]>0)) {
		for(byte i=0; i<maxSD; i++) {
			if(getbit8(iftttchange,i)==1) {   // SD i
				if((getbit8(conf.iftttpinSD,i)==1) && (getbit8(conf.MbC8,i)==1))
				{ ifttttrigger(conucochar, conf.iftttkey, conf.aliasdevice, readdescr(filedesclocal,i+14,20), textonoff(1)); }

				if((getbit8(conf.iftttpinSD,i+8)==1) && (getbit8(conf.MbC8,i)==0))
				{ ifttttrigger(conucochar, conf.iftttkey, conf.aliasdevice, readdescr(filedesclocal,i+14,20), textonoff(0)); }
			}
		}

		for(byte i=0; i<maxED; i++) {
			if(getbit8(iftttchange,i+8)==1) {    // ED i
				if((getbit8(conf.iftttpinED,i)==1) && (getbit8(conf.MbC8,i+8)==1))      // en ON
				{ ifttttrigger(conucochar, conf.iftttkey, conf.aliasdevice, readdescr(filedesclocal,i+10,20), textonoff(1)); }

				if((getbit8(conf.iftttpinED,i+8)==1) && (getbit8(conf.MbC8,i+8)==0))      // en OFF
				{ ifttttrigger(conucochar, conf.iftttkey, conf.aliasdevice, readdescr(filedesclocal,i+10,20), textonoff(0)); }
			}
		}

		iftttchange[0]=0;
	}
}

void printhelp()
{
  Serial.println("alias,aliasdevice      (set alias)");
  Serial.println("debug                  (debug ON/OFF)");
  Serial.println("files                  (check files)");
  Serial.println("help");
  Serial.println("id,iddevice            (150-166)");
  Serial.println("instal,instaldevice    (set instal)");
  Serial.println("pass,SSIDpass          (set passSTA)");
  Serial.println("passAP,SSID-APpass     (set pass AP)");
  Serial.println("reset");
  Serial.println("resetfact              (reset factory)");
  Serial.println("seg,segnet             (segmento red)");
  Serial.println("ssid,SSIDname          (set SSID STA)");
  Serial.println("ssidAP,SSID-AP         (set SSID AP)");
  Serial.println("st                     (show status)");
  Serial.println("wm,n                   (wifimode:0,1,2=STA,AP,STA+AP)");
  Serial.println("---------------------------------------");
}

void printstatus()
{
  Serial.print("DEV:");       Serial.println(conf.iddevice);
  Serial.print("alias:");     Serial.println(conf.aliasdevice);
  
  if(conf.wifimode==0) { WiFi.mode(WIFI_STA); }
  else if(conf.wifimode==1) { WiFi.mode(WIFI_AP); }
  else if(conf.wifimode==2) { WiFi.mode(WIFI_AP_STA); }

  Serial.print("WiFi Mode:"); Serial.println(conf.wifimode==0?"STA":conf.wifimode==1?"AP":"AP+STA");
  Serial.print("IP:");        Serial.println(WiFi.localIP());
  Serial.print("Port:");      Serial.println(conf.webPort);
  Serial.print("mask:");      Serial.println(WiFi.subnetMask());
  Serial.print("GW:");        Serial.println(WiFi.gatewayIP());
  Serial.print("SSID:");      Serial.println(conf.ssidSTA);
  Serial.print("Pass:");      Serial.println(conf.passSTA);
  Serial.print("APSSID:");    Serial.println(conf.ssidAP);
  Serial.print("APPass:");    Serial.println(conf.passAP);
  Serial.print("Conn:");      Serial.println(WiFi.isConnected()?ok:"NO");
  Serial.println("--------------------------");
}

void execcom() {
	String command, param;
	command=sinput.substring(0,sinput.indexOf(","));
	param=sinput.substring(sinput.indexOf(",")+1);
  boolean done=false;
//  Serial.print("comando/param: "); Serial.print(command);Serial.print("/");Serial.println(param);
	if( command=="files") {
		File dir=SPIFFS.open(barra);
		File file=dir.openNextFile();
		while(file) { Serial.print(file.name()); Serial.print(b); Serial.println(file.size()); file=dir.openNextFile(); }
		Serial.print("files ");
		Serial.println(checkfiles()?"OK":"ERROR"); } 
	else if (command=="help") { printhelp(); } 
	else if(command=="st") { printstatus();	} 
	else if (command=="id") { conf.iddevice=param.toInt(); conf.EEip[3]=conf.iddevice; param.toCharArray(conf.mqttpath[2],10); done=true; }
	else if (command=="alias") { param.toCharArray(conf.aliasdevice,20);done=true; }
	else if (command=="instal") { param.toCharArray(conf.instname,10); param.toCharArray(conf.mqttpath[1],10); done=true; }
  else if (command=="ssid") { param.toCharArray(conf.ssidSTA,20); done=true; }
  else if (command=="pass") { param.toCharArray(conf.passSTA,20); done=true; }
  else if (command=="ssidAP") { param.toCharArray(conf.ssidAP,20); done=true; }
  else if (command=="passAP") { param.toCharArray(conf.passAP,20); done=true; }
  else if (command=="seg") { conf.netseg=param.toInt(); conf.EEip[2]=conf.netseg; conf.EEgw[2]=conf.netseg; done=true; }
  else if (command=="debug") { MARKTIME=!MARKTIME; done=true; }
  else if (command=="reset") { ESP.restart(); }
  else if (command=="resetfact") { initFab(); }
  else if (command=="wm") { if ((param.toInt()>=0) &&(param.toInt()<=2)) { conf.wifimode=param.toInt(); done=true; }}
  else { Serial.println("Command not found"); }
  if (done)
    {
    saveconf();
    Serial.println("Done"); 
    printstatus();  
    }
}

void task1() {
	tini=millis();
	countfaulttime++;   // si se hace mayor que TempDesactPrg,desactiva ejecucion programas dependientes de fecha
	leevaloresDIG();  marktime("leevaloresDIG");
  leevaloresGPIO();  marktime("leevaloresGPIO");
  procesaeventos();  marktime("procesaeventos");
	//procesaTimeMax();

	if(conf.modofi==0)    // normal
	  {
		for(byte j=0; j<maxsalrem; j++)
			if(conf.idsalremote[j]>0)
				if((conf.senalrem[j]==6) || (conf.senalrem[j]==7)) { contaremote[j]++; }
    if (tftpage<7) drawTFT();
	  } 
	else if(conf.modofi==1) { procesaBC(); marktime("procesaBC");} // bomba calor
	testChange();
	tfton=(millis()-lasttouch)<60000;
//  if (!tfton) { tft.fillScreen(TFT_BLACK); }
	if(conf.rstper>0) {	if(millis() > 3600000*conf.rstper) { Serial.println("RESTART");	ESP.restart();	}	}
  procesaSemanal(); marktime("procesaSemanal");
	mact1=millis();
	}

void taskvar() {
  tini=millis();
	if (conf.TFTenabled) lcdshowstatus();
	leevaloresOW();
  //leevaloresDHT();
	procesaconsignas();
  mqttpublishvalues();
  if (internetON)
    {
		if(conf.mododweet == 1) { postdweet(mac); }
	  if(conf.iottweetenable == 1) { postIoTweet(); }
    }
	if((millis()-tini)>5000) {printhora(); Serial.print(F(" 5 SEG:")); Serial.println(millis()-tini);}
	mact10 = millis();
}

void task60() {   // 60 segundos
  Serial.println("task60");
	tini = millis();
  internetON=(checkInternet()==200); marktime("checkInternet");
  Serial.print("Internet:"); Serial.println(internetON?"ON":"OFF");
	memset(bevenENABLE, sizeof(bevenENABLE),0);
	procesaSemanal();
	procesaFechas();
	if((millis() - tini) > 5000) { Serial.print(60); Serial.print(F(" SEG:")); Serial.println(millis() - tini); }
	mact60 = millis();
}

void task3600() {       // 3600 segundos=1 hora
	tini = millis();
//  if (WiFi.isConnected()) {
//    timeClient.setTimeOffset(7200);
//    if (timeClient.update()==1) { countfaulttime=0; setTime(timeClient.getEpochTime());  }
//    checkMyIP();
//    checkForUpdates();
//    }
	if(millis()-tini>1000) { Serial.print(3600); Serial.print(F(" seg.:")); Serial.println(millis()-tini); }
	mact3600 = millis();
}

void handleSerial()
{

  if (Serial.available()) {
    char tChar = Serial.read(); 
    if((tChar=='\n') || (tChar=='\r')) { execcom();  sinput="";  }
    else { sinput=sinput+tChar;  } }
}

void handleFTP()  
  { 
  if(conf.ftpenable) 
    { 
    ftpSrv.handleFTP(); 
    } 
  }
    
void handleWebclient()  { server.handleClient(); }
void handlePubSub()
{
  if (WiFi.isConnected()) 
    {
    if(conf.mqttenabled) 
      {
      if (!PSclient.connected()) { mqttreconnect(); }
      }
    PSclient.loop();
    }
}

void tictacxxx(byte pin, int dur)
  {
  Serial.print(pin); Serial.print(":");  
  digitalWrite(pin,0); delay(dur/2);  Serial.print(digitalRead(pin));
  digitalWrite(pin,1); delay(2*dur); Serial.print(digitalRead(pin));  
  digitalWrite(pin,0); delay(dur/2);Serial.println(digitalRead(pin));
  }
long lastMsg = 0;
char msgx[50];
int value = 0;

void loop(void) 
{
	tini=millis();
  handlePubSub();
  handleSerial();     debugV("handleSerial"); marktime("handleSerial");
  handleFTP();        debugV("handleFTP"); marktime("handleFTP");
  handleWebclient();  debugV("handleWebclient"); marktime("handleWebclient");
	leevaloresDIG();    debugV("leevaloresDIG"); marktime("leevaloresDIG");
	handletfttouch();   debugV("handletfttouch"); marktime("handletfttouch");
  Debug.handle();     debugV("Debug.handle"); marktime("Debug.handle");
	//handleRF();
	//////////////////////////////////////////////////////////////////////////////////////////
	if((millis() > (mact1 + 1000))) { task1(); marktime("task1"); }                         // tareas que se hacen cada segundo
	if((millis() > (mact10 + (conf.peractrem * 1000)))) { taskvar(); marktime("taskvar");}   // tareas que se hacen cada "peractrem" segundos
	if((millis() > (mact60 + 60000))) { task60; marktime("task60");}    // tareas que se hacen cada 60 segundos:1 minuto
	if((millis() > (mact3600 + 3600000))) { task3600(); }    // tareas que se hacen cada 3600 segundos:1 hora
	if((millis() > (mact86400 + 86400000))) {   // tareas que se hacen cada 86400 segundos: 1 día
		tini = millis();
		ifttttrigger(conucochar, conf.iftttkey, conf.aliasdevice, "MyIP", conf.myippub);  
		if(millis()-tini>1000) {Serial.print(F("1 día.:")); Serial.println(millis()-tini);}
		mact86400 = millis();
  	}
  marktime("---------------------");
}
