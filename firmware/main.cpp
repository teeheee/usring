#include "usring.h"
#include "time.h"
#include <avr/wdt.h>
#include <avr/io.h>

Usring ring;

uint8_t schwellwerte[16];
uint8_t analogwerte[16];
uint16_t DigitaleAusgabe;
uint16_t DigitaleAusgabe_ErsterKontakt;

int hysteresis_counter[16];
int global_filter;

// Initialisiert alle Variablen
void init()
{
  for (int x = 0; x < 16; x++)
  {
    schwellwerte[x] = 10;
		analogwerte[x] = 0;
    hysteresis_counter[x] = 0;
  }
	global_filter = 0;
	DigitaleAusgabe = 0;
	wdt_enable(WDTO_15MS);
}

void messen()
{
  //OPTIMIERUNGS PARAMETER
  ring.getDifferenceValue(analogwerte); //misst die differenz aus LED an und aus (-> bessere Werte)
  //ring.getSingleValue(analogwerte); //misst mit den LEDS immer an (-> schneller)
}

//Schwellwerte auswerten und Ergebnisse in 16 bit Typ Speichern
void berechnen()
{
  if(global_filter)
  {
    for (uint16_t x = 0; x < 16; x++)
    {
      if (analogwerte[x] > schwellwerte[x] && hysteresis_counter[x] < global_filter)
      {
        hysteresis_counter[x] += 1;
      }
      else if(analogwerte[x] < schwellwerte[x] && hysteresis_counter[x] > -global_filter){
        hysteresis_counter[x] -= 1;
      }
      if(hysteresis_counter[x] == global_filter)
          DigitaleAusgabe |= (1<<x);
      else if(hysteresis_counter[x] == -global_filter)
          DigitaleAusgabe &= ~(1<<x);
    }
  }
  else
  {
	 uint16_t tmp = DigitaleAusgabe;
    DigitaleAusgabe = 0;
    for (uint16_t x = 0; x < 16; x++)
    {
      if (analogwerte[x] > schwellwerte[x])
          DigitaleAusgabe |= (1<<x);
    }
	 if(DigitaleAusgabe && !tmp){
	 	DigitaleAusgabe_ErsterKontakt = DigitaleAusgabe;
	 } else if(!DigitaleAusgabe && !tmp){
	 	DigitaleAusgabe_ErsterKontakt = 0;
	 }
  }
}

//Daten in i2c Puffer Schreiben
void ausgeben()
{
  ring.setI2CData(0,2,(uint8_t*)&DigitaleAusgabe);
  for (int x = 0; x < 16; x++)
    ring.setI2C(x+2, analogwerte[x]);
  for (int x = 0; x < 16; x++)
    ring.setI2C(x+18, schwellwerte[x]);
  ring.setI2CData(0,18+16,(uint8_t*)&DigitaleAusgabe_ErsterKontakt);
}

void lesen()
{
  // lese filter Threshhold
	uint8_t global_th_value = ring.getI2C(1);
	if(global_th_value > 0)
	   for (int x = 0; x < 16; x++)
	      schwellwerte[x] = global_th_value;

  // lese globalen Schwellwert
	uint8_t global_filter_value = ring.getI2C(2);
	if(global_filter_value > 0)
		global_filter = global_filter_value;

  // lese einzelne Schwellwerte
  for (int x = 0; x < 16; x++)
	{
		uint8_t local_th = ring.getI2C(x+3);
		if(local_th > 0)
			schwellwerte[x] = local_th;
	}
}



void generateDebugInfo(){
  static int max = 0;
  static int min = 100;
  static uint32_t start_time = 0;
  int wert = (millis()-start_time);
  if(wert > 0xff)
    wert = 0xff;
  if(wert > max)
    max = wert;
  if(wert < min)
    min = wert;
  ring.setI2C(40,min);
  ring.setI2C(41,max);
  if(MCUCSR & (1<<WDRF)){
      ring.setI2C(39,1);// a watchdog reset occurred
  }
  if(MCUCSR & (1<<BORF)){
      ring.setI2C(39,2);//  a brownout reset occurred
  }
  if(MCUCSR & (1<<EXTRF)){
      ring.setI2C(39,3);//  an external reset occurred
  }
  start_time = millis();
}

int main (void) {
  init();
  while (1) {
    messen();
    berechnen();
    ausgeben();
    lesen();
		if(PINC & (1<<PC4))
			wdt_reset();
    //generateDebugInfo();
  }
}
