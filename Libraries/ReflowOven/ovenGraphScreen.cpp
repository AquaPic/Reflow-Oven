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
#include "ovenGraphScreen.h"
#include <StringUtils.h>

/******************************************************************************/
/* Variable Definition                                                        */
/******************************************************************************/
enum btn_t {
    STOP_BTN = 0,
    START_BTN,
    ACK_BTN,
    PREHEAT_H_BOX,
    SOAK_TEMP_H_BOX,
    SOAK_TIME_H_BOX,
    RAMP_H_BOX,
    PEAK_TEMP_H_BOX,
    PEAK_TIME_H_BOX,
    TOTAL_BUTTONS
};

enum t_box_t {
    TEMPERATURE_T_BOX = 0,
    RUNTIME_T_BOX,
    SETPOINT_T_BOX,
    STAGE_T_BOX,
    PREHEAT_T_BOX,
    SOAK_TEMP_T_BOX,
    SOAK_TIME_T_BOX,
    RAMP_T_BOX,
    PEAK_TEMP_T_BOX,
    PEAK_TIME_T_BOX,
    NUM_T_BOX
};

static char tBoxStrings[NUM_T_BOX][18] {
    "TC Temp: C",
    "Runtime: Sec",
    "Setpoint: C",
    "Stage",
    "Preheat Slope",
    "Soak Temperature:",
    "Soak Time:",
    "Ramp Slope:",
    "Peak Temperature:",
    "Peak Time:"
};

static char units[3][6] {
    "C/sec",
    "Sec",
    "C"
};

#define SLOPE_UNITS 0
#define TEMP_UNITS  2
#define TIME_UNITS  1

#define _WIDTH      610
#define _HEIGHT     325
#define _LEFT       175
#define _TOP        35

#define _BOX_LEFT   10

void clearString (char* st, uint8_t length);

/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/
void ovenGraphScreen::initScreen (void) {
    int8_t error;

    //*****Buttons*************************************************************/
    error = myBui.btns.initBtn (TOTAL_BUTTONS);
    if (error < 0) 
        myBui.error ("Button");
    
    myBui.btns.addBtn (STOP_BTN, 635, 395, 150, 65, &scrColors.btn_c, "Stop");
    myBui.btns.addBtn (START_BTN, 635, 395, 150, 65, &scrColors.barGFill_c, "Start", &scrColors.bckgnd_c);
    myBui.btns.addBtn (ACK_BTN, _LEFT + 220, 395, 230, 65, &scrColors.btn_c);
    myBui.btns.disableBtn (ACK_BTN);
    for (uint8_t i = 0; i < (TOTAL_BUTTONS - PREHEAT_H_BOX); ++i)
        myBui.btns.addHitBox (i + PREHEAT_H_BOX, _BOX_LEFT, i * 65 + 60, 130, 15);
    
    //*****Text Box************************************************************/
    error = myBui.textBox.initTextBox(NUM_T_BOX);
    if (error < 0)
        myBui.error ("Text Box");
    
    myBui.textBox.addTextBox (TEMPERATURE_T_BOX, _LEFT, 395, 100);
    myBui.textBox.addTextBox (RUNTIME_T_BOX, _LEFT, 435, 100);
    myBui.textBox.addTextBox (SETPOINT_T_BOX, _LEFT + 110, 395, 100);
    myBui.textBox.addTextBox (STAGE_T_BOX, _LEFT + 110, 435, 100);
    for (uint8_t i = 0; i < (NUM_T_BOX - PREHEAT_T_BOX); ++i)
        myBui.textBox.addTextBox (i + PREHEAT_T_BOX, _BOX_LEFT, i * 65 + 60, 130, 15);
    
    drawScreen ();
    
    drawn = true;
}

