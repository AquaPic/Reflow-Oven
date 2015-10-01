/*******************************************************************************
 * Created by Skyler Brandt on October 2014
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

#ifndef AP_STRING_UTILS_H
#define AP_STRING_UTILS_H

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/
/*******Errors List************************************************************/

/*******User Defined***********************************************************/
#define decimalShift(num)   ((uint32_t)pow(10, num));

/******************************************************************************/
/* Variable Definition                                                        */
/******************************************************************************/

/******************************************************************************/
/* Class Definition                                                           */
/******************************************************************************/
class StringUtilities {
public:
    StringUtilities(void) { };
        
    uint8_t ftos (float var, char* varSt, uint8_t decRes);
    uint8_t itos (int32_t var, char* varSt);
    bool    stof (char* varSt, float* val);
    bool    stoi (char* varSt, int32_t* val);
    bool    compare (char* st1, char* st2);
    void    copy (char* stFrom, char* stTo, uint8_t numChars = 0);
    void    append (char* begin, char* end);
   
private:
    int8_t ctod (char c);
};

extern StringUtilities stringUtils;

#endif /* AP_STRING_UTILS_H */
