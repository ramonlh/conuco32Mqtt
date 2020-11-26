TFT_eSPI_Button btPar1[10];         // buttons parametros
char btPar1text[10][10]={"T1","T2","T3","T4","CC","CCC","FRO","ACS","PIS","RES"};
TFT_eSPI_Button btPar2[10];         // buttons parametros
char btPar2text[10][10]={"HYC","HYE","HYF","HYr","Cr1","Cr2","AH2","AH4","Ab4","RES"};
TFT_eSPI_Button btYN[3];          // buttons OK / Cancel / Backspace
TFT_eSPI_Button btKey[50];        // buttons Key 0..9, A..Z & symbols
char btYNtext[3][8]={"OK","Cancel","<--"};
char btKeytext[50][2]={"0","1","2","3","4","5","6","7","8","9",
                       "A","B","C","D","E","F","G","H","I","J",
                       "K","L","M","N","x","O","P","Q","R","S",
                       "T","U","V","W","X","Y","Z","x","x","x",
                       "-","=","_",".",",",";","/","(",")","x"};

void clearTFT()
{
  if (conf.TFTenabled==0) return;
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

void displayYN(boolean enok, boolean encancel, boolean enback)
{
  // botones Yes/Cancel/backspace
  if (enok)
    {
    btYN[0].initButtonUL(&tft,0,210,90,30,2,TFT_WHITE,TFT_BLACK,btYNtext[0],2);
    btYN[0].drawButton();
    }
  if (encancel)
    {
    btYN[1].initButtonUL(&tft,100,210,90,30,2,TFT_WHITE,TFT_BLACK,btYNtext[1],2);
    btYN[1].drawButton();
    }
  if (enback)
    {
    btYN[2].initButtonUL(&tft,200,210,90,30,2,TFT_WHITE,TFT_BLACK,btYNtext[2],2);
    btYN[2].drawButton();
    }
}

void displayKey(byte mode) // 0: all keys, 1:numeric, 2:alfa only  
{ 
  if (mode<=1)
    for (byte i=0;i<10;i++)
      {
      btKey[i].initButtonUL(&tft,31*(i%10),35+(31*int(i/10)),30,30,2,TFT_WHITE,TFT_BLACK,btKeytext[i],2);
      btKey[i].drawButton();
      }
  if(mode !=1)
    for (byte i=10;i<50;i++)
      {
      btKey[i].initButtonUL(&tft,31*(i%10),35+(31*int(i/10)),30,30,2,TFT_WHITE,TFT_BLACK,btKeytext[i],2);
      btKey[i].drawButton();
      }
  displayYN(true,true,true);
}

long getNumberTFT(long valini, byte len,char *tunits)
{
  clearTFT();
  tft.setTextSize(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(tunits,200,0);
  displayKey(1);
  strcpy(auxtft,itoa(valini,buff,10));
  tft.setTextSize(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(auxtft,0,0);
  Serial.print(valini);Serial.print("-"); Serial.print(itoa(valini,buff,10));Serial.print("-"); Serial.println(auxtft);
  uint16_t x, y;
  while (true) 
    {
    if (tft.getTouch(&x, &y))
      {
      if (tft.getRotation()==1) { y=tft.height()-y;}
      else if (tft.getRotation()==3) { x=tft.width()-x; y=tft.height()-y;}
      for (byte i=0; i<10; i++)    // check KeyN buttons
        {
        if (btKey[i].contains(x,y)) 
          {
          delay(100);
          if (strlen(auxtft)<len)
            {
            strcat(auxtft, itoa(i,buff,10));
            tft.setTextSize(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.drawString(auxtft,0,0);
            }
          }
        }
      for (byte i=0; i<3; i++)    // check YN buttons
        {
        if (btYN[i].contains(x,y)) 
          {
          delay(100);
          if (i==0) { return atol(auxtft);   }   // OK
          else if (i==1) { return -1; }     // Cancel
          else if (i==2)                    // Backspace
            { 
            if (strlen(auxtft)>0) auxtft[strlen(auxtft)-1]=0; 
            tft.drawString("                    ",0,0);  
            tft.drawString(auxtft,0,0); }     
          }
        }
      }
    }
}

void drawST(boolean flechas, boolean escok)     // barra navegación
{
  if (conf.TFTenabled==0) return;
  if (flechas)
    {
    btST[0].initButtonUL(&tft,0,200,75,40,2,TFT_WHITE,TFT_BLACK,flecha[0],2);  btST[0].drawButton();
    btST[3].initButtonUL(&tft,245,200,75,40,2,TFT_WHITE,TFT_BLACK,flecha[3],2);  btST[3].drawButton();
    }
  if (escok)
    {
    btST[1].initButtonUL(&tft,80,200,75,40,2,TFT_WHITE,TFT_BLACK,"ESC",2);  btST[1].drawButton();
    btST[2].initButtonUL(&tft,165,200,75,40,2,TFT_WHITE,TFT_BLACK,"OK",2);  btST[2].drawButton();
    }
  else
    {
    tft.setTextSize(2);
    tft.setTextColor(WiFi.isConnected()?TFT_GREEN:TFT_RED, TFT_BLACK);
    tft.drawString(WiFi.isConnected()?"CON":"DIS",105,220);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawNumber(tftpage,85,220);
    if (hour()<10) tft.drawNumber(0,167,220); 
    tft.drawNumber(hour(),hour()<10?167:155,220); 
    tft.drawString(":",178,220);
    if (minute()<10) tft.drawNumber(0,188,220); 
    tft.drawNumber(minute(),minute()<10?200:188,220); 
    tft.drawString(":",210,220);
    if (second()<10) { tft.drawNumber(0,220,220); }
    tft.drawNumber(second(),second()<10?232:220,220);   
    }
}

void readtextTFT()
{
  for (byte i=0;i<5;i++)
    { btKEY[i].initButtonUL(&tft,60*i,0,58,40,2,TFT_WHITE,TFT_BLACK,itoa(i,buff,10),2); btKEY[i].drawButton(); }
  for (byte i=5;i<10;i++)
    { btKEY[i].initButtonUL(&tft,60*(i-5),50,58,40,2,TFT_WHITE,TFT_BLACK,itoa(i,buff,10),2); btKEY[i].drawButton(); }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("                           ",0,150);
  tft.drawString(auxtft,0,150);
  drawST(true,true);
}

void drawStarting()
  {
  if (conf.TFTenabled==0) return;
  clearTFT();
  tft.setTextSize(2);
  tft.drawString("Starting",0,0);
  tft.drawString("Vers.",0,25);
  tft.drawNumber(versinst,100,25);
  tft.drawString("Connecting WiFi",0,50);
  }

void initTFT()
{
  if (conf.TFTenabled==0) return;
  pinMode(TFT_RST,OUTPUT);
  digitalWrite(TFT_RST,0);delay(100); digitalWrite(TFT_RST,1);
  tft.init(); tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  lasttouch=millis();
  tfton=true;
  drawStarting(); 
}

void drawTE()   // temperaturas
{
  if (conf.TFTenabled==0) return;
  tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.setTextSize(2);
  for (byte i=0;i<4;i++)
      {
      tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.setTextSize(2);
      tft.drawString(readdescr(filedesclocal,i,20),0,50*i+15);
      dtostrf(MbR[i]*0.01,4,1,auxdesc);
      btTE[i].initButtonUL(&tft,100,50*i,58,40,2,TFT_WHITE,TFT_BLACK,auxdesc,2);
      btTE[i].drawButton();
      }
  for (byte i=4;i<maxTemp;i++)
      {
      tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.setTextSize(2);
      tft.drawString(readdescr(filedesclocal,i,20),165,50*(i-4)+15);
      dtostrf(MbR[i]*0.01,4,1,auxdesc);
      btTE[i].initButtonUL(&tft,265,50*(i-4),58,40,2,TFT_WHITE,TFT_BLACK,auxdesc,2);
      btTE[i].drawButton();
      }
}

void drawSO()   // sondas y códigos
{
  if (conf.TFTenabled==0) return;
  tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.setTextSize(2);
  for (byte i=0;i<maxTemp;i++)
    {
    tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.setTextSize(2);
    tft.drawString(letraS,0,25*i); tft.drawNumber(i, 15, 25*i);
    if (conf.nprobe[i]>0) 
      {
      tft.drawNumber(conf.nprobe[i], 35,25*i);
      for (byte j=0;j<8;j++) 
        {
        tft.drawHex(conf.probecode[i][j], 75+(j*30), 25*i);
        if (j<7) tft.drawString(":", 97+(j*30), 25*i);
        }
      }
    else 
      tft.drawString("NA",35,25*i);
    }
}

void drawED()   // entradas digitales
{
  if (conf.TFTenabled==0) return;
  for (byte i=0;i<4;i++)
    {
    btED[i].initButtonUL(&tft,0,50*i,158,40,2,getbit8(conf.MbC8,i+8)==1?TFT_YELLOW:TFT_WHITE,TFT_BLACK,readdescr(filedesclocal,i+8,20),2);
    btED[i].drawButton();
    tft.setTextColor(TFT_WHITE, TFT_BLACK);  tft.setTextSize(2);
    tft.drawString("      ", 180,50*i+10);
    tft.drawNumber(conf.contadores[i],180, 50*i+10); 
    }
}

void drawSD()   // salidas digitales
{
  if (conf.TFTenabled==0) return;
  for (byte i=0;i<4;i++)
    {
    btSD[i].initButtonUL(&tft,0,50*i,158,40,2,getbit8(conf.MbC8,i)==1?TFT_YELLOW:TFT_WHITE,TFT_BLACK,readdescr(filedesclocal,i+12,20),2);
    btSD[i].drawButton();
    }
  for (byte i=4;i<maxSD;i++)
    {
    btSD[i].initButtonUL(&tft,160,50*(i-4),158,40,2,getbit8(conf.MbC8,i)==1?TFT_YELLOW:TFT_WHITE,TFT_BLACK,readdescr(filedesclocal,i+12,20),2);
    btSD[i].drawButton();
    }
}

int checkYN()
{
  uint16_t x, y;
  if (tft.getTouch(&x, &y))
    {
    if (tft.getRotation()==1) { y=tft.height()-y;}
    else if (tft.getRotation()==3) { x=tft.width()-x; y=tft.height()-y;}
    for (byte i=0;i<3;i++)
      if (btYN[i].contains(x,y)) { delay(100); return i;  }
    }
  return -1;
}

int hayalarma()
{
  
}

void drawAlarma()
{
  byte i=0;
  while ((i<10) && (estalarma[i]==0)) { i++; }
  if (i==10) return;
  if (estalarma[i]==1)      // pendiente reconocer   
    {
    clearTFT();
    tft.setTextSize(5); tft.setTextColor(TFT_YELLOW);
    tft.drawString("Alarma",0,50);
    tft.drawString(textalarma[i],200,50);
    displayYN(true,false,false);
    }
}

void displayBtBC()
{
  btBC[0].initButtonUL(&tft,0,0,60,40,2,conf.Cal==1?CALisON?TFT_GREEN:TFT_YELLOW:TFT_LIGHTGREY,TFT_BLACK,"CAL",2);
  btBC[0].drawButton();
  btBC[1].initButtonUL(&tft,0,45,60,40,2,conf.Ref==1?REFisON?TFT_GREEN:TFT_YELLOW:TFT_LIGHTGREY,TFT_BLACK,"REF",2);
  btBC[1].drawButton();
  btBC[2].initButtonUL(&tft,0,90,60,40,2,conf.ACS==1?ACSisON?TFT_GREEN:TFT_YELLOW:TFT_LIGHTGREY,TFT_BLACK,"ACS",2);
  btBC[2].drawButton();

  btBC[6].initButtonUL(&tft,260,0,60,30,2,getbit8(conf.MbC8,2)==1?TFT_GREENYELLOW:TFT_LIGHTGREY,TFT_BLACK,"BCal",2);
  btBC[6].drawButton();
  btBC[7].initButtonUL(&tft,260,32,60,30,2,getbit8(conf.MbC8,3)==1?TFT_GREENYELLOW:TFT_LIGHTGREY,TFT_BLACK,"BCap",2);
  btBC[7].drawButton();
  btBC[8].initButtonUL(&tft,260,64,60,30,2,getbit8(conf.MbC8,4)==1?TFT_GREENYELLOW:TFT_LIGHTGREY,TFT_BLACK,"BAcs",2);
  btBC[8].drawButton();
  btBC[9].initButtonUL(&tft,260,96,60,30,2,getbit8(conf.MbC8,5)==1?TFT_GREENYELLOW:TFT_LIGHTGREY,TFT_BLACK,"Comp",2);
  btBC[9].drawButton();
  btBC[10].initButtonUL(&tft,260,128,60,30,2,getbit8(conf.MbC8,1)==1?TFT_GREENYELLOW:TFT_LIGHTGREY,TFT_BLACK,"V4V",2);
  btBC[10].drawButton();
  
  btBC[11].initButtonUL(&tft,0,157,60,40,2,idbc[idTerm]==1?TFT_YELLOW:TFT_LIGHTGREY,TFT_BLACK,"Term",2);
  btBC[11].drawButton();
  btBC[12].initButtonUL(&tft,65,157,60,40,2,idbc[idACS]==1?TFT_YELLOW:TFT_LIGHTGREY,TFT_BLACK,"ACS",2);
  btBC[12].drawButton();
  btBC[13].initButtonUL(&tft,130,157,60,40,2,idbc[idBP]==1?TFT_RED:TFT_LIGHTGREY,TFT_BLACK,"BP",2);
  btBC[13].drawButton();
  btBC[14].initButtonUL(&tft,195,157,60,40,2,idbc[idHP]==1?TFT_RED:TFT_LIGHTGREY,TFT_BLACK,"HP",2);
  btBC[14].drawButton();
  btBC[15].initButtonUL(&tft,260,157,60,40,2,idbc[idFrio]==1?TFT_YELLOW:TFT_LIGHTGREY,TFT_BLACK,"Frio",2);
  btBC[15].drawButton();
}

void displayValBC()
{
  tft.setTextSize(2);  
  tft.setTextColor(REFisON?TFT_YELLOW:TFT_WHITE, TFT_BLACK);  
  tft.drawString(AiText[1],70,5);  tft.drawNumber(Ai[1],120,5);
  tft.setTextColor(CALisON?TFT_YELLOW:TFT_WHITE, TFT_BLACK);  
  tft.drawString(AiText[0],160,5);  tft.drawNumber(Ai[0],220,5);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  
  tft.drawString(AiText[2],70,25);  tft.drawNumber(Ai[2],120,25);
  tft.drawString(AiText[3],160,25);  tft.drawNumber(Ai[3],220,25);

  tft.setTextColor(CALisON?TFT_YELLOW:TFT_WHITE, TFT_BLACK);  
  tft.drawString("Consigna CAL",70,45);  tft.drawNumber(conf.T[3],230,45);
  tft.setTextColor(REFisON?TFT_YELLOW:TFT_WHITE, TFT_BLACK);  
  tft.drawString("Consigna FRIO",70,65);  tft.drawNumber(conf.FRO,230,65);
  tft.setTextColor(ACSisON?TFT_YELLOW:TFT_WHITE, TFT_BLACK);  
  tft.drawString("Consigna ACS",70,85);  tft.drawNumber(conf.ECS,230,85);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);  
}

void displayAlarma()
{
  tft.setTextSize(3); tft.setTextColor(TFT_ORANGE, TFT_BLACK);  
  if (tipoalarma1<9)
    {
    tft.drawString("Alarma",70,120);
    tft.drawString(textalarma[tipoalarma1],190,120);
    }
  else
    tft.drawString("          ",70,120);
}

void drawBC()   // principal bomba de calor
{
  if (conf.TFTenabled==0) return;
 /** if (tipoalarma>0) 
    {
    if (ackalarma==2)   // alarma reconocida
      {
      tft.setTextSize(3); tft.setTextColor(TFT_YELLOW);
      tft.drawString("Alarma",70,125);
      tft.drawString(textalarma[tipoalarma],200,125);
      }
    }**/
  displayBtBC();
  displayValBC();
  displayAlarma();
}

void drawBCSet1()   // parámetros nivel 1
{
  if (conf.TFTenabled==0) return;
  clearTFT();
  tft.setTextSize(2);  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  
  tft.drawString("Parametros nivel 1",0,0);

  for (byte i=0;i<5;i++)
    {
    btPar1[i].initButtonUL(&tft,0,20+(30*i),60,30,2,TFT_WHITE,TFT_BLACK,btPar1text[i],2);
    btPar1[i].drawButton();
    tft.setTextSize(2);  tft.setTextColor(TFT_WHITE, TFT_BLACK);  
    tft.drawNumber(conf.T[i], 70,30+(30*i));
    }
  for (byte i=5;i<10;i++)
    {
    btPar1[i].initButtonUL(&tft,160,20+(30*(i-5)),60,30,2,TFT_WHITE,TFT_BLACK,btPar1text[i],2);
    btPar1[i].drawButton();
    tft.setTextSize(2);  tft.setTextColor(TFT_WHITE, TFT_BLACK);  
    tft.drawNumber(conf.T[i], 230,30+(30*(i-5)));
    }
  }

void drawBCSet2()   // parámetros nivel 2
{
  if (conf.TFTenabled==0) return;
  clearTFT();
  tft.setTextSize(2); tft.setTextColor(TFT_WHITE, TFT_BLACK);  
  tft.drawString("Parametros nivel 2",0,0);

  for (byte i=0;i<5;i++)
    {
    btPar2[i].initButtonUL(&tft,0,20+(30*i),60,30,2,TFT_WHITE,TFT_BLACK,btPar2text[i],2);
    btPar2[i].drawButton();
    tft.setTextSize(2);  tft.setTextColor(TFT_WHITE, TFT_BLACK); 
    if (i==0) tft.drawNumber(conf.HYC, 70,30+(30*i));
    else if (i==1) tft.drawNumber(conf.HYE, 70,30+(30*i));
    else if (i==2) tft.drawNumber(conf.HYF, 70,30+(30*i));
    else if (i==3) tft.drawNumber(conf.HYr, 70,30+(30*i));
    else if (i==4) tft.drawNumber(conf.Cr1, 70,30+(30*i));
    }
  for (byte i=5;i<10;i++)
    {
    btPar2[i].initButtonUL(&tft,160,20+(30*(i-5)),60,30,2,TFT_WHITE,TFT_BLACK,btPar2text[i],2);
    btPar2[i].drawButton();
    tft.setTextSize(2);  tft.setTextColor(TFT_WHITE, TFT_BLACK);  
    if (i==5) tft.drawNumber(conf.Cr2, 230,30+(30*(i-5)));
    else if (i==6) tft.drawNumber(conf.AH2, 230,30+(30*(i-5)));
    else if (i==7) tft.drawNumber(conf.AH4, 230,30+(30*(i-5)));
    else if (i==8) tft.drawNumber(conf.Ab4, 230,30+(30*(i-5)));
    else if (i==9) tft.drawNumber(conf.RES, 230,30+(30*(i-5)));
    }
  }

void drawGPIOs()   // GPIOS variables
{
  if (conf.TFTenabled==0) return;
  byte j=0;
  for (byte i=0;i<maxgpiovar;i++) 
    if (gpiovis(i))
      {
      int posy=34*j+5;
      strcpy(auxdesc,readdescr(filedescgpio,i,20)); 
      if (conf.gpiosensortype[i]==0)     // INPUT
        {
        btGPIO[i].initButtonUL(&tft,0,34*j,180,33,2,getbit8(conf.MbC8gpio,i+16)==1?TFT_YELLOW:TFT_WHITE,TFT_BLACK,auxdesc,2);
        btGPIO[i].drawButton();
        tft.setTextColor(TFT_WHITE, TFT_BLACK);  tft.setTextSize(2);
        tft.drawString("      ", 190,34*j+10);
        tft.drawNumber(conf.contadoresgpio[i],190, 34*j+10); 
        }
      else if (conf.gpiosensortype[i]==1)     // OUTPUT
        {
        btGPIO[i].initButtonUL(&tft,0,34*j,180,33,2,getbit8(conf.MbC8gpio,i)==0?TFT_WHITE:TFT_YELLOW,TFT_BLACK,auxdesc,2);
        btGPIO[i].drawButton();
        }
      else if (conf.gpiosensortype[i]==2)     // ADC estándar
        {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.drawString(auxdesc,0,posy);
        tft.drawFloat(MbRgpio[i],1,270,posy); 
        }
      else if (conf.gpiosensortype[i]==4)    // DHT
        {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.drawString(auxdesc,0,posy);
        tft.drawFloat(dhtdata[i][0],1,170,posy);  tft.drawString("C/",220,posy);
        tft.drawFloat(dhtdata[i][1],1,240,posy);  tft.drawString("%",300,posy);
        }
      else if ((conf.gpiosensortype[i]==5) || (conf.gpiosensortype[i]==6))
        {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.drawString(auxdesc,0,posy);
        tft.drawFloat(MbRgpio[i]+conf.gpiogamma[i],1,240,posy); tft.drawString("C",300,posy);
        }
      else if (conf.gpiosensortype[i]==7)     // ACS712  sensor de corriente
        {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.drawString(auxdesc,0,posy);
        tft.drawFloat(MbRgpio[i]+conf.gpiogamma[i],1,270,posy); 
        }
      else
        {
        btGPIO[i].initButtonUL(&tft,0,34*j,180,33,2,TFT_WHITE,TFT_BLACK,auxdesc,2);
        btGPIO[i].drawButton();
        }
      j++;
      }
}

void drawIP()   // valores LAN
{
  if (conf.TFTenabled==0) return;
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString(WiFi.isConnected()?"Connected":"NOT Connected",0,0);
 // tft.drawString(WiFi.SSID(),150,0);
  tft.drawString(conf.ssidSTA,150,0);
  for (byte i=0;i<4;i++)
    {
    tft.drawString("IP",0,30); tft.drawNumber(conf.EEip[i], 50*i+70, 30); if (i<3) tft.drawString(".",50*i+105,30);
    tft.drawString("Mask",0,60); tft.drawNumber(conf.EEmask[i], 50*i+70, 60); if (i<3) tft.drawString(".",50*i+105,60);
    tft.drawString("GW",0,90); tft.drawNumber(conf.EEgw[i], 50*i+70, 90); if (i<3) tft.drawString(".",50*i+105,90);
    tft.drawString("DNS",0,120); tft.drawNumber(conf.EEdns[i], 50*i+70, 120); if (i<3) tft.drawString(".",50*i+105,120);
    }
  btIP[0].initButtonUL(&tft,0,150,75,40,2,TFT_WHITE,TFT_BLACK,"SCAN",2);  btIP[0].drawButton();
  btIP[1].initButtonUL(&tft,100,150,75,40,2,TFT_WHITE,TFT_BLACK,"PASS",2);  btIP[1].drawButton();
}

void drawSET()     // barra setup
{
  if (conf.TFTenabled==0) return;
  btSET[0].initButtonUL(&tft,0,0,100,40,2,TFT_WHITE,TFT_BLACK,"RESET",2);  btSET[0].drawButton();
  btSET[1].initButtonUL(&tft,0,50,100,40,2,TFT_WHITE,TFT_BLACK,"XXXX",2);  btSET[1].drawButton();
  btSET[2].initButtonUL(&tft,0,100,100,40,2,TFT_WHITE,TFT_BLACK,"XXXXX",2);  btSET[2].drawButton();
  btSET[3].initButtonUL(&tft,0,150,100,40,2,TFT_WHITE,TFT_BLACK,"XXXXX",2);  btSET[3].drawButton();
}

void drawAP(byte nact)
{
    clearTFT();
    for (int i=0; i<nAP; i++)
      //if (WiFi.RSSI(i)>-90)
        {
        WiFi.SSID(i).toCharArray(auxchar, 20);
        if (i==tftapactual) { tft.setTextColor(TFT_BLACK, TFT_WHITE);    }
        tft.drawNumber(i,0,i*20);
        tft.drawString(auxchar,20,i*20); 
        tft.drawNumber(WiFi.RSSI(i),240,i*20);
        tft.drawString("dbm",280,i*20); 
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }
    drawST(true,true);
}

void setidTFT()
{
  tftpage=21;
  strcpy(auxtft,"");
  clearTFT();  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  readtextTFT();    // devuelve texto en auxdesc[60]
}

void ICACHE_FLASH_ATTR selectapTFT()
{
  tftpage=7; 
  nAPact=0;
  tftapactual=0;
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  clearTFT();
  tft.drawString("Scanning...",0,0); 
  nAP=WiFi.scanNetworks(false, false);  if (nAP>maxAP) nAP=maxAP;
  drawAP(tftapactual);
}


void drawTFT()   // Setting 
{
  if (conf.TFTenabled==0) return;
  if (conf.modofi==0)   // modo normal
    {
    if (tftpage==0) { drawSD();  drawST(true,false); }
    else if (tftpage==1) { drawED(); drawST(true,false); }
    else if (tftpage==2) { drawTE(); drawST(true,false); }
    else if (tftpage==3) { drawGPIOs(); drawST(true,false); }
    else if (tftpage==4) { drawIP(); drawST(true,false); }
    else if (tftpage==5) { drawSET(); drawST(true,false); }
    else if (tftpage==6) { drawSO(); drawST(true,false); }
    }
  else if (conf.modofi==1)  // bomba calor
    {
    if (tftpage==0) { drawBC(); drawST(true,false);}
    else if (tftpage==1) { drawBCSet1(); drawST(true,false);}
    else if (tftpage==2) { drawBCSet2(); drawST(true,false);}
    else if (tftpage==3) { drawGPIOs(); drawST(true,false);}
    else if (tftpage==4) { drawIP(); drawST(true,false);}
    else if (tftpage==5) { drawSET(); drawST(true,false);}
    else if (tftpage==6) { drawSO(); drawST(true,false);}
    else if (tftpage==22) { drawAlarma(); }
    }
}

