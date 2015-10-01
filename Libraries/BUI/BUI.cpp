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

#include <WProgram.h>
#include <inttypes.h>
#include <BUI.h>

void BUI::initUI(void) {
    //Backlight
    pinMode(LCD_BIGHTLIGHT, OUTPUT);
    setBrightness(FULL_BRIGHT);

    //LCD Init
    myLCD.InitLCD ();
    myLCD.clrScr ();
    myLCD.setFont (SmallFont);

    //Touchscreen Init
    myTouch.InitTouch();
    myTouch.setPrecision(PREC_MEDIUM);
    
    currentScreen = 0;
    for (int i = 0; i < MAX_SCREENS; ++i)
        screens[i] = NULL;
    
    setGeneralColors();
}

void BUI::addScreen(uint8_t index, iLcdScreen* newScreen) {
    if ((index >= MAX_SCREENS) || (index < 0))
        return;
        
    if (newScreen != NULL)
        screens[index] = newScreen;
}

void BUI::runTouch(void) { 
    if(myTouch.dataAvailable()) {
        if(displayPopUp == POPUP_NONE) {
            if (screens[currentScreen] != NULL)
                screens[currentScreen]->runTouch ();
        } else {
            switch(displayPopUp) {
                case POPUP_NUM_INPUT:
                    numInput.runTouch();
                    break;
                default:
                    break;
            }
        }
    }
}

void BUI::changeScreens(uint8_t screen) {
    if ((screen >= MAX_SCREENS) || (screen < 0))
        return;
    
    if (screens[currentScreen] != NULL)
        screens[currentScreen]->disposeScreen ();
    
    if (screens[screen] != NULL) {
        currentScreen = screen;
        screens[currentScreen]->initScreen ();
    }
}

void BUI::screenPopUp(uint8_t popup) {
    displayPopUp = popup;
}

void BUI::returnToScreen(void) {
    displayPopUp = POPUP_NONE;
    if (screens[currentScreen] != NULL)
        screens[currentScreen]->drawScreen ();
}

bool BUI::isCurrentScreen (uint8_t screen) {
    return ((screen == currentScreen) && (displayPopUp == POPUP_NONE));
}

/******************************************************************************/
/* General LCD Functions                                                      */
/******************************************************************************/
void BUI::clearAndPrintHeader (char* st) {
    myLCD.clrScr();
    setGeneralColors();
    myLCD.print(st, CENTER, 0);
}

void BUI::error(char* st) {
    myLCD.clrScr();
    setGeneralColors();
    myLCD.print("ERROR", 0, 0);
    myLCD.print(st, 0, 15);
    while(true)
        continue;
}

void BUI::print(char* st, uint16_t x, uint16_t y, uint8_t stLen) {
    uint8_t fontXsize, i;
    
    fontXsize = myLCD.getFontXsize();
    
    for(i=0 ; i<stLen ; ++i) {
        myLCD.printChar(*st, x + (i * fontXsize), y);
        ++st;
    } 
}

void BUI::printRight(char* st, uint16_t x, uint16_t y, uint8_t stLen) {
    uint16_t xOffset;
    
    if(stLen == 0) stLen = strlen(st);
    
    xOffset = stLen * myLCD.getFontXsize();
    
    print(st, x - xOffset, y, stLen);
}

void BUI::printCenter(char* st, uint16_t x, uint16_t y, uint8_t stLen) {
    uint16_t xOffset;
    
    if(stLen == 0) stLen = strlen(st);

    xOffset = (stLen * myLCD.getFontXsize()) / 2;
    
    print(st, x - xOffset, y, stLen);
}

void BUI::saveCurrent() {
    current.color = myLCD.getColor();
    current.backColor = myLCD.getBackColor();
    current.font = myLCD.getFont();
}

void BUI::restoreCurrent() {
    myLCD.setColor(current.color);
    myLCD.setBackColor(current.backColor);
    if(current.font != NULL)
        myLCD.setFont(current.font);
}

void BUI::setColor(myColors cc) {
    myLCD.setColor(cc.sRed, cc.sGreen, cc.sBlue);
}

void BUI::setBackColor(myColors cc) {
    myLCD.setBackColor(cc.sRed, cc.sGreen, cc.sBlue);
}

void BUI::setGeneralColors(void) {
    setBackColor(scrColors.bckgnd_c);
    setColor(scrColors.text_c);
    myLCD.setFont(SmallFont);
}

void BUI::setBrightness(uint8_t brightness) {
    backlightBrighness = brightness;
    analogWrite(LCD_BIGHTLIGHT, backlightBrighness);
}

uint8_t BUI::getBrightness(void) {
    return backlightBrighness;
}

void* BUI::getBrightnessAddess(void) {
    return (void*)&backlightBrighness;
}

/******************************************************************************/
/* PrivateFunctions                                                           */
/******************************************************************************/

BUI myBui;