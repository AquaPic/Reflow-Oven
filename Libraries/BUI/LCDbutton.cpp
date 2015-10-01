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
#include "LCDbutton.h"

/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/
int8_t LCDbutton::initBtn(int numBtns) {
    btnQue = (LCDbtnObj)UImem.allocConMem(sizeof(struct lcd_btn_obj) * numBtns);
    if(btnQue == NULL) return ERR_NOMEM;
    numBtn = numBtns;
    return ERR_NOERROR;
}



int8_t LCDbutton::addBtn   (int         quePtr, 
                            uint16_t    x, 
                            uint16_t    y, 
                            uint16_t    length, 
                            uint16_t    height, 
                            myColors*   btnColorVar, 
                            char*       textVar, 
                            myColors*   btnTextColorVar, 
                            bool        scnChangeVar, 
                            bool        hitBoxVar, 
                            bool        enabledVar)
{
    //Serial.print("Adding Button ");
    //Serial.println(quePtr);
    if(quePtr >= numBtn) return ERR_UNKNOWN;

    btnQue[quePtr].sX = x;
    //Serial.print("Start X: ");
    //Serial.println(btnQue[quePtr].sX);
    
    btnQue[quePtr].sY = y;
    //Serial.print("Start Y: ");
    //Serial.println(btnQue[quePtr].sY);
    
    btnQue[quePtr].eX = btnQue[quePtr].sX + length;
    //Serial.print("End X: ");
    //Serial.println(btnQue[quePtr].eX);
    
    btnQue[quePtr].eY = btnQue[quePtr].sY + height;
    //Serial.print("End Y: ");
    //Serial.println(btnQue[quePtr].eY);
    
    btnQue[quePtr].btnColor = btnColorVar;
    btnQue[quePtr].text = textVar;
    btnQue[quePtr].btnTextColor = btnTextColorVar;
    
    btnQue[quePtr].scnChange = scnChangeVar;
    //Serial.print("Screen Change: ");
    //Serial.println(btnQue[quePtr].scnChange);
    
    btnQue[quePtr].hitBox = hitBoxVar;
    //Serial.print("Hit Box: ");
    //Serial.println(btnQue[quePtr].hitBox);
    
    btnQue[quePtr].enabled = enabledVar;
    //Serial.print("Enabled: ");
    //Serial.println(btnQue[quePtr].enabled);
    
    //Serial.println("Returning From Button");
    
    return ERR_NOERROR;
}

int8_t LCDbutton::addHitBox(int quePtr, uint16_t x, uint16_t y, uint16_t length, uint16_t height) {
    return addBtn(quePtr, x, y, length, height, &scrColors.bckgnd_c, NULL, &scrColors.text_c, BTN_REDRAW, BTN_HIT_BOX);
}

int8_t LCDbutton::checkBtn(void) {
    if (myBui.myTouch.dataAvailable()) {
        uint16_t tX, tY;
        int8_t result = -1;
        uint8_t i = 0;
        
        myBui.myTouch.read();
        tX = myBui.myTouch.getX();
        tY = myBui.myTouch.getY();
        //Serial.print(tX);
        //Serial.print(",");
        //Serial.println(tY);
        
        for(i=0 ; i<numBtn ; ++i)
        {
            if((tX >= btnQue[i].sX) && (tX <= btnQue[i].eX) && (tY >= btnQue[i].sY) && (tY <= btnQue[i].eY))
            {
                if (btnQue[i].enabled) {
                    result = i;
                    //Serial.print("Button number ");
                    //Serial.println(result);
                    break;
                }
            }
        }
        
        if(result != -1)
        {
            setBtnHLColor(result);
            myBui.myLCD.drawRect(btnQue[result].sX, btnQue[result].sY, btnQue[result].eX, btnQue[result].eY);
            
            while(myBui.myTouch.dataAvailable())
            {
                continue;
            }
            
            if(!btnQue[result].scnChange)
            {
                setBtnColor(result);
                myBui.myLCD.drawRect(btnQue[result].sX, btnQue[result].sY, btnQue[result].eX, btnQue[result].eY);
            }
        }
        return result;
    }
    else
    {
        return -1;
    }
}



void LCDbutton::drawBtn(int quePtr)
{
    if(btnQue[quePtr].enabled && !btnQue[quePtr].hitBox)
    {
        //Serial.println("Drawing Button");
        
        setBtnColor(quePtr);
        myBui.myLCD.fillRect(btnQue[quePtr].sX, btnQue[quePtr].sY, btnQue[quePtr].eX, btnQue[quePtr].eY);
        
        if(btnQue[quePtr].text != NULL)
        {
            uint16_t textX, textY, length, height;
            
            length = btnQue[quePtr].eX - btnQue[quePtr].sX;
            textX = length / 2;
            textX += btnQue[quePtr].sX;
            textX -= ((strlen(btnQue[quePtr].text) * (myBui.myLCD.getFontXsize() - 1)) / 2);
            
            height = btnQue[quePtr].eY - btnQue[quePtr].sY;
            textY =  height / 2;
            textY += btnQue[quePtr].sY;
            textY -= (myBui.myLCD.getFontYsize() / 2);
            
            myBui.myLCD.setFont(SmallFont);
            setBtnTextColor(quePtr);
            setBtnBackColor(quePtr);
            myBui.myLCD.print(btnQue[quePtr].text, textX, textY);
        }
    }
}



void LCDbutton::drawAllBtns(void)
{
    int i;
    
    for(i=0 ; i<numBtn ; ++i)
    {
        drawBtn(i);
    }
}



/******************************************************************************/
/* PrivateFunctions                                                           */
/******************************************************************************/
void LCDbutton::setBtnColor(int quePtr)
{
    myBui.setColor(*btnQue[quePtr].btnColor);
}



void LCDbutton::setBtnBackColor(int quePtr)
{
    myBui.setBackColor(*btnQue[quePtr].btnColor);
}



void LCDbutton::setBtnHLColor(int quePtr)
{
    myColors hlColor;
    float tf;
    
    tf = (float)btnQue[quePtr].btnColor->sRed;
    tf *= 0.50;
    hlColor.sRed = (byte)tf;
    //hlColor.sRed = *btnQue[quePtr].btnColor.sRed - 100;
    
    tf = (float)btnQue[quePtr].btnColor->sGreen;
    tf *= 0.50;
    hlColor.sGreen = (byte)tf;
    //hlColor.sGreen = *btnQue[quePtr].btnColor.sGreen - 100;
    
    tf = (float)btnQue[quePtr].btnColor->sBlue;
    tf *= 0.50;
    hlColor.sBlue = (byte)tf;
    //hlColor.sBlue = *btnQue[quePtr].btnColor.sBlue - 100;
    
    myBui.setColor(hlColor);
}



void LCDbutton::setBtnTextColor(int quePtr)
{
    myBui.setColor(*btnQue[quePtr].btnTextColor);
}