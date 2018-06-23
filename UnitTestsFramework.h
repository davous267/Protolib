#pragma once
#include <cassert>

// Special namespace for counters to avoid polluting of global namespace with global variables
namespace utestsCounters
{
	int passedTests = 0;
	int failedTests = 0;
}

// Verifies given condition
// In case of failure, outputs information about failed test but does NOT abort program
#define VERIFY_TRUE(cond) { if (!(cond)) { std::cout << "VERIFY test failed at " << __FILE__ << ":" << __LINE__ << std::endl; ++utestsCounters::failedTests; } else { ++utestsCounters::passedTests; } }
#define VERIFY_FALSE(cond) VERIFY_TRUE(!(cond))

// Same as VERIFY_* but aborts program in case of failed test
#define REQUIRE_TRUE(cond) { if (!(cond)) { std::cout << "REQUIRE test failed at " << __FILE__ << ":" << __LINE__ << std::endl; abort(); ++utestsCounters::failedTests; } else { ++utestsCounters::passedTests; } }
#define REQUIRE_FALSE(cond) REQUIRE_TRUE(!(cond))

// Enables for specifying test group of given unit test
// ------
// Example usage:
// #define TEST_GROUP_1 1 [or 0 if you want to disable testing of particular group]
// ....
// UNIT_TEST(TEST_GROUP_1, VERIFY_TRUE, 2 > 1);
// ------
#define UNIT_TEST(group, func, cond) { if (group) { func(cond); } }

// Outputs number of failed/passed tests
#define SUMMARIZE_TESTS(os) { os << "Passed tests: " << utestsCounters::passedTests << " | Failed tests: " << utestsCounters::failedTests << std::endl; }

// Aborts program execution if needed
#define ABORT_TESTS(message) { std::cout << "Unit testing aborted by user at " << __FILE__ << ":" << __LINE__ << " with message: " << message << std::endl; abort(); }
