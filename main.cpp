#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdio>
#include "UnitTestsFramework.h"
#include "ArgsParser.h"
#include "Logger.h"
#include "ContainerWrapper.h"
#include "SvgExporter.h"
#include "PnmExporter.h"
#include "Utils.h"

#define TESTS_ARGS_PARSER 1
#define TESTS_LOGGER 1
#define TESTS_CONT_WRAP 1
#define TESTS_SVG_EXP 1
#define TESTS_PNM_EXP 1
#define TESTS_UTILS 1

void testsArgsParser()
{
	int argcEmpty = 1;
	char** argvEmpty = new char*[1];
	argvEmpty[0] = new char[12];
	memcpy(argvEmpty[0], "program.exe", 12);

	protolib::ArgsParser argsParser(argcEmpty, argvEmpty);

	UNIT_TEST(TESTS_ARGS_PARSER, REQUIRE_TRUE, argsParser.getProgramName() == "program.exe");
	UNIT_TEST(TESTS_ARGS_PARSER, REQUIRE_FALSE, argsParser.getOption("nonsense"));

	delete[] argvEmpty[0];
	delete[] argvEmpty;
	
	int argcComplete = 5;
	char** argvComplete = new char*[argcComplete];
	for (int i = 0; i < argcComplete; ++i)
	{
		argvComplete[i] = new char[30];
	}
	memcpy(argvComplete[0], "program.exe", 12);
	memcpy(argvComplete[1], "/files", 7);
	memcpy(argvComplete[2], "inFile1;inFile2;inFile3", 24);
	memcpy(argvComplete[3], "oFile1;oFile2;oFile3", 21);
	memcpy(argvComplete[4], "/testFlag", 10);

	argsParser = protolib::ArgsParser(argcComplete, argvComplete, '/');

	UNIT_TEST(TESTS_ARGS_PARSER, REQUIRE_TRUE, argsParser.getProgramName() == "program.exe");

	UNIT_TEST(TESTS_ARGS_PARSER, REQUIRE_FALSE, argsParser.getOption("nonsense"));
	UNIT_TEST(TESTS_ARGS_PARSER, REQUIRE_TRUE, argsParser.getOption("/files"));
	UNIT_TEST(TESTS_ARGS_PARSER, REQUIRE_FALSE, argsParser.getOption("inFile1;inFile2;inFile3"));
	UNIT_TEST(TESTS_ARGS_PARSER, REQUIRE_TRUE, argsParser.getOption("/testFlag"));
	std::vector<std::string> options;
	UNIT_TEST(TESTS_ARGS_PARSER, REQUIRE_TRUE, argsParser.getOption("/files", options));
	UNIT_TEST(TESTS_ARGS_PARSER, REQUIRE_TRUE, 
		options == std::vector<std::string>({"inFile1;inFile2;inFile3", "oFile1;oFile2;oFile3"}));
	UNIT_TEST(TESTS_ARGS_PARSER, REQUIRE_TRUE, argsParser.getOption("/files", options, ';'));
	UNIT_TEST(TESTS_ARGS_PARSER, REQUIRE_TRUE,
		options == std::vector<std::string>({ "inFile1","inFile2","inFile3","oFile1","oFile2","oFile3" }));

	UNIT_TEST(TESTS_ARGS_PARSER, REQUIRE_TRUE, argsParser.containsOnlyValidOptions({ "/files", "/testFlag" }));
	UNIT_TEST(TESTS_ARGS_PARSER, REQUIRE_FALSE, argsParser.containsOnlyValidOptions({ "/files" }));

	for (int i = 0; i < argcComplete; ++i)
	{
		delete[] argvComplete[i];
	}
	delete[] argvComplete;
}

