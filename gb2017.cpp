#include "gb2017.h"
#include "exchange.h"

using namespace GB2017;

ErrorCheckStatus GBLightInfo::check() const
{
    for (size_t i = 0; i < num; i++) {
    
        if (conf[i].id == 0 || conf[id] > MAX_GB2017_LIGHT_NUM)
            return ERROR60;
        if (conf[i].type < LT_Vehicle || conf[i].type > LT_Special) //灯组类型判断 
            return ERROR61;
        if (ctrl[i].shielding > 1 || ctrl[i].forbidden > 1)         //屏蔽值，禁止值判断
            return ERROR51;
    }
    return RIGHT;
}               
const GBPhaseConf* GBLightInfo::findconf(uint8_t lightId) const
{
    for (size_t i = 0; i < num; i++) {  //找到指定id的配置
        if (conf[i].id == lightId)
            return &conf[i];
    }
    return nullptr;
    
}
const GBLightCtrl* GBLightInfo::findctrl(uint8_t lightId) const
{
    for (size_t i = 0; i < num; i++) {  
        if (ctrl[i].id == lightId)
            return &ctrl[i];
    }
    return nullptr;
}

//返回true表示检查无误,返回false表示检查有误
static inline bool checkColor(uint8_t color)  //判断灯色设置
{
   //失去路权
    switch (color) {
        case 0:
        case LS_Off:
        case LS_Green:
        case LS_GreenFlash:
        case LS_GreenFastFlash:
        case LS_Yellow:
        case LS_YellowFlash:
        case LS_YellowFastFlash:
        case LS_RedYellow: 
        case LS_Red:
        case LS_RedFlash:
        case LS_RedFastFlash: return true;
        default: break;
    }
    return false;
}


ErrorCheckStatus GBPhaseInfo::check(const All *all) const
{
    uint64_t phaseBootAcquireGreenBits = 0; //相位开机获取路权有绿灯或绿闪时置位
    uint64_t phaseBootLostGreenBits = 0;    //相位开机失去路权有绿灯或绿闪时置位
    auto IsGreen = [](uint8_t color)->bool{
        return (color == LS_Green || color == LS_GreenFlash || color == LS_GreenFastFlash);
    };
    for (size_t i = 0; i < num; i++)
    {
        if (conf[i].id == 0 || conf[i].id > MAX_GB2017_PHASE_NUM)
            return ERROR62;
        uint64_t lights = common::ntoh(conf[i].lights);
        //检查关联灯组id
        for (int j = 0; j < 64; j++) {
            //lights对于bit位，从0开始， findConf查找id对应j+1
            if (GET_BIT(lights, j) == 1 && all->phase.info.findConf(j + 1) == nullptr)
                return ERROR63;
        }

        if (!checkColor(conf[i].acquireColor1) || !checkColor(conf[i].acquireColor2) || !checkColor(conf[i].acquireColor3) ||
            !checkColor(conf[i].lostColor1) || !checkColor(conf[i].lostColor2) || !checkColor(conf[i].lostColor3) ||
            !checkColor(conf[i].bootAcquireColor1) || !checkColor(conf[i].bootAcquireColor2) || !checkColor(conf[i].bootAcquireColor3) ||
            !checkColor(conf[i].bootLostColor1) || !checkColor(conf[i].bootLostColor2) || !checkColor(conf[i].bootLostColor3) )
            return ERROR54;

        //检查最小绿，最大绿1，最大绿2的大小关系
        uint16_t minGreen = common::ntoh(conf[i].minGreen);
        uint16_t maxGreen1 = common::ntoh(conf[i].maxGreen1);
        uint16_t maxGreen2 = common::ntoh(conf[i].maxGreen2);
        if (minGreen > maxGreen1 || minGreen > maxGreen2 || maxGreen1 > maxGreen2)
            return ERROR55;
        //检查屏蔽值，禁止值
        if (ctrl[i].shielding > 1 || ctrl[i].forbidden > 1)
            return ERROR52;
        
        //判断开机获取路权灯色为绿灯或绿闪，记录相位id
        if (IsGreen(conf[i].bootAcquireColor1) || IsGreen(conf[i].bootAcquireColor2) || IsGreen(conf[i].bootAcquireColor3))
            SET_BIT(phaseBootAcquireGreenBits, conf[i].id - 1);
        //判断开机失去路权灯色为绿灯或绿闪，记录相位id
        if (IsGreen(conf[i].bootLostColor1) || IsGreen(conf[i].bootLostColor2) || IsGreen(conf[i].bootLostColor3))
            SET_BIT(phaseBootLostGreenBits, conf[i].id - 1);
    }

    //检查相位绿冲突，2个相位开机获得路权灯色类型或开机失去路权灯色类型均配置为绿灯或绿闪，判断为绿冲突
    if (all->phaseSecurity.info.isConflict(phaseBootAcquireGreenBits))
        return ERROR56;
    if (all->phaseSecurity.info.isConflict(phaseBootLostGreenBits))
        return ERROR57;
    return RIGHT;
}
const PhaseConf* GBPhaseInfo::findConf(uint8_t phaseId) const
{
    for (size_t i = 0; i < num; i++) {  
        if (conf[i].id == phaseId)
            return &conf[i];
    }
    return nullptr;
}
const PhaseCtrl* GBPhaseInfo::findCtrl(uint8_t phaseId) const
{
    for (size_t i = 0; i < num; i++) {  
        if (ctrl[i].id == phaseId)
            return &ctrl[i];
    }
    return nullptr;
}

