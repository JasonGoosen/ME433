#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#define SPI__H__
#include<stdio.h>
#include "ws2812b.h"

#define IODIRA 0x00
#define IODIRB 0x01
#define GPIOB 0x13
#define OLATA 0x14
#define WRITE 0b01000000
#define READ 0b01000001
// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 00011000// some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations
//NEW
void setPin( unsigned char reg, unsigned char val);
unsigned char readPin(unsigned char reg);



int main() {
    
    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISAbits.TRISA4 = 0 ;
    LATAbits.LATA4 =0;
    __builtin_enable_interrupts();
    //Intialize(write and read)
    //Send Start Bit
    
    
   
    __builtin_enable_interrupts();
    ws2812b_setup();
    //Start Colors
    wsColor clrs[4];
    int Heff1 =0;
    int Heff2 =60;
    int Heff3 =120;
    int Heff4 =180;
    int Up =1;
    
    while(1){
        if(Heff1>=360){
            Heff1=0;
        }
        else if(Heff2>=360){
            Heff2=0;
        }
        else if(Heff3>=360){
            Heff3=0;
        }
        else if(Heff4>=360){
            Heff4=0;
        }
        clrs[0]=HSBtoRGB(Heff1,1,.5);
        clrs[1]=HSBtoRGB(Heff2,1,.5);
        clrs[2]=HSBtoRGB(Heff3,1,.5);
        clrs[3]=HSBtoRGB(Heff4,1,.5);
        
        Heff1=Heff1+Up;
        Heff2=Heff2+Up;
        Heff3=Heff3+Up;
        Heff4=Heff4+Up;
         //Heartbeat
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<48000000/240){
            ;
        }   
            
        ws2812b_setColor(clrs,4);
    }
}
