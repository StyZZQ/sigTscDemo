#pragma once
#include <sstream>
#include "gb2017.h"
#include "sqlite3.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/serialize.hpp"
#include "rapidjson/deserialize.hpp"


using namespace rapidjson;

#define MAX_LIGHT_NUM 32
/*
	data操作，rapidjson保存为JSON文件
	打开、关闭、读取、存储
 */
class Tscdb
{
private:
	sqlite3 *db;
	std::string filename;
public:
	Tscdb()
	{
		db = nullptr;
		filename = "/home/gb2017.db";
	}
	~Tscdb()
	{
		Close();
	}

	bool Open() //打开文件
	{
		if (db != nullptr)
		{
			INFO("tsc.db has already been open!");
			return true;
		}
		if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK)
		{
			ERR("open /home/gb2017.db fail!");
			db = nullptr;
			return false;
		}
		sqlite3_exec(db, "begin;", 0, 0, 0);
		return true;
	}
	void Close()//关闭文件操作句柄	
	{
		if (db != nullptr)
		{
			sqlite3_exec(db, "commit;", 0, 0, 0);
			sqlite3_close(db);
			db = nullptr;
		}
	}
	template<typename T>
	bool Store(T& t,const char* tableName )	//存储设备信息
	{
		std::ostringstream ss;
		char *err = nullptr;
		ss << "drop table " << tableName << ";";
	
		if (db == nullptr)
			return false;
	
		std::string sql = serialize(t); //序列化成字符串

		ss << "create table " << tableName <<"(jsonstr text);";	//创建表
		ss << "insert into " << tableName << "(jsonstr)values(' " << sql  << " ');"; //插入JSON字符串

		if (SQLITE_OK != sqlite3_exec(db, ss.str().c_str(), 0, 0, &err))
		{
			
			if (err != nullptr)
			{
				ERR("store table %s fail, error: %s",tableName,err);
				sqlite3_free(err);
			}
			return false;
		}
		return true;
	}

	template<typename T>
	void Load(T& t,const char* tableName)	//读取
	{
		int nRow, nColumn;
		char **dbResult;
		char *err = nullptr;
		std::string str =  std::string("select * from ") + tableName + ";";

		if(db == nullptr)
			return ;
		if (SQLITE_OK != sqlite3_get_table(db, str.c_str(), &dbResult, &nRow, &nColumn, &err))
		{
			if (err != nullptr)
			{
				ERR("load table %s fail, error: %s", tableName, err);
				sqlite3_free(err);
			}
			return;
		}
		// INFO("load %s table nRow = %d, nColumn = %d", tableName, nRow, nColumn);
		str = dbResult[nColumn];
		// std::cout << str << std::endl;

		bool res = deserialize(t,str);
		if(!res)
			ERR("table %s deserialize fail",tableName);
	}

};

