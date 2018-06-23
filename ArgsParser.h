/*
   ArgsParser is class created with intention to simplify
   parsing of command line arguments.
   This approach introduces some memory/efficiency overhead,
   on the other hand it makes working with cmd args a bit less painful.

   (c) 2017 David Kutak
*/

#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>

namespace protolib
{
	class ArgsParser
	{
	private:
		char mOptionsPrefix;
		std::string mProgramName;
		std::map<std::string, std::vector<std::string>> mOptions;

		void initOptionsMap(int argc, const char *const *const argv);
	public:
		/**
		Main constructor to initialize the whole class

		@param argc number of command line arguments
		@param argv string with command line arguments
		@param optionsPrefix determines which character is used as a switch specifier 
			   (e.g. in 'ls -h', '-' is a switch specifier and option name is 'h')
		*/
		ArgsParser(int argc, const char *const *const argv, char optionsPrefix = '-');

		/**
		Checks whether the parsed command line arguments contain only valid options/switches

		@param allowedOptions set of possible command line switches/options
		@return true if parsed arguments contain only options included in allowedOptions set, false otherwise
		*/
		bool containsOnlyValidOptions(const std::set<std::string>& allowedOptions) const;

		/**
		Member function for accessing name of executable

		@return name of executable, i.e. string saved in argv[0]
		*/
		const std::string& getProgramName() const;

		/**
		Checks whether the given option/switch is present

		@param optionName name of the option/switch
		@return true if optionName switch is present, false otherwise
		*/
		bool getOption(const std::string& optionName) const;

		/**
		Checks whether the given option/switch is present and provides 
		its arguments as well

		@param optionName name of the option/switch
		@param[out] optionArgs vector which will be emptied and filled with arguments
		@return true if optionName switch is present, false otherwise
		*/
		bool getOption(const std::string& optionName, std::vector<std::string>& optionArgs) const;

		/**
		Checks whether the given option/switch is present and provides
		its arguments parsed by delimiter which is passed as a parameter of the function

		@param optionName name of the option/switch
		@param[out] optionArgs vector which will be emptied and filled with arguments
		@param valueDelim delimiter character which will be used when parsing arguments
		@return true if optionName switch is present, false otherwise
		*/
		bool getOption(const std::string& optionName, std::vector<std::string>& optionArgs, char valueDelim) const;

		/**
		Outputs structured parsed data to std::cout
		*/
		void printParsedData() const;
	};
}
