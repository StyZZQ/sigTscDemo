#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include "gb2017.h"
#include "value.h"
#include "exchange.h"
#include "rapidjson/meta.h"
#include "rapidjson/serialize.hpp"
#include "rapidjson/deserialize.hpp"
using namespace rapidjson;
using namespace std;
// #define WRONG_LIGHT_TYPE   			//错误灯组类型检查，
// #define WRONG_LIGHT_CTRL				//错误灯组控制检查
// #define WRONG_SCHEDULE_ROADID		//错误调度路口id	
// #define WRONG_PLAN_ROADID			//错误日计划路口id	
// #define WRONG_SCHEME_ROADID			//错误方案路口id	

// #define WRONG_SCHEDULE_WEEK			//错误调度星期
// #define WRONG_SCHEDULE_MONTH			//错误调度月份
// #define WRONG_SCHEDULE_DAY			//错误调度日期
// #define WRONG_SCHEDULE_PLANID		//错误调度日计划id

// #define WRONG_PLAN_SCHEMEID			//错误日计划的方案id
// #define WRONG_PLAN_STARTTIME0		//错误时段1的时间设置
// #define WRONG_PLAN_STARTTIME1		//错误计划时段2时间设置
// #define WRONG_PLAN_MODE				//错误计划模式

// #define WRONG_SCHEME_PHASESTAGETYPE	//错误方案的相位阶段类型
// #define WRONG_SCHEME_PHASESTAGE		//错误方案的相位阶段id
// #define WRONG_SCHEME_COORD			//错误方案的协调序号
// #define WRONG_SCHEME_CYCLE			//错误方案的周期

// #define WRONG_PHASESTAGE_PHASEID		//错误相位阶段的相位id
// #define WRONG_PHASESTAGE_LISTTIME	//错误相位阶段的时间
// #define WRONG_PHASESTAGE_CTRL		//错误相位阶段的控制

// #define WRONG_PHASE_LIGHTS			//错误相位的灯组
// #define WRONG_PHASE_GREEN			//错误相位的最大，最小绿
// #define WRONG_PHASE_CTRL				//错误相位的控制



void initLight(All& a)
{
	//配置表
	a.light.info.num = 4;
	a.light.info.conf[0].id = 1;
#ifdef WRONG_LIGHT_TYPE
	a.light.info.conf[0].type = 10;
#else
	a.light.info.conf[0].type = 1;
#endif

	a.light.info.conf[1].id = 2;
	a.light.info.conf[1].type = 1;
	a.light.info.conf[2].id = 3;
	a.light.info.conf[2].type = 1;
	a.light.info.conf[3].id = 4;
	a.light.info.conf[3].type = 1;

	//控制表
	a.light.info.ctrl[0].id = 1;
#ifdef WRONG_LIGHT_CTRL
	a.light.info.ctrl[0].shielding = 10;
#else 
	a.light.info.ctrl[0].shielding = 1;
#endif
	a.light.info.ctrl[0].forbidden = 1;
}

