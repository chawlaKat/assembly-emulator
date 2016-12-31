//#pragma once
//#include<string>
//#include<fstream>
//
//using namespace std;
//
//class Testing
//{
//	// OPT: optimize tests
//	// pass a bool of whether test should pass
//	// return a bool of whether expected result was obtained
//public:
//	std::string memoryEmpty(Memory memPool)
//	{
//		//make sure all elements are equal to "nop"
//		std::string j;
//		for (int i = 0; i < 1000; i++)
//		{
//			j = memPool.get(i);
//			if (j != "nop")
//				return "Memory is empty failed\n";
//		}
//		return "Memory is empty succeeded\n";
//	}
//
//	std::string memorySet(Memory memPool)
//	{
//		std::string temp;
//		std::string newMem = "";
//		bool success = false;
//
//		memPool.set(0, newMem);
//
//		//check if set worked
//		if (memPool.get(0) == newMem)
//			success = true;
//
//		//reset data
//		memPool.set(0, temp);
//
//		if (success)
//			return "Memory set succeeded\n";
//		return "Memory set failed\n";
//	}
//
//	//Test Read In
//	//pre: main function has read a file and saved it to memory object, file is good
//	//post: returns failed message if memory does not match file (ignoring
//	//		line number repetition)
//	//		returns succeed message otherwise
//	string fileSaved(string name, Memory memPool, bool shouldPass)
//	{
//		ifstream file;
//		file.open(name);
//		string input;
//		regex lineNumber("(\\d+)\\s*(.*)");
//		int lineNum;
//		string toStore;
//		cmatch res;
//		Memory noMatch;
//		bool success = true;
//		while (file.good())
//		{
//			lineNum = -1;
//			toStore = "";
//
//			getline(file, input);
//
//
//			regex_search(input.c_str(), res, lineNumber);
//			if (res.size() >= 2)
//				toStore = res[2].str();
//
//
//			if (res.size() >= 1)
//			{
//				lineNum = stoi(res.str());
//			}
//
//			if (lineNum >= 0 && lineNum < 1000)
//			{
//				string stored = memPool.get(lineNum);
//
//				if (toStore != stored)
//				{
//					noMatch.set(lineNum, toStore);
//				}
//				else
//				{
//					noMatch.set(lineNum, "nop");
//				}
//			}
//		}
//		file.close();
//		
//		for (int i = 0; i < 1000; i++)
//		{
//			if (noMatch.get(i) != "nop")
//			{
//				success = false;
//				break;
//			}
//		}
//
//		if (success == shouldPass)
//			return "File saved succeeded\n";
//		return "File saved failed\n";
//	}
//
//};