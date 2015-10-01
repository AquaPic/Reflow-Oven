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
 
#ifndef LCD_TEXT_BOX_H
#define LCD_TEXT_BOX_H
 
/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/
/*******Errors List************************************************************/
#define ERR_NOERROR     0  // no error has occurred
#define ERR_UNKNOWN     -1 // unknown error
#define ERR_NOMEM       -2 // out of memory

/*******User Defined***********************************************************/

/*******EEPROM STUFF***********************************************************/

/******************************************************************************/
/* Variable Definition                                                        */
/******************************************************************************/
struct lcd_tBox_obj
{
    uint16_t sX;
    uint16_t sY;
    uint16_t length;
    uint8_t enabled:1;
};

typedef struct lcd_tBox_obj* LCDtBoxObj;

/******************************************************************************/
/* Class Definition                                                           */
/******************************************************************************/
class LCDtBox
{
public:
    LCDtBox(void) : numTBox(0), tBoxQue(0) {};
    int8_t initTextBox(int numTBoxes);
    int8_t addTextBox(int quePtr, uint16_t x, uint16_t y, uint16_t lengthVar = 55, bool enabledVar = true);
    void drawTextBox(int quePtr);
    void drawAllTextBoxes(void);
    void clearTextBox(int quePtr);
    void printTextBox(int quePtr, char* st, uint8_t stLen = 0);
    void printTextBox(int quePtr, float number, uint8_t decRes = 1);
    void printTextBox(int quePtr, int32_t number);
    
    void disableTextBox(int quePtr)    { tBoxQue[quePtr].enabled = false; };
    void enableTextBox(int quePtr)     { tBoxQue[quePtr].enabled = true; };

private:
    void setWhiteColor(void);
    void setBlackColor(void);
    void setTextColor(void);
    
    uint8_t numTBox;
    LCDtBoxObj tBoxQue;
};

#endif /* LCD_TEXT_BOX_H */