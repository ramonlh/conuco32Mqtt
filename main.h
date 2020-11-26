

void ICACHE_FLASH_ATTR mqttpublish(byte idori, int i)
{
  if (!WiFi.isConnected()) return;
  strcpy(auxdesc,conf.mqttpath[0]); strcat(auxdesc,"/");
  for (byte j=1;j<6;j++) { if (strlen(conf.mqttpath[j])>0) { strcat(auxdesc,conf.mqttpath[j]); strcat(auxdesc,"/"); } }
  if (i<=7)         // temperaturas
    { 
    strcat(auxdesc,letrat); strcat(auxdesc,itoa(i,buff,10));
    char buff1[10], buff2[10];
    itoa(MbR[i]/100,buff1,10); itoa(MbR[i]%100,buff2,10);      
    strcpy(auxchar,buff1); strcat(auxchar,"."); strcat(auxchar,buff2);  
    }
  else if (i<=11)  // entradas
    { 
    strcat(auxdesc,letrae); strcat(auxdesc,itoa(i-8,buff,10));
    strcpy(auxchar,itoa(getbit8(conf.MbC8,i),buff,10));  
    }       
  else if (i<=19)     // salidas
    { 
    strcat(auxdesc,letras); strcat(auxdesc,itoa(i-12,buff,10));
    strcpy(auxchar,itoa(getbit8(conf.MbC8,i-12),buff,10));  
    }
  else if (i<=27)    // consignas
    { 
    strcat(auxdesc,letrat); strcat(auxdesc,letras); strcat(auxdesc,itoa(i-20,buff,10));
    char buff1[10], buff2[10];
    long l1, l2;
    l1=conf.setpoint[i-20]*100; // 2 decimales
    l2=l1%100;                  // los dos decimales
    l1=l1/100;                  // la parte entera
    itoa(l1,buff1,10); itoa(l2,buff2,10);      
    strcpy(auxchar,buff1); strcat(auxchar,"."); strcat(auxchar,buff2);  
    }  
  else if (i<=28) { strcat(auxdesc,ID); strcpy(auxchar,itoa(conf.iddevice,buff,10));  }   // iddevice
  else if (i<=29) // IP privada 
    {
    strcat(auxdesc,IP); strcat(auxchar,"x.x");
    for (byte j=2;j<4;j++) { strcat(auxchar,"."); strcat(auxchar,itoa(WiFi.localIP()[j],buff,10)); } }
  else if (i<=30) { strcat(auxdesc,IPP); strcpy(auxchar,conf.myippub);  }    // IP pública
  else if (i<=40)    // Gpios
    {
    strcat(auxdesc,letrag); strcat(auxdesc,itoa(i-31,buff,10));
    if (conf.gpiosensortype[i-31]==0)   // INPUT
      strcpy(auxchar,itoa(getbit8(conf.MbC8gpio,i-31+16),buff,10));  
    else if (conf.gpiosensortype[i-31]==1)   // OUTPUT
      strcpy(auxchar,itoa(getbit8(conf.MbC8gpio,i-31),buff,10));  
    }   
  else if (i==41)   // RCF  Request Conf Remote
    {
    strcpy(auxdesc,conf.mqttpath[0]); strcat(auxdesc,"/");
    strcat(auxdesc,conf.mqttpath[1]); strcat(auxdesc,"/");
    strcat(auxdesc,itoa(idori,buff,10)); strcat(auxdesc,"/"); 
    strcat(auxdesc,letraR);strcat(auxdesc,letraC);strcat(auxdesc,letraF);
    strcpy(auxchar,vacio);
    }
  else if (i<=45)  // contaremote de entradas
    { 
    strcat(auxdesc,letrae); strcat(auxdesc,letran); strcat(auxdesc,itoa(i-42,buff,10));
    strcpy(auxchar,itoa(conf.contadores[i-42],buff,10));  
    }       
  else if (i<=53)  // timeremote de salidas
    { 

    unsigned long segundos=(millis()/1000)-(getbit8(conf.MbC8,i-46)==1?tempact[i-46]:tempdes[i-46]);
    strcat(auxdesc,letras); strcat(auxdesc,letrat); strcat(auxdesc,itoa(i-46,buff,10));
    strcpy(auxchar,itoa(segundos,buff,10));  
    } 
  PSclient.publish(auxdesc, auxchar);   // topic, payload
  strcpy(auxdesc,vacio);strcpy(auxchar,vacio);
}

void ICACHE_FLASH_ATTR mqttpublishc(byte idori, int i)
{
  if (!WiFi.isConnected()) return;
  if (i<=19)         // nombres de señales locales
    { 
    strcpy(auxchar,readdescr(filedesclocal,i,20));
    strcpy(auxdesc,conf.mqttpath[0]); strcat(auxdesc,"/");
    for (byte j=1;j<6;j++) { if (strlen(conf.mqttpath[j])>0) { strcat(auxdesc,conf.mqttpath[j]); strcat(auxdesc,"/"); } }
    strcat(auxdesc,idpinn[i]);
    }
  else if (i<=27)    // consignas
    { 
    }  
  else if ((i>=31) && (i<=40))    // Gpios
    {
    }   
  PSclient.publish(auxdesc, auxchar);   // topic, payload
  strcpy(auxdesc,vacio);strcpy(auxchar,vacio);
}

void ICACHE_FLASH_ATTR testTX433()
{
  mySwitch.send(conf.rfkeys.code[0], 24);
}

byte tipodevrem(byte salrem)
{
  boolean encontrado=false;  byte i=0;
  while ((i<maxdevrem) && (!encontrado))  { encontrado=(conf.tiporemote[i]==salrem); if (!encontrado) i++;  }
  return encontrado?i:0;
}

void senddashtag(File f, int tag)
{ f.print(comillas); f.print(c(tag)); f.print(comillas); f.print(dp); }
void senddashint(File f, long data, boolean wcoma)
{ f.print(data); if (wcoma) f.print(coma); }
void senddashfloat(File f, float data, boolean wcoma)
{ f.print(comillas); f.print(data); f.print(comillas); if (wcoma) f.print(coma); }
void senddashbool(File f, boolean data, boolean wcoma)
{ if (data) f.print(c(ttrue)); else f.print(c(tfalse)); if (wcoma) f.print(coma); }
void senddashtext(File f, int ntext, boolean wcoma)
{ f.print(comillas); f.print(c(ntext)); f.print(comillas); if (wcoma) f.print(coma); }
void senddashtext(File f, PGM_P data, boolean wcoma)
{ f.print(comillas); f.print(data); f.print(comillas); if (wcoma) f.print(coma); }

void senddashlocal(File f, int descr, boolean wcoma)
{  
  f.print(comillas); 
  if (descr<=30)
    f.print(readdescr(filedesclocal, descr, 20)); 
  else
    f.print(readdescr(filedescgpio, descr-31, 20)); 
  f.print(comillas); if (wcoma) f.print(coma); 
}

void senddashrem(File f, int descr, boolean wcoma)
{  f.print(comillas); f.print(readdescr(filesalrem, descr, 20)); f.print(comillas); if (wcoma) f.print(coma); }

void senddashpub(File f, int npin, boolean wcoma, PGM_P suf)
{
  strcpy(auxdesc,conf.mqttpath[0]); strcat(auxdesc,barra);
  for (byte j=1;j<6;j++) { if (strlen(conf.mqttpath[j])>0) { strcat(auxdesc,conf.mqttpath[j]); strcat(auxdesc,"/"); } }
  if (npin<=27)    // 8 temp. + 4 e.dig. + 8 sal. dig
    { strcat(auxdesc,idpin[npin]); strcat(auxdesc,suf); }
  else if (npin<=30)    // ID, IP, IPP, 
    { strcat(auxdesc,idpin[npin]); strcat(auxdesc,suf); }
  else if (npin<=40)
    { strcat(auxdesc,letrag); strcat(auxdesc,itoa(npin-31,buff,10)); strcat(auxdesc,suf); }
  f.print(comillas); f.print(auxdesc); f.print(comillas); if (wcoma) f.print(coma);
}

void senddashpubrem(File f, byte rem, int npin, boolean wcoma, PGM_P suf)
{
  strcpy(auxdesc,conf.mqttpath[0]); strcat(auxdesc,barra);
  for (byte j=1;j<6;j++) 
    { 
    if (strlen(conf.mqttpath[j])>0) 
      {
      if (j==2) strcat(auxdesc,itoa(conf.idsalremote[rem],buff,10)); 
      else  strcat(auxdesc,conf.mqttpath[j]);
      strcat(auxdesc,"/"); 
      } 
    }
  if (true) 
//  if (conf.tiporemote[]==1)   // ESP32
    { strcat(auxdesc,idpin[npin]); strcat(auxdesc,suf); }
  else                        // ESP8266
    { strcat(auxdesc,idpin8266[npin]); strcat(auxdesc,suf); }
  f.print(comillas); f.print(auxdesc); f.print(comillas); if (wcoma) f.print(coma);
}

void senddashi2c(File f, byte rem, int npin, boolean wcoma, PGM_P suf)
{
  strcpy(auxdesc,conf.mqttpath[0]); strcat(auxdesc,barra);
  for (byte j=1;j<6;j++) { if (strlen(conf.mqttpath[j])>0) { strcat(auxdesc,conf.mqttpath[j]); strcat(auxdesc,"/"); }  }
  strcat(auxdesc,letrar); strcat(auxdesc,itoa(rem,buff,10)); strcat(auxdesc,suf);
  f.print(comillas); f.print(auxdesc); f.print(comillas); if (wcoma) f.print(coma);
}

void senddashtap(File f, int npin, boolean wcoma, PGM_P suf)
{
  strcpy(auxdesc,comillas);
  strcat(auxdesc,"app.publish");
  strcat(auxdesc,paren_i);strcat(auxdesc,comilla);
  strcat(auxdesc,conf.mqttpath[0]); strcat(auxdesc,barra);
  for (byte j=1;j<6;j++) { if (strlen(conf.mqttpath[j])>0) { strcat(auxdesc,conf.mqttpath[j]); strcat(auxdesc,"/"); } }
  strcat(auxdesc,idpin[npin]); strcat(auxdesc,suf);
  f.print(auxdesc); 
  f.print(comilla);f.print(coma);
  f.print(c(eventdatalast01));f.print(comillas); 
  if (wcoma) f.print(coma);
}

void senddashrec(File f)
{
  f.print(comillas);
  f.print(c(ifeventdata));
  f.print(c(evendatalast));
  f.print(comillas);f.print(coma);
}

boolean primero=true;

void sendcomunes(File f, byte i)
{
  if (!primero) f.print(coma);
  primero=false;
  f.print(llave_i);
  senddashtag(f, dashenableIntermediateState);  senddashbool(f, true, true); 
  senddashtag(f, dashenteredIntermediateStateAt);  senddashint(f, 1, true); 
  senddashtag(f, dashintermediateStateTimeout);  senddashint(f, 1, true); 
  senddashtag(f, dashqos);  senddashint(f, 0, true); 
  senddashtag(f, dashretained);  senddashbool(f, false, true); 
  senddashtag(f, dashjsOnReceive);  senddashtext(f, vacio, true); 
  senddashtag(f, dashjsonPath);  senddashtext(f, vacio, true);
  senddashtag(f, dashlastActivity);  senddashint(f, 0, true);
  senddashtag(f, dashupdateLastPayloadOnPub);  senddashbool(f, true, true); 
  senddashtag(f, dashjsBlinkExpression);  senddashtext(f, vacio, true); 
  senddashtag(f, dashenablePub);  senddashbool(f, true, true); 
  senddashtag(f, dashjsOnTap);  senddashtext(f, vacio, true); 
  senddashtag(f, dashjsOnDisplay);  senddashtext(f, vacio, true); 
  senddashtag(f, dashid);  senddashint(f, i+1, true);
  senddashtag(f, dashlongId);  senddashint(f, i+1 , true); 
}

void ICACHE_FLASH_ATTR leevaloresDIG()
{
  for (byte i=0;i<maxSD;i++) setbit8(conf.MbC8,i,digitalRead(sdPin[i]));
  for (byte i=0;i<maxED;i++) 
    {
    byte valaux=digitalRead(edPin[i]);
    if (conf.tipoED[i]==0)
      setbit8(conf.MbC8,i+8,valaux);  
    else if (conf.tipoED[i]==1) 
      if (valaux==0)
        setbit8(conf.MbC8,i+8,1);
      else
        setbit8(conf.MbC8,i+8,0);
    statusChange=((getbit8(conf.MbC8,i+8) != getbit8(MbC8ant,i+8)));
    if (statusChange) 
      {
      setbit8(iftttchange,i+8,1);   
      if (getbit8(conf.MbC8,i+8)==1) conf.contadores[i]++;  
      }
    }

  MbC8ant[1]=conf.MbC8[1];
  
  for (byte i=0;i<maxgpiovar;i++)   // lee Gpios que son ED
    {
    if (gpiovis(i))
      {
      if (conf.gpiosensortype[i]==0)   // input
        {
        byte valaux=digitalRead(listgpiovar[i]); 
        setbit8(conf.MbC8gpio,i+16,valaux); 
        }
      statusChange=((getbit8(conf.MbC8gpio,i+16) != getbit8(MbC8antgpio,i+16)));
      if (statusChange) 
        {
        setbit8(iftttchange,i+16,1);   
        if (getbit8(conf.MbC8gpio,i+16)==1) conf.contadoresgpio[i]++;  
        }
      }
    }
  MbC8antgpio[2]=conf.MbC8gpio[2];  MbC8antgpio[3]=conf.MbC8gpio[3];
  if (conf.modofi==1)
    {
    for (byte i=0;i<5;i++)
      {
      idbc[i]=getbit8(conf.MbC8gpio,idxgpio[i]+16);
      }
    }
}

void ICACHE_FLASH_ATTR leevaloresGPIO() 
{ 
  for (byte i=0;i<maxgpiovar;i++)
    if (gpiovis(i))
      {
      if (conf.gpiosensortype[i]==0)     // Entr. digitales
        {
        setbit8(conf.MbC8gpio,i+16,digitalRead(listgpiovar[i])); 
        }
      else if (conf.gpiosensortype[i]==1)       //  Sal. digitales
        {
        setbit8(conf.MbC8gpio,i,digitalRead(listgpiovar[i])); 
        }
      else if (conf.gpiosensortype[i]==2)      // ADC
        {
        MbRgpio[i]=analogRead(listgpiovar[i]); 
        MbRantgpio[i]=MbRgpio[i];   
        }
      else if (conf.gpiosensortype[i]==5)   // Jonshon Controls PT1000, 1035 a 25º, B= , A99 
        {
        const float Vref=5.00;                                      // tensión de referencia
        const float V33=3.3;                                        // tensión de referencia
        const float R0=1000;                                        // resistencia del divisor
        MbRgpio[i]=analogRead(listgpiovar[i]);                      // lee valor digital del pin
        float Rt=(V33*R0*MbRgpio[i])/((Vref*4095)-(MbRgpio[i]*V33));// resistencia medida del sensor
        float auxF= (1.0 / ((1.0/298.0) - (log(Rt/conf.gpioalfa[i])/(conf.gpiobeta[i])))); // temperatura en grados K
        sumMbRgpio[i]=sumMbRgpio[i]+(auxF-273.15); avrcount[i]++;   // acumulación para la media
        MbRgpio[i]=sumMbRgpio[i]/avrcount[i];                       // media del período
        MbRgpio[i]=auxF-273.15;
        MbRantgpio[i]=MbRgpio[i];                                   // guardar valor anterior
        }
      else if (conf.gpiosensortype[i]==6)   // Carel NTC 10K, 25º B=3435
       {
        const float Vref=5.00;                                      // tensión de referencia
        const float V33=3.2;                                        // tensión de referencia
        const float R0=10000.0;                                     // resistencia del divisor
        MbRgpio[i]=analogRead(listgpiovar[i]);
        float Rt=(V33*R0*MbRgpio[i])/((Vref*4095)-(MbRgpio[i]*V33));// resistencia medida del sensor
        float auxF= (1.0 / ((1.0/298.0) - (log(Rt/conf.gpioalfa[i])/(conf.gpiobeta[i])))); // temperatura en grados K
        sumMbRgpio[i]=sumMbRgpio[i]+(auxF-273.15); avrcount[i]++;
        MbRgpio[i]=sumMbRgpio[i]/avrcount[i];
        MbRgpio[i]=auxF-273.15;
        MbRantgpio[i]=MbRgpio[i];  
        }
      else if (conf.gpiosensortype[i]==7)   // ACS712 sensor de corriente
       {
        MbRgpio[i]=analogRead(listgpiovar[i]);
        MbRantgpio[i]=MbRgpio[i];  
        }
      }
}

void ICACHE_FLASH_ATTR leevaloresDHT() 
{ 
  for (byte i=0;i<maxgpiovar;i++)
    if (gpiovis(i))
      {
      if (conf.gpiosensortype[i]==4)   // DHT 
        {
        dhtdata[i][0]=dht[i].getTemperature();
        dhtdata[i][1]=dht[i].getHumidity();
        }
      }
}

void ICACHE_FLASH_ATTR leevaloresOW()
{
  sensors0.requestTemperatures();
  for (byte i=0; i<maxTemp; i++)  
    {
    if (conf.nprobe[i]>0)
      {
      float auxF=sensors0.getTempC(conf.probecode[i]);
      if (auxF>=(-50))
        {
        auxF=auxF*100;
        MbR[i]=auxF;
        MbRant[i]=MbR[i];
        if ((conf.modofi==1) && (i<4))
          Ai[i]=round(float(MbR[i])/100);
        }
      }
    }
}

void ICACHE_FLASH_ATTR loginHTML()
{
  printremote();
  msg=vacio;
  if (server.method()==HTTP_POST)
    {
    if (server.hasArg("0") && server.hasArg("1"))
      {
      if ((server.arg(0)==conf.userDev) && (server.arg(1)==conf.passDev))
        { setCookie(1);  return; }
      }
    }
  if (server.hasArg("DISCONNECT")) { setCookie(0); return; }

  writeHeader(false,false);
  pc(body_i);
  pc(form_action);
  printP(loghtm,comillas,b);
  pc(Form_post);
  printP(menor,table,b);
  printP(c(tclass), ig, tnormal, mayor);
  printparCP(t(usuario), 0, conf.userDev, 20, false);
  printparCP(t(contrasena), 1, "", 20, true);
  printP(menor, barra, table, mayor);
  printP(menor, c(tinput), b, type, ig, comillas);
  printP(submit, comillas, b, tvalue, ig, comillas);
  printP(tguardar, comillas, mayor);
  printP(menor, barra, c(tinput), mayor);
  pc(form_f);
  pc(body_f);
  printP(menor, barra, thtml, mayor);
  serversend200();
}

//////////////////  DWEET  ///////////////////

int ICACHE_FLASH_ATTR getdweet(char* key)
{
  if (conf.mododweet==0) return 0;
  msg=vacio;
  printP(c(getlastdweett),conucochar,key);
  return callhttpGET("Dweet.io",80,true,conf.timeoutNTP);
}

int ICACHE_FLASH_ATTR postIoTweet()
{
  if (conf.iottweetenable==0) return 0;
  float data[4];                        //Your sending data variable.
  data[0]=MbR[0]*0.01;
  data[1]=sondaremote[3];
  data[2]=sondaremote[5];
  data[3]=getbit8(bstatremote, 2);
  myiot.WriteDashboard(conf.iottweetuser, conf.iottweetkey, data[0], data[1], data[2], data[3], private_tweet, public_tweet);
}

int ICACHE_FLASH_ATTR postdweet(char* key)
{
  if (conf.mododweet==0) return 0;
  msg=vacio;
  buildJson();
  strcpy(auxchar, c(dweetfor)); strcat(auxchar, conucochar); strcat(auxchar, key);
  HTTPClient http;
  http.begin(c(dweetio), 80, auxchar);
  http.addHeader(type, tPOST);
  http.addHeader(contenttype, applicationjson);
  http.addHeader(dataType, json);
  http.setConnectTimeout(conf.timeoutNTP);
  int httpCode = http.POST(msg);
  http.end();
  msg=vacio;
  return httpCode;
}

//////////////////  IFTTT  ///////////////////
int ICACHE_FLASH_ATTR ifttttrigger(char *evento, char* key, char* value1, char* value2, char* value3)
{
  if ((conf.iftttenabled==0) || (!WiFi.isConnected())) return 0;
  strcpy(auxchar, "/trigger/"); strcat(auxchar, evento); // value1, 2 y 3 tamaño máximo = 20
  strcat(auxchar, "/with/key/"); strcat(auxchar, key); strcat(auxchar, interr);
  strcat(auxchar, tvalue); strcat(auxchar, uno); strcat(auxchar, ig); strcat(auxchar, value1); strcat(auxchar, ampersand);
  strcat(auxchar, tvalue); strcat(auxchar, dos); strcat(auxchar, ig); strcat(auxchar, value2); strcat(auxchar, ampersand);
  strcat(auxchar, tvalue); strcat(auxchar, tres); strcat(auxchar, ig); strcat(auxchar, value3);
  byte i=0;
  while (auxchar[i]!='\0') { if (auxchar[i]==' ') auxchar[i]='_';  i++; }
  HTTPClient http;
  http.begin(c(makeriftttcom), 80, auxchar);
  http.setConnectTimeout(conf.timeoutNTP);
  Serial.print("ifttttrigger: ");Serial.print(c(makeriftttcom));Serial.print(auxchar);
  int httpCode=http.GET();
  Serial.print(" ");Serial.println(httpCode);
  if (httpCode<0) addlog(2, httpCode, c(ifttt));
  http.end();
  return httpCode;
}
//////////////////  END IFTTT  ///////////////////

void ICACHE_FLASH_ATTR pinVAL(byte n, byte value, byte ori)
{
  //debugI("pinVal( %d, %d, %d)",n,value,ori);
  if (n<maxSD)
    {
    if (getbit8(conf.MbC8,n)!=value)
      {
      digitalWrite(sdPin[n], valorpin[value]);
      setbit8(conf.MbC8,n,value);
      setbit8(MbC8ant,n,value);
      if (value) tempact[n]=millis()/1000; else tempdes[n]=millis()/1000;
      setbit8(iftttchange, n,1);
      statusChange=true;  
      mqttpublish(0,n+12); 
      saveconf();
      }
    }
  else
    if (getbit8(conf.MbC8gpio,n-10)!=value)
      {
      if ((n>=10) && (n<=19))   // gpios configurables
        {
        digitalWrite(listgpiovar[n-10], valorpin[value]);
        setbit8(conf.MbC8gpio,n-10,value);
        setbit8(MbC8antgpio,n-10,value);
        if (value) tempactgpio[n]=millis()/1000; else tempdesgpio[n]=millis()/1000;
        setbit8(iftttchange, n,1);
        statusChange=true; 
        mqttpublish(0,n+12); 
        saveconf();
        }
      }
}

int ICACHE_FLASH_ATTR pinvalR(byte ip, int port, byte pin, byte valor) // ejecuta comando remoto
{
  createhost(ip);
  msg=vacio;
  printP(barra, valor?on:off, interr, letrap, ig, itoa(pin+12,buff,10));
  printP(amper, letrar, ig, itoa(conf.iddevice, buff, 10));
  return callhttpGET(host,port,false,conf.timeoutrem);
}

int ICACHE_FLASH_ATTR checkInternet()
{
  msg=vacio;
  printP(barra);
  HTTPClient http;
  Serial.print("hostmyip:"); Serial.println(conf.hostmyip);
  http.begin("www.google.com", 80, msg);
  http.setConnectTimeout(1000);
  Serial.print("checkInternet ");Serial.print("host:");Serial.print("www.google.com");Serial.print(80);Serial.print(":");Serial.print(msg);
  int httpCode=http.GET();
  Serial.print(" ");Serial.println(httpCode);
  http.end();
  msg=vacio;
  return httpCode;
}

int ICACHE_FLASH_ATTR getMyIP()
{
  if (!WiFi.isConnected()) return 0;
  msg=vacio;
  printP(barra);
  HTTPClient http;
  Serial.print("hostmyip:"); Serial.println(conf.hostmyip);
  http.begin(conf.hostmyip, 80, msg);
  http.setConnectTimeout(conf.timeoutNTP);
  Serial.print("getMyIP:");Serial.print("host:");Serial.print(conf.hostmyip);Serial.print(80);Serial.print(":");Serial.print(msg);
  int httpCode=http.GET();
  Serial.print(" ");Serial.println(httpCode);
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) { msg=http.getString(); msg.toCharArray(conf.myippub, msg.length());  } }
  http.end();
  msg=vacio;
  return httpCode;
}

int ICACHE_FLASH_ATTR checkMyIP()
{
  char auxip[16];
  strcpy(auxip, conf.myippub);
  getMyIP();    // busca actual y la guarda en conf.txt
  if (strcmp(conf.myippub, auxip) != 0) // son diferentes
    {
    saveconf();
    ifttttrigger(conucochar, conf.iftttkey, conf.aliasdevice, "NewIP", conf.myippub);
    }
}

void ICACHE_FLASH_ATTR setactivarem(byte num, boolean estado)
{
  actirem[num] = estado;
  for (byte j=0; j<maxsalrem; j++) if (conf.idsalremote[j]==conf.idremote[num]) actisenal[j] = estado;
}

void onoffremote(byte idremote, byte senalremote, byte value)
{
  char auxT[30]="";   // topic
  char auxP[10]="";   // payload
  strcpy(auxT,"conuco/ASR/");
  strcat(auxT, itoa(idremote,buff,10));         // iddevice remoto   
  strcat(auxT,"/s");
  strcat(auxT, itoa(senalremote,buff,10));   // pin remoto
  strcat(auxT, "/set");   
  strcpy(auxP,value==1?"1":"0"); 
  PSclient.publish(auxT, auxP);   // topic, payload
}

void ICACHE_FLASH_ATTR onescena(byte nesc)
{
  int auxerr;
  for (byte j = 0; j < maxSD; j++) //  Salidas digitales locales
    if (getbit8(conf.bescenaact[nesc], j))
      pinVAL(sdPin[j], getbit8(conf.bescena[nesc], j), conf.iddevice);
  for (byte i = 0; i < maxsalrem; i++) // para cada salida remota
    {
    onescenaact=true;
    if (conf.idsalremote[i]>0)
      if (conf.senalrem[i]>=12)
        if (getbit8(conf.bescenaact[nesc],i+2)==1)
          {
          byte auxest=getbit8(conf.bescena[nesc], i+2);
          onoffremote(conf.idsalremote[i],conf.senalrem[i]-12,auxest);
          }
    }
}

void HtmlGetStateOut(byte tipo, byte ind)
{
  if (tipo==0) colorea=(getbit8(conf.MbC8, ind)==1);
  else colorea=(getbit8(conf.MbC8gpio, ind)==1);
  unsigned long segundos=0;
  if (tipo==0)
    {
    segundos = (millis()/1000)-(colorea?tempact[ind]:tempdes[ind]);
    printP(colorea?th:td, href_i,comilla, colorea?off:on,interr, letrap);
    printP(ig);
    printI(ind);
    printP(amper,letran,ig);
    printI(conf.iddevice);
    printP(comilla, mayor,readdescr(filedesclocal,ind+12,20),href_f,colorea?th_f:td_f,td);
    }
  else
    {
    segundos = (millis()/1000)-(colorea?tempactgpio[ind]:tempdesgpio[ind]);
    printP(colorea?th:td, href_i,comilla, colorea?off:on,interr, letrap);
    printP(ig);
    printI(ind+10);
    printP(amper,letran,ig);
    printI(conf.iddevice);
    printP(comilla, mayor,readdescr(filedescgpio,ind,20),href_f,colorea?th_f:td_f,td);
    }
  printtiempo(segundos);
  printP(td_f);
}

void ICACHE_FLASH_ATTR onCmd()
{
  if (autOK())
    if (server.args() > 0)
      {
      byte auxi=server.arg(0).toInt();
      if (auxi<20)
        {
        pinVAL(auxi, 1, server.arg(1).toInt());   // tercer parámetro es el Id ori que lo pide
   //     HtmlGetStateOut(0, auxi);
        }
      else if (auxi <= 200)
        onescena(auxi - 100);
      }
  sendOther(panelhtm, panelact);
}

void ICACHE_FLASH_ATTR offCmd()
{
  if (autOK())
    if (server.args() > 0)
      {
      byte auxi=server.arg(0).toInt();
      if (auxi<20)
        pinVAL(auxi, 0, server.arg(1).toInt());
  //    HtmlGetStateOut(0, auxi);
      }
  sendOther(panelhtm, panelact);
}

void ICACHE_FLASH_ATTR htmlNotFound()
{
  msg=vacio;
  printP(c(HTTP11), b);
  printP(c(t404), b);
  server.send(404, textplain, msg);
  msg=vacio;
}

void procesaeventos()
{
  for (byte i=0; i<nEVE; i++) // número de condición, de 0 a 7
    {
    if (((conf.actPrg[0]) && (getbit8(conf.bPRGeve[i], 0))) ||
        ((conf.actPrg[1]) && (getbit8(conf.bPRGeve[i], 1))) ||
        ((conf.actPrg[2]) && (getbit8(conf.bPRGeve[i], 2))) ||
        ((conf.actPrg[3]) && (getbit8(conf.bPRGeve[i], 3))) ||
        ((conf.actPrg[4]) && (getbit8(conf.bPRGeve[i], 4))) ||
        ((conf.actPrg[5]) && (getbit8(conf.bPRGeve[i], 5))) ||
        ((conf.actPrg[6]) && (getbit8(conf.bPRGeve[i], 6))) ||
        ((conf.actPrg[7]) && (getbit8(conf.bPRGeve[i], 7))) ) // si algún programa activo en la condición
      {
      if (((conf.condact[i]>=8) && (conf.condact[i]<=19)) ||  // activadora entrada o salida digital local (10-21)
          ((conf.condact[i]>=100) && (conf.condact[i]<=149)))
        {
        boolean cumple=false;
        if (conf.condact[i]<=11) cumple=(getbit8(conf.MbC8, conf.condact[i]+10)==conf.condvalD[i]);
        if ((conf.condact[i]>=12) && (conf.condact[i]<=19)) cumple=(valorpin[getbit8(conf.MbC8,conf.condact[i]-12)]==conf.condvalD[i]);
        else if (conf.condact[i]>=100) cumple=(getbit8(bstatremote,conf.condact[i]-100)==conf.condvalD[i]);
        if (cumple)                   // si valor pin = valor en condición // CUMPLE
          {
          if (conf.evensal[i]<=7)         // señal local 
            {
            pinVAL(conf.evensal[i], getbit8(conf.bevenniv,i),conf.iddevice);  // señal local
            }
          if (conf.evensal[i]<=15)         // escena
            {
            onescena(conf.evensal[i]-8);
            }
          else if (conf.evensal[i] <= 200)  // señal remota
            {
            if (getbit8(conf.bconactmode,i)==0) // modo ESTADO
              {
              if (getbit8(bstatremote,conf.evensal[i]-100) != getbit8(conf.bevenniv,i))       // si el estado es diferente y hay que cambiarlo
                {
                onoffremote(conf.idsalremote[conf.evensal[i]-100], conf.senalrem[conf.evensal[i]-100]-12, getbit8(conf.bevenniv,i)); 
                }
              }
            else                             // modo CAMBIO
              {
//              if (getbit8(bconactcumple, i) == 0) // antes no cumplía
              }
            }
          else if (conf.evensal[i]==despIFTTT) // mandar notificación
            {
            if (conf.iftttenabled) 
              if (getbit8(bevenENABLE[0],i)==1)
                {
                Serial.println("IFTTT dig sent");
//                if (ifttttrigger(conucochar, conf.iftttkey, conf.aliasdevice, itoa(conf.condact[i],buff,10), "testdig")==200)
//                  { setbit8(bevenENABLE[0],i,0);  }
                }
            }
          setbit8(bconactcumple, i, 1);
          }  // fin CUMPLE
        else    // no CUMPLE
          {
          setbit8(bconactcumple, i, 0);
          }     // fin no CUMPLE
        }   // fin activadora digital Local
////////************************************************************///////////        
      else if ((conf.condact[i]<=7) ||                               // activadora temperatura local  0..7
              ((conf.condact[i]>=200) && (conf.condact[i]<=249)))    // activadora temperatura remota  200..249
        {
        float vact,tcomp;
        if (conf.condact[i]<=7)                // temperaturas locales 
          vact = float(MbR[conf.condact[i]])/100; 
        else if (((conf.condact[i]>=200) && (conf.condact[i]<=249))) // valores remotos
          {
          if (conf.senalrem[conf.condact[i]-200]<=7)       //  temperaturas remotas
            {
            vact = sondaremote[conf.condact[i]-200]; 
            }
          }
        if (conf.evenbaseA[i]<8)
          tcomp = (float(MbR[conf.evenbaseA[i]]))/100 +conf.evenvalA[i];
        else 
          tcomp = float(conf.evenvalA[i]);
        if (((conf.evencomp[i]==0) && (vact>=tcomp)) || ((conf.evencomp[i] != 0) && (vact <= tcomp)))
          { // CUMPLE
          if (conf.evensal[i]<=7)         // señal local 
            {
            pinVAL(conf.evensal[i], getbit8(conf.bevenniv,i),conf.iddevice);  // señal local
            }
          else if (conf.evensal[i]<=15)      // escena
            {
            onescena(conf.evensal[i]-8);
            }
          else if (conf.evensal[i] <= 200)   // señal remota
            {
            if (getbit8(conf.bconactmode,i)==0) // modo ESTADO
              {
              if (getbit8(bstatremote,conf.evensal[i]-100) != getbit8(conf.bevenniv,i))       // si el estado es diferente y hay que cambiarlo
                {
                onoffremote(conf.idsalremote[conf.evensal[i]-100], conf.senalrem[conf.evensal[i]-100]-12, getbit8(conf.bevenniv,i)); 
                }
              }
            else                             // modo CAMBIO
              {
//              if (getbit8(bconactcumple, i) == 0) // antes no cumplía
//                pinVAL(sdPin[conf.evensal[i]], getbit8(conf.bevenniv, i), conf.iddevice); // señal local
              }
            }
          else if (conf.evensal[i]==despIFTTT) // mandar notificación
            {
            if (conf.iftttenabled) 
              if (getbit8(bevenENABLE[0],i)==1)
                {
                Serial.println("IFTTT dig sent");
//                if (ifttttrigger(conucochar, conf.iftttkey, conf.aliasdevice, itoa(conf.condact[i],buff,10), "testdig")==200)
//                  { setbit8(bevenENABLE[0],i,0);  }
                }
            }
          setbit8(bconactcumple, i, 1);
          }
        else
          { // NO CUMPLE
          setbit8(bconactcumple, i, 0);
          }
        }   // fin activadora sonda
      }
    }
}