void ovenGraphScreen::drawScreen (void) {
    myBui.clearAndPrintHeader ("Skyler's Reflow Oven");

    drawGraph ();
    
    x2 = _LEFT + 1;
    y2 = _TOP + _HEIGHT - 1;
    
    updateButtons ();
    
    myBui.btns.drawAllBtns ();
    
    myBui.myLCD.setColor (VGA_WHITE);
    myBui.myLCD.setBackColor (VGA_BLACK);
    myBui.printCenter ("Time (seconds)", _LEFT + (_WIDTH / 2), _TOP + _HEIGHT + 4 + myBui.myLCD.getFontYsize ());
    
    myBui.myLCD.setColor (VGA_RED);
    myBui.myLCD.setBackColor (VGA_BLACK);
    myBui.myLCD.print ("Temperature (C)", _LEFT - 4 - myBui.myLCD.getFontYsize (), _TOP + (_HEIGHT / 2) - (7 * myBui.myLCD.getFontXsize ()), 90);
    
    myBui.myLCD.setColor (VGA_WHITE);
    myBui.myLCD.setBackColor (VGA_BLACK);
    myBui.myLCD.print (&(tBoxStrings[TEMPERATURE_T_BOX][0]), _LEFT, 412);
    myBui.textBox.printTextBox (TEMPERATURE_T_BOX, (float)oven.temp, 2);
    
    myBui.myLCD.setColor (VGA_WHITE);
    myBui.myLCD.setBackColor (VGA_BLACK);
    myBui.myLCD.print (&(tBoxStrings[RUNTIME_T_BOX][0]), _LEFT, 452);
    myBui.textBox.printTextBox (RUNTIME_T_BOX, (int32_t)oven.runtime);
    
    myBui.myLCD.setColor (VGA_WHITE);
    myBui.myLCD.setBackColor (VGA_BLACK);
    myBui.myLCD.print (&(tBoxStrings[SETPOINT_T_BOX][0]), _LEFT + 110, 412);
    myBui.textBox.printTextBox (SETPOINT_T_BOX, (float)oven.temp, 2);
    
    myBui.myLCD.setColor (VGA_WHITE);
    myBui.myLCD.setBackColor (VGA_BLACK);
    myBui.myLCD.print (&(tBoxStrings[STAGE_T_BOX][0]), _LEFT + 110, 452);
    myBui.textBox.printTextBox (STAGE_T_BOX, "Off");
    
    myBui.myLCD.setColor (VGA_WHITE);
    myBui.myLCD.setBackColor (VGA_BLACK);
    myBui.myLCD.print (&(tBoxStrings[PREHEAT_T_BOX][0]), _BOX_LEFT, 30);
    myBui.myLCD.print (&(units[SLOPE_UNITS][0]), _BOX_LEFT, 45);
    myBui.textBox.printTextBox (PREHEAT_T_BOX, oven.reflowVars.preheatRamp, 1);
    
    myBui.myLCD.setColor (VGA_WHITE);
    myBui.myLCD.setBackColor (VGA_BLACK);
    myBui.myLCD.print (&(tBoxStrings[SOAK_TEMP_T_BOX][0]), _BOX_LEFT, 95);
    myBui.myLCD.print (&(units[TEMP_UNITS][0]), _BOX_LEFT, 110);
    myBui.textBox.printTextBox (SOAK_TEMP_T_BOX, oven.reflowVars.soakTemp, 1);
    
    myBui.myLCD.setColor (VGA_WHITE);
    myBui.myLCD.setBackColor (VGA_BLACK);
    myBui.myLCD.print (&(tBoxStrings[SOAK_TIME_T_BOX][0]), _BOX_LEFT, 160);
    myBui.myLCD.print (&(units[TIME_UNITS][0]), _BOX_LEFT, 175);
    myBui.textBox.printTextBox (SOAK_TIME_T_BOX, (int32_t)oven.reflowVars.soakTime);
    
    myBui.myLCD.setColor (VGA_WHITE);
    myBui.myLCD.setBackColor (VGA_BLACK);
    myBui.myLCD.print (&(tBoxStrings[RAMP_T_BOX][0]), _BOX_LEFT, 225);
    myBui.myLCD.print (&(units[SLOPE_UNITS][0]), _BOX_LEFT, 240);
    myBui.textBox.printTextBox (RAMP_T_BOX, oven.reflowVars.ramp, 1);
    
    myBui.myLCD.setColor (VGA_WHITE);
    myBui.myLCD.setBackColor (VGA_BLACK);
    myBui.myLCD.print (&(tBoxStrings[PEAK_TEMP_T_BOX][0]), _BOX_LEFT, 290);
    myBui.myLCD.print (&(units[TEMP_UNITS][0]), _BOX_LEFT, 305);
    myBui.textBox.printTextBox (PEAK_TEMP_T_BOX, oven.reflowVars.peakTemp, 1);
    
    myBui.myLCD.setColor (VGA_WHITE);
    myBui.myLCD.setBackColor (VGA_BLACK);
    myBui.myLCD.print (&(tBoxStrings[PEAK_TIME_T_BOX][0]), _BOX_LEFT, 355);
    myBui.myLCD.print (&(units[TIME_UNITS][0]), _BOX_LEFT, 370);
    myBui.textBox.printTextBox (PEAK_TIME_T_BOX, (int32_t)oven.reflowVars.peakTime);
    
    myBui.myLCD.setColor (VGA_GRAY);
    myBui.myLCD.drawLine (145, 0, 145, 480);
}

