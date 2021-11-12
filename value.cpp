#include <string.h>
#include <assert.h>
#include "value.h"

static Value null;

inline const Value & Root::operator[](int classId) const
{
	for (int i = 0; i < count; i++) {
		if (children[i].classId == classId)
			return children[i];
	}
	return null;
}

inline Value & Root::operator[](int classId)
{
	for (int i = 0; i < count; i++) {
		if (children[i].classId == classId)
			return children[i];
	}
	return null;
}

QueryResult query(const Data &data, const Root &root, int& eleLen)
{
	const Value &dc = root[data.classId];
	if (!dc) {
		return QueryResult(Status_Null, nullptr);	//数据类ID不存在
	}
	if (data.objId == 0 || data.objId > dc.count) {
		return QueryResult(Status_Null, nullptr);	//对象ID不存在
	}
	const Value &obj = dc.children[data.objId - 1];
	if (obj.count > 0) {	//说明对象有多个成员或者是对象数组
		if (obj.isArray) {	//说明是对象数组
			if (data.attrId > obj.children[0].count)
				return QueryResult(Status_Null, nullptr);	//属性ID不存在或者数组元素ID不存在
		} else {	//说明是单个对象
			if (data.attrId > obj.count || data.eleId > 0)
				return QueryResult(Status_Null, nullptr);	//属性ID不存在或者数组元素ID不存在
		}
	} else {	//说明此对象是单个数值
		if (data.attrId > 0 || data.eleId > 0)
			return QueryResult(Status_Null, nullptr);	//属性ID不存在或者数组元素ID不存在
	}

	int num = (dc.count > 1 && dc.children[0].size == 1 && obj.isArray) ? (*(uint8_t *)(dc.children[0].addr)) : 0;
	QueryResult ret;
	int vlen = 0;
	if (data.attrId == 0 && data.eleId == 0) {	//获取整个对象或者对象数组
		vlen = obj.isArray ? (num * obj.children[0].size) : obj.size;
		
		ret.data = (Data *)malloc(sizeof(Data) + vlen);
		assert(ret.data != nullptr);
		memcpy(ret.data, &data, sizeof(Data));
		memcpy(ret.data->value, obj.addr, vlen);
	} else if (data.attrId == 0 && data.eleId > 0) {	//获取对象数组中的某个对象
		//assert(obj.isArray);	只可能是对象数组
		for (int i = 0; i < num; i++) {
			const Value &ele = obj.children[i];
			uint8_t id = *(uint8_t *)ele.children[0].addr;
			if (id == data.eleId) {
				vlen = ele.size;
				ret.data = (Data *)malloc(sizeof(Data) + vlen);
				assert(ret.data != nullptr);
				memcpy(ret.data, &data, sizeof(Data));	
				memcpy(ret.data->value, ele.addr, ele.size);
				break;
			}
		}
		if (vlen == 0)
			return QueryResult(Status_Null, nullptr);	//数组元素ID不存在
	} else if (data.attrId > 0 && data.eleId == 0) {	//获取对象的固定成员或者获取对象数组中的每个对象的固定成员
		if (obj.isArray) {
			vlen = num * obj.children[0].children[data.attrId - 1].size;
			ret.data = (Data *)malloc(sizeof(Data) + vlen);
			assert(ret.data != nullptr);
			memcpy(ret.data, &data, sizeof(Data));
			for (int i = 0; i < num; i++) {
				const Value &ele = obj.children[i].children[data.attrId - 1];
				memcpy(ret.data->value + i * ele.size, ele.addr, ele.size);
			}
		} else {
			const Value &ele = obj.children[data.attrId - 1];
			vlen = ele.size;
			ret.data = (Data *)malloc(sizeof(Data) + vlen);
			assert(ret.data != nullptr);
			memcpy(ret.data, &data, sizeof(Data));	
			memcpy(ret.data->value, ele.addr, ele.size);
		}
	} else {	//获取对象数组中某个对象的固定成员
		//assert(obj.isArray);	只可能是对象数组
		for (int i = 0; i < num; i++) {
			const Value &child = obj.children[i];
			uint8_t id = *(uint8_t *)child.children[0].addr;
			if (id == data.eleId) {
				const Value &ele = child.children[data.attrId - 1];
				vlen = ele.size;
				ret.data = (Data *)malloc(sizeof(Data) + vlen);
				assert(ret.data != nullptr);
				memcpy(ret.data, &data, sizeof(Data));	
				memcpy(ret.data->value, ele.addr, ele.size);
				break;
			}
		}
		if (vlen == 0)
			return QueryResult(Status_Null, nullptr);	//数组元素ID不存在
	}
	ret.data->len = (vlen + 4 > 255) ? 0 : (uint8_t)(vlen + 4);	//如果数据值长度超过255则赋值为0
	eleLen = vlen;				//获取实际元素值长度
	return std::move(ret);
}

