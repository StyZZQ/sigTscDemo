#pragma once 
#include "meta.h"
#include "value.h"
#include "error.h"
#define MAX_GB2017_LIGHT_NUM        64      //最大灯组个数
#define MAX_GB2017_PHASE_NUM        64      //最大相位个数
#define MAX_GB2017_PHASESTAGE_NUM   64      //最大相位阶段个数
#define MAX_GB2017_SCHEME_NUM       128     //最大方案个数
#define MAX_GB2017_PLAN_NUM         128     //最大日计划个数
#define MAX_GB2017_SCHEDULE_NUM     128     //最大调度个数
#define MAX_GB2017_DETECTOR_NUM     64     //最大检测器个数
#define MAX_ROAD_NUM                8       //最大控制路口数量
#ifdef __linux__
	/*以下宏定义是printf时的一些颜色打印，可以根据自己的喜好进行选择 */
	#define COL_DEF "\033[m"			//系统默认的打印颜色，一般是白色
	#define COL_RED "\033[0;32;31m"		//红色打印
	#define COL_GRE "\033[0;32;32m"		//绿色打印
	#define COL_BLU "\033[0;32;34m"		//蓝色打印
	#define COL_YEL "\033[1;33m"		//黄色打印
#else	//__WIN32__ C++
	#define COL_DEF 
	#define COL_RED 
	#define COL_GRE 
	#define COL_BLU 
	#define COL_YEL
#endif

#define ERR(fmt, ...) fprintf(stderr, COL_RED "file[%s]:" COL_YEL "line[%d]:" COL_DEF fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define INFO(fmt, ...) printf(COL_GRE "file[%s]:" COL_YEL "line[%d]:" COL_DEF fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define GET_BIT(v, bit)     (((v) >> (bit)) & 0x1)  //取v的某一bit位
#define SET_BIT(v, bit)     ({(v) |= (1 << (bit));})    //设置v的某一bit位
#define CLR_BIT(v, bit)     ({(v) &= (~(1 << (bit)));}) //清零v的某一bit位

namespace GB2017 {

#pragma pack(push,1) 

struct All;

struct FrameHead{
    uint16_t version;
    uint8_t ctrlCenterID;
    uint32_t tscID;
    uint8_t roadID;
    uint8_t frameSeq;
    uint8_t frameType;
    uint8_t dataNum;
};     //帧结构

//以下是标志号定义包括 数据类ID、对象ID、属性ID、元素ID
enum ClassType
{
    DEVICE = 1,
    BASIC = 2,
    LIGHT = 3,
    PHASE = 4,
    DETECTOR = 5,
    PHASESTAGE = 6,
    PHASESECURITY = 7,
    EMERGENCYPRIORTY = 8,
    PLAN = 9,
    TRANSITIONCONSTRAINT = 10,
    SCHEME = 11,
    SCHEDULE = 12,
    RUNNINGMODE = 13,
    TRAFFIC = 14,
    ALARM = 15,
    FAULT = 16,
    CENTERCONTROL = 17,
    ORDER = 18,
};  //数据类

enum OverTime
{ 
    overTime1 = 0x32,
    overTime2 = 0x64,
    overTime3 = 0xC8,
};   //超时时间

enum LightType
{
    LT_Vehicle = 0x01,      //机动车灯
    LT_NonVehicle = 0x02,   //非机动车灯
    LT_Pedstrain = 0x03,    //行人灯
    LT_Road = 0x04,         //车道灯
    LT_Trafficsign = 0x05, //可变交通标志
    LT_Bus = 0x06,          //公交专用灯
    LT_Tramcar = 0x07,     //有轨电车
    LT_Special = 0x08,      //特殊
};     //灯组类型

enum PhaseStageType
{
    PST_Fix = 0x10,         //固定出现
    PST_Demand = 0x20,      //按需求出现
    PST_Timed = 0x30,       //时间时变
    PST_Adaptive = 0x40,    //自适应
};    //相位阶段出现类型