void ovenGraphScreen::disposeScreen (void) {
    drawn = false;    
    UImem.resetMemory ();
}

void ovenGraphScreen::runTouch (void) {
    if(drawn == true) {
        int8_t result;
    
        result = myBui.btns.checkBtn();
        switch (result) {
            case STOP_BTN:
                oven.turnOvenOff ();
                updateButtons ();
                break;
            case START_BTN:
                oven.turnOvenOn ();
                drawGraph ();
                updateButtons ();
                break;
            case ACK_BTN:
                oven.soundOff ();
                hideInfo ();
                break;
            case PREHEAT_H_BOX:
                myBui.numInput.initNumberInput  (&(tBoxStrings[PREHEAT_T_BOX][0]), 
                                                _NUM_TYPE_FLOAT,
                                                (void*)&(oven.reflowVars.preheatRamp),
                                                0.5f,
                                                6.0f);
                break;
            case SOAK_TEMP_H_BOX:
                myBui.numInput.initNumberInput  (&(tBoxStrings[SOAK_TEMP_T_BOX][0]), 
                                                _NUM_TYPE_FLOAT,
                                                (void*)&(oven.reflowVars.soakTemp),
                                                100.0f,
                                                oven.reflowVars.peakTemp - 10);
                break;
            case SOAK_TIME_H_BOX:
                myBui.numInput.initNumberInput  (&(tBoxStrings[SOAK_TIME_T_BOX][0]), 
                                                _NUM_TYPE_UINT16_T,
                                                (void*)&(oven.reflowVars.soakTime),
                                                5,
                                                600);
                break;
            case RAMP_H_BOX:
                myBui.numInput.initNumberInput  (&(tBoxStrings[RAMP_T_BOX][0]), 
                                                _NUM_TYPE_FLOAT,
                                                (void*)&(oven.reflowVars.ramp),
                                                0.5f,
                                                6.0f);
                break;
            case PEAK_TEMP_H_BOX:
                myBui.numInput.initNumberInput  (&(tBoxStrings[PEAK_TEMP_T_BOX][0]), 
                                                _NUM_TYPE_FLOAT,
                                                (void*)&(oven.reflowVars.peakTemp),
                                                oven.reflowVars.soakTemp + 10,
                                                300.0f);
                break;
            case PEAK_TIME_H_BOX:
                myBui.numInput.initNumberInput  (&(tBoxStrings[PEAK_TIME_T_BOX][0]), 
                                                _NUM_TYPE_UINT16_T,
                                                (void*)&(oven.reflowVars.peakTime),
                                                5,
                                                60);
            default:
                break;
        }
    }
}

