#pragma once 
#include "meta.h"
#include "inc.h"
#include "value.h"

#define LIGHT_NUM		    16	//灯组个数
#define PHASE_NUM		    4	//相位个数
#define PHASESTAGE_NUM      1  //相位阶段数
#define PHASESCY_NUM        1  //相位安全数
#define PRIORITY_NUM        1  //优先数
#define EMRG_NUM            1  //紧急数 
#define DETECTOR_NUM	    1	//检测器个数
#define PLAN_NUM            2   //方案个数
#define SCHEME_NUM		    2	//日计划个数
#define SCHEDULE_NUM	    1	//周期个数
#define ROAD_NUM            1   //路口数
#define MAX_GB2017_LIGHT_NUM        64      //最大灯组个数
#define MAX_GB2017_PHASE_NUM        64      //最大相位个数
#define MAX_GB2017_PHASESTAGE_NUM   64      //最大相位阶段个数
#define MAX_GB2017_SCHEME_NUM       128     //最大方案个数
#define MAX_GB2017_PLAN_NUM         128     //最大日计划个数
#define MAX_GB2017_SCHEDULE_NUM     128     //最大调度个数

#pragma pack(push,1) 

struct FrameHead{
    uint16_t version;
    uint8_t ctrlCenterID;
    uint32_t tscID;
    uint8_t roadID;
    uint8_t frameSeq;
    uint8_t frameType;
    uint8_t dataNum;
};     //帧结构

enum eOverTime
{ 
    overTime1 = 0x32,
    overTime2 = 0x64,
    overTime3 = 0xC8,
};   //超时时间

enum eFrameType
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
};     //帧类型

enum eLightType
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

enum eLightStatus
{
    LS_Off = 0x01,              //灭灯
    LS_Red = 0x10,
    LS_RedFlash = 0x11,
    LS_RedFastFlash = 0x12,
    LS_Green = 0x20,
    LS_GreenFlash = 0x21,
    LS_GreenFastFlash = 0x22,   //绿快闪
    LS_Yellow = 0x30,
    LS_YellowFlash = 0x31,
    LS_YellowFastFlash = 0x32,  
    LS_RedYellow = 0x40,        //红黄灯
};   //灯组状态

enum eDetectorType
{
    Detector_Coil = 0x01,           //线圈
    Detector_Video = 0x02,          //视频
    Detector_Geomagnetic = 0x03,    //地磁
    Detector_MicroWave = 0x04,      //微波检测器
    Detector_Ultrasonic = 0x05,     //超声波
    Detector_Infrared = 0x06,       //红外
    Detector_RFID = 0x07,           //RFID
    Detector_Laser = 0x08,          //激光
    Detector_Other = 0x09,
};  //检测器类型

enum ePhaseStageType
{
    PST_Fix = 0x10,         //固定出现
    PST_Demand = 0x20,      //按需求出现
    PST_Timed = 0x30,       //时间时变
    PST_Adaptive = 0x40,    //自适应
};    //相位阶段出现类型

 enum ePhaseStageStatus
{
    PSS_NotOfWay = 0x10,    //未放行
    PSS_OnTheWay = 0x20,    //正在放行
    PSS_Transion = 0x30,    //过渡
};  //相位阶段状态

enum eMode
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

enum eAlarmType
{
    AT_Light = 0x10,
    AT_Detector = 0x30,
    AT_Device = 0x40,
    AT_Environment = 0x60,
    AT_OverFlow = 0xE0,
    AT_Reboot = 0xF0,
};  //报警类型

enum eFaultType
{
    FT_GreenConflict = 0x10,
    FT_GreenRedConflict = 0x11,
    FT_RedLight = 0x20,
    FT_YellowLight = 0x21,
    FT_GreenLight = 0x22,
    FT_Communication = 0x30,
    FT_Self = 0x40,
    FT_Detector = 0x41,
    FT_Realy = 0x42,
    FT_Memory = 0x43,
    FT_Clock = 0x44,
    FT_MotherBoard = 0x45,
    FT_PhaseBoard = 0x46,
    FT_DetectorBoard = 0x47,
    FT_Config = 0x50,
    FT_Response = 0x70,
};  //故障类型

