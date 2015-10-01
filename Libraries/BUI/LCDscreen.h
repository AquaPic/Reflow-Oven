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
 
#ifndef I_LCD_SCREEN_H
#define I_LCD_SCREEN_H
 
/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/
/*******Errors List************************************************************/
#define ERR_NOERROR     0   // no error has occurred
#define ERR_UNKNOWN    -1   // unknown error
#define ERR_NOMEM      -2   // out of memory

/*******User Defined***********************************************************/

/*******EEPROM STUFF***********************************************************/

/******************************************************************************/
/* Variable Definition                                                        */
/******************************************************************************/

/******************************************************************************/
/* Class Definition                                                           */
/******************************************************************************/
struct iLcdScreen {
    iLcdScreen(void) : drawn(0) {};
    virtual void initScreen (void) {};
    virtual void drawScreen (void) {};
    virtual void disposeScreen (void) {};
    virtual void runTouch (void) {};
    bool drawn;
};

#endif /* I_LCD_SCREEN_H */