 enum PhaseStageState
{
    PSS_NotOfWay = 0x10,    //未放行
    PSS_OnTheWay = 0x20,    //正在放行
    PSS_Transion = 0x30,    //过渡
};

enum FrameType
{
    Type_Query = 0x10,         //查询   
    Type_Query_Reply = 0x20,
    Type_Query_Error_Reply = 0x21,
    Type_Set = 0x30,           //设置   
    Type_Set_Reply = 0x40,
    Type_Set_Error_Reply = 0x41,
    Type_Broadcast = 0x50,    
    Type_Trap = 0x60,         //主动上报
    Type_Heart_Search = 0x70, //心跳
    Type_Heart_Reply = 0x80,
}; 

enum LightStatus
{
    LS_Off = 0x01,              //灭灯
    LS_Red = 0x10,              //红灯
    LS_RedFlash = 0x11,         //红闪
    LS_RedFastFlash = 0x12,     //红快闪
    LS_Green = 0x20,            //绿灯
    LS_GreenFlash = 0x21,       //绿快闪    
    LS_GreenFastFlash = 0x22,   //绿快闪
    LS_Yellow = 0x30,           //黄灯
    LS_YellowFlash = 0x31,      //黄闪
    LS_YellowFastFlash = 0x32,  //黄快闪    
    LS_RedYellow = 0x40,        //红黄灯
};   //灯组状态 A.2.7

enum Mode
{
    Mode_Center_Control = 0x10,                 //中心控制模式
    Mode_Center_TimeTable_Control = 0x11,
    Mode_Center_Optimization_Control = 0x12,
    Mode_Center_Coordination_Control = 0x13,
    Mode_Center_Adaptive_Control = 0x14,
    Mode_Center_Manual_Control = 0x15,
    Mode_Center_Flash_Control = 0x16,
    Mode_Center_AllRed_Control = 0x17,
    Mode_Center_AllOff_Control = 0x18,
    Mode_Center_Va_Control = 0x19,
    Mode_Center_Plan_Control = 0x1A,
    Mode_Local_Control = 0x20,          //本地控制模式
    Mode_Local_FixCycle_Control = 0x21,
    Mode_Local_Va_Control = 0x22,
    Mode_Local_Coordination_Control = 0x23,
    Mode_Local_Adaptive_Control = 0x24,
    Mode_Local_Manual_Control = 0x25,
    Mode_Local_Manual_Flash_Control = 0x26,
    Mode_Local_Manual_AllRed_Control = 0x27,
    Mode_Vehicle_Priority_Control = 0x28,
    Mode_Pedestrain_Request_Control = 0x29,
    Mode_AllRed_To_Green_Control = 0x2A,
    Mode_Arrange_Control = 0x2B,
    Mode_Semi_Va_Control = 0x2C,
    Mode_Anti_Overflow_Control = 0x2D,
    Mode_Special_Control = 0x30,        //特殊控制
    Mode_Special_Flash_Control = 0x31,
    Mode_Special_AllRed_Control = 0x32,
    Mode_Special_AllOff_Control = 0x33,
};  //运行模式

struct GBDeviceInfo
{
    char producer[128];
    uint32_t version;
    char NO[16];
    uint8_t dateOfProduction[7];
    uint8_t dateOfConfig[7];
    METAS(producer, version, NO, dateOfProduction, dateOfConfig);
};    //数据类1:设备信息

struct GBTscIPV4
{
    uint32_t IP;
    uint32_t netMask;
    uint32_t gateway;
    METAS(IP, netMask, gateway);
};      

struct GBCenterIPV4 
{
    uint32_t IP;
    uint16_t port;
    uint8_t protocol;
    METAS(IP, port, protocol);
};

struct GBTscIPV6 
{
    uint8_t IP[16];
    uint8_t netMask[16];
    uint8_t gateway[16];
    METAS(IP, netMask, gateway);
};      
    
struct GBCenterIPV6
{
    uint8_t IP[16];
    uint16_t port;
    uint8_t protocol;
    METAS(IP, port, protocol);
};      

struct GBBasicInfo
{
    char installRoad[128];
    GBTscIPV4 tscIPV4;
    GBCenterIPV4 centerIPV4;
    uint32_t timezone;
    uint32_t tscID;
    uint8_t ctrlRoadNum;
    uint8_t gpsFlag;
    GBTscIPV6 tscIPV6;
    GBCenterIPV6 centerIPV6;
    METAS(installRoad, tscIPV4, centerIPV4, timezone, tscID, ctrlRoadNum, gpsFlag,
        tscIPV6, centerIPV6);
}; //数据类2 基础信息

struct GBLightConf
{
    uint8_t id;
    uint8_t type;
    METAS(id, type);
};

struct GBLightStatus
{
    uint8_t id;
    uint8_t status;
    METAS(id, status);
};

struct GBLightCtrl
{
    uint8_t id;
    uint8_t shielding;
    uint8_t forbidden;
    METAS(id, shielding, forbidden);
};

//init 表示初始化该结构体的配置
//print 表示打印该结构体的配置
//findConf 表示查找配置表中相关的id
//findCtrl 表示查找控制表中相关id

struct GBLightInfo
{
    uint8_t num;
    GBLightConf conf[MAX_GB2017_LIGHT_NUM];
    GBLightStatus status[MAX_GB2017_LIGHT_NUM];
    GBLightCtrl ctrl[MAX_GB2017_LIGHT_NUM];
    ErrorCheckStatus check() const;                 
    const GBPhaseConf* findconf(uint8_t lightId) const; 
    const GBLightCtrl* findctrl(uint8_t lightId) const;