enum eFaultAction
{
    Switch_Null = 0x00,
    Switch_To_Flash = 0x10,
    Switch_To_Off = 0x20,
    Switch_To_Red = 0x30,
    Switch_To_Local_FixCycle = 0x40,
    Switch_To_Local_Coordination = 0x50,
    Switch_To_Local_Va = 0x60,
};  //故障动作

enum eOrderType
{
    Order_Flash = 0x01,
    Order_Red = 0x02,
    Order_On = 0x03,
    Order_Off = 0x04,
    Order_Reset = 0x05,
    Order_Cancel = 0x00,
};  //命令管道

enum eTrafficFlowDirection
{
    East = 0x01,
    West = 0x02,
    South = 0x03,
    North = 0x04,
    Northeast = 0x05,
    Northwest = 0x06,
    Southeast = 0x07,
    Southwest = 0x08,
};  //交通流方向

enum eTrafficFlowTurn
{
    Left = 0x01,
    Straight = 0x02,
    Right = 0x03,
    StraightLeft = 0x04,
    StraightRight = 0x05,
    LeftRight = 0x06,
    LeftStraightRight = 0x07,
    UTurn = 0x08,
    UTurnLeft = 0x09,
    UTurnStraight = 0x0A,
    UTurnRight = 0x0B,
    UTurnStraightLeft = 0x0C,
    UTurnStraightRight = 0x0D,
    UTurnLeftRight = 0x0E,
    UTurnLeftStraightRight = 0x0F,
    PedestrainCross = 0x10,
    PedestrainCrossOne = 0x11,
    PedestrainCrossTwo = 0x12,
    PedestrainWaitingAreaOne = 0x13,
    PedestrainWaitingAreaTwo = 0x14,
};  //交通流的流向

//以下是标志号定义包括 数据类ID、对象ID、属性ID、元素ID
enum eClassType
{
    deviceInfo = 0x01,
    basicInfo = 0x02,
    lightInfo = 0x03,
    phaseInfo = 0x04,
    detectorInfo = 0x05,
    phaseStageInfo = 0x06,
    phaseSecurityInfo = 0x07,
    emergencyPriority = 0x08,
    planInfo = 0x09,
    transitionConstraint = 0x0A,
    scheme = 0x0B,
    schedule = 0x0C,
    runningMode = 0x0D,
    trafficInfo = 0x0E,
    alarmInfo = 0x0F,
    faultInfo = 0x10,
    centerControl = 0x11,
    order = 0x12,
};  //数据类

struct DeviceInfo
{
    char producer[128];
    uint32_t version;
    char NO[16];
    uint8_t dateOfProduction[7];
    uint8_t dateOfConfig[7];
    METAS(producer,version,NO,dateOfProduction,dateOfConfig);
};    //数据类1:设备信息

struct TscIPV4
{
    uint32_t IP;
    uint32_t netMask;
    uint32_t gateway;
    METAS(IP,netMask,gateway);
};      

struct CenterIPV4 
{
    uint32_t IP;
    uint16_t port;
    uint8_t protocol;
    METAS(IP,port,protocol);
};

struct TscIPV6 
{
    uint8_t IP[16];
    uint8_t netMask[16];
    uint8_t gateway[16];
    METAS(IP,netMask,gateway);
};      
    
struct CenterIPV6
{
    uint8_t IP[16];
    uint16_t port;
    uint8_t protocol;
    METAS(IP,port,protocol);
};      

struct BasicInfo
{
    char installRoad[128];
    TscIPV4 tscIPV4;
    CenterIPV4 centerIPV4;
    uint32_t timezone;
    uint32_t tscID;
    uint8_t ctrlRoadNum;
    uint8_t gpsFlag;
    TscIPV6 tscIPV6;
    CenterIPV6 centerIPV6;
    METAS(installRoad,tscIPV4,centerIPV4,timezone,tscID,ctrlRoadNum,gpsFlag,tscIPV6,centerIPV6);
}; //数据类2 基础信息

