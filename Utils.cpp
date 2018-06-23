#include "Utils.h"

namespace protolib
{
	std::vector<std::string> parseString(const std::string& input, const std::vector<std::string>& delimiters)
	{
		std::vector<std::string> result;

		std::pair<size_t, size_t> currPos(0, 0);
		size_t lastPos = 0;

		auto findMinDelimPos = [&]() {
			size_t minPos = std::string::npos;
			size_t minPosDelLength = 0;
			for (const auto& str : delimiters)
			{
				size_t pos = input.find(str, currPos.first);
				if (pos < minPos)
				{
					minPos = pos;
					minPosDelLength = str.size();
				}
			}
			return std::make_pair(minPos, minPosDelLength);
		};

		while ((currPos = findMinDelimPos()).first != std::string::npos)
		{
			result.push_back(input.substr(lastPos, currPos.first - lastPos));
			currPos.first += currPos.second;
			lastPos = currPos.first;
		}

		auto residual = input.substr(lastPos, currPos.first - lastPos);
		if (!residual.empty())
		{
			result.push_back(residual);
		}

		return result;
	}
}