#include "FSMDriver.h"

#include "StraightLine.h"
typedef StraightLine StateStraightLine;
#include "Curve.h"
typedef Curve StateCurve;
#include "OutOfTrack.h"
typedef OutOfTrack StateOutOfTrack;

//Define constants for transition method:
    const float FsmDriver::LEFT_EDGE        =-1.0;
    const float FsmDriver::RIGHT_EDGE       =1.0;
    const float FsmDriver::MAX_SPEED_DIST   =  70.0;
    const float FsmDriver::MAX_STR_ANGLE    =0.3;
    const float FsmDriver::MIN_STR_ANGLE    =-0.3;
    const int FsmDriver::STUCK_TICKS        =25;
    //Global variable to count the tics in stuck mode
    int stuck_Counter   =   0;
    int in_Stuck_Counter    =0;
//-------------------------------------------------------------------------------------------------------------------
//FsmDriver Class

FsmDriver::FsmDriver() {
    this->accel = 0;
    this->brake = 0;
    this->steer = 0;
    this->gear = 0;
}

CarState& FsmDriver::getCarState() {
    return this->_cs;
}

void FsmDriver::setCarState(CarState cs) {
    this->_cs = cs;
}

State* FsmDriver::getState() {
    return this->_state;
}

void FsmDriver::SetState(State *_newState) {
    delete this->_state;
    this->_state = _newState;
}

float FsmDriver::getAccel() {
    return this->accel;
}

void FsmDriver::setAccel(float accel) {
    this->accel = accel;
}

float FsmDriver::getBrake() {
    return this->brake;
}

void FsmDriver::setBrake(float brake) {
    this->brake = brake;
}

float FsmDriver::getSteer() {
    return this->steer;
}

void FsmDriver::setSteer(float steer) {
    this->steer = steer;
}

int FsmDriver::getGear() {
    return this->gear;
}

void FsmDriver::setGear(int gear) {
    this->gear = gear;
}

CarControl FsmDriver::wDrive(CarState cs) {
    this->setCarState(cs);
    this->SetState(transition(this->getCarState()));
    return (this->getState())->execute(this);
}

void FsmDriver::onRestart() {
    cout << "Restarting the race!" << endl;
}

void FsmDriver::onShutdown() {
    cout << "End of race!" << endl;
}

void FsmDriver::init(float *angles){
    for (int i = 0; i < 19; ++i)
        angles[i]=-90+i*10;
}

int iterate_Stuck(){
    if(abs(cs.getSpeedX()<15)){
        stuck_Counter++;
    }else{
        stuck_Counter = 0;
    }
}

State* FsmDriver::transition(CarState &cs) {
    State* p;
    if(stuck_Counter > STUCK_TICKS){
        p = new Stuck;
        cout << "Stuck" << endl;
        // @todo global counter to run stuck state for a defined time
        if(++in_Stuck_Counter == 50){
            in_Stuck_Counter = 0;
            stuck_Counter = 0;
        }
        return p;
    }else{
        stuck_Counter=iterate_Stuck();
    }
    if(cs.getTrackPos() > LEFT_EDGE && cs.getTrackPos() < RIGHT_EDGE) {
        // Getting track information from the sensor at +5 degrees towards the car axis
        float rSensor = cs.getTrack(10);
        // Getting track information from the sensor parallel to the car axis
        float cSensor = cs.getTrack(9);
        // Getting track information from the sensor at -5 degrees towards the car axis
        float lSensor = cs.getTrack(8);
        // Characteristics of a 'straight' to the FSM:
        // 		- If the central sensor is beyond the distance of maximum speed or if it
        // 		the biggest of {central, right (+5 degrees), left (-5 degrees)} sensors
        if (cSensor > MAX_SPEED_DIST || (cSensor >= rSensor && cSensor >= lSensor)) {
            p = new StraightLine;
            cout << "Straight" << endl;
            return p;
        }
        // Characteristics of a 'curve' to the FSM
        else {
            p = new Curve;
            cout << "Curve" << endl;
            return p;
        }
    }
    // Characteristics of 'outside of track' states
    else {
        // Returns
        cout << "OutOfTrack" << endl;
        p = new OutOfTrack;
        return p;
    }
}