struct LightConf
{
    uint8_t id;
    uint8_t type;
    METAS(id,type);
};

struct LightStatus
{
    uint8_t id;
    uint8_t status;
    METAS(id,status);
};

struct LightCtrl
{
    uint8_t id;
    uint8_t shielding;
    uint8_t forbidden;
    METAS(id,shielding,forbidden);
};

struct LightInfo
{
    uint8_t num ;
    LightConf lightConf[MAX_GB2017_LIGHT_NUM];
    LightStatus lightStatus[MAX_GB2017_LIGHT_NUM];
    LightCtrl lightCtrl[MAX_GB2017_LIGHT_NUM];
    METAS(num, lightConf, lightStatus, lightCtrl);
}; //数据类3 灯组信息

struct PhaseConf
{
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
    uint8_t boot_acquireColor1;
    uint8_t boot_acquireStep1;
    uint8_t boot_acquireColor2;
    uint8_t boot_acquireStep2;
    uint8_t boot_acquireColor3;
    uint8_t boot_acquireStep3;
    uint8_t boot_lostColor1;
    uint8_t boot_lostStep1;
    uint8_t boot_lostColor2;
    uint8_t boot_lostStep2;
    uint8_t boot_lostColor3;
    uint8_t boot_lostStep3;
    uint16_t minGreen;
    uint16_t maxGreen1;         
    uint16_t maxGreen2;
    uint16_t extendGreen;
    uint64_t detector;
    METAS(  id,lights,lostColor1,lostStep1,lostColor2,lostStep2,lostColor3,lostStep3,
            acquireColor1,acquireStep1,acquireColor2,acquireStep2,acquireColor3,acquireStep3,
            boot_acquireColor1,boot_acquireStep1,boot_acquireColor2,boot_acquireStep2,boot_acquireColor3,boot_acquireStep3,
            boot_lostColor1,boot_lostStep1,boot_lostColor2,boot_lostStep2,boot_lostColor3,boot_lostStep3,
            minGreen,maxGreen1,maxGreen2,extendGreen,detector );
};

struct PhaseCtrl
{
    uint8_t id;
    uint8_t shielding;
    uint8_t forbidden; 
    METAS(id,shielding,forbidden);   
};

struct PhaseInfo //数据类4 相位信息
{
    uint8_t num;
    PhaseConf phaseConf[MAX_GB2017_PHASE_NUM];
    PhaseCtrl phaseCtrl[MAX_GB2017_PHASE_NUM];
    METAS(num,phaseConf,phaseCtrl);
}; 

struct DetectorConf
{
    uint8_t id;
    uint8_t type;
    uint16_t flowCycle;
    uint16_t occupancyCycle;
    char installPos[128];
    METAS(id,type,flowCycle,occupancyCycle,installPos);
};

struct DetectorData
{
    uint8_t id;
    uint8_t vheExist;
    uint8_t vheSpeed;    
    uint8_t vheType;    
    char vhePlate[16];    
    uint16_t vheQueLen;   
    METAS(id,vheExist,vheSpeed,vheType,vhePlate,vheQueLen);
};   

struct DetectorInfo     //数据类5 检测器信息
{
    uint8_t num;
    DetectorConf dectConf[DETECTOR_NUM];
    DetectorData dectData[DETECTOR_NUM];
    METAS(num,dectConf,dectData);
};  

struct PhaseStageConf
{
    uint8_t id;
    uint64_t phases;
    uint8_t lateStartTime[64];
    uint8_t earlyEndTime[64];
    METAS(id,phases,lateStartTime,earlyEndTime);
};

struct PhaseStageStatus
{
    uint8_t id;
    uint8_t status;
    uint16_t used;   
    uint16_t left;  
    METAS(id,status,used,left);
}; 

struct PhaseStageCtrl
{
    uint8_t id;
    uint8_t softwareReq;
    uint8_t shielding;    
    uint8_t forbidden;    
    METAS(id,softwareReq,shielding,forbidden);  
}; 