ErrorValueStatus set(const Data &data, Root &root)
{
	if (data.len < 5) {
		return Status_BadValue;	//数据值长度必须大于等于5
	}
	Value &dc = root[data.classId];
	if (!dc) {
		return Status_Null;	//数据类ID不存在
	}
	if (data.objId == 0 || data.objId > dc.count) {
		return Status_Null;	//对象ID不存在
	}
	Value &obj = dc.children[data.objId - 1];
	if (obj.count > 0) {	//说明对象有多个成员或者是对象数组
		if (obj.isArray) {	//说明是对象数组
			if (data.attrId > obj.children[0].count || data.eleId > obj.count)
				return Status_Null;	//属性ID不存在或者数组元素ID不存在
		} else {	//说明是单个对象
			if (data.attrId > obj.count || data.eleId > 0)
				return Status_Null;	//属性ID不存在或者数组元素ID不存在
		}
	} else {	//说明此对象是单个数值
		if (data.attrId > 0 || data.eleId > 0)
			return Status_Null;	//属性ID不存在或者数组元素ID不存在
	}

	int num = (dc.count > 1 && dc.children[0].size == 1 && obj.isArray) ? (*(uint8_t *)(dc.children[0].addr)) : 0;
	int vlen = data.len - 4;
	if (data.attrId == 0 && data.eleId == 0) {	//设置整个对象或者对象数组
		if (obj.isArray) {
			if (vlen > obj.size || (vlen % obj.children[0].size) != 0)
				return Status_WrongLength;
			if (dc.count > 1 && dc.children[0].size == 1) {	//写入数组元素个数
				uint8_t n = (uint8_t)(vlen / obj.children[0].size);
				memcpy(dc.children[0].addr, &n, 1);
			}
		} else {
			if (vlen != obj.size)
				return Status_WrongLength;
		}
		memcpy(obj.addr, data.value, vlen);
	} else if (data.attrId == 0 && data.eleId > 0) {	//设置对象数组中的某个对象
		//assert(obj.isArray);	只可能是对象数组
		if (vlen != obj.children[0].size)
			return Status_WrongLength;
		for (int i = 0; i < num; i++) {
			Value &ele = obj.children[i];
			uint8_t id = *(uint8_t *)ele.children[0].addr;
			if (id == data.eleId) {
				memcpy(ele.addr, data.value, vlen);
				return OK;
			}
		}
		return Status_Null;	//数组元素ID不存在
	} else if (data.attrId > 0 && data.eleId == 0) {	//设置对象的固定成员或者设置对象数组中的每个对象的固定成员
		if (obj.isArray) {
			int attrSize = obj.children[0].children[data.attrId - 1].size;
			if (vlen > num * attrSize || (vlen % attrSize) != 0) {
				return Status_WrongLength;
			}
			for (int i = 0; i < num && vlen > 0; i++) {
				Value &ele = obj.children[i].children[data.attrId - 1];
				memcpy(ele.addr, data.value + i * ele.size, ele.size);
				vlen -= ele.size;
			}
		} else {
			Value &ele = obj.children[data.attrId - 1];
			if (vlen != ele.size) {
				return Status_WrongLength;
			}
			memcpy(ele.addr, data.value, vlen);
		}
	} else {	//设置对象数组中某个对象的固定成员
		//assert(obj.isArray);	只可能是对象数组
		if (vlen != obj.children[0].children[data.attrId - 1].size)
			return Status_WrongLength;
		for (int i = 0; i < num; i++) {
			Value &child = obj.children[i];
			uint8_t id = *(uint8_t *)child.children[0].addr;
			if (id == data.eleId) {
				Value &ele = child.children[data.attrId - 1];
				memcpy(ele.addr, data.value, vlen);
				return OK;
			}
		}
		return Status_Null;	//数组元素ID不存在
	}
	return OK;
}