    METAS(num, conf, status, ctrl);
}; //数据类3 灯组信息

struct GBPhaseConf{
    uint8_t id;
    uint64_t lights;
    uint8_t lostColor1;
    uint8_t lostStep1;
    uint8_t lostColor2;
    uint8_t lostStep2;
    uint8_t lostColor3;
    uint8_t lostStep3;
    uint8_t acquireColor1;
    uint8_t acquireStep1;
    uint8_t acquireColor2;
    uint8_t acquireStep2;
    uint8_t acquireColor3;
    uint8_t acquireStep3;
    uint8_t bootAcquireColor1;
    uint8_t bootAcquireStep1;
    uint8_t bootAcquireColor2;
    uint8_t bootAcquireStep2;
    uint8_t bootAcquireColor3;
    uint8_t bootAcquireStep3;
    uint8_t bootLostColor1;
    uint8_t bootLostStep1;
    uint8_t bootLostColor2;
    uint8_t bootLostStep2;
    uint8_t bootLostColor3;
    uint8_t bootLostStep3;
    uint16_t minGreen;
    uint16_t maxGreen1;         
    uint16_t maxGreen2;
    uint16_t extendGreen;
    uint64_t requests;

    uint16_t loseTotal() const { return lostStep1 + lostStep2 + lostStep3; } //返回总失去路权灯色时间
    METAS(id, lights, lostColor1, lostStep1, lostColor2, lostStep2, lostColor3, lostStep3,
            acquireColor1, acquireStep1, acquireColor2, acquireStep2, acquireColor3, acquireStep3,
            bootAcquireColor1, bootAcquireStep1, bootAcquireColor2, bootAcquireStep2, bootAcquireColor3, bootAcquireStep3,
            bootLostColor1, bootLostStep1, bootLostColor2, bootLostStep2, bootLostColor3, bootLostStep3,
            minGreen, maxGreen1, maxGreen2, extendGreen, requests);
};

struct GBPhaseCtrl
{
    uint8_t id;
    uint8_t shielding;
    uint8_t forbidden; 
    METAS(id, shielding, forbidden);   
};

struct GBPhaseInfo //数据类4 相位信息
{
    uint8_t num;
    GBPhaseConf conf[MAX_GB2017_PHASE_NUM];
    GBPhaseCtrl ctrl[MAX_GB2017_PHASE_NUM];
    ErrorCheckStatus check(const All *all) const;
    const PhaseConf *findConf(uint8_t phaseId) const;
    const PhaseCtrl *findCtrl(uint8_t phaseId) const;
    METAS(num, conf, ctrl);
};

/* struct GBDetectorConf
{
    uint8_t id;
    uint8_t type;
    uint16_t flowCycle;
    uint16_t occupancyCycle;
    char installPos[128];
    METAS(id, type, flowCycle, occupancyCycle, installPos);
};

struct GBDetectorStatus
{
    uint8_t id;
    uint8_t vheExist;
    uint8_t vheSpeed;    
    uint8_t vheType;    
    char vhePlate[16];    
    uint16_t vheQueLen;   
    METAS(id, vheExist, vheSpeed, vheType, vhePlate, vheQueLen);
};   

struct GBDetectorInfo     //数据类5 检测器信息
{
    uint8_t num;
    GBDetectorConf conf[MAX_GB2017_DETECTOR_NUM];
    GBDetectorStatus status[MAX_GB2017_DETECTOR_NUM];
    METAS(num, conf, status);
};   */

struct GBPhaseStageConf
{
    uint8_t id;
    uint64_t phases;
    uint8_t lateStartTime[64];
    uint8_t earlyEndTime[64];
    METAS(id, phases, lateStartTime, earlyEndTime);
};

struct GBPhaseStageStatus
{
    uint8_t id;
    uint8_t status;
    uint16_t used;   
    uint16_t left;  
    METAS(id,status,used,left);
}; 

struct GBPhaseStageCtrl
{
    uint8_t id;
    uint8_t softwareReq;
    uint8_t shielding;    
    uint8_t forbidden;    
  