void ICACHE_FLASH_ATTR termostatoHTML() {
  printremote();
  boolean conectado = autOK();
  msg=vacio;
  if (server.args() > 0)
    {
    if (server.argName(0)=="up") {
      conf.evenvalA[0] = constrain(conf.evenvalA[0] + 0.5, 5, 35);
      conf.evenvalA[1] = conf.evenvalA[0] + 1.0;
      }
    else if (server.argName(0)=="dw") {
      conf.evenvalA[0] = constrain(conf.evenvalA[0] - 0.5, 5, 35);
      conf.evenvalA[1] = conf.evenvalA[0] + 1.0;
      }
    saveconf();
    procesaeventos();
    sendOther(termhtm,-1);
    return;
    }

  writeHeader(false,false);
  printP(c(body_i), menor, barra);
  printP(table, mayor);
  printP(menor, table, b);
  printP(c(tclass), ig, tmenu);
  printP(mayor);

  // TEMPERATURA
  byte val=getbit8(conf.MbC8,0);
  printP(tr);
  printColspan(2);
  pc(center_i);
  printP(t(Modo), b);
  pt(termostato);
  printP(c(center_f), td_f);
  printP(td, c(center_i));
  printP(c(fontsize6), val == 0 ? "☼" : "☀");
  pc(font_f);
  pc(center_f);

  printP(td_f, tr_f, tr);
  printColspan(3);
  pc(center_i);
  printP(c(fontsize_i), ig,siete, mayor);
  printF(MbR[0] * 0.01, 2);
  pc(font_f);
  printP(c(fontsize_i), ig,cinco, mayor, b, celsius);
  pc(font_f);
  printP(c(center_f), td_f, tr_f);
  // SALIDA DIGITAL

  printP(tr, td, c(center_i));
  printP(href_i, comillas, termhtm);
  printP(interr, letrad, letraw, ig, uno, comillas);
  printP(href_f);
  printP(c(fontsize_i), ig,cinco, mayor);
  printP(downs);
  pc(font_f);
  pc(center_f);
  printP(td_f, td, c(center_i));
  printP(c(fontsize_i), ig,tres, mayor);
  printF(conf.evenvalA[0], 1);
  printP(c(font_f), b, celsius);
  printP(c(center_f), td_f);
  printP(td, c(center_i));
  printP(href_i, comillas, termhtm);
  printP(interr, letrau, letrap, ig, uno, comillas);
  printP(href_f, c(fontsize_i), ig,cinco, mayor);
  printP(ups);
  pc(font_f);
  printP(c(center_f), td_f, tr_f);

  // final
  printP(tr);
  printColspan(2);
  printI(day()); printPiP(barra, month(), barra); printI(year());
  printP(b);
  printhora();
  printP(td_f, td, href_i, comillas);
  printP(barra, panelhtm, comillas, href_f, c(panel), td_f);
  printP(tr_f, menor, barra, table, mayor);
  printP(c(body_f), menor, barra);
  printP(thtml, mayor);
  serversend200();
  onescenaact = false;
}

void HtmlGetStateTemp(byte tipo, byte i)
{
  if (conf.modofi==1)   // modo bomba de calor
    {
    printP(td,b, AiText[i], td_f, td);
    printI(Ai[i]);
    printP(b, celsius, td_f);
    }
  else
    {
    if (tipo==0)
      {
      printP(td,b, readdescr(filedesclocal,i,20), td_f, td);
      printF(MbR[i]*0.01,1);
      printP(b, celsius, td_f);
      }
    else if (tipo==1)         // PT1000 o NTC
      {
      if (conf.gpiosensortype[i]==4)    // DHT
          {
          printP(td,b, readdescr(filedescgpio,i,20), td_f, td);
          printF(dhtdata[i][0],2);    // temperatura
          printP(b,celsius,barra);
          printF(dhtdata[i][1],2);    // humedad
          printP(b,porcen,td_f);
          }
        else if ((conf.gpiosensortype[i]==5) || (conf.gpiosensortype[i]==6))   // PT1000 o NTC
          {
          printP(td,b, readdescr(filedescgpio,i,20), td_f, td);
          printF(MbRgpio[i]+conf.gpiogamma[i],2);
          printP(b, celsius, td_f);
          sumMbRgpio[i]=0; avrcount[i]=0;
          }
      }
    }
}

void HtmlGetStateGPIO(byte i)
{
  if (conf.modofi==1) 
    {
    printP(td);
    printP(b, readdescr(filedescgpio,i,20), td_f, td);
    if (conf.gpiosensortype[i]==2)   // ADC estándar
      {
      printF(0.01*(MbRgpio[i]*MbRgpio[i]*conf.gpioalfa[i]+MbRgpio[i]*conf.gpiobeta[i]+conf.gpiogamma[i]),2);
      }
    else if (conf.gpiosensortype[i]==7)   // ACS712 corriente
      {
      printF((MbRgpio[i]+conf.gpiogamma[i]),2);
      }
    else
      {
      printF(0.01*(MbRgpio[i]*MbRgpio[i]*conf.gpioalfa[i]+MbRgpio[i]*conf.gpiobeta[i]+conf.gpiogamma[i]),2);
      }
    printP(b, td_f);
    }
  else
    {
    printP(td);
    printP(b, readdescr(filedescgpio,i,20), td_f, td);
    if (conf.gpiosensortype[i]==2)   // ADC estándar
      {
      printF(0.01*(MbRgpio[i]*MbRgpio[i]*conf.gpioalfa[i]+MbRgpio[i]*conf.gpiobeta[i]+conf.gpiogamma[i]),2);
      }
    else if (conf.gpiosensortype[i]==7)   // ACS712 corriente
      {
      printF((MbRgpio[i]+conf.gpiogamma[i]),2);
      }
    else
      {
      printF(0.01*(MbRgpio[i]*MbRgpio[i]*conf.gpioalfa[i]+MbRgpio[i]*conf.gpiobeta[i]+conf.gpiogamma[i]),2);
      }
    printP(b, td_f);
    }
}

void HtmlGetStateIn(byte tipo, byte ind)    // tipos: 0=ED, 1=GPIO como ED
{
  if (tipo==0) colorea=(getbit8(conf.MbC8,ind+8)==1);
  else colorea=(getbit8(conf.MbC8gpio,ind+16)==1);
  printP(colorea?th:td,c(resetcontp));
  printI(tipo==0?ind:ind+10);
  printP(comillas,mayor);
  if (tipo==0) printP(readdescr(filedesclocal,ind+8,20));
  else printP(readdescr(filedescgpio,ind,20));
  printP(href_f,colorea?th_f:td_f);
  cell(tipo==0?conf.contadores[ind]:conf.contadoresgpio[ind]);
}

void handleStateTime() { msg=vacio; HtmlGetStateTime(); serversend200();  }

void handleStateTemp(byte tipo, int ind) { msg=vacio; HtmlGetStateTemp(tipo, ind); serversend200();  }
void handleStateTemp0() { handleStateTemp(0,0); }
void handleStateTemp1() { handleStateTemp(0,1); }
void handleStateTemp2() { handleStateTemp(0,2); }
void handleStateTemp3() { handleStateTemp(0,3); }
void handleStateTemp4() { handleStateTemp(0,4); }
void handleStateTemp5() { handleStateTemp(0,5); }
void handleStateTemp6() { handleStateTemp(0,6); }
void handleStateTemp7() { handleStateTemp(0,7); }

void handleStateTemp0g() { handleStateTemp(1,0); }
void handleStateTemp1g() { handleStateTemp(1,1); }
void handleStateTemp2g() { handleStateTemp(1,2); }
void handleStateTemp3g() { handleStateTemp(1,3); }
void handleStateTemp4g() { handleStateTemp(1,4); }
void handleStateTemp5g() { handleStateTemp(1,5); }
void handleStateTemp6g() { handleStateTemp(1,6); }
void handleStateTemp7g() { handleStateTemp(1,7); }
void handleStateTemp8g() { handleStateTemp(1,8); }
void handleStateTemp9g() { handleStateTemp(1,9); }

void handleStateIn(byte tipo, int ind) { msg=vacio; HtmlGetStateIn(tipo,ind); serversend200();  }
void handleState0In() { handleStateIn(0,0); }
void handleState1In() { handleStateIn(0,1); }
void handleState2In() { handleStateIn(0,2); }
void handleState3In() { handleStateIn(0,3); }

void handleState0Ing() { handleStateIn(1,0); }
void handleState1Ing() { handleStateIn(1,1); }
void handleState2Ing() { handleStateIn(1,2); }
void handleState3Ing() { handleStateIn(1,3); }
void handleState4Ing() { handleStateIn(1,4); }
void handleState5Ing() { handleStateIn(1,5); }
void handleState6Ing() { handleStateIn(1,6); }
void handleState7Ing() { handleStateIn(1,7); }
void handleState8Ing() { handleStateIn(1,8); }
void handleState9Ing() { handleStateIn(1,9); }

void handleStateOut(byte tipo, int ind) { msg=vacio; HtmlGetStateOut(tipo,ind); serversend200();  }
void handleState0Out() { handleStateOut(0,0); }
void handleState1Out() { handleStateOut(0,1); }
void handleState2Out() { handleStateOut(0,2); }
void handleState3Out() { handleStateOut(0,3); }
void handleState4Out() { handleStateOut(0,4); }
void handleState5Out() { handleStateOut(0,5); }
void handleState6Out() { handleStateOut(0,6); }
void handleState7Out() { handleStateOut(0,7); }

void handleState0Outg() { handleStateOut(1,0); }
void handleState1Outg() { handleStateOut(1,1); }
void handleState2Outg() { handleStateOut(1,2); }
void handleState3Outg() { handleStateOut(1,3); }
void handleState4Outg() { handleStateOut(1,4); }
void handleState5Outg() { handleStateOut(1,5); }
void handleState6Outg() { handleStateOut(1,6); }
void handleState7Outg() { handleStateOut(1,7); }
void handleState8Outg() { handleStateOut(1,8); }
void handleState9Outg() { handleStateOut(1,9); }

void printLastTimeR(byte i)
{
  unsigned long auxL=(millis()/1000)-lasttimeremote[i];
  if (auxL>60)
    {
    printP(b,paren_i);
    if (auxL<60) { printL(auxL); printP(comillas); }            // en segundos
    else if (auxL<3600) { printL(auxL/60); printP(comilla); }   // en minutos
    else if (auxL<86400) { printL(auxL/3600); printP(letrah); } // en horas
    else  { printL(auxL/86400); printP(letrad); }               // en días
    printP(paren_f);
    }
  printP(td_f);
}

void HtmlGetStateR(byte srem)
{
  if (conf.senalrem[srem]<=7)   // temperaturas
    { 
    printP(td,b, readdescr(filesalrem,srem,20), td_f, td);
    printF(sondaremote[srem],1);
    printP(b, celsius); 
    printLastTimeR(srem);
    printP(td_f);
    }
  else if (conf.senalrem[srem]<=19)    // entradas o salidas
    {
    colorea=getbit8(bstatremote, srem);
    unsigned long segundos = (millis()/1000)-(contaremote[srem]);
    printP(colorea?th:td);
      if (conf.showN) { printparentesis(letrar,srem+1); printparentesis(letraR,conf.idsalremote[srem]); }
    printP(href_i, comilla);
    printP(syhtm, interr, letrar, ig);
    printI(srem);
    printP(amper, colorea?c(roff):c(ron), ig, cero);
    printP(comilla, mayor, readdescr(filesalrem, srem, 20));
    printP(href_f, colorea?th_f:td_f, td);
    if (conf.senalrem[srem]<=11)
      printI(contaremote[srem]);
    else
      printtiempo(timeremote[srem]);
    printLastTimeR(srem);
    printP(td_f);
    }
}

void handleStater(int i) {  msg=vacio; HtmlGetStateR(i); serversend200(); }
void handleStater0() { handleStater(0); }
void handleStater1() { handleStater(1); }
void handleStater2() { handleStater(2); }
void handleStater3() { handleStater(3); }
void handleStater4() { handleStater(4); }
void handleStater5() { handleStater(5); }
void handleStater6() { handleStater(6); }
void handleStater7() { handleStater(7); }
void handleStater8() { handleStater(8); }
void handleStater9() { handleStater(9); }
void handleStater10() { handleStater(10); }
void handleStater11() { handleStater(11); }
void handleStater12() { handleStater(12); }
void handleStater13() { handleStater(13); }
void handleStater14() { handleStater(14); }
void handleStater15() { handleStater(15); }
void handleStater16() { handleStater(16); }
void handleStater17() { handleStater(17); }
void handleStater18() { handleStater(18); }
void handleStater19() { handleStater(19); }
void handleStater20() { handleStater(20); }
void handleStater21() { handleStater(21); }
void handleStater22() { handleStater(22); }
void handleStater23() { handleStater(23); }
void handleStater24() { handleStater(24); }
void handleStater25() { handleStater(25); }
void handleStater26() { handleStater(26); }
void handleStater27() { handleStater(26); }
void handleStater28() { handleStater(28); }
void handleStater29() { handleStater(29); }
void handleStater30() { handleStater(30); }
void handleStater31() { handleStater(31); }

void voicecommandHTML()
{
  printremote();
  boolean conectado = (autOK());
  msg =vacio;
  if (server.args()==2)   // parámetro 1, a=0/1 apagar/encender, parámetro 2, t= nombre señal o escena
    {
    if ((server.argName(0).compareTo(letraa)==0) && (server.argName(1).compareTo(letrat)==0))
      {
      server.arg(1).toLowerCase();
      server.arg(1).toCharArray(auxchar, sizeof(auxchar));
      byte i=6;  boolean encontrado = false; // buscar en salidas locales
      while ((i<=7) && (!encontrado))
        {
        msg=vacio;
        printP(readdescr(filedesclocal, i++, 20));
        encontrado=strcharcomp();
        }
      if (encontrado) pinVAL(i+5, server.arg(0).toInt(), conf.iddevice);
      
      i=0; encontrado=false; // buscar en salidas remotas
      while ((i<=maxsalrem) && (!encontrado))
        {
        msg=vacio;
        printP(readdescr(filesalrem, i++, 20));
        encontrado=strcharcomp();
        }
      if (encontrado)
        if (conf.senalrem[i-1]>=6) pinvalR(conf.idsalremote[i-1],conf.webPort,conf.senalrem[i-1]-6, server.arg(0).toInt());

      i=0; encontrado=false; // buscar en escenas
      while ((i<=maxEsc) && (!encontrado))
        {
        msg=vacio;
        printP(readdescr(filedescesc, i++, 20));
        encontrado=strcharcomp();
        }
      if (encontrado) onescena(i-1);
      }
    }
  msg=vacio;
  serversend200();
}

void ICACHE_FLASH_ATTR panelnoHTML() {
  printremote();
  boolean conectado = (autOK());
  msg=vacio;
  if (server.method()==HTTP_POST) return; 
  writeHeader(false,true);
  byte auxI=server.arg(0).toInt();
  panelact=auxI;
  writeMenu(1, auxI);
  printP(menor, table);
  printP(b, c(tclass), ig, tpanel, mayor, tr);
  printColspan(2);
  printP(readdescr(filezonas, auxI, 20), td_f, tr_f);

  /////////////  CONTENIDO   ///////////
  if (conectado)
    {
    // ESCENAS
    for (byte i=0; i<maxEsc; i++)
      if (getbit8(conf.bshowEsc, i))
        {
        printP(tr);
        printColspan(2);
        printP(href_i);
        printPiP(hrefon, i+100, comillas);
        printP(c(colorffffcc), mayor);
        printP(readdescr(filedescesc, i, 20), href_f, td_f, tr_f);
        }
    }
  if (conectado)    // webcalls
    for (byte i = 0; i < maxwebcalls; i++)
      if (getbit8(conf.bPRGwebcall, i) == 1)
        {
        printP(tr);
        printColspan(2);
        printP(href_i, comillas, syhtm, interr);
        printP(c(actwc), amper);
        printP(letran);
        printPiP(ig, i, comillas);
        printP(c(colorffffcc), mayor);
        printP(readdescr(filewebcall, i, 20), href_f, td_f, tr_f);
        }

  // TEMPERATURAS DS18B20 Locales
  for (byte i=0; i<maxTemp; i++)
    if (getbit8(conf.bshowbypanel[auxI],i))
      {
      printP(menor,letrat,letrar,b);
      printP(c(tid),ig,comilla,letrat,letrae);
      printI(i);
      printP(comilla,mayor);
      HtmlGetStateTemp(0,i);
      printP(tr_f);
      }

  for (byte i=0; i<maxsalrem; i++) // TEMPERATURAS DS18B20 remotas
    {
    if (getbit8(conf.bshowbypanel[auxI], i+30))
      {
      if ((conf.idsalremote[i] >= 150) && (conf.idsalremote[i] <= 166) && (conf.senalrem[i] < 8)) // sonda de remoto conuco
        {
        printP(tr);
        printP(menor,letrat,letrar,b);
        printP(c(tid),ig,comilla,letrar);
        printI(i);
        printP(comilla,mayor);
        HtmlGetStateR(i);
        printP(tr_f);
        }
      }
    }

  for (byte i=0;i<maxsalrem; i++) // Sensores remotos Modbus o I2C
    {
    if (getbit8(conf.bshowbypanel[auxI], i+30))
      {
      if ((conf.idsalremote[i]>0) && ((conf.idsalremote[i]<=149) || (conf.idsalremote[i]>=167))) // modbus o I2C
        {
        if (posrem(conf.idsalremote[i])>0)
          {
          if (conf.tipoi2cmbrem[posrem(conf.idsalremote[i])-1]==1)  // BMP085
            {
            printP(tr, td);
            if (conf.showN) { printparentesis(letrar, i+1); printparentesis(letraR, conf.idsalremote[i]);  }
            printP(b, readdescr(filesalrem, i, 20), td_f, td);
            if (bmp085enabled) printF(bmp085.readTemperature(),2);
            printP(b, celsius, b,barraesp);
            if (bmp085enabled) printF(bmp085.readPressure()/100,0);
            printP(b, letram, letrab, td_f, tr_f);
            }
          if (conf.tipoi2cmbrem[posrem(conf.idsalremote[i])-1]==7)   // T-32-P
            {
            printP(tr, getbit8(mbstatus, i) == 1 ? th : td);
            if (conf.showN) { printparentesis(letrar, i+1); printparentesis(letraR, conf.idsalremote[i]);   }
            printP(b, readdescr(filesalrem, i, 20), getbit8(mbstatus,i)==1?th_f:td_f,td);
            printF(mbtemp[i] * 0.01, 2);
            printP(barra);
            printF(mbcons[i]*0.01,2);
            printP(b,celsius,td_f, tr_f);
            }
          }
        }
      }
    }

  // ENTRADAS DIGITALES
  for (byte i=0; i<maxED; i++)
    if (getbit8(conf.bshowbypanel[auxI], i+8))
      {
      printP(menor, letrat, letrar, b);
      printP(c(tid), ig, comilla, letral); 
      printI(i);
      printP(comilla, mayor);
      HtmlGetStateIn(0,i);
      printP(tr_f);
      }

  for (byte i=0; i<maxsalrem; i++) // entradas digitales remotas
    {
    if (getbit8(conf.bshowbypanel[auxI], i+30))
      {
      if ((conf.senalrem[i]>=8) && (conf.senalrem[i]<=11))
        {
        printP(tr);
        byte val=getbit8(bstatremote,i);
        if (tipoedremote[i]<=1)
          {
          printP((val==0)?td:th);
          if (conf.showN) { printparentesis(letrar,i+1); printparentesis(letraR,conf.idsalremote[i]); }
          if (!actisenal[i]) printP(aster, b);
          printP(readdescr(filesalrem,i,20),(val==0)?td_f:th_f);
          cell(contaremote[i]);
          }
        else if (tipoedremote[i]==2)
          {
          printP(td,readdescr(filesalrem,i,20),td_f,td);
          printF(datosremoto.dhtdata[conf.senalrem[i]-4][0],2);
          printP(celsius,barra,b);
          printF(datosremoto.dhtdata[conf.senalrem[i]-4][1],2);
          printP(b,porcen,td_f);
          }
        printP(tr_f);
        }
      }
    }

  // SALIDAS DIGITALES
  if (conectado)
    {
    for (byte i=0;i<maxSD;i++)
      if (getbit8(conf.bshowbypanel[auxI], i+12))
        {
        printP(menor, letrat, letrar, b);
        printP(c(tid), ig, comilla, letral); 
        printI(i+4);
        printP(comilla, mayor);
        HtmlGetStateOut(0,i);
        printP(tr_f);
        }
    }

  for (byte i=0; i<maxsalrem; i++) // Salidas digitales remotas
    {
    if (getbit8(conf.bshowbypanel[auxI], i+30))
      {
      if (conf.senalrem[i] >= 12)
        {
        byte val=getbit8(bstatremote, i);
        printP(menor,letrat,letrar,b);
        printP(c(tid), ig, comilla, letrar); 
        printI(i);
        printP(comilla, mayor);
        HtmlGetStateR(i);
        printP(tr_f);
        }
      }
    }

   for (byte i=0;i<maxgpiovar;i++)    // GPIOs
     {
      if (gpiovis(i))
        if (getbit8(conf.bshowbypanel[auxI], i+20))
          {
          printP(tr);
          if (conf.gpiosensortype[i]==0)    // input
            {
            printP(menor, letrat, letrar, b);
            printP(c(tid), ig, comilla, letrag, letrai); 
            printI(i);      // número de la etiqueta "#ln"
            printP(comilla, mayor);
            HtmlGetStateIn(1,i);
            printP(tr_f);
            }
          else if (conf.gpiosensortype[i]==1)   // output
            {
            printP(menor, letrat, letrar, b);
            printP(c(tid), ig, comilla, letrag, letrao); 
            printI(i);
            printP(comilla, mayor);
            HtmlGetStateOut(1,i);
            printP(tr_f);
            }
          else if (conf.gpiosensortype[i]==2)   // ADC
            {
            printP(menor, letrat, letrar, b);
            printP(c(tid), ig, comilla, letral); 
            printI(i);
            printP(comilla, mayor);
            HtmlGetStateGPIO(i);
            printP(tr_f);
            }
          else if (conf.gpiosensortype[i]==3)   // DAC
            {
            printP(menor, letrat, letrar, b);
            printP(c(tid), ig, comilla, letral); 
            printI(i);
            printP(comilla, mayor);
            HtmlGetStateOut(1,i);
            printP(tr_f);
            }
          else if (conf.gpiosensortype[i]==4)    // DHT 
            {
            printP(menor, letrat, letrar, b);
            printP(c(tid), ig, comilla, letrag,letrae); 
            printI(i);
            printP(comilla, mayor);
            HtmlGetStateTemp(1, i);
            printP(tr_f);
            }
          else if ((conf.gpiosensortype[i]==5) || (conf.gpiosensortype[i]==6))   // PT1000 A99 o NTC10K Carel
            {
            printP(menor, letrat, letrar, b);
            printP(c(tid), ig, comilla, letrag,letrae); 
            printI(i);
            printP(comilla, mayor);
            HtmlGetStateTemp(1, i);
            printP(tr_f);
            }
          else if (conf.gpiosensortype[i]==7)    // ACS712 corriente 
            {
            printP(menor, letrat, letrar, b);
            printP(c(tid), ig, comilla, letrag,letrae); 
            printI(i);
            printP(comilla, mayor);
            HtmlGetStateGPIO(i);
            printP(tr_f);
            }
          else
            {
            printP(td,readdescr(filedescgpio,i,20),b);
            printP(sensortype[conf.gpiosensortype[i]]); 
            printP(b); printI(conf.gpioalfa[i]);
            printP(b); printI(conf.gpiobeta[i]);
            printP(b); printI(conf.gpiogamma[i]);
            printP(td_f,tr_f);
            }
          }
     }

  // final
  printP(menor,letrat,letrar,b,c(tid));
  printP(ig,comilla,letrat,letrat,comilla,mayor);
  HtmlGetStateTime();
  printP(tr_f, menor, barra, table, mayor);
  printP(c(body_f), menor, barra,thtml, mayor);
  serversend200();
}

void ICACHE_FLASH_ATTR panelbcHTML() {
  printremote();
  boolean conectado = (autOK());
  msg=vacio;
  if (server.method()==HTTP_POST) return; 
  writeHeader(false,true);
  byte auxI=server.arg(0).toInt();
  panelact=auxI;
  writeMenu(1, auxI);
  printP(menor, table);
  printP(b, c(tclass), ig, tpanel, mayor, tr);
  printColspan(2);
  printP(readdescr(filezonas, auxI, 20), td_f, tr_f);

  /////////////  CONTENIDO   ///////////

  // TEMPERATURAS Ai1 .. Ai4
  for (byte i=0; i<4; i++)
    {
    printP(menor,letrat,letrar,b);
    printP(c(tid),ig,comilla,letrat,letrae);
    printI(i);
    printP(comilla,mayor);
    HtmlGetStateTemp(0,i);
    printP(tr_f);
    }

     // ENTRADAS DIGITALES
/** for (byte i=0; i<4; i++)
    {
    printP(menor, letrat, letrar, b);
    printP(c(tid), ig, comilla, letral); 
    printI(i);
    printP(comilla, mayor);
    HtmlGetStateIn(0,i);
    printP(tr_f);
    }**/

for (byte i=0;i<maxgpiovar;i++)    // GPIOs
     {
      if (gpiovis(i))
        {
        printP(tr);
        if (conf.gpiosensortype[i]==0)    // input
          {
          printP(menor, letrat, letrar, b);
          printP(c(tid), ig, comilla, letrag, letrai); 
          printI(i);      // número de la etiqueta "#ln"
          printP(comilla, mayor);
          HtmlGetStateIn(1,i);
          printP(tr_f);
          }
        }
     }

  // SALIDAS DIGITALES
  if (conectado)
    {
    for (byte i=0;i<maxSD;i++)
      if (getbit8(conf.bshowbypanel[auxI], i+12))
        {
        printP(menor, "tr", b);
        printP(c(tid), ig, comilla, letral); 
        printI(i+4);
        printP(comilla, mayor);
        HtmlGetStateOut(0,i);
        printP(tr_f);
        }
    }


  // final
  printP(menor,letrat,letrar,b,c(tid));
  printP(ig,comilla,letrat,letrat,comilla,mayor);
  HtmlGetStateTime();
  printP(tr_f, menor, barra, table, mayor);
  printP(c(body_f), menor, barra,thtml, mayor);
  serversend200();
}

void ICACHE_FLASH_ATTR panelHTML() 
{
  if (conf.modofi==0)
    {
    if (conf.modoterm==0) panelnoHTML();  else termostatoHTML(); 
    }
  else
    panelbcHTML();
}  

void ICACHE_FLASH_ATTR indexHTML() 
  {
  if (conf.modofi==0)
    { if (conf.modoterm==0) panelHTML();  else termostatoHTML();   }
  else if (conf.modofi==1) panelbcHTML(); 
  }

void ICACHE_FLASH_ATTR AddOri(byte numori)
{
  byte i=0;
  boolean encontrado=false;
  while ((i<16) && (ListOri[i]!=numori) && (ListOri[i]!=0) && (!encontrado)) 
    { 
      encontrado=(ListOri[i]==numori); 
      i++;
    }
  if (!encontrado) 
    if (i<16) ListOri[i]=numori; 
    else ListOri[0]=numori;
}

void ICACHE_FLASH_ATTR jsonHTML() {
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  if (server.args() > 0)
    if (server.argName(0) == letrar)
      AddOri(server.arg(0).toInt());
  msg=vacio;
  buildJson();
  serversend200();
}

void ICACHE_FLASH_ATTR jsonconfHTML() {   // responde a /jc y envía jsonconf
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  if (server.args()>0)
    if (server.argName(0)==letrar) AddOri(server.arg(0).toInt());
    Serial.print("recibido /jc desde el "); Serial.print(server.arg(0).toInt());
    Serial.println("  se envía JsonConf");
  msg=vacio;
  buildJsonConf(false, server.argName(0)==letram, false);
  serversend200();
}

void ICACHE_FLASH_ATTR initPRG(void)
{
  memset(conf.actPrg,0,sizeof(conf.actPrg));        // todos los programas desactivados
  //////////////////////////////////  SEMANAL
  memset(conf.bprgval, 0, sizeof(conf.bprgval));
  memset(conf.bPRGsem, 0, sizeof(conf.bPRGsem));
  memset(conf.prgsal, 0, sizeof(conf.prgsal));
  memset(conf.prgdia, 0, sizeof(conf.prgdia));
  memset(conf.prghor, 0, sizeof(conf.prghor));
  memset(conf.prgmin, 0, sizeof(conf.prgmin));
  //////////////////////////////////// CONDICIONES
  memset(conf.bconacttipo,0,sizeof(conf.bconacttipo));
  memset(conf.condact,0,sizeof(conf.condact));
  memset(conf.bconactmode,0,sizeof(conf.bconactmode));
  memset(conf.condvalD,0,sizeof(conf.condvalD));
  memset(conf.evencomp,0,sizeof(conf.evencomp));
  memset(conf.evenvalA,0,sizeof(conf.evenvalA));
  memset(conf.evenhis,0,sizeof(conf.evenhis));
  memset(conf.bconsaltipo,0,sizeof(conf.bconsaltipo));
  memset(conf.evensal, 0, sizeof(conf.evensal));
  memset(conf.bevenniv, 0, sizeof(conf.bevenniv));
  memset(conf.bPRGeve, 0, sizeof(conf.bPRGeve));
  ///////////////////////////////////// FECHAS
  memset(conf.bactfec, 0, sizeof(conf.bactfec));
  memset(conf.fecsal, 0, sizeof(conf.fecsal));
  memset(conf.fecdia, 0, sizeof(conf.fecdia));
  memset(conf.fecmes, 0, sizeof(conf.fecmes));
  memset(conf.fechor, 0, sizeof(conf.fechor));
  memset(conf.fecmin, 0, sizeof(conf.fecmin));
  memset(conf.bfecval, 0, sizeof(conf.bfecval));
  ////////////////////////////////////// ESCENAS
  memset(conf.bshowEsc,0,sizeof(conf.bshowEsc));
  for (byte i = 0; i < maxEsc; i++) {
    strcpy(auxdesc, t(escena)); strcat(auxdesc, b); strcat(auxdesc, itoa(i+1,buff,10));
    savedescr(filedescesc, auxdesc, i, 20);
    }
  memset(conf.bescena,0,sizeof(conf.bescena));
  memset(conf.bescenaact,0,sizeof(conf.bescenaact));
  memset(conf.bPRGeve,0,sizeof(conf.bPRGeve));
  conf.bPRGwebcall[0]=0;       //  desactiva WEBCALLS
  saveconf();
}

