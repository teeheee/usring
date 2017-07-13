#define F_CPU   14745000UL

#include <avr/io.h>          // (1)
#include <util/delay.h>
#include <stdlib.h>
#include "sensor.h"
#include "twislave.h"
#include "uart.h"


#define ledtime 10
#define messurement_time 100




int main (void) {

  init_sensors();
  uart_init(UART_BAUD_SELECT(9600, F_CPU));
  init_twi_slave(0x69);
  int x = 0;
  int i = 0;
  int werte[16];


  int mittelwert[16];

  rxbuffer[0]=1;

  while (1) {
    if(rxbuffer[0]>0)
    {
        for (x = 0; x < 16; x++)
          mittelwert[x] = 0;

        for (x = 0; x < 10; x++)
        {
          getdiffValue(werte);
          for (i = 0; i < 16; i++)
            mittelwert[i] += werte[i];
          _delay_ms(10);
        }
        for (x = 0; x < 16; x++)
          mittelwert[x] /= 10;

        _delay_ms(1000);
        for ( x = 0; x < 16; x++)
          txbuffer[18+x]=rxbuffer[2+x];
        rxbuffer[0]=0;
    }
    if(rxbuffer[1]>0)
    {
      for ( x = 0; x < 16; x++)
        txbuffer[18+x]=rxbuffer[2+x]=rxbuffer[1];
      rxbuffer[1]=0;
    }

    uint16_t ret = 0;
    getdiffValue(werte);
    for ( x = 0; x < 16; x++)
    {
      uint16_t d = abs(werte[x] - mittelwert[x]);
      if (d > rxbuffer[2+x])
      {
        ret |= (1<<x);
      }
      if(d>0xff)
        txbuffer[x+2]=0xff;
      else
        txbuffer[x+2]=d;
    }
    txbuffer[0]=ret&0xff;
    txbuffer[1]=ret>>8;
  }
  return 0;                 
}
