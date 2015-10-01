/*******************************************************************************
 * Created by Skyler Brandt on April 2014
 * AquaPic
 *
 *******************************************************************************
 * Copyright (C) 2013 Skyler Brandt
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
 
#ifndef LCD_BUTTON_H
#define LCD_BUTTON_H
 
/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include "LCDcolors.h"

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/
/*******Errors List************************************************************/
#define ERR_NOERROR     0  // no error has occurred
#define ERR_UNKNOWN     -1 // unknown error
#define ERR_NOMEM       -2 // out of memory

/*******User Defined***********************************************************/
#define BTN_SCN_CHANGE  true
#define BTN_REDRAW      false
#define BTN_HIT_BOX     true
#define BTN_NORM_BTN    false

/*******EEPROM STUFF***********************************************************/

/******************************************************************************/
/* Variable Definition                                                        */
/******************************************************************************/
struct lcd_btn_obj
{
    uint16_t sX;
    uint16_t sY;
    uint16_t eX;
    uint16_t eY;
    myColors* btnColor;
    myColors* btnTextColor;
    char* text;
    uint8_t enabled:1;
    uint8_t scnChange:1;
    uint8_t hitBox:1;
};

typedef struct lcd_btn_obj* LCDbtnObj;

/******************************************************************************/
/* Class Definition                                                           */
/******************************************************************************/
class LCDbutton
{
public:
    LCDbutton (void) : numBtn(0), btnQue(0) {};
    int8_t initBtn (int numBtns);
    //addBtn(   query location,
    //          start x,
    //          start y,
    //          length,
    //          height,
    //          (optional) button color, --default: black
    //          (optional) text, --default: void
    //          (optional) text color, --default: white
    //          (optional) screen change button, --BTN_SCN_CHANGE (don't redraw button, changing screens) -OR- BTN_REDRAW (redraw button) --default: BTN_REDRAW
    //          (optional) hit box, --BTN_HIT_BOX (hit box button, doesn't draw only highlight) -OR- BTN_NORM_BTN (normally drawn button) --default: BTN_NORM_BTN
    //          (optional) enabled) --default: true
    int8_t addBtn (int quePtr, uint16_t x, uint16_t y, uint16_t length, uint16_t height, myColors* btnColorVar = &scrColors.bckgnd_c, char* textVar = NULL, myColors* btnTextColorVar = &scrColors.text_c, bool scnChangeVar = BTN_REDRAW, bool hitBoxVar = BTN_NORM_BTN, bool enabledVar = true);
    int8_t addHitBox (int quePtr, uint16_t x, uint16_t y, uint16_t length, uint16_t height);
    int8_t checkBtn (void);
    void drawBtn (int quePtr);
    void drawAllBtns (void);
    
    void disableBtn (int quePtr) { btnQue[quePtr].enabled = false; };
    void enableBtn (int quePtr)  { btnQue[quePtr].enabled = true; };

private:
    void setBtnColor (int quePtr);
    void setBtnBackColor (int quePtr);
    void setBtnHLColor (int quePtr);
    void setBtnTextColor (int quePtr);
    
    uint8_t numBtn;
    LCDbtnObj btnQue;
};

#endif /* LCD_BUTTON_H */