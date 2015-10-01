/*******************************************************************************
 * Created by Skyler Brandt on May 2014
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
 
#ifndef LCD_NUMBER_INPUT_H
#define LCD_NUMBER_INPUT_H
 
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
#define NUMBER_CHARS        12

#define _NUM_TYPE_UINT8_T   1
#define _NUM_TYPE_INT8_T    2
#define _NUM_TYPE_UINT16_T  3
#define _NUM_TYPE_INT16_t   4
#define _NUM_TYPE_UINT32_t  5
#define _NUM_TYPE_INT32_t   6
#define _NUM_TYPE_FLOAT     7

#define ONE_BY_ONE          1
#define TWO_BY_ONE          2     

/*******EEPROM STUFF***********************************************************/

/******************************************************************************/
/* Variable Definition                                                        */
/******************************************************************************/
struct number_input_obj {
    uint8_t numberType;
    char value[NUMBER_CHARS];
    uint8_t numI;
    float min;
    float max;
    void* valToChange;
    uint8_t negative:1;
};

typedef struct number_input_obj* numberInputObj;
/******************************************************************************/
/* Class Definition                                                           */
/******************************************************************************/
class LCDnumInput {
    public:
        int8_t initNumberInput(char* title, uint8_t numberTypeVar, void* valToChangeVar, float minVar, float maxVar);
        void runTouch(void);
        
    private:
        numberInputObj vars;
        
        void addToChar(char charVar);
        void drawButton(char* st, uint16_t x, uint16_t y, bool doubleSize = false);
        void printTextBox(void);
        void drawTextBox(void);
        int32_t checkInt (int32_t num);
        float checkFloat (float num);
};

#endif /* LCD_NUMBER_INPUT_H */