    METAS(id, softwareReq, shielding, forbidden);  
}; 

struct GBPhaseStageInfo
{
    uint8_t num;
    GBPhaseStageConf conf[MAX_GB2017_PHASESTAGE_NUM];
    GBPhaseStageStatus status[MAX_GB2017_PHASESTAGE_NUM];
    GBPhaseStageCtrl ctrl[MAX_GB2017_PHASESTAGE_NUM];
    ErrorCheckStatus check(const All *all) const;
    const GBPhaseStageConf *findConf(uint8_t stageId) const;
    const GBPhaseStageCtrl *findCtrl(uint8_t stageId) const;
    METAS(num, conf, status, ctrl);
};    //数据类6 相位阶段信息

struct PhaseConflictConf
{
    uint8_t id;
    uint64_t conflicts;
    METAS(id, conflicts);
};

struct PhaseGreenIntervalConf
{
    uint8_t id;
    uint8_t intervalTime[64];  
    METAS(id, intervalTime);
};

struct GBPhaseSecurity
{
    PhaseConflictConf conflict[MAX_GB2017_PHASE_NUM];
    PhaseGreenIntervalConf interval[MAX_GB2017_PHASE_NUM];

    void init();
    ErrorCheckStatus check(const All *all) const;
    bool isConflict(uint64_t phaseBits) const;
    METAS(conflict, interval);
};

struct GBSchemeConf //方案配置表
{ 
    uint8_t id;
    uint8_t roadId;
    uint32_t cycle;
    uint8_t coordinatePhaseStage;
    uint16_t offset;
    uint8_t phaseStageList[16];
    uint16_t phaseStageListTime[16];
    uint8_t phaseStageType[16];
    ErrorCheckStatus check() const;
    METAS(id, roadId, cycle, coordinatePhaseStage, offset, phaseStageList, phaseStageListTime, phaseStageType);
};

struct GBSchemeInfo
{
    uint8_t num;
    GBSchemeConf conf[MAX_GB2017_SCHEME_NUM];
    ErrorCheckStatus check(const All *all, bool onlyCheckRelated = false) const;  //onlyCheckRelated关联性检查
    const GBSchemeConf *findConf(uint8_t schemeId) const;