void ICACHE_FLASH_ATTR seleccionatipoi2cmb(byte i)
{
  printP(c(Select_name),comillas);
  printIP(mpi + 3, comillas);
  printP(mayor);
  for (byte j=0; j<30; j++)  { // añade tipo de remoto I2C o Modbus
    pc(optionvalue);
    printPiP(comillas, j, comillas);
    if (i==j) printP(b, selected, ig, comillas, selected, comillas);
    printP(mayor,readdescr(filei2ctypes,j,20));
    pc(option_f);
    }
  pc(select_f);
}

void ICACHE_FLASH_ATTR extraevaloresTempConf(boolean withpass)
{                       // extrae datos de json.conf
  if (withpass)
    {
    extrae(true, msg, PSTR("ss")).toCharArray(ssidSTAtemp, 20);
    extrae(true, msg, PSTR("ps")).toCharArray(passSTAtemp, 20);
    }
  moddevicetemp = extrae(false, msg, PSTR("M")).toInt();
  iddevicetemp = extrae(false, msg, PSTR("DV")).toInt();
  versinsttemp = extrae(false, msg, PSTR("V")).toInt();
  actualizauttemp = extrae(false, msg, PSTR("AA")).toInt();
  extrae(true, msg, PSTR("AL")).toCharArray(aliasdevicetemp, 20);
  iftttenabledtemp = extrae(false, msg, PSTR("IF")).toInt();
  extrae(true, msg, PSTR("IFK")).toCharArray(iftttkeytemp, 30);
  mododweettemp = extrae(false, msg, PSTR("DW")).toInt();
  peractpantemp = extrae(false, msg, PSTR( "PAP")).toInt();
  peractremtemp = extrae(false, msg, PSTR( "PAR")).toInt();
  TempDesactPrgtemp = extrae(false, msg, PSTR("TDP")).toInt();
  iottweetenabletemp = extrae(false, msg, PSTR("IOW")).toInt();
  extrae(true, msg, PSTR("IOWU")).toCharArray(iottweetusertemp, 10);
  extrae(true, msg, PSTR("IOWP")).toCharArray(iottweetkeytemp, 15);
  latitudtemp = extrae(true, msg, PSTR("LAT")).toFloat();
  longitudtemp = extrae(true, msg, PSTR("LON")).toFloat();

  byte auxmax=moddevicetemp==8266?3:8;    // sondas temperatura
  for (byte i=0;i<auxmax;i++) 
    { extrae(true,msg,idpin[i]).toCharArray(auxdesc,20); 
      savedescr(filedesctemp,auxdesc,i,20); 
    }
  
  auxmax=moddevicetemp==8266?1:0;     // entrada analógica (sól en ESP8266)
  for (byte i=0;i<auxmax;i++) 
    {
    extrae(true, msg, idpin[i+8]).toCharArray(auxdesc, 20); 
    savedescr(filedesctemp, auxdesc, i+8, 20);
    extrae(true, msg, PSTR("au0")).toCharArray(unitpinAtemp, 4);
    }
    
  auxmax=moddevicetemp==8266?2:4;   // entradas digitales
  for (byte i=0;i<auxmax;i++) 
    { extrae(true,msg,idpin[i+8]).toCharArray(auxdesc,20); 
    savedescr(filedesctemp,auxdesc,i+8,20); }
    
  auxmax=moddevicetemp==8266?2:8;   // salidas digitales
  for (byte i=0;i<auxmax;i++) 
  { extrae(true,msg,idpin[i+12]).toCharArray(auxdesc,20); 
    savedescr(filedesctemp,auxdesc,i+12,20); }
  // analógica, solo 8266
  factorAtemp[0] = extrae(true, msg, PSTR("af0")).toFloat();
  offsetAtemp[0] = extrae(true, msg, PSTR("ao0")).toFloat();
  bsumatAtemp[0] = extrae(true, msg, PSTR("asu0")).toInt();
  tipoEDtemp[0] = extrae(true, msg, PSTR("et0")).toInt();   // tipo entrada digital
  tipoEDtemp[1] = extrae(true, msg, PSTR("et1")).toInt();
  tipoEDtemp[2] = extrae(true, msg, PSTR("et2")).toInt();
  tipoEDtemp[3] = extrae(true, msg, PSTR("et3")).toInt();
  valinictemp[0] = extrae(false, msg, PSTR( "vi0")).toInt();
  valinictemp[2] = extrae(false, msg, PSTR("vi1")).toInt();
  valinictemp[3] = extrae(false, msg, PSTR("vi2")).toInt();
  valinictemp[1] = extrae(false, msg, PSTR("vi3")).toInt();
  tempdefacttemp[0] = extrae(false, msg, PSTR("on0")).toInt();
  tempdefacttemp[1] = extrae(false, msg, PSTR("on1")).toInt();
  tempdefacttemp[2] = extrae(false, msg, PSTR("on2")).toInt();
  tempdefacttemp[3] = extrae(false, msg, PSTR("on3")).toInt();
  tempdefacttemp[4] = extrae(false, msg, PSTR("on4")).toInt();
  tempdefacttemp[5] = extrae(false, msg, PSTR("on5")).toInt();
  tempdefacttemp[6] = extrae(false, msg, PSTR("on6")).toInt();
  tempdefacttemp[7] = extrae(false, msg, PSTR("on7")).toInt();
  tempdefdestemp[0] = extrae(false, msg, PSTR("off0")).toInt();
  tempdefdestemp[1] = extrae(false, msg, PSTR("off1")).toInt();
  tempdefdestemp[2] = extrae(false, msg, PSTR("off2")).toInt();
  tempdefdestemp[3] = extrae(false, msg, PSTR("off3")).toInt();
  tempdefdestemp[4] = extrae(false, msg, PSTR("off4")).toInt();
  tempdefdestemp[5] = extrae(false, msg, PSTR("off5")).toInt();
  tempdefdestemp[6] = extrae(false, msg, PSTR("off6")).toInt();
  tempdefdestemp[7] = extrae(false, msg, PSTR("off7")).toInt();
  iftttpinEDtemp[0] = extrae(false, msg, PSTR("ife0")).toInt();
  iftttpinEDtemp[1] = extrae(false, msg, PSTR("ife1")).toInt();
  iftttpinEDtemp[2] = extrae(false, msg, PSTR("ife2")).toInt();
  iftttpinEDtemp[3] = extrae(false, msg, PSTR("ife3")).toInt();
  iftttpinSDtemp[0] = extrae(false, msg, PSTR("ifs0")).toInt();
  iftttpinSDtemp[1] = extrae(false, msg, PSTR("ifs1")).toInt();
  iftttpinSDtemp[2] = extrae(false, msg, PSTR("ifs2")).toInt();
  iftttpinSDtemp[3] = extrae(false, msg, PSTR("ifs3")).toInt();
  iftttpinSDtemp[4] = extrae(false, msg, PSTR("ifs4")).toInt();
  iftttpinSDtemp[5] = extrae(false, msg, PSTR("ifs5")).toInt();
  iftttpinSDtemp[6] = extrae(false, msg, PSTR("ifs6")).toInt();
  iftttpinSDtemp[7] = extrae(false, msg, PSTR("ifs7")).toInt();
  extrae(true, msg, PSTR("PATH")).toCharArray(mqttpathtemp, 60);
  extrae(true, msg, PSTR("FSV")).toCharArray(fwUrlBasetemp, 80);
  Serial.print("mqttpathtemp:"); Serial.println(mqttpathtemp);
  msg=vacio;
}

void addsignal(byte n)    // n de 0 a 7, es la señal del remoto que hay que añadir a ñas señales remotas del master
{
  byte i=0;
  boolean hayhueco=false;
  boolean existe=false;
  while ((i<maxsalrem) and (!hayhueco) && (!existe))
    {
    hayhueco=(conf.idsalremote[i]==0);  
    existe=((conf.idsalremote[i]==iddevicetemp) && (conf.senalrem[i]==n));
    if (!hayhueco) i++;
    }
  if (hayhueco)
    {
    conf.idsalremote[i]=iddevicetemp;  conf.senalrem[i]=n;
    readdescr(filedesctemp,n,20);  savedescr(filesalrem,auxdesc,i,20);
    }
}

void ICACHE_FLASH_ATTR mqttsubscribe(char* topic)
{ PSclient.subscribe(topic); }

void mqttsubscribevalue(byte idremote, byte i)   // subscribe a un pin remoto
{
  strcpy(auxdesc,conf.mqttpath[0]); strcat(auxdesc,"/");
  strcat(auxdesc,conf.mqttpath[1]); strcat(auxdesc,"/");
  strcat(auxdesc,itoa(idremote,buff,10)); strcat(auxdesc,"/");
  strcat(auxdesc,idpin[i]); 
  mqttsubscribe(auxdesc);
}

void mqttsubscribeconf(byte idremote, byte i)   // subscribe a un pin remoto
{
  strcpy(auxdesc,conf.mqttpath[0]); strcat(auxdesc,"/");
  strcat(auxdesc,conf.mqttpath[1]); strcat(auxdesc,"/");
  strcat(auxdesc,itoa(idremote,buff,10)); strcat(auxdesc,"/");
  strcat(auxdesc,idpinn[i]);
  mqttsubscribe(auxdesc);
}

void ICACHE_FLASH_ATTR mqttsubscriberemotevalues(byte idremote)
{
  long tini=millis();
  for (byte i=0;i<38;i++)
    {
    boolean suscribir=true;
    if (suscribir)
      {
      strcpy(auxdesc,conf.mqttpath[0]); strcat(auxdesc,"/");
      strcat(auxdesc,conf.mqttpath[1]); strcat(auxdesc,"/");
      strcat(auxdesc,itoa(idremote,buff,10)); strcat(auxdesc,"/");
      strcat(auxdesc,idpin[i]);
      mqttsubscribe(auxdesc);
      }
    }
  strcpy(auxdesc,"");
}

void subscribeconfrem()   // subscribe a datos de conf. para saber nombres y tipos de señales remotas definidas
{
  for (byte i=0;i<maxsalrem;i++)
    {
    if (conf.idsalremote[i]!=0)      
      {
      mqttsubscribeconf(conf.idsalremote[i], conf.senalrem[i]);   // subscribe a conf de un pin remoto
      }
    }
}

void subscribevaluesrem()     // subscribe a valores de señales remotas definidas
{
  for (byte i=0;i<maxsalrem;i++)
    {
    if (conf.idsalremote[i]!=0)      
      {
      mqttsubscribevalue(conf.idsalremote[i], conf.senalrem[i]);  // subscribe a un pin remoto
      if ((conf.senalrem[i]>=8) && (conf.senalrem[i]<=19))    // entradas y salidas
        {
        mqttsubscribevalue(conf.idsalremote[i], conf.senalrem[i]+23); // subscribe a veces entrada=0n o tiempo    
        }
      }
    }
}

void ICACHE_FLASH_ATTR setupremHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  mp=5; // número de parámetros por fila
  if (server.method()==HTTP_POST)
    {
    for (int i=0; i<server.args(); i++)
      {
      calcindices(i);
      if (resto==1) { conf.idremote[indice] = server.arg(i).toInt(); } // Id del dispositivo
      else if (resto==2) { conf.tiporemote[indice] = server.arg(i).toInt(); }
      else if (resto==3) { conf.tipoi2cmbrem[indice] = server.arg(i).toInt(); }
      }
    //subscribeconfrem();
    //subscribevaluesrem();
    saveconf();
//    //    initPRG();    // inicializa programaciones
    sendOther(slkhtm,-1);
    return;
    }
  // headers
  if (server.args()>0) { posactrem = constrain(server.arg(0).toInt(), 0, 15); }
  if (server.args()>1) { if (server.arg(1).toInt()==1) { /**scanremotes();**/  }  }
  writeHeader(false,false);
  writeMenu(3, 10);
  writeForm(slkhtm);

  printP(tr, td, ID,b,barraesp, c(addrt));
  printP(td_f);
  tcell(ttipo);
  ccell(tlink);
  tcell(configuracion);
  ccell(salremotas);
  printP(tr_f);
  for (byte i=0; i<maxdevrem; i++)
    {
    mpi=mp*i;
    colorea=(posactrem==i);
    printP(tr);
    if (posactrem==i)   {
      printP(colorea?th:td);
      printcampoI(mpi+1,conf.idremote[i], 3, true,false);
      printP(colorea?th_f:td_f);
      printP(td);
      printcampoCB(mpi+2, conf.tiporemote[i], tconuco,tconuco32,false);
      printP(td_f);
      }
    else
      {
      strcpy(auxchar, slkhtm); strcat(auxchar, igualp); strcat(auxchar, itoa(i, buff, 10));
      printOpc(false, false, (conf.idremote[i]==0)?c(notdefined):itoa(conf.idremote[i],buff,10), auxchar);
      printP(td);
      printI(conf.tiporemote[i]==0?8266:32);
      printP(td_f);
      }

    if (conf.idremote[i]!=0)
      {
      if ((conf.idremote[i] >= 150) && (conf.idremote[i] <= 166)) { // conuco
        printP(td);
        if (!clientremote())
          {
          printP(href_i, comillas);
          pc(thttp);
          printP(hostraiz);
          printIP(conf.netseg,punto);
          printIP(conf.idremote[i],dp);
          printI(conf.webPort);
          printP(comillas, b, c(newpage), mayor);
          pc(conuco);
          printP(href_f);
          }
        printP(td_f,td, href_i, comillas, sdremhtm, interr);
        printPiP(c(trem), conf.idremote[i], comillas);
        printP(b, c(newpage), mayor);
        printP(readdescr(filedevrem, i, 20), href_f, td_f);
        printP(td, href_i, comillas, sdremiohtm, interr);
        printPiP(c(trem), conf.idremote[i], comillas);
        printP(b, c(newpage), mayor, io, href_f, td_f);
        }
      else if (conf.idremote[i]<=31)          // modbus
        {
        printP(td, modbust, td_f);
        printColspan(2);
        if (posactrem == i)
          seleccionatipoi2cmb(conf.tipoi2cmbrem[i]);
        else
          printP(readdescr(filei2ctypes,conf.tipoi2cmbrem[i],20));
        printP(td_f);
        }
      else            // I2C
        {
        printP(td, i2c);
        if (posactrem == i)
          {
          printP(td_f);
          printColspan(2);
          seleccionatipoi2cmb(conf.tipoi2cmbrem[i]);
          }
        else
          {
          printP(b);
          printH(conf.idremote[i]);
          printP(letrah, td_f);
          printColspan(2);
          printP(readdescr(filei2ctypes,conf.tipoi2cmbrem[i],20));
          }
        printP(td_f);
        }
      }
    else
      {
      printColspan(3);
      if (posactrem==i)
        { printP(href_i, sdrem150htm, mayor);
          printP(t(nuevoremoto), href_f); }
      printP(td_f);
      }
    printP(tr_f);
    }
  printP(tr);
  printColspan(5);
  printP(t(pietiporem));
  printP(href_i,"\"sr?p=0&m=1\"> Scan",href_f);
  printP(td_f,tr_f);
  writeFooter(guardar, false);
  serversend200();
}

void ICACHE_FLASH_ATTR setupsalremHTML()
{
  printremote();
  msg=vacio;
  if (!autOK()) { sendOther(loghtm,-1); return; }
  mp=5;  // número de parámetros por fila:
  if (server.method()==HTTP_POST)
    {
    for (int i=0; i<server.args(); i++)
      {
      calcindices(i);
      if (resto == 2) { conf.idsalremote[indice] = ((server.arg(i).toInt()==0)?0:conf.idremote[server.arg(i).toInt() - 1]);  }
      else if (resto==3) { server.arg(i).toCharArray(auxdesc,20);  savedescr(filesalrem, auxdesc, indice, 20); } // sólo si no es remoto conuco
      else if (resto==4)
        if (conf.idsalremote[indice]>32)       {
          conf.senalrem[indice] = server.arg(i).toInt();     // señal de salida, 0 a 7
          if (conf.senalrem[indice] >= 6) conf.pinremote[indice] = conf.senalrem[indice] - 6; else conf.pinremote[indice] = 99;
        }
      }
    for (byte j=0; j<maxpaneles; j++)
      for (byte i=0; i<maxsalrem; i++)
        if (conf.idsalremote[i]==0)
          setbit8(conf.bshowbypanel[j], i+30, 0);
    saveconf();
    subscribeconfrem();     // suscribe para recibir configuracion del remoto
    for (byte j=0;j<maxsalrem;j++)
      if (conf.idsalremote[j]>0)    // entre 150 y 166
        mqttpublish(conf.idsalremote[j],41);      // publish RCF del remoto
    subscribevaluesrem();   // subscribe a las señales remotas, sólo las definidas
    //    initPRG();    // inicializa programaciones
    strcpy(auxchar, sremhtm);
    strcat(auxchar, igualp); strcat(auxchar, itoa(0, buff, 10));
    strcat(auxchar, amper); strcat(auxchar, letran); strcat(auxchar, ig); strcat(auxchar, itoa(posactsalrem, buff, 10));
    sendOther(auxchar,-1);  return;
    }
  if (server.args()>0) { posactsalrem = constrain(server.arg(1).toInt(), 0, 31); }
  writeHeader(false,false);
  writeMenu(3, 11);
  printP(c(form_action), sremhtm, interr, letrap, ig);
  printP(cero,amper, letran, ig);
  printI(posactsalrem);
  printP(comillas,b);
  printP(c(Form_post), menor);
  printP(table, b);
  printP(c(tclass), ig, tnormal, mayor);

  printP(tr, td, td_f);
  tcell(disp);
  tcell(descripcion);
  tcell(senal);
  printP(tr_f);
  for (byte i=0; i<maxsalrem; i++)
    {
    colorea=(conf.idsalremote[i]!=0);
    mpi=mp*i;
    printP(tr, td);
    printPiP(letraR, i, td_f);
    if (i==posactsalrem)  // código de remoto
      {
      printP(td);   // número de remoto
      printP(c(Select_name),comillas);
      printIP(mpi+2, comillas);
      printP(mayor);
      pc(optionvalue);
      printPiP(comillas, 0, comillas);
      if (conf.idsalremote[i]==0) printP(b, selected, ig, comillas, selected, comillas);
      printPiP(mayor, 0, c(option_f));
      for (byte j = 0; j < maxdevrem; j++)  { // añade remotos
        if (conf.idremote[j] > 0) {
          pc(optionvalue);
          printPiP(comillas, j+1, comillas);
          if (conf.idremote[j] == conf.idsalremote[i]) printP(b, selected, ig, comillas, selected, comillas);
          printPiP(mayor, conf.idremote[j], c(option_f));
          }
        }
      printP(c(select_f), td_f);

      printP(td); // descripción de señal remota
      if ((conf.idsalremote[i]==0) || ((conf.idsalremote[i]>=150) && (conf.idsalremote[i]<=166)))
        printP(readdescr(filesalrem, i, 20));
      else
        {
        printP(menor, c(tinput), b, type, ig, comillas);
        printP(c(ttext), comillas, b);
        printP(c(namet), ig, comillas);
        printI(mpi+3);
        printP(comillas, b, tvalue);
        printP(ig, comillas, readdescr(filesalrem, i, 20));
        printP(comillas,b,c(max_length));
        printIP(19, size_i);
        printI(19);
        printP(comillas, mayor, menor, barra, c(tinput), mayor);
        }
      printP(td_f);

      printP(td);   // señal de salida
      if ((conf.idsalremote[i]==0) || ((conf.idsalremote[i]>=150) && (conf.idsalremote[i]<=166)))
        {
        pc(Select_name);
        printPiP(comillas,mpi+4,comillas);
        printP(mayor);
        for (byte j=0; j<20; j++)  { // añade señales del remoto
          pc(optionvalue);
          printPiP(comillas, j, comillas);
          if (conf.senalrem[i]==j) printP(b, selected, ig, comillas, selected, comillas);
          printP(mayor);
          if (j<=7) { printP(t(sonda), b); printI(j+1);  }
          else if (j<=11) { printP(t(ent), b); printP(c(digital), b); printI(j-8+1);  }
          else if (j<=19) { printP(t(saldig), b); printI(j-12+1);  }
          pc(option_f);
          }
        pc(select_f);
        }
      printP(td_f);
      }
    else
      {
      strcpy(auxchar, sremhtm); strcat(auxchar, igualp); strcat(auxchar, cero);
      strcat(auxchar, amper); strcat(auxchar, letran); strcat(auxchar, ig); strcat(auxchar, itoa(i, buff, 10));
      printOpc(false, false, conf.idsalremote[i]==0?c(notdefined):itoa(conf.idsalremote[i], buff, 10));
      printP(colorea?th:td); // descripción de señal remota
//      if (conf.idsalremote[i]>0) 
      printP(readdescr(filesalrem, i, 20));
      printP(colorea?th_f:td_f);
      printP(td);
      if (conf.idsalremote[i]>32) // escribir señal de conuco
        {
        if (conf.idsalremote[i]>0)
          {
          if (conf.senalrem[i]<=7) { printP(t(sonda), b); printI(conf.senalrem[i]+1);  }
          else if (conf.senalrem[i]<=11) { printP(t(ent), b); printP(c(digital), b); printI(conf.senalrem[i]-8+1); }
          else if (conf.senalrem[i]<=19) { printP(t(saldig), b); printI(conf.senalrem[i]-12+1); }
          }
        }
      printP(td_f);
      }
    printP(tr_f);
    }
  writeFooter(guardar, false);
  serversend200();
}


void ICACHE_FLASH_ATTR setupioHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  mp=21;  // número de parámetros por fila
  if (server.method()==HTTP_POST)
    {
    if ((posactio>=8) && (posactio<=11)) { setbit8(conf.iftttpinED, posactio-8,0); setbit8(conf.iftttpinED, posactio,0); }
    if ((posactio>=12) && (posactio<=19)) { setbit8(conf.iftttpinSD, posactio-12,0); setbit8(conf.iftttpinSD, posactio-4,0); }
    if ((posactio>=20) && (posactio<=29)) { setbit8(conf.mqttgpioenable,posactio-20,0); }
    setbit8(conf.mqttsalenable,posactio,0);
    for (int i=0  ;i<server.args(); i++)
      {
      calcindices(i);
      if (indice<8)  // temperaturas
        {
        if (resto==0) { server.arg(i).toCharArray(auxdesc, 20); savedescr(filedesclocal,auxdesc,indice,20); }
        else if (resto==1) { setbit8(conf.mqttsalenable,indice,1); }
        else if (resto==2) { conf.setpoint[indice]=server.arg(i).toFloat();  }  // valor consigna
        else if (resto==3) { conf.salsetpoint[indice]=server.arg(i).toInt(); }  // salida asociada
        else if (resto==4) { conf.accsetpoint[indice]=server.arg(i).toInt(); }  // acción consigna
        else if (resto==5)    // número y código de sonda
          { 
          conf.nprobe[indice]=server.arg(i).toInt();
          for (byte j=0;j<8;j++) conf.probecode[indice][j]=addr1Wire[server.arg(i).toInt()-1][j];
          } 
        }
      else if ((indice>=8) && (indice<=11)) // entradas digitales
        {
        if (resto==0) { server.arg(i).toCharArray(auxdesc,20); savedescr(filedesclocal, auxdesc,indice,20); }
        else if (resto==1) { setbit8(conf.mqttsalenable,indice,1); }
        else if (resto==5) { conf.tipoED[indice-8] = server.arg(i).toInt(); }  
        else if (resto==6) { setbit8(conf.iftttpinED, indice-8, server.arg(i).toInt()); }   // Notificar si/no
        else if (resto==7) { setbit8(conf.iftttpinED, indice, server.arg(i).toInt()); }  // Notificar si/no
        }
      else if ((indice>=12) && (indice<=19)) // salidas digitales
        {
        if (resto==0) { server.arg(i).toCharArray(auxdesc, 20); savedescr(filedesclocal, auxdesc, indice, 20); }
        else if (resto==1) { setbit8(conf.mqttsalenable,indice,1); }
        else if (resto==6) { setbit8(conf.iftttpinSD, indice-12, server.arg(i).toInt());  }   // Notificar si/no
        else if (resto==7) { setbit8(conf.iftttpinSD, indice-4, server.arg(i).toInt());  }    // Notificar si/no
        else if (resto==8) { conf.valinic[indice-12] = server.arg(i).toInt();  }              // valor inicial
        else if (resto==9) { conf.tempdefact[indice-12] = server.arg(i).toInt();  }           // Seg. ON
        else if (resto==10) { conf.tempdefdes[indice-12] = server.arg(i).toInt();  }          // Seg. OFF
        }
      else if ((indice>=20) && (indice<=29))  // GPIOS configurables
        {
        if (resto==0) { server.arg(i).toCharArray(auxdesc, 20); savedescr(filedescgpio,auxdesc,indice-20,20); }
        else if (resto==1) { setbit8(conf.mqttgpioenable,indice-20,1); }
        else if (resto==17) 
          { 
          conf.gpiosensortype[indice-20]=server.arg(i).toInt(); }
        else if (resto==18) { conf.gpioalfa[indice-20]=server.arg(i).toFloat(); }
        else if (resto==19) { conf.gpiobeta[indice-20]=server.arg(i).toFloat(); }
        else if (resto==20) { conf.gpiogamma[indice-20]=server.arg(i).toFloat(); }
        }
      }
    if (conf.modoterm==1) { conf.tempdefact[0]=0; conf.tempdefdes[0]=0;  }
    memset(conf.contadoresgpio,0,sizeof(conf.contadoresgpio));// contadores de Gpios 
    saveconf();
    posactio++; if (posactio>20+maxgpiovar) posactio=0;
    sendOther(siohtm,-1); return;
    }
  if (server.args() > 0) 
    { 
    saveconf();
    posactio = constrain(server.arg(0).toInt(),0,22+maxgpiovar); 
    }
  writeHeader(false,false);
  writeMenu(3,1);
  writeForm(siohtm);
  printP(tr);
  tcell(descripcion); 
  ccell(gpio);
  printP(td,c(mqtt),td_f);
  printP(td,ID,td_f);
  tcell(consigna);
  tcell(salida);
  printP(td,ONOFF,td_f,tr_f);
  for (byte i=0; i<20+maxgpiovar; i++)
    {
    mpi=mp*i;
    if (i<=7) // Sondas temperatura
      {
      printP(tr);
      if (posactio==i)
        {
        printP(td, menor, c(tinput), b, type, ig);
        printP(comillas, c(ttext), comillas, b);
        printP(c(namet), ig);
        printPiP(comillas, mpi, comillas);
        printP(b, tvalue, ig, comillas);
        printP(readdescr(filedesclocal,i,20));
        printP(comillas,b,c(max_length));
        printIP(19, size_i);
        printI(19);
        printP(comillas, mayor, menor, barra, c(tinput), mayor);
        printP(td_f);
        cell(W0);
        pc(select_f);
        checkBox(mpi+1,(getbit8(conf.mqttsalenable,i)),true);
        selectProbe(mpi+5, conf.nprobe[i], true);
        printP(td);
        printcampoF(mpi+2, conf.setpoint[i], 1); // valor de consigna
        printP(td_f,td,c(Select_name),comillas);
        printI(mpi+3);
        printP(comillas, mayor);       // señal de salida
        for (byte j=0; j<maxSD; j++)   { // salidas digitales locales
          pc(optionvalue);
          printPiP(comillas, j, comillas);
          if (conf.salsetpoint[i]==j) printP(b, selected, ig, comillas, selected, comillas);
          if (conf.showN) printPiP(mayorparen, j, parenguion); else printP(mayor);
          printP(readdescr(filedesclocal,j+12,20));
          pc(option_f);
          }
        pc(select_f);
        printP(td_f,td,c(Select_name),comillas);
        printIP(mpi+4, comillas);
        printP(mayor);
        for (byte k=0; k<3; k++) {
          pc(optionvalue);
          printPiP(comillas, k, comillas);
          if (k==conf.accsetpoint[i]) printP(b, selected, ig, comillas, selected, comillas);
          printP(mayor, k==0?OFF:k==1?ON:guion,c(option_f));
          }
        pc(select_f);
        }
      else
        {
        readdescr(filedesclocal,i,20);
        strcpy(auxchar, siohtm); strcat(auxchar, igualp); strcat(auxchar,itoa(i,buff,10));
        printOpc(false, false, auxdesc);
        cell(W0);
        cell(getbit8(conf.mqttsalenable,i)?symyes:symnot);
        printP(td); 
        printaddr1Wire(conf.nprobe[i]);
        printP(td_f);
        cell(conf.setpoint[i],2);
        printP(td); 
        printP(readdescr(filedesclocal,conf.salsetpoint[i]+12,20));  
        printP(td_f);
        printP(td,conf.accsetpoint[i]==0?OFF:conf.accsetpoint[i]==1?ON:guion,td_f);
        }
      printP(tr_f);
      }
    if ((i>=8) && (i<=11))    // entradas digitales
      {
      if (i==8)  
        { 
        espacioSep(7);
        printP(tr); printColspan(3); tcell(ttipo); printColspan(2); printP(td_f); ccell(ifttt); printP(tr_f); 
        }
      printP(tr);
      if (posactio==i) 
        {
        printP(td, menor, c(tinput), b, type, ig);
        printP(comillas);
        printP(c(ttext), comillas, b);
        printP(c(namet), ig, comillas);
        printI(mpi);
        printP(comillas, b, tvalue, ig, comillas);
        printP(readdescr(filedesclocal, i, 20));
        printP(comillas,b,c(max_length));
        printIP(19, size_i);
        printI(19);
        printP(comillas, mayor, menor, barra, c(tinput), mayor);
        printP(td_f);
        cell(edPin[i-8]);
        checkBox(mpi+1,(getbit8(conf.mqttsalenable,i)),true);
        printcampoCB(mpi+5, conf.tipoED[i-8], ONOFF, OFFON,true);
        printColspan(2);
        printP(td);
        checkBox(mpi+6, getbit8(conf.iftttpinED, i-8),false); // checkbox Notificar ON
        printP(barra);
        checkBox(mpi+7, getbit8(conf.iftttpinED, i-0),false); // checkbox Notificar OFF
        }
      else
        {
        readdescr(filedesclocal,i,20);
        strcpy(auxchar, siohtm); strcat(auxchar, igualp); strcat(auxchar, itoa(i, buff, 10));
        printOpc(false, false, auxdesc);
        cell(edPin[i-8]);
        cell(getbit8(conf.mqttsalenable,i)?symyes:symnot);
        cell(conf.tipoED[i-8]==0?ONOFF:conf.tipoED[i-8]==1?OFFON:dhtt);
        printColspan(2);
        printP(td, getbit8(conf.iftttpinED,i-8)?symyes:symnot);
        printP(barra, getbit8(conf.iftttpinED,i-0)?symyes:symnot);
        }
      printP(td_f,tr_f);
      }
   if ((i>=12) && (i<=19))    // salidas digitales
     {
     printP(tr);
    if (i==12) { espacioSep(7);  printP(tr); printColspan(3); ccell(defaultt); ccell(tdefact); ccell(tdefdes); ccell(ifttt); printP(tr_f); }
    if (posactio==i) 
      {
      printP(td, menor, c(tinput), b, type, ig);
      printP(comillas, c(ttext), comillas, b);
      printP(c(namet), ig);
      printPiP(comillas, mpi, comillas);
      printP(b, tvalue, ig, comillas);
      printP(readdescr(filedesclocal, i, 20));
      printP(comillas,b,c(max_length));
      printIP(19, size_i);
      printI(19);
      printP(comillas, mayor, menor, barra, c(tinput), mayor);
      printP(td_f);
      cell(sdPin[i-12]);

      checkBox(mpi+1,(getbit8(conf.mqttsalenable,i)),true);
      printcampoCB(mpi+8, conf.valinic[i-12], OFF, ON, ultimovalor,true);
      printcampoL(mpi+9, conf.tempdefact[i-12], 8, true,true);
      printcampoL(mpi+10, conf.tempdefdes[i-12], 8, true,true);
      printP(td);
      checkBox(mpi+6, getbit8(conf.iftttpinSD, i-12),false); // checkbox Notificar ON
      printP(barra);
      checkBox(mpi+7, getbit8(conf.iftttpinSD, i-4),false); // checkbox Notificar OFF
      }
    else
      {
      readdescr(filedesclocal,i,20);
      strcpy(auxchar, siohtm); strcat(auxchar, igualp); strcat(auxchar, itoa(i, buff, 10));
      printOpc(false, false, auxdesc);
      cell(sdPin[i-12]);

      cell(getbit8(conf.mqttsalenable,i)?symyes:symnot);
      cell(conf.valinic[i-12]==0?OFF : conf.valinic[i-12] == 1 ? ON : ultimovalor);
      cell(conf.tempdefact[i-12]);
      cell(conf.tempdefdes[i-12]);
      printP(td,getbit8(conf.iftttpinSD,i-12)?symyes:symnot, barra,getbit8(conf.iftttpinSD,i-4)?symyes:symnot);
      }
      printP(td_f);
     printP(tr_f);
     }
    if (i>=20)    // GPIOs configurables
      {
      if (i==20)
        {
        espacioSep(7); printP(tr);
        printColspan(4); printP("GPIOS configurables", td_f);
        printP(td,letraa,td_f,td,letrab,td_f); printP(td,letrac,td_f,tr_f);   
        }
      if (gpiovis(i-20))
        {
        if (posactio==i)
          {
          printP(td, menor, c(tinput), b, type, ig);
          printP(comillas, c(ttext), comillas, b);
          printP(c(namet), ig);
          printPiP(comillas, mpi, comillas);
          printP(b, tvalue, ig, comillas);
          printP(readdescr(filedescgpio,i-20,20));
          printP(comillas,b,c(max_length));
          printIP(19, size_i);
          printI(19);
          printP(comillas, mayor, menor, barra, c(tinput), mayor);
          printP(td_f);
          
          printPiP(td,listgpiovar[i-20],td_f);
          checkBox(mpi+1,(getbit8(conf.mqttgpioenable,i-20)),true);
          printP(td,c(Select_name),comillas);
          printIP(mpi+17, comillas);
          printP(mayor);
          for (byte k=0; k<maxsensortype; k++) {
            pc(optionvalue);
            printPiP(comillas, k, comillas);
            if (k==conf.gpiosensortype[i-20]) printP(b, selected, ig, comillas, selected, comillas);
            printP(mayor, sensortype[k],c(option_f));
            }
          pc(select_f);
          printP(td_f,td);
          printcampoF(mpi+18, conf.gpioalfa[i-20], 5);  printP(td_f,td);
          printcampoF(mpi+19, conf.gpiobeta[i-20], 5);  printP(td_f,td);  
          printcampoF(mpi+20, conf.gpiogamma[i-20], 5);  printP(td_f);  
          }
        else
          {
          readdescr(filedescgpio,i-20,20);
          strcpy(auxchar, siohtm); strcat(auxchar, igualp); strcat(auxchar, itoa(i, buff, 10));
          printOpc(false, false, auxdesc);
          printPiP(td,listgpiovar[i-20],td_f);
          cell(getbit8(conf.mqttgpioenable,i-20)?symyes:symnot);
          printP(td,sensortype[conf.gpiosensortype[i-20]],td_f);
          printP(td); printF(conf.gpioalfa[i-20],5); printP(td_f);
          printP(td); printF(conf.gpiobeta[i-20],5); printP(td_f);
          printP(td); printF(conf.gpiogamma[i-20],5); printP(td_f);
          }
        }
      }
    printP(tr_f);
    }
  writeFooter(guardar, false);
  serversend200();
}