void initPhase(All& a)
{
	a.phase.info.num = 2;
	a.phase.info.conf[0].id = 1;
#ifdef WRONG_PHASE_LIGHTS
	a.phase.info.conf[0].lights = 9;
#else
	a.phase.info.conf[0].lights = common::hton((uint64_t)1);  //uint64 大端存储 
#endif
#ifdef WRONG_PHASE_COLOR
	a.phase.info.conf[0].lostColor1 = 9;
#else
	a.phase.info.conf[0].lostColor1 = 1;
#endif
	a.phase.info.conf[0].lostColor2 = 1;
	a.phase.info.conf[0].lostColor3 = 1;
	a.phase.info.conf[0].acquireColor1 = 1;
	a.phase.info.conf[0].acquireColor2 = 1;
	a.phase.info.conf[0].acquireColor3 = 1;
	a.phase.info.conf[0].bootLostColor1 = 1;
	a.phase.info.conf[0].bootLostColor2 = 1;
	a.phase.info.conf[0].bootLostColor3 = 1;
	a.phase.info.conf[0].bootAcquireColor1 = 1;
	a.phase.info.conf[0].bootAcquireColor2 = 1;
	a.phase.info.conf[0].bootAcquireColor3 = 1;
	
	
	
#ifdef WRONG_PHASE_STEP
	a.phase.info.conf[0].lostStep1 = 50;
#else
	a.phase.info.conf[0].lostStep1 = 1;
#endif
	a.phase.info.conf[0].lostStep2 = 1;
	a.phase.info.conf[0].lostStep3 = 1;
#ifdef WRONG_PHASE_GREEN
	a.phase.info.conf[0].minGreen = common::hton((uint16_t)20);
#else
	a.phase.info.conf[0].minGreen = common::hton((uint16_t)10);    //unit16  大端模式
#endif 
	a.phase.info.conf[0].maxGreen1 = common::hton((uint16_t)20);   //unit16	大端模式
	a.phase.info.conf[0].maxGreen2 = common::hton((uint16_t)25);   //unit16  大端模式

	a.phase.info.conf[1].id = 2;
	a.phase.info.conf[1].lights = common::hton((uint64_t)2);
	a.phase.info.conf[1].lostColor1 = 1;
	a.phase.info.conf[1].lostColor2 = 1;
	a.phase.info.conf[1].lostColor3 = 1;
	a.phase.info.conf[1].acquireColor1 = 1;
	a.phase.info.conf[1].acquireColor2 = 1;
	a.phase.info.conf[1].acquireColor3 = 1;
	a.phase.info.conf[1].bootLostColor1 = 1;
	a.phase.info.conf[1].bootLostColor2 = 1;
	a.phase.info.conf[1].bootLostColor3 = 1;
	a.phase.info.conf[1].bootAcquireColor1 = 1;
	a.phase.info.conf[1].bootAcquireColor2 = 1;
	a.phase.info.conf[1].bootAcquireColor3 = 1;
	a.phase.info.conf[1].lostStep1 = 1;
	a.phase.info.conf[1].lostStep2 = 1;
	a.phase.info.conf[1].lostStep3 = 1;
	a.phase.info.conf[1].minGreen = common::hton((uint16_t)10);    //unit16  大端模式
	a.phase.info.conf[1].maxGreen1 = common::hton((uint16_t)20);   //unit16	大端模式
	a.phase.info.conf[1].maxGreen2 = common::hton((uint16_t)25);   //unit16  大端模式

	a.phase.info.conf[2].id = 3;
	a.phase.info.conf[2].lights = common::hton((uint64_t)4);      
	a.phase.info.conf[2].lostColor1 = 1;
	a.phase.info.conf[2].lostColor2 = 1;
	a.phase.info.conf[2].lostColor3 = 1;
	a.phase.info.conf[2].lostStep1 = 1;
	a.phase.info.conf[2].lostStep2 = 1;
	a.phase.info.conf[2].lostStep3 = 1;
	a.phase.info.conf[2].acquireColor1 = 1;
	a.phase.info.conf[2].acquireColor2 = 1;
	a.phase.info.conf[2].acquireColor3 = 1;
	a.phase.info.conf[2].bootLostColor1 = 1;
	a.phase.info.conf[2].bootLostColor2 = 1;
	a.phase.info.conf[2].bootLostColor3 = 1;
	a.phase.info.conf[2].bootAcquireColor1 = 1;
	a.phase.info.conf[2].bootAcquireColor2 = 1;
	a.phase.info.conf[2].bootAcquireColor3 = 1;
	a.phase.info.conf[2].minGreen = common::hton((uint16_t)10);    //unit16  大端模式
	a.phase.info.conf[2].maxGreen1 = common::hton((uint16_t)20);   //unit16	大端模式
	a.phase.info.conf[2].maxGreen2 = common::hton((uint16_t)25);   //unit16  大端模式

	a.phase.info.conf[3].id = 4;
	a.phase.info.conf[3].lights = common::hton((uint64_t)8);
	a.phase.info.conf[3].lostColor1 = 1;
	a.phase.info.conf[3].lostColor2 = 1;
	a.phase.info.conf[3].lostColor3 = 1;
	a.phase.info.conf[3].lostStep1 = 1;
	a.phase.info.conf[3].lostStep2 = 1;
	a.phase.info.conf[3].lostStep3 = 1;
	a.phase.info.conf[3].acquireColor1 = 1;
	a.phase.info.conf[3].acquireColor2 = 1;
	a.phase.info.conf[3].acquireColor3 = 1;
	a.phase.info.conf[3].bootLostColor1 = 1;
	a.phase.info.conf[3].bootLostColor2 = 1;
	a.phase.info.conf[3].bootLostColor3 = 1;
	a.phase.info.conf[3].bootAcquireColor1 = 1;
	a.phase.info.conf[3].bootAcquireColor2 = 1;
	a.phase.info.conf[3].bootAcquireColor3 = 1;
	a.phase.info.conf[3].minGreen = common::hton((uint16_t)10);    //unit16  大端模式
	a.phase.info.conf[3].maxGreen1 = common::hton((uint16_t)20);   //unit16	大端模式
	a.phase.info.conf[3].maxGreen2 = common::hton((uint16_t)25);   //unit16  大端模式

	a.phase.info.ctrl[0].id = 1;
	a.phase.info.ctrl[0].shielding = 1;
#ifdef WRONG_PHASE_CTRL
	a.phase.info.ctrl[0].forbidden = 2;
#else
	a.phase.info.ctrl[0].forbidden = 1;
#endif
	
}
void initPhaseStage(All& a)
{
	a.phaseStage.info.num = 4;
	a.phaseStage.info.conf[0].id = 1;
	a.phaseStage.info.conf[0].phases = common::hton((uint64_t)1);  //bit0为1，代表相位1 ,大端模式存


#ifdef WRONG_PHASESTAGE_LISTTIME
	a.phaseStage.info.conf[0].lateStartTime[0] = 20;
	a.phaseStage.info.conf[0].earlyEndTime[0] = 20;
#else 
	a.phaseStage.info.conf[0].lateStartTime[0] = 1;
	a.phaseStage.info.conf[0].earlyEndTime[0] = 1;
#endif
	a.phaseStage.info.conf[1].id = 2;
	a.phaseStage.info.conf[1].phases = common::hton((uint64_t)2);
	a.phaseStage.info.conf[1].lateStartTime[2] = 2;
	a.phaseStage.info.conf[1].earlyEndTime[2] = 2;

	a.phaseStage.info.conf[2].id = 3;
	a.phaseStage.info.conf[2].phases = common::hton((uint64_t)4);
	a.phaseStage.info.conf[2].lateStartTime[3] = 1;
	a.phaseStage.info.conf[2].earlyEndTime[3] = 1;
	
	a.phaseStage.info.conf[3].id = 4;
#ifdef WRONG_PHASESTAGE_PHASEID		//错误相位id检查， 宏定义
	a.phaseStage.info.conf[3].phases = common::hton((uint64_t)16);
#else	
	a.phaseStage.info.conf[3].phases = common::hton((uint64_t)8);
#endif	
	a.phaseStage.info.conf[3].lateStartTime[4] = 2;
	a.phaseStage.info.conf[3].earlyEndTime[4] = 2;

	a.phaseStage.info.ctrl[0].id = 1;
	a.phaseStage.info.ctrl[0].shielding = 1;
#ifdef WRONG_PHASESTAGE_CTRL
	a.phaseStage.info.ctrl[0].forbidden = 3;
#else 
	a.phaseStage.info.ctrl[0].forbidden = 1;
#endif
}

