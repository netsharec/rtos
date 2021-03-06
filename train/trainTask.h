//
//--
//
//	Train Task class
//	
//	using FEMOS(Fast EMbedded OS) kernel main  
//
//
// 	Written by 
//
//	INSOP SONG
//
// 	Systems Design
//	
//	Michael Cole Computer Science
//
// 	University of Waterloo, Canada
//
//	June,July 2001
//--
//

#ifndef	_TRAIN_TASK_H_
#define _TRAIN_TASK_H_

extern "C" {
    #include "../include/i486/cs452.h"
    #include "../syscall/sysCall.h"
    // train application definitions
    #include "../train/ironBird.h"
    #include "../pathfind/graphtype.h"
}; // C function interface

#include "../include/util/errorCode.h"
#include "../include/util/type.h"

const BYTE MaxCommandSetSize 	= 100;

// switch command sets
const BYTE SwitchStraight 	= 0x21;
const BYTE SwitchCurve	 	= 0x22;

// you should send this command after you send the command
// after 150 ms 
const BYTE SwitchOffPower 	= 0x20;

// direction change command
const BYTE DirectionChange 	= 15;

// starting speed for acceleration
const BYTE SpeedStart	 	= 3;
// starting speed for deceleration
const BYTE SpeedStop	 	= 0;
// starting speed for crusing
const BYTE SpeedCruise	 	= 6;

const BYTE SpeedOperationFull 	= 7;
const BYTE SpeedRealFull 	= 14;
const BYTE SpeedFull	 	= 14;
const BYTE SpeedOperationLowest	= 4;
const BYTE SpeedLowest	 	= 1;
const char SpeedRange 		= SpeedFull+1;

const int  SpeedChangeInterval	= 10;

const BYTE SpeedChangeStep 	= 1;

const unsigned char EndSensorSignal 	= 0xf1;

// sensor data size using communication with sensor server
const BYTE SensorDataSize 	= 20;

// sensor type for safty checking
// these are place by its importance order
// !!! orders are important
enum SensorType { 
    Not_mine, 
    Behind_two, 
    Behind_one, 
    Ahead_two_tail,
    Ahead_one_tail, 
    Mine, 
    Ahead_two_head , 
    Ahead_one_head 
}; 
typedef SensorType	SensorType;

enum TrainSpeedType { Normal, Express };
typedef TrainSpeedType	SpeedType;

enum TrainType { Passenger, Cargo, Zamboni };
typedef TrainType	TrainType; 

enum TrainShowType { Leader, Wingman, Solo };
typedef TrainShowType	ShowType;

//
//--
//
// TrainTask Class
//
//--
//
class TrainTask {

    private:

	BYTE	trainNum;

	//BYTE	sensorData[SensorDataSize];
	unsigned char sensorData[SensorDataSize];

	BYTE	commandSize;
	BYTE	commandIndex;

	SpeedType	speedType; // express or normal 
	TrainType	trainType; // passenger or carge  
	ShowType	trainShowType; // leader or wingman

	Boolean started;
	Boolean missionStarted;
	Boolean finished;

	Boolean normalOperation;
	Boolean emergency;
	Boolean timeOut;
	Boolean deRailed;

	Boolean running;
	Boolean stopped;

	BYTE 	command[CMD_SIZE];

	// current position at corresponding segment 
	int	currentDistance;
	int	velocity;

	// current segment 
	int	currentSegment;

	// next segment 
	int	nextSegment;

	int	runningTime;
	int	segmentTime;

	BYTE 	speed;

	// for calculating Accerlation , namely Vo
	BYTE 	speed_old;
	//
	// basic physics...
	//
	//	d(t) = (1/2) * ( a * t^2) + Vo * t
	//	d(t) = 0.5 * delta_t * delta_v + Vo * t
	//

	// acceralation or deceralation flag
	Boolean	breakPadelFlag;

	BYTE	inputSensorNum;

	// last triggered sensor
	BYTE 	lastSensor;

	// one step command set struct
	oneHop	stepsToFollow[NUMSEGMENTS];

	// index for command set
	BYTE	indexCommandSet;
	BYTE	numCommandSet;
	int 	speedTable[15];

	// for change speed check, this is uptime
	int 	lastSpeedChangeTime;


