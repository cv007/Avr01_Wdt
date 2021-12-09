#pragma once

//--------------------------------------------
//  AVR0/1 wdt.h replacement
//  can put in same folder as wdt.h
//  then use #include <avr/wdt01.h>
//
//  if the WDTCFG fuse is not 0, then the
//  watchdog LOCK bit will be set so none of 
//  the below functions will have any effect, 
//  except for wdt_restart()
//--------------------------------------------

#include <avr/io.h>
#include <stdint.h>

#define SI static inline __attribute((always_inline))

//--------------------------------------------
//  wdt periods for either window or period
//--------------------------------------------
typedef enum {
    WDT_OFF, WDT_8MS, WDT_16MS, WDT_32MS,
    WDT_64MS, WDT_128MS, WDT_256MS, WDT_512MS, 
    WDT_1S, WDT_2S, WDT_4S, WDT_8S
} WDT_period_t;

//--------------------------------------------
//  set period only
//--------------------------------------------
SI void wdt_period( WDT_period_t e ) {
    while( WDT.STATUS & 1 ){}          //check SYNCBUSY
    _PROTECTED_WRITE( WDT.CTRLA, e );  //WINDOW=0|PERIOD
}

//--------------------------------------------
//  set window and period
//  window timer activated by first wdr
//  window duration then excludes wdr usage 
//  until it times out, followed by the period
//  timer which requires wdr before timeout
//--------------------------------------------
SI void wdt_window( WDT_period_t window, WDT_period_t period ) {
    while ( WDT.STATUS & 1 ){}         //check SYNCBUSY
    uint8_t v = (window<<4)|period;
    _PROTECTED_WRITE( WDT.CTRLA, v );  //WINDOW|PERIOD
}

//--------------------------------------------
//  lock wdt (can only set LOCK, so will be
//            locked until reset)
//--------------------------------------------
SI void wdt_lock() {
    _PROTECTED_WRITE( WDT.STATUS, 0x80 ); //set LOCK
}

//--------------------------------------------
//  restart (reset) wd counter
//--------------------------------------------
SI void wdt_restart(){ asm("wdr"); }

#undef SI