struct PhaseStageInfo
{
    uint8_t num;
    PhaseStageConf PSConf[PHASESTAGE_NUM];
    PhaseStageStatus PSStatus[PHASESTAGE_NUM];
    PhaseStageCtrl PSCtrl[PHASESTAGE_NUM];
    METAS(num,PSConf,PSStatus,PSCtrl);
};    //数据类6 相位阶段信息


struct PhaseConflictConf
{
    uint8_t phaseId;
    uint64_t conflictQueue;
    METAS(phaseId,conflictQueue);
};

struct PhaseInterGreenConf
{
    uint8_t phaseId;
    uint8_t timeQue[64];  
    METAS(phaseId,timeQue);
};

struct PhaseSecurityInfo
{
    PhaseConflictConf PCConf[PHASESCY_NUM];
    PhaseInterGreenConf PIGConf[PHASESCY_NUM];
    METAS(PCConf,PIGConf);
}; //数据类7 相位安全信息


struct PriorityConf
{
    uint8_t num;
    uint8_t id;
    uint8_t phaseStage;
    uint8_t level;
    uint8_t shielding;
    METAS(num,id,phaseStage,level,shielding);
};

struct PriorityStatus
{
    uint8_t id;
    uint8_t apply;  
    uint8_t execute;
    METAS(id,apply,execute);  
};   

struct EmergencyConf
{
    uint8_t id;
    uint8_t phaseStageId;
    uint8_t level;
    uint8_t shielding;
    METAS(id,phaseStageId,level,shielding);
};

struct EmergencyStatus
{
    uint8_t id;
    uint8_t apply;  
    uint8_t execute;  
    METAS(id,apply,execute);
};  

struct EmergencyPriority
{
    uint8_t priorityNum;
    PriorityConf priorConf[PRIORITY_NUM];
    PriorityStatus priorStatus[PRIORITY_NUM];
    EmergencyConf emergencyConf[EMRG_NUM];
    EmergencyStatus emergencyStatus[EMRG_NUM];
    METAS(priorityNum,priorConf,priorStatus,emergencyConf,emergencyStatus);
}; // 数据类8 紧急优先

struct SchemeConf //方案配置表
{ 
    uint8_t id;
    uint8_t roadId;
    uint32_t cycle;
    uint8_t coordinatePhase;
    uint16_t offset;
    uint8_t phaseStageList[16];
    uint16_t phaseStageListTime[16];
    uint8_t phaseStageType[16];
    METAS(id,roadId,cycle,coordinatePhase,offset,phaseStageList,phaseStageListTime,phaseStageType);
};

struct SchemeInfo
{
    uint8_t num;
    SchemeConf schemeConf[PLAN_NUM];
    METAS(num,schemeConf);

};  //数据类9 方案信息  

struct TransitionConf
{
    uint8_t phaseStageId;
    uint8_t val[64];
    METAS(phaseStageId,val);
};

struct Transition{
    TransitionConf transConf[PHASESTAGE_NUM];
    METAS(transConf);
};

struct PlanConf
{
    uint8_t id;
    uint8_t roadId;
    uint8_t startTimeList[96];
    uint8_t planList[48];
    uint8_t runningMode[48];
    uint8_t actionList1[96];
    uint8_t actionList2[96];
    uint8_t actionList3[96];
    uint8_t actionList4[96];
    uint8_t actionList5[96];
    uint8_t actionList6[96];
    uint8_t actionList7[96];
    uint8_t actionList8[96];
    METAS(  id,roadId,startTimeList,planList,runningMode,actionList1,actionList2,
            actionList3,actionList4,actionList5,actionList6,actionList7,actionList8 );
};

struct PlanInfo
{
    uint8_t num;
    PlanConf planConf[SCHEME_NUM];
    METAS(num,planConf);

};  //数据类11 日计划


struct ScheduleConf
{
    uint8_t id;
    uint8_t roadId;
    uint8_t level;
    uint8_t week;
    uint16_t month;
    uint32_t day;
    uint8_t schemeId;
    METAS( id,roadId,level,week,month,day,schemeId);
};

