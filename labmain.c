/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );

#include <stdlib.h>
#include <stdio.h>

int timeoutcount = 0;

#define TIMER_BASE 0x04000020
#define TIMER_STATUS  ((volatile unsigned int*)(TIMER_BASE + 0x0))
#define TIMER_CONTROL ((volatile unsigned int*)(TIMER_BASE + 0x4))
#define TIMER_PERIODL ((volatile unsigned int*)(TIMER_BASE + 0x8))
#define TIMER_PERIODH ((volatile unsigned int*)(TIMER_BASE + 0xC))

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{}

/* Add your code here for initializing interrupts. */
void labinit(void){
/*Ställ in period = 2 999 999 (0x2DC6BF)*/
    *TIMER_PERIODL = 2999999 & 0xFFFF;
    *TIMER_PERIODH = (2999999 >> 16) & 0xFFFF;
    /* Nollställ status (rensa ev. TO-flagga)*/
    *TIMER_STATUS = 1;
    /* Starta: kontinuerlig mode (CONT=1), starta (START=1)*/
    /* ITO = 0 (vi använder inte IRQ utan polling)*/
    *TIMER_CONTROL = (1 << 1) | (1 << 2);  // CONT=1, START=1
}

void set_leds (int led_mask){
  volatile int * p = (volatile int *) 0x04000000;
  led_mask &= 0x3FF;
  *p = led_mask;
}

void set_display( int display_number, int value){
  volatile int * p = (volatile int *) 0x04000050 + (display_number * 0x10)/sizeof(int);
  switch (value)
  {
  case 0:
    *p = 0b11000000;
    break;
  case 1:
    *p = 0b11111001;
    break;
  case 2:
    *p = 0b10100100;
    break;
  case 3:
    *p = 0b10110000;
    break;
  case 4:
    *p = 0b10011001;
    break;
  case 5:
    *p = 0b10010010;
    break;
  case 6:
    *p = 0b10000010;
    break;
  case 7:
    *p = 0b11111000;
    break;
  case 8:
    *p = 0b10000000;
    break;
  case 9:
    *p = 0b10010000;
    break;
  
  default:
    *p = 0b11011010;
    break;
  }

}

int get_sw(void){
  volatile int* p = (volatile int*) 0x04000010;
  int a = *p;
  a &= 0x3FF;
  return a;
}

int get_btn(void){
  volatile int* p = (volatile int*) 0x040000d0;
  int a = *p;
  a &= 0x1;
  return a;

}

unsigned int hexDecimalToHex(unsigned int value) {
    unsigned int result = 0;
    unsigned int factor = 1;
    while (value > 0) {
        unsigned int digit = value % 10;   // plocka ut sista decimalsiffran
        result += digit * factor;          // lägg till i resultatet
        factor *= 16;                      // förskjut en hex-position
        value /= 10;                       // gå vidare till nästa decimalsiffra
    }
    return result;
}

/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  labinit();

  int led_mask = 0;
  // Enter a forever loop
  while (1) {
    if (*TIMER_STATUS & 0x1){
      *TIMER_STATUS = 1;
      if (timeoutcount%10 == 0){
      time2string( textstring, mytime ); // Converts mytime to string
      display_string( textstring ); //Print out the string 'textstring'
      tick( &mytime );     // Ticks the clock once
      set_leds(led_mask);

      led_mask++;
      int tid = mytime;
      for (int i = 0; i<6; i++){
        int siffran = tid & 0xF;
        set_display (i, siffran);
        tid /= 16;
      
      }
      timeoutcount = 0;
      }
    timeoutcount ++;
    }
    
    if (get_btn()){
      int spakar = get_sw();
      int avbryt = spakar & 0x80;
      if (avbryt == 0x80){
        break;
      }
      int inställning = spakar & 0x300;
      switch (inställning){
        
      case 0x100:
        mytime &= 0xFFFF00;
        spakar &= 0x3F;
        spakar = hexDecimalToHex(spakar);
        if (spakar >0x59){
          spakar -= 0x60;
        }
        mytime += spakar;
        break;
      case 0x200:
        mytime &= 0xFF00FF;
        spakar &= 0x3F;
        spakar = hexDecimalToHex(spakar);
        if (spakar >0x59){
          spakar -= 0x60;
        }
        spakar *= 0x100;
        mytime += spakar;
        break;
      case 0x300:
        mytime &= 0x00FFFF;
        spakar &= 0x3F;
        spakar = hexDecimalToHex(spakar);
        if (spakar >0x59){
          spakar -= 0x60;
        }
        spakar *= 0x10000;
        mytime += spakar;
        break;
      
      default:
        break;
      }
    }
  }

}