void testsLogger()
{
	using protolib::Logger;
	std::vector<int> testVector = { 2, 4, 6, 8 };
	size_t lineNum = 0;
	std::string tmp;

	Logger::setLogFile("testsLogger_1.txt", true);
	Logger::writeStructuredLog(testVector, "testVector");
	Logger::writeSimpleLog("3.14", "pi", "file.cpp", Logger::LogType::WAR);
	Logger::writeSimpleBoolLog(true && false, "true && false", "file2.cpp", Logger::LogType::ERR);
	Logger::disableLogging();
	Logger::writeSimpleBoolLog(true || false, "true || false", "file2.cpp", Logger::LogType::ERR);
	Logger::enableLogging();
	Logger::closeLogFile();

	std::ifstream tl1("testsLogger_1.txt");
	UNIT_TEST(TESTS_LOGGER, REQUIRE_TRUE, tl1.good());
	while (std::getline(tl1, tmp))
	{
		switch (lineNum)
		{
		case 0:
			UNIT_TEST(TESTS_LOGGER, REQUIRE_TRUE, tmp == "[File NONE] [Type INF] testVector = 2, 4, 6, 8, ");
			break;
		case 1:
			UNIT_TEST(TESTS_LOGGER, REQUIRE_TRUE, tmp == "[File file.cpp] [Type WAR] pi = 3.14");
			break;
		case 2:
			UNIT_TEST(TESTS_LOGGER, REQUIRE_TRUE, tmp == "[File file2.cpp] [Type ERR] true && false is false");
			break;
		default: ABORT_TESTS("File contains more lines than it should!");
			break;
		}
		++lineNum;
	}

	UNIT_TEST(TESTS_LOGGER, REQUIRE_TRUE, lineNum == 3);

	Logger::setLogFile("testsLogger_2.txt", true);
	Logger::enableSyncedLogging();
	Logger::writeSimpleInfoLog("hello world");
	Logger::writeSimpleInfoLog("another info", "file.cpp");
	Logger::closeLogFile();

	std::ifstream tl2("testsLogger_2.txt");
	UNIT_TEST(TESTS_LOGGER, REQUIRE_TRUE, tl2.good());

	lineNum = 0;
	while (std::getline(tl2, tmp)) { ++lineNum; }
	UNIT_TEST(TESTS_LOGGER, REQUIRE_TRUE, lineNum == 0);
	tl2.close();

	Logger::setLogFile("testsLogger_2.txt", true);
	Logger::syncedOutput();
	Logger::closeLogFile();

	tl2 = std::ifstream("testsLogger_2.txt");
	UNIT_TEST(TESTS_LOGGER, REQUIRE_TRUE, tl2.good());
	while (std::getline(tl2, tmp))
	{
		switch (lineNum)
		{
		case 0:
			UNIT_TEST(TESTS_LOGGER, REQUIRE_TRUE, tmp == "[File NONE] [Type INF] hello world");
			break;
		case 1:
			UNIT_TEST(TESTS_LOGGER, REQUIRE_TRUE, tmp == "[File file.cpp] [Type INF] another info");
			break;
		default: ABORT_TESTS("File contains more lines than it should!");
			break;
		}
		++lineNum;
	}

	UNIT_TEST(TESTS_LOGGER, REQUIRE_TRUE, lineNum == 2);

	// Cleanup
	tl1.close();
	tl2.close();
	if (remove("testsLogger_1.txt") != 0 || remove("testsLogger_2.txt") != 0)
	{
		std::cout << "UNIT TESTS WARNING! Removing of files in testsLogger method failed." << std::endl;
	}
}

