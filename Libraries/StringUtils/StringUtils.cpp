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
 
/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <WProgram.h>
#include <inttypes.h>
#include <StringUtils.h>

//#define DEBUG

/******************************************************************************/
/* Public Methods                                                             */
/******************************************************************************/
uint8_t StringUtilities::ftos (float var, char* varSt, uint8_t decRes) {
    uint8_t i, j;
    uint32_t longVal, decVal, decShift;
    uint8_t stLen = 0;

#ifdef DEBUG
    Serial.println("starting convert");
#endif
    
    if (var<0) {
        varSt[stLen] = '-';
        var *= -1;
        ++stLen;
    }
    
    //decShift = (uint32_t)pow(10, decRes);
    decShift = decimalShift(decRes);
    
    longVal = (long)(var * decShift);
    decVal = longVal % decShift;
    longVal /= decShift;
    
#ifdef DEBUG
    Serial.println(decVal);
    Serial.println(longVal);
#endif
    
    if (longVal == 0) {
        varSt[stLen] = '0';
        ++stLen;
    } else {
        stLen = itos(longVal, varSt);
    }
    
#ifdef DEBUG   
    Serial.println("number converted");
#endif
    
    varSt[stLen] = '.';
    ++stLen;
    
    for (i=0 ; i<decRes ; ++i) {
        j = stLen + ((decRes - 1) - i);
        varSt[j] = (decVal % 10) + 0x30;
        decVal /= 10;
    }
    
#ifdef DEBUG 
    Serial.println("decimal convert");
#endif
    
    stLen += decRes;
    
    return stLen;
}

uint8_t StringUtilities::itos (int32_t var, char* varSt) {
    uint8_t i, j, stLen;
    char tBuf[10] = { 0 };
    stLen = 0;

    if (var<0) {
        varSt[stLen] = '-';
        var *= -1;
        ++stLen;
    }
    
    if (var == 0) {
        varSt[0] = '0';
        varSt[1] = '\0';
        return 1;
    }
    
    j = 10;
    while (var) {
        --j;
        tBuf[j] = (var % 10) + 0x30;  //get LSB digit and convert to ASCII
        var /= 10;
        
        if(j < 0)
            break;
    }
    
    for (i=j ; i<10 ; ++i) {
        varSt[stLen] = tBuf[i];
        ++stLen;
    }
    
    return stLen;
}

bool StringUtilities::stof (char* st, float* val) {
    uint8_t i      = 0;
    float   decVal = 0.0;
    int8_t  rslt   = 0;
    uint8_t decLen = 0;
    bool    neg    = false;

    *val = 0.0;
    
    if (st[i] == '-') {
        neg = true;
        ++i;
    }
    
    while (!isdigit(st[i]))
        ++i;

    while (st[i] != '.') {
        if (st[i] == '\0') {
           if (neg)
                *val *= -1;
                
           return true;
        }
        
        rslt = ctod(st[i]);
        if (rslt == -1)
            return false;
        else {
            *val *= 10;
            *val += rslt;
        }
        ++i;
    }

    ++i;

    while (st[i] != '\0') {
        ++decLen;
        decVal *= 10;
        rslt = ctod(st[i]);
        if (rslt == -1)
            return false;
        else
            decVal += rslt;
        ++i;
    }

    decVal /= decimalShift(decLen);

    *val += decVal;

    if (neg)
        *val *= -1;

    return true;
}

bool StringUtilities::stoi (char* st, int32_t* val) {
    uint8_t i    = 0;
    int8_t  rslt = 0;
    bool    neg  = false;

    *val = 0;
    
    if (st[i] == '-') {
        neg = true;
        ++i;
    } 
    
    while (!isdigit(st[i]))
        ++i;

    while (st[i] != '\0') {
        if (st[i] == '.')
            break;
        
        rslt = ctod(st[i]);
        if (rslt == -1)
            return false;
        else {
            *val *= 10;
            *val += rslt;
        }
        
        ++i;
    }

    if (neg)
        *val *= -1;

    return true;
}

bool StringUtilities::compare (char* st1, char* st2) {
    uint8_t i;

    uint8_t stLen1 = strlen(st1);
    uint8_t stLen2 = strlen(st2);

    if (stLen1 != stLen2)
        return false;

    for (i = 0; i < stLen1; ++i) {
        if (st1[i] != st2[i])
            return false;
    }

    return true;
}

void StringUtilities::copy (char* stFrom, char* stTo, uint8_t numChars) {
    uint8_t i;

    if (numChars == 0)
        numChars = strlen(stFrom);

    for (i = 0; i < numChars; ++i)
        stTo[i] = stFrom[i];

    stTo[numChars] = '\0';
}

void StringUtilities::append(char* begin, char* end) {
    uint8_t i, j = 0;

    uint8_t stLenB = strlen(begin);
    uint8_t stLenE = stLenB + strlen(end);

    for (i = stLenB; i < stLenE; ++i) {
        begin[i] = end[j];
        ++j;
    }

    begin[stLenE] = '\0';
}

/******************************************************************************/
/* Private Methods                                                            */
/******************************************************************************/
int8_t StringUtilities::ctod(char c) {
    int8_t result;
    
    result = c - 0x30;
    
    if ((result < 0) || (result > 9))
        result = -1;
        
    return result;
}

StringUtilities stringUtils;