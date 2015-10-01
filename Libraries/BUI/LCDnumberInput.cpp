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
#include <StringUtils.h>
#include "LCDcolors.h"
#include "LCDmemory.h"
#include "LCDnumberInput.h"

/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/
int8_t LCDnumInput::initNumberInput(char* title, uint8_t numberTypeVar, void* valToChangeVar, float minVar, float maxVar) {
    int8_t error;
    char varSt[20] = { 0 };
    uint8_t stLen;
    
    uint16_t row1, row2, row3, row4, row5, column1, column2, column3, column4;
    
    row1 = 95;
    row2 = 165;
    row3 = 235;
    row4 = 305;
    row5 = 375;
    column1 = 265;
    column2 = 335;
    column3 = 405;
    column4 = 475;
    
    vars = (numberInputObj)UImem.allocVarMem(sizeof(struct number_input_obj));
    if(vars == NULL) return ERR_NOMEM;
    vars->numberType = numberTypeVar;
    vars->valToChange = valToChangeVar;
    vars->value[0] = ' ';
    for(uint8_t i = 1 ; i < NUMBER_CHARS ; ++i)
        vars->value[i] = '\0';
    vars->numI = 1;
    vars->max = maxVar;
    vars->min = minVar;
    vars->negative = false;
    
    myBui.setColor(scrColors.darkRed_c);
    myBui.myLCD.fillRect(250, 15, 550, 450);
    
    myBui.setColor(scrColors.text_c);
    myBui.setBackColor(scrColors.darkRed_c);
    myBui.printCenter(title, 400, 15);
    
    myBui.myLCD.print("Max:", column1, 30);
    stLen = stringUtils.ftos(vars->max, varSt, 5);
    myBui.print(varSt, 305, 30, stLen);

    myBui.myLCD.print("Min:", column1, 45);
    stLen = stringUtils.ftos(vars->min, varSt, 5);
    myBui.print(varSt, 305, 45, stLen);
    
    drawTextBox();
    myBui.setColor(scrColors.bckgnd_c);
    myBui.myLCD.drawRect(264, 59, 536, 86);
    printTextBox();
    
    drawButton("Clear", column1, row1, true);
    drawButton("Backspace", column3, row1, true);
    drawButton("1", column1, row2);
    drawButton("2", column2, row2);
    drawButton("3", column3, row2);
    drawButton("4", column1, row3);
    drawButton("5", column2, row3);
    drawButton("6", column3, row3);
    drawButton("7", column1, row4);
    drawButton("8", column2, row4);
    drawButton("9", column3, row4);
    drawButton("Cancel", column4, row4);
    drawButton("-", column1, row5);
    drawButton("0", column2, row5);
    drawButton(".", column3, row5);
    drawButton("Okay", column4, row5);
    
    myBui.screenPopUp(POPUP_NUM_INPUT);
    
    return ERR_NOERROR;
}