void testsContainerWrapper()
{
	using protolib::ContainerWrapper;

	ContainerWrapper<std::vector<int>> cont1;

	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.empty());
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.size() == 0);

	cont1.insert(4);
	cont1.insert(2);
	cont1.insert(6);

	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_FALSE, cont1.empty());
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.size() == 3);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.min() == 2);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.max() == 6);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.sum() == 12);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.average() == 4);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.count([](auto val) { return val > 2; }) == 2);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_FALSE, cont1.isSorted());

	cont1.erase(++cont1.begin());
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.size() == 2);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.min() == 4);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.max() == 6);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.isSorted());

	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.find(5) == cont1.end());
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.find(4) != cont1.end());

	cont1.insert(1);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_FALSE, cont1.isSorted());
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.size() == 3);
	cont1 = cont1.getSorted();
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.isSorted());
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.size() == 3);

	ContainerWrapper<std::vector<int>> cont2(cont1.getContainer());

	cont1.insert(1);
	cont1.insert(4);

	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.size() == 5);
	cont1 = cont1.unique();
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.size() == 3);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont2.size() == 3);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1 == cont2);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_FALSE, cont1 != cont2);

	cont1.addRange(1, 5, 2);
	cont2.addRange(1, 5, 2);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1.size() == 6);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont2.size() == 6);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont1 == cont2);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_FALSE, cont1 != cont2);

	ContainerWrapper<std::vector<int>> cont3(cont1.begin(), cont1.end());
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont3.getContainer() == std::vector<int>({1, 4, 6, 1, 3, 5}));
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont3.reverse().getContainer() == std::vector<int>({ 5, 3, 1, 6, 4, 1 }));
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont3[2] == 6);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont3[3] == 1);
	auto groups = cont3.groupBy([](auto val) { return val % 2; });
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, groups.size() == 2);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, groups[0] == std::vector<int>({ 4, 6 }));
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, groups[1] == std::vector<int>({ 1, 1, 3, 5 }));

	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont3.take(100).getContainer() == std::vector<int>({ 1, 4, 6, 1, 3, 5 }));
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont3.skip(100).getContainer() == std::vector<int>());
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont3.take(2).getContainer() == std::vector<int>({ 1, 4}));
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont3.skip(2).getContainer() == std::vector<int>({ 6, 1, 3, 5 }));
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont3.takeWhile([](auto val) { return val < 6; }).getContainer() == std::vector<int>({ 1, 4 }));
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont3.skipWhile([](auto val) { return val != 3; }).getContainer() == std::vector<int>({ 3, 5 }));

	cont3.erase(1);
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont3.getContainer() == std::vector<int>({ 4, 6, 3, 5 }));
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont3.where([](auto val) { return std::abs(val - 3) <= 1; }).getContainer() == std::vector<int>({ 4, 3 }));


	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, 
		cont3.accumulateLeft(std::string("init"), [](auto a, auto b) {return a + "-" + std::to_string(b); }) == "init-4-6-3-5");
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE,
		cont3.accumulateRight(std::string("fin"), [](auto a, auto b) {return std::to_string(a) + "-" + b; }) == "4-6-3-5-fin");

	ContainerWrapper<std::string> cont4("abcd");
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont4.getContainer() == "abcd");
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, cont4[2] == 'c');
	auto res = cont4.map<char, std::string>([](auto val) { return val + 1; }).getContainer() == "bcde"; // Simple Caesar cipher, right? ;-)
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, res);
	res = cont4.map<char, std::string>([](auto val) { return val + 1; }).getContainer() == "bcdf";
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_FALSE, res);
	res = cont4.map<int, std::vector<int>>([](auto val) { return static_cast<int>(val); }).getContainer() == std::vector<int>({ 97, 98, 99, 100 });
	UNIT_TEST(TESTS_CONT_WRAP, REQUIRE_TRUE, res);
}

void testsSvgExporter()
{
	protolib::SvgExporter svgExporter1(256, 512, "white", "yellow", 3);

	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getWidth() == 256);
	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getHeight() == 512);
	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getDefaultFillColor() == "white");
	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getDefaultStrokeColor() == "yellow");
	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getDefaultStrokeWidth() == 3);
	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getSvgObjects().empty());

	svgExporter1.setWidth(1024);
	svgExporter1.setHeight(768);
	svgExporter1.setDefaultFillColor("green");
	svgExporter1.setDefaultStrokeColor("red");
	svgExporter1.setDefaultStrokeWidth(1);

	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getWidth() == 1024);
	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getHeight() == 768);
	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getDefaultFillColor() == "green");
	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getDefaultStrokeColor() == "red");
	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getDefaultStrokeWidth() == 1);

	svgExporter1.addRectangle(0, 0, 10 ,10);
	svgExporter1.addCircle(0, 0, 10);
	svgExporter1.addEllipse(0, 0, 10, 15);
	svgExporter1.addLine(0, 0, 10, 10);
	svgExporter1.addPolygon({std::make_pair(0,0), std::make_pair(10,10)});
	svgExporter1.addPolyline({ std::make_pair(0,0), std::make_pair(10,10) });
	svgExporter1.addText(0, 0, 10, "hello world");
	
	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getSvgObjects().size() == 7);
	svgExporter1.save("svgImage1.svg");

	svgExporter1.removeLast();
	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getSvgObjects().size() == 6);
	svgExporter1.removeLast();
	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getSvgObjects().size() == 5);
	svgExporter1.removeAll();
	UNIT_TEST(TESTS_SVG_EXP, REQUIRE_TRUE, svgExporter1.getSvgObjects().empty());
	svgExporter1.removeLast();

	protolib::SvgExporter svgExporter2;
	svgExporter2.setDefaultStrokeWidth(0);
	svgExporter2.addText(20, 30, 16, "Hello SVG!", "blue");
	svgExporter2.addCircle(256, 256, 128, "green", "brown", "style=\"stroke-width:3;stroke:#FF0000\"");
	svgExporter2.setDefaultStrokeWidth(2);
	svgExporter2.addLine(30, 50, 180, 50);
	svgExporter2.addEllipse(256, 256, 64, 32, "yellow");
	svgExporter2.addPolygon({ std::make_pair(300,100), std::make_pair(100,100), std::make_pair(200,200) });
	svgExporter2.addPolyline({ std::make_pair(50,100), std::make_pair(75,150), std::make_pair(25,200) }, "yellow", "style=\"fill:none;\"");
	svgExporter2.addRectangle(300, 40, 150, 50);
	svgExporter2.setDefaultStrokeWidth(1);
	svgExporter2.addLine(0, 0, 512, 512);
	svgExporter2.save("svgImage2.svg");
}

