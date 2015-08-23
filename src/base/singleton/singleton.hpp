#pragma once

/**
*	@简介: 该单例类的实现是将具体类定义为static						
*	@作者: Haust	
*	@日期: 2015/08/20
*
*	需要注意有两个缺点：
*	（1）不适合于在多线程的情况下创建单件对象
*	（2）程序结束时，若需要调用程序的析构函数，则调用Release;
*		若不需要，则可以无视
*
*	唠叨：
*		以下两种是我能想到的两种单例的实现，但这两种单例实现我着实想
*	不出来两者的太大的优缺点。但之所以能想出来，是因为第二种是看到有
*	项目有类似的写法。
*/

#if 1
template<class T>
class Singleton
{
public:
	static T& Instance()
	{
		if (NULL == instance_)
		{
			instance_ = new T;
		}

		return *instance_;
	}

	static void Release()
	{
		if (NULL != instance_)
		{
			delete instance_;
		}

		instance_ = NULL;
	}
private:
	Singleton(){}
	~Singleton(){}

private:
	static T *instance_;
};

template<class T>
T* Singleton<T>::instance_ = NULL;
#else
template<class T>
class Singleton
{
public:
	static T& Instance()
	{
		if (NULL == single_instance_)
		{
			single_instance_ = new Singleton<T>;
		}

		return single_instance_->instance_;
	}

	static void Release()
	{
		if (NULL != single_instance_)
		{
			delete single_instance_;
		}

		single_instance_ = NULL;
	}
private:
	Singleton(){}
	~Singleton(){}

private:
	T instance_;
	static Singleton<T> *single_instance_;
};

template<class T>
Singleton<T>* Singleton<T>::single_instance_ = NULL;
#endif
