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
 
#ifndef LCD_MEMORY_H
#define LCD_MEMORY_H

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
#define CONTROL_MEMORY_SIZE     500
#define VARIABLE_MEMORY_SIZE    100

/*******EEPROM STUFF***********************************************************/

/******************************************************************************/
/* Variable Definition                                                        */
/******************************************************************************/

/******************************************************************************/
/* Class Definition                                                           */
/******************************************************************************/
class UImemory
{
    public:
        UImemory(void) : nextCon(0) {};
        void* allocConMem(size_t blockSize);
        void* getVarAddress(size_t blockSize);
        void* allocVarMem(size_t blockSize);
        void resetMemory(void) { nextCon = 0; nextVar = 0; };
        
    private:
        uint8_t cons[CONTROL_MEMORY_SIZE];
        uint8_t vars[VARIABLE_MEMORY_SIZE];
        
        int nextCon;
        int nextVar;
};

extern UImemory UImem;

#endif /* LCD_MEMORY_H */