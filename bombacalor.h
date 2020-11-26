#define pinBCal 02
#define pinBCap 03
#define pinBACS 04
#define pinComp 05
#define pinV4V 06
#define pinAlarm 07

boolean V4VON() { return digitalRead(sdPin[pinV4V])==1; }
boolean BCalON() { return digitalRead(sdPin[pinBCal])==1; }
boolean BCapON() { return digitalRead(sdPin[pinBCap])==1; }
boolean BACSON() { return digitalRead(sdPin[pinBACS])==1; }
boolean CompON() { return digitalRead(sdPin[pinComp])==1; }
boolean AlarmON() { return digitalRead(sdPin[pinAlarm])==1; }

void setV4VON() { if (V4VON()) return; pinVAL(pinV4V, 1, conf.iddevice);}
void setV4VOFF() { if (!V4VON()) return; pinVAL(pinV4V, 0, conf.iddevice);  }
void setBCalON() { if (BCalON()) return; pinVAL(pinBCal, 1, conf.iddevice);}
void setBCalOFF() { if (!BCalON()) return; pinVAL(pinBCal, 0, conf.iddevice);  }
void setBCapON() { if (BCapON()) return; pinVAL(pinBCap, 1, conf.iddevice);  }
void setBCapOFF() { if (!BCapON()) return; pinVAL(pinBCap, 0, conf.iddevice);  }
void setBACSON() { if (BACSON()) return; pinVAL(pinBACS, 1, conf.iddevice);  }
void setBACSOFF() { if (!BACSON()) return; pinVAL(pinBACS, 0, conf.iddevice);  }
void setComprON() { if (CompON()) return; pinVAL(pinComp, 1, conf.iddevice);  }
void setComprOFF() { if (!CompON()) return; pinVAL(pinComp, 0, conf.iddevice);  }
void setAlarmON() { if (AlarmON()) return; pinVAL(pinAlarm, 1, conf.iddevice); tftpage=22; }
void setAlarmOFF() { if (!AlarmON()) return; pinVAL(pinAlarm, 0, conf.iddevice); drawTFT(); }

int consignaCal()
{
  return conf.T[3];   // pendiente de mejorar  
}

void ArrancarCal()
{
  CALisON=true;
  REFisON=false;
  ACSisON=false;
  setV4VOFF();  // desactivar valvula de 4 vias
  setBCalON();  // encender bomba calefacción
  if ((millis()/1000-tempact[2])<=conf.CCC*60) return;  // esperar CCC minutos
  setBCapON();  // encender Bomba captador 
  if ((millis()/1000-tempact[3])<conf.CC*60) return;  // esperar CC minutos
  setComprON();  // encender compresor
}

void pararCal()
{ setComprOFF(); setBCapOFF(); setBCalOFF(); CALisON=false; }

void ArrancarRef()
{
  REFisON=true;
  CALisON=false;
  ACSisON=false;
  setV4VON();  // activar valvula de 4 vias
  setBCalON();  // encender bomba calefacción
  if ((millis()/1000-tempact[2])<=conf.CCC*60) return;  // esperar CCC minutos
  setBCapON();  // encender Bomba captador 
  if ((millis()/1000-tempact[3])<conf.CC*60) return;  // esperar CC minutos
  setComprON();  // encender compresor
}

void pararRef()
{ setComprOFF(); setBCapOFF(); setBCalOFF(); setV4VOFF(); REFisON=false;}

void ArrancarACS()
{
  ACSisON=true;
  CALisON=false;
  REFisON=false;
  setV4VOFF();  // desactivar valvula de 4 vias
  setBCalON();  // encender bomba calefacción
  setBACSON();  // encender bomba ACS y Válvula 3 vías
  if ((millis()/1000-tempact[2])<=conf.CCC*60) return;  // esperar CCC minutos
  setBCapON();  // encender Bomba captador 
  if ((millis()/1000-tempact[3])<conf.CC*60) return;  // esperar CC minutos
  setComprON();  // encender compresor
}

void pararACS()
{ 
  setComprOFF(); setBACSOFF(); setBCapOFF(); setBCalOFF(); ACSisON=false; }