    public:

	// command set
	CommandSet	aCommandSet[MaxCommandSetSize];

	TrainTask(void);
	TrainTask(int _trainNo);

// train number
	       void setTrainNumber(BYTE _trainNo);
	inline BYTE getTrainNumber( void );

// switch command sets
	inline void	setSwitch( BYTE swNum, BYTE swType );
	inline void	setSwitchCurve( BYTE swNum );
	inline void	setSwitchStraight( BYTE swNum );

// train speed control function sets
	inline void 	setSpeed( BYTE _speed );
	inline BYTE 	getSpeed( void );

// read speedTable for postion estimation calculation
	inline int 	getVelocity( void );
	inline int 	getVelocity( BYTE _speed );

// command for the train 
	inline void	startTrain( void );
	inline void 	stopTrain( void );
	inline void 	cruiseTrain( void );

// change direction 
	inline void 	changeDirection( void );

// check the command set about the direction 
	inline Boolean  isChangeDirection( void );
	inline BYTE	getDirection( void );
	inline BYTE	getDirection( BYTE _index );

	inline void	setChangeSpeedTime(int _lastSpeedTime);
	inline Boolean  canChangeSpeed(int _currentTime);

	// dev
	inline Boolean	canIncreaseSpeed( void );
	inline Boolean	needSlowDown( void );
	inline Boolean	increaseSpeed( void );
	inline Boolean	decreaseSpeed( void );
// go to start pos and return 
// corresponding sensor data
		BYTE 	goStartPosition( void );
		Boolean isStartPosition( BYTE _sensor );
		Boolean isCounterStartPosition( BYTE _sensor );

// task related function sets
	inline Boolean  isFinished( void );
	inline Boolean  isStarted( void );
	inline Boolean  isMissionStarted( void );
	inline Boolean  isNormalOperation( void );
	inline Boolean  isEmergency( void );
	inline Boolean  isTimeOut( void );
	inline Boolean	isDeRailed( void );
	inline Boolean	isRunning( void );
	inline Boolean	isStopped( void );
	inline Boolean	isBreakPadel( void );

	// check current and return the boolean 
	inline Boolean 	isDistanceRange();
	inline Boolean 	isDistanceRange2();
	inline Boolean 	amIlost() ;
	// check inputted sensor and returns the result
	inline SensorType isMySensor( BYTE * sensorData);

	inline Boolean 	isHeadWith(BYTE sensor, BYTE sensorInputted);
	inline Boolean 	isTailWith(BYTE sensor, BYTE sensorInputted);
	
	inline void  setFinished( void );
	inline void  setStarted( void );
	inline void  setMissionStart( void );
	inline void  setNormalOperation( void );
	inline void  setEmergency( void );
	inline void  setTimeOut( void );
	inline void  setDeRailed( void );
	inline void  setBreakPadel( void );
	
	inline void  resetFinished( void );
	inline void  resetStarted( void );
	inline void  resetMissionStart( void );
	inline void  resetNormalOperation( void );
	inline void  resetEmergency( void );
	inline void  resetTimeOut( void );
	inline void  resetDeRailed( void );

	inline void  setRunning( void );
	inline void  setStopped( void );
// command set functions
		 int 	setCommandSet( oneHop * _steps);
		void	printCommandSet( void );
		//void	printCommandSet( oneHop * _oneHop ); 
		// print by index
		void	printCommandSet( BYTE _indexCommand );
// -- 
	inline Boolean	setCommandSwitch( BYTE _indexCommand );
	inline Boolean	setCommandSwitch( void );
	inline Boolean	setNextCommandSwitch( void );
	inline Boolean	isFirstCommandSet( void );
	inline Boolean	hasMoreCommandSet( void );
	inline oneHop *	getNextCommandSet( void );
	inline Boolean 	setNextCommandSet( void );


// sensor function sets
	//inline oneHop *	getNextSensor( void );
	inline BYTE 	getNextSensor( void );
	inline BYTE 	get2ndNextSensor( void );
	inline BYTE 	getBeforeSensor( void );
	inline BYTE 	get2ndBeforeSensor( void );
	inline BYTE 	getFirstSensor( void );

