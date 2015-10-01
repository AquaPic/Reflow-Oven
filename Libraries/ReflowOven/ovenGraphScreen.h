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
 
#ifndef BUI_OVEN_GRAPH_H
#define BUI_OVEN_GRAPH_H
 
/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <BUI.h>

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

/******************************************************************************/
/* Class Definition                                                           */
/******************************************************************************/
struct ovenGraphScreen : public iLcdScreen {
    ovenGraphScreen () : fullScaleTemp(0), fullScaleTime(0), x2(0), y2(0), infoShown(0) { };
    virtual void initScreen (void);
    virtual void drawScreen (void);
    virtual void disposeScreen (void);
    virtual void runTouch (void);
    
    void update (float temp, uint16_t time);
    void updateSetpoint (float setpoint);
    void updateStage (char* st);
    void updateButtons (void);
    void showInfo (char* message);
    void hideInfo (void);

private:
    void drawGraph (void);
    void disableHitBoxes (void);
    void enableHitBoxes (void);
    void drawThickLine (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

    float fullScaleTemp;
    uint16_t fullScaleTime;
    uint16_t x2;
    uint16_t y2;
    
    bool infoShown;
};

#endif /* BUI_OVEN_GRAPH_H */