void ICACHE_FLASH_ATTR setupDevHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  mp=1;
  if (server.method()==HTTP_POST)
    {
    conf.showN=0; conf.modoterm=0; conf.actualizaut=0;
    conf.RX433enabled=0; conf.TX433enabled=0; conf.SPIenabled=0;
    conf.I2Cenabled=0; conf.TFTenabled=0; conf.SERIAL2enabled=0;
    for (int i=0; i<server.args(); i++)
      {
      calcindices(i);
      if (param_number==0)
        {
        conf.iddevice = server.arg(i).toInt(); // núm. dispositivo
        conf.EEip[3] = conf.iddevice;
        strcpy(conf.ssidAP, c(CONUCO)); strcat(conf.ssidAP, subray); strcat(conf.ssidAP, itoa(conf.iddevice, buff, 10));
        }
      else if (param_number==1) { conf.showN=server.arg(i).toInt(); } // mostrar datos pin
      else if (param_number==2) { conf.peractpan=server.arg(i).toInt();  }  // Per. actualización
      else if (param_number==3) { conf.prectemp=server.arg(i).toInt(); sensors0.setResolution(conf.prectemp); }  // Precision sondas
      else if (param_number==6) { conf.TempDesactPrg = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==8) { server.arg(i).toCharArray(conf.aliasdevice, sizeof(conf.aliasdevice)); } // aliasdevice
      else if (param_number==9) { conf.peractrem = server.arg(i).toInt(); }  // Per. actualización remotos
      else if (param_number==10) { server.arg(i).toCharArray(conf.instname, server.arg(i).length()+1); }  // instname
      else if (param_number==14) { conf.modoterm=server.arg(i).toInt(); if (conf.modoterm==1) { conf.tempdefact[0]=0; conf.tempdefdes[0]=0; }}  // modo termostato
      else if (param_number==15) { conf.latitud = server.arg(i).toFloat();  }
      else if (param_number==16) { conf.longitud = server.arg(i).toFloat(); }
      else if (param_number==17) { conf.actualizaut=server.arg(i).toInt(); } // actualización automática
      else if (param_number==18) { server.arg(i).toCharArray(conf.fwUrlBase, server.arg(i).length()+1); }  // fwUrlBase
      else if (param_number==20) { conf.lang = server.arg(i).toInt(); } // idioma
      else if (param_number==21) { conf.modofi = server.arg(i).toInt(); } // modo normal/bomba de calor/radio
      else if (param_number==22) { conf.RX433enabled = server.arg(i).toInt(); } // 
      else if (param_number==23) { conf.TX433enabled = server.arg(i).toInt(); } // 
      else if (param_number==24) { conf.SPIenabled = server.arg(i).toInt(); } // 
      else if (param_number==25) { conf.I2Cenabled = server.arg(i).toInt(); } // 
      else if (param_number==26) { conf.TFTenabled = server.arg(i).toInt(); } // 
      else if (param_number==27) { conf.rstper = server.arg(i).toInt(); } // período rset automatico
      else if (param_number==28) { conf.SERIAL2enabled = server.arg(i).toInt(); } // 
      }
    saveconf();
    readconf();
    sendOther(sdhtm,-1);
    return;
    }

  writeHeader(false,false);
  writeMenu(3, 0);
  writeForm(sdhtm);

  printP(tr, td, t(instalacion),barraesp);
  printP(c(alias), barraesp);
  printP(t(dispositivo), td_f);
  printcampoC(10, conf.instname, 10, true, true, false,true);
  printcampoC(8, conf.aliasdevice, 20, true, true, false,true);
  printcampoCB(0, conf.iddevice, 150, 166,true);
  printP(tr_f);
  
  printP(tr,conf.showN?th:td);  
  pt(mostrarpines);
  checkBox(1, conf.showN,false);
  printP(conf.showN?th_f:td_f);

  printP(conf.TFTenabled?th:td,"TFT");  
  checkBox(26, conf.TFTenabled,false);
  printP(conf.TFTenabled?th_f:td_f);

  printP(conf.RX433enabled?th:td,"RX 433");  
  checkBox(22, conf.RX433enabled,false);
  printP(conf.RX433enabled?th_f:td_f);

  printP(conf.TX433enabled?th:td,"TX 433");  
  checkBox(23, conf.TX433enabled,false);
  printP(conf.TX433enabled?th_f:td_f,tr_f);

  printP(tr,conf.SPIenabled?th:td,"SPI");  
  checkBox(24, conf.SPIenabled,false);
  printP(conf.SPIenabled?th_f:td_f);

  printP(conf.I2Cenabled?th:td,"I2C");  
  checkBox(25, conf.I2Cenabled,false);
  printP(conf.I2Cenabled?th_f:td_f);

  printP(conf.SERIAL2enabled?th:td,"Serial 2");  
  checkBox(28, conf.SERIAL2enabled,false);
  printP(conf.SERIAL2enabled?th_f:td_f,td,td_f,tr_f);

  printP(tr,td);
  pt(aut);
  printP(barra,t(versiont), td_f, conf.actualizaut ? th : td);
  checkBox(17, conf.actualizaut,false);
  colorea = (verserver > versinst);
  printP(conf.actualizaut?th_f:td_f,colorea?th:td);
  printI(versinst);
  printP(colorea ? th_f : td_f, colorea ? th : td);
  printI(verserver);
  printP(colorea?th_f:td_f,tr_f);

  printP(tr, td, href_i, comillas, conf.fwUrlBase, comillas);
  printP(b, c(newpage), mayor);
  pc(Firmware);
  printP(b,c(tserver), href_f, td_f);
  printColspan(3);
  printcampoC(18, conf.fwUrlBase, 50, true, true, false,false);  
  printP(td_f,tr_f);

  printP(tr, td, t(periodoact255), barra);
  printP(t(periodoactrem), barra);
  printP(t(tdp), td_f);
  printcampoI(2, conf.peractpan, 5, true,true); 
  printcampoI(9, conf.peractrem, 5, true,true);  
  printcampoL(6, conf.TempDesactPrg, 5, true,true);  
  printP(tr_f);

  printP(tr, td, href_i, comillas);
  pc(thttp);
  pc(gmaps);
  printP(comillas, b, c(newpage), mayor);
  printP(t(latitudt), barraesp);
  printP(t(longitudt), href_f, td_f, td);
  printcampoF(15, conf.latitud, 6);  printP(td_f, td);
  printcampoF(16, conf.longitud, 6);  printP(td_f, td, td_f, tr_f);

  printP(tr, td, t(idioma),td_f);
  printcampoCB(20, conf.lang, PSTR("Español"), PSTR("English"),true); 
  printP(td_f,td, t(Modo),td_f);
  printcampoCB(21, conf.modofi, PSTR("Normal"), PSTR("Bomba de calor"), true); 
  printP(td_f,tr_f);

  printP(tr, td, "Reset periodico (horas)",td_f,td);
  printcampoCB(27,conf.rstper,1,24,false); 
  printP(td_f,td,td_f,td,td_f,tr_f);

  writeFooter(guardar, false);
  serversend200();
}

void ICACHE_FLASH_ATTR setupbcHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  mp=1;
  if (server.method()==HTTP_POST)
    {
    conf.SAI=0; conf.Cal=0; conf.Ref=0; conf.ACS=0; 
    conf.APP=0; conf.Pro=0; conf.ECO=0; conf.APP=0; 
    for (int i=0; i<server.args(); i++)
      {
      calcindices(i);
      if      (param_number==0) { conf.SAI=server.arg(i).toInt(); }  // Verano/Invierno
      else if (param_number==1) { conf.Cal=server.arg(i).toInt(); }  // Calefacción
      else if (param_number==2) { conf.Ref=server.arg(i).toInt();  } // Refrigeración
      else if (param_number==3) { conf.ACS=server.arg(i).toInt();  } // ACS
      else if ((param_number>=4)&&(param_number<=7))
        { conf.T[indice] = server.arg(i).toInt(); } // parámetros T1..T4
      else if (param_number==8) { conf.CC = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==9) { conf.CCC = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==10) { conf.FRO = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==11) { conf.ECS = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==12) { conf.PIS = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==13) { conf.RES = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==14) { conf.Cr1 = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==15) { conf.Cr2 = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==16) { conf.AoC = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==17) { conf.AoF = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==18) { conf.HYC = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==19) { conf.HYE = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==20) { conf.HYF = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==21) { conf.HYr = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==22) { conf.bPC = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==23) { conf.bPF = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==24) { conf.AH2 = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==25) { conf.AH4 = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==26) { conf.Ab4 = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==27) { conf.tLP = server.arg(i).toInt(); } // seg. desact. prog.
      else if (param_number==28) { conf.ECO=server.arg(i).toInt(); }  // Calefacción
      else if (param_number==29) { conf.TOF=server.arg(i).toInt(); }  // Calefacción
      else if (param_number==30) { conf.TON=server.arg(i).toInt(); }  // Calefacción
      else if (param_number==31) { conf.Pro=server.arg(i).toInt(); }  // Calefacción
      else if (param_number==32) { conf.Tin=server.arg(i).toInt(); }  // Calefacción
      else if (param_number==33) { conf.Tfi=server.arg(i).toInt(); }  // Calefacción
      else if (param_number==34) { conf.Nbh=server.arg(i).toInt(); }  // Calefacción
      else if (param_number==35) { conf.TL2=server.arg(i).toInt(); }  // Calefacción
      else if (param_number==36) { conf.APP=server.arg(i).toInt(); }  // Calefacción
      }
    if (conf.SAI==0)    // invierno
      { conf.Ref=0; }     // frío OFF
    else
      { conf.Cal=0; }     // Calefacción OFF
    saveconf();
    readconf();
    sendOther(sbhtm,-1);
    return;
    }

  writeHeader(false,false);
  writeMenu(3, 12);
  writeForm(sbhtm);
//////////////////////////////////
  printP(tr,conf.SAI==1?th:td,conf.SAI==0?"Invierno (SAI)":"Verano (SAI)");  
  printP(conf.SAI==1?th_f:td_f,conf.SAI==1?th:td);  
  checkBox(0, conf.SAI==1, false);
  printP(conf.SAI==1?th_f:td_f,tr_f);

  printP(tr,conf.Cal==1?th:td,"Calefaccion");  
  printP(conf.Cal==1?th_f:td_f,conf.Cal==1?th:td);  
  checkBox(1, conf.Cal==1,false);
  printP(conf.Cal==1?th_f:td_f,tr_f);
  
  printP(tr,conf.Ref==1?th:td,"Refrigeracion");  
  printP(conf.Ref==1?th_f:td_f,conf.Ref==1?th:td);  
  checkBox(2, conf.Ref==1,false);
  printP(conf.Ref==1?th_f:td_f,tr_f);

  printP(tr,conf.ACS==1?th:td,"ACS");  
  printP(conf.ACS==1?th_f:td_f,conf.ACS==1?th:td);  
  checkBox(3, conf.ACS==1,false);
  printP(conf.ACS==1?th_f:td_f,tr_f);

  printP(tr,td,"Parametros T1/T2/T3/T4",td_f,td);
  for (byte i=0; i<4;i++) { printcampoI(4+i, conf.T[i], 5, true,false); if (i<3) printP(barra); }
  printP(td_f,tr_f);

  printP(tr, td,"Temporizacion CC/CCC",td_f,td);
  printcampoI(8, conf.CC, 5, true,false); printP("minutos ", barra); 
  printcampoI(9, conf.CCC, 5, true,false);printP("minutos");  
  printP(td_f,tr_f);

  printP(tr, td,"Consigna Frio / ACS C (FRO/ECS)",td_f,td);
  printcampoI(10, conf.FRO, 5, true,false); printP(barra);
  printcampoI(11, conf.ECS, 5, true,false); 
  printP(td_f,tr_f);
  printP(tr, td,"Consigna Piscina / Resistencia (PIS/RES)",td_f,td);
  printcampoI(12, conf.PIS, 5, true,false); printP(barra);
  printcampoI(13, conf.RES, 5, true,false); 
  printP(td_f,tr_f);
  printP(tr, td,"Tempor. Resistencia / T. minimo (Cr1/Cr2)",td_f,td);
  printcampoI(14, conf.Cr1, 5, true,false); printP("minutos ",barra);
  printcampoI(15, conf.Cr2, 5, true,false); printP("horas"); 
  printP(td_f,tr_f);
  printP(tr, td,"Consigna mezcladora calor / frio (AoC/AoF)",td_f,td);
  printcampoI(16, conf.AoC, 5, true,false); printP(barra);
  printcampoI(17, conf.AoF, 5, true,false); 
  printP(td_f,tr_f);
  printP(tr, td,"Banda proporcional calor / frio (bPC(bPF)",td_f,td);
  printcampoI(22, conf.bPC, 5, true,false); printP(barra);
  printcampoI(23, conf.bPF, 5, true,false); 
  printP(td_f,tr_f);

  printP(tr, td,"Histeresis HYC/HYE/HYF/HYr",td_f,td);
  printcampoI(18, conf.HYC, 5, true,false); printP(barra);
  printcampoI(19, conf.HYE, 5, true,false); printP(barra); 
  printcampoI(20, conf.HYF, 5, true,false); printP(barra);
  printcampoI(21, conf.HYr, 5, true,false); 
  printP(td_f,tr_f);

  printP(tr, td,"Limite Ida / Sonda 4 (AH2/Ab4)",td_f,td);
  printcampoI(24, conf.AH2, 5, true,false); printP(barra);
  printcampoI(25, conf.AH4, 5, true,false); 
  printP(td_f,tr_f);
  printP(tr, td,"Limite capa freatica / piscina (Ab4/tLP)",td_f,td);
  printcampoI(26, conf.Ab4, 5, true,false); printP(barra);
  printcampoI(27, conf.tLP, 5, true,false); 
  printP(td_f,tr_f);

  printP(tr,conf.ECO==1?th:td,"Modo ECO");  
  printP(conf.ECO==1?th_f:td_f,conf.ECO==1?th:td);  
  checkBox(28, conf.ECO==1, false);
  printP(conf.ECO==1?th_f:td_f,tr_f);

  printP(tr, td,"Tiempo parada/arranque bomba TOF(h)/TON(min)",td_f,td);
  printcampoI(29, conf.TOF, 5, true,false); printP("horas", barra);
  printcampoI(30, conf.TON, 5, true,false); printP("minutos");
  printP(td_f,tr_f);

  printP(tr,conf.Pro==1?th:td,"Modo progresivo (Pro)");  
  printP(conf.Pro==1?th_f:td_f,conf.Pro==1?th:td);  
  checkBox(31, conf.Pro==1, false);
  printP(conf.Pro==1?th_f:td_f,tr_f);
  
  printP(tr, td,"Temperatura inicial/final (Tin/Tfi)",td_f,td);
  printcampoI(32, conf.Tin, 5, true,false); printP(barra);
  printcampoI(33, conf.Tfi, 5, true,false); 
  printP(td_f,tr_f);

  printP(tr, td,"Numero de horas / Temp. limite (Nbh/TL2)",td_f,td);
  printcampoI(34, conf.Nbh, 5, true,false); printP(barra);
  printcampoI(35, conf.TL2, 5, true,false); 
  printP(tr_f);
  printP(tr,conf.APP==1?th:td,"Modo apoyo (APP)");  
  printP(conf.APP==1?th_f:td_f,conf.APP==1?th:td);  
  checkBox(36, conf.APP==1, false);
  printP(conf.APP==1?th_f:td_f,tr_f);

//////////////////////////////////
  writeFooter(guardar, false);
  serversend200();
}

void ICACHE_FLASH_ATTR setupPanelHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  mp=2;
  if (server.method() == HTTP_POST)
    {
    memset(conf.bshowpanel,0,sizeof(conf.bshowpanel));
    for (int i=0; i<server.args(); i++)
      {
      calcindices(i);
      if (resto==0) { server.arg(i).toCharArray(auxdesc, 20); savedescr(filezonas, auxdesc, indice, 20); }
      else if (resto==1) { setbit8(conf.bshowpanel, indice, server.arg(i).toInt()); }
      }
    saveconf();
    sendOther(sphtm,-1);
    return;
    }

  writeHeader(false,false);
  writeMenu(3, 5);
  writeForm(sphtm);
  printP(tr, c(td_if), td);
  printP(t(descripcion), td_f);
  tcell(activo);
  printP(tr_f);
  for (byte i=0; i<maxpaneles; i++)  {
    mpi=mp*i;
    colorea=(getbit8(conf.bshowpanel, i));
    printP(tr, td, href_i, sbphtm, interr, letran);
    printPiP(ig, i, b);
    printP(c(newpage), mayor);
    printPiP(letraZ, i+1, td_f);
    printP(td,menor);
    printP(c(tinput), b, type, ig, comillas);
    printP(c(ttext), comillas, b);
    printP(c(namet), ig);
    printPiP(comillas,mpi,comillas);    // número de parámetro
    printP(b, tvalue, ig, comillas);
    printP(readdescr(filezonas, i, 20));
    printP(comillas,b,c(max_length));
    printIP(19, size_i);
    printI(19);
    printP(comillas, b, mayor, menor, barra);
    printP(c(tinput), mayor, colorea ? th_f : td_f, colorea ? th : td);
    checkBox(mpi+1, colorea,false);
    printP(colorea?th_f:td_f,tr_f);
    }
  writeFooter(guardar, false);
  serversend200(); 
}

void ICACHE_FLASH_ATTR setupbyPanelHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  mp=1;
  byte auxI=server.arg(0).toInt();
  if (server.method() == HTTP_POST)
    {
    memset(conf.bshowbypanel[auxI],0, sizeof(conf.bshowbypanel[auxI]));
    for (int i=0; i<server.args(); i++)
      {
      if ((server.argName(i)!="n") && (server.argName(i)!="p"))
        { 
        calcindices(i); 
        if (indice<30)
          setbit8(conf.bshowbypanel[auxI], indice, 1);   
        else if (indice>=100)
          setbit8(conf.bshowbypanel[auxI], indice-100+30, 1);   
        }
      }
    saveconf();
    strcpy(auxchar, sbphtm); strcat(auxchar, paramn); strcat(auxchar, itoa(auxI, buff, 10));
    sendOther(auxchar,-1); return;
    }

  writeHeader(false,false);
  writeMenu(3, 5);
  printP(c(form_action), sbphtm, paramn);
  printI(auxI);
  printP(comillas,b);
  pc(Form_post);
  printP(menor,table, b);
  printP(c(tclass), ig, tnormal, mayor);

  printP(tr); printColspan(7); printP(t(zona),dp); printP(readdescr(filezonas, auxI, 20), td_f, tr_f);
  printP(tr); printColspan(3); printP("Locales"); printP(td_f,td,td_f);
  printColspan(3); printP("Remotas"); printP(td_f,tr_f);

  for (byte i=0; i<maxsalrem; i++)  {  // maxsalrem=32
    mpi=mp*i;
    if (i<20)   // señales locales
      {
      colorea=(getbit8(conf.bshowbypanel[auxI], i));
      printP(tr,td,letraL);
      printI(i);
      printP(td_f, colorea?th:td);
      printP(readdescr(filedesclocal, i, 20));
      printP(colorea?th_f:td_f, colorea?th:td);
      checkBox(mpi, colorea,false);
      printP(colorea?th_f:td_f);
      }
    else if (i<30)    // GPIOs configurables
      {
      colorea=(getbit8(conf.bshowbypanel[auxI], i));
      printP(tr,td,letraG,letraP);
      printI(i-20);
      printP(td_f, colorea?th:td);
      printP(readdescr(filedescgpio,i-20,20));
      printP(colorea?th_f:td_f, colorea?th:td);
      checkBox(mpi, colorea,false);
      printP(colorea?th_f:td_f);
      }
    else
      printColspan(3);
    printP(td,td_f);
    // señales remotas
    colorea=(getbit8(conf.bshowbypanel[auxI], i+30));
    printP(td,letraR);
    printI(i);
    printP(td_f,colorea?th:td);
    printP(conf.idsalremote[i]>0?readdescr(filesalrem,i,20):c(notdefined));
    printP(colorea?th_f:td_f,colorea?th:td);
    checkBox(mpi+100,colorea,false);
    printP(colorea?th_f:td_f,tr_f);
    }
  writeFooter(guardar, false);
  serversend200();
}

void ICACHE_FLASH_ATTR lineasetuprfonoff(boolean remote, byte num, char* texto)
{
  printP(tr, td, href_i, syhtm, interr, letran);
  printPiP(ig, num, amper);
  printP(letrar, ig);
  if (remote) printP(letrar);
  printP(rfon, mayor);
  printP(texto, b, ON, td_f,td);
  printL(conf.code433.codeon[remote?num+2:num]);
  printP(td_f,td, href_i, syhtm, interr, letran);
  printPiP(ig, num, amper);
  printP(letrar,ig);
  if (remote) printP(letrar);
  printP(rfoff, mayor);
  printP(texto, b, OFF, td_f,td);
  printL(conf.code433.codeoff[remote?num+2:num]);
  printP(td_f,tr_f);
}

void ICACHE_FLASH_ATTR lineasetuprfkey(byte num, PGM_P texto, unsigned long valor)
{
  printP(td, href_i, syhtm, interr, letran);
  printPiP(ig, num, amper);
  printP(letrar, ig, PSTR("rfkey"), mayor);
  printP(texto, td_f);
  cell(valor);
}

void ICACHE_FLASH_ATTR setuprfHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  if (server.method()==HTTP_POST) { saveconf(); sendOther(rfhtm,-1); return; }
  writeHeader(false,false);
  writeMenu(3,2);
  writeForm(rfhtm);
  printP(tr);
  lineasetuprfkey(1, intro, conf.rfkeys.code[0]);
  lineasetuprfkey(2, esc, conf.rfkeys.code[1]);  printP(tr_f, tr);
  lineasetuprfkey(3, ups, conf.rfkeys.code[2]);
  lineasetuprfkey(4, downs, conf.rfkeys.code[3]);  printP(tr_f, tr);
  lineasetuprfkey(5, lefts, conf.rfkeys.code[4]);
  lineasetuprfkey(6, rigths, conf.rfkeys.code[5]);  printP(tr_f);

  espacioSep(4);
  for (byte i=0;i<maxSD;i++) { lineasetuprfonoff(false,i,readdescr(filedesclocal,i+14,20));  }
  for (byte i=0; i<maxsalrem; i++) 
    if (conf.idsalremote[i]!=0) 
      if (conf.senalrem[i]>=6) { lineasetuprfonoff(true, i,readdescr(filesalrem,i,20)); }
  writeFooter(guardar, false);
  serversend200();
}

void ICACHE_FLASH_ATTR linearoomba(PGM_P texto, byte comm)
{
  printP(tr, td, href_i, syhtm, interr, letrap);
  printPiP(ig, comm, amper);
  printP(letrar, ig, ro, mayor, texto);
  printP(td_f, tr_f);
}

void ICACHE_FLASH_ATTR roombaHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  writeHeader(false,false);
  writeMenu(3, 2);
  writeForm(rohtm);
  linearoomba(c(START), 128);
  linearoomba(c(BAUD), 129);
  linearoomba(c(CONTROL), 130);
  linearoomba(c(SAFE), 131);
  linearoomba(c(FULL), 132);
  linearoomba(c(POWER), 133);
  linearoomba(c(SPOT), 134);
  linearoomba(c(CLEAN), 135);
  linearoomba(c(MAX), 136);
  linearoomba(c(DRIVE), 137);
  linearoomba(c(MOTORS), 138);
  linearoomba(c(LEDS), 139);
  linearoomba(c(SONG), 140);
  linearoomba(c(PLAY), 141);
  linearoomba(c(SENSORS), 142);
  linearoomba(c(DOCK), 143);
  writeFooter(guardar, false);
  serversend200();
}

void ICACHE_FLASH_ATTR downloadHTML() {
  File download=SPIFFS.open(server.arg(0),letrar);
  if (download) {
    server.sendHeader(contenttype, texttext);     // "Content-Type", "text"
    server.sendHeader(c(contentdisposition), attachfilename+server.arg(0));    //"Content-Disposition","attachment; filename=xxxx" 
    server.sendHeader(c(tconnection), closet);          // "Connection", "close"
    server.streamFile(download, c(applicationoctet));  // "application/octet-stream"
    download.close();
  }
}

void ICACHE_FLASH_ATTR setupDevRemHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  iddevicetemp = server.arg(0).toInt(); // núm. dispositivo
  int auxerr = ReqJsonConf(iddevicetemp, conf.webPort);
  byte posdevice = 99;
  if (auxerr==HTTP_CODE_OK) {
    extraevaloresTempConf(false);
    for (byte i=0; i<maxdevrem; i++)  {
      if (conf.idremote[i]==iddevicetemp) posdevice=i;
      if ((conf.idremote[i]>=150) && (conf.idremote[i]<=166))
        {
        strcpy(auxdesc, aliasdevicetemp);  
        savedescr(filedevrem, auxdesc, posdevice, 20);    // guarda nombre dispositivo en devrem.txt
        for (byte j=0; j<maxsalrem; j++)                  // guarda nombres de señales en salrem.txt
          {
          if (conf.idsalremote[j]==iddevicetemp)         // coincide dispositivo
            if (moddevicetemp==8266)
              {
              if ((conf.senalrem[j]>=0) && (conf.senalrem[j]<=7))
                { 
                if (conf.senalrem[j]<=2)
                  {
                  readdescr(filedesctemp, conf.senalrem[j], 20); 
                  savedescr(filesalrem, auxdesc, j, 20); 
                  }
                else if (conf.senalrem[j]<=3)
                  {
                  readdescr(filedesctemp, conf.senalrem[j]+8-3, 20); 
                  savedescr(filesalrem, auxdesc, j, 20); 
                  }
                else if (conf.senalrem[j]<=5)
                  {
                  readdescr(filedesctemp, conf.senalrem[j]+10-4, 20); 
                  savedescr(filesalrem, auxdesc, j, 20); 
                  }
                else if (conf.senalrem[j]<=7)
                  {
                  readdescr(filedesctemp, conf.senalrem[j]+14-6, 20); 
                  savedescr(filesalrem, auxdesc, j, 20); 
                  }
                }
              }
            else
              {
              if ((conf.senalrem[j]>=0) && (conf.senalrem[j]<=22))
                { 
                readdescr(filedesctemp, conf.senalrem[j], 20); 
                savedescr(filesalrem, auxdesc, j, 20); 
                }
              }
          }
        saveconf();
        }
      }
    mqttpublish(0,41);    // auxchar=mqttpathtemp;
    } 
  msg=vacio;
  mp=1;
  
  if (server.method()==HTTP_POST)
    {
    mododweettemp=0; iftttenabledtemp=0; iottweetenabletemp=0;
    actualizauttemp=0;
    for (int i=0; i<server.args(); i++)
      {
      calcindices(i);
      if (param_number==1) { iddevicetemp = server.arg(i).toInt(); } // núm. dispositivo
      else if (param_number==2) { server.arg(i).toCharArray(aliasdevicetemp, sizeof(aliasdevicetemp)); } // aliasdevicetemp
      else if (param_number==3) { server.arg(i).toCharArray(iftttkeytemp, sizeof(iftttkeytemp)); } // iftttkeytemp
      else if (param_number==4) { iftttenabledtemp=server.arg(i).toInt(); } // iftttenabletemp
      else if (param_number==6) { mododweettemp=server.arg(i).toInt(); } // enable mododweettemp
      else if (param_number==7) { iottweetenabletemp=server.arg(i).toInt(); } // enable iottweet
      else if (param_number==9) { actualizauttemp=server.arg(i).toInt(); } 
      else if (param_number==10) { server.arg(i).toCharArray(fwUrlBasetemp, 80); }
      else if (param_number==11) { peractpantemp=server.arg(i).toInt();  }
      else if (param_number==12) { peractremtemp=server.arg(i).toInt();  }
      else if (param_number==13) { TempDesactPrgtemp=server.arg(i).toInt(); }
      else if (param_number==14) { latitudtemp=server.arg(i).toFloat(); }
      else if (param_number==15) { longitudtemp=server.arg(i).toFloat(); }
      }
    sendJsonConf(iddevicetemp, conf.webPort, false, false); // envia jsonConf al remoto para configurar
    strcpy(auxchar, sdremhtm);  strcat(auxchar, interr); strcat(auxchar, c(trem)); strcat(auxchar, itoa(iddevicetemp, buff, 10));
    sendOther(auxchar,-1);
    return;
    }

  writeHeader(false,false);
  writeMenu(3, 5);
  writeForm(sdremhtm);

  if (auxerr!=HTTP_CODE_OK)
    {
    printP(t(noconf), menor, barra);
    printP(table, mayor);
    pc(form_f);
    printP(c(body_f), menor, barra);
    printP(thtml, mayor);
    }
  else
    {
    printP(tr);
    printColspan(4);
    printP(c(devremotot), td_f, tr_f);
    printP(td, t(dispositivo), barraesp, c(alias), td_f);
    printcampoL(1, iddevicetemp, 3, true,true);
    printColspan(2);
    printcampoC(2, aliasdevicetemp, 20, true, true, false,false);
    printP(td_f, tr_f, tr, td, t(Modo), b);
    pt(pines);
    printP(b, cuatro, guion, cinco, td_f);
    printP(td, td_f);
    printColspan(2);
    printP(td_f, tr_f, tr, td_f, td);
    printP(t(Act), b);
    printP(t(aut), barraesp);
    printP(t(versiont), actualizauttemp?th:td);
    checkBox(9, actualizauttemp,false);
    printP(actualizauttemp?th_f:td_f, td);
    printI(versinsttemp);
    printP(td_f, c(td_if), tr_f, tr, td);
    printP(c(Firmware), b);
    printP(c(tserver), td_f);
    printColspan(3);
    printcampoC(10, fwUrlBasetemp, 50, true, true, false,false);
    printP(td_f, tr_f, tr, td, t(periodoact255));

    printP(barra, t(periodoactrem), barra, t(tdp), td_f);
    printcampoI(11, peractpantemp, 5, true,true);
    printcampoI(12, peractremtemp, 5, true,true);
    printcampoL(13, TempDesactPrgtemp, 5, true,true);
    printP(tr_f);

    espacioSep(4);
    printP(tr);
    printP(td, c(ifttt), barraesp);
    printP(c(Key), td_f, iftttenabledtemp ? th : td);
    checkBox(4, iftttenabledtemp,false);
    printP(iftttenabledtemp ? th_f : td_f);
    printColspan(2);
    printcampoC(3, iftttkeytemp, 30, true, true, false,false);
    printP(td_f, tr_f, tr);

    printP(td, c(dweet), barraesp);
    printP(c(dweetname), td_f, mododweettemp ? th : td);
    checkBox(6, mododweettemp,false);
    printP(mododweettemp ? th_f : td_f);
    printColspan(2);
    if (mododweettemp==1) { pc(conuco); printP(readdescr(filemacdevrem, posdevice, 14)); }
    printP(td_f, tr_f, tr);

    printP(tr, td, href_i, comillas);
    printP(c(urliotwweet), comillas);
    printP(b, c(newpage), mayor);
    pc(iottweett);
    printP(barraesp, t(usuario), barraesp);
    printP(c(Key), href_f, td_f, iottweetenabletemp==1?th:td);
    checkBox(7, iottweetenabletemp,false);
    printP(conf.iottweetenable==1?th_f:td_f,td);
    printP(td_f, td);
    if (clientremote()) pc(hidden); else printP(iottweetusertemp);
    printP(td_f, tr_f);

    printP(td, t(latitudt), barraesp, t(longitudt), td_f, td);
    printcampoF(14, latitudtemp, 6);   printP(td_f, td);
    printcampoF(15, longitudtemp, 6);   printP(td_f, c(td_if), tr_f);

    writeFooter(guardar, false);
    }
  serversend200();
}