void ovenGraphScreen::update (float temp, uint16_t time) {
    uint16_t intTemp = (uint16_t)temp;
    myBui.textBox.printTextBox (TEMPERATURE_T_BOX, temp, 2);
    myBui.textBox.printTextBox (RUNTIME_T_BOX, (int32_t)time);
    
    if (oven.ovenOn) {
        float percent = (float)time / (float)fullScaleTime;
        uint16_t x = (uint16_t)((float)_WIDTH * percent) + _LEFT;
        
        percent = (temp - oven.reflowVars.startTemp) / fullScaleTemp;
        uint16_t y = (_HEIGHT - (uint16_t)((float)_HEIGHT * percent)) + _TOP;
        
        if ((x > _LEFT) && (x < (_LEFT + _WIDTH))) {
            if ((y > _TOP) && (y < (_TOP + _HEIGHT))) {
                myBui.myLCD.setColor (VGA_FUCHSIA);
                drawThickLine (x2, y2, x, y);
            }
        }
        
        x2 = x;
        y2 = y;
    }
}

void ovenGraphScreen::updateSetpoint (float setpoint) {
    myBui.textBox.printTextBox (SETPOINT_T_BOX, setpoint, 2);
}

void ovenGraphScreen::updateStage (char* st) {
    myBui.textBox.printTextBox (STAGE_T_BOX, st);
}