    METAS(num, conf);
};  //数据类9 方案信息

struct TimeInterval 
{
    uint8_t min;
    uint8_t hour;
};

struct GBPlanConf
{
    uint8_t id;
    uint8_t roadId;
    uint8_t startTimeList[48];  //对应TimeInterval[48]
    uint8_t schemeList[48];
    uint8_t runningMode[48];
    uint8_t actionList1[96];
    uint8_t actionList2[96];
    uint8_t actionList3[96];
    uint8_t actionList4[96];
    uint8_t actionList5[96];
    uint8_t actionList6[96];
    uint8_t actionList7[96];
    uint8_t actionList8[96];
    ErrorCheckStatus check() const;
    METAS(id, roadId, startTimeList, schemeList, runningMode, actionList1, actionList2,
        actionList3, actionList4, actionList5, actionList6, actionList7, actionList8);
};

struct GBPlanInfo
{
    uint8_t num;
    GBPlanConf conf[MAX_GB2017_PLAN_NUM];
    ErrorCheckStatus check(const All *all, bool onlyCheckRelated = false) const; //onlyCheckRelated关联性检查
    const GBPlanConf *findConf(uint8_t planId) const;
    METAS(num, conf);
};    //数据类11 日计划

struct GBScheduleConf
{
    uint8_t id;
    uint8_t roadId;
    uint8_t priority;
    uint8_t week;
    uint16_t month;
    uint32_t day;
    uint8_t planId;
    ErrorCheckStatus check() const;
    METAS(id, roadId, priority, week, month, day, planId);
};

struct GBScheduleInfo
{
    uint8_t num;
    GBScheduleConf  conf[MAX_GB2017_SCHEDULE_NUM];
    ErrorCheckStatus check(const All *all, bool onlyCheckRelated = false) const; //onlyCheckRelated关联性检查
    METAS(num, conf);
};  //数据类 12 调度表

struct GBStatistics
{
    uint8_t detectorId;
    uint16_t flow;
    uint8_t occupancy;
    uint16_t avgSpeed;
    METAS(detectorId, flow, occupancy, avgSpeed);
};

struct GBTrafficInfo
{
    uint8_t realtime_data[8];
    GBStatistics statistics[MAX_GB2017_DETECTOR_NUM];
    METAS(realtime_data, statistics);
}; //数据类14 交通数据

struct GBAlarmData
{
    uint32_t id;
    uint8_t type;
    uint8_t val;
    uint8_t timestamp[7];
    uint8_t detail[2];
    METAS(id, type, val, timestamp, detail);
};

struct GBAlarmInfo
{
    uint8_t num;
    GBAlarmData data;
    METAS(num, data);
}; //数据类15 报警数据

struct GBFaultRecord
{
    uint32_t id;
    uint8_t type;
    uint8_t timestamp[7];
    uint8_t action;
    uint8_t detail[2];
    METAS(id, type, timestamp, action, detail);
};

struct GBFaultInfo
{
    uint8_t num;
    GBFaultRecord fault;
    METAS(num, fault);
}; //数据类16 故障数据


struct All
{
    DataClass<GBDeviceInfo, 1> device;
    DataClass<GBBasicInfo, 2> basic;
	DataClass<GBLightInfo, 3> light;
    DataClass<GBPhaseInfo, 4> phase;
    DataClass<GBPhaseStageInfo, 6> phaseStage;
    DataClass<GBPhaseSecurity, 7> phaseSecurity;
    DataClass<GBSchemeInfo, 9> scheme;
    DataClass<GBPlanInfo, 11> plan;
    DataClass<GBScheduleInfo, 12> schedule;
    DataClass<GBTrafficInfo, 14> traffic;
    DataClass<GBAlarmInfo, 15> alarm;
    DataClass<GBFaultInfo, 16> fault;

    //全检查
    ErrorCheckStatus lightCheck();
    ErrorCheckStatus phaseCheck();
    ErrorCheckStatus phaseStageCheck();
    ErrorCheckStatus phaseSecurityCheck();
    ErrorCheckStatus schemeCheck();
    ErrorCheckStatus planCheck();
    ErrorCheckStatus scheduleCheck();

	METAS(device, basic, light, phase, phaseStage, phaseSecurity, scheme, plan, schedule, traffic, alarm, fault);
};

#pragma pack(pop)


//以下两个函数用以获取网络字节序数组中的元素
template<int N>
static inline uint8_t get(const uint8_t (&arr)[N], int index)
{
    return (index < N) ? arr[N - 1 - index] : 0;
}

template<int N>
static inline uint16_t get(const uint16_t (&arr)[N], int index)
{
    uint16_t v = (index < N) ? arr[N - 1 - index] : 0;
    return (v << 8) | (v >> 8);
}

}  //end of namespace
