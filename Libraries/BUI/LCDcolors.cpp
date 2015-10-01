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
#include <LCDcolors.h>

/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/
LCDcolors::LCDcolors(void)
{
    //Black
    bckgnd_c.sRed = 0;
    bckgnd_c.sGreen = 0;
    bckgnd_c.sBlue = 0;
    //White
    text_c.sRed = 255;
    text_c.sGreen = 255;
    text_c.sBlue = 255;
    
    //Red
    btn_c.sRed = 248;
    btn_c.sGreen = 0;
    btn_c.sBlue = 0;
    
    //Dark Grey
    barGBar_c.sRed = 95;
    barGBar_c.sGreen = 95;
    barGBar_c.sBlue = 95;
    //Green
    barGFill_c.sRed = 30;
    barGFill_c.sGreen = 252;
    barGFill_c.sBlue = 30;
    
    
    slider_c.sRed = 75;
    slider_c.sGreen = 75;
    slider_c.sBlue = 100;
    
    sliderOutline_c.sRed = 128;
    sliderOutline_c.sGreen = 128;
    sliderOutline_c.sBlue = 142;
    
    sliderLine_c.sRed = 200;
    sliderLine_c.sGreen = 200;
    sliderLine_c.sBlue = 225;
    
    //Light Grey
    lightGrey_c.sRed = 195;
    lightGrey_c.sGreen = 195;
    lightGrey_c.sBlue = 195;
    
    darkRed_c.sRed = 24;
    darkRed_c.sGreen = 24;
    darkRed_c.sBlue = 24;
}

LCDcolors scrColors;