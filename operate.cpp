#include "operate.h"

bool Operate::setData(int num, Data** data)
{

    for(int i = 0 ; i < num ; i++) {
        //符合以下条件的，可设置
        if (data[i].classId == 1 && data[i].objId == 5  || data[i].classId == 2 && data[i].objId != 7 ||
            (data[i].classId == 3 && (data[i].objId == 2 && data[i].attrId == 2 || data[i].objId == 4 && (data[i].attrId == 2|| data[i].attrId == 3))) ||
            (data[i].classId == 4 && (data[i].objId == 2 || data[i].objId == 3) && data[i].attrId != 1) ||
            (data[i].classId == 5 && data[i].objId == 2  && data[i].attrId != 1) ||
            (data[i].classId == 6 && (data[i].objId == 2 || data[i].objId == 4) && data[i].attrId != 1) ||
            (data[i].classId == 7 && (data[i].objId == 1 || data[i].objId == 2) && data[i].attrId != 1) ||
            (data[i].classId == 8 && (data[i].objId == 2 || data[i].objId == 5) && data[i].attrId != 1) ||
            (data[i].classId == 9 && data[i].objId == 2  && data[i].attrId != 1) ||
            (data[i].classId == 10 && data[i].objId == 1  && data[i].attrId == 2) ||
            (data[i].classId == 11 && data[i].objId == 2  && data[i].attrId != 1) ||
            (data[i].classId == 12 && data[i].objId == 2  && data[i].attrId != 1) ||
            (data[i].classId == 13 && data[i].objId == 1  && (data[i].attrId == 10 || data[i].attrId == 11) ) ||
            (data[i].classId == 17 && data[i].objId == 1  && data[i].attrId != 1) ||
            (data[i].classId == 18 && data[i].objId == 1)) {

            auto status = set(*data[i], root);  //设置值,获取返回值状态
            if(status == OK) {              
                ErrorCheckStatus cStatus = checkConfig(data[i].classId); //检查当前数据类配置是否正确
                if (cStatus != OK) {        //检查状态异常
                    dataNum = 1;
                    errorData = data[i];
                    errorData->len = 5 ;
                    errorData->value[0] = cStatus; //值状态
                    return false;
                } else {
                    ++dataNum;
                    rightData[i] = data[i];
                    rightData[i]->len = 5;      //设置返回的数据值长度固定为5
                    rightData->value[0] = OK;  
                    continue;
                }
            } else {                        //返回值值状态异常
                dataNum = 1;
                errorData = data[i];
                errorData->len = 5 ;
                errorData->value[0] = ret.status; //值状态
                return false;
            }
        } else {    //除以上条件，只读属性
            dataNum = 1;
            errorData = data[i];
            errorData->len = 5 ;
            errorData->value[0] = Status_ReadOnly;  
            return false;
        }
    }
    updateConfig();    //以上检查都正常，则更新配置到数据库
    return true
}

void Operate::readData(int num ,  Data** data)
{
    for(int i = 0 ; i < num ; i++) {
        QueryResult ret = query(*data[i], root, eleLen);
        if(ret.status == OK) {  //查询正确返回，直接拷贝data
            ++dataNum;
            rightData[i] = ret.data;   
            continue;
        } else {               //查询错误返回，value拷贝错误值，len置为5，其余拷贝
            dataNum = 1; 
            errorData = data[i];
            errorData->len = 5;        
            errorData->value[0] = ret.status;
            return false;
        }   
    }
    return true;
}
ErrorCheckStatus Operate::checkConfig(int& classId)
{
    switch (classId)
    {
    case 3:
        return bakAll.lightCheck();   
    case 4:
        return bakAll.phaseCheck(); 
    case 6:
        return bakAll.phaseStageCheck(); 
    case 9:
        return bakAll.schemeCheck(); 
    case 11:
        return bakAll.planCheck(); 
    case 12:
        return bakAll.scheduleCheck(); 
    default:
        break;
    }
    return OK;
}

void Operate::updateConfig()
{
    Tscdb db;
	db.Open();
	pthread_rwlock_wrlock(&rwlock);
    db.Store();
    db.Store();
    db.Store();
    db.Store();

	pthread_rwlock_unlock(&rwlock);
	db.Close();

}

void Operate::clear()
{
    dataNum = 0;
    memset(rightData,0,sizeof(Data) * UCHAR_MAX);
    memset(errorData,0,sizeof(Data));
}