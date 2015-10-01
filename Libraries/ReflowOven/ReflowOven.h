/*******************************************************************************
 * Created by Skyler Brandt on August 2015
 * AquaPic
 *
 *******************************************************************************
 * Copyright (C) 2015 Skyler Brandt
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 ******************************************************************************/
 
#ifndef GLOBAL_SCREEN_H
#define GLOBAL_SCREEN_H
 
/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include "ovenGraphScreen.h"
#include <drv_pid.h>
#include <MAX31855.h>

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/
/*******Errors List************************************************************/

/*******User Defined***********************************************************/

/*******EEPROM STUFF***********************************************************/

/******************************************************************************/
/* Variable Definition                                                        */
/******************************************************************************/
enum REFLOW_SCREENS_T {
    OVEN_GRAPH = 0,
};

enum reflowState_t {
    PREHEAT = 0,
    SOAK,
    RAMP,
    PEAK,
    COOLDOWN
};

typedef struct reflow_var {
    float preheatRamp;
    float soakTemp;
    uint16_t soakTime;
    float ramp;
    float peakTemp;
    uint16_t peakTime;
    
    float startTemp;
    uint16_t preheatTime;
    uint16_t rampTime;
    uint16_t cooldownTime;
    uint16_t totalTime;
    float soakTempIncrease;
    float cooldownRamp;
}ReflowVar;

/******************************************************************************/
/* Class Definition                                                           */
/******************************************************************************/
class ReflowOven {
public:
    ReflowOven (void) : tc (23, 25, 22) {};
    void init (void);
    void loop (void);
    uint32_t timerService (uint32_t currentTime);
    void turnOvenOn (void);
    void turnOvenOff (void);
    void turnElementOn (void);
    void turnElementOff (void);
    void soundOn (void);
    void soundOff (void);
    void calculateReflowVars (void);
    
    ReflowVar reflowVars;
    PIDvar pidVars;
    
    double temp;
    uint16_t runtime;
    bool ovenOn;

private:
    ovenGraphScreen ovenGraph;
    
    MAX31855 tc;
    
    PIDobj pid;

    bool elementOn;
    reflowState_t reflowState;
    float setpoint;
    int faultCount;
    
    unsigned long ms1000;
    unsigned long msPTerm;
    unsigned long msITerm;
    unsigned long msDTerm;
    unsigned long elementOffTime;
};

extern ReflowOven oven;

#endif /* GLOBAL_SCREEN_H */