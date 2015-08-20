#pragma once

/**
*	@���: �õ������ʵ���ǽ������ඨ��Ϊstatic						
*	@����: Haust	
*	@����: 2015/08/20
*
*	��Ҫע��������ȱ�㣺
*	��1�����ʺ����ڶ��̵߳�����´�����������
*	��2���������ʱ������Ҫ���ó�������������������Release;
*		������Ҫ�����������
*
*	��߶��
*		���������������뵽�����ֵ�����ʵ�֣��������ֵ���ʵ������ʵ��
*	���������ߵ�̫�����ȱ�㡣��֮�����������������Ϊ�ڶ����ǿ�����
*	��Ŀ�����Ƶ�д����
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
