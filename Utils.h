/*
Utils.h contains some general functions which might come in handy.

(c) 2018 David Kutak
*/

#pragma once
#include <vector>
#include <fstream>
#include <string>

namespace protolib
{
	/**
	Generates all permutations of given vector

	@param possibleValues std::vector with values being permuted
	@return std::vector of all permutations
	*/
	template<typename T>
	std::vector<std::vector<T>> genPermutations(const std::vector<T>& possibleValues)
	{
		if (possibleValues.size() <= 1)
		{
			return { possibleValues };
		}

		std::vector<std::vector<T>> result;

		for (size_t i = 0; i < possibleValues.size(); ++i)
		{
			auto localCopy = possibleValues;
			localCopy.erase(localCopy.begin() + i);

			auto restPerm = genPermutations(localCopy);
			for (auto& vct : restPerm)
			{
				vct.insert(vct.begin(), possibleValues[i]);
				result.push_back(vct);
			}
		}

		return result;
	}

	/**
	Generates variations of given vector (i.e. rearrangements where order is important)

	@param possibleValues std::vector with values from which to choose
	@param k determines how many items to choose, i.e. k-tuples are chosen
	@param allowRepetition if set to true, elements might appear more than once
	@return std::vector with all corresponding variations
	*/
	template<typename T>
	std::vector<std::vector<T>> genVariations(const std::vector<T>& possibleValues, size_t k, bool allowRepetition = false)
	{
		std::vector<std::vector<T>> result;

		if (k == 1)
		{
			// To generate all possible variations,
			// it's necessary to return the rest of the list
			// divided into individual elements
			for (const auto& el : possibleValues)
			{
				result.push_back({ el });
			}

			return result;
		}

		for (size_t i = 0; i < possibleValues.size(); ++i)
		{
			auto localCopy = possibleValues;
			if (!allowRepetition)
			{
				localCopy.erase(localCopy.begin() + i);
			}

			auto restPerm = genVariations(localCopy, k - 1, allowRepetition);
			for (auto& vct : restPerm)
			{
				vct.insert(vct.begin(), possibleValues[i]);
				result.push_back(vct);
			}
		}

		return result;
	}

	/**
	Generates combinations of given vector

	@param possibleValues std::vector with values from which to choose
	@param k determines how many items to choose, i.e. k-tuples are chosen
	@param allowRepetition if set to true, elements might appear more than once
	@return std::vector with all corresponding combinations
	*/
	template<typename T>
	std::vector<std::vector<T>> genCombinations(const std::vector<T>& possibleValues, size_t k, bool allowRepetition = false)
	{
		std::vector<std::vector<T>> result;

		if (k == 1)
		{
			for (const auto& el : possibleValues)
			{
				result.push_back({ el });
			}

			return result;
		}

		for (size_t i = 0; i < possibleValues.size(); ++i)
		{
			auto localCopy = possibleValues;
			// When generating combinations, where the order of elements doesn't matter,
			// I can delete all preceding values because they will already be in a
			// selection with this element
			for (int j = static_cast<int>(i) - allowRepetition; j >= 0; --j)
			{
				localCopy.erase(localCopy.begin() + j);
			}

			auto restPerm = genCombinations(localCopy, k - 1, allowRepetition);
			for (auto& vct : restPerm)
			{
				vct.insert(vct.begin(), possibleValues[i]);
				result.push_back(vct);
			}
		}

		return result;
	}

	/**
	Parses input string by given list of string delimiters.
	If there are more delimiters set, the closest one is always chosen,
	i.e. all are considered of equal strength.

	@param input input string
	@param delimiters vector of string delimiters
	@return std::vector containing parsed data
	*/
	std::vector<std::string> parseString(const std::string& input, const std::vector<std::string>& delimiters);

	/**
	Returns all lines in a file fulfilling given predicate

	@param fileName file name
	@param pred unary predicate to determine which lines to include
	@return std::vector where each element corresponds to one line in a file
	*/
	template<typename UnPred>
	std::vector<std::string> readAllLinesFromFile(const std::string& fileName, UnPred pred)
	{
		std::ifstream ifile(fileName);
		std::vector<std::string> result;
		std::string tmp;

		while (std::getline(ifile, tmp))
		{
			if (pred(tmp))
			{
				result.push_back(tmp);
			}
		}

		return result;
	}

	/**
	Returns all lines in a file

	@param fileName file name
	@return std::vector where each element corresponds to one line in a file
	*/
	std::vector<std::string> readAllLinesFromFile(const std::string& fileName);
}
