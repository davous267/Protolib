#include "Logger.h"

namespace protolib
{
	std::queue<Logger::Log> Logger::mLogs;
	std::mutex Logger::mLogMutex;
	bool Logger::mLogToFileOnly = false;
	bool Logger::mLoggingEnabled = true;
	bool Logger::mSyncedLogging = false;
	std::ofstream Logger::mLogFile;

	void Logger::resolveLog(const Log& newLog)
	{
		if (mSyncedLogging)
		{
			mLogs.push(newLog);
		}
		else
		{
			if (logToCout())
			{
				outputToStream(std::cout, newLog);
			}

			if (logToFile())
			{
				outputToStream(mLogFile, newLog);
			}
		}
	}

	void Logger::outputToStream(std::ostream& os, const Log& log)
	{
		os << "[File ";
		if (!log.file.empty())
		{
			os << log.file;
		}
		else
		{
			os << "NONE";
		}
		os << "] ";

		os << "[Type " << log.getTypeString() << "] ";

		if (log.variableName.empty())
		{
			os << log.message << std::endl;
		}
		else
		{
			os << log.variableName << " = " << log.message << std::endl;
		}
	}

	void Logger::setLogFile(const std::string& logFileName, bool logToFileOnly)
	{
		std::lock_guard<std::mutex> lock(mLogMutex);
		if (!mLoggingEnabled) { return; }
		
		try
		{
			closeLogFile();
			mLogFile = std::ofstream(logFileName);
			mLogToFileOnly = logToFileOnly;
		}
		catch (std::exception& e)
		{
			std::cout << "[ERROR] During \"Logger::setLogFile\": " << e.what();
		}
	}

	void Logger::syncedOutput()
	{
		std::lock_guard<std::mutex> lock(mLogMutex);
		if (!mLoggingEnabled) { return; }
		
		while (!mLogs.empty())
		{
			if (logToCout())
			{
				outputToStream(std::cout, mLogs.front());
			}

			if (logToFile())
			{
				outputToStream(mLogFile, mLogs.front());
			}

			mLogs.pop();
		}
	}

	void Logger::writeSimpleInfoLog(const std::string& message, const std::string& file, LogType logType)
	{
		std::lock_guard<std::mutex> lock(mLogMutex);
		if (!mLoggingEnabled) { return; }

		Log newLog;
		newLog.message = message;
		newLog.file = file;
		newLog.logType = logType;

		resolveLog(newLog);
	}

	void Logger::writeSimpleBoolLog(bool message, const std::string& condition, const std::string& file, LogType logType)
	{
		std::string newMessage = condition + " is " + (message ? "true" : "false");
		writeSimpleInfoLog(newMessage, file, logType);
	}
}
