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

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <WProgram.h>
#include <inttypes.h>

#include "drv_pid.h"

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
PIDobj pid_new (void) {
    PIDobj self = (PIDobj)calloc(1, sizeof(struct pidobj));
    return self;
}

int8_t pid_init (PIDobj self, PIDvar varsVar) {
    if(self == NULL) return ERR_NOMEM;

    self->vars = varsVar;
    pid_restart(self);

    return ERR_NOERROR;
}

void pid_restart (PIDobj self) {
    self->error = 0.0F;
    self->pTerm = 0.0F;
    self->iTerm = 0.0F;
    self->dTerm = 0.0F;
    self->output = 0.0F;
    self->e2 = 0.0F;
}

float pid_run(PIDobj self, float PV, float SP) {
    self->error = (SP - PV) * self->vars.gain;
    self->pTerm = self->error;
    
    self->output = self->pTerm + self->iTerm + self->dTerm;
        
    self->output = constrain (self->output, 0.0f, 1.0f);
    
    return self->output;
}

void pid_calcI (PIDobj self) {
    self->iTerm += self->error;
    self->iTerm = constrain (self->iTerm, (float)self->vars.minI, (float)self->vars.maxI);
}

void pid_calcD (PIDobj self) {
    self->dTerm = (self->error - self->e2);
    self->e2 = self->error;
}

float pid_getOutput(PIDobj self) { return self->output; }