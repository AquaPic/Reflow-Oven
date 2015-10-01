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

#ifndef AQUA_PIC_UI_H
#define AQUA_PIC_UI_H

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <UTFT.h>
#include <UTouch.h>

#include "LCDbutton.h"
#include "LCDtextBox.h"

#include "LCDnumberInput.h"

#include "LCDcolors.h"
#include "LCDmemory.h"

#include "LCDscreen.h"

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/
/*******Errors List************************************************************/
#define ERR_NOERROR     0  // no error has occurred
#define ERR_UNKNOWN     -1 // unknown error
#define ERR_NOMEM       -2 // out of memory

/*******User Defined***********************************************************/
#define LCD_BIGHTLIGHT  5
#define FULL_BRIGHT     255

#define POPUP_NONE      0
#define POPUP_NUM_INPUT 1

#define UI_LEFT_JUST    true
#define UI_RIGHT_JUST   false

#define sEnabled        true
#define sDisabled       false

#define MAX_SCREENS     5

/*******EEPROM STUFF***********************************************************/

/******************************************************************************/
/* Variable Definition                                                        */
/******************************************************************************/
typedef struct current_data {
    word color;
    word backColor;
    uint8_t* font;
}currentData;

/******************************************************************************/
/* Class Definition                                                           */
/******************************************************************************/
class BUI {
public:
    BUI() : myLCD(CTE70,82,83,85,84), myTouch(45,46,47,48,49) {};
    void initUI (void);
    void addScreen (uint8_t index, iLcdScreen* newScreen);
    void runTouch (void);
    void changeScreens (uint8_t screen);
    void screenPopUp (uint8_t popup);
    void returnToScreen (void);
    bool isCurrentScreen (uint8_t screen);
    
    void clearAndPrintHeader (char* st);
    void error (char* st);
    void print (char* st, uint16_t x, uint16_t y, uint8_t stLen);
    void printRight (char* st, uint16_t x, uint16_t y, uint8_t stLen = 0);
    void printCenter (char* st, uint16_t x, uint16_t y, uint8_t stLen = 0);
    void saveCurrent ();
    void restoreCurrent ();
    void setColor (myColors cc);
    void setBackColor (myColors cc);
    void setGeneralColors (void);
    void setBrightness (uint8_t brightness);
    uint8_t getBrightness (void);
    void* getBrightnessAddess (void);
    
    //LCD and Touch Drivers
    UTFT myLCD;
    UTouch myTouch;
    
    LCDbutton btns;
    LCDtBox textBox;
    LCDnumInput numInput;
    
private:
    iLcdScreen* screens[MAX_SCREENS];
    currentData current;
    uint8_t currentScreen;
    uint8_t displayPopUp;
    uint8_t backlightBrighness;
};

extern BUI myBui;

//extern uint8_t BigFont[];
extern uint8_t SmallFont[];

#endif