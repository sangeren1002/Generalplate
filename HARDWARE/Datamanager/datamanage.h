#ifndef __DATAMANAGE_H__
#define __DATAMANAGE_H__

//#include "stm32f4xx_hal.h"

#include "tcp_server_demo.h"

#define MOTORCONTROL_ENABLE          1
#define TCPECHOSERVER_ENABLE         1
#define MOTORINQUIRE_ENABLE          1
#define FALLARREST_ENABLE            1
#define ULTRASONICMEASUREMENT_ENABLE 1
#define SLAMCBUS_ENABLE              1

#ifdef MOTORCONTROL_ENABLE
	#include "straightmotorcontrol.h"
#endif

//#ifdef TCPECHOSERVER_ENABLE
//	#include "tcpechoserver.h"
//#endif

#ifdef MOTORINQUIRE_ENABLE
	#include "straightmotorinquire.h"
#endif

//#ifdef FALLARREST_ENABLE
//	#include "fallarrest.h"
//#endif

#ifdef ULTRASONICMEASUREMENT_ENABLE
	//#include "ultrasonicmeasurement.h"
#endif

//#ifdef SLAMCBUS_ENABLE
//  #include "slamcbus.h"
//#endif



extern void DataManage(void);

#endif
