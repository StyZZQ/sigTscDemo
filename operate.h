#pragma once
#include <vector>
#include <pthread.h>
#include "tscdb.h"
#include "value.h"
#include "gb2017.h"

typedef pthread_rwlock_t RWLOCK;    

class Operate
{   
public:
    int     eleLen;                 //元素值长度，全表查询时数据长度为0时使用
    int     dataNum;                //统计操作数据量
    Data    *rightData[UCHAR_MAX];  //正确返回数据
    Data    *errorData;             //错误返回数据
private:
    All     all;                    //当前所有数据表
    All     bakAll;                 //备份所有数据表
    Root    root;
    RWLOCK  rwlock;                 //读写锁
public:
    Operate(/* args */)
    {	//初始化
        dataNum = 0;
#ifdef __linux__
			pthread_rwlock_init(&rwlock, NULL);
#else	//__WIN32__
			InitializeSRWLock(&rwlock); //初始化
#endif
        Tscdb db;
        if (!db.Open())
            return;
        db.Load(all.device.info, "device");
        db.Load(all.basic.info, "basic");
        db.Load(all.light.info, "light");
        db.Load(all.phase.info, "phase");
        db.Load(all.phaseStage.info, "phaseStage");
        db.Load(all.scheme.info, "scheme");
        db.Load(all.plan.info, "plan");
        db.Load(all.schedule.info, "schedule");
        db.Load(all.traffic.info, "traffic");
        db.Load(all.alarm.info, "alarm");
        db.Load(all.fault.info, "fault");
        db.Close();
        bakAll = all;
        memset(rightData, 0, UCHAR_MAX);
        errorData = nullptr;
        Assist::generate(root, bakAll);  //以备份的All对象生成一个Root对象，以此为地址更新数据
    }

    bool setData(int num , Data** val);     //设置数据值
    bool readData(int num , Data** val);    //读取数据值    
    ErrorCheckStatus checkConfig(int& classId); //检查配置
    void updateConfig();                    //更新配置到数据库
    void clear();                           //初始化变量
};
