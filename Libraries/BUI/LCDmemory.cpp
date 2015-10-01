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
#include <LCDmemory.h>

/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/
void* UImemory::allocConMem(size_t blockSize)
{
    int availMem;
    uint8_t* ptr;
    
    availMem = CONTROL_MEMORY_SIZE - nextCon;
    if(blockSize > availMem) return NULL;
    ptr = &cons[nextCon];
    nextCon += (int)blockSize;
    return (void*)ptr;
}



void* UImemory::getVarAddress(size_t blockSize)
{
    if(blockSize > VARIABLE_MEMORY_SIZE) return NULL;
    nextVar = (int)blockSize;
    return (void*)&vars[0];
}



void* UImemory::allocVarMem(size_t blockSize)
{
    int availMem;
    
    availMem = VARIABLE_MEMORY_SIZE - nextVar;
    if(blockSize > availMem) return NULL;
    return (void*)&vars[nextVar];
}



UImemory UImem;