	inline Boolean	hasSwitchCommand( oneHop * _oneHop);
	inline Boolean	hasSwitchCommand( BYTE _indexCommand );
	inline Boolean	hasSwitchCommand( void );
	inline int	hasNextSwitchCommand( void );

	inline Boolean	isHeadToHead( BYTE _sensor );
	inline Boolean  isHeadToTail( BYTE _sensor );

	inline void 	getSensorSet( void );

	inline void 	getCommandSet( void );

	inline Boolean  isTrackLocked( void );
	inline Boolean  requestTrackLock( void );
	inline Boolean  requestTrackUnLock( void );

// distance (position) related fuctions
	inline Boolean	isRightPosition( void );
	inline int	getCurrentDistance( void );
	inline int	updateDistance( void );
	inline int	updateDistance( int delta_t );
	inline void	resetDistance( void );
	inline int	distanceToGoCurrent( void );
	inline int	distanceToGoNext( void );
	inline int	distanceToGoBefore( void );

// time related functions
	inline void 	setSegmentTime( int _segTime );
	inline int 	getSegmentTime( void );

// mission related functions
	inline void 	sendReport( void );
	inline void	reportPosition( void );
	inline void	reportTime( void );
	inline void	reportStatus( void );

// XXX Ememgency manuver is needed
//	such as if collision or lost or invalid data
///
// information print of itself
		void	printTrain( void );

};



inline BYTE TrainTask::getTrainNumber( void ) {
    return trainNum;
}

//
//  set switch
//
// 
// XXX sensor check routine need
inline void TrainTask::setSwitch( BYTE swNum, BYTE swType ) {

    if( swNum != -1 && swNum !=0 && 
	((swType == STRAIGHT) || (swType == CURVE))) {
	command[ CMD_TYPE ] = SWITCH_CMD; 
	command[ SWITCH_NUMBER ] = swNum; 
	command[ SWITCH_TYPE ] = swType; 

	// send command to sensor&command task to serial data output
	Send(SensorTid, &command, CMD_SIZE * sizeof(char), NULL, NULL);
    }
    else {
	printf("setSwitch Line %d Invalid switch number %d sw type%d\n",
	    __LINE__, swNum, swType);
    }
}

//
//  set switch CURVE
//
inline void TrainTask::setSwitchCurve( BYTE swNum ) {

    if( swNum != -1 && swNum !=0 ) {
	command[ CMD_TYPE ] = SWITCH_CMD; 
	command[ SWITCH_NUMBER ] = swNum; 
	command[ SWITCH_TYPE ] = SwitchCurve; 

	// send command to sensor&command task to serial data output
	Send(SensorTid, &command, CMD_SIZE * sizeof(char), NULL, NULL);
    }
    else {
	printf("setSwitch Line %d Invalid switch number %d\n",__LINE__, swNum);
    }

}

//
//  set switch STRAIGHT
//
inline void TrainTask::setSwitchStraight( BYTE swNum ) {

    if( swNum != -1 && swNum !=0 ) {
	command[ CMD_TYPE ] = SWITCH_CMD; 
	command[ SWITCH_NUMBER ] = swNum; 
	command[ SWITCH_TYPE ] = SwitchStraight; 

	// send command to sensor&command task to serial data output
	Send(SensorTid, &command, CMD_SIZE * sizeof(char), NULL, NULL);
    }
    else {
	printf("setSwitch Line %d Invalid switch number %d\n",__LINE__, swNum);
    }

}

//
// get train speed
//
inline BYTE TrainTask::getSpeed( void ) {

    return speed;

}

//
// get train velocity for position estimation caculation
//
inline int TrainTask::getVelocity( void ) {

    return speedTable[speed];
}

//
// get train velocity for position estimation caculation
//
inline int TrainTask::getVelocity( BYTE _speed ) {

    return speedTable[_speed];

}

//
// set train speed
//
inline void TrainTask::setSpeed( BYTE _speed ){

    // set current speed as speed_old
    speed_old = speed;

    command[ CMD_TYPE ] = TRAIN_CMD; 

    // set speed variable as given speed(_speed)
    command[ TRAIN_SPEED ] = speed = _speed; 

    // my own number
    command[ TRAIN_NUMBER ] = trainNum; 

    // send command to sensor&command task to serial data output
    Send(SensorTid, &command, CMD_SIZE * sizeof(char), NULL, NULL);

}

