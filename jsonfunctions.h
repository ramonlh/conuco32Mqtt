
void buildtipo(PGM_P cad1, PGM_P cad2, int pos)
{
  printP(comillas, cad1,cad2);
  printI(pos);
  printP(comillas,dp, llave_i);
}

void buildvalorI(PGM_P cad1, PGM_P cad2, PGM_P cad3, int pos, int value, PGM_P cadf,boolean printcoma)
{
  printP(comillas,cad1,cad2,cad3);
  if (pos>=0) printI(pos);
  printP(comillas,dp);
  printI(value);
  printP(cadf);
  if (printcoma) printP(coma);
}

void buildvalorL(PGM_P cad1, PGM_P cad2, PGM_P cad3, int pos, long value, PGM_P cadf,boolean printcoma)
{
  printP(comillas,cad1,cad2,cad3);
  if (pos>=0) printI(pos);
  printP(comillas,dp);
  printL(value);
  printP(cadf);
  if (printcoma) printP(coma);
}

void buildvalorF(PGM_P cad1, PGM_P cad2, PGM_P cad3, int pos, float value, byte deci, PGM_P cadf,boolean printcoma)
{
  printP(comillas,cad1,cad2,cad3);
  if (pos>=0) printI(pos);
  printP(comillas,dp);
  printF(value,deci);
  printP(cadf);
  if (printcoma) printP(coma);
}

void buildvalorC(PGM_P cad1, PGM_P cad2, PGM_P cad3, int pos, char *value, PGM_P cadf,boolean printcoma)
{
  printP(comillas,cad1,cad2,cad3);
  if (pos>=0) printI(pos);
  printP(comillas,dp,comillas,value,comillas);
  printP(cadf);
  if (printcoma) printP(coma);
}

void ICACHE_FLASH_ATTR buildJson()
{
  printP(llave_i);
  printP(comillas,letraM,comillas,dp); printI(modelo); printP(coma);
  printP(comillas,MC,comillas,dp,comillas); for (byte i=0;i<6;i++) printP(conf.EEmac[i]);      // MAC
  printP(comillas,coma);
  printP(comillas,letraD,letraV,comillas); printPiP(dp,conf.iddevice,coma);                   // DV Device
  
  for (byte i=0; i<maxTemp; ++i) buildvalorF(letrat,vacio,vacio,i,(MbR[i]*0.01),2,vacio,true);  // TEMPERATURAS
  for (byte i=0; i<maxED; ++i) 
    { 
    if (conf.tipoED[i]==0)      // ON/OFF
      buildvalorI(letrae,vacio,vacio,i,getbit8(conf.MbC8,i+8),vacio,true);   // ENTRADAS DIGITALES
    else if (conf.tipoED[i]==1)      //OFF/ON
      {
      if (getbit8(conf.MbC8,i+8)==0)
        buildvalorI(letrae,vacio,vacio,i,1,vacio,true);   // ENTRADAS DIGITALES invertida
      else
        buildvalorI(letrae,vacio,vacio,i,0,vacio,true);   // ENTRADAS DIGITALES invertida
      }
    else if (conf.tipoED[i]==2)
      {
      buildvalorI(letrae,vacio,vacio,i,2,vacio,true);   // DHT
      buildvalorF(letrad,letrah,letrat,i,dhtdata[i][0],2,vacio,true);   // temperatura
      buildvalorF(letrad,letrah,letrah,i,dhtdata[i][1],2,vacio,true);   // humedad
      }
    }
  for (byte i=0; i<maxSD; ++i) buildvalorI(letras,vacio,vacio,i,getbit8(conf.MbC8,i),vacio,true);  // SALIDAS DIGITALES
  for (byte i=0; i<maxED; ++i) buildvalorL(letrat,letrae,vacio,i,conf.contadores[i],vacio,true);   // contador ENTRADAS DIGITALES
  for (byte i=0; i<maxSD; ++i) // tiempos de SALIDAS DIGITALES
    buildvalorL(letrat,letras,vacio,i,(millis()/1000)-((getbit8(conf.MbC8,i)==0)?tempdes[i]:tempact[i]),vacio,i<maxSD-1);   
/**  for (byte i=0; i<maxSD; ++i)    
    {
    long segundos = (millis()/1000)-((getbit8(conf.MbC8,i)==0)?tempdes[i]:tempact[i]);
    printP(comillas,letrat,letras);
    printIP(i,comillas);
    printP(dp);
    printL((millis()/1000)-((getbit8(conf.MbC8,i)==0)?tempdes[i]:tempact[i]));
    if (i<maxSD-1) printP(coma);
    }   **/
 printP(llave_f);
}


