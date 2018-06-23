/*
   Logger is a fully static class designed with the aim to 
   simplify logging of events/messages during program execution.
   Logger utilizes mutexes to ensure thread-safety.

(c) 2017 David Kutak
*/

#pragma once
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <string>
#include <queue>
#include <sstream>

namespace protolib
{
	class Logger
	{
	public:
		enum class LogType
		{
			INF = 1,
			WAR,
			ERR,
		};
	private:
		struct Log
		{
			std::string message;
			std::string variableName;
			std::string file;
			LogType logType;

			std::string getTypeString() const
			{
				if (logType == LogType::INF)
				{
					return "INF";
				}
				else if (logType == LogType::WAR)
				{
					return "WAR";
				}
				return "ERR";
			}
		};

		static std::queue<Log> mLogs;
		static std::mutex mLogMutex;
		static bool mLogToFileOnly;
		static bool mLoggingEnabled;
		static bool mSyncedLogging;
		static std::ofstream mLogFile;

		static bool logToCout()
		{
			return !mLogToFileOnly;
		}

		static bool logToFile()
		{
			return mLogFile.is_open();
		}

		static void resolveLog(const Log& newLog);

		static void outputToStream(std::ostream& os, const Log& log);
	public:
		/**
		This function provides Logger with necessary information
		if outputting of logs to file is desired.

		@param logFileName desired file name
		@param logToFileOnly if set to true, logs will be outputted only to file and not to std::cout as well
		*/
		static void setLogFile(const std::string& logFileName, bool logToFileOnly = false);

		/**
		Closes log file and enables outputting of logs to std::cout
		*/
		static void closeLogFile()
		{
			if (mLogFile.is_open())
			{
				mLogFile.close();
				mLogToFileOnly = false;
			}
		}

		/**
		Disables logging, i.e. calls to Logger functions will have no effect
		and e.g. calls to write(...)Log will instantly return
		*/
		static void disableLogging()
		{
			std::lock_guard<std::mutex> lock(mLogMutex);
			mLoggingEnabled = false;
		}

		/**
		Enables logging
		*/
		static void enableLogging()
		{
			std::lock_guard<std::mutex> lock(mLogMutex);
			mLoggingEnabled = true;
		}

		/**
		Disables synced logging
		*/
		static void disableSyncedLogging()
		{
			std::lock_guard<std::mutex> lock(mLogMutex);
			mSyncedLogging = false;
		}

		/**
		Enables synced logging

		When synced logging is enabled, new logs
		are saved to a queue which is emptied during the
		"syncedOutput" call. 
		If synced logging is disabled (by default), logs are
		processed (i.e. sent to output) instantly during write call.
		Synced logging is useful during "heavy loads" since
		it might significantly decrease number of disk accesses
		when log is saved to file.
		*/
		static void enableSyncedLogging()
		{
			std::lock_guard<std::mutex> lock(mLogMutex);
			mSyncedLogging = true;
		}

		/**
		Sends logs in queue to output if synced logging is enabled
		*/
		static void syncedOutput();

		/**
		Writes a simple log

		@param message variable content
		@param variableName variable name
		@param file file which contains the given variable (e.g. main.cpp)
		@param logType type of log
		*/
		template<typename T>
		static void writeSimpleLog(T message, const std::string& variableName,
			const std::string& file = std::string() /*__FILE__*/, LogType logType = LogType::INF)
		{
			std::lock_guard<std::mutex> lock(mLogMutex);
			if (!mLoggingEnabled) { return; }

			std::stringstream ss;
			ss << message;

			Log newLog;
			newLog.message = ss.str();
			newLog.variableName = variableName;
			newLog.file = file;
			newLog.logType = logType;

			resolveLog(newLog);
		}

		/**
		Writes a simple information log

		@param message message to show
		@param file file which contains this write call
		@param logType type of log
		*/
		static void writeSimpleInfoLog(const std::string& message,
			const std::string& file = std::string() /*__FILE__*/, LogType logType = LogType::INF);

		/**
		Writes a simple log with boolean variable

		@param message variable content (e.g. george && bool)
		@param condition how the condition looks (e.g. "george && bool")
		@param file file which contains the given variable (e.g. main.cpp)
		@param logType type of log
		*/
		static void writeSimpleBoolLog(bool message, const std::string& condition,
			const std::string& file = std::string() /*__FILE__*/, LogType logType = LogType::INF);

		/**
		Writes a structured log (e.g. std::vector or array)
		Template type T must support range-based for loop

		@param message variable content (e.g. simply an instance of std::vector class)
		@param variableName variable name
		@param file file which contains the given variable (e.g. main.cpp)
		@param logType type of log
		@param delim how to delimit values of the container during output
		*/
		template<typename T>
		static void writeStructuredLog(T message, const std::string& variableName,
			const std::string& file = std::string() /*__FILE__*/, LogType logType = LogType::INF, char delim = ',')
		{
			std::lock_guard<std::mutex> lock(mLogMutex);
			if (!mLoggingEnabled) { return; }

			std::stringstream ss;
			for (const auto& val : message)
			{
				ss << val << delim << " ";
			}

			Log newLog;
			newLog.message = ss.str();
			newLog.variableName = variableName;
			newLog.file = file;
			newLog.logType = logType;

			resolveLog(newLog);
		}
	};
}