//
// start train
//
inline void TrainTask::startTrain( void ){

    // set speed_old zero
    speed_old = 0;

    // no break padel
    breakPadelFlag = False;

    command[ CMD_TYPE ] = TRAIN_CMD; 

    command[ TRAIN_SPEED ] = speed = SpeedStart; 

    // my own number
    command[ TRAIN_NUMBER ] = trainNum; 

    // send command to sensor&command task to serial data output
    Send(SensorTid, &command, CMD_SIZE * sizeof(char), NULL, NULL);

    setRunning();
    //printf("startTrain() >>>\n");
}

//
// stop train
//
inline void TrainTask::stopTrain( void ){

    command[ CMD_TYPE ] = TRAIN_CMD; 

    command[ TRAIN_SPEED ] = speed = speed_old = SpeedStop; 

    // my own number
    command[ TRAIN_NUMBER ] = trainNum; 

    // send command to sensor&command task to serial data output
    Send(SensorTid, &command, CMD_SIZE * sizeof(char), NULL, NULL);

    setStopped();
    //printf("stopTrain() >>>\n");

}

//
//  make train cruise
//
inline void TrainTask::cruiseTrain( void ){

    command[ CMD_TYPE ] = TRAIN_CMD; 
    command[ TRAIN_SPEED ] = speed = SpeedCruise; 
    // my own number
    command[ TRAIN_NUMBER ] = trainNum; 

    // send command to sensor&command task to serial data output
    Send(SensorTid, &command, CMD_SIZE * sizeof(char), NULL, NULL);
}

//
//  change direction
//
inline void TrainTask::changeDirection( void ){

    command[ CMD_TYPE ] = TRAIN_CMD; 

    // before call this function, speed should be zero
    speed = 0;
    command[ TRAIN_SPEED ] = DirectionChange; 
    // my own number
    command[ TRAIN_NUMBER ] = trainNum; 

    // send command to sensor&command task to serial data output
    Send(SensorTid, &command, CMD_SIZE * sizeof(char), NULL, NULL);

    //printf("Train change direction \n");

}


//
// get direction
//
inline BYTE   TrainTask::getDirection( void ) {

    return stepsToFollow[ indexCommandSet ].direction;
}

//
// get direction
//
inline BYTE   TrainTask::getDirection( BYTE _index) {

// if valid boundary 
    if( _index < numCommandSet ) 
	return stepsToFollow[ _index ].direction;
    else 
	// error message
	return 0;
}


inline Boolean 	TrainTask::isDistanceRange() {
    register int distanceToGo = stepsToFollow[indexCommandSet].distance;

    return 
    ((currentDistance >  distanceToGo*0.8) ||
      (distanceToGo < 4000) ) ? True : False;

}

// this is for slow down function
#define SOME_MORE_DISTANCE	1500

inline Boolean 	TrainTask::isDistanceRange2() {
    register int distanceToGo = stepsToFollow[indexCommandSet].distance;


    return 
	((currentDistance+SOME_MORE_DISTANCE >  distanceToGo*0.2) ||
	  (distanceToGo < 4000)) ? True : False;

}

inline Boolean 	TrainTask::amIlost() {
    register int distanceToGo = stepsToFollow[indexCommandSet].distance;


    return (currentDistance >  (distanceToGo*4)) ? True : False;

}

