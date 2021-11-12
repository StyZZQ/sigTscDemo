#pragma once

#include <type_traits>
#include <tuple>
#include <initializer_list>
#include <stdlib.h>

#pragma pack(push,1)
template<typename T, int CI>
struct DataClass {
    T info;
    operator T &() {return info;}
};
#pragma pack(pop)

struct Value {
	void *addr = nullptr;
	int size = 0;
	struct Value *children = nullptr;
	int count = 0;
	bool isArray = false;
	uint8_t classId = 0;

	Value() = default;
	~Value()
	{
		if (children != nullptr) {
			delete []children;
			children = nullptr;
		}
	}
	explicit operator bool() const { return addr != nullptr; }
};

struct Root : Value {
	const Value & operator[](int classId) const;
	Value & operator[](int classId);
};

template<int... Indexs>
struct tuple_index{};

template<int N, int... Indexs>
struct make_tuple_index : make_tuple_index<N - 1, N - 1, Indexs...>{};
template<int... Indexs>
struct make_tuple_index<0, Indexs...>
{
	using type = tuple_index<Indexs...>;
};

struct Assist {
	//针对整数数组
	template<typename T, std::size_t N>
	static void generate(Value &value, const T (&arr)[N], typename std::enable_if<std::is_integral<T>::value, int>::type = 0)
	{
		value.addr = (void *)arr;
		value.size = sizeof(arr);
	}
	//针对结构体数组
	template<typename T, std::size_t N>
	static void generate(Value &value, const T (&arr)[N], typename std::enable_if<std::is_class<T>::value, int>::type = 0)
	{
		value.addr = (void *)arr;
		value.size = sizeof(arr);
		value.children = new Value[N];
		value.count = N;
		value.isArray = true;
		for (int i = 0; i < N; i++) {
			generate(value.children[i], arr[i]);
		}
	}
	//针对整数类型
	template<typename T>
	static void generate(Value &value, const T & t, typename std::enable_if<std::is_integral<T>::value, int>::type = 0)
	{
		value.addr = (void *)&t;
		value.size = sizeof(t);
	}
	//DataClass
	template<typename T, int CI>
	static void generate(Value &value, const DataClass<T, CI> &dc)
	{
		value.classId = (uint8_t)CI;
		generate(value, dc.info);
	}
	//遍历类的Metas分别对类中的成员生成value
	template<int... Indexs, typename TP>
	static void foreach_tuple(Value &value, tuple_index<Indexs...> ids, TP && metas)
	{
		std::initializer_list<int>{(generate(value.children[Indexs], std::get<Indexs>(metas)), 0)...};
	}
	//针对struct类型
	template<typename T>
	static void generate(Value &value, const T & t, typename std::enable_if<std::is_class<T>::value, int>::type = 0)
	{
		value.addr = (void *)&t;
		value.size = sizeof(t);
		value.children = new Value[t.element_size];
		value.count = t.element_size;
		foreach_tuple(value, typename make_tuple_index<T::element_size>::type(), t.Metas());
	}
};

#pragma pack(push,1) 
struct Data {
	uint8_t index;
	uint8_t len;
	uint8_t classId;
	uint8_t objId;
	uint8_t attrId;
	uint8_t eleId;
	char value[0];
};
#pragma pack(pop)

enum ErrorValueStatus
{
	OK = 0,							//正确
	Status_BadValue = 0x10,			//值错误,用于整型数据,数据值不在规定范围内的返回值
	Status_WrongLength = 0x11,		//值长度错误
	Status_OverFlow = 0x12,			//值越界
	Status_ReadOnly = 0x20,			//值只读
	Status_Null = 0x30,				//值不存在
	Status_Error = 0x40,			//值一般错误
	Control_Fail = 0x50,			//值一般错误
	Other_Error = 0x60,				//其他错误
};

struct QueryResult {
	ErrorValueStatus status = OK;
	Data *data = nullptr;
	QueryResult() = default;
	QueryResult(ErrorValueStatus st, Data *d) : status(st), data(d) {}
	QueryResult(const QueryResult &) = delete;
	QueryResult(QueryResult && q)
	{
		status = q.status;
		data = q.data;
		q.data = nullptr;
	}
	QueryResult & operator=(const QueryResult &) = delete;
	QueryResult & operator=(QueryResult &&q)
	{
		if (data != nullptr) {
			free(data);
			data = nullptr;
		}
		status = q.status;
		data = q.data;
		q.data = nullptr;
		return *this;
	}
	~QueryResult()
	{
		if (data != nullptr) {
			free(data);
			data = nullptr;
		}
	}
};

extern QueryResult query(const Data &data, const Root &root, int& eleLen);
extern ErrorValueStatus set(const Data &data, Root &root);