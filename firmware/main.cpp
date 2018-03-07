

#include "usring.h"



Usring ring;

int mittelwert[16];
int werte[16];
uint16_t ausgabe;

void kalibrieren()
{
  //mittelwerte auf 0 setzen
  for (int x = 0; x < 16; x++)
    mittelwert[x] = 0;

  //10 Messwerte Aufnehmen und den mittelwert Berechnen
  for (int x = 0; x < 10; x++)
  {
    ring.getDifferenceValue(werte);
    for (int i = 0; i < 16; i++)
        mittelwert[i] += werte[i];
    delay(10);
  }
  for (int x = 0; x < 16; x++)
    mittelwert[x] /= 10;
}

void messen()
{
  ring.getDifferenceValue(werte);
}

void berechnen()
{
  //Schwellwerte auswerten und Ergebnisse in 16 bit Typ Speichern
  ausgabe = 0;
  for (int x = 0; x < 16; x++)
  {
    if (werte[x] > mittelwert[x])
    {
      ausgabe |= (1<<x);
    }
  }
}

void ausgeben()
{
  //daten in i2c Puffer Schreiben
  ring.setI2CData(0,2,(uint8_t*)&ausgabe);
  for (int x = 0; x < 16; x++)
    ring.setI2C(x+2, werte[x]);
}



int main (void) {
  kalibrieren();
  while (1) {
    messen();
    berechnen();
    ausgeben();
  }                
}
