#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<stdio.h> 
#define SPI__H__

#include "font.h"
#include "ssd1306.h"
#include "i2c_master_noint.h"

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
    //Vars
    char m[50];
    int s=0;
    int i=0;
    int j=0;
    int k=0;
    int x=0;
    int y=0;
    
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
    i2c_master_setup();
    //Send Start Bit
    
    
    
    ssd1306_setup();
    __builtin_enable_interrupts();
    sprintf(m,"i=%d",i);
    while(m[s]!=0){
        char letter = m[s]-0x20;
        
        LATAbits.LATA4 = !LATAbits.LATA4;
        for(j=0;j<4;j++){
            for(k=0;k<7;k++){
                if(((ASCII[letter][j])>>k)&1){
                    ssd1306_drawPixel((x+j+(s*5)),(y+k),1);
                }
                else{
                    ssd1306_drawPixel(j,k,0);
                }
                    
            }
         
    }
        
        s++;
    }
    
    //Heartbeat
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<48000000/24){
            ;
        }
}

void setPin( unsigned char reg, unsigned char val){
    i2c_master_start();
    i2c_master_send(WRITE);
    i2c_master_send(reg);
    i2c_master_send(val);
    i2c_master_stop();
}
unsigned char readPin( unsigned char reg){
    i2c_master_start();
    i2c_master_send(WRITE);
    i2c_master_send(reg);
    i2c_master_restart();
    i2c_master_send(READ);
    unsigned char data=i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    return data;
}