ErrorCheckStatus GBPhaseStageInfo::check(const All *all) const
{
    for (uint8_t i = 0; i < num; i++) {
        if (conf[i].id == 0 || conf[i].id > MAX_GB2017_PHASESTAGE_NUM)  //检查相位id
            return ERROR64; 
        if (ctrl[i].shielding > 1 || ctrl[i].forbidden > 1 || ctrl[i].softwareReq > 1)  //检查屏蔽值、禁止值、软件需求
            return ERROR53;
        if (all->phaseSecurity.info.isConflict(common::ntoh(conf[i].phases)))   //检查相位阶段中是否包含冲突相位
            return ERROR59;
    }
    return RIGHT;
}

const GBPhaseStageConf* GBPhaseStageInfo::findConf(uint8_t stageId) const
{
    for (uint8_t i = 0; i < num; i++) {
        if (conf[i].id == stageId)
            return &conf[i];
    }
    return nullptr;
}

const GBPhaseStageCtrl* GBPhaseStageInfo::findCtrl(uint8_t stageId) const
{
    for (uint8_t i = 0; i < num; i++) {
        if (ctrl[i].id == stageId) 
            return &ctrl[i];
    }
    return nullptr;
}

ErrorCheckStatus GBPhaseSecurity::check(const All *all) const
{
    //检查相位冲突序列中是否包含自身
    for (uint8_t i = 0; i < MAX_GB2017_PHASE_NUM; i++) {
        auto conflicts = common::ntoh(conflict[i].conflicts);
        if (GET_BIT(conflicts, conflict[i].id - 1) == 1)
            return ERROR58;
    }

    uint64_t phaseBootAcqurebits = 0;   //开机启动获取路权相位
    uint64_t phaseBootLostbits = 0;     //开启启动失去路权相位
    auto IsGreen = [](uint8_t color)->bool {
        return (color == LS_Green || color == LS_GreenFastFlash || color == LS_GreenFlash);
    };

    const auto &phase = all->phase.info; 
    for (int i = 0; i < phase.num; i++)
    {
        if (IsGreen(phase.conf[i].bootAcquireColor1) || IsGreen(phase.conf[i].bootAcquireColor2) || IsGreen(phase.conf[i].bootAcquireColor3) )
            SET_BIT(phaseBootAcqurebits, phase.conf[i].id - 1);
        if (IsGreen(phase.conf[i].bootLostColor1) || IsGreen(phase.conf[i].bootLostColor2) || IsGreen(phase.conf[i].bootLostColor3) )
            SET_BIT(phaseBootLostbits, phase.conf[i].id - 1);
    }
    //检查开机获取路权相位存在冲突  
    if(isConflict(phaseBootAcqurebits)) 
        return ERROR56;
    //检查开机失去路权相位存在冲突
    if(isConflict(phaseBootAcqurebits)) 
        return ERROR57;
    //检查相位阶段包含相位是否存在冲突
    const auto &phaseStage = all->phaseStage.info;
    for (uint8_t i = 0; i < phaseStage.num; i++) {
        if (isConflict(common::ntoh(phaseStage.conf[i].phases)))
            return ERROR59;
    }
}
bool GBPhaseSecurity::isConflict(uint64_t phaseBits) const;
{
    //检查是否时冲突相位
    for (uint8_t i = 0; i < MAX_GB2017_PHASE_NUM; i++) {
        //若使用相位，则用相位的bit & 冲突相位序 > 0 则表示有冲突
        if (GET_BIT(phaseBits, i) == 1 && (phaseBits & common::ntoh(conflict[i].conflicts)) > 0)
            return true;
    }
    return false;
}

