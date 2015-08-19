#pragma once

#include <stdio.h>

#define CompareOperatorDefine(T, oper)	\
	bool operator##oper(const T& rh)	\
	{								\
	return this->value_ oper rh.value_;	\
	}								\

#define CompareOperatorDefine2(T, t, oper)	\
	bool operator##oper(const t value )	\
	{								\
	return this->value_ oper value;	\
	}								\

#define SelfOperDefine(T, oper)	\
	T operator##oper(int)		\
	{							\
		T ref(this->value_);			\
		oper this->value_;			\
		return ref;				\
	}						\

#define SelfOperDefine2(T, oper)	\
	T& operator##oper()			\
{							\
	oper this->value_;			\
	return *this;			\
}							\

#define InterOperDefine(T, t, oper)	\
	T& operator##oper(t inter)	\
{								\
	this->value_ oper inter;			\
	return *this;				\
}								\

#define TypeDefine(T, t, f)					\
class T:public BaseType					\
{										\
public:									\
	T(){};								\
	T(t value){ value_ = value; };		\
	~T(){}								\
	\
	virtual const char *name()			\
{								\
	static char name[40];			\
	sprintf(name, "%s", #T);		\
	return name;						\
}									\
	\
	virtual const char *toString()		\
{									\
	static char valuestr[20];			\
	sprintf(valuestr, "%%%s", #f);	\
	sprintf(valuestr, valuestr, value_);\
	return valuestr;				\
}									\
	\
	SelfOperDefine(T, ++)\
	SelfOperDefine(T, --)	\
	SelfOperDefine2(T, ++)	\
	SelfOperDefine2(T, --)	\
	\
	InterOperDefine(T, t, +=)	\
	InterOperDefine(T, t, -=)	\
	\
	CompareOperatorDefine(T, >)	\
	CompareOperatorDefine(T, <)	\
	CompareOperatorDefine(T, ==) \
	CompareOperatorDefine(T, >=)\
	CompareOperatorDefine(T, <=)\
	CompareOperatorDefine(T, !=)\
	CompareOperatorDefine2(T, t, >)\
	CompareOperatorDefine2(T, t, <)\
	CompareOperatorDefine2(T, t, ==)\
	CompareOperatorDefine2(T, t, >=)\
	CompareOperatorDefine2(T, t, <=)\
	CompareOperatorDefine2(T, t, !=)\
	\
private:								\
	t value_;							\
};										\

class BaseType
{
public:
	BaseType(){}
	~BaseType(){}

	virtual const char *name() { return "BaseType"; }
	virtual const char *toString(){ return ""; }
};

TypeDefine(Integer, int, d);
TypeDefine(Character, char, c);
TypeDefine(Float, float, f);
TypeDefine(Short, short, d);
TypeDefine(Long, long, ld);
TypeDefine(Double, double, g);
