#pragma once

#include <string>
#include <stdio.h>

#include "singleton/singleton.hpp"

enum LogLevel
{
	LOG_DEBUG = 0,
	LOG_WARNING = 1,
	LOG_ERROR = 2,
	LOG_INFO = 3,
};

class Logger
{
public:
	Logger(void);
	virtual ~Logger(void);

	inline LogLevel GetLogLevel() { return log_level_; } 
	inline void SetLogLevel(LogLevel log_level) { log_level_ = log_level; }

	void Initialize(std::string path, LogLevel log_level = LOG_WARNING, int max_size = 50*1024*1024);

	void LogTrace(LogLevel log_level, char *file_name, int line, char *format, ...);

	void Flush();

	void NewDayLog();
private:
	void OpenLog();

	void CloseLog();
private:	
	std::string log_path_;
	LogLevel log_level_;
	int max_size_;
	int log_sequence_;
	FILE* log_fp_;
};

#define LogAdapter Singleton<Logger>::Instance()