void ICACHE_FLASH_ATTR buildJsonConf(boolean remoto, boolean sendpass, boolean resetear)
{
  printP(llave_i);
  printP(comillas,letraM,comillas,dp); printI(modelo); printP(coma);
  printP(comillas,letraS,letraS,comillas,dp,comillas); printP(sendpass?remoto?ssidSTAtemp:conf.ssidSTA:vacio,comillas,coma);  // SSID
  printP(comillas,letraP,letraS,comillas,dp,comillas); printP(sendpass?remoto?passSTAtemp:conf.passSTA:vacio,comillas,coma);  // PASS
  
  printP(comillas,MC,comillas,dp,comillas);   // MAC 
    for (byte i=0;i<6;i++) printP(conf.EEmac[i]); printP(comillas,coma);
  buildvalorI(letraI,letraD,vacio,-1,remoto?iddevicetemp:conf.iddevice,vacio,true);          // DV, device
  buildvalorI(letraR,letraS,letraT,-1,resetear?1:0,vacio,true);                              // rst, Resetear
  buildvalorI(letraV,vacio,vacio,-1,versinst,vacio,true);                                    // v, versión
  buildvalorI(letraA,letraA,vacio,-1,remoto?actualizauttemp:conf.actualizaut,vacio,true);    // aa, actualización automática
  buildvalorC(letraA,letraL,vacio,-1,remoto?aliasdevicetemp:conf.aliasdevice,vacio,true);    // a1, alias
  buildvalorI(letraI,letraF,vacio,-1,remoto?iftttenabledtemp:conf.iftttenabled,vacio,true);  // if, IFTTT enable
  buildvalorC(letraI,letraF,letraK,-1,remoto?iftttkeytemp:conf.iftttkey,vacio,true);         // k, IFTTT key
  buildvalorI(letraD,letraW,vacio,-1,remoto?mododweettemp:conf.mododweet,vacio,true);        // dw, dweet enable 
  buildvalorL(letraP,letraA,letraP,-1,remoto?peractpantemp:conf.peractpan,vacio,true);       // pap, período refresco panel
  buildvalorL(letraP,letraA,letraR,-1,remoto?peractremtemp:conf.peractrem,vacio,true);       // par, período actualización remotos
  buildvalorL(letraT,letraD,letraP,-1,remoto?TempDesactPrgtemp:conf.TempDesactPrg,vacio,true);   // tdp, tiempo desactivación programas
  buildvalorI(letraI,letraO,letraW,-1,remoto?iottweetenabletemp:conf.iottweetenable,vacio,true); // iot, IoTweet enable
  printP(comillas,letraI,letraO,letraW,letraU,comillas); printP(dp,comillas,remoto?iottweetusertemp:conf.iottweetuser,comillas,coma); // iotu, IoTweet user
  printP(comillas,letraI,letraO,letraW,letraP,comillas); printP(dp,comillas,remoto?iottweetkeytemp:conf.iottweetkey,comillas,coma);   // iotk, IoTweet pass
  buildvalorF(letraL,letraA,letraT,-1,remoto?latitudtemp:conf.latitud,6,vacio,true);             // lat, latitud
  buildvalorF(letraL,letraO,letraN,-1,remoto?longitudtemp:conf.longitud,6,vacio,true);           // lon, longitud
  for (byte i=0; i<maxTemp; ++i) // TEMPERATURAS                                              //  temperaturas
    {
    buildvalorC(letrat,letrad,vacio,i,remoto?readdescr(confiles[filedesctemp],i,20):readdescr(confiles[filedesclocal],i,20),vacio,true);  
    buildvalorI(letrat,letram,vacio,i,getbit8(conf.mqttsalenable,i),vacio,true);  
    }
  for (byte i=0; i<maxED; ++i)    // ENTRADAS DIGITALES
    {
    buildvalorC(letrae,letrad,vacio,i,remoto?readdescr(confiles[filedesctemp],i+8,20):readdescr(confiles[filedesclocal],i+8,20),vacio,true); // ed, descriptor
    buildvalorI(letrae,letrat,vacio,i,remoto?tipoEDtemp[i]:conf.tipoED[i],vacio,true);                                   // et, tipo
    buildvalorI(letrae,letraf,vacio,i,remoto?iftttpinEDtemp[i]:conf.iftttpinED[i],vacio,true);                          // ifen, IFTTT activo
    buildvalorI(letrae,letram,vacio,i,getbit8(conf.mqttsalenable,i+8),vacio,true);   
    }
  for (byte i=0; i<maxSD; ++i)    // SALIDAS DIGITALES
    {
    buildvalorC(letras,letrad,vacio,i,remoto?readdescr(confiles[filedesctemp],i+12,20):readdescr(confiles[filedesclocal],i+12,20),vacio,true);  // sn, descriptor
    buildvalorI(letras,letrai,vacio,i,remoto?valinictemp[i]:conf.valinic[i],vacio,true);                                 // vin, valor inicial
    buildvalorL(letras,letrao,letran,i,remoto?tempdefacttemp[i]:conf.tempdefact[i],vacio,true);                           // onn, tiempo máximo ON
    buildvalorL(letras,letrao,letraf,i,remoto?tempdefdestemp[i]:conf.tempdefdes[i],vacio,true);                          // offn, tiempo máximo OFF
    buildvalorI(letras,letraf,vacio,i,remoto?iftttpinSDtemp[i]:conf.iftttpinSD[i],vacio,true);                          // ifsn, IFTTT activo
    buildvalorI(letras,letram,vacio,i,getbit8(conf.mqttsalenable,i+12),vacio,true);
    }
  for (byte i=0; i<maxgpiovar; ++i)    // GPios 
    {
    buildvalorC(letrag,letrad,vacio,i,readdescr(confiles[filedescgpio],i,20),vacio,true);       // sn, descriptor
    buildvalorI(letrag,letrat,vacio,i,conf.gpiosensortype[i],vacio,true);            // tipo gpio
    buildvalorI(letrag,letram,vacio,i,getbit8(conf.mqttsalenable,i+20),vacio,true);  // mqtt enable
    buildvalorF(letrag,letraa,vacio,i,conf.gpioalfa[i],5,vacio,true);               // gpio alfa
    buildvalorF(letrag,letrab,vacio,i,conf.gpiobeta[i],5,vacio,true);               // gpio beta
    buildvalorF(letrag,letrac,vacio,i,conf.gpiogamma[i],5,vacio,true);               // gpio gamma
    }
  printP(comillas,letraP,letraA,letraT,letraH,comillas);    // PATH Mqtt
  printP(dp,comillas);
  strcpy(auxdesc,conf.mqttpath[0]); strcat(auxdesc,"/");
  for (byte j=1;j<6;j++) { if (strlen(conf.mqttpath[j])>0) { strcat(auxdesc,conf.mqttpath[j]); strcat(auxdesc,"/"); } }
  printP(auxdesc);  
  printP(comillas,coma);             
  printP(comillas,letraF,letraS,letraV,comillas);    // fsv, URLbase
  printP(dp,comillas,remoto?fwUrlBasetemp:conf.fwUrlBase,comillas);                                                 // fsv, URLbase
  printP(llave_f);
}

