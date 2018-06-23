#include "ArgsParser.h"
#include <exception>

namespace protolib
{
	void ArgsParser::initOptionsMap(int argc, const char *const *const argv)
	{
		std::string lastOption;
		std::vector<std::string> lastOptionArgs;
		std::string tmp;

		for (int i = 1; i < argc; ++i)
		{
			tmp = std::string(argv[i]);
			if (tmp.empty())
			{
				throw std::invalid_argument("Command line argument is empty!");
			}

			if (tmp[0] == mOptionsPrefix)
			{
				if (!lastOption.empty())
				{
					mOptions.insert({ lastOption, lastOptionArgs });
				}

				lastOption = tmp;
				lastOptionArgs.clear();
			}
			else
			{
				if (lastOption.empty())
				{
					throw std::invalid_argument("Invalid command line option: " + tmp);
				}
				lastOptionArgs.push_back(tmp);
			}
		}

		mOptions.insert({ lastOption, lastOptionArgs });
	}

	ArgsParser::ArgsParser(int argc, const char *const *const argv, char optionsPrefix)
		:mOptionsPrefix(optionsPrefix)
	{
		if (argc < 1)
		{
			throw std::invalid_argument("Value of 'argc' must be always >= 1!");
		}

		mProgramName = argv[0];
		initOptionsMap(argc, argv);
	}

	bool ArgsParser::containsOnlyValidOptions(const std::set<std::string>& allowedOptions) const
	{
		for (const auto& foundOpt : mOptions)
		{
			if (allowedOptions.find(foundOpt.first) == allowedOptions.end())
			{
				return false;
			}
		}

		return true;
	}

	const std::string& ArgsParser::getProgramName() const
	{
		return mProgramName;
	}

	bool ArgsParser::getOption(const std::string& optionName) const
	{
		return mOptions.find(optionName) != mOptions.end();
	}

	bool ArgsParser::getOption(const std::string& optionName,
		std::vector<std::string>& optionArgs) const
	{
		auto it = mOptions.find(optionName);
		if (it == mOptions.end())
		{
			return false;
		}

		optionArgs = it->second;
		return true;
	}

	bool ArgsParser::getOption(const std::string& optionName, 
		std::vector<std::string>& optionArgs, char valueDelim) const
	{
		auto it = mOptions.find(optionName);
		if (it == mOptions.end())
		{
			return false;
		}

		optionArgs.clear();
		for (const auto& opt : it->second)
		{
			size_t lastDelimPos = 0;
			std::string tmp;

			for (size_t i = 0; i < opt.size(); ++i)
			{
				if (opt[i] == valueDelim)
				{
					lastDelimPos = i;
					optionArgs.push_back(tmp);
					tmp.clear();
				}
				else
				{
					tmp += opt[i];
				}
			}

			if (lastDelimPos != opt.size() - 1)
			{
				optionArgs.push_back(opt.substr(lastDelimPos + 1));
			}
		}

		return true;
	}

	void ArgsParser::printParsedData() const
	{
		std::cout << "Parsed command line arguments for program " << mProgramName << std::endl;
		for (const auto& option : mOptions)
		{
			std::cout << option.first << ": ";
			for (const auto& optArg : option.second)
			{
				std::cout << optArg << " ";
			}
			std::cout << std::endl;
		}
	}
}
