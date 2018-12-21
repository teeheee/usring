#include "usring.h"

Usring ring;

int mittelwert[16];
int werte[16];
uint8_t werte_nach_filter[16];
uint16_t ausgabe;
uint8_t global_filter;

void kalibrieren()
{
  //mittelwerte auf 0 setzen
  for (int x = 0; x < 16; x++)
  {
    mittelwert[x] = 0; 
  }

  //10 Messwerte Aufnehmen und den mittelwert Berechnen
  for (int x = 0; x < 32; x++)
  {
    ring.getDifferenceValue(werte);
    for (int i = 0; i < 16; i++)
        mittelwert[i] += werte[i];
  }

  for (int x = 0; x < 16; x++)
  {
    mittelwert[x] /= 32;
    mittelwert[x] += 20;
  }
}

void messen()
{
  ring.getDifferenceValue(werte);
}

void berechnen()
{
  //Schwellwerte auswerten und Ergebnisse in 16 bit Typ Speichern
  ausgabe = 0;
  for (uint16_t x = 0; x < 16; x++)
  {
    if (werte[x] > mittelwert[x])
	werte_nach_filter[x]++;
    else
	werte_nach_filter[x] = 0;
    if (werte_nach_filter[x] > global_filter){
        ausgabe |= (1<<x);
	werte_nach_filter[x] = global_filter;
    }
  }
}

void ausgeben()
{
  //daten in i2c Puffer Schreiben
  ring.setI2CData(0,2,(uint8_t*)&ausgabe);
  for (int x = 0; x < 16; x++)
    ring.setI2C(x+2, werte[x]);
  for (int x = 0; x < 16; x++)
    ring.setI2C(x+18, mittelwert[x]);
}

void lesen()
{
	uint8_t global_th_value = ring.getI2C(1);
	if(global_th_value > 0)
	   for (int x = 0; x < 16; x++)
	      mittelwert[x] = global_th_value;
	uint8_t global_filter_value = ring.getI2C(2);
	if(global_filter_value > 0)
		global_filter = global_filter_value;
  	for (int x = 0; x < 16; x++)
	{
		uint8_t local_th = ring.getI2C(x+3);
		if(local_th > 0)
			mittelwert[x] = local_th;
	}
}

int main (void) {
  kalibrieren();
  while (1) {
    messen();
    berechnen();
    ausgeben();
    lesen();
  }                
}
