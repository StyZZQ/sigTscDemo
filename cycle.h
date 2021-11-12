#pragma once
#include <atomic>
#include "inc.h"
#include "tscdb.h"
#include "gb2017.h"
#include "frame.h"

class Cycle
{
private:
    /* data */
    All all;
    All bakAll;
public:
	

    Cycle(/* args */) 
    ~Cycle() {}

    void UploadTraffic();
    void UploadAlarm();
    void UploadFault();
    bool GetLocalRule(); 
    void StartInit();
    void run(void* args)
private:
    void initAll(All& a); //初始化All对象
};