void ovenGraphScreen::drawGraph (void) {
    oven.calculateReflowVars ();
    
    uint16_t width = _WIDTH;
    uint16_t height = _HEIGHT;
    uint16_t x = _LEFT;
    uint16_t left = x;
    x2 = left + width;
    uint16_t y = _TOP;
    uint16_t top = y;
    uint16_t bottom = top + height;
    
    char st[10] = { 0 };
    
    float currentTemp = oven.reflowVars.startTemp;
    
    fullScaleTemp = oven.reflowVars.peakTemp + 15.0f - currentTemp;
    
    //draw graph background
    myBui.myLCD.setColor (VGA_SILVER);
    myBui.myLCD.fillRect (left, top, x2, bottom);
    
    //print 0 time
    myBui.myLCD.setColor (VGA_WHITE);
    myBui.myLCD.setBackColor (VGA_BLACK);
    myBui.myLCD.print ("0", left + 1, bottom + 2);
    
    //print full time
    fullScaleTime = oven.reflowVars.totalTime;
    uint8_t stLen = stringUtils.itos(fullScaleTime, st);
    myBui.printRight (st, x2 - 1, bottom + 2, stLen);
    
    //print current temp
    myBui.myLCD.setColor (VGA_RED);
    clearString (st, 10);
    stLen = stringUtils.ftos(oven.temp, st, 2);
    myBui.myLCD.print (st, left - 2, bottom - (stLen * myBui.myLCD.getFontXsize ()), 90);
    
/******************************************************************************/ 
    //determine x (time) coordinate for preheat 
    uint16_t time = oven.reflowVars.preheatTime;
    float percent = (float)time / (float)fullScaleTime;
    x = (uint16_t)((float)width * percent) + left;
    
    //print x (time) coordinate for preheat
    myBui.myLCD.setColor (VGA_WHITE);
    clearString (st, 10);
    stLen = stringUtils.itos(time, st);
    myBui.printCenter (st, x, bottom + 2, stLen);
    
    //determine y (temp) coordinate for preheat
    float temp = oven.reflowVars.soakTemp;
    percent = (temp - currentTemp) / fullScaleTemp;
    y = (height - (uint16_t)((float)height * percent)) + top;
    
    //print y (temp) coordinate for preheat 
    myBui.myLCD.setColor (VGA_RED);
    clearString (st, 10);
    stLen = stringUtils.itos(temp, st);
    myBui.myLCD.print (st, left - 2, y, 90);
    
    //draw lines for preheat
    myBui.myLCD.setColor (VGA_GRAY);
    myBui.myLCD.drawLine (left, y, x, y); // temp line
    myBui.myLCD.drawLine (x, y, x, bottom); // time line
    myBui.myLCD.setColor (VGA_LIME);
    drawThickLine (left + 1, bottom - 1, x, y); // setpoint slop
    
    x2 = x;
    y2 = y;

/******************************************************************************/     
    //determine x (time) coordinate for soak 
    time = oven.reflowVars.preheatTime + oven.reflowVars.soakTime;
    percent = (float)time / (float)fullScaleTime;
    x = (uint16_t)((float)width * percent) + left;
    
    //print x (time) coordinate for soak 
    myBui.myLCD.setColor (VGA_WHITE);
    clearString (st, 10);
    stLen = stringUtils.itos(time, st);
    myBui.printCenter (st, x, bottom + 2, stLen);
    
    //print y (temp) coordinate for preheat 
    temp = oven.reflowVars.soakTemp + oven.reflowVars.soakTempIncrease;
    percent = (temp - currentTemp) / fullScaleTemp;
    y = (height - (uint16_t)((float)height * percent)) + top;
    
    //print y (temp) coordinate for preheat 
    myBui.myLCD.setColor (VGA_RED);
    clearString (st, 10);
    stLen = stringUtils.itos(temp, st);
    myBui.myLCD.print (st, left - 2, y - (stLen * myBui.myLCD.getFontXsize ()), 90);
    
    //draw lines for soak
    myBui.myLCD.setColor (VGA_GRAY);
    myBui.myLCD.drawLine (left, y, x, y); // temp line
    myBui.myLCD.drawLine (x, y, x, bottom); // time line
    myBui.myLCD.setColor (VGA_LIME);
    drawThickLine (x2, y2, x, y); // setpoint slop
    
    x2 = x;
    y2 = y;

/******************************************************************************/    
    //determine x (time) coordinate for ramp 
    time = oven.reflowVars.preheatTime + oven.reflowVars.soakTime + oven.reflowVars.rampTime;
    percent = (float)time / (float)fullScaleTime;
    x = (uint16_t)((float)width * percent) + left;
    
    //print x (time) coordinate for ramp 
    myBui.myLCD.setColor (VGA_WHITE);
    clearString (st, 10);
    stLen = stringUtils.itos(time, st);
    myBui.printCenter (st, x, bottom + 2, stLen);
    
    //determine y (temp) coordinate for ramp
    temp = oven.reflowVars.peakTemp;
    percent = (temp - currentTemp) / fullScaleTemp;
    y = (height - (uint16_t)((float)height * percent)) + top;
    
    //print y (temp) coordinate for ramp 
    myBui.myLCD.setColor (VGA_RED);
    clearString (st, 10);
    stLen = stringUtils.itos(temp, st);
    myBui.myLCD.print (st, left - 2, y - 5, 90);
    
    //draw lines for ramp
    myBui.myLCD.setColor (VGA_GRAY);
    myBui.myLCD.drawLine (left, y, x, y); // temp line
    myBui.myLCD.drawLine (x, y, x, bottom); // time line
    myBui.myLCD.setColor (VGA_LIME);
    drawThickLine (x2, y2, x, y); // setpoint slop
    
    x2 = x;
    y2 = y;
    
/******************************************************************************/
    //determine x (time) coordinate for peak 
    time = oven.reflowVars.preheatTime + oven.reflowVars.soakTime + oven.reflowVars.rampTime + oven.reflowVars.peakTime;
    percent = (float)time / (float)fullScaleTime;
    x = (uint16_t)((float)width * percent) + left;
    
    //print x (time) coordinate for ramp 
    myBui.myLCD.setColor (VGA_WHITE);
    clearString (st, 10);
    stLen = stringUtils.itos(time, st);
    myBui.printCenter (st, x, bottom + 2, stLen);
    
    //draw lines for peak
    myBui.myLCD.setColor (VGA_GRAY);
    myBui.myLCD.drawLine (x, y, x, bottom); // time line
    myBui.myLCD.setColor (VGA_LIME);
    drawThickLine (x2, y2, x, y); // setpoint slop
    
    x2 = x;
    
/******************************************************************************/
    //determine x (time) coordinate for peak 
    x = left + width - 1;
    y = top + height - 1;
    
    drawThickLine (x2, y2, x, y); // setpoint slop
}