void ICACHE_FLASH_ATTR setupDevRemioHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  iddevicetemp = server.arg(0).toInt(); // núm. dispositivo

  int auxerr=ReqJsonConf(iddevicetemp, conf.webPort);
  if (auxerr==HTTP_CODE_OK) {
    extraevaloresTempConf(false);
    byte posdevice=99;
    for (byte i=0; i<maxdevrem; i++)  {
      if (conf.idremote[i]==iddevicetemp) posdevice=i;
      if ((conf.idremote[i]>=150) && (conf.idremote[i]<=166))
        {
        strcpy(auxdesc, aliasdevicetemp);  
        savedescr(filedevrem, auxdesc, posdevice, 20);    // guarda nombre dispositivo en devrem.txt
        for (byte j=0; j<maxsalrem; j++)                  // guarda nombres de señales en salrem.txt
          {
          if (conf.idsalremote[j]==iddevicetemp)         // coincide dispositivo
            if (moddevicetemp==8266)
              {
              if ((conf.senalrem[j]>=0) && (conf.senalrem[j]<=7))
                { 
                if (conf.senalrem[j]<=2)
                  {
                  readdescr(filedesctemp, conf.senalrem[j], 20); 
                  savedescr(filesalrem, auxdesc, j, 20); 
                  }
                else if (conf.senalrem[j]<=3)
                  {
                  readdescr(filedesctemp, conf.senalrem[j]+5, 20); 
                  savedescr(filesalrem, auxdesc, j, 20); 
                  }
                else if (conf.senalrem[j]<=5)
                  {
                  readdescr(filedesctemp, conf.senalrem[j]+6, 20); 
                  savedescr(filesalrem, auxdesc, j, 20); 
                  }
                else if (conf.senalrem[j]<=7)
                  {
                  readdescr(filedesctemp, conf.senalrem[j]+8, 20); 
                  savedescr(filesalrem, auxdesc, j, 20); 
                  }
                }
              }
            else      // tipo ESP32
              {
              if ((conf.senalrem[j]>=0) && (conf.senalrem[j]<=22))
                { 
                readdescr(filedesctemp, conf.senalrem[j], 20); 
                savedescr(filesalrem, auxdesc, j, 20); 
                }
              }
          }
        saveconf();
        }
      }
    }
  msg=vacio;
  mp=1;
  if (server.method()==HTTP_POST)
    {
    iftttpinEDtemp[0]=0; iftttpinSDtemp[0]=0;
    iftttpinEDtemp[1]=0; iftttpinSDtemp[1]=0;
    bsumatAtemp[0] = 0;
    for (int i = 0; i < server.args(); i++)
      {
      calcindices(i);
      if (param_number==0) { server.arg(i).toCharArray(auxdesc, 20); savedescr(filedesctemp, auxdesc, 0, 20); }
      else if (param_number==5) { server.arg(i).toCharArray(auxdesc, 20);  savedescr(filedesctemp, auxdesc, 1, 20); }
      else if (param_number==10) { server.arg(i).toCharArray(auxdesc, 20); savedescr(filedesctemp, auxdesc, 2, 20); }
      else if (param_number==15) { server.arg(i).toCharArray(auxdesc, 20); savedescr(filedesctemp, auxdesc, 4, 20); }
      else if (param_number==16) { server.arg(i).toCharArray(unitpinAtemp, 3); }
      else if (param_number==17) { factorAtemp[0] = server.arg(i).toFloat(); }
      else if (param_number==18) { offsetAtemp[0] = server.arg(i).toFloat(); }
      else if (param_number==19) { setbit8(bsumatAtemp, 0, 1); }
      else if (param_number==20) { server.arg(i).toCharArray(auxdesc, 20); savedescr(filedesctemp, auxdesc, 4, 20); }
      else if (param_number==24) { setbit8(iftttpinEDtemp,0,1); }
      else if (param_number==124) { setbit8(iftttpinEDtemp,8,1); }
      else if (param_number==25) { server.arg(i).toCharArray(auxdesc, 20); savedescr(filedesctemp, auxdesc, 5, 20); }
      else if (param_number==29) { setbit8(iftttpinEDtemp, 1, 1); }
      else if (param_number==129) { setbit8(iftttpinEDtemp,9, 1); }
      else if (param_number==30) { server.arg(i).toCharArray(auxdesc, 20); savedescr(filedesctemp, auxdesc, 6, 20); }
      else if (param_number==31) { valinictemp[0] = server.arg(i).toInt(); }
      else if (param_number==32) { tempdefacttemp[0] = server.arg(i).toInt(); }
      else if (param_number==33) { tempdefdestemp[0] = server.arg(i).toInt(); }
      else if (param_number==34) { setbit8(iftttpinSDtemp, 0, 1); }
      else if (param_number==134) { setbit8(iftttpinSDtemp, 8, 1); }
      else if (param_number==35) { server.arg(i).toCharArray(auxdesc, 20); savedescr(filedesctemp, auxdesc, 7, 20); }
      else if (param_number==36) { valinictemp[1] = server.arg(i).toInt(); }
      else if (param_number==37) { tempdefacttemp[1] = server.arg(i).toInt(); }
      else if (param_number==38) { tempdefdestemp[1] = server.arg(i).toInt(); }
      else if (param_number==39) { setbit8(iftttpinSDtemp, 1, 1); }
      else if (param_number==139) { setbit8(iftttpinSDtemp, 9, 1); }
      }
    sendJsonConf(iddevicetemp, conf.webPort, false, false); // envia jsonConf al remoto para configurar
    strcpy(auxchar, sdremiohtm);  strcat(auxchar, interr); strcat(auxchar, c(trem)); strcat(auxchar, itoa(iddevicetemp, buff, 10));
    sendOther(auxchar,-1);   // "sdrem?rem=xxx";
    return;
    }
//    sendJsonConf(iddevicetemp, conf.webPort, false, false); // envia jsonConf al remoto para configurar
//    strcpy(auxchar, sdremhtm);  strcat(auxchar, interr); strcat(auxchar, c(trem)); strcat(auxchar, itoa(iddevicetemp, buff, 10));
//    sendOther(auxchar,-1);
//    return;

//  if (server.args()>1) { priremio = constrain(server.arg(1).toInt(),0,7); }
  if (server.args()>2) { if (server.arg(2).toInt()==1) { addsignal(server.arg(1).toInt());  }  }
  writeHeader(false,false);
  writeMenu(3, 5);
  writeForm(sdremiohtm);

  if (auxerr!=HTTP_CODE_OK)
    {
    printP(t(noconf), menor, barra);
    printP(table, mayor);
    pc(form_f);
    printP(c(body_f), menor, barra);
    printP(thtml, mayor);
    }
  else
    {
    printP(tr);
    printP(td, t(dispositivo), barra);
    printP(c(alias), td_f, td);
    printI(iddevicetemp);
    printP(guion, aliasdevicetemp, td_f, tr_f);
    byte auxmax=moddevicetemp==8266?3:maxTemp;
    for (byte i=0; i<auxmax; i++)     // temperaturas del remoto
      {
      printP(tr,td,href_i,"\"sdrio?rem=");
      printI(iddevicetemp);
      printP("&p="); printI(i); printP("&m=1\">", t(sonda));
      printI(i+1);
      printP(href_f,td_f);
      
      printP(td,menor, c(tinput), b, type, ig);
      printP(comillas, c(ttext), comillas, b);
      printP(c(namet), ig);
      printPiP(comillas, i*5, comillas);
      printP(b, tvalue, ig, comillas,readdescr(filedesctemp, i, 20));
      printP(comillas,b,c(max_length));
      printIP(19,size_i);
      printI(19);
      printP(comillas, mayor, menor, barra, c(tinput), mayor);
      printP(td_f,tr_f);
      }

    auxmax=moddevicetemp==8266?2:maxED;   // Entradas digitales
    for (byte i=0; i<auxmax; i++)
      {
      printP(tr, td, t(entradasdig), b);
      printI(i+1);
      printP(td_f, td);
      printP(menor, c(tinput), b, type, ig, comillas);
      printP(c(ttext), comillas, b);
      printP(c(namet), ig, comillas);
      printI(20 + (i*5));
      printP(comillas, b, tvalue, ig, comillas);
      printP(readdescr(filedesctemp, i+8, 20));
      printP(comillas,b,c(max_length));
      printIP(19, size_i);
      printI(19);
      printP(comillas, mayor, menor, barra, c(tinput), mayor);
      printP(td_f, td, t(ttipo), b);
      printcampoCB(21 + (i * 5), tipoEDtemp[i], ONOFF, OFFON,dhtt,false); // número de parámetro ????

      printP(td_f, c(td_if));
      printP(c(td_if), td);
      checkBox(24+(i*5), getbit8(iftttpinEDtemp,i),false); // checkbox Notificar
      printP(barra);
      checkBox(124+(i*5), getbit8(iftttpinEDtemp, i+8),false); // checkbox Notificar
      printP(td_f, tr_f);
      }
    printP(tr, c(td_if));
    printP(c(td_if), td_f);
    printP(td, letraV, punto, b, t(inicial), td_f);
    ccell(tdefact);
    ccell(tdefdes);
    ccell(ifttt);
    printP(tr_f);

    auxmax=moddevicetemp==8266?2:maxSD;
    for (byte i=0; i<auxmax; i++)
      {
      printP(tr, td, t(saldig));
      printI(i+1);
      printP(td_f, td);

      printP(menor, c(tinput), b, type, ig, comillas);
      printP(c(ttext), comillas, b);
      printP(c(namet), ig, comillas);
      printI(30+i*5);   // número parámetro
      printP(comillas, b, tvalue, ig, comillas);
      printP(readdescr(filedesctemp, i+12, 20));
      printP(comillas,b,c(max_length));
      printIP(19, size_i);
      printI(19);
      printP(comillas, mayor, menor, barra, c(tinput), mayor);
      printP(td_f);
      printcampoCB(31+i*5, valinictemp[i], OFF, ON, ultimovalor,true);
      printcampoL(32+i*5, tempdefacttemp[i], 8, true,true);
      printcampoL(33+i*5, tempdefdestemp[i], 8, true,true);
      printP(td);
      checkBox(34+i*5, getbit8(iftttpinSDtemp, i),false); // checkbox Notificar
      printP(barra);
      checkBox(134+i*5, getbit8(iftttpinSDtemp, i + 8),false); // checkbox Notificar
      printP(td_f, tr_f);
      }
    writeFooter(guardar, false);
    }
  serversend200();
}

/*********************************************************************************************/
void ICACHE_FLASH_ATTR scanapHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  nAPact=0;
  nAP=WiFi.scanNetworks(false, false);
  msg=vacio;
  writeHeader(false,false);
  printP(menor,table, b);
  printP(c(tclass), ig, tnormal, mayor);

  for (int i=0; i<nAP; i++)
  {
    WiFi.SSID(i).toCharArray(auxchar, 20);
    printP(tr, td);
    printP(href_i, syhtm,interr,letran,ig);
    printI(i);
    printP(amper, letras,letrai, ig, cero, mayor);
    printP(auxchar, td_f, td);
    printI(WiFi.RSSI(i));
    printP(b, c(dbm), td_f, tr_f);
  }
  printP(menor, barra, table, mayor);  
  printP(c(body_f), menor, barra);
  printP(thtml, mayor);
  serversend200();
}

/*********************************************************************************************/
void ICACHE_FLASH_ATTR setupNetHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  mp=1;
  if (server.method()==HTTP_POST)
    {
    conf.staticIP=0;
    for (int i = 0; i < server.args(); i++)
      {
      calcindices(i);
      if (param_number>=0 && param_number <= 5) { server.arg(i).toCharArray(conf.EEmac[i], 3);  }
      else if (param_number>=6 && param_number <= 8)
        {
        conf.EEip[param_number-6]=server.arg(i).toInt();
        conf.EEgw[2]=conf.EEip[2];
        strcpy(hostraiz,itoa(conf.EEip[0],buff,10)); strcat(hostraiz, punto);
        strcat(hostraiz,itoa(conf.EEip[1],buff,10)); strcat(hostraiz, punto);
        strcat(hostraiz,itoa(conf.EEip[2],buff,10)); strcat(hostraiz, punto);
        }
      else if (param_number>=10 && param_number <= 13) { conf.EEmask[param_number-10] = server.arg(i).toInt(); }
      else if (param_number>=14 && param_number <= 17) { conf.EEgw[param_number-14] = server.arg(i).toInt();   }
      else if (param_number>=18 && param_number <= 21) { conf.EEdns[param_number-18] = server.arg(i).toInt();  }
      else if (param_number==22) { conf.webPort = server.arg(i).toInt();  }
      else if (param_number==41) { conf.wifimode = server.arg(i).toInt(); }
      else if (param_number==42) { server.arg(i).toCharArray(conf.ssidSTA, 20);}
      else if (param_number==43) { server.arg(i).toCharArray(conf.passSTA, 20); }
      //      else if (param_number == 44) {server.arg(i).toCharArray(conf.ssidAP,20);}
      else if (param_number==45) { server.arg(i).toCharArray(conf.passAP, 9); }
      else if (param_number==46) { conf.staticIP = server.arg(i).toInt(); }
      else if (param_number>=47 && param_number <= 50) { conf.EEdns2[param_number - 47] = server.arg(i).toInt(); }
      else if (param_number==53) { conf.timeoutrem = server.arg(i).toInt(); }
      else if (param_number==54) { conf.timeoutNTP = server.arg(i).toInt(); }
      else if (param_number==56) { conf.canalAP = server.arg(i).toInt()+1; }
      }
    //
    //    nAP = 0;
    saveconf();
    sendOther(snehtm,-1); return;
    }

  writeHeader(false,false);
  writeMenu(3, 3);
  writeForm(snehtm);

  printP(tr, td, t(Modo), b, td_f);
  printcampoCB(41, conf.wifimode, sta, ap, apsta,true);
  printP(tr_f);

  // ssid
  printP(tr);
  ccell(routerssid);
  printP(td);
  printcampoC(42, conf.ssidSTA, 20, true, true, false,false);
  printP(href_i, comillas, scanap, comillas,mayor, b);
  printP(t(explorar), href_f, td_f, tr_f);

  printparCP(c(routerpass), 43, conf.passSTA, 20, clientremote()); 

  printP(tr);
  ccell(apssid);
  cell(conf.ssidAP);
  printP(tr_f);
  printparCP(c(appass), 45, conf.passAP, 9, clientremote());
  printP(tr, td, t(canal), td_f, td);
  printP(c(Select_name),comillas);
  printIP(56, comillas);
  printP(mayor);
  for (byte j = 0; j < 13; j++)   { // canales
    pc(optionvalue);
    printPiP(comillas, j, comillas);
    if (conf.canalAP-1==j) printP(b, selected, ig, comillas, selected, comillas);
    printPiP(mayor, j+1, c(option_f));
  }
  printP(c(select_f), td_f, tr_f);

  espacioSep(2);
  printP(tr);
  ccell(MAC);
  printP(td);
  if (clientremote())  pc(hidden);
  else { for (byte i=0; i<5; i++) printP(conf.EEmac[i]); printP(conf.EEmac[5]);  }
  printP(td_f, tr_f);
  printP(tr, td, t(staticip), td_f, conf.staticIP ? th : td);
  checkBox(46,conf.staticIP,false);
  printP(conf.staticIP?th_f:td_f,tr_f);

  // print the current IP
  printP(tr, td, t(DIRIP), td_f, td);
  if (clientremote()) pc(hidden);
  else  { for (byte i=0; i<3; i++) { printI(conf.EEip[i]); printP(punto);  }  printI(conf.EEip[3]);  }
  printP(td_f, tr_f);

  printP(tr);
  ccell(tIP);
  printP(td);
  for (byte i=0; i<4; i++) printcampoI(6+i, conf.EEip[i],3,i!=3,false);
  printP(td_f, tr_f,tr);
  ccell(tmask);
  printP(td);
  for (byte i=0; i<4; i++) printcampoI(i+10, conf.EEmask[i], 3, true,false);
  printP(td_f, tr_f);
  
  printP(tr, td, c(gateway), td_f, td);
  for (byte i=0; i<4; i++) printcampoI(i+14, i==2?conf.EEip[i]:conf.EEgw[i],3,i!=2,false);
  printP(td_f, tr_f);

  printP(tr,td);
  printP("DNS");
  printP(td_f,td);
  for (byte i=0; i<4; i++) printcampoI(i+18, conf.EEdns[i], 3, true,false);
  printP(td_f, tr_f);
  
  printP(tr, td, t(ippublica), td_f, td);
  printP(clientremote() ? c(hidden) : conf.myippub, td_f, tr_f);

  //  printP(tr,td,ttimeoutrem,td_f,td);
  //  printcampoL(53, timeoutrem, 5,true);
  //  printP(td_f,tr_f);
  //  printP(tr,td,c(ttimeoutNTP),td_f,td);
  //  printcampoL(54, conf.timeoutNTP, 5,true);
  //  printP(td_f,tr_f);

  printP(menor, barra, table, mayor, menor, c(tinput));
  printP(b, type, ig, comillas, submit, comillas);
  printP(b, tvalue, ig, comillas);
  printP(tguardar, comillas);
  printP(mayor, menor, barra, c(tinput), mayor);
  pc(form_f);
  printP(c(body_f), menor, barra);
  printP(thtml, mayor);
  serversend200();
}

void ICACHE_FLASH_ATTR setupSegHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  mp=1;
  char passDevtemp1[20];
  char passDevtemp2[20];
  if (server.method()==HTTP_POST)
    {
    conf.usepassDev = 0;
    for (int i=0; i<server.args(); i++)
      {
      calcindices(i);
      if (param_number==0) conf.usepassDev = server.arg(i).toInt();
      else if (param_number==1) server.arg(i).toCharArray(conf.userDev, 20);
      else if (param_number==2) server.arg(i).toCharArray(passDevtemp1, 20);
      else if (param_number==3) server.arg(i).toCharArray(passDevtemp2, 20);
      }
    if (conf.usepassDev)    // contraseña activa
      {
      if (strcmp(passDevtemp1, passDevtemp2)==0)   // si coinciden ambas password se almacena
        strcpy(conf.passDev, passDevtemp1);
      else
        conf.usepassDev=0; // no se guarda y se desactiva contraseña
      }
    else    // contraseña NO activa
      if (strcmp(passDevtemp1, conf.passDev) != 0)  // si no se da la contraseña correcta, no se desactiva
        conf.usepassDev = 1;
    saveconf();
    sendOther(sshtm,-1); return;
    }

  /////////////////////
  writeHeader(false,false);
  writeMenu(4, 5);
  writeForm(sshtm);

  printP(tr, td, t(autenticacion), td_f, conf.usepassDev ? th : td);
  checkBox(0, conf.usepassDev,false);
  if (conf.usepassDev) printP(th_f, tr_f); else printP(td_f, tr_f);
  printparCP(t(usuario), 1, conf.userDev, 20, false);
  printparCP(t(contrasena), 2, "", 20, true);
  printparCP(t(confcontrasena), 3, "", 20, true);
  writeFooter(guardar, false);
  serversend200();
}

void ICACHE_FLASH_ATTR setupPrgHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg = vacio;
  mp = 2;
  if (server.method()==HTTP_POST)
  {
    memset(conf.actPrg, 0, sizeof(conf.actPrg));
    for (int i = 0; i < server.args(); i++)
      {
      calcindices(i);
      if (resto==0) { server.arg(i).toCharArray(auxdesc, 20); savedescr(filedescprg, auxdesc, indice, 20);   }
      else if (resto==1) conf.actPrg[indice] = server.arg(i).toInt();
      }
    saveconf();
    sendOther(sprghtm,-1);
    return;
  }

  writeHeader(false,false);
  writeMenu(2, 5);
  writeForm(sprghtm);
  printP(tr, c(td_if));
  printP(td, t(descripcion), td_f);
  printP(td, t(activo), td_f, tr_f);
  for (byte i=0; i<maxPrg; i++)  {
    mpi=mp*i;
    colorea=(conf.actPrg[i]);
    printP(tr);
    printPiP(td, i+1, td_f);
    printP(colorea?th:td);
    printP(menor, c(tinput), b, type, ig, comillas);
    printP(c(ttext), comillas, b);
    printP(c(namet), ig, comillas);
    printI(mpi);    // número de parámetro
    printP(comillas, b, tvalue, ig, comillas);
    printP(readdescr(filedescprg, i, 20));
    printP(comillas,b,c(max_length));
    printIP(19, size_i);
    printI(19);
    printP(comillas, mayor, menor, barra, c(tinput), mayor);
    printP(colorea?th_f:td_f, colorea?th:td);
    checkBox(mpi+1, colorea,false);
    printP(colorea ? th_f : td_f, tr_f);
  }
  writeFooter(guardar, false);
  server.send(200, texthtml, msg);
  msg=vacio;
}

void ICACHE_FLASH_ATTR setupWebCallHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  mp=3;
  if (server.method()==HTTP_POST)
   {
    memset(conf.bPRGwebcall, 0, sizeof(conf.bPRGwebcall));
    for (int i=0; i<server.args(); i++)
      {
      calcindices(i);
      if (resto==0) { server.arg(i).toCharArray(auxdesc, 20); savedescr(filewebcall, auxdesc, indice, 20); }
      else if (resto==1) { server.arg(i).toCharArray(auxdesc, 60); savedescr(fileurlwebcall, auxdesc, indice, 60);  }
      else if (resto==2) if (server.arg(i).toInt() == 1) setbit8(conf.bPRGwebcall, indice, 1);
      }
    saveconf();
    sendOther(swchtm,-1);
    return;
   }

  writeHeader(false,false);
  writeMenu(2, 7);
  writeForm(swchtm);
  printP(tr, c(td_if));
  tcell(descripcion);
  ccell(urltext);
  tcell(activo);
  printP(tr_f);
  for (byte i=0; i<maxwebcalls; i++)  {
    mpi = mp*i;
    colorea=(getbit8(conf.bPRGwebcall, i)==1);
    printP(tr);
    printPiP(td,i+1,td_f);

    printP(colorea?th:td);
    printP(menor, c(tinput), b, type, ig, comillas);
    printP(c(ttext), comillas, b);
    printP(c(namet), ig, comillas);
    printI(mpi);    // número de parámetro
    printP(comillas, b, tvalue, ig, comillas);
    printP(readdescr(filewebcall, i, 20));
    printP(comillas,b,c(max_length));
    printIP(19, size_i);
    printI(19);
    printP(comillas, mayor, menor, barra, c(tinput), mayor);
    printP(colorea ? th_f : td_f);

    printP(colorea ? th : td);
    printP(menor, c(tinput), b, type, ig, comillas);
    printP(c(ttext), comillas, b);
    printP(c(namet), ig, comillas);
    printI(mpi + 1);  // número de parámetro
    printP(comillas, b, tvalue, ig, comillas);
    printP(readdescr(fileurlwebcall, i, 60));
    printP(comillas,b,c(max_length));
    printIP(59, size_i);
    printI(59);
    printP(comillas, mayor, menor, barra, c(tinput), mayor);
    printP(colorea ? th_f : td_f, colorea ? th : td);

    checkBox(mpi + 2, colorea,false);
    printP(colorea ? th_f : td_f, tr_f);
  }
  writeFooter(guardar, false);
  serversend200();
}

void addSalidas(byte tipo, int npar, byte i)    // tipo: 1=semanal, 2=Condiciones, 3=fecha
{
  printP(c(Select_name),comillas);
  printI(npar);
  printP(comillas, mayor);           // señal de salida
  boolean linactiva=false;
  for (byte j=0; j < maxSD; j++)   { // añade salidas digitales locales, 8
    pc(optionvalue);
    printPiP(comillas, j, comillas);
    if (tipo==1) linactiva=(conf.prgsal[i]==j);
    else if (tipo==2) linactiva=(conf.evensal[i]==j);
    else if (tipo==3) linactiva=(conf.fecsal[i]==j);
    if (linactiva) printP(b, selected, ig, comillas, selected, comillas);
    
    if (conf.showN) printPiP(mayorparen,j,parenguion); else printP(mayor);
    printP(readdescr(filedesclocal,j+12, 20));
    pc(option_f);
    }
  for (byte j=0; j<maxEsc; j++) // añade escenas, 8
    {
    pc(optionvalue);
    printPiP(comillas, j+8, comillas);
    if (tipo==1) linactiva=(conf.prgsal[i]==j+8);
    else if (tipo==2) linactiva=(conf.evensal[i]==j+8);
    else if (tipo==3) linactiva=(conf.fecsal[i]==j+8);
    if (linactiva) printP(b, selected, ig, comillas, selected, comillas);
    if (conf.showN) printPiP(mayorparen, j, parenguion); else printP(mayor);
    printP(readdescr(filedescesc, j, 20));
    pc(option_f);
    }
  for (byte j=0; j<maxEsc; j++) // añade programas, 8
    {
    pc(optionvalue);
    printPiP(comillas, j+16, comillas);
    if (tipo==1) linactiva=(conf.prgsal[i]==j+16);
    else if (tipo==2) linactiva=(conf.evensal[i]==j+16);
    else if (tipo==3) linactiva=(conf.fecsal[i]==j+16);
    if (linactiva) printP(b, selected, ig, comillas, selected, comillas);
    if (conf.showN) printPiP(mayorparen, j, parenguion); else printP(mayor);
    printP(readdescr(filedescprg, j, 20));
    pc(option_f);
    }
  for (byte j=0; j<maxsalrem; j++) // salidas digitales remotas, hasta 32
    if (conf.idsalremote[j]>0)
      if (conf.senalrem[j]>=12)
        {
        pc(optionvalue);
        printPiP(comillas, j+100, comillas);
        if (tipo==1) linactiva=(conf.prgsal[i]==j+100);
        else if (tipo==2) linactiva=(conf.evensal[i]==j+100);
        else if (tipo==3) linactiva=(conf.fecsal[i]==j+100);
        if (linactiva) printP(b, selected, ig, comillas, selected, comillas);
        if (conf.showN) printPiP(mayorparen, j+100, parenguion); else printP(mayor);
        printP(readdescr(filesalrem,j,20));
        pc(option_f);
        }
  pc(optionvalue);      // IFTTT
  printPiP(comillas, despIFTTT, comillas);
  if (conf.evensal[i]==despIFTTT) printP(b, selected, ig, comillas, selected, comillas);
  printP(mayor);
  pc(ifttt);
  pc(option_f);
  pc(select_f);
}

void ICACHE_FLASH_ATTR setupSemHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  mp=19;  // número de parámetros por fila
  if (server.method()==HTTP_POST)
    {
    for (byte i = 0; i < maxPrg; i++) setbit8(conf.bPRGsem[posactsem], i, 0);
    memset(conf.prgdia[posactsem], 0, sizeof(conf.prgdia[posactsem]));
    for (int i = 0; i < server.args(); i++)
      {
      calcindices(i);
      if ((resto>=0) && (resto <= 7)) {setbit8(conf.bPRGsem[indice], resto, mival); } // asociacion PRG
      if (resto==8) { conf.prgsal[indice]=mival; }  // Número de salida
      if (resto==9) { setbit8(conf.bprgval, indice, mival);  } // valor de la salida
      if ((resto>=10) && (resto<=16)) { setbit8(conf.prgdia[indice], resto-10,mival); }  // domingo-sábado
      if (resto==17) { conf.prghor[indice]=constrain(mival,0,23); }   // Hora
      if (resto==18) { conf.prgmin[indice]=constrain(mival,0,59); }   // Minuto
      }
    saveconf();
    sendOther(ssehtm,-1);
    return;
    }
  if (server.args() > 0) { posactsem=constrain(server.arg(0).toInt(),0,7); }
  writeHeader(false,false);
  writeMenu(2, 1);
  writeForm(ssehtm);

  printP(tr, td, td_f,td);
  printP(t(asociara), b);
  printP(t(programa), td_f);
  printColspan(2);
  printP(t(salida), td_f, td);
  printP(t(dia), letras, td_f);
  printColspan(2);
  printP(t(hora), td_f, tr_f, tr, td, td_f);

  printP(td, c(pre_i));
  for (byte i=0; i<maxPrg; i++) { printPiP(b, i+1, b); }
  printP(c(pre_f), td_f);
  printP(td, t(nombre), td_f);
  printP(td, t(tvalor), td_f, td);
  pc(pre_i);
  printP(b, letraD, b, b);
  printP(letraL, b, b, letraM, b, b);
  printP(letraX, b, b, letraJ, b, b);
  printP(letraV, b, b, letraS);
  printP(c(pre_f), td_f);
  tcell(hora);
  ccell(minuto);
  printP(tr_f);

  for (byte i=0; i<maxPrgSem; i++)
    {
    mpi=mp*i;
    printP(tr);
    strcpy(auxchar, ssehtm); strcat(auxchar, igualp); strcat(auxchar, itoa(i, buff, 10));
    char auxO[20]="Semanal"; strcat(auxO,b); strcat(auxO,itoa(i+1,buff,10));
    printOpc(false, (i==posactsem), auxO);   // selección de fila
    
    if (posactsem==i)   // editando fila i
      {
      printP(td, c(pre_i));   // checkbox programas
      for (byte j=0; j<maxPrg; j++) checkBox(mpi+j, getbit8(conf.bPRGsem[i],j),false); // asociar a programa
      printP(c(pre_f),td_f,td);
      addSalidas(1,mpi+8,i);
      }
    else
      {
      printP(td);
      if (conf.bPRGsem[i][0]>0)   // 8 checkbox de programas
        {
        pc(pre_i);
        for (byte j=0;j<maxPrg;j++) printP(b,getbit8(conf.bPRGsem[i],j)?itoa(j+1,buff,10):b,b); // asociar a programa
        pc(pre_f);
        }
      printP(td_f, td);
      
      if (conf.bPRGsem[i][0]>0)
        {
        if (conf.prgsal[i]<=7) { printP(readdescr(filedesclocal, conf.prgsal[i]+12, 20)); }
        else if (conf.prgsal[i]<=15) { printP(readdescr(filedescesc, conf.prgsal[i]-8, 20)); }
        else if (conf.prgsal[i]<=23) { printP(readdescr(filedescprg, conf.prgsal[i]-16, 20)); }
        else if ((conf.prgsal[i]>=100) && (conf.prgsal[i]<=149)) 
          { printP(readdescr(filesalrem, conf.prgsal[i]-100, 20)); }    // salidas remotas
        }
      }
    printP(td_f,td);
    if (posactsem==i)  printcampoSiNo(mpi+9, getbit8(conf.bprgval, i)); // valor on/off
    else if (conf.bPRGsem[i][0] > 0) printP(getbit8(conf.bprgval,i)?ON:OFF);
    printP(td_f);

    printP(td, c(pre_i));
    for (byte j=0; j<7; j++)
      if (posactsem==i)
        checkBox(mpi+10+j, getbit8(conf.prgdia[i], j),false); // días de la semana
      else
        {
        printP(b);
        if (getbit8(conf.prgdia[i], j) == 1) printDiaSem(j); else printP(b);
        printP(b);    // días de la semana
        }
    printP(c(pre_f), td_f);

    if (posactsem == i)
      {
      printcampoI(mpi+17, conf.prghor[i], 2, true,true);
      printcampoI(mpi+18, conf.prgmin[i], 2, true,true); // minuto
      }
    else
      {
      printP(td);
      if (conf.bPRGsem[i][0] > 0) printI(conf.prghor[i]);
      printP(td_f,td);
      if (conf.bPRGsem[i][0] > 0) printI(conf.prgmin[i]);
      printP(td_f);
      }
    printP(tr_f);
  }
  writeFooter(guardar, false);
  serversend200();
}