void initScheme(All& a)
{
	a.scheme.info.num = 2;
	a.scheme.info.conf[0].id = 1; 		//编号1方案
#ifdef WRONG_SCHEME_ROADID				//错误路口id检查
	a.scheme.info.conf[0].roadId = 10;   
#else
	a.scheme.info.conf[0].roadId = 1;
#endif
#ifdef WRONG_SCHEME_CYCLE				//错误周期设置
	a.scheme.info.conf[0].cycle = common::hton((uint32_t)20); 
#else  
	a.scheme.info.conf[0].cycle = common::hton((uint32_t)60);   //大端模式
#endif
#ifdef WRONG_SCHEME_COORD				//错误协调相位阶段序号
	a.scheme.info.conf[0].coordinatePhaseStage = 3; 
#else	
	a.scheme.info.conf[0].coordinatePhaseStage = 1; 
#endif
#ifdef WRONG_SCHEME_PHASESTAGE
	a.scheme.info.conf[0].phaseStageList[0] = 0; //至少下标0包含一个相位阶段id
#else
	a.scheme.info.conf[0].phaseStageList[0] = 1;  //phaseStage id 检查，可以先不initPhaseStage
#endif 
	a.scheme.info.conf[0].phaseStageList[1] = 2; 
	a.scheme.info.conf[0].phaseStageListTime[0] = common::hton((uint16_t)30); 	 //uint16_t 大端模式存
	a.scheme.info.conf[0].phaseStageListTime[1] = common::hton((uint16_t)30);  

#ifdef WRONG_SCHEME_PHASESTAGETYPE
	a.scheme.info.conf[0].phaseStageType[0] = 0x1;  
#else	
	a.scheme.info.conf[0].phaseStageType[0] = 0x10;
#endif  
	a.scheme.info.conf[0].phaseStageType[1] = 0x10;


	a.scheme.info.conf[1].id = 2; 			//编号2方案
	a.scheme.info.conf[1].roadId = 1;
	a.scheme.info.conf[1].cycle = common::hton((uint32_t)60); 
	a.scheme.info.conf[1].coordinatePhaseStage = 3; 
	a.scheme.info.conf[1].phaseStageList[0] = 3;  //phaseStage id 检查，可以先不initPhaseStage
	a.scheme.info.conf[1].phaseStageList[1] = 4; 
	a.scheme.info.conf[1].phaseStageListTime[0] = common::hton((uint16_t)30);
	a.scheme.info.conf[1].phaseStageListTime[1] = common::hton((uint16_t)30);  
	a.scheme.info.conf[1].phaseStageType[0] = 0x10;  
	a.scheme.info.conf[1].phaseStageType[1] = 0x10;  
	
}