ErrorCheckStatus GBSchemeConf::check() const
{
    //方案id有误
    if (id == 0 || id > MAX_GB2017_SCHEDULE_NUM)
        return ERROR65;
    //控制路口id有误
    if (roadId == 0 || roadId > MAX_ROAD_NUM)
        return ERROR66;
    
    uint32_t sum = 0;           //计算的周期时间
    for (size_t i = 0; i < 16; i++)
    {
        if (get(phaseStageList, i) == 0)
            break;   
        sum += get(phaseStageListTime, i);  //累加相位阶段链时间
        uint8_t type = get(phaseStageType, i);
        if (type != PST_Fix && type != PST_Demand)  //检查相位阶段出现类型
            return ERROR67;
    }
    if (sum != common::ntoh(cycle))     //检查相位阶段链时间和与周期不等
        return ERROR68;
    
    return RIGHT;

}

const GBSchemeConf *GBSchemeInfo::findConf(uint8_t schemeId) const
{
    for (uint8_t i = 0; i < num; i++) {
        if (conf[i].id == schemeId)
            return &conf[i];
    }
    return nullptr;
}

ErrorCheckStatus GBPlanConf::check() const
{
    //时段计划id有误
    if (id == 0 || id > MAX_GB2017_SCHEDULE_NUM)
        return ERROR6C;
    //控制路口id有误
    if (roadId == 0 || roadId > MAX_ROAD_NUM)
        return ERROR6D;
    if (get(startTimeList, 0) != 0)
        return ERROR6E;    //第一个时段必须是00:00

    for (int i = 0; i < 48; i++)
    {
        uint16_t t = GET(startTimeList, i);
        if (i != 0 && t == 0) //除第1个时段，时间为0，退出
            break;
        const TimeInterval *ti = (const TimeInterval *)&t; //转换为TimerInterval
        if (ti->hour > 23 || ti->min > 59)
            return ERROR6F;
        if (get(schemeList, i) == 0)    //检查有时段，但是对应方案id为0
            return ERROR70;
        if (!modeCheck(get(runningMode, i))) //检查运行模式
            return ERROR71;
        
        if (i < 47 && get(schemeList, i + 1) != 0) {
            uint16_t v2 = get(startTimeList, i + 1); //获取下一阶段时间
            const TimeInterval *ti2 = (const TimeInterval *)&v2;
            if (ti->hour > ti2->hour || (ti->hour == ti2->hour && ti->min >= ti2->min))
                return ERROR5A;   
        } //检查下一时段小于等于上一时段, 最后一个阶段不检查      
    }
    
    return RIGHT;
}
ErrorCheckStatus GBSchemeInfo::check(const All *all, bool onlyCheckRelated = false) const
{

}

ErrorCheckStatus GBPlanInfo::check(const All *all, bool onlyCheckRelated = false) const
{

}
const GBPlanConf *GBPlanInfo::findConf(uint8_t planId) const
{

}

ErrorCheckStatus GBScheduleConf::check() const
{

}

ErrorCheckStatus GBScheduleInfo::check(const All *all, bool onlyCheckRelated = false) const
{

}


ErrorCheckStatus All::lightCheck()
{
    auto ret = light.info.check();
    return (ret == RIGHT) ? phase.info.check(this) : ret;
}
ErrorCheckStatus All::phaseCheck()
{
    auto ret = phase.info.check();
    return (ret == RIGHT) ? scheme.info.check(this, true) : ret;
}
ErrorCheckStatus All::phaseStageCheck()
{
    auto ret = phaseStage.info.check();
    return (ret == RIGHT) ? scheme.info.check(this, true) : ret;
}
ErrorCheckStatus All::phaseSecurityCheck()
{
    return phaseSecurity.info.check();
}
ErrorCheckStatus All::schemeCheck()
{
    auto ret = scheme.info.check();
    return (ret == RIGHT) ? plan.info.check(this, true) : ret;
}
ErrorCheckStatus All::planCheck()
{
    auto ret = plan.info.check();
    return (ret == RIGHT) ? schedule.info.check(this, true) : ret;    
}
ErrorCheckStatus All::scheduleCheck()
{
    return schedule.info.check(this);
}