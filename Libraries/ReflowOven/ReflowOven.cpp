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

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <WProgram.h>
#include <inttypes.h>
#include <BUI.h>
#include <ReflowOven.h>

/******************************************************************************/
/* Variable Definition                                                        */
/******************************************************************************/
#define MAX_TEMPERATURE     350
#define TEMP_ALARM          300
#define MAX_OVER_SETPOINT   30
#define SPEAKER_PIN         9
#define RELAY_PIN           28

/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/
void ReflowOven::init (void) {
/**PID*************************************************************************/
    //135 sec (Tu) oscillation at 100 gain (Ku)
    pidVars.gain = 60;          //Ku*0.60
    pidVars.sampleTime = 2500;  //milliseconds
    pidVars.iTime = 33750;      //milliseconds Tu*0.5   ::67500
    pidVars.dTime = 8438;       //milliseconds Tu*0.125 ::16875
    pidVars.minI = -0.25;
    pidVars.maxI = 0.75;
    pid = pid_new ();
    pid_init (pid, pidVars);
    
/**Initial Oven Variables******************************************************/
    reflowVars.preheatRamp = 0.8f;  // C/sec
    reflowVars.soakTemp = 150.0f;   // C
    reflowVars.soakTime = 60;       // sec
    reflowVars.ramp = 0.6;          // C/sec
    reflowVars.peakTemp = 225.0f;   // C
    reflowVars.peakTime = 25;       // sec
    
    reflowVars.startTemp = 25.0f;   // C
    reflowVars.preheatTime = 0;     // sec
    reflowVars.rampTime = 0;        // sec
    reflowVars.cooldownTime = 0;    // sec
    reflowVars.totalTime = 0;       // sec
    reflowVars.soakTempIncrease = 10.0f;    // C
    reflowVars.cooldownRamp = 5.0f; // C/sec
    
    temp = tc.readCelsius ();
    
    if (temp > reflowVars.soakTemp)
        temp = 25;
    
/**GUI*************************************************************************/
    myBui.initUI ();
    myBui.addScreen (OVEN_GRAPH, &ovenGraph);
    myBui.changeScreens (OVEN_GRAPH);
    
/**Global/Class Variables******************************************************/  
    setpoint = temp;
    startingTemp = temp;
    runtime = 0;
    reflowState = PREHEAT;
    
    ms1000 = 0;
    msPTerm = 0;
    msITerm = 0;
    msDTerm = 0;
    elementOffTime = 0;
    faultCount = 0;
    
    ovenOn = false;
    elementOn = false;
    
    pinMode (RELAY_PIN, OUTPUT);
    turnElementOff ();
    
    ovenGraph.updateStage ("Off");
    ovenGraph.updateSetpoint (setpoint);
}

void ReflowOven::loop (void) {
    myBui.runTouch ();
}