struct ScheduleInfo
{
    uint8_t num;
    ScheduleConf scheduleConf[SCHEDULE_NUM];
    METAS(num,scheduleConf);
};  //数据类 12 调度表

struct DeviceStatus
{
    uint8_t detector_status[8];
    uint8_t module_status[8];
    uint8_t door_status[8];
    uint16_t volt;
    uint16_t current;
    uint8_t temperature;
    uint8_t humidity;
    uint8_t immersion;
    uint8_t smoke;
    uint8_t UTCTime[7];
    uint8_t localTime[7];
    METAS(detector_status,module_status,door_status,volt,current,temperature,humidity,immersion,smoke,UTCTime,localTime);
};

struct CtrlStatus
{
    uint8_t roadId;
    uint8_t runningMode;
    uint8_t curPlan;
    uint8_t curPhaseStage;
    METAS(roadId,runningMode,curPlan,curPhaseStage);
};

struct RunningStatus
{
    uint8_t num; 
    DeviceStatus deviceStatus;
    CtrlStatus ctrlStatus;
    METAS(num,deviceStatus,ctrlStatus);
}; //数据类13 运行状态

struct Statistics
{
    uint8_t detectorId;
    uint16_t detectorFlow;
    uint8_t detectorOccupancy;
    uint16_t avgSpeed;
    METAS(detectorId,detectorFlow,detectorOccupancy,avgSpeed);
};

struct TrafficData
{
    uint8_t realtime_data[8];
    Statistics statistics[DETECTOR_NUM];
    METAS(realtime_data,statistics);
}; //数据类14 交通数据

struct AlarmData
{
    uint32_t id;
    uint8_t type;
    uint8_t val;
    uint8_t timestamp[7];
    uint8_t detail[2];
    METAS(id,type,val,timestamp,detail);
};

struct AlarmInfo
{
    uint8_t num;
    AlarmData alarmData;
    METAS(num,alarmData);
}; //数据类15 报警数据

struct FaultRecord
{
    uint32_t id;
    uint8_t type;
    uint8_t timestamp[7];
    uint8_t action;
    uint8_t detail[2];
    METAS(id,type,timestamp,action,detail);
};

struct Fault
{
    uint8_t num;
    FaultRecord faultRecord;
    METAS(num,faultRecord);
}; //数据类16 故障数据

struct CtrlTable
{
    uint8_t roadId;
    uint8_t phaseStage;
    uint8_t plan;
    uint8_t runningMode;
    uint8_t cycle;
    METAS(roadId,phaseStage,plan,runningMode,cycle);
};

  //数据类17 中心控制
struct CenterCtrl
{
    CtrlTable ctrlTable[ROAD_NUM];
    METAS(ctrlTable);
};

struct Order
{
    uint8_t orderVal[16];
    METAS(orderVal);
};

struct All
{
    DataClass<DeviceInfo,1> device;
    DataClass<BasicInfo,2> basic;
    DataClass<LightInfo,3> light;
    DataClass<PhaseInfo,4> phase;
    DataClass<DetectorInfo,5> detector;
    DataClass<PhaseStageInfo,6> phaseStage;
    DataClass<PhaseSecurityInfo,7> phaseSCY;
    DataClass<EmergencyPriority,8> emrgPrior;
    DataClass<PlanInfo,9> plan;
    DataClass<Transition,10> transition;
    DataClass<SchemeInfo,11> scheme;
    DataClass<ScheduleInfo,12> schedule;
    DataClass<RunningStatus,13> runningStatus;
    DataClass<TrafficData,14> traffic;
    DataClass<AlarmInfo,15> alarm;
    DataClass<Fault,16> fault;
    DataClass<CenterCtrl,17> centerCtrl;
    DataClass<Order,18> order;

    bool lightCheck();
    bool phaseCheck();
    bool phaseStageCheck();
    bool schemeCheck();
    bool planCheck();
    bool scheduleCheck();

	METAS(device,basic,light,phase,detector,phaseStage,phaseSCY,emrgPrior,plan,transition,
            scheme,schedule,runningStatus,traffic,alarm,fault,centerCtrl,order);
};
#pragma pack(pop)
