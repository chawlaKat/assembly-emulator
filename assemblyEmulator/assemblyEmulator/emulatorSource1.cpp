//// Kathy Chawla			16 Nov 2016		algorithm development
////						29 Nov 2016		optimizations
//
///*  Assignment
//
//Implement a simulator for the IAS assembly language, following the fetch-execute 
//cycle. 
//
//Valid instructions:
//							AR: accumulator
//							MR: multiplication register
//							M(X): value in location x
//
//	load MQ					load value in MR into AR
//	load MQ, M(X)			load value in M(X) into MR
//	stor M(X)				store AR into M(X)
//	load M(X)				load value in M(X) into AR
//	load - M(X)				load (value in M(X)) * -1 into AR
//	load |M(X)|				load absolute value of M(X) into AR
//	load - |M(X)|			load (absolute value of M(X)) * -1 into AR
//	halt					last line of program; stop fetch-execute, print final memory
//	begin					first line of program; begin PC here
//	.						comment as a single line with no assembly code, do nothing
//	variables / constants
//	nop						no operation
//
//
//Requirements:
//1) Follow fetch-execute cycle
//	- Uses "registers" PC, IR, MAR, and MBR, implemented as global variables
//	- Does not need a separate class
//2) Implement memory as a class
//	- Contains get and set methods
//	- Memory is an array of 1000 strings
//	- Not directly accessible from main
//	- Default value for memory is nop
//	- If the same line number appears more than once, keep only the last instance
//3) The decode method must be separate from main
//	- Is the Control Unit
//	- If the instruction cannot be decoded, method fails, outputs an error message, and 
//		stops execution
//	- Uses regular expressions
//	- Interprets instructions, sends them to execute as two parameters
//		Example: decode("load -|M(21)|") passes 7 and 231 as parameters to execute, 
//		where 7 means load address location 21 and make the value negative.
//	- Begin, halt, nop, and comments do not call execute
//4) The execute method must be separate from main 
//	- Called by decode, as detailed above
//	- Takes two parameters
//	- Performs specified instruction
//5) Read a user-specified file at run time, then load it into the memory object
//	- Stores code in index specified by line number
//	- Does not store line numbers
//	- Uses regular expressions
//6) After file is loaded:
//	- Locates "begin" in memory
//	- Sets PC equal to begin's location
//	- Begins fetch execute cycle
//7) Produces the following output: 
//	- The initial memory contents 
//	- Execution of each cycle on two lines:
//		i. PC and IR			printed after the instruction is fetched, but before 
//								PC increments
//		ii. PC, AR, and MQ		printed after the instruction is executed
//	- The final memory contents from 0 to halt
//8) Stop execution when a "halt" is decoded
//
//*/
//
//#include<iostream>
//#include<fstream>
//#include<string>
//#include<regex>
//#include"Memory.h"
//
//using namespace std;
//
//void printMemory();
//void decode(string);
//void execute(int, int);
//void waitForUser();
//
//// Registers
//int PC = 0;									// program counter
//int MAR = 0;								// memory address register
//string MBR = "";							// memory buffer register
//string IR = "";								// instruction register
//
//int AR = 0;									// accumulator register
//int MQ = 0;									// multiplier/quotient register
//
//Memory memPool;								// memory object
//
//int main()
//{
//	int beginAdr = 0;						// location of begin
//	string fName;							// user-specified file name
//	ifstream inputFile;
//
//	do
//	{
//		// read in name
//		cout << "Enter file name: \n";
//		cin >> fName;
//
//		inputFile.open(fName);
//
//		if (!inputFile.is_open())
//		{
//			cout << "Could not open file.\n\n";
//		}
//	} while (!inputFile.is_open());			// validate file name
//
//	cout << "File opened successfully.\n\n";
//	memPool.clear();
//
//	// Read in file
//	// TODO: double check this still works using + not *
//	regex parseInput("(\\d+)\\s*(.+)");		// line number, any whitespace, instruction
//											/* Note to self: regex quantifiers
//												+ 1 or more
//												? 0 or 1
//												* 0 or more
//											*/
//	int lineNum;
//	string toStore;
//	string fromFile;
//	cmatch results;							// results from regex
//	while (inputFile.good())				// until end of file
//	{
//		lineNum = -1;						// flag if line has incorrect form
//		toStore = "nop";					// if bad string, store default
//
//		getline(inputFile, fromFile);
//
//		// parse input
//		regex_search(fromFile.c_str(), results, parseInput);
//
//		// save any matches
//		if (results.size() >= 1)
//			lineNum = stoi(results.str());
//		if (results.size() >= 2)
//			toStore = results[2].str();
//
//		// store to memory, if possible
//		if (lineNum >= 0 && lineNum < 1000)
//			memPool.set(lineNum, toStore);
//	}
//										
//	inputFile.close();
//
//	// Print Initial Memory
//	cout << "Initial Memory: \n";
//	printMemory();
//
//	// Find begin
//	regex beginStr("begin.*");				// "begin", followed by anything
//	//cmatch results;
//	for (beginAdr = 0; beginAdr < 1000; beginAdr++)
//	{
//		if (regex_search(memPool.get(beginAdr).c_str(), results, beginStr))
//		{
//			break;
//		}
//	}
//
//	if (beginAdr == 1000)
//	{
//		cout << "Could not find begin statement." << endl;
//		void waitForUser();
//		return 0;
//	}
//
//	// Fetch
//	PC = beginAdr;
//	cout << "Execution: \n";
//	do
//	{
//		MAR = PC;							// MAR <- (PC)
//		MBR = memPool.get(MAR);				// MBR <- (memory)
//
//		cout << "PC: " << PC << " IR: " << IR << endl;
//
//		PC++;								// PC <- (PC) + 1
//		IR = MBR;							// IR <- (MBR)
//		decode(IR);
//
//		cout << "PC: " << PC << " AR: " << AR << "MQ: " << MQ << endl << endl;
//	} while (IR != "halt");
//
//	// Print Final Memory
//	cout << "Final Memory: \n";
//	printMemory();
//
//	waitForUser();
//	return 0;
//}
//
//void printMemory()
//{
//	string currentLine = "";
//	for (int i = 0; i < 1000; i++)
//	{
//		currentLine = memPool.get(i);
//		if (currentLine != "nop")
//		{
//			cout << i << " " << currentLine << endl;
//		}
//	}
//}
//
//void decode(string inst)
//{
//	cmatch res;
//	regex nop("nop");									// no op			no call
//	regex comment("\\..*");								// comment			no call
//	regex loadMQ("load\\s*MQ");							// load MQ			1
//	regex loadMQMX("load\\s*MQ,\\s*M\\((\\d)\\)");		// load MQ,M(X)		2
//	regex storMX("stor\\s*M\\((\\d)\\)");				// stor M(X)		3
//	regex load("load\\s*dM\\((\\d)\\)");				// load M(X)		4
//	regex loadNeg("load\\s*-//s*M\\s((\\d)\\)");		// load -M(X)		5
//	regex loadAbs("load\\s*\\|\\s*M\\((\\d)\\)\\s*\\|"); 
//														// load |M(X)|		6
//	regex loadNabs("load\\s*-\\s*\\|\\s*M\\((\\d||)\\s*\\|");
//														// load -|M(X)|		7
//
//	if (regex_search(inst.c_str(), res, nop) || regex_search(inst.c_str(), res, comment));
//	else if (regex_search(inst.c_str(), res, loadMQMX))
//	{
//		execute(2, stoi(res[1]));
//	}
//	else if (regex_search(inst.c_str(), res, loadMQ))
//	{
//		execute(1, stoi(res[1]));
//	}
//	else if (regex_search(inst.c_str(), res, storMX))
//	{
//		execute(3, stoi(res[1]));
//	}
//	else if (regex_search(inst.c_str(), res, load))
//	{
//		execute(4, stoi(res[1]));
//	}
//	else if (regex_search(inst.c_str(), res, loadNeg))
//	{
//		execute(5, stoi(res[1]));
//	}
//	else if (regex_search(inst.c_str(), res, loadAbs))
//	{
//		execute(6, stoi(res[1]));
//	}
//	else if (regex_search(inst.c_str(), res, loadNabs))
//	{
//		execute(7, stoi(res[1]));
//	}
//	else
//	{
//		cout << "Instruction not recognized. Halting execution.\n";
//		IR = "halt";
//	}
//}
//
//void execute(int op, int val)
//{
//	int fromMem = 0;
//	if (stoi(memPool.get(val)))
//	{
//		fromMem = stoi(memPool.get(val));
//	}
//
//	switch (op)
//	{
//	case 1:
//		AR = MQ;
//		break;
//	case 2:
//		MQ = val;
//		break;
//	case 3:
//		memPool.set(val, to_string(AR));
//		break;
//	case 4:
//		AR = fromMem;
//		break;
//	case 5:
//		AR = -1 * fromMem;
//		break;
//	case 6:
//		if (fromMem >= 0)
//			AR = fromMem;
//		else
//			AR = -1 * fromMem;
//		break;
//	case 7:
//		if (fromMem >= 0)
//			AR = -1 * fromMem;
//		else
//			AR = fromMem;
//	}
//}
//
//void waitForUser()
//{
//	cout << "Program exiting. Press enter to continue.\n";
//	cin.clear();							// restores cin if bad
//	cin.ignore(256, '\n');					// ignores anything left in cin
//	cin.get();								// forces program to wait for user
//}
//
//// Kathy Chawla			16 Nov 2016		algorithm development
////						18 Nov 2016		write and test read fName, open file
//
///* Assignment
//
//Implement a simulator for the IAS assembly language, following the fetch-execute
//cycle.
//
//The simulator will be able to decode and run the following instructions:
//- load MQ
//- load MQ, M(X)
//- stor M(X)
//- load M(X)
//- load -M(X)
//- load |M(X)|
//- load -|M(X)|
//- halt
//- begin
//- . (comment, but only as a single line without any assembly code)
//- variables/constants
//- nop (no operation)
//
//Requirements:
//1) Follow the fetch-execute cycle.
//- Use the "registers" PC, IR, MAR, and MBR and implement them as global variables.
//- Fetch does not need a separate class.
//2) Implement memory as a class.
//- Class contains set and get methods.
//- Memory is an array of 1000 strings.
//- Default value for memory is nop.
//- The same line number may appear more than once; only keep the last instance.
//- The memory is not accessible directly from main.
//3) The decode function/method must be separate.
//- This is the Control Unit.
//- If the instruction cannot be decoded, the method fails, outputs an
//error message, and stops execution.
//- Use regular expressions.
//- Interprets instructions and sends to execute as two pieces of information,
//then calls execute.
//Example: decode("load -|M(21)|") passes 7 and 21 as parameters to
//execute,where 7 means load address location 21 and make the value
//negative.
//- Begin, halt, nop, and comments do not call execute.
//4) The execute funtion/method must be separate.
//- Execute is called by decode.
//- Takes two parameters.
//- Performs the instruction.
//5) Read a user-specified file at run time, then load it into the memory class.
//- Store code in index indicated by line numbers.
//- Do not store the line numbers.
//- Use regular expressions.
//6) After the file is loaded:
//- Locate "begin" in memory.
//- Set PC equal to begin's location.
//- Begin the fetch-execute cycle.
//7) Produce the following output:
//- The starting memory contents
//- The execution of each cycle on two lines:
//i. PC and IR		printed after the instruction is fetched,
//but before PC increments
//ii. PC, AC, and MQ	printed after the instruction is executed
//- The ending memory contents from 0 to halt
//8) Stop execution when a "halt" is decoded.
//
//
//
//Tasks:
//Create a unit test class
//Read file name, open file
//Create memory class
//Read file, store to memory
//Print starting memory
//Set up fetch-execute
//Decode function
//Execute function, including print statements
//Search for begin and for halt
//Print ending memory
//*/
///*
//#include<iostream>
//#include<fstream>
//#include<string>
//#include<regex>
//#include"Memory.h"
//#include"Testing.h"
//
//using namespace std;
//
//void printMemory(Memory);
//void decode(string);
//void execute(int, string);
//
//int AR = 0;						// arithmetic register
//int MQ = 0;						// multiplication register
//
//Memory memPool;					// memory object
//
////Fetch-Execute variables
//int PC = 0;						// program counter
//int MAR = 0;					// memory address register
//string MBR = "";				// memory buffer register
//string IR = "";					// instruction register
//
//int main()
//{
//
//int beginAdr = 1000;			// location of begin statement
//int haltAdr = 1000;				// location of halt statement
//
//string fName;					// file name
//cmatch res;						// regex results
//
////Testing
//Testing test;
//string onCreateTest = test.memoryEmpty(memPool);
///*
//string goodTest = "file.txt";
//string badTestBegin = "fileNoBegin.txt";
//string badTestHalt = "fileNoHalt.txt";
//
//
////Read file name, open file
//std::ifstream inputFile;
//do
//{
////read in name
//cout << "Enter file name: \n";
//cin >> fName;
//
////open file
//inputFile.open(fName);
//
////OPT: open file -> do while loop?
//while (!inputFile.is_open())
//{
////output error
//std::cout << "Could not open file. Enter a new file name: \n";
////get new file name
//cin >> fName;
////open new file
//inputFile.open(fName);
//}
//
//cout << "File opened successfully.\n\n";
//memPool.clear();
//
//
////Read in file
//regex lineNumber("(\\d+)\\s*(.*)");
///*Note to self - regex quantifiers
//+ 1 or more
//? 0 or 1
//* 0 or more
//
//string input;
//int lineNum;
//string toStore;
//while (inputFile.good())
//{
//lineNum = -1;
//toStore = "";
//
//getline(inputFile, input);
//
////separate line number from data
//regex_search(input.c_str(), res, lineNumber);
//if (res.size() >= 1)
//lineNum = stoi(res.str());
//
//if (res.size() >= 2)
//toStore = res[2].str();
//
////store the rest to memory
//if (lineNum >= 0 && lineNum < 1000)
//{
//memPool.set(lineNum, toStore);
//}
//}
//inputFile.close();
//
////Print Starting Memory
//cout << "Starting Memory: \n";
//printMemory(memPool);
//
////Find begin
////OPT: check findBegin for multiples
//regex beginStr("begin.*");
//for (beginAdr = 0; beginAdr < 1000; beginAdr++)
//{
//if (regex_search(memPool.get(beginAdr).c_str(), res, beginStr))
//{
//break;
//}
//}
//
////Find halt
////OPT: check findHalt for multiples
//regex haltStr("halt.*");
//for (haltAdr = 0; haltAdr < 1000; haltAdr++)
//{
//if (regex_search(memPool.get(haltAdr).c_str(), res, haltStr))
//{
//break;
//}
//}
//
//if (beginAdr == 1000)
//cout << "Could not find begin statement." << endl;
//if (haltAdr == 1000)
//cout << "Could not find halt statement." << endl;
//
//cout << endl;
//} while (beginAdr == 1000 || haltAdr == 1000);
//
//// Fetch
//PC = beginAdr;				// start at "begin"
//cout << "Execution: \n";
//while (PC != haltAdr)		// stop at "halt"
//{
//MAR = PC;				// MAR <- (PC)
//MBR = memPool.get(MAR);	// MBR <- (memory)
//
//cout << "\nPC: " << PC << " IR: " << IR;
//
//PC++;					// PC <- (PC) + 1
//IR = MBR;				// IR <- (MBR)
//decode(IR);
//
//cout << "\nPC: " << PC << " AR: " << AR << " MQ: " << MQ << endl;
//}
//
//
//// TODO: Execute
//
//
////Print Ending Memory
//cout << "\nEnding Memory: \n";
//printMemory(memPool);
//
//cout << "\nTesting: \n";
//Memory emptyMem;
////Test memory class
//cout << "On create, " << onCreateTest;
//cout << "expected fail: " << test.memoryEmpty(memPool); //rewrite intuitive
//cout << test.memoryEmpty(emptyMem);
//cout << test.memorySet(memPool);
////test clear
////Test read in file
//cout << "true data: " << test.fileSaved(fName, memPool, true);
//cout << "empty memory: " << test.fileSaved(fName, emptyMem, false);
//
//cout << "\nPress any key to exit.\n";
//cin.clear();
//cin.ignore(100, '\n');
//cin.get();
//}
//
//void printMemory(Memory memory)
//{
//string line;
//for (int i = 0; i < 1000; i++)
//{
//line = memory.get(i);
//if (line != "nop")
//{
//cout << i << " " << line << endl;
//}
//}
//cout << endl;
//}
//
//void decode(string instruction)
//{
//// TODO: Test decode
///*							Op values
//-load MQ					1
//- load MQ, M(X)				2
//- stor M(X)					3
//- load M(X)					4
//- load - M(X)				5
//- load | M(X) |				6
//-load - | M(X) |			7
//-halt
//- begin
//- . (comment, but only as a single line without any assembly code)
//- variables / constants
//- nop(no operation)
//
//cmatch res;
//regex nop("nop");
//regex comment("\\..*");
//regex loadMQ("load MQ"); //OPT: check size, no mqmx
//regex loadMQMx("load MQ,\\s+M\\((\\d)\\)");
//regex storMx("stor M\\((\\d)\\)");
//regex load("load M\\((\\d)\\)");
//regex loadNeg("load\\s*-\\s*M\\((\\d)\\)");
//regex loadAbs("load\\s*\\|\\s*M\\((\\d)\\)\\s*\\|");
//regex loadAbsNeg("load\\s*-\\s*\\|\\s*M\\((\\d)\\)\\s*\\|");
//
//if (regex_search(instruction.c_str(), res, nop) || instruction == "")
//{
//execute(0, res[1]);
//}
//else if (regex_search(instruction.c_str(), res, comment))
//{
////cout << "Comment found.\n";
//execute(0, res[1]);
//}
//
//else if (regex_search(instruction.c_str(), res, loadMQMx))
//{
////cout << "Load " << res[1] << " to MQ.\n";
//execute(2, res[1]);
//}
//
//else if (regex_search(instruction.c_str(), res, loadMQ))
//{
////cout << "Load multiplication register.\n";
//execute(1, res[1]);
//}
//
//else if (regex_search(instruction.c_str(), res, storMx))
//{
////cout << "Store " << res[1] << endl;
//execute(3, res[1]);
//}
//
//else if (regex_search(instruction.c_str(), res, load))
//{
////cout << "Load " << res[1] << endl;
//execute(4, res[1]);
//}
//
//else if (regex_search(instruction.c_str(), res, loadNeg))
//{
////cout << "Load " << res[1] << " * -1.\n";
//execute(5, res[1]);
//}
//
//else if (regex_search(instruction.c_str(), res, loadAbs))
//{
////cout << "Load abs of " << res[1] << endl;
//execute(6, res[1]);
//}
//
//else if (regex_search(instruction.c_str(), res, loadAbsNeg))
//{
////cout << "Load neg abs of " << res[1] << endl;
//execute(7, res[1]);
//}
//}
//
//void execute(int op, string variable)
//{
//int var;
//if (variable == "" || variable == "0")
//{
//var = 0;
//}
//else
//{
//var = atoi(variable.c_str());
//}
//
////cout << "var: " << var << endl;
//string fromMem = memPool.get(var);
//int intFromMem;
//if (fromMem.empty() || !isdigit(fromMem[0])) //TODO: doesn't catch 0
//intFromMem = -999;
//else
//intFromMem = stoi(fromMem);
//
//switch (op)
//{
//case 1:
//AR = MQ;
//break;
//case 2:
//MQ = var;
//break;
//case 3:
//memPool.set(AR, variable);
//break;
//case 4:
//AR = intFromMem;
//break;
//case 5:
//AR = -1 * intFromMem;
//break;
//case 6:
//AR = abs(intFromMem);
//break;
//case 7:
//AR = -1 * abs(intFromMem);
//default:
//break;
//}
//}
//*/