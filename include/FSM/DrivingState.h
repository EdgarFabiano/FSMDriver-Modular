/**  @file: DrivingState.h
 *
 * https://github.com/bruno147/fsmdriver
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version. 
 */

#ifndef UNB_FSMDRIVER_DRIVING_STATE_H
#define UNB_FSMDRIVER_DRIVING_STATE_H

#include "CarControl.h"
#include "CarState.h"

class FSMDriver;

/** Abstract class defining a state for a Finite State Machine controller. */
class DrivingState {
public:
	/** Construtor.
     */
	DrivingState();

	/** Virtual destructor. */
    virtual ~DrivingState();

    /** Defines the driving policy of the state.
     *
     * @param cs the driver's perception of the environment. */
    virtual CarControl drive(CarState &cs);

    /** Called when entering the state. */
    virtual void enter();

    /** Called when exiting the state. */
    virtual void exit();


    int get_ticks_in_state();

    void add_ticks_in_state();


/**************************************************************************
 * Modularização*/

protected:
    /*Declarar os métodos e implementar  no DrivingState.cpp*/
    virtual float get_steer(CarState &cs) = 0;
    virtual int get_gear(CarState &cs) = 0;     
    virtual float get_accel(CarState &cs) = 0;
    virtual float get_brake(CarState &cs) = 0;
    virtual float get_clutch(CarState &cs) = 0;

    int ticks_in_state;

    
};

#endif // UNB_FSMDRIVER_DRIVING_STATE_H