inline SensorType TrainTask::isMySensor( BYTE * sensorData) {
#if 0
    register int index = 0;
    register SensorType retValue = Not_mine;

    register BYTE mine = getNextSensor();
    register BYTE mine_head = 
	(((mySensor %2) == 0) ? mySensor-1 : mySensor+1);

    register BYTE sensorAhead = get2ndNextSensor();

    // if even, - one, if odd, +1
    register BYTE sensoAhead_head = 
	(((sensorAhead %2) == 0) ? sensorAhead-1 : sensorAhead+1);

    register BYTE sensorBehind_2 = get2ndBeforeSensor();
    register BYTE sensorBehind   = getBeforeSensor();

    while(sensorData[index] != 0) {

	// if the sensor is my next anticipating sensor
	if( sensorData[index] == mySensor()) {

	    // if it is in my distance
	    if( isDistanceRange()) {

		// if there are not other emergent sensor 
		if( retValue < Mine) {

		    console_xy(0,20);
		    cprintf("MINE");

		    retValue = Mine;
		} // if

	    } // if my distance
	} // if my 2nd  next sensor

	// if two ara getting one sensor head 
	if( sensorData[index] == getNextSensor()) {

	    // if it is in my distance
	    if( isDistanceRange()) {

		// if there are not other emergent sensor 
		if( retValue < Mine) {

		    console_xy(0,20);
		    cprintf("MINE");

		    retValue = Mine;
		} // if

	    } // if my distance
	} // if my 2nd  next sensor

	else if( sensorData[index] == sensorAhead_head) {
	    // check head or tail

		// if there are not other emergent sensor 
		if( retValue < Ahead_one_head) {
		    console_xy(0,20);
		    cprintf("two ahead HEAD");

		    retValue = Mine;
		} // if

	} // else if

	else if( sensorData[index] == sensorAhead) {
	    // check head or tail

		// if there are not other emergent sensor 
		if( retValue < Ahead_one_tail) {
		    console_xy(0,20);
		    cprintf("One ahead TAIL");

		    retValue = Mine;
		} // if

	} // else if
	// sensor section before
	else if( sensorData[index] == sensorBehind) {
	    // check head or tail

		// if there are not other emergent sensor 
		if( retValue < Mine) {

		    retValue = Mine;
		} // if

	} // else if

	// sensor 2 section before
	else if( sensorData[index] == sensorBehind_2) {
	    // check head or tail

		// if there are not other emergent sensor 
		if( retValue < Mine) {

		    retValue = Mine;
		} // if

	} // else if

	index++;

    } // while

    return retValue;
#endif // 0
}


// check two sensors are different direction or not
inline Boolean 	TrainTask::isHeadWith(BYTE sensor, BYTE sensorInputted) {

    return True;

}

// check two sensors are different direction or not
inline Boolean 	TrainTask::isTailWith(BYTE sensor, BYTE sensorInputted) {

    return True;

}

//
//  check change direction is needed or not
//
inline Boolean TrainTask::isChangeDirection( void ){

    return ((stepsToFollow[indexCommandSet].direction == -1) 
	    ? True : False);

}

inline Boolean	TrainTask::isBreakPadel( void ) {
    return breakPadelFlag;
}

inline void	TrainTask::setBreakPadel( void ) {
    breakPadelFlag = True;
}

inline Boolean	TrainTask::canIncreaseSpeed( void ) {
    return True;
}