void ICACHE_FLASH_ATTR setupEveHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  mp=16; // número de parámetros por fila
  if (server.method()==HTTP_POST)
    {
    for (byte i=0; i<maxPrg; i++) setbit8(conf.bPRGeve[posacteve], i, 0);
    for (int i=0; i<server.args(); i++)
      {
      calcindices(i);
      if (resto==1) setbit8(conf.bconacttipo, indice, mival);       // Tipo activadora: local o remota
      else if (resto==2) conf.condact[indice] = mival;              // señal activadora
      else if (resto==3) setbit8(conf.bconactmode, indice, mival);  // modo activación: cambio/estado
      else if (resto==4) conf.condvalD[indice] = mival;             // valor activador si digital
      else if (resto==5) conf.evencomp[indice] = mival;             // tipo comparador: menor-igual o mayor-igual
      else if (resto==6) conf.evenvalA[indice] = server.arg(i).toFloat();
      else if (resto==7) conf.evenhis[indice] = server.arg(i).toFloat();
      else if (resto==8) setbit8(conf.bconsaltipo, indice, mival);    // Tipo salida: local o remota
      else if (resto==9) conf.evensal[indice] = mival;                // señal a activar
      else if (resto==10) setbit8(conf.bevenniv, indice, mival);      // valor que debe tomar la señal de salida
      else if (resto==11) conf.evenbaseA[indice] = mival;             // señal base comparación
      else if (resto>=12) setbit8(conf.bPRGeve[indice], resto-12, mival); // asociacion PRG
      }
    saveconf();
    sendOther(svhtm,-1); return;
    }
  writeHeader(false,false);
  writeMenu(2, 2);
  writeForm(svhtm);

  printP(tr, td, td_f);
  printP(td, t(asociara), b);
  printP(t(programa), td_f);
  printColspan(2); printP(t(senal), b); printP(t(activadora),td_f);
  printColspan(4); printP(t(ana), td_f);
  printColspan(2);  
  printP(t(senal), b);
  printP(t(de),b);
  printP(t(salida), td_f);
  printP(tr_f, tr, c(td_if), td);
  pc(pre_i);
  for (byte i=0; i<maxPrg; i++) printPiP(b, i+1, b);
  pc(pre_f);
  printP(td_f, td, t(senal), td_f);
  printP(td, t(tvalor), td_f);
  printP(td, t(compa), td_f);
  printP(td, "Base", td_f);
  printP(td, "Offset", td_f);
  printP(td, t(hist), td_f);
  printP(td, t(senal), td_f);
  printP(td, ON, barra, OFF, td_f, tr_f);
  //
  /////////////////////////////////////////////////////////
  if (server.args() > 0) { posacteve = constrain(server.arg(0).toInt(), 0, 7); }
  for (byte i=0; i<nEVE; i++) // nEVE=8
    {
    colorea=false;
    boolean actdigital = (((conf.condact[i]>=8)&&(conf.condact[i]<=19)) ||
                          ((conf.condact[i]>=100)&&(conf.condact[i]<=149)));
    mpi=mp*i;
    // indice=(i*mp); // parámetros del 420/426 en adelante
    printP(tr);
    strcpy(auxchar, svhtm); strcat(auxchar, igualp); strcat(auxchar, itoa(i, buff, 10));
    char auxO[20]="Condicion"; strcat(auxO,b); strcat(auxO,itoa(i+1,buff,10));
    printOpc(false, (i==posacteve), auxO);
    if (i==posacteve)
      {
      printP(td, c(pre_i));
      for (byte j=0; j<maxPrg; j++) checkBox(mpi+12+j, getbit8(conf.bPRGeve[i],j),false);

      printP(c(pre_f), td_f, td);   // selección de señal activadora
      printP(c(Select_name),comillas);
      printIP(mpi+2, comillas);
      printP(mayor);
      for (byte j=0; j<maxTemp; j++)   { // añade temperaturas locales
        pc(optionvalue);
        printPiP(comillas, j, comillas);
        if (j==conf.condact[i]) printP(b, selected, ig, comillas, selected, comillas);
        if (conf.showN) printPiP(mayorparen, j, parenguion); else printP(mayor);
        printP(readdescr(filedesclocal,j,20));
        pc(option_f);
        }
      for (byte j=0; j<maxED; j++)  { // añade entradas digitales locales
        pc(optionvalue);
        printPiP(comillas, j+8, comillas);
        if (j+8==conf.condact[i]) printP(b, selected, ig, comillas, selected, comillas);
        if (conf.showN) printPiP(mayorparen, j+8, parenguion); else printP(mayor);
        printP(readdescr(filedesclocal,j+8,20));
        printP(c(option_f));
        }
      for (byte j=0; j<maxSD; j++)  { // añade salidas digitales locales
        pc(optionvalue);
        printPiP(comillas, j+12, comillas);
        if (j+12==conf.condact[i]) printP(b, selected, ig, comillas, selected, comillas);
        if (conf.showN) printPiP(mayorparen, j+12, parenguion); else printP(mayor);
        printP(readdescr(filedesclocal,j+12,20));
        pc(option_f);
        }
      for (byte j=0; j<maxsalrem; j++) // entradas digitales remotas
        {
        if (conf.idsalremote[j]>0)
          {
          if ((conf.senalrem[j]>=8) && (conf.senalrem[j]<=11))   {
            pc(optionvalue);
            printPiP(comillas, j+100, comillas);
            if (j+100==conf.condact[i]) printP(b, selected, ig, comillas, selected, comillas);
            if (conf.showN) printPiP(mayorparen, j+100, parenguion); else printP(mayor);
            printP(readdescr(filesalrem, j, 20));
            pc(option_f);
            }
          }
        }
      for (byte j=0; j<maxsalrem; j++) // salidas digitales remotas
        {
        if (conf.idsalremote[j]>0)
          {
          if ((conf.senalrem[j]>=12) && (conf.senalrem[j]<=19))   {
            pc(optionvalue);
            printPiP(comillas, j+100, comillas);
            if (j+100==conf.condact[i]) printP(b, selected, ig, comillas, selected, comillas);
            if (conf.showN) printPiP(mayorparen, j+100, parenguion); else printP(mayor);
            printP(readdescr(filesalrem, j, 20));
            pc(option_f);
            }
          }
        }
//      for (byte j=0; j<maxsalrem; j++) // entradas analógicas remotas (sólo si ESP8266)
//        if (conf.idsalremote[j]>0)
//          if (conf.senalrem[j]==3)   {
//            pc(optionvalue);
//            printPiP(comillas, j+160, comillas);
//            if (j+160 == conf.condact[i]) printP(b, selected, ig, comillas, selected, comillas);
//            if (conf.showN) printPiP(mayorparen, j+160, parenguion); else printP(mayor);
//            printP(readdescr(filesalrem, j,20));
//            pc(option_f);
//          }
      for (byte j=0; j<maxsalrem; j++) // temperaturas remotas
        {
        if (conf.idsalremote[j] > 32)  {
          if (conf.senalrem[j] <=7)     // temperaturas remotas
            {  
            pc(optionvalue);
            printPiP(comillas, j+200, comillas);
            if (j+200==conf.condact[i]) printP(b, selected, ig, comillas, selected, comillas);
            if (conf.showN) printPiP(mayorparen, j + 200, parenguion); else printP(mayor);
            printP(readdescr(filesalrem, j, 20));
            pc(option_f);
            }
          }
//        else if (conf.idsalremote[j] > 0)  {    // no sé qué es ésto
//          pc(optionvalue);
//          printPiP(comillas, j+220, comillas);
//          if (j + 220 == conf.condact[i]) printP(b, selected, ig, comillas, selected, comillas);
//          if (conf.showN) printPiP(mayorparen, j + 220, parenguion); else printP(mayor);
//          printP(readdescr(filesalrem, j, 20));
//          pc(option_f);
//          }
        }
      pc(optionvalue);
      printPiP(comillas, 254, comillas);
      if (254==conf.condact[i]) printP(b, selected, ig, comillas, selected, comillas);
      if (conf.showN) printPiP(mayorparen, 254, parenguion); else printP(mayor);
      pt(preciokwh);
      pc(option_f);
      pc(select_f);
      }
    else
      {
      printP(td, c(pre_i));
      if (conf.bPRGeve[i][0]>0) for (byte j=0; j<maxPrg; j++) printP(b,getbit8(conf.bPRGeve[i],j)?itoa(j+1,buff,10):b,b);
      printP(c(pre_f), td_f, td);
      if (conf.bPRGeve[i][0] > 0)
        {
        if (conf.condact[i]<=19) printP(readdescr(filedesclocal, conf.condact[i], 20));
        else if (conf.condact[i]<=149) printP(readdescr(filesalrem, conf.condact[i]-100, 20));
        else if (conf.condact[i]<=249) printP(readdescr(filesalrem, conf.condact[i]-200, 20));
        else if (conf.condact[i]==254) pt(preciokwh);
        else printP(vacio);
        }
      }
    printP(td_f);
    if (actdigital)    // la señal activadora es digital
      {
      printP(td);
      if (i==posacteve)
        printcampoSiNo(mpi + 4, conf.condvalD[i]);
      else if (conf.bPRGeve[i][0] > 0)
        printP(conf.condvalD[i] ? ON : OFF);
      printP(td_f);
      printColspan(4);
      }
    else    // activadora es analógica
      {
      if (i==posacteve)
        {
        printP(c(td_if), td);
        printP(c(Select_name),comillas);
        printI(mpi + 5);
        printP(comillas, mayor);
        for (byte j=0; j<2; j++)  { // añade >=  y  <=
          pc(optionvalue);
          printPiP(comillas, j, comillas);
          if (conf.evencomp[i] == j) printP(b, selected, ig, comillas, selected, comillas);
          printP(mayor,(j==0)?mayoroigual:menoroigual,c(option_f));
          }
        printP(c(select_f), td);

        printP(c(Select_name),comillas);    // añade base de comparación
        printIP(mpi+11, comillas);
        printP(mayor);
        for (byte j=0; j<maxTemp; j++)   { // añade temperaturas locales
          pc(optionvalue);
          printPiP(comillas, j, comillas);  //////////////////////////////////////////////////
          if (j==conf.evenbaseA[i]) printP(b, selected, ig, comillas, selected, comillas);
          if (conf.showN) printPiP(mayorparen, j, parenguion); else printP(mayor);
          printP(readdescr(filedesclocal,j,20));
          pc(option_f);
          }
        pc(optionvalue);
        printPiP(comillas, maxTemp, comillas);
        if (maxTemp==conf.evenbaseA[i]) printP(b, selected, ig, comillas, selected, comillas);
        if (maxTemp==conf.condact[i]) printP(b, selected, ig, comillas, selected, comillas);
        if (conf.showN) printPiP(mayorparen, maxTemp, parenguion); else printP(mayor);
        printP("0");
        pc(option_f);
        pc(select_f);
        printP(td_f,td);
        
        printcampoF(mpi + 6, conf.evenvalA[i], 5); // valor analógico de comparación (offset)
        printP(td_f, td);
        printcampoF(mpi + 7, conf.evenhis[i], 5); // histéresis
        printP(td_f);
        }
      else
        {
        pc(td_if);
        if (conf.bPRGeve[i][0] > 0)
          {
          printP(td,conf.evencomp[i] ? menoroigual : mayoroigual,td_f);
          if (conf.evenbaseA[i]==maxTemp)
            printP(td,"0",td_f);
          else
            printP(td,readdescr(filedesclocal,conf.evenbaseA[i],20),td_f);
          printP(td); printF(conf.evenvalA[i], 5); printP(td_f);
          printP(td); printF(conf.evenhis[i], 5); printP(td_f);
          }
        else
          printColspan(4);
      }
      printP(td_f);
    }
    printP(td);
    if (i==posacteve)
      {
      addSalidas(2,mpi+9,i);
      }
    else
      {
      if (conf.bPRGeve[i][0] > 0)
        {
        if (conf.evensal[i]<=7) printP(readdescr(filedesclocal, conf.evensal[i]+12, 20)); // 0-7
        else if (conf.evensal[i]<=15) printP(readdescr(filedescesc, conf.evensal[i]-8, 20)); // 8-15
        else if (conf.prgsal[i]<=23) { printP(readdescr(filedescprg, conf.prgsal[i]-16, 20)); }
        else if ((conf.prgsal[i]>=100) && (conf.prgsal[i]<=149)) 
          { printP(readdescr(filesalrem, conf.prgsal[i]-100, 20)); }    // salidas remotas
        else if (conf.evensal[i] == despIFTTT) pt(notifttt);  // 252
        }
      }
    printP(td_f, td);
    if (i==posacteve) printcampoSiNo(mpi+10, getbit8(conf.bevenniv, i));
    else if (conf.bPRGeve[i][0]>0) printP(getbit8(conf.bevenniv, i)?ON:OFF);
    printP(td_f, tr_f);
    }
  writeFooter(guardar, false);
  serversend200();
}

void ICACHE_FLASH_ATTR setupFecHTML()
{
  printremote();
  if (!autOK()) {sendOther(loghtm,-1); return; }
  msg=vacio;
  mp=7; // número de parámetros por fila
  if (server.method() == HTTP_POST)
  {
    memset(conf.bactfec, 0, sizeof(conf.bactfec));
    for (int i = 0; i < server.args(); i++)
    {
      calcindices(i);
      if (resto==0) setbit8(conf.bactfec, indice, server.arg(i).toInt());  // programa activo si/no
      else if (resto==1) conf.fecsal[indice] = server.arg(i).toInt();         // Número de salida
      else if (resto==2) setbit8(conf.bfecval, indice, server.arg(i).toInt());  // valor de la salida
      else if (resto==3) conf.fecdia[indice] = constrain(server.arg(i).toInt(), 1, 31); // día
      else if (resto==4) conf.fecmes[indice] = constrain(server.arg(i).toInt(), 1, 12); // mes
      else if (resto==5) conf.fechor[indice] = constrain(server.arg(i).toInt(), 0, 23); // Hora
      else if (resto==6) conf.fecmin[indice] = constrain(server.arg(i).toInt(), 0, 59); // minuto
    }
    saveconf();
    sendOther(sfhtm,-1); return;
  }

  if (server.args()>0) { posactfec = constrain(server.arg(0).toInt(), 0, 3); }
  writeHeader(false,false);
  writeMenu(2, 3);
  writeForm(sfhtm);

  printP(tr,td,td_f);
  tcell(Act);
  tcell(salida);
  tcell(tvalor);
  tcell(dia);
  tcell(mes);
  tcell(hora);
  ccell(minuto);
  printP(tr_f);
  for (byte i=0; i<maxPrgFec; i++)
  {
    mpi=mp*i;
    printP(tr);
    strcpy(auxchar, sfhtm); strcat(auxchar, igualp); strcat(auxchar, itoa(i, buff, 10));
    char auxO[20]="Fecha"; strcat(auxO,b); strcat(auxO,itoa(i+1,buff,10));
    printOpc(false, (i == posactfec), auxO);
    checkBox(mpi, getbit8(conf.bactfec, i),true);

    if (i==posactfec)
      {
      printP(td);
      addSalidas(3,mpi+1,i);
      printP(td_f, td);
      printcampoSiNo(mpi+2, getbit8(conf.bfecval, i));
      printP(td_f);
      printcampoI(mpi+3, conf.fecdia[i], 2, true,true); // día
      printcampoI(mpi+4, conf.fecmes[i], 2, true,true); // día
      printcampoI(mpi+5, conf.fechor[i], 2, true,true);
      printcampoI(mpi+6, conf.fecmin[i], 2, true,true); // minuto
      }
    else
      {
      if (getbit8(conf.bactfec, i) > 0)
        {
        printP(td);
        if (conf.fecsal[i]<=7) printP(readdescr(filedesclocal, conf.fecsal[i]+12, 20));
        else if (conf.fecsal[i]<=15) printP(readdescr(filedescesc, conf.fecsal[i]-8, 20));
        else if (conf.prgsal[i]<=23) { printP(readdescr(filedescprg, conf.prgsal[i]-16, 20)); }
        else if ((conf.prgsal[i]>=100) && (conf.prgsal[i]<=149)) 
          { printP(readdescr(filesalrem, conf.prgsal[i]-100, 20)); }    // salidas remotas
        printP(td_f);
        printP(td, getbit8(conf.bfecval, i) ? ON : OFF, td_f);
        printPiP(td,conf.fecdia[i],td_f);
        printPiP(td,conf.fecmes[i],td_f);
        printPiP(td,conf.fechor[i],td_f);
        printPiP(td,conf.fecmin[i],td_f);
        }
      else
        printColspan(6);
      }
    printP(tr_f);
  }
  writeFooter(guardar, false);
  serversend200();
}

void ICACHE_FLASH_ATTR setupEscHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  mp=8;  // número de parámetros por fila
  if (server.method()==HTTP_POST)
    {
    setbit8(conf.bshowEsc, posactesc, 0);
    for (int i=0; i<server.args(); i++)
      {
      calcindices(i);
      if (indice==0) { setbit8(conf.bshowEsc, resto, server.arg(i).toInt()); }  // mostrar escena
      else if (indice==1) { server.arg(i).toCharArray(auxdesc, 20); savedescr(filedescesc, auxdesc, resto, 20); }   // descripción escena
      else if (indice<=34) {
        setbit8(conf.bescenaact[resto], indice - 2, server.arg(i).toInt() == 0 ? 0 : 1);
        setbit8(conf.bescena[resto], indice - 2, server.arg(i).toInt() == 2 ? 1 : 0);
        }
      }
    saveconf();
    sendOther(seschtm,-1); return;
    }

  if (server.args()>0) { posactesc=constrain(server.arg(0).toInt(),0,7); }
  writeHeader(false,false);
  writeMenu(2, 4);
  writeForm(seschtm);

  printP(tr, td, t(escena), td_f);
  for (byte i=0; i<maxEsc; i++)    {
    strcpy(auxchar, seschtm); strcat(auxchar,igualp); strcat(auxchar, itoa(i,buff,10));
    char auxO[20]="Escena"; strcat(auxO,b); strcat(auxO,itoa(i+1,buff,10));
    printOpc(false, (i==posactesc), auxO);
    }

  printP(tr);
  ccell(tver);  // fila ver
  for (byte i=0; i<maxEsc; i++)
    {
    mpi=i;
    printP(td);
    if (posactesc==i) checkBox(mpi, getbit8(conf.bshowEsc, i),false);
    else printP(getbit8(conf.bshowEsc, i)?c(si):b);
    printP(td_f);
    }
  printP(tr_f);

  printP(tr, td, t(descripcion), td_f); // fila descripción
  for (byte i=0;i<maxEsc; i++)
    {
    printP(td);
    if (posactesc == i)
      {
      mpi=mp+i;
      printP(menor, c(tinput), b, type, ig, comillas);
      printP(c(ttext), comillas, b);
      printP(c(namet), ig, comillas);
      printI(mpi);
      printP(comillas, b, tvalue, ig, comillas);
      printP(readdescr(filedescesc, i, 20));
      printP(comillas,b,c(max_length));
      printIP(19, size_i);
      printI(19);
      printP(comillas, mayor, menor, barra, c(tinput), mayor);
      }
    else
      printP(getbit8(conf.bshowEsc, i) ? readdescr(filedescesc, i, 20) : puntossus);
    printP(td_f);
    }
  printP(tr_f);

  for (byte i=0; i<maxSD; i++) // para cada salida local
    {
    printP(tr, td, readdescr(filedesclocal, i+12, 20), td_f);
    for (byte j = 0; j < maxEsc; j++) // para cada escena de cada salida
      {
      mpi=(mp*(i+2)+j);
      colorea=false;
      byte valaux=(getbit8(conf.bescenaact[j],i)==0?0:getbit8(conf.bescena[j],i)+1);
      printP(td);
      if (posactesc==j)
        {
        printP(c(Select_name),comillas);
        printIP(mpi, comillas);
        printP(mayor);
        for (byte k=0; k<3; k++) {
          pc(optionvalue);
          printPiP(comillas, k, comillas);
          if (k == valaux) printP(b, selected, ig, comillas, selected, comillas);
          printP(mayor, k==0?guion:k==1?OFF:ON,c(option_f));
          }
        pc(select_f);
        }
      else
        printP(valaux==0?guion:(valaux==1?OFF:ON));
      printP(td_f);
      }
    printP(tr_f);
    }

  for (byte i=0;i<maxsalrem;i++) // para cada salida remota
    if (conf.idsalremote[i] > 0)
      if (conf.senalrem[i]>=12)   // soólo salidas
        {
        printP(tr, td, readdescr(filesalrem,i,20), td_f);
        for (byte j=0; j<maxEsc; j++) // para cada escena de cada salida
          {
          mpi=(mp*(i+4)+j); // número de parámetro
          colorea=false;
          byte valaux=(getbit8(conf.bescenaact[j],i+2)==0?0:getbit8(conf.bescena[j],i+2)+1);
          printP(td);
          if (posactesc==j)
            {
            printP(c(Select_name),comillas);
            printIP(mpi, comillas);
            printP(mayor);
            for (byte k=0;k<3;k++)  {
              pc(optionvalue);
              printPiP(comillas, k, comillas);
              if (k==valaux) printP(b, selected, ig, comillas, selected, comillas);
              printP(mayor, k == 0 ? guion : k == 1 ? OFF : ON, c(option_f));
              }
            printP(c(select_f), td_f);
            }
          else
            printP(valaux == 0 ? guion : valaux == 1 ? OFF : ON);
          printP(td_f);
          }
        printP(tr_f);
        }
  writeFooter(guardar, false);
  serversend200();
}

void ICACHE_FLASH_ATTR resetWiFi(void)
{
  dPrint(t(reiniciando)); dPrint(b); dPrint(c(twifi)); dPrint(crlf);
  conf.wifimode = 1;                  // AP
  strcpy(conf.ssidSTA, c(tssid)); strcat(conf.ssidSTA, subray); strcat(conf.ssidSTA, sta);
  strcpy(conf.passSTA, c(tpass)); strcat(conf.passSTA, subray); strcat(conf.passSTA, sta);
  strcpy(conf.ssidAP, c(CONUCO)); strcat(conf.ssidAP, subray); strcat(conf.ssidAP, itoa(conf.iddevice, buff, 10));
  strcpy(conf.passAP, t12341234);
  conf.canalAP=3;
  for (byte i=0; i<6; i++) strcpy(conf.EEmac[i], vacio);
  conf.staticIP = 1;
  conf.EEip={192,168,1,150};  conf.EEip[3]=conf.iddevice;
  conf.EEmask={255,255,255,0};
  conf.EEgw={192,168,1,1};
  conf.EEdns={8,8,8,8};
  conf.EEdns={8,8,4,4};
  conf.webPort=88;
  strcpy(conf.hostmyip,c(icanhazip));
  saveconf();
}

void initConf()
{
  ////////////////////////////////// RF
  memset(conf.rfkeys.pro, 0, sizeof(conf.rfkeys.pro));      // teclas de control: up,down,left,rigth,esc,intro
  memset(conf.rfkeys.code, 0, sizeof(conf.rfkeys.code));
  memset(conf.rfkeys.len, 0, sizeof(conf.rfkeys.len));
  memset(conf.code433.proon, 0, sizeof(conf.code433.proon));   // teclas de on/off
  memset(conf.code433.codeon, 0, sizeof(conf.code433.codeon));
  memset(conf.code433.lenon, 0, sizeof(conf.code433.lenon));
  memset(conf.code433.prooff, 0, sizeof(conf.code433.prooff));
  memset(conf.code433.codeoff, 0, sizeof(conf.code433.codeoff));
  memset(conf.code433.lenoff, 0, sizeof(conf.code433.lenoff));
  //////////////////////////////////////// DEVICE
  strcpy(conf.watermark,"conuco");
  conf.iddevice=150;
  strcpy(conf.aliasdevice, "NUEVO");
  conf.bestado=0;
  memset(conf.valinic,0,sizeof(conf.valinic));
  conf.showN=0;
  conf.peractpan=15;
  conf.prectemp=12;
  memset(conf.tipoED,0,sizeof(conf.tipoED));
  memset(conf.bPRGeve,0,sizeof(conf.bPRGeve));
  conf.usepassDev=0;                                // control usuario desactivado
  strcpy(conf.userDev,admin);                       // 20 bytes, usuario device
  strcpy(conf.passDev,admin);                       // 20 bytes, password device
  conf.iottweetenable=0;                            // IoTTweet desactivado
  memset(conf.bprgval,0,sizeof(conf.bprgval));
  for (byte i=0;i<maxEA;i++) conf.factorA[i]=1.0;
  for (byte i=0;i<maxEA;i++) conf.offsetA[i]=1.0;
  for (byte i=0;i<maxEA;i++) strcpy(conf.unitpinA[i],vacio);  // unidades entrada analógica
  for (byte i=0;i<maxEA;i++) conf.bsumatA[i]=0;             // acumular valores entrada analógica
  memset(conf.tempdefact, 0, sizeof(conf.tempdefact));
  memset(conf.tempdefdes, 0, sizeof(conf.tempdefdes));
  conf.staticIP=1;                                  // en initWiFi
  conf.TempDesactPrg=600;                           // tiempo desactivación programas si falla la hora en segundos  
  memset(conf.contadores,0,sizeof(conf.contadores));// contadores de número de activaciones de entradas digitales
  memset(conf.actPrg, 0, sizeof(conf.actPrg));
  conf.webPort=88;                                  // en initWiFi
  conf.wifimode=1;                                  // en initWifi, modo AP
  strcpy(conf.ssidSTA,"SSID_AP");
  strcpy(conf.passSTA,"PASS_AP");
  strcpy(conf.ssidAP,"CONUCO_150");
  strcpy(conf.passAP,t12341234);
  memset(conf.pinremote,0,sizeof(conf.pinremote));      // pin de cada salida remota
  memset(conf.idsalremote,0,sizeof(conf.idsalremote));  // dispositivo de cada salida remota
  memset(conf.bPRGsem,0,sizeof(conf.bPRGsem));          // 
  memset(conf.prgsal,0,sizeof(conf.prgsal));            // 
  memset(conf.prgdia,0,sizeof(conf.prgdia));            // 
  memset(conf.prghor,0,sizeof(conf.prghor));            // 
  memset(conf.prgmin,0,sizeof(conf.prgmin));            // 
  memset(conf.bshowEsc,0,sizeof(conf.bshowEsc));            // 
  memset(conf.bescena,0,sizeof(conf.bescena));            // 
  memset(conf.bescenaact,0,sizeof(conf.bescenaact));            // 
  memset(conf.bactfec,0,sizeof(conf.bactfec));            // 
  memset(conf.fecsal,0,sizeof(conf.fecsal));            // 
  memset(conf.mqttsalenable,0,sizeof(conf.mqttsalenable));            // 
  memset(conf.fecmes,0,sizeof(conf.fecmes));            // 
  memset(conf.fecdia,0,sizeof(conf.fecdia));            // 
  memset(conf.fechor,0,sizeof(conf.fechor));            // 
  memset(conf.fecmin,0,sizeof(conf.fecmin));            // 
  memset(conf.bfecval,0,sizeof(conf.bfecval));            // 
  memset(conf.bconacttipo,0,sizeof(conf.bconacttipo));            // 
  memset(conf.condact,0,sizeof(conf.condact));            // 
  memset(conf.bconactmode,0,sizeof(conf.bconactmode));            // 
  memset(conf.condvalD,0,sizeof(conf.condvalD));            // 
  memset(conf.evencomp,0,sizeof(conf.evencomp));            // 
  memset(conf.evenvalA,0,sizeof(conf.evenvalA));            // 
  memset(conf.evenhis,0,sizeof(conf.evenhis));            // 
  memset(conf.bconsaltipo,0,sizeof(conf.bconsaltipo));            // 
  memset(conf.evensal,0,sizeof(conf.evensal));            // 
  memset(conf.bevenniv,0,sizeof(conf.bevenniv));            // 
  memset(conf.EEmac,0,sizeof(conf.EEmac));            // 
  conf.EEip={192,168,1,150}; 
  conf.EEgw={192,168,1,1}; 
  conf.EEmask={255,255,255,255}; 
  conf.EEdns={8,8,8,8}; 
  conf.EEdns2={8,8,4,4}; 
  memset(conf.ipremote,0,sizeof(conf.ipremote));
  memset(conf.bshowpanel,0,sizeof(conf.bshowpanel)); 
  memset(conf.idremote,0,sizeof(conf.idremote));
  memset(conf.senalrem,0,sizeof(conf.senalrem));
  strcpy(conf.iottweetkey,vacio);              // IoTtweet account user ID (6 digits, included zero pre-fix)
  conf.timeoutrem=500;
  conf.timeoutNTP=1000;
  strcpy(conf.iottweetuser,vacio);             // IoTtweet account user ID (6 digits, included zero pre-fix)
  conf.peractrem=10;                           // período de actualización automática a dispositivo maestro
  memset(conf.tipoi2cmbrem, 0, sizeof(conf.tipoi2cmbrem));
  conf.iftttenabled=0;                          // ifttt desactivado
  strcpy(conf.iftttkey, vacio);                // IoTtweet account user ID (6 digits, included zero pre-fix)
  memset(conf.iftttpinED,0,sizeof(conf.iftttpinED));
  memset(conf.iftttpinSD,0,sizeof(conf.iftttpinSD));
  conf.mododweet=0;                            // dweet.io desactivado
  conf.canalAP=3;
  conf.mqttserverIP={192,168,1,1};
  conf.mqttmodeserverIP=0;
  conf.modoterm=0;
  conf.latitud=0.0;  conf.longitud=0.0;
  strcpy(conf.myippub,vacio);
  conf.actualizaut = 0;
  strcpy(conf.hostmyip,"icanhazip.com");
  strcpy(conf.fwUrlBase,"http://conucoserver.000webhostapp.com/");
  memset(conf.bPRGwebcall,0,sizeof(conf.bPRGwebcall));        // 
  conf.netseg=1;                                        // segmento de red local
  memset(conf.setpoint,0,sizeof(conf.setpoint));        // setpoint temperaturas
  memset(conf.salsetpoint,0,sizeof(conf.salsetpoint));  // salida asociada al setpoint (0,1 salidas locales; 2-17 salidas remotas)
  for (byte i=0;i<maxTemp;i++) conf.accsetpoint[i]=2;   // acción asociada al setpoint (0=OFF,1=ON,2=ninguna)
  memset(conf.MbC8,0,sizeof(conf.MbC8));        // 
  conf.ftpenable=1;                         // FTP activado
  conf.lang=0;                              // español, por defecto
  conf.mqttenabled=0;               // desactiva MQTT
  strcpy(conf.mqttserver,vacio);           // MQTT broker
  strcpy(conf.mqttpath[0],"conuco");
  strcpy(conf.mqttpath[1],"INSTAL");
  strcpy(conf.mqttpath[2],"150");
  strcpy(conf.mqttpath[3],vacio);strcpy(conf.mqttpath[4],vacio);strcpy(conf.mqttpath[5],vacio);
  strcpy(conf.instname,"INSTAL");
  memset(conf.mqttgpioenable,0,sizeof(conf.mqttgpioenable));        // 
  conf.modofi=0;
 // conf.ngpio={26,26,26,26,26,26,26,26,36,39,34,35,17,23,34,27,19,5,18,16,0,0,0,0,0,0,0,0,0};  // pin para cada señal 8x1-wire,4xDI,8xDO, 6 libres)
  memset(conf.tiporemote,0,sizeof(conf.tiporemote));        // 
  memset(conf.bshowbypanel,0,sizeof(conf.bshowbypanel)); 
    conf.bshowbypanel[0][0]=255;  conf.bshowbypanel[0][1]=255; 
    conf.bshowbypanel[0][2]=255;  conf.bshowbypanel[0][3]=255;
  memset(conf.gpiosensortype,0,sizeof(conf.gpiosensortype));        // 
  conf.RX433enabled=0;
  conf.TX433enabled=0;
  conf.SPIenabled=0;
  conf.I2Cenabled=0;
  conf.TFTenabled=0;
  conf.SERIAL2enabled=0;
  memset(conf.gpioalfa,0,sizeof(conf.gpioalfa));        // 
  memset(conf.gpiobeta,0,sizeof(conf.gpiobeta));        // 
  memset(conf.gpiogamma,0,sizeof(conf.gpiogamma));        // 
  memset(conf.contadoresgpio,0,sizeof(conf.contadoresgpio));        // 
  memset(conf.MbC8gpio,0,sizeof(conf.MbC8gpio));        // 
  conf.rstper=0;
  memset(conf.nprobe,0,sizeof(conf.nprobe));        // 
  memset(conf.probecode,0,sizeof(conf.probecode));        // 
  conf.Cal=0;
  conf.Ref=0;
  conf.ACS=0;
  conf.SAI=0;
  conf.T[0]=1; conf.T[1]=15; conf.T[2]=33; conf.T[3]=28;  // // -20/+20, -20/+20,  +20/+60, +20/+60
  conf.CC=5;     // de 1 a 15, temporización compresor en minutos
  conf.CCC=5;    // de 0 a 15, temporización bomba captador en minutos
  conf.FRO=7;   // de +7 a +25, consigna Frío
  conf.ECS=55;   // de +40 a +65, consigna ACS
  conf.PIS=28;   // de +25 a +35, consigna Piscina
  conf.RES=15;   // de -10 a +25, consigna Resistencia
  conf.Cr1=10;   // temporización resistencia en minutos
  conf.Cr2=5;    // tiempo en horas continuo compresor antes de activar resistencia
  conf.AoC=33;   // de +25 a +45, consigna vávula mezcladora calefacción
  conf.AoF=33;   // de +5 a +25, consigna vávula mezcladora frío
  conf.HYC=2;    // de 1 a 10, histéresis calefacción
  conf.HYE=2;    // de 1 a 10, histéresis ACS
  conf.HYF=2;    // de 1 a 10, histéresis Frío
  conf.HYr=1;    // de 1 a 10, histéresis Resistencia
  conf.bPF=2;    // banda proporcional 0..10 válvula mezcladora, frío
  conf.AH2=65;   // de +40 a +65, límite ida calefacción, sonda 2
  conf.AH4=55;   // de -15 a +65, alarma, sonda 4
  conf.Ab4=5;    // de -15 a +45, alarma capa reática o retorno captador
  conf.tLP=55;   // de +35 a +45, límite ida en modo piscina
  memset(conf.So,0,sizeof(conf.So));  // compensación sondas
  conf.ECO=0;   // modo ECO
  conf.TOF=1;    // de 1 a 24 horas, tiempo parada bomba modo ECO
  conf.TON=15;   // de a 15 minutos, tiempo arranque bomba modo ECO
  conf.Pro=0;   // modo progresivo
  conf.Tin=10;   // de 10 a 20º, temperatura inicial
  conf.Tfi=30;   // de 25 a 40º, temperatura final
  conf.Nbh=240;  // número de horas
  conf.TL2=65;   // temperatura límite salida agua
  conf.APP=0;   // Modo apoyo
  memset(conf.evenbaseA,0,sizeof(conf.evenbaseA));        // 

  
  ///////////////////////////////////// REMOTOS
  memset(mbtemp, 0, sizeof(mbtemp));         // estado relés remotos modbus (1 bit cada uno);
  memset(mbcons, 0, sizeof(mbcons));         // estado relés remotos modbus (1 bit cada uno);
  memset(mbstatus, 0, sizeof(mbstatus));     // estado relés remotos modbus (1 bit cada uno);
  /////// mqtt
  strcpy(conf.mqttpath[0],c(conuco));
  strcpy(conf.mqttpath[1],conf.instname);
  strcpy(conf.mqttpath[2],itoa(conf.iddevice,buff,10));
}