int ICACHE_FLASH_ATTR ReqJson(int ip, int port) // pide json a remoto 
{
  createhost(ip);
  msg=vacio;
  printP(barra,json,interr,letrar,ig);
  printI(conf.iddevice);
  return callhttpGET(host,port,true,conf.timeoutrem);
}

int ICACHE_FLASH_ATTR ReqJsonConf(int ip, int port) // pide jsonconf a remoto
{
  createhost(ip);
  msg=vacio;
  printP(barra,jsonconf);
  printP(interr,letrao,ig,itoa(conf.iddevice,buff,10));
  if (ip==1) printP(interr,letram,ig,uno);
  return callhttpGET(host,port,true,conf.timeoutrem);
}

int ICACHE_FLASH_ATTR sendJsonConf(int ip, int port, boolean sendpass,boolean resetear) // envia json conf, recibe el comando "/rjc"->parsejconf->saveconf
{
  createhost(ip);
  msg=vacio;
  buildJsonConf(true,sendpass,resetear);
  HTTPClient http;
  http.begin(host,port,barrarjc);
  http.addHeader(type, tPOST);
  http.addHeader(contenttype, applicationjson);
  http.addHeader(dataType, json);
  http.setConnectTimeout(conf.timeoutNTP);
  int httpCode=http.POST(msg);
  if (httpCode>0) {  msg=http.getString();  }
  http.end();
  msg=vacio;
  return httpCode;
}

int ICACHE_FLASH_ATTR sendJson(int ip, int port) // envia json al master/ o a los masters
{
  createhost(ip);
  msg=vacio;
  printP(barra,rjson,interr,c(tdata),ig);
  buildJson();
  return callhttpGET(host,port,false,conf.timeoutrem);
}