void pararTODO()
{
  pararACS();
  pararCal();
  pararRef();
}

void setALARMA(byte tipoalarma)
{
  pinVAL(pinAlarm, 1, conf.iddevice); 
  tftpage=22;
}

void checkAlarma()
{
  if (idbc[idBP]==1)
    { 
    if (estalarma[4]==0) estalarma[4]=1; 
    }
  else                          
    { estalarma[4]=0; }
  if (idbc[idHP]==1)
    { 
    if (estalarma[5]==0) estalarma[5]=1; 
    }
  else
    { estalarma[5]=0;  }
  byte i=0;
  while (i<9)
    { 
    if (estalarma[i]==2)
      {
      Serial.println("ASIGNADA 2");
      break;
      }
    i++; 
    }
  tipoalarma2=i;
  i=0;
  while (i<9)
    { 
    if (estalarma[i]==1)
      {
      Serial.println("ASIGNADA 1");
      break;
      }
    i++; 
    }
  tipoalarma1=i;
}

void procesaBC() 
{
  checkAlarma();
  if (tipoalarma2<9) 
    { 
    if (estalarma[tipoalarma2]==1)
      {
      Serial.println("setAlarmON");
      setAlarmON(); 
      drawTFT();
      return; 
      }
    } 
  else 
    {
    if (estalarma[tipoalarma2]==0) 
      setAlarmOFF(); 
    }
  
  if (CALisON) consignaAct=consignaCal();
  if (REFisON) consignaAct=conf.FRO;
  if (ACSisON) consignaAct=conf.ECS;
  if (ACSisON) if (conf.ACS==0) pararACS();   // se apaga ACS
  if (CALisON) if (conf.Cal==0) pararCal();   // se apaga Calefacción
  if (REFisON) if (conf.Ref==0) pararRef();   // se apaga Refrigeración

  /////////////////////////////
  if (conf.ACS==1)    // ACS encendida
    {
    if (idbc[idACS]==1)    // ACS solicitada por reloj
      {
      if (demACS)     //  por debajo de consigna
        {
        ArrancarACS();
        }
      else
        {
        setBACSOFF();
        ACSisON=false;
        if (conf.Cal==1)    // Calefacción encendida
          {
          if (idbc[idTerm]==1)   // Termostato activado
            {
            if (demCAL)    // demanda Cal
              ArrancarCal();
            else
              pararCal();
            }
          }
        else if (conf.Ref==1)      // refrigeración encendida
          {
          if (idbc[idTerm]==1)   // Termostato activado
            {
            if (demREF)    // demanda Ref
              ArrancarRef();
            else
              pararRef();
            }
          }
        else
          {
          pararACS();
          }
        }
      }
    else    // ACS desactivada reloj
      {
      if (conf.Cal==1)    // Calefacción encendida
        {
        setBACSOFF();
        ACSisON=false;
        if (idbc[idTerm]==1)   // Termostato activado
          {
          if (demCAL)    // demanda Cal
            ArrancarCal();
          else
            pararCal();
          }
        else
          {
          pararCal();
          }
        }
      else if (conf.Ref==1)
        {
        setBACSOFF();
        ACSisON=false;
        if (idbc[idTerm]==1)   // Termostato activado
          {
          if (demREF)    // demanda Ref
            ArrancarRef();
          else
            pararRef();
          }
        else
          {
          pararRef();
          }
        }
      else
        {
        pararACS();  
        }
      }
    }
  else     // ACS apagada
    {
    if (conf.Cal==1)    // Calefacción encendida
      {
      if (idbc[idTerm]==1)   // Termostato activado
        {
        if (demCAL)    // demanda Cal
          ArrancarCal();
        else
          pararCal();
        }
      else
        pararCal();
      }
    else if (conf.Ref==1)   // Refrigeración encendida
      {
      if (idbc[idTerm]==1)   // Termostato activado
        {
        if (demREF)    // demanda Ref
          ArrancarRef();
        else
          pararRef();
        }
      else
        {
        pararRef();
        }
      }
    else
      {
      pararRef();
      }
    }
}