void testsPnmExporter()
{
	using namespace protolib;

	PnmExporter<> pnmExport(640, 480);

	UNIT_TEST(TESTS_PNM_EXP, REQUIRE_TRUE, pnmExport.getWidth() == 640);
	UNIT_TEST(TESTS_PNM_EXP, REQUIRE_TRUE, pnmExport.getHeight() == 480);
	UNIT_TEST(TESTS_PNM_EXP, REQUIRE_TRUE, pnmExport.getImageType() == PnmImageType::PPM_BIN);
	pnmExport.swapBtwnASCIIandBIN();
	UNIT_TEST(TESTS_PNM_EXP, REQUIRE_TRUE, pnmExport.getImageType() == PnmImageType::PPM_ASCII);
	pnmExport.swapBtwnASCIIandBIN();
	UNIT_TEST(TESTS_PNM_EXP, REQUIRE_TRUE, pnmExport.getImageType() == PnmImageType::PPM_BIN);
	pnmExport.setPixel(10, 10, PnmExporter<>::PixelColor(127, 255, 1));
	UNIT_TEST(TESTS_PNM_EXP, REQUIRE_TRUE, pnmExport.getPixel(10, 10).r == 127 && 
		pnmExport.getPixel(10, 10).g == 255 && pnmExport.getPixel(10, 10).b == 1);

	pnmExport.setWidth(512);
	pnmExport.setHeight(256);
	pnmExport.clear();

	UNIT_TEST(TESTS_PNM_EXP, REQUIRE_TRUE, pnmExport.getWidth() == 512);
	UNIT_TEST(TESTS_PNM_EXP, REQUIRE_TRUE, pnmExport.getHeight() == 256);

	PnmExporter<> pnmExport2(640, 480, PnmImageType::PBM_BIN);
	UNIT_TEST(TESTS_PNM_EXP, REQUIRE_TRUE, pnmExport2.getImageType() == PnmImageType::PBM_BIN);
	pnmExport2.swapBtwnASCIIandBIN();
	UNIT_TEST(TESTS_PNM_EXP, REQUIRE_TRUE, pnmExport2.getImageType() == PnmImageType::PBM_ASCII);
	pnmExport2.swapBtwnASCIIandBIN();
	UNIT_TEST(TESTS_PNM_EXP, REQUIRE_TRUE, pnmExport2.getImageType() == PnmImageType::PBM_BIN);

	PnmExporter<> pnmExport3(640, 480, PnmImageType::PGM_BIN);
	UNIT_TEST(TESTS_PNM_EXP, REQUIRE_TRUE, pnmExport3.getImageType() == PnmImageType::PGM_BIN);
	pnmExport3.swapBtwnASCIIandBIN();
	UNIT_TEST(TESTS_PNM_EXP, REQUIRE_TRUE, pnmExport3.getImageType() == PnmImageType::PGM_ASCII);
	pnmExport3.swapBtwnASCIIandBIN();
	UNIT_TEST(TESTS_PNM_EXP, REQUIRE_TRUE, pnmExport3.getImageType() == PnmImageType::PGM_BIN);

	
	pnmExport.setPixels([](size_t x, size_t y) { return (x % 5 == 0 && y % 3 != 0); },
		PnmExporter<>::PixelColor(255, 0, 0));
	pnmExport2.setPixels([](size_t x, size_t y) { return x % 5 == 0 && y % 3 != 0; },
		PnmExporter<>::PixelColor(1));
	pnmExport3.setPixels([](size_t x, size_t y) { return x % 5 == 0 && y % 3 != 0; },
		PnmExporter<>::PixelColor(255));

	pnmExport.addCircle(pnmExport.getWidth() / 2, pnmExport.getHeight() / 2, 15,
		PnmExporter<>::PixelColor(255, 255, 255));
	pnmExport2.addCircle(pnmExport2.getWidth() / 2, pnmExport2.getHeight() / 2, 15,
		PnmExporter<>::PixelColor(1));
	pnmExport3.addCircle(pnmExport3.getWidth() / 2, pnmExport3.getHeight() / 2, 15,
		PnmExporter<>::PixelColor(255));

	pnmExport.save("testBin.ppm");
	pnmExport2.save("testBin.pbm");
	pnmExport3.save("testBin.pgm");

	pnmExport.swapBtwnASCIIandBIN();
	pnmExport2.swapBtwnASCIIandBIN();
	pnmExport3.swapBtwnASCIIandBIN();

	pnmExport.save("test.ppm");
	pnmExport2.save("test.pbm");
	pnmExport3.save("test.pgm");


}

