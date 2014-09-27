#include <16F877.h>
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES HS                       //High speed Osc (> 4mhz) 
#FUSES NOPUT                    //No Power Up Timer 
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOPROTECT
#FUSES NOBROWNOUT
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOCPD                    //No EE protection 
#FUSES NOWRT                    //Program memory not write protected 

#use delay(crystal=20000000)
#use FIXED_IO( A_outputs=PIN_A5,PIN_A4,PIN_A3,PIN_A2,PIN_A1,PIN_A0 )
#use FIXED_IO( B_outputs=PIN_B7,PIN_B6,PIN_B5,PIN_B4,PIN_B3,PIN_B2,PIN_B1,PIN_B0 )
#use FIXED_IO( D_outputs=PIN_D7,PIN_D6,PIN_D5,PIN_D4,PIN_D3,PIN_D2,PIN_D1,PIN_D0 )
#use FIXED_IO( E_outputs=PIN_E2,PIN_E1,PIN_E0 )
#use i2c(Slave,Slow,sda=PIN_C4,scl=PIN_C3,force_hw,address=0x70)

int8 state = 0;           // Last i2c_isr_state() value
int addr_pointer = 0;     // addr_pointer into data[]
int incoming;             // last byte read over i2c from master

int pin_durum(int pin){
   int sonuc;
   switch(pin){
      case 1:
         sonuc = input_state(PIN_A0);
         break;
      case 2:
         sonuc = input_state(PIN_A1);
         break;
      case 3:
         sonuc = input_state(PIN_A2);
         break;
      case 4:
         sonuc = input_state(PIN_A3);
         break;
      case 5:
         sonuc = input_state(PIN_A4);
         break;
      case 6:
         sonuc = input_state(PIN_A5);
         break;
      case 7:
         sonuc = input_state(PIN_E0);
         break;
      case 8:
         sonuc = input_state(PIN_E1);
         break;
      case 9:
         sonuc = input_state(PIN_E2);
         break;
      case 10:
         sonuc = input_state(PIN_C0);
         break;
      case 11:
         sonuc = input_state(PIN_C1);
         break;
      case 12:
         sonuc = input_state(PIN_C2);
         break;
      case 13:
         sonuc = input_state(PIN_D0);
         break;
      case 14:
         sonuc = input_state(PIN_D1);
         break;
      case 15:
         sonuc = input_state(PIN_D2);
         break;
      case 16:
         sonuc = input_state(PIN_D3);
         break;
      case 17:
         sonuc = input_state(PIN_C5);
         break;
      case 18:
         sonuc = input_state(PIN_C6);
         break;
      case 19:
         sonuc = input_state(PIN_C7);
         break;
      case 20:
         sonuc = input_state(PIN_D4);
         break;
      case 21:
         sonuc = input_state(PIN_D5);
         break;
      case 22:
         sonuc = input_state(PIN_D6);
         break;
      case 23:
         sonuc = input_state(PIN_D7);
         break;
      case 24:
         sonuc = input_state(PIN_B0);
         break;
      case 25:
         sonuc = input_state(PIN_B1);
         break;
      case 26:
         sonuc = input_state(PIN_B2);
         break;
      case 27:
         sonuc = input_state(PIN_B3);
         break;
      case 28:
         sonuc = input_state(PIN_B4);
         break;
      case 29:
         sonuc = input_state(PIN_B5);
         break;
      case 30:
         sonuc = input_state(PIN_B6);
         break;
   }
   return sonuc;
}

void ac_kapa(int pin, int durum){
   if(durum == 0){
      output_low(pin);
   }
   if(durum == 1){
      output_high(pin);
   }
}

int GetRealPinNo(int pin){
   int dondur = 0;
   switch(pin){
         case 1:
            dondur = PIN_A0;
            break;
         case 2:
            dondur = PIN_A1;
            break;
         case 3:
            dondur = PIN_A2;
            break;
         case 4:
            dondur = PIN_A3;
            break;
         case 5:
            dondur = PIN_A4;
            break;
         case 6:
            dondur = PIN_A5;
            break;
         case 7:
            dondur = PIN_E0;
            break;
         case 8:
            dondur = PIN_E1;
            break;
         case 9:
            dondur = PIN_E2;
            break;
         case 10:
            dondur = PIN_C0;
            break;
         case 11:
            dondur = PIN_C1;
            break;
         case 12:
            dondur = PIN_C2;
            break;
         case 13:
            dondur = PIN_D0;
            break;
         case 14:
            dondur = PIN_D1;
            break;
         case 15:
            dondur = PIN_D2;
            break;
         case 16:
            dondur = PIN_D3;
            break;
         case 17:
            dondur = PIN_C5;
            break;
         case 18:
            dondur = PIN_C6;
            break;
         case 19:
            dondur = PIN_C7;
            break;
         case 20:
            dondur = PIN_D4;
            break;
         case 21:
            dondur = PIN_D5;
            break;
         case 22:
            dondur = PIN_D6;
            break;
         case 23:
            dondur = PIN_D7;
            break;
         case 24:
            dondur = PIN_B0;
            break;
         case 25:
            dondur = PIN_B1;
            break;
         case 26:
            dondur = PIN_B2;
            break;
         case 27:
            dondur = PIN_B3;
            break;
         case 28:
            dondur = PIN_B4;
            break;
         case 29:
            dondur = PIN_B5;
            break;
         case 30:
            dondur = PIN_B6;
            break;
      }
   return dondur;
}

void tumunuAcKapa(int durum){
   int i = 0;
   for(i=0; i<=28; i++){
      ac_kapa(GetRealPinNo(i), durum);
   }
}

#INT_SSP
void  SSP_isr(void) 
{
   state = i2c_isr_state();
   output_high(PIN_B6);
   
   if (state < 0x80)    // Master is sending data
   {
      if (state == 0)  {
         incoming = i2c_read();        // Eat the I2C address
      }
      if (state == 1) {
         addr_pointer = i2c_read();        // Second value is addr_pointer update
      }
      if (state == 2) {
         incoming = i2c_read();        // Third optional value is the value to write
         if(addr_pointer == 1000){
            tumunuAcKapa(incoming);
         }
         else{
            ac_kapa(GetRealPinNo(addr_pointer), incoming);
         }   
      }
   }
   if (state == 0x80)         // Master is requesting data 
   {
      if(addr_pointer == 1000){
         i2c_write(1);
      }
      else{
         if(pin_durum(addr_pointer) == 0){
            i2c_write(0);
         }
         else if(pin_durum(addr_pointer) == 1){
            i2c_write(1);
         }
      }
   }
   output_low(PIN_B6);
}

void main()
{
   
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
   
   output_high(PIN_B7);

   // Update our outputs while waiting for interrupts
   while(TRUE)
   {
      //output_toggle(PIN_A4);
      //delay_ms(500);
      //delay_us(5);
   }
   
}
