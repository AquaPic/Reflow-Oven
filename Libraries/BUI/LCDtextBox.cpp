/*******************************************************************************
 * Created by Skyler Brandt on February 2014
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

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <WProgram.h>
#include <inttypes.h>
#include <UTFT.h>
#include <UTouch.h>
#include <BUI.h>
#include "LCDcolors.h"
#include "LCDmemory.h"
#include "LCDtextBox.h"
#include <StringUtils.h>

#define T_BOX_HEIGHT    15

/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/
int8_t LCDtBox::initTextBox(int numTBoxes) {
    tBoxQue = (LCDtBoxObj)UImem.allocConMem(sizeof(struct lcd_tBox_obj) * numTBoxes);
    if (tBoxQue == NULL) 
        return ERR_NOMEM;
    numTBox = numTBoxes;
    return ERR_NOERROR;
}

int8_t LCDtBox::addTextBox(int quePtr, uint16_t x, uint16_t y, uint16_t lengthVar, bool enabledVar) {
    if (quePtr >= numTBox) 
        return ERR_UNKNOWN;
    
    tBoxQue[quePtr].sX = x;
    tBoxQue[quePtr].sY = y;
    tBoxQue[quePtr].length = lengthVar;
    tBoxQue[quePtr].enabled = enabledVar;
    
    return ERR_NOERROR;
}

void LCDtBox::drawTextBox(int quePtr) {
    if(tBoxQue[quePtr].enabled) {
        uint16_t seX, seY;
        seX = tBoxQue[quePtr].sX + tBoxQue[quePtr].length;
        seY = tBoxQue[quePtr].sY + T_BOX_HEIGHT;
        
        setWhiteColor();
        myBui.myLCD.fillRect(tBoxQue[quePtr].sX, tBoxQue[quePtr].sY, seX, seY);
        setBlackColor();
        myBui.myLCD.drawRect(tBoxQue[quePtr].sX - 1, tBoxQue[quePtr].sY - 1, seX + 1, seY + 1);
    }
}

void LCDtBox::drawAllTextBoxes(void) {
    int i;
    
    for (i=0 ; i<numTBox ; ++i)
        drawTextBox(i);
}

void LCDtBox::clearTextBox (int quePtr) {
    if (tBoxQue[quePtr].enabled) {
        uint16_t seX, seY;
        seX = tBoxQue[quePtr].sX + tBoxQue[quePtr].length;
        seY = tBoxQue[quePtr].sY + T_BOX_HEIGHT;
        
        setWhiteColor();
        myBui.myLCD.fillRect(tBoxQue[quePtr].sX, tBoxQue[quePtr].sY, seX, seY);
    }
}

void LCDtBox::printTextBox(int quePtr, char* st, uint8_t stLen) {
    if (tBoxQue[quePtr].enabled) {
        uint16_t maxChar, ssX;
        
        if (stLen == 0) 
            stLen = strlen(st);
        
        maxChar = (tBoxQue[quePtr].length - 4) / myBui.myLCD.getFontXsize();
        
        if(stLen > maxChar) 
            stLen = maxChar;
        
        ssX = (tBoxQue[quePtr].length / 2) + tBoxQue[quePtr].sX;
        
        clearTextBox(quePtr);
        setTextColor();
        myBui.printCenter(st, ssX, tBoxQue[quePtr].sY + 2, stLen);
    }
}

void LCDtBox::printTextBox(int quePtr, float number, uint8_t decRes) {
    if (tBoxQue[quePtr].enabled) {    
        char varSt[10] = { 0 };
        uint8_t stLen;
        
        stLen = stringUtils.ftos(number, varSt, decRes);
        printTextBox(quePtr, varSt, stLen);
    }
}

void LCDtBox::printTextBox(int quePtr, int32_t number) {
    if(tBoxQue[quePtr].enabled) {    
        char varSt[10] = { 0 };
        uint8_t stLen;
        
        stLen = stringUtils.itos(number, varSt);
        printTextBox(quePtr, varSt, stLen);
    }
}

/******************************************************************************/
/* PrivateFunctions                                                           */
/******************************************************************************/
void LCDtBox::setWhiteColor(void) {
    myBui.setColor(scrColors.text_c);
}

void LCDtBox::setBlackColor(void) {
    myBui.setColor(scrColors.bckgnd_c);
}

void LCDtBox::setTextColor(void) {
    myBui.setColor(scrColors.bckgnd_c);
    myBui.setBackColor(scrColors.text_c);
}