uint32_t ReflowOven::timerService (uint32_t currentTime) {
    unsigned long ms = millis (); // using millis because currentTime rolls over every ~107secs and millis rolls over every 50 days
    
    if (ms - ms1000 >= 1000) {
        ms1000 = ms;
        
        double cjt;
        bool open, sGnd, sVcc;
        double lastTemp = temp;
        bool fault = tc.readMAX31855 (&temp, &cjt, &open, &sGnd, &sVcc, false);
        
        if (fault) {
            if (faultCount == 1000) {
                turnOvenOff ();
                soundOn ();
                if (open)
                    ovenGraph.showInfo ("TC Not Connected");
                else if (sGnd)
                    ovenGraph.showInfo ("TC Grounded");
                else if (sVcc)
                    ovenGraph.showInfo ("TC Shorted to Vcc");
            } else {
                ++faultCount;
                temp = lastTemp;
            }
        } else
            faultCount = 0;
        
        if (myBui.isCurrentScreen (OVEN_GRAPH))
            ovenGraph.update ((float)temp, runtime);
        
        if (ovenOn) {
            ++runtime;
        
            if ((temp > TEMP_ALARM) || (temp > (setpoint + MAX_OVER_SETPOINT))) {
                if (reflowState != COOLDOWN) {
                    turnOvenOff ();
                    soundOn ();
                    ovenGraph.showInfo ("Oven Overheat");
                }
            }

            switch (reflowState) {
                case PREHEAT:
                    setpoint += reflowVars.preheatRamp;
                    if (setpoint >= reflowVars.soakTemp) {
                        setpoint = reflowVars.soakTemp;
                        ovenGraph.updateStage ("Soak");
                        reflowState = SOAK;
                    }
                    break;
                case SOAK:
                    setpoint += (reflowVars.soakTempIncrease / reflowVars.soakTime);
                    if (runtime >= (reflowVars.preheatTime + reflowVars.soakTime)) {
                        reflowState = RAMP;
                        ovenGraph.updateStage ("Ramp");
                    }
                    break;
                case RAMP:
                    setpoint += reflowVars.ramp;
                    if (setpoint >= reflowVars.peakTemp) {
                        setpoint = reflowVars.peakTemp;
                        reflowState = PEAK;
                        ovenGraph.updateStage ("Peak");
                    }
                    break;
                case PEAK:
                    if (runtime >= (reflowVars.preheatTime + reflowVars.soakTime + reflowVars.rampTime + reflowVars.peakTime)) {
                        soundOn ();
                        ovenGraph.showInfo ("Reflow Complete");
                        reflowState = COOLDOWN;
                        ovenGraph.updateStage ("Cooldown");
                    }
                    break;
                case COOLDOWN:
                    setpoint -= reflowVars.cooldownRamp;
                    if (runtime >= reflowVars.totalTime) {
                        turnOvenOff ();
                        reflowState = PREHEAT;
                        ovenGraph.updateButtons ();
                    }
                default:
                    break;
            }
        }
    }
    
    if (ovenOn) {
        if (temp > (startingTemp + 10)) {
            if (ms - msPTerm >= pidVars.sampleTime) {
                msPTerm = ms;
                
                ovenGraph.updateSetpoint (setpoint);
                
                float pv = (float)temp / MAX_TEMPERATURE;
                float sp = setpoint / MAX_TEMPERATURE;
                //Serial.print ("Present value: ");
                //Serial.println (pv);
                //Serial.print ("Setpoint: ");
                //Serial.println (sp);
                
                float dutyCycle = pid_run (pid, pv, sp);
                elementOffTime = pidVars.sampleTime * dutyCycle + ms;
                if (dutyCycle != 0.0f)
                    turnElementOn ();
                
                //Serial.print ("Current time: ");
                //Serial.println (ms);
                //Serial.print ("Duty cycle: ");
                //Serial.println (dutyCycle);
                
                //Serial.print ("Element off time: ");
                //Serial.println (elementOffTime);
                //Serial.println ();
                
            }
            
            // this needs to be after the new elementOffTime is calculated because a duty cycle of 1.0 was getting turned off
            if (elementOn) {
                if (ms >= elementOffTime) {
                    turnElementOff ();
                }
            }
            
            if (pidVars.iTime != 0) {
                if (ms - msITerm >= pidVars.iTime) {
                    msITerm = ms;
                    pid_calcI (pid);
                }
            }
            
            if (pidVars.dTime != 0) {
                if (ms - msDTerm >= pidVars.dTime) {
                    msDTerm = ms;
                    pid_calcD (pid);
                }
            }
        }
    }
    
    return (currentTime + CORE_TICK_RATE); // CORE_TICK_RATE is the number of ticks in 1msec
}

void ReflowOven::turnOvenOn (void) {
    ovenOn = true;
    elementOn = false;
    
    temp = (float)tc.readCelsius ();
    setpoint = temp;
    startingTemp = temp;
    runtime = 0;
    reflowState = PREHEAT;
    
    unsigned long ms = millis ();
    ms1000 = ms;
    msPTerm = ms;
    msITerm = ms;
    msDTerm = ms;
    
    pid_restart (pid);
    
    calculateReflowVars ();
    ovenGraph.updateStage ("Preheat");
    
    turnElementOn ();
}

void ReflowOven::turnOvenOff (void) {
    ovenOn = false;
    turnElementOff ();
    ovenGraph.updateStage ("Off");
}

void ReflowOven::turnElementOn (void) {
    if (reflowState == COOLDOWN)
        return;
    
    elementOn = true;
    digitalWrite (RELAY_PIN, HIGH);
}

void ReflowOven::turnElementOff (void) {
    elementOn = false;
    digitalWrite (RELAY_PIN, LOW);
}

void ReflowOven::soundOn (void) {
    analogWrite(SPEAKER_PIN, 128); 
}

void ReflowOven::soundOff (void) {
    analogWrite(SPEAKER_PIN, 0); 
}

void ReflowOven::calculateReflowVars (void) {
    reflowVars.startTemp = oven.temp;
    reflowVars.preheatTime = (uint16_t)((reflowVars.soakTemp - temp) / reflowVars.preheatRamp);
    reflowVars.rampTime = (uint16_t)((reflowVars.peakTemp - (reflowVars.soakTemp + reflowVars.soakTempIncrease)) / reflowVars.ramp);
    reflowVars.cooldownTime = (uint16_t)((reflowVars.peakTemp - reflowVars.startTemp) / reflowVars.cooldownRamp);
    reflowVars.totalTime = reflowVars.preheatTime + reflowVars.soakTime + reflowVars.rampTime + reflowVars.peakTime + reflowVars.cooldownTime;
}

ReflowOven oven;