void initPlan(All& a)
{
	//正确配置
	a.plan.info.num = 1;
	a.plan.info.conf[0].id = 1;
#ifdef WRONG_PLAN_ROADID				//错误路口id检查
	a.plan.info.conf[0].roadId = 10;   
#else
	a.plan.info.conf[0].roadId = 1;
#endif
	a.plan.info.conf[0].schemeList[0] = 1;  
#ifdef WRONG_PLAN_SCHEMEID
	a.plan.info.conf[0].schemeList[1] = 3;	//错误id
#else
	a.plan.info.conf[0].schemeList[1] = 2;
#endif
#ifdef WRONG_PLAN_STARTTIME0				//错误时段1设置
	a.plan.info.conf[0].startTimeList[0] = 1;
#else 
	a.plan.info.conf[0].startTimeList[0] = 0;  //正确时段1固定为0:00
#endif 	
	a.plan.info.conf[0].startTimeList[1] = 0;  

#ifdef WRONG_PLAN_STARTTIME1			 //错误时段2设置
	a.plan.info.conf[0].startTimeList[2] = 0;  
#else 
	a.plan.info.conf[0].startTimeList[2] = 7;  //分
#endif 	
	a.plan.info.conf[0].startTimeList[3] = 0;  //时

#ifdef WRONG_PLAN_MODE						//错误运行模式	
	a.plan.info.conf[0].runningMode[0] = 1;
#else
	a.plan.info.conf[0].runningMode[0] = 0x21;
#endif 	
	a.plan.info.conf[0].runningMode[1] = 0x22;

}

void initSchedule(All& a)
{

	a.schedule.info.num = 1;  		
	a.schedule.info.conf[0].id = 1;   
#ifdef WRONG_SCHEDULE_ROADID				//错误路口id检查
	a.schedule.info.conf[0].roadId = 10;   
#else
	a.schedule.info.conf[0].roadId = 1;   
#endif	
	a.schedule.info.conf[0].priority = 1;   
#ifdef WRONG_SCHEDULE_WEEK		//错误星期设置
	a.schedule.info.conf[0].week = 0xff;   
#else	
	a.schedule.info.conf[0].week = 1;   
#endif
#ifdef WRONG_SCHEDULE_MONTH		//错误月份设置
	a.schedule.info.conf[0].month = common::hton((uint16_t)0x2fff);  
#else	
	a.schedule.info.conf[0].month = common::hton((uint16_t)1);    //uint16
#endif
#ifdef WRONG_SCHEDULE_DAY		//错误日期设置	
	a.schedule.info.conf[0].day = common::hton((uint32_t)0x8fffffff);   
#else	
	a.schedule.info.conf[0].day = common::hton((uint32_t)1);   //uint32
#endif
#ifdef WRONG_SCHEDULE_PLANID
	a.schedule.info.conf[0].planId = 2;    //错误id检查，使用宏定义
#else
	a.schedule.info.conf[0].planId = 1;
#endif 
}
void init(All& a)
{
	initLight(a);  		//灯组配置初始化
	initPhase(a);		//相位配置初始化	
	initPhaseStage(a);	//相位阶段初始化
	initScheme(a);		//方案初始化
	initPlan(a);		//日计划初始化
	initSchedule(a);	//调度初始化
}

int main(int argc, char const *argv[])
{

	All all;
	memset(&all,0,sizeof(All));
	ErrorCheckStatus status;
	init(all);
	// cout << "light conf[0] type: " << (int)all.light.info.conf[0].type << endl;

	status = all.scheduleCheck();
	cout << "schedule status val: " << status << endl;

	status = all.planCheck();
	cout << "plan status val: " << status << endl;
	
	status = all.schemeCheck();
	cout << "scheme status val: " << status << endl;
	
	status = all.phaseStageCheck();
	cout << "phaseStage status val: " << status << endl;

	status = all.phaseCheck();
	cout << "phase status val: " << status << endl;
	
	status = all.lightCheck();
	cout << "light status val: " << status << endl;
	return 0;
}