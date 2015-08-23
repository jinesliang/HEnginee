#include "logger.hpp"

Logger::Logger(void)
{
}


Logger::~Logger(void)
{
}

void Logger::Initialize(string path, LogLevel log_level /* = LOG_WARNING */, int max_size /* = 50*1024*1024 */)
{
	log_path_ = path;
	log_level_ = log_level;
	max_size_ = max_size;
}

void Logger::LogTrace(LogLevel log_level, char *file_name, int line, char *format, ...)
{

}

void Logger::Flush()
{
	if (NULL != log_fp_)
	{
		fflush(log_fp_);
	}
}

void Logger::NewDayLog()
{
	log_sequence_ = NULL;
	//log_path_.append()
}