void LCDnumInput::runTouch(void) {
    if (myBui.myTouch.dataAvailable()) {
        uint16_t tX, tY;
        
        myBui.myTouch.read();
        tX = myBui.myTouch.getX();
        tY = myBui.myTouch.getY();
        
        if ((tY >= 95) && (tY <= 155)) { //row one
            if ((tX >= 265) && (tX <= 395)) { //clear button
                uint8_t i;
            
                for(i=1 ; i<vars->numI ; ++i)
                    vars->value[i] = '\0';
                vars->numI = 1;
                vars->value[0] = ' ';
                vars->negative = false;
                //vars->value[1] = '0';
                printTextBox();
            } else if ((tX >= 405) && (tX <= 535)) { //backspace button
                if (vars->numI > 1) {
                    --vars->numI;
                    vars->value[vars->numI] = '\0';
                    printTextBox();
                }
            }
        } else if ((tY >= 165) && (tY <= 225)) { //row two
            if ((tX >= 265) && (tX <= 325)) //number one button
                addToChar('1');
            else if ((tX >= 335) && (tX <= 395)) //number two button
                addToChar('2');
            else if ((tX >= 405) && (tX <= 465)) //number three button
                addToChar('3');
        } else if ((tY >= 235) && (tY <= 295)) { //row three
            if ((tX >= 265) && (tX <= 325)) //number four button
                addToChar('4');
            else if ((tX >= 335) && (tX <= 395)) //number five button
                addToChar('5');
            else if ((tX >= 405) && (tX <= 465)) //number six button
                addToChar('6');
        } else if ((tY >= 305) && (tY <= 365)) { //row four
            if ((tX >= 265) && (tX <= 325)) //number seven button
                addToChar('7');
            else if ((tX >= 335) && (tX <= 395)) //number eight button
                addToChar('8');
            else if ((tX >= 405) && (tX <= 465)) //number nine button
                addToChar('9');
            else if ((tX >= 475) && (tX <= 535)) //cancel button
                myBui.returnToScreen();
        } else if ((tY >= 375) && (tY <= 435)) { //row five
            if ((tX >= 265) && (tX <= 325)) { //negative button
                if(!vars->negative) {
                    vars->value[0] = '-';
                    vars->negative = true;
                } else {
                    vars->value[0] = ' ';
                    vars->negative = false;
                }
                printTextBox();
            } else if ((tX >= 335) && (tX <= 395)) //number zero button
                addToChar('0');
            else if ((tX >= 405) && (tX <= 465)) //decimal button
                addToChar('.');
            else if ((tX >= 475) && (tX <= 535)) { //okay button
                vars->value[vars->numI] = '\0';
                switch (vars->numberType) {
                    case _NUM_TYPE_UINT8_T: {
                        int32_t v = 0;
                        if (stringUtils.stoi (&(vars->value[0]), &v)) {
                            v = checkInt (v);
                            uint8_t* ptr = (uint8_t*)vars->valToChange;
                            *ptr = (uint8_t)v;
                        }
                        break;
                    }
                    case _NUM_TYPE_FLOAT: {
                        float v = 0.0;
                        if (stringUtils.stof (&(vars->value[0]), &v)) {
                            v = checkFloat (v);
                            float* ptr = (float*)vars->valToChange;
                            *ptr = v;
                        } 
                        break;
                    }
                    case _NUM_TYPE_UINT16_T: {
                        int32_t v = 0;
                        if (stringUtils.stoi (&(vars->value[0]), &v)) {
                            v = checkInt (v);
                            uint16_t* ptr = (uint16_t*)vars->valToChange;
                            *ptr = (uint16_t)v;
                        }
                        break;
                    }
                    default:
                        break;
                }
                myBui.returnToScreen();
            }
        }
        
        while(myBui.myTouch.dataAvailable())
            continue;
    }
}

/******************************************************************************/
/* PrivateFunctions                                                           */
/******************************************************************************/
void LCDnumInput::addToChar(char charVar) {
    if(vars->numI >= NUMBER_CHARS) 
        return;
    
    vars->value[vars->numI] = charVar;
    ++vars->numI;
    printTextBox();
}

void LCDnumInput::drawButton(char* st, uint16_t x, uint16_t y, bool doubleSize) {
    uint16_t eX, eY, textX;
    
    if(!doubleSize)
        eX = x + 60;
    else
        eX = x + 130;
    
    eY = y + 60;
    
    myBui.setColor(scrColors.btn_c);
    myBui.myLCD.fillRect(x, y, eX, eY);
    
    textX = eX - x;
    textX /= 2;
    textX += x;
    
    myBui.setColor(scrColors.text_c);
    myBui.setBackColor(scrColors.btn_c);
    myBui.printCenter(st, textX, y + 22);
}

void LCDnumInput::printTextBox(void) {
    drawTextBox();
    
    myBui.setColor(scrColors.bckgnd_c);
    myBui.setBackColor(scrColors.text_c);
    myBui.printRight(vars->value, 530, 65, vars->numI);
}

void LCDnumInput::drawTextBox(void) {
    myBui.setColor(scrColors.text_c);
    myBui.myLCD.fillRect(265, 60, 535, 85);
}

int32_t LCDnumInput::checkInt (int32_t num) {
    if (num > vars->max)
        num = (int32_t)vars->max;
    if (num < vars->min)
        num = (int32_t)vars->min;
        
    return num;
}

float LCDnumInput::checkFloat (float num) {
    if (num > vars->max)
        num = vars->max;
    if (num < vars->min)
        num = vars->min;
        
    return num;
}