void testsUtils()
{
	using namespace protolib;

	std::vector<char> charVector = { 'a','b','c' };

	auto perm = genPermutations(charVector);
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, perm.size() == 6);
	for (const auto& el : std::vector<std::vector<char>>{
		{ 'a','b','c' },{ 'b','c','a' },{ 'c','a','b' },{ 'a','c','b' },{ 'b','a','c' },{ 'c','b','a' } 
		})
	{
		UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, std::find(perm.begin(), perm.end(), el) != perm.end());
	}

	charVector.push_back('d');

	auto vars = genVariations(charVector, 2, false);
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, vars.size() == 12);
	for (const auto& el : std::vector<std::vector<char>>{
		{'a','b'},{ 'a','c' },{ 'a','d' },
		{ 'b','a' },{ 'b','c' },{ 'b','d' },
		{ 'c','a' },{ 'c','b' },{ 'c','d' },
		{ 'd','a' },{ 'd','b' },{ 'd','c' },
		})
	{
		UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, std::find(vars.begin(), vars.end(), el) != vars.end());
	}

	vars = genVariations(charVector, 2, true);
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, vars.size() == 16);
	for (const auto& el : std::vector<std::vector<char>>{
		{ 'a','b' },{ 'a','c' },{ 'a','d' },
		{ 'b','a' },{ 'b','c' },{ 'b','d' },
		{ 'c','a' },{ 'c','b' },{ 'c','d' },
		{ 'd','a' },{ 'd','b' },{ 'd','c' },
		{ 'a', 'a'},{ 'b', 'b' },{ 'c', 'c' },{ 'd', 'd' }
		})
	{
		UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, std::find(vars.begin(), vars.end(), el) != vars.end());
	}

	auto combs = genCombinations(charVector, 2, false);
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, combs.size() == 6);
	for (const auto& el : std::vector<std::vector<char>>{
		{ 'a','b' },{ 'a','c' },{ 'a','d' },
		{ 'b','c' },{ 'b','d' },{ 'c','d' },
		})
	{
		UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, std::find(combs.begin(), combs.end(), el) != combs.end());
	}

	combs = genCombinations(charVector, 2, true);
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, combs.size() == 10);
	for (const auto& el : std::vector<std::vector<char>>{
		{ 'a','b' },{ 'a','c' },{ 'a','d' },
		{ 'b','c' },{ 'b','d' },{ 'c','d' },
		{ 'a', 'a' },{ 'b', 'b' },{ 'c', 'c' },{ 'd', 'd' }
		})
	{
		UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, std::find(combs.begin(), combs.end(), el) != combs.end());
	}

	auto parseResult = parseString("hello;world;nice", { ";" });
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult.size() == 3);
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[0] == "hello");
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[1] == "world");
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[2] == "nice");

	parseResult = parseString("hello;;world;;nice", { ";;" });
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult.size() == 3);
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[0] == "hello");
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[1] == "world");
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[2] == "nice");

	parseResult = parseString("hello;;world;;nice;;", { ";;" });
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult.size() == 3);
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[0] == "hello");
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[1] == "world");
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[2] == "nice");

	parseResult = parseString("hello", { ";;" });
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult.size() == 1);
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[0] == "hello");

	parseResult = parseString("hello;;", { ";;" });
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult.size() == 1);
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[0] == "hello");

	parseResult = parseString("hello/world;;nice", { ";;","/" });
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult.size() == 3);
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[0] == "hello");
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[1] == "world");
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[2] == "nice");

	parseResult = parseString("hello;world,it's.great::right?", { ";","/",",",".","::" });
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult.size() == 5);
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[0] == "hello");
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[1] == "world");
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[2] == "it's");
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[3] == "great");
	UNIT_TEST(TESTS_UTILS, REQUIRE_TRUE, parseResult[4] == "right?");
} 

int main()
{
	testsArgsParser();
	testsLogger();
	testsContainerWrapper();
	testsSvgExporter();
	testsPnmExporter();
	testsUtils();

	SUMMARIZE_TESTS(std::cout);

	int exitTmp;
	std::cin >> exitTmp;
	return 0;
}