void initFiles()
{
  
}

void ICACHE_FLASH_ATTR initFab(void)
{
  MARKTIME=true;
  dPrint(t(reiniciando)); dPrint(b); dPrint(t(fabrica)); dPrint(crlf);
  initConf(); marktime("initConf");                 // variables de estructura Conf
  resetWiFi(); marktime("resetWiFi");               // WiFi y Red
  initPRG();  marktime("initPRG");                  // PROGRAMAS
  saveconf(); marktime("saveconf");
  initFiles(); marktime("initFiles");
  MARKTIME=false;
  ESP.restart();
}

void ICACHE_FLASH_ATTR resetHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  if (server.method() == HTTP_POST)
    {
    for (int i=0; i<server.args(); i++)
      {
      if (server.argName(i).toInt()==0)
        {
        byte idaccion = server.arg(i).toInt();
        if (idaccion > 0)
          {
          writeHeader(false,false);
          server.sendHeader("Connection", "close");
          server.sendHeader("Access-Control-Allow-Origin", "*");
          server.send(200, "text/html", espere);
          if (idaccion==1)      { ESP.restart(); }
          else if (idaccion==2) { ESP.restart(); }
          else if (idaccion==3) { resetWiFi();  }
          else if (idaccion==4) { initFab(); }
          }
        }
      }
    return;
    }
  writeHeader(false,false);
  writeMenu(4, 2);
  writeForm(rshtm);
  printP(tr);
  printP(td, treset, barra);
  printP(trestart, td_f);
  printcampoCB(0, 0, nohacernada, treset, trestart, tresetwifi,tresetfab,true);
  printP(tr_f);
  writeFooter(ejecutar, false);
  serversend200();
}

void ICACHE_FLASH_ATTR resetcontador(byte n)
{
  printremote();
  if (n<10) conf.contadores[n]=0;
  else conf.contadoresgpio[n-10]=0;
  saveconf();
  sendOther(barra,-1);
}

void ICACHE_FLASH_ATTR systemHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg =vacio;
  if (server.method()==HTTP_GET)
  {
    for (int i=0; i<server.args(); i++)
      {
      if (server.arg(i).compareTo("conuco")==0) {saveconf(); return; }
      else if (server.arg(i).compareTo(PSTR("rp0"))==0) { resetcontador(0); return;  }
      else if (server.arg(i).compareTo(PSTR("rp1"))==0) { resetcontador(1); return; }
      else if (server.arg(i).compareTo(PSTR("rp2"))==0) { resetcontador(2); return; }
      else if (server.arg(i).compareTo(PSTR("rp3"))==0) { resetcontador(3); return; }
      else if (server.arg(i).compareTo(PSTR("rp10"))==0) { resetcontador(10); return; }
      else if (server.arg(i).compareTo(PSTR("rp11"))==0) { resetcontador(11); return; }
      else if (server.arg(i).compareTo(PSTR("rp12"))==0) { resetcontador(12); return; }
      else if (server.arg(i).compareTo(PSTR("rp13"))==0) { resetcontador(13); return; }
      else if (server.arg(i).compareTo(PSTR("rp14"))==0) { resetcontador(14); return; }
      else if (server.arg(i).compareTo(PSTR("rp15"))==0) { resetcontador(15); return; }
      else if (server.arg(i).compareTo(PSTR("rp16"))==0) { resetcontador(16); return; }
      else if (server.arg(i).compareTo(PSTR("rp17"))==0) { resetcontador(17); return; }
      else if (server.arg(i).compareTo(PSTR("rp18"))==0) { resetcontador(18); return; }
      else if (server.arg(i).compareTo(PSTR("rp19"))==0) { resetcontador(19); return; }
      else if (server.argName(i).compareTo(PSTR("pon"))==0)
        { onoffremote(conf.idsalremote[server.arg(i-1).toInt()],conf.senalrem[server.arg(i-1).toInt()]-12, 1); }
      else if (server.argName(i).compareTo(PSTR("pof")) == 0)
        { onoffremote(conf.idsalremote[server.arg(i-1).toInt()],conf.senalrem[server.arg(i-1).toInt()]-12, 0); }
      else if (server.arg(i).compareTo(PSTR("rfon")) == 0)
        {
        if ((server.arg(i - 1).toInt() >= 0) && (server.arg(i - 1).toInt() <= 1))
          {
          conf.code433.proon[server.arg(i - 1).toInt()] = lastpro;
          conf.code433.codeon[server.arg(i - 1).toInt()] = lastcode;
          conf.code433.lenon[server.arg(i - 1).toInt()] = lastlen;
          }
        sendOther(rfhtm,-1); return;
        }
      else if (server.arg(i).compareTo(PSTR("rfoff")) == 0)
        {
        if ((server.arg(i - 1).toInt() >= 0) && (server.arg(i - 1).toInt() <= 1))
          {
          conf.code433.prooff[server.arg(i - 1).toInt()] = lastpro;
          conf.code433.codeoff[server.arg(i - 1).toInt()] = lastcode;
          conf.code433.lenoff[server.arg(i - 1).toInt()] = lastlen;
          }
        sendOther(rfhtm,-1); return;
        }
      else if (server.arg(i).compareTo(PSTR("rrfon")) == 0)
        {
        if ((server.arg(i - 1).toInt() >= 0) && (server.arg(i - 1).toInt() <= 15))
          {
          conf.code433.proon[server.arg(i - 1).toInt() + 2] = lastpro;
          conf.code433.codeon[server.arg(i - 1).toInt() + 2] = lastcode;
          conf.code433.lenon[server.arg(i - 1).toInt() + 2] = lastlen;
          }
        sendOther(rfhtm,-1); return;
        }
      else if (server.arg(i).compareTo(PSTR("rrfoff")) == 0)
        {
        if ((server.arg(i - 1).toInt() >= 0) && (server.arg(i - 1).toInt() <= 15))
          {
          conf.code433.prooff[server.arg(i - 1).toInt() + 2] = lastpro;
          conf.code433.codeoff[server.arg(i - 1).toInt() + 2] = lastcode;
          conf.code433.lenoff[server.arg(i - 1).toInt() + 2] = lastlen;
          }
        sendOther(rfhtm,-1); return;
        }
      else if (server.arg(i).compareTo(PSTR("rfkey")) == 0)
        {
        if ((server.arg(i - 1).toInt() >= 1) && (server.arg(i - 1).toInt() <= 6))
          {
          conf.rfkeys.pro[server.arg(i-1).toInt()-1] = lastpro;
          conf.rfkeys.code[server.arg(i-1).toInt()-1] = lastcode;
          conf.rfkeys.len[server.arg(i-1).toInt()-1] = lastlen;
          }
        sendOther(rfhtm,-1); return;
        }
      else if (server.arg(i).compareTo(PSTR("wc")) == 0)   {      // webcall
        msg=readdescr(fileurlwebcall, server.arg(i + 1).toInt(), 60);
        HTTPClient http;
        int port=msg.substring(msg.indexOf(":")+1, msg.indexOf("/")).toInt(); if (port==0) port=80;
        http.begin(msg.substring(0, msg.indexOf(":")), port, msg.substring(msg.indexOf("/"), msg.length()));
        http.setConnectTimeout(conf.timeoutrem);
        int httpCode = http.GET();
        http.end();
        msg=vacio;
        }
      else if (server.arg(i).compareTo(PSTR("ro")) == 0)   {        // ROOMBA
        Serial.print(server.arg(0).toInt());
        sendOther(rohtm,-1); return; 
        }
      else if (server.argName(i).compareTo(PSTR("si")) == 0)   {
        nAPact=server.arg(i-1).toInt();
        WiFi.SSID(nAPact).toCharArray(conf.ssidSTA, 20);
        saveconf();
        sendOther(snehtm,-1); return;
        }
      }
    sendOther(panelhtm, panelact);
  }
}

void ICACHE_FLASH_ATTR checkForUpdates() {
  String fwFileName = String("conuco-1-8");
  String fwFileVer = String(conf.fwUrlBase);
  fwFileVer.concat(fwFileName); fwFileVer.concat(".ver");
  HTTPClient httpClient;
  addlog(0, 0, "fwFileVer:");
  httpClient.begin(fwFileVer);
  int httpCode = httpClient.GET();
  if ( httpCode == 200) {
    String newFWVersion = httpClient.getString();
    verserver = newFWVersion.toInt();
    if (conf.actualizaut == 1)   {
      if (verserver > versinst) {
        addlog(0, 0, "Updating...");
        String fwFileBin = String(conf.fwUrlBase);
        fwFileBin.concat(fwFileName);   fwFileBin.concat(inobin);
        t_httpUpdate_return ret = ESPhttpUpdate.update(fwFileBin);
        switch (ret) {
          case HTTP_UPDATE_FAILED:
            addlog(2, ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            //            Serial.printf("UPDATE_FAILED Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            break;
          case HTTP_UPDATE_NO_UPDATES:
            addlog(2, 2, "UPDATE_NO_UPDATES");
            break;
        }
      }
      else
      {
        addlog(0, 0, t(versionact));
      }
    }
  }
  else {
    addlog(2, httpCode, t(versioncheckfail));
  }
  httpClient.end();
}

uint8_t portArray[] = {16, 5, 4, 0, 2, 14, 12, 13};
//String portMap[] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7"}; //for Wemos
String portMap[] = {"GPIO16", "GPIO5", "GPIO4", "GPIO0", "GPIO2", "GPIO14", "GPIO12", "GPIO13"};

void ICACHE_FLASH_ATTR check_if_exist_I2C() 
{
  byte error, address;
  int nDevices;
  nDevices = 0;
  for (address = 1; address < 127; address++ )  {
    // The i2c_scanner uses the return value of the Write.endTransmisstion to see if a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print(c(i2cdevnotfound));
      if (address < 16) Serial.print(cero);
      Serial.print(address, HEX);
      Serial.println(PSTR("  !"));
      nDevices++;
    } else if (error == 4) {
      Serial.print(c(unkerrorat));
      if (address < 16) Serial.print(cero);
      Serial.println(address, HEX);
    }
  } //for loop
  if (nDevices == 0)
    Serial.println(c(i2cnodevices));
  else
    printlinea(ig);
}

//void ICACHE_FLASH_ATTR scanPorts() {
//  for (uint8_t i = 0; i < sizeof(portArray); i++) {
//    for (uint8_t j = 0; j < sizeof(portArray); j++) {
//      if (i != j){
//        Serial.print("Scanning (SDA : SCL) - " + portMap[i] + " : " + portMap[j] + " - ");
//        Wire.begin(portArray[i], portArray[j]);
//        check_if_exist_I2C();
//      }
//    }
//  }
//}

void ICACHE_FLASH_ATTR lcdshowstatus()
{
  lcd.clear();
  for (byte i=0;i<2;i++) { lcd.setCursor(i*6,0); lcd.print(MbR[i]*0.01,1); lcd.print(letrac); }
  for (byte i=0;i<3;i++) { lcd.setCursor(i*4,1); lcd.print(getbit8(conf.MbC8,i)==1?ON:OFF); lcd.print(b); }
}

void ICACHE_FLASH_ATTR lcdshowconf(boolean editing)
{
  lcd.clear(); if (pendsave) lcd.print(aster); lcd.print(paract);
  lcd.setCursor(4, 0);
  if (paract == 0) lcdshowstatus();
  else if (paract == 1) {
    if (!WiFi.isConnected()) lcd.print(NO); lcd.print(b);
    lcd.print(t(tconectado)); lcd.print(b); lcd.print(letraa);
    lcd.setCursor(0, 1); lcd.print(conf.ssidSTA);
    }
  else if (paract == 2) { lcd.print(t(dispositivo)); lcd.setCursor(0,1); lcd.print(conf.iddevice); lcd.print(b); lcd.print(conf.aliasdevice); }
  else if (paract == 3) { lcd.print(t(versiont)); lcd.setCursor(0,1); lcd.print(versinst); }
  else if (paract == 4) { lcd.print(t(Modo)); lcd.setCursor(0,1); lcd.print(conf.wifimode); lcd.print(b);
                          lcd.print(conf.wifimode==0?sta:conf.wifimode==1?ap:apsta);  }
  else if (paract == 5) { lcd.print(c(tIP)); lcd.setCursor(0,1); for (byte i=0;i<4;i++) { lcd.print(conf.EEip[i]); if (i<3) lcd.print(punto); } }
  else if (paract == 6) { lcd.print(t(ippublica)); lcd.setCursor(0,1); lcd.print(conf.myippub); }
  else if (paract == 7) { lcd.print(c(routerssid)); lcd.setCursor(0,1); lcd.print(conf.ssidSTA); }
  else if (paract == 8) { lcd.print(c(routerpass)); lcd.setCursor(0,1); lcd.print(conf.passSTA); }
  else if (paract == 9) { lcd.print(t(usuario)); lcd.setCursor(0,1); lcd.print(conf.userDev); }
  else if (paract == 10) { lcd.print(c(tpassword)); lcd.setCursor(0,1); lcd.print(conf.passDev); }
  else if (paract == 11) { lcd.print(c(apssid)); lcd.setCursor(0,1); lcd.print(conf.ssidAP); }
  else if (paract == 12) { lcd.print(c(appass)); lcd.setCursor(0,1); lcd.print(conf.passAP); }
  else if (paract == 13) { lcd.print(t(canal)); lcd.setCursor(0,1); lcd.print(conf.canalAP); }
  else if (paract == 14) { lcd.print(c(MAC)); lcd.setCursor(0,1); lcd.print(mac); }
  else if (paract == 15) { lcd.print(c(watermarkt)); lcd.setCursor(0,1); lcd.print(conf.watermark); }
}

int ICACHE_FLASH_ATTR mqttextraepin(char* topic, String command)
{
  msg="";
  for (byte i=0;i<strlen(topic);i++) msg+=topic[i];
  String auxS="";
  byte i=0; boolean encontrado=false;
  while ((i<33) && (!encontrado))
    {
    auxS="/"; for (byte j=0;j<strlen(idpin[i]);j++) auxS+=idpin[i][j]; auxS+="/"; auxS+=command;
    encontrado=(msg.indexOf(auxS)>0);
    if (!encontrado) i++;
    }
  return encontrado?i:-1;
}

int ICACHE_FLASH_ATTR mqttextraeidpin(char* topic)
{
  msg="";
  for (byte i=0;i<strlen(topic);i++) msg+=topic[i];
  String auxS="";
  byte i=0; boolean encontrado=false;
  while ((i<43) && (!encontrado))
    {
    auxS="/"; for (byte j=0;j<strlen(idpin[i]);j++) auxS+=idpin[i][j];
    encontrado=(msg.indexOf(auxS)>0);
    if (!encontrado) i++;
    }
  return encontrado?i:-1;
}

int ICACHE_FLASH_ATTR mqttextraeidpinn(char* topic)
{
  msg="";  for (byte i=0;i<strlen(topic);i++) msg+=topic[i];
  String auxS="";
  byte i=0; boolean encontrado=false;
  while ((i<31) && (!encontrado))
    {
    auxS="/"; for (byte j=0;j<strlen(idpinn[i]);j++) auxS+=idpinn[i][j];
    encontrado=(msg.indexOf(auxS)>0);
    if (!encontrado) i++;
    }
  return encontrado?i:-1;
}

int ICACHE_FLASH_ATTR mqttextraeID(char* topic)   // extrae ID de cualquier topic de tipo XXX/YYY/nnn/ZZZ
{
  msg="";  for (byte i=0;i<strlen(topic);i++) msg+=topic[i];   // se convierte topic a String msg
  int j=msg.indexOf("/");  if (j>0) msg.remove(0,j+1);  
  j=msg.indexOf("/");  if (j>0) msg.remove(0,j+1); 
  j=msg.indexOf("/");  if (j>0) msg.remove(j,msg.length()-j); 
  return msg.toInt();
}

void ICACHE_FLASH_ATTR mqttpublishvalues()
{  
  for (byte i=0;i<41;i++)   // 8 temp + 4 ent. + 8 sal. + 8 consignas + 
                            // 10 Gpios + 1 iddevice + 1 IP +1 IPpub 
    {
    if (i<=7) { if (getbit8(conf.mqttsalenable,i)) mqttpublish(0,i); }           // 8 temp 
    else if (i<=11) { if (getbit8(conf.mqttsalenable,i)) { mqttpublish(0,i); mqttpublish(0,i+34); } }  //  4 ent. 
    else if (i<=19) { if (getbit8(conf.mqttsalenable,i)) { mqttpublish(0,i); mqttpublish(0,i+34); } } //  8 sal.  
    else if (i<=27) { if (getbit8(conf.mqttsalenable,i-20)) mqttpublish(0,i); }  //  8 consignas
    else if (i<=30) { mqttpublish(0,i); }    // 38-40: Id, IP, IP pub.
    else if (i<=40) { if (getbit8(conf.mqttgpioenable,i-31)) { mqttpublish(0,i); } }  // 10 Gpios  
    }
}

void ICACHE_FLASH_ATTR mqttpublishconf()
{  
  for (byte i=0;i<41;i++)   // 8 temp + 4 ent. + 8 sal. + 8 consignas + 
                            // 10 Gpios + 1 iddevice + 1 IP +1 IPpub 
    {
    if (i<=7) { if (getbit8(conf.mqttsalenable,i)) mqttpublishc(0,i); }           // 8 temp 
    else if (i<=11) { if (getbit8(conf.mqttsalenable,i)) { mqttpublishc(0,i); } } //  4 ent. 
    else if (i<=19) { if (getbit8(conf.mqttsalenable,i)) { mqttpublishc(0,i); } } //  8 sal.  
    else if (i<=27) { if (getbit8(conf.mqttsalenable,i-20)) mqttpublishc(0,i); }  //  8 consignas
    else if (i<=30) { mqttpublishc(0,i); }    // 38-40: Id, IP, IP pub.
    else if (i<=40) { if (getbit8(conf.mqttgpioenable,i-31)) { mqttpublishc(0,i); } }  // 10 Gpios  
    }
}

int buscasenalremota(byte auxID, byte auxsenal)    // busca si alguna señal remota coincide con ID y señal
{
  byte i=0; boolean encontrado=false;
  while ((i<maxsalrem) && (!encontrado))
    {
     encontrado=((auxID==conf.idsalremote[i]) && (auxsenal==conf.senalrem[i]));   
     if (encontrado) return i;
     else i++;
    }
  return -1;
}

void mqttcallback(char* topic, uint8_t* payload, unsigned int length) 
{
  msg=vacio;
  for (byte j=0; j<length;j++) msg+=(char)payload[j];
  //////////////////  DEBUG   ////////////////////////////////////
  if (strstr(topic,"/DEBUG")!=NULL) { MARKTIME=!MARKTIME; return; }  // recibido DEBUG, activas debug
  //////////////////  RCF   ////////////////////////////////////
  if (strstr(topic,"/RCF")!=NULL) { mqttpublishconf(); return; }   // publish valores conf a petición de un RCF
  ///////////////////// SET //////////////////////////////////
  int auxb=mqttextraepin(topic,"set");    // buscar si hay un comando "set"
  if ((auxb>=0) && (auxb<=7))         // set a t0-7, cambia la consigna c0-7
    {
    conf.setpoint[auxb]=msg.toFloat(); 
    saveconf(); 
    mqttpublish(0,auxb);      // temperatura
    mqttpublish(0,auxb+23);   // consigna
    return;
    }
  else if ((auxb>=12) && (auxb<=19))   // salidas digitales
    {
    if ((char)payload[0]=='0') { pinVAL(auxb-12,0,0); }
    else if ((char)payload[0]=='1') { pinVAL(auxb-12,1,0); }
    return;
    }
  else if ((auxb>=23) && (auxb<=31))     //    // consignas
    {
    conf.setpoint[auxb-23]=msg.toFloat(); 
    saveconf(); 
    mqttpublish(0,auxb-23);   // temperatura
    mqttpublish(0,auxb);      // consigna
    return;
    }
  /////////////////  STATE  ///////////////////////////////////  
  auxb=mqttextraepin(topic,"state");    // buscar si hay un comando "state" y publish valor
  if ((auxb>=0) && (auxb<=32)) { mqttpublish(0,auxb); return;  }
  
  //////////////  VALORES  /////////////////////////////////////////
  // buscar valores recibidos para cada idpin y guardar valores en su variable
  byte auxID=mqttextraeID(topic);   // extrae ID del remoto que manda el valor
  auxb=mqttextraeidpin(topic);
  if ((auxb>=0) && (auxb<=42)) 
    {
    int indRem=buscasenalremota(auxID,auxb<31?auxb:auxb-23);   
    msg=vacio; for (byte j=0; j<length;j++) msg+=(char)payload[j];
    if (indRem>-1)
      {
      actisenal[indRem]=true;
      if ((auxb>=0) && (auxb<=7))     // recibido temperatura
        { 
        sondaremote[indRem]=msg.toFloat(); 
        lasttimeremote[indRem]=(millis()/1000); 
        }        // temperaturas
      else if ((auxb>=8) && (auxb<=19))   // recibido entradas y salidas digitales
        {
        if ((char)payload[0]=='0') { setbit8(bstatremote,indRem,0); }
        else if ((char)payload[0]=='1') { setbit8(bstatremote,indRem,1); }
        lasttimeremote[indRem]=millis()/1000; 
        }
      else if ((auxb>=20) && (auxb<=27)) { setpointrem[indRem]=msg.toFloat(); } // consignas
      else if ((auxb>=31) && (auxb<=33)) { contaremote[indRem]=msg.toInt(); }   // veces ON entradas
      else if ((auxb>=34) && (auxb<=41)) { timeremote[indRem]=msg.toInt(); }    // tiempo salidas On/Off
      }
    }
   
  /////////////////////////  CONF  /////////////////////////////////
  // buscar valores de conf. recibidos para cada idpinn y guardar en "filesalrem"
  auxID=mqttextraeID(topic);   // extrae ID del remoto que manda el valor
  auxb=mqttextraeidpinn(topic);
  if ((auxb>=0) && (auxb<=29))    // 8 temp+4 ent.+8 sal.+10 gpios
    { 
    int indRem=buscasenalremota(auxID,auxb);  
    msg=vacio; for (byte j=0; j<length;j++) msg+=(char)payload[j];
    if (indRem>-1)
      {
      msg.toCharArray(auxdesc,20);
      savedescr(filesalrem,auxdesc,indRem,20);
      }
    }
}

void ICACHE_FLASH_ATTR subscribevalues()
{
  long tini=millis();
  // subscribe "RCF", para poder enviar conf si otro lo solicita
  strcpy(auxdesc,conf.mqttpath[0]); strcat(auxdesc,"/");
  for (byte j=1;j<6;j++) { if (strlen(conf.mqttpath[j])>0) {  strcat(auxdesc,conf.mqttpath[j]); strcat(auxdesc,"/"); } }
  strcat(auxdesc,"RCF");
  mqttsubscribe(auxdesc);
  // subscribe "RCF", para poder enviar conf si otro lo solicita
  strcpy(auxdesc,conf.mqttpath[0]); strcat(auxdesc,"/");
  for (byte j=1;j<6;j++) { if (strlen(conf.mqttpath[j])>0) {  strcat(auxdesc,conf.mqttpath[j]); strcat(auxdesc,"/"); } }
  strcat(auxdesc,"DEBUG");
  mqttsubscribe(auxdesc);
  for (byte i=0;i<38;i++)
    {
    boolean suscribir=false;
    if (i<20) suscribir=(getbit8(conf.mqttsalenable,i)==1);
    else if (i<28) suscribir=(getbit8(conf.mqttsalenable,i-20)==1);    // consignas 
    else if (i<38) suscribir=(getbit8(conf.mqttsalenable,i-8)==1);     // Gpios
    if (suscribir)
      {
      strcpy(auxdesc,conf.mqttpath[0]); strcat(auxdesc,"/");
      for (byte j=1;j<6;j++) { if (strlen(conf.mqttpath[j])>0) {  strcat(auxdesc,conf.mqttpath[j]); strcat(auxdesc,"/"); } }
      strcat(auxdesc,idpin[i]);
      strcat(auxdesc,"/state");
      mqttsubscribe(auxdesc);
      if ((i<8) || (i>=12))
        {
        strcpy(auxdesc,conf.mqttpath[0]); strcat(auxdesc,"/");
        for (byte j=1;j<6;j++) { if (strlen(conf.mqttpath[j])>0) {  strcat(auxdesc,conf.mqttpath[j]); strcat(auxdesc,"/"); } }
        strcat(auxdesc,idpin[i]);
        strcat(auxdesc,"/set");
        mqttsubscribe(auxdesc);
        }
      }
    }
  strcpy(auxdesc,"");
}

