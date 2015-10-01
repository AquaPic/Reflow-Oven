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
 
#ifndef LCD_COLORS_H
#define LCD_COLORS_H
 
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
typedef struct s_color
{
    byte sRed;
    byte sGreen;
    byte sBlue;
} myColors;

/******************************************************************************/
/* Class Definition                                                           */
/******************************************************************************/
class LCDcolors
{
    public:
        LCDcolors(void);
        
        //General
        myColors bckgnd_c;
        myColors text_c;
        
        //Buttons
        myColors btn_c;
        
        //Bar Graphs
        myColors barGBar_c;
        myColors barGFill_c;
        
        //Slider
        myColors slider_c;
        myColors sliderOutline_c;
        myColors sliderLine_c;
        
        myColors lightGrey_c;
        myColors darkRed_c;
};

extern LCDcolors scrColors;

#endif /* LCD_COLORS_H */