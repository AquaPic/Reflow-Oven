/* Created by Skyler Brandt on January, 2014
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 ******************************************************************************/

#ifndef PID_H
#define	PID_H

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/
/*******Errors List************************************************************/
#define ERR_NOERROR     0 // no error has occured
#define ERR_UNKNOWN    -1 // unknown error
#define ERR_NOMEM      -2 // out of memory

/*******User Defined***********************************************************/
//#define INTEGRAL_SAMPLES    10
//#define INTEG_MAX           1000U
//#define INTEG_MIN           0U

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/
typedef struct pidvar {
    float gain;
    uint16_t sampleTime;
    uint16_t iTime;
    uint16_t dTime;
    int16_t minI;
    int16_t maxI;
}PIDvar;

struct pidobj {
    float error;
    float pTerm;
    float iTerm;
    float dTerm;
    float output;
    float e2;
    PIDvar vars;
};

typedef struct pidobj * PIDobj;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

PIDobj pid_new(void);
int8_t pid_init(PIDobj self, PIDvar varVar);
void pid_restart(PIDobj self);
float pid_run(PIDobj self, float PV, float SP);
void pid_calcI(PIDobj self);
void pid_calcD(PIDobj self);
float pid_getOutput(PIDobj self);

#endif	/* PID_H */