void ICACHE_FLASH_ATTR procesaSemanal()
{
  if ((conf.actPrg[0])||(conf.actPrg[1])||(conf.actPrg[2])||(conf.actPrg[3])|| 
      (conf.actPrg[4])||(conf.actPrg[5])||(conf.actPrg[6])||(conf.actPrg[7]))   // algún programa activo
    {   // actPRG guarda los programas activos
    for (byte i=0; i<maxPrgSem; i++)
      {
      if (((conf.actPrg[0]) && (getbit8(conf.bPRGsem[i],0))) ||
          ((conf.actPrg[1]) && (getbit8(conf.bPRGsem[i],1))) ||
          ((conf.actPrg[2]) && (getbit8(conf.bPRGsem[i],2))) ||
          ((conf.actPrg[3]) && (getbit8(conf.bPRGsem[i],3))) ||
          ((conf.actPrg[4]) && (getbit8(conf.bPRGsem[i],4))) ||
          ((conf.actPrg[5]) && (getbit8(conf.bPRGsem[i],5))) ||
          ((conf.actPrg[6]) && (getbit8(conf.bPRGsem[i],6))) ||
          ((conf.actPrg[7]) && (getbit8(conf.bPRGsem[i],7))))
        {
        if (conf.prgdia[i] != 0)                    // hay algún día marcado
          {
          if (getbit8(conf.prgdia[i], weekday()-1)==1) // coincide el día de la semana
            {
            if (conf.prghor[i] == hour())           // coincide la hora
              {
              if (conf.prgmin[i] == minute())       // coincide el minuto
                {
                if (conf.prgsal[i] <= 7)              // salida digital local
                  {
                  pinVAL(conf.prgsal[i], getbit8(conf.bprgval, i), conf.iddevice); // señal local
                  }
                else if (conf.prgsal[i] <= 15)        // escena: prgsal[i]-2
                  { onescena(conf.prgsal[i]-8); }
                else if ((conf.prgsal[i]>=100)&&(conf.prgsal[i]<=149))  // señal remota
                  {
                  if (getbit8(conf.bconactmode,i)==0) // modo ESTADO
                    {
                    if (getbit8(bstatremote,conf.prgsal[i]-100)!=getbit8(conf.bprgval, i))       // si el estado es diferente y hay que cambiarlo
                      {
                      onoffremote(conf.idsalremote[conf.prgsal[i]-100], conf.senalrem[conf.prgsal[i]-100]-12, getbit8(conf.bprgval, i)); 
                      }
                    }
                  else                             // modo CAMBIO
                    {
      //              if (getbit8(bconactcumple, i) == 0) // antes no cumplía
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  for (byte i=0; i<maxPrgSem; i++)
    if (conf.prgdia[i]!=0)                    // hay algún día marcado
      if (getbit8(conf.prgdia[i], weekday()-1)==1) // coincide el día de la semana
        if (conf.prghor[i]==hour())           // coincide la hora
          if (conf.prgmin[i]==minute())       // coincide el minuto
            if (conf.prgsal[i]>=16)         // activar/desactivar programa
              conf.actPrg[conf.prgsal[i]-16]=getbit8(conf.bprgval, i);
}

void mqttreconnect() 
  { 
  while (!PSclient.connected()) 
    {
    Serial.println("Conectando broker");
    String clientID="conuco-";
    clientID += String(random(0xffff), HEX);
    if (PSclient.connect(clientID.c_str())) 
      { 
      Serial.println("connected");
      subscribevalues();        // subscribe valores del dispositivo
      subscribevaluesrem();     // subscribe valores de remotos controlados por el dispositivo
      }
    else
      {
      Serial.print("failed, rc=");   Serial.println(PSclient.state());
      }
    }
  }

void createdashfile()
{
  File f=SPIFFS.open(filedash,letraw);
  if (f)  
    { 
    primero=true;
    f.print(corchete_i); 
    for (int i=0;i<41;i++)     
      { 
      if (i<=7)     // temperaturas
        {
        if (getbit8(conf.mqttsalenable,i)==1)
          {
          sendcomunes(f,i); 
          senddashtag(f, dashlastPayload);  senddashfloat(f, 0.0, true);
          senddashtag(f, dashmainTextSize);  senddashtext(f, medium,true); 
          senddashtag(f, dashtextcolor);  senddashint(f, 0x334CFF, true); 
          senddashtag(f, dashprefix);  senddashtext(f, vacio, true); 
          senddashtag(f, dashtopicPub); senddashpub(f, i, true,tset); 
          senddashtag(f, dashpostfix);  senddashtext(f, grados, true); 
          senddashtag(f, dashtype); senddashint(f, 1, true); 
          senddashtag(f, dashtopic); senddashpub(f, i, true, vacio); 
          senddashtag(f, dashname); senddashlocal(f, i, false); 
          f.print(llave_f); 
          }
        }
      else if (i<=11)     // entradas digitales
        {
        if (getbit8(conf.mqttsalenable,i)==1)
          {
          sendcomunes(f,i); 
          senddashtag(f, dashlastPayload);  senddashtext(f, cero, true);
          senddashtag(f, dashoffcolor);  senddashint(f, -1,true); 
          senddashtag(f, dashoncolor);  senddashint(f, -192,true); 
          senddashtag(f, dashpayloadoff);  senddashtext(f, cero,true); 
          senddashtag(f, dashpayloadon);  senddashtext(f, uno,true); 
          senddashtag(f, dashtopicPub); senddashpub(f, i, true, tstate); 
          senddashtag(f, dashtype); senddashint(f, 2, true); 
          senddashtag(f, dashtopic); senddashpub(f, i, true, vacio); 
          senddashtag(f, dashname); senddashlocal(f, i, false); 
          f.print(llave_f); 
          }
        }
      else if (i<=19)   // salidas digitales
        { 
        if (getbit8(conf.mqttsalenable,i)==1)
          {
          sendcomunes(f,i); 
          senddashtag(f, dashlastPayload);  senddashtext(f, cero, true);
          senddashtag(f, dashoffcolor);  senddashint(f, -1,true); 
          senddashtag(f, dashoncolor);  senddashint(f, -192,true); 
          senddashtag(f, dashpayloadoff);  senddashtext(f, cero,true); 
          senddashtag(f, dashpayloadon);  senddashtext(f, uno,true); 
          senddashtag(f, dashiconoff);  senddashtext(f, ic_settings_poweroff,true); 
          senddashtag(f, dashtopicPub); senddashpub(f, i, true, tset); 
          senddashtag(f, dashiconon);  senddashtext(f, ic_settings_poweron,true); 
          senddashtag(f, dashtype); senddashint(f, 2, true); 
          senddashtag(f, dashtopic); senddashpub(f, i, true, vacio); 
          senddashtag(f, dashname); senddashlocal(f, i, false); 
          f.print(llave_f); 
          }
        }
      else if (i<=27)        // consignas
        {
        if (getbit8(conf.mqttsalenable,i-20)==1)
          {
          sendcomunes(f,i); 
          senddashtag(f, dashlastPayload);  senddashfloat(f, 0.0, true);
          senddashtag(f, dashmainTextSize);  senddashtext(f, medium,true); 
          senddashtag(f, dashtextcolor);  senddashint(f, 233, true); 
          senddashtag(f, dashtopicPub); senddashpub(f, i, true,tset); 
          senddashtag(f, dashpostfix);  senddashtext(f, grados, true); 
          senddashtag(f, dashtype); senddashint(f, 1, true); 
          senddashtag(f, dashtopic); senddashpub(f, i, true, vacio); 
          senddashtag(f, dashname); f.print("\"SP "); f.print(readdescr(filedesclocal, i-20, 20)); f.print("\"");
          f.print(llave_f); 
          }
        }
      else if (i<=30)        // ID, IP, IPP
        {
        sendcomunes(f,i); 
        senddashtag(f, dashlastPayload);  senddashint(f, 0, true);
        senddashtag(f, dashmainTextSize);  senddashtext(f, "SMALL",true); 
        senddashtag(f, dashtextcolor);  senddashint(f, -192, true); 
        senddashtag(f, dashprefix);  senddashtext(f, vacio, true); 
        senddashtag(f, dashtopicPub); senddashpub(f, i, true, tstate);  
        senddashtag(f, dashpostfix); senddashtext(f, vacio,true);
        senddashtag(f, dashtype); senddashint(f, 1, true); 
        senddashtag(f, dashname); senddashtext(f, i==28?maindevice:i==29?localip:publicip,true); 
        senddashtag(f, dashtopic); senddashpub(f, i, false, vacio);
        f.print(llave_f); 
        }
      else if (i<=40)   // GPIOs
        {
        if (getbit8(conf.mqttgpioenable,i-30)==1)
          {
          sendcomunes(f,i); 
          senddashtag(f, dashlastPayload);  senddashtext(f, cero, true);
          senddashtag(f, dashoffcolor);  senddashint(f, -1,true); 
          senddashtag(f, dashoncolor);  senddashint(f, -192,true); 
          senddashtag(f, dashpayloadoff);  senddashtext(f, cero,true); 
          senddashtag(f, dashpayloadon);  senddashtext(f, uno,true); 
          if (conf.gpiosensortype[i-31]==0)   // INPUT
            {
            senddashtag(f, dashtopicPub); senddashpub(f, i, true, tstate); 
            }
          else if (conf.gpiosensortype[i-31]==1)    // OUTPUT
            {
            senddashtag(f, dashiconoff);  senddashtext(f, ic_settings_poweroff,true); 
            senddashtag(f, dashtopicPub); senddashpub(f, i, true, tset); 
            senddashtag(f, dashiconon);  senddashtext(f, ic_settings_poweron,true); 
            }
          senddashtag(f, dashtype); senddashint(f, 2, true); 
          senddashtag(f, dashtopic); senddashpub(f, i, true, vacio); 
          senddashtag(f, dashname); senddashlocal(f, i, false); 
          f.print(llave_f); 
          }
        }
      }
    for (byte i=0;i<maxsalrem;i++)    // señales remotas
      {
      if ((conf.idsalremote[i] >=150) && (conf.idsalremote[i] <=166))
        {
        if (conf.senalrem[i]<=7)       // TEMPERATURAS
          {
          sendcomunes(f,i+41); 
          senddashtag(f, dashlastPayload);  senddashfloat(f, 0.0, true);
          senddashtag(f, dashmainTextSize);  senddashtext(f, medium,true); 
          senddashtag(f, dashtextcolor);  senddashint(f, 0x334CFF, true); 
          senddashtag(f, dashprefix);  senddashtext(f, vacio, true); 
          senddashtag(f, dashtopicPub); senddashpubrem(f, i, conf.senalrem[i], true,tset);     ///////////////////////////   modificar
          senddashtag(f, dashpostfix); senddashtext(f, vacio,true);
          senddashtag(f, dashtype); senddashint(f, 1, true); 
          senddashtag(f, dashtopic); senddashpubrem(f, i, conf.senalrem[i], true, vacio); 
          senddashtag(f, dashname); senddashrem(f, i, false); 
          f.print(llave_f); 
          }
        else if (conf.senalrem[i]<=11) // ent. dig.
          {
          sendcomunes(f,i+41); 
          senddashtag(f, dashlastPayload);  senddashtext(f, cero, true);
          senddashtag(f, dashoffcolor);  senddashint(f, -1,true); 
          senddashtag(f, dashoncolor);  senddashint(f, -192,true); 
          senddashtag(f, dashpayloadoff);  senddashtext(f, cero,true); 
          senddashtag(f, dashpayloadon);  senddashtext(f, uno,true); 
          senddashtag(f, dashtopicPub); senddashpubrem(f, i, conf.senalrem[i], true, tstate); 
          senddashtag(f, dashtype); senddashint(f, 2, true); 
          senddashtag(f, dashtopic); senddashpubrem(f, i, conf.senalrem[i], true, vacio); 
          senddashtag(f, dashname); senddashrem(f, i, false); 
          f.print(llave_f); 
          }
        else if (conf.senalrem[i]<=19)      // sal. dig
          {
          sendcomunes(f,i+41); 
          senddashtag(f, dashlastPayload);  senddashtext(f, cero, true);
          senddashtag(f, dashoffcolor);  senddashint(f, -1,true); 
          senddashtag(f, dashoncolor);  senddashint(f, -192,true); 
          senddashtag(f, dashpayloadoff);  senddashtext(f, cero,true); 
          senddashtag(f, dashpayloadon);  senddashtext(f, uno,true);
          senddashtag(f, dashiconoff);  senddashtext(f, ic_settings_poweroff,true); 
          senddashtag(f, dashtopicPub); senddashpubrem(f, i, conf.senalrem[i], true, tset);     
          senddashtag(f, dashiconon);  senddashtext(f, ic_settings_poweron,true); 
          senddashtag(f, dashtype); senddashint(f, 2, true); 
          senddashtag(f, dashtopic); senddashpubrem(f, i, conf.senalrem[i], true, vacio);
          senddashtag(f, dashname); senddashrem(f, i, false); 
          f.print(llave_f); 
          }
        }
      else if (conf.idsalremote[i]>0)          // resto, I2C, BMP085,etc
        {
        sendcomunes(f,i+41); 
        senddashtag(f, dashlastPayload);  senddashtext(f, cero, true);
        senddashtag(f, dashmainTextSize);  senddashtext(f, medium,true); 
        senddashtag(f, dashtextcolor);  senddashint(f, -192, true); 
        senddashtag(f, dashprefix);  senddashtext(f, vacio, true); 
        senddashtag(f, dashpostfix);  senddashtext(f, vacio, true); 
        senddashtag(f, dashtopicPub); senddashtext(f, vacio, true); 
        senddashtag(f, dashname); senddashrem(f, i, true); 
        senddashtag(f, dashtopic); senddashi2c(f, i, conf.senalrem[i], true, vacio);
        senddashtag(f, dashtype); senddashint(f, 1, false); 
        f.print(llave_f); 
        }
      }
    f.print(corchete_f);
    f.close();   
    }
    
  if (conf.mqttenabled) {
    File f=SPIFFS.open(filedash,"r");
    if (f)  {
      if (!PSclient.connected()) 
        mqttreconnect();
      if (PSclient.beginPublish("conuco/dash", f.size(), false))
        {
        char auxb[1];
        for (int i=0;i<f.size();i++) { f.readBytes(auxb,1); PSclient.write(auxb[0]); }
        PSclient.endPublish();
        }
      f.close();    }  }    
}

void ICACHE_FLASH_ATTR setupNetServHTML()
{
  printremote();
  if (!autOK()) { sendOther(loghtm,-1); return; }
  msg=vacio;
  mp=1;
  if (server.method() == HTTP_POST)
    {
    conf.mododweet=0; conf.iftttenabled=0; conf.mqttmodeserverIP =0;
    conf.ftpenable=0; conf.mqttenabled=0; conf.iottweetenable=0;
    conf.exportardash=0;
    for (int i=0; i<server.args(); i++)
      {
      calcindices(i);
      if (param_number==0) { server.arg(i).toCharArray(conf.hostmyip, 30); }
      else if (param_number==1) { conf.ftpenable=server.arg(i).toInt();  } // ftp server enabled      }
      else if (param_number==2) { conf.iftttenabled=server.arg(i).toInt(); } // enable IFTTT
      else if (param_number==3) { server.arg(i).toCharArray(conf.iftttkey, 30); }
      else if (param_number==4) { conf.mododweet=server.arg(i).toInt(); }   // modo Dweet.io
      else if (param_number==6) { conf.iottweetenable=server.arg(i).toInt();  } // iottweet enabled      }
      else if (param_number==7) { server.arg(i).toCharArray(conf.iottweetuser, 10); }
      else if (param_number==8) { server.arg(i).toCharArray(conf.iottweetkey, 15);  }
      else if (param_number==9) { conf.mqttenabled=server.arg(i).toInt(); } // enable MQTT
      else if (param_number==10) { server.arg(i).toCharArray(conf.mqttserver, 40);  }
      else if ((param_number>=11)&&(param_number<=16)) 
                                 { server.arg(i).toCharArray(conf.mqttpath[param_number-11], 10);  }
      else if (param_number==17) { conf.mqttmodeserverIP=server.arg(i).toInt(); } // 
      else if (param_number==18) { conf.exportardash=server.arg(i).toInt(); } // 
      else if (param_number>=19) { conf.mqttserverIP[param_number-19]=server.arg(i).toInt(); }
      }
    saveconf();
    if (conf.exportardash==1) createdashfile();
    sendOther(snshtm,-1);
    return;
    }

  writeHeader(false,false);
  writeMenu(3, 4);
  writeForm(snshtm);

  printP(tr);
  tcell(ippubserver);
  printP(td, td_f);
  printColspan(2);
  printcampoC(0, conf.hostmyip, 30, true, true, false,false);
  printP(td_f, tr_f);

  printP(tr);
  tcell(ftpserver);
  checkBox(1, conf.ftpenable,true);
  printColspan(2);
  printP(tr_f);

  printP(tr, td, href_i, comillas);
  pc(thttps);
  pc(iftttcom);
  printP(comillas, b, c(newpage), mayor);
  pc(ifttt);
  printP(barraesp,c(Key), href_f, td_f, conf.iftttenabled?th:td);
  checkBox(2, conf.iftttenabled,false);
  printP(conf.iftttenabled?th_f:td_f);
  printColspan(2);
  if (clientremote()) pc(hidden); else printcampoC(3, conf.iftttkey, 30, true, true, false,false);
  printP(td_f, tr_f);

  printP(tr, td, href_i, comillas);
  pc(thttp);
  pc(dweetio);
  printP(comillas, b, c(newpage), mayor);
  pc(dweet);
  printP(barraesp,c(dweetname), href_f, td_f, conf.mododweet ? th : td);
  checkBox(4, conf.mododweet,false);
  printP(td_f);
  printColspan(2);
  if (conf.mododweet==1)
    if (clientremote()) { pc(hidden);  }
    else   {
      printP(href_i);
      pc(urldweet);
      pc(conuco); // 5 últimos dweets 24h
      printP(mac, mayor, c(conuco), mac, href_f);
    }
  printP(td_f, tr_f);

  printP(tr, td, href_i, comillas);
  pc(thttp);
  pc(iottweetcom);
  printP(comillas, b, c(newpage), mayor);
  pc(iottweett);
  printP(barraesp, t(usuario), barraesp);
  printP(c(Key), href_f, td_f, conf.iottweetenable == 1 ? th : td);
  checkBox(6, conf.iottweetenable,false);
  printP(conf.iottweetenable==1?th_f:td_f);
  if (clientremote()) ccell(hidden); else printcampoC(7, conf.iottweetuser, 10, true, true, false,true);
  if (clientremote()) ccell(hidden); else printcampoC(8, conf.iottweetkey, 15, true, true, false,true);
  printP(tr_f);

  printP(tr,td,c(mqtt),b);
  printP("enable",td_f);
  printP(conf.mqttenabled==1?th:td);
  checkBox(9, conf.mqttenabled,false);
  printP(conf.mqttenabled==1?th_f:td_f,td,td_f,tr_f);

  printP(tr);
  printColspan(2);
  printP(c(mqtt),b);
  printP(c(tserver),td_f);
  printColspan(2);
  printcampoC(10, conf.mqttserver, 40, true, true, false,false);
  printP(td_f,tr_f);

  printP(tr,td,c(mqtt),b);
  printP(c(tserver),b,IP,td_f);
  printP(conf.mqttmodeserverIP==1?th:td);
  checkBox(17, conf.mqttmodeserverIP,false);
  printP(td_f);
  printColspan(2);
  for (byte i=0; i<4; i++) printcampoI(19+i, conf.mqttserverIP[i],3,true,false);
  printP(conf.mqttmodeserverIP==1?th_f:td_f,tr_f);

  for (byte i=0;i<3;i++)
    {
    printP(tr);
    printColspan(2);
    if (i==0) { printP(c(mqtt),b); pc(tpath); }
    printP(td_f,td);
    printcampoC(11+(i*2), conf.mqttpath[i*2], 10, true, true, false,false);
    printP(barra,td_f,td);
    printcampoC(12+(i*2), conf.mqttpath[i*2+1], 10, true, true, false,false);
    printP(barra,td_f,tr_f); 
    }
  printP(tr,td,"Exportar Dash",td_f,conf.exportardash?th:td);
  checkBox(18, conf.exportardash,false);
  printP(conf.exportardash?th_f:td_f);
  printColspan(2);
  printP("topic: 'conuco/dash'",td_f,tr_f);
  
  writeFooter(guardar, false);
  serversend200();
}


void ICACHE_FLASH_ATTR procesaFechas()
{
  for (byte i = 0; i < maxPrgFec; i++)
    if (getbit8(conf.bactfec, i))
      if (conf.fecmes[i] == month())
        if (conf.fecdia[i] == day())
          if (conf.fechor[i] == hour())
            if (conf.fecmin[i] == minute())      {
              if (conf.fecsal[i] <= 3)
                pinVAL(sdPin[conf.fecsal[i]], getbit8(conf.bfecval, i), conf.iddevice);
              else
                {
                int auxerr = pinvalR(conf.idsalremote[conf.fecsal[i] - 4], conf.webPort, conf.pinremote[conf.fecsal[i] - 4], getbit8(conf.bfecval, i));
                if ((auxerr == 200) || (auxerr == 303) || (auxerr == (-11)))  {
                  setbit8(bstatremote, conf.fecsal[i] - 4, getbit8(conf.bfecval, i));
                  contaremote[conf.fecsal[i] - 4] = 0;
                  }
                else  {
                  Serial.print(c(tpinvalr)); Serial.println(auxerr); }
                }
              }
}

void ICACHE_FLASH_ATTR procesaconsignas() // procesa consignas
{
  int auxerr=0;
  for (byte i=0;i<maxTemp;i++)
    {
    if (conf.accsetpoint[i]<=1)    // poner a ON/OFF
      {
      if ((MbR[i])<(((int)(conf.setpoint[i]*100))-50))    // temperatura menor que consigna-0,5
        {
        if (conf.salsetpoint[i]<=7)    // salida local
          {
          if (conf.accsetpoint[i]==1)   // poner ON
            {
            if ((getbit8(MbC8ant,conf.salsetpoint[i])==0)) 
              pinVAL(conf.salsetpoint[i], 1, conf.iddevice); 
            }    
          else 
            {
            if ((getbit8(MbC8ant,conf.salsetpoint[i])==1)) 
              pinVAL(conf.salsetpoint[i], 0, conf.iddevice); 
            }
          }
        }
      else if (MbR[i]>=(int)(conf.setpoint[i]*100))      // temperatura mayor que consigna
        {
        if (conf.salsetpoint[i]<=7)    // salida local
          { 
          if (conf.accsetpoint[i]==1)   // poner OFF
            {
            if ((getbit8(MbC8ant,conf.salsetpoint[i])==1)) 
              pinVAL(conf.salsetpoint[i], 0, conf.iddevice);   
            }
          else
            {
            if ((getbit8(MbC8ant,conf.salsetpoint[i])==0)) 
              pinVAL(conf.salsetpoint[i], 1, conf.iddevice);   
            }
          }
        }
      }
    }
}

void ICACHE_FLASH_ATTR procesaTimeMax() // procesa tiempos máximos de ON/OFF de las salidas
{
  for (byte i=0;i<maxSD;i++)
    {
    if (getbit8(conf.MbC8,i)==valorpin[1]) // está ON
      {
      if (conf.tempdefact[i] != 0) 
        if (((millis()/1000)-tempact[i])>=conf.tempdefact[i]) pinVAL(i, 0, conf.iddevice);
      }
    else
      {
      if (conf.tempdefdes[i]!=0) 
        if (((millis()/1000)-tempdes[i])>=conf.tempdefdes[i]) pinVAL(i, 1, conf.iddevice);
      }
    }
}

void ICACHE_FLASH_ATTR leevaloresMB()
{
  for (byte i = 0; i < maxsalrem; i++)
  {
    if ((conf.idsalremote[i] > 0) && (conf.idsalremote[i] <= 32))
    {
      static uint32_t k;   k++;
      MBnode.setTransmitBuffer(0, lowWord(k));  MBnode.setTransmitBuffer(1, highWord(k));
      uint8_t auxerr2 = MBnode.readHoldingRegisters(315, 1);
      Serial.print(F("Reg 315:")); Serial.print(auxerr2, 16);
      if (auxerr2 == MBnode.ku8MBSuccess)  {
        mbtemp[i] = ((MBnode.getResponseBuffer(0) * 100) - 8000) / 2;
      }

      auxerr2 = MBnode.readHoldingRegisters(317, 1);
      Serial.print(F("  Reg 317:")); Serial.print(auxerr2, 16);
      //        auxerr2=MBnode.readHoldingRegisters(14, 1);   // en modo manual
      if (auxerr2 == MBnode.ku8MBSuccess) mbcons[i]=((MBnode.getResponseBuffer(0) * 100) - 2000) / 2;
      delay(1);
      auxerr2=MBnode.readCoils(5, 1);
      Serial.print(F("  coil 1:")); Serial.println(auxerr2, 16);
      if (auxerr2 == MBnode.ku8MBSuccess)  {
        setbit8(mbstatus, i, MBnode.getResponseBuffer(0));
      }
    }
  }
}

void ICACHE_FLASH_ATTR procesaRF()
{
  byte i=0;
  boolean encontrado=false;
  while ((i<6) && (!encontrado))
    {
    if (lastcode==conf.rfkeys.code[i])
      {
      encontrado = true;
      lcd.init();
      lcd.clear();
      if (i==0) { if (paract==1) { saveconf(); pendsave=0; } }    // Intro
      else if (i==1) { paract=0; }                                        // Esc
      else if (i==2) { paract++; if (paract > maxparam) paract=1; }       // Up
      else if (i==3) { paract--; if (paract < 1) paract = maxparam; }     // Down
      else if (i==4) { if (paract==1) { // Left
          conf.iddevice--; conf.EEip[3]=conf.iddevice;
          strcpy(conf.ssidAP, c(CONUCO)); strcat(conf.ssidAP, subray); strcat(conf.ssidAP, itoa(conf.iddevice, buff, 10));
          pendsave = true;       }  }
      else if (i==5) { if (paract==1) {   // Rigth
          conf.iddevice++; conf.EEip[3]=conf.iddevice;
          strcpy(conf.ssidAP, c(CONUCO)); strcat(conf.ssidAP, subray); strcat(conf.ssidAP, itoa(conf.iddevice, buff, 10));
          pendsave = true;  }  }
      lcdshowconf(true);
      return;
      }
    i++;
    }

  i=0;
  encontrado=false;
  while ((i<18) && (!encontrado))
    {
    if (lastcode==conf.code433.codeon[i])   {
      encontrado=true;
      if (i<=1) {
        pinVAL (sdPin[i], 1, conf.iddevice);   }
      else    {
        int auxerr=pinvalR(conf.idsalremote[i-2], conf.webPort, conf.senalrem[i-2]-6, 1);
        if ((auxerr==200) || (auxerr==303) || (auxerr == (-11))) setbit8(bstatremote,i-2,1);
        }
      }
    if (lastcode==conf.code433.codeoff[i])    {
      encontrado=true;
      if (i<=1) {
        pinVAL (sdPin[i], 0, conf.iddevice);   }
      else    {
        int auxerr = pinvalR(conf.idsalremote[i-2], conf.webPort, conf.senalrem[i-2]-6, 0);
        if ((auxerr==200) || (auxerr==303) || (auxerr==(-11))) setbit8(bstatremote,i-2,0);
        }
      }
    i++;
    }
}

void handleRF()
{
  if (mySwitch.available())    {
    int value=mySwitch.getReceivedValue();
    Serial.print("rf:"); Serial.println(value);
    if (value!=0)
      {
      lastpro=mySwitch.getReceivedProtocol();
      lastcode=mySwitch.getReceivedValue();
      lastlen=mySwitch.getReceivedBitlength();
      if (lastcode!=0) procesaRF();
      }
    mySwitch.resetAvailable();
    }
}

void ICACHE_FLASH_ATTR initHTML()
{
  server.onNotFound (htmlNotFound);
  initupdateserver();
  server.on("/f", filesHTML);
  server.on("/rs", resetHTML);
  server.on("/sy", systemHTML);
  
  if (!checkfiles()) { server.on("/", filesHTML); return;  }
  server.on("/", indexHTML);
  server.on("/dw", downloadHTML);
  server.on("/es", espsysHTML);
  server.on("/j", jsonHTML);
  server.on("/jc", jsonconfHTML);
  server.on("/l", loginHTML);
  server.on("/on", onCmd);
  server.on("/of", offCmd);
  server.on("/p", panelHTML);
  server.on("/pbc", panelbcHTML);
  server.on("/rf", setuprfHTML);
  server.on("/sb", setupbcHTML);
  server.on("/sbp", setupbyPanelHTML);
  server.on("/sc", scanapHTML);
  server.on("/sd", setupDevHTML);
  server.on("/sdr", setupDevRemHTML);
  server.on("/sdrio", setupDevRemioHTML);
  server.on("/se", setupEscHTML);
  server.on("/sf", setupFecHTML);
  server.on("/sio", setupioHTML);
  server.on("/sne", setupNetHTML);
  server.on("/sns", setupNetServHTML);
  server.on("/sp", setupPanelHTML);
  server.on("/spr", setupPrgHTML);
  server.on("/sr", setupremHTML);
  server.on("/ssr", setupsalremHTML);
  server.on("/ss", setupSegHTML);
  server.on("/sse", setupSemHTML);
  server.on("/sv", setupEveHTML);
  server.on("/swc", setupWebCallHTML);
  server.on("/t", termostatoHTML);
  server.on("/v", voicecommandHTML);
  
  server.on("/l0", handleState0In);     // Entrada digital 0
  server.on("/l1", handleState1In);     // Entrada digital 1
  server.on("/l2", handleState2In);     // Entrada digital 2
  server.on("/l3", handleState3In);     // Entrada digital 3
  server.on("/l4", handleState0Out);    // Salida digital 0
  server.on("/l5", handleState1Out);    // Salida digital 1
  server.on("/l6", handleState2Out);    // Salida digital 2
  server.on("/l7", handleState3Out);    // Salida digital 3
  server.on("/l8", handleState4Out);    // Salida digital 4
  server.on("/l9", handleState5Out);    // Salida digital 5
  server.on("/l10", handleState6Out);   // Salida digital 6
  server.on("/l11", handleState7Out);   // Salida digital 7

  server.on("/gi0", handleState0Ing);     // Entrada digital gpio 0
  server.on("/gi1", handleState1Ing);     // Entrada digital gpio 1
  server.on("/gi2", handleState2Ing);     // Entrada digital gpio 2
  server.on("/gi3", handleState3Ing);     // Entrada digital gpio 3
  server.on("/gi4", handleState4Ing);     // Entrada digital gpio 4
  server.on("/gi5", handleState5Ing);     // Entrada digital gpio 5
  server.on("/gi6", handleState6Ing);     // Entrada digital gpio 6
  server.on("/gi7", handleState7Ing);     // Entrada digital gpio 7
  server.on("/gi8", handleState8Ing);     // Entrada digital gpio 8
  server.on("/gi9", handleState9Ing);     // Entrada digital gpio 9

  server.on("/go0", handleState0Outg);     // Salida digital gpio 0
  server.on("/go1", handleState1Outg);     // Salida digital gpio 1
  server.on("/go2", handleState2Outg);     // Salida digital gpio 2
  server.on("/go3", handleState3Outg);     // Salida digital gpio 3
  server.on("/go4", handleState4Outg);     // Salida digital gpio 4
  server.on("/go5", handleState5Outg);     // Salida digital gpio 5
  server.on("/go6", handleState6Outg);     // Salida digital gpio 6
  server.on("/go7", handleState7Outg);     // Salida digital gpio 7
  server.on("/go8", handleState8Outg);     // Salida digital gpio 8
  server.on("/go9", handleState9Outg);     // Salida digital gpio 9
  

  server.on("/r0", handleStater0);         // entrada/salida digital remota 0
  server.on("/r1", handleStater1);         // entrada/salida digital remota 1
  server.on("/r2", handleStater2);
  server.on("/r3", handleStater3);
  server.on("/r4", handleStater4);
  server.on("/r5", handleStater5);
  server.on("/r6", handleStater6);
  server.on("/r7", handleStater7);
  server.on("/r8", handleStater8);
  server.on("/r9", handleStater9);
  server.on("/r10", handleStater10);
  server.on("/r11", handleStater11);
  server.on("/r12", handleStater12);
  server.on("/r13", handleStater13);
  server.on("/r14", handleStater14);
  server.on("/r15", handleStater15);
  server.on("/r16", handleStater16);
  server.on("/r17", handleStater17);
  server.on("/r18", handleStater18);
  server.on("/r19", handleStater19);
  server.on("/r20", handleStater20);
  server.on("/r21", handleStater21);
  server.on("/r22", handleStater22);
  server.on("/r23", handleStater23);
  server.on("/r24", handleStater24);
  server.on("/r25", handleStater25);
  server.on("/r26", handleStater26);
  server.on("/r27", handleStater27);
  server.on("/r28", handleStater28);
  server.on("/r29", handleStater29);
  server.on("/r30", handleStater30);
  server.on("/r31", handleStater31);         // entrada/salida digital remota 31
  
  server.on("/te0", handleStateTemp0);       // temperaturas
  server.on("/te1", handleStateTemp1);       // temperaturas
  server.on("/te2", handleStateTemp2);       // temperaturas
  server.on("/te3", handleStateTemp3);       // temperaturas
  server.on("/te4", handleStateTemp4);       // temperaturas
  server.on("/te5", handleStateTemp5);       // temperaturas
  server.on("/te6", handleStateTemp6);       // temperaturas
  server.on("/te7", handleStateTemp7);       // temperaturas

  server.on("/ge0", handleStateTemp0g);       // temperaturas
  server.on("/ge1", handleStateTemp1g);       // temperaturas
  server.on("/ge2", handleStateTemp2g);       // temperaturas
  server.on("/ge3", handleStateTemp3g);       // temperaturas
  server.on("/ge4", handleStateTemp4g);       // temperaturas
  server.on("/ge5", handleStateTemp5g);       // temperaturas
  server.on("/ge6", handleStateTemp6g);       // temperaturas
  server.on("/ge7", handleStateTemp7g);       // temperaturas
  server.on("/ge8", handleStateTemp8g);       // temperaturas
  server.on("/ge9", handleStateTemp9g);       // temperaturas

  server.on("/tt", handleStateTime);       // Pie
}

void procesaHP()
{
  
}