void clearString (char* st, uint8_t length) {
    for (uint8_t i = 0; i < length; ++i)
        st[i] = '\0';
}

void ovenGraphScreen::showInfo (char* message) {
    if (!infoShown) {
        infoShown = true;
        myBui.btns.enableBtn (ACK_BTN);
        myBui.btns.drawBtn (ACK_BTN);
        myBui.myLCD.setColor (VGA_WHITE);
        myBui.myLCD.setBackColor (VGA_RED);
        myBui.printCenter (message, _LEFT + 330, 427 - (myBui.myLCD.getFontXsize () / 2));
    }
}

void ovenGraphScreen::hideInfo (void) {
    infoShown = false;
    myBui.btns.disableBtn (ACK_BTN);
    myBui.myLCD.setColor (VGA_BLACK);
    myBui.myLCD.fillRect (_LEFT + 220, 395, 625, 460);
}

void ovenGraphScreen::disableHitBoxes (void) {
    myBui.btns.disableBtn (PREHEAT_H_BOX);
    myBui.btns.disableBtn (SOAK_TEMP_H_BOX);
    myBui.btns.disableBtn (SOAK_TIME_H_BOX);
    myBui.btns.disableBtn (RAMP_H_BOX);
    myBui.btns.disableBtn (PEAK_TEMP_H_BOX);
    myBui.btns.disableBtn (PEAK_TIME_H_BOX);
}

void ovenGraphScreen::enableHitBoxes (void) {
    myBui.btns.enableBtn (PREHEAT_H_BOX);
    myBui.btns.enableBtn (SOAK_TEMP_H_BOX);
    myBui.btns.enableBtn (SOAK_TIME_H_BOX);
    myBui.btns.enableBtn (RAMP_H_BOX);
    myBui.btns.enableBtn (PEAK_TEMP_H_BOX);
    myBui.btns.enableBtn (PEAK_TIME_H_BOX);
}

void ovenGraphScreen::updateButtons (void) {
    if (oven.ovenOn) {
        myBui.btns.disableBtn (START_BTN);
        myBui.btns.enableBtn (STOP_BTN);
        myBui.btns.drawBtn (STOP_BTN);
        disableHitBoxes ();
    } else {
        myBui.btns.disableBtn (STOP_BTN);
        myBui.btns.enableBtn (START_BTN);
        myBui.btns.drawBtn (START_BTN);
        enableHitBoxes ();
    }
}

void ovenGraphScreen::drawThickLine (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    myBui.myLCD.drawLine (x1, y1, x2, y2); 
    myBui.myLCD.drawLine (x1 - 1, y1 - 1, x2 - 1, y2 - 1);
    myBui.myLCD.drawLine (x1 + 1, y1 + 1, x2 + 1, y2 + 1);
    myBui.myLCD.drawLine (x1 + 1, y1, x2 + 1, y2);
    myBui.myLCD.drawLine (x1 - 1, y1, x2 - 1, y2);
    myBui.myLCD.drawLine (x1, y1 - 1, x2, y2 - 1);
    myBui.myLCD.drawLine (x1, y1 + 1, x2, y2 + 1);
}