inline Boolean	TrainTask::needSlowDown( void ) {

// very slow mode
#undef SAFE_RUN
#ifdef SAFE_RUN
    if((indexCommandSet+2) >= numCommandSet  ||
	((stepsToFollow[indexCommandSet+1].direction) == -1) 
#else 

	// if the end position
    if( (indexCommandSet+1 >= numCommandSet) ||
	// if need to change direction
	((stepsToFollow[indexCommandSet+1].direction) == -1) 
	// and my dis
	&& isDistanceRange2()

#endif // SAFE_RUN
	//((stepsToFollow[indexCommandSet+1].direction) == -1) 
	//|| ((stepsToFollow[indexCommandSet+2].direction) == -1)) 
	){
	return True;
    }
    else {
	return False;
    }
}


// 
//  increase speed 
//
inline Boolean TrainTask::increaseSpeed( void ){

    // if the speed is below full speed
    if( getSpeed() < ( SpeedOperationFull ) ) {

	// save current speed at speed_old
	speed_old = speed;

	// increase speed by step size
	speed += SpeedChangeStep;

	command[ CMD_TYPE ] = TRAIN_CMD; 

	command[ TRAIN_SPEED ] = speed; 

	// my own number
	command[ TRAIN_NUMBER ] = trainNum; 

	// send command to sensor&command task to serial data output
	Send(SensorTid, &command, CMD_SIZE * sizeof(char), NULL, NULL);
	//printf("increaseSpeed() speed:%d >>>\n", speed);
	return True;
    } // if
    else {
	return False;
    } //else 
    
}

//
//  decrease speed
//
inline Boolean TrainTask::decreaseSpeed( void ){

    // if the speed is higher then minimum 
    if( getSpeed() > ( SpeedOperationLowest ) ) {

	// save current speed at speed_old
	speed_old = speed;

	// decrease speed by step
	speed -= SpeedChangeStep;

	command[ CMD_TYPE ] = TRAIN_CMD; 

	command[ TRAIN_SPEED ] = speed; 
	// my own number
	command[ TRAIN_NUMBER ] = trainNum; 

	// send command to sensor&command task to serial data output
	Send(SensorTid, &command, CMD_SIZE * sizeof(char), NULL, NULL);

	//printf("decreaseSpeed() speed:%d >>>\n", speed);

	return True;

    } // if 

    else {

	return False;

    } // else

// because speed 1 is not good, it is going to be stucked 
#undef NO_SPEED_1
#ifdef NO_SPEED_1

    else if( getSpeed() > SpeedLowest ) {
	// decrease speed by one
	speed--;
	command[ CMD_TYPE ] = TRAIN_CMD; 
	command[ TRAIN_SPEED ] = speed; 
	// my own number
	command[ TRAIN_NUMBER ] = trainNum; 

	// send command to sensor&command task to serial data output
	Send(SensorTid, &command, CMD_SIZE * sizeof(char), NULL, NULL);
    } // else if

#endif // NO_SPEED_1
	

}




inline Boolean  TrainTask::isFinished( void ) 
{
    return finished;
}
inline Boolean  TrainTask::isStarted( void ) 
{
    return started;
}
inline Boolean  TrainTask::isMissionStarted( void ) 
{
    return missionStarted;
}


inline void  TrainTask::setFinished( void ) 
{
    finished = True;
}
inline void  TrainTask::setStarted( void ) 
{
    started = True;
}
inline void  TrainTask::setMissionStart( void ) 
{
    missionStarted = True;
}


inline void  TrainTask::resetFinished( void ) 
{
    finished = False;
}

inline void  TrainTask::resetStarted( void ) 
{
    started = False;
}

inline void  TrainTask::resetMissionStart( void ) {
    missionStarted = False;
}

inline Boolean  TrainTask::isNormalOperation( void ) 
{
    return normalOperation;
}
inline Boolean  TrainTask::isEmergency( void ) 
{
    return emergency;
}
inline Boolean  TrainTask::isTimeOut( void ) 
{
    return timeOut;
}
inline Boolean  TrainTask::isDeRailed( void ) 
{
    return deRailed;
}


inline void  TrainTask::setNormalOperation( void ) {
    normalOperation = True;
}
inline void  TrainTask::setEmergency( void ) {
    emergency = True;
}
inline void  TrainTask::setTimeOut( void ) {
    timeOut = True;
}
inline void  TrainTask::setDeRailed( void ) {
    deRailed = True;
}


inline void  TrainTask::resetNormalOperation( void ) {
    normalOperation = False;
}
inline void  TrainTask::resetEmergency( void ) {
    emergency = False;
}
inline void  TrainTask::resetTimeOut( void ) {
    timeOut = False;
}
inline void  TrainTask::resetDeRailed( void ) {
    deRailed = False;
}

inline Boolean  TrainTask::isRunning( void ) {
    return running;
}
inline Boolean  TrainTask::isStopped( void ) {
    return stopped;
}


inline void  TrainTask::setRunning( void ) {
    running = True;
    stopped = False;
}
inline void  TrainTask::setStopped( void ) {
    running = False;
    stopped = True;
}

inline Boolean  TrainTask::isFirstCommandSet( void ) {
    return( indexCommandSet == 0 ? True : False);
}

inline Boolean	TrainTask::hasMoreCommandSet( void ) {

    // if the distance is not NIL then Yes it has more 
    return( indexCommandSet < numCommandSet ? True : False);
    //return( stepsToFollow[indexCommandSet].distance != NIL 
	//? True : False);

}

// 
// returns next command set as in pointer form
//
// should check the avaiability of the next set
// before call this function
//
inline oneHop *	TrainTask::getNextCommandSet( void ) {

// XXX XXX XXX XXX HERE XXX ? about index ++ or not
    return &stepsToFollow[indexCommandSet++];

}


inline Boolean 	TrainTask::setNextCommandSet( void ) {
    if(indexCommandSet+1 < numCommandSet) {
	indexCommandSet++;
	return True;
    }
    else 
	return False;
}

//
// return next sensor data
//
inline BYTE TrainTask::getNextSensor( void ) 
{

    register BYTE module;
    register BYTE number;

    if( indexCommandSet + 1 < numCommandSet ) {
	module = stepsToFollow[indexCommandSet+1].thisNode.module;
	number = stepsToFollow[indexCommandSet+1].thisNode.sensorNum;
	return( (module<<4) | (number-1) );
    }
    else {
	return EndSensorSignal;
    }

}

//
// return very first sensor data
//
inline BYTE TrainTask::getFirstSensor( void ) 
{

    register BYTE module;
    register BYTE number;

    module = stepsToFollow[0].thisNode.module;
    number = stepsToFollow[0].thisNode.sensorNum;
    return( (module<<4) | (number-1) );

}

#undef GET_NEXT_ONE_HOP_RETURN
#ifdef GET_NEXT_ONE_HOP_RETURN
inline oneHop * TrainTask::getNextSensor( void ) 
{

    if( indexCommandSet + 1 < numCommandSet ) {
	return &stepsToFollow[indexCommandSet + 1];
    }
    else {
	return (oneHop *)NIL;
    }

}
#endif // GET_NEXT_ONE_HOP_RETURN


inline BYTE TrainTask::get2ndNextSensor( void ) 
{

    register BYTE module;
    register BYTE number;

    if( indexCommandSet + 2 < numCommandSet ) {
	module = stepsToFollow[indexCommandSet+2].thisNode.module;
	number = stepsToFollow[indexCommandSet+2].thisNode.sensorNum;
	return( (module<<4) | (number-1) );
    }
    else {
	return EndSensorSignal;
    }

}

inline BYTE TrainTask::getBeforeSensor( void ) 
{

    register BYTE module;
    register BYTE number;

    if( indexCommandSet - 1 >= 0 ) {
	module = stepsToFollow[indexCommandSet-1].thisNode.module;
	number = stepsToFollow[indexCommandSet-1].thisNode.sensorNum;
	return( (module<<4) | (number-1) );
    }
    else {
	return 0; 
    }

}

inline BYTE TrainTask::get2ndBeforeSensor( void ) 
{

    register BYTE module;
    register BYTE number;

    if( indexCommandSet - 2 >= 0 ) {
	module = stepsToFollow[indexCommandSet-2].thisNode.module;
	number = stepsToFollow[indexCommandSet-2].thisNode.sensorNum;
	return( (module<<4) | (number-1) );
    }
    else {
	return 0; 
    } 
}

inline Boolean	TrainTask::hasSwitchCommand( oneHop * _oneHop ) 
{
    return ((_oneHop->instlist[0].sequence !=-1)
	    ? True : False);
}

inline Boolean	TrainTask::hasSwitchCommand( BYTE _indexCommand ) 
{
    return ((stepsToFollow[_indexCommand].instlist[0].sequence !=-1)
	    ? True : False);
}

inline Boolean	TrainTask::hasSwitchCommand( void ) 
{
    return ((stepsToFollow[indexCommandSet].instlist[0].sequence !=-1)
	    ? True : False);
}

//
// check next switch command avaiable
//
inline int	TrainTask::hasNextSwitchCommand( void ) 
{
    register int innerIndex = 0;
    if( indexCommandSet+1 < numCommandSet ) { 
	while(
	  stepsToFollow[indexCommandSet+1].instlist[innerIndex].sequence!= -1) {
	    innerIndex++;
	} // inner while
	return innerIndex;
    }
    else 
	return 0;
}

// 
// set corresponding command set
//
inline Boolean	TrainTask::setCommandSwitch( BYTE _indexCommand ) {
    register char innerIndex = 0;
    register int sequence;
    register int setting;
    

    while(((stepsToFollow[_indexCommand].instlist[innerIndex].sequence) != NIL) 
	&& (innerIndex < NUMSWITCH) ) 
    {
	sequence = stepsToFollow[_indexCommand].instlist[innerIndex].sequence;
	setting = stepsToFollow[_indexCommand].instlist[innerIndex].setting;

	if( setting == straight) {
	    setSwitchStraight(sequence);
	}
	else {
	    setSwitchCurve(sequence);
	}

	//Delay(6);

	innerIndex++;
    } // inner while

}

// 
// set corresponding command set
//
inline Boolean	TrainTask::setCommandSwitch( void ) {
    register char innerIndex = 0;
    register int sequence;
    register int setting;
    

    while(
	((stepsToFollow[indexCommandSet].instlist[innerIndex].sequence) != NIL) 
	&& (innerIndex < NUMSWITCH) ) 
    {
	sequence = stepsToFollow[indexCommandSet].instlist[innerIndex].sequence;
	setting = stepsToFollow[indexCommandSet].instlist[innerIndex].setting;

	if( setting == straight) {
	    setSwitchStraight(sequence);
	}
	else {
	    setSwitchCurve(sequence);
	}

	//Delay(6);

	innerIndex++;
    } // inner while

}

// 
// set next corresponding command set
//
inline Boolean	TrainTask::setNextCommandSwitch( void ) {
    register char innerIndex = 0;
    register int sequence;
    register int setting;
    

    while(
	((stepsToFollow[indexCommandSet+1].instlist[innerIndex].sequence) != NIL) 
	&& (innerIndex < NUMSWITCH) ) 
    {
	sequence = stepsToFollow[indexCommandSet+1].instlist[innerIndex].sequence;
	setting = stepsToFollow[indexCommandSet+1].instlist[innerIndex].setting;

	if( setting == straight) {
	    setSwitchStraight(sequence);
	}
	else {
	    setSwitchCurve(sequence);
	}

	//Delay(6);

	innerIndex++;
    } // inner while

}
//
// position related fuctions
// dev
//
inline Boolean	TrainTask::isRightPosition( void ) 
{
    return True;
}

inline int	TrainTask::getCurrentDistance( void ) 
{
    return currentDistance;
}

//
//  distance to go 
//
inline int TrainTask::distanceToGoCurrent( void ) 
{
    return stepsToFollow[indexCommandSet].distance;
}

//
//  distance to go 
//
inline int TrainTask::distanceToGoNext( void ) {
    if(indexCommandSet+1 < numCommandSet) {
	 return stepsToFollow[indexCommandSet+1].distance;
    }
}

//
//  distance to go 
//
inline int TrainTask::distanceToGoBefore( void ) {

    if(indexCommandSet-1 >= 0) {

	 return stepsToFollow[indexCommandSet-1].distance;
    }
}

//
// update estimated distance with velocity(from speed lookup table)
//
inline int TrainTask::updateDistance( void ) {

    // XXX this is really naive approach, 
    //currentDistance += (TRAIN_TASK_TICKS * getVelocity());

    return currentDistance;

}

// 
// calculate distance based on basic physics law
//
// simple physics...
//
//	d(t) = (1/2) * ( a * t^2) + Vo * t
//	d(t) = 0.5 * delta_t * delta_v + Vo * t
//
inline int TrainTask::updateDistance( int delta_t ) {


    // XXX this is really naive approach, 
    //currentDistance += (_timeDiff * getVelocity());
    register int speed_old_local = getVelocity(speed_old);
    register int delta_v = getVelocity(speed) - (speed_old_local);
    
    // else, if we have acceraltion term 
    if( delta_v != 0 ) {

	// left shift one means divide 2
	currentDistance += (delta_v * delta_t) / 2;

    } // if

    currentDistance += speed_old_local * delta_t;

    return currentDistance;

}

//
// reset distance 
//
inline void	TrainTask::resetDistance( void ) {

    currentDistance = 0;

}

// 
// set last speed change time
//
inline void	TrainTask::setChangeSpeedTime(int _lastSpeedTime) {

    lastSpeedChangeTime = _lastSpeedTime;

}

// 
// change speed with certain( SpeedChangeInterval )
inline Boolean  TrainTask::canChangeSpeed(int _currentTime) {

    if( (_currentTime - lastSpeedChangeTime) > SpeedChangeInterval ) {

	// reset the variable to new one
	lastSpeedChangeTime = _currentTime;
	return True;
    }

    else {

	return False;
    }

    //return( ( (_currentTime - lastSpeedChangeTime) > SpeedChangeInterval)
	    //? True : False);
}
#endif // _TRAIN_TASK_H_
