// Kathy Chawla			30 Nov 2016			handles hw 9 expected strings, 
//											some input validation
//						2 Dec 2016			hw 9 corrections (moved pc++, 
//												fixed initial print, added 
//												extra halt message)
//											added jump instruction
//						7 Dec 2016			added remaining hw 10 instructions


/*  Assignment

Implement a simulator for the IAS assembly language, following the 
fetch-execute cycle.

Valid instructions

Homework 9:
load MQ					load value in MR into AR
load MQ, M(X)			load value in M(X) into MR
stor M(X)				store AR into M(X)
load M(X)				load value in M(X) into AR
load - M(X)				load (value in M(X)) * -1 into AR
load |M(X)|				load absolute value of M(X) into AR
load - |M(X)|			load (absolute value of M(X)) * -1 into AR
halt					last line of program; stop fetch-execute, 
							print final memory
begin					first line of program; begin PC here
.						comment as a single line with no assembly code, 
							do nothing
variables / constants
nop						no operation

						AR: accumulator
						MR: multiplication register
						M(X): value in location x

Homework 10: 
jump M(X)				PC = X
jump+ M(X)				If the AR is nonnegative, then PC = X
add M(X)				Add M(X) to AR
add |M(X)|				Add |M(X)| to AR
sub M(X)				Subtract M(X) from AR
sub |M(X)|				Subtract |M(X)| from AR
mul M(X)				Multiply M(X) by MQ and put the result in AR
div M(X)				Divide AC by M(X), put the result in MQ 
							and the remainder in AR
lsh						Multiply AC by 2 (shift left one bit position)
rsh						Divide AC by 2 (shift right one position)

Requirements:
1) Follow fetch-execute cycle
	- Uses "registers" PC, IR, MAR, and MBR, implemented as global variables
	- Does not need a separate class
2) Implement memory as a class
	- Contains get and set methods
	- Memory is an array of 1000 strings
	- Not directly accessible from main
	- Default value for memory is nop
	- If the same line number appears more than once, keeps only last instance
3) The decode method must be separate from main
	- Is the Control Unit
	- If the instruction cannot be decoded, method fails, outputs an error 
		message, and stops execution
	- Uses regular expressions
	- Interprets instructions, sends them to execute as two parameters
		Example: decode("load -|M(21)|") passes 7 and 231 as parameters to 
		execute, where 7 means load address location 21 and make the value 
		negative.
	- Begin, halt, nop, and comments do not call execute
4) The execute method must be separate from main
	- Called by decode, as detailed above
	- Takes two parameters	
	- Performs specified instruction
5) Read a user-specified file at run time, then load it into the memory object
	- Stores code in index specified by line number
	- Does not store line numbers
	- Uses regular expressions
6) After file is loaded:
	- Locates "begin" in memory
	- Sets PC equal to begin's location
	- Begins fetch execute cycle
7) Produces the following output:
	- The initial memory contents
	- Execution of each cycle on two lines:
	i. PC and IR			printed after the instruction is fetched, but 
							before PC increments
	ii. PC, AR, and MQ		printed after the instruction is executed
	- The final memory contents from 0 to halt with line numbers
8) Stop execution when a "halt" is decoded

*/

#include<iostream>
#include<fstream>
#include<string>
#include<regex>
#include"Memory.h"

using namespace std;

void decode(string);
void execute(int, int);
void waitForUser();

// Registers
int PC = 0;									// program counter
int MAR = 0;								// memory address register
string MBR = "";							// memory buffer register
string IR = "";								// instruction register

int AR = 0;									// accumulator register
int MQ = 0;									// multiplier/quotient register

Memory memPool;								// memory object

int main()
{
	int beginAdr = 0;						// location of begin
	string fName;							// user-specified file name
	ifstream inputFile;

	do
	{
		// read in name
		cout << "Enter file name: \n";
		cin >> fName;

		inputFile.open(fName);

		if (!inputFile.is_open())
		{
			cout << "Could not open file.\n\n";
		}
	} while (!inputFile.is_open());			// validate file name

	cout << "File opened successfully.\n\n";
	memPool.clear();

	// Read and print file
	regex parseInput("(\\d+)\\s*(.*)");		// line num, whitespace, instruct
											/* Note to self: regex quantifiers
												+ 1 or more
												? 0 or 1
												* 0 or more
											*/
	int lineNum;
	string toStore;
	string fromFile;
	cmatch results;							// results from regex

	cout << "Initial Memory: \n";

	while (inputFile.good())				// until end of file
	{
		lineNum = -1;						// flag if line has incorrect form
		toStore = "nop";					// if bad string, store default

		getline(inputFile, fromFile);
		cout << fromFile << endl;

		// parse input
		regex_search(fromFile.c_str(), results, parseInput);

		// save any matches
		if (results.size() >= 1)
			lineNum = stoi(results.str());
		if (results.size() >= 2)
			toStore = results[2].str();

		// avoid storing blank strings, use "nop" instead
		if (toStore == "")
			toStore = "nop";

		// if relevant and in range, store
		if (toStore != "nop" && lineNum >= 0 && lineNum < 1000)
			memPool.set(lineNum, toStore);
	}

	inputFile.close();
	cout << endl;

	// Find begin
	regex beginStr("begin.*");				// "begin", followed by anything
	for (beginAdr = 0; beginAdr < 1000; beginAdr++)
	{
		if (regex_search(memPool.get(beginAdr).c_str(), results, beginStr))
			break;
	}

	if (beginAdr == 1000)
	{
		cout << "Could not find begin statement." << endl;
		waitForUser();
		return 0;
	}

	// Fetch
	PC = beginAdr;
	cout << "Execution: \n";
	do
	{
		MAR = PC;							// MAR <- (PC)
		MBR = memPool.get(MAR);				// MBR <- (memory)
		IR = MBR;							// IR <- (MBR)

		cout << "PC: " << PC << "\tIR: " << IR << endl;

		PC++;								// PC <- (PC) + 1

		decode(IR); 
		if (IR == "broken")					// unrecognized instruction, quit
		{
			cout << "\nInstruction not recognized.\n";
			waitForUser();
			return 0;
		}

		cout << "PC: " << PC << "\tAR: " << AR << "\tMQ: " << MQ << endl 
			<< endl;

	} while (IR != "halt" && PC < 1000);	// not halt or in working memory

	if (IR == "halt")
	{
		cout << "Halt decoded. Stopping execution.\n\n";
	}

	if (PC == 1000)							// out of memory bounds, quit
	{
		cout << "No halt found. Memory out of bounds.\n";
		waitForUser();
		return 0;
	}

	// Print Final Memory
	cout << "Final Memory: \n";
	for (int i = 0; i < PC; i++)
	{
		cout << i << " " << memPool.get(i) << endl;
	}
	cout << endl;

	waitForUser();
	return 0;
}

void decode(string inst)
{
	cmatch res;
													// no op			no call
	regex comment("\\..*");							// comment			no call
	regex begin("begin.*");							// begin			no call
	regex halt("halt.*");							// halt				no call
	regex loadMQ("load\\s*MQ");						// load MQ			1
	regex loadMQMX("load\\s*MQ,\\s*M\\((\\d+)\\)");	// load MQ,M(X)		2
	regex storMX("stor\\s*M\\((\\d+)\\)");			// stor M(X)		3
	regex load("load\\s*M\\((\\d+)\\)");			// load M(X)		4
	regex loadNeg("load\\s*-\\s*M\\s*\\((\\d+)\\)");// load -M(X)		5
	regex loadAbs									// load |M(X)|		6
		("load\\s*\\|\\s*M\\s*\\((\\d+)\\)\\s*\\|");
	regex loadNabs									// load -|M(X)|		7
		("load\\s*-\\s*\\|\\s*M\\((\\d+)\\)\\s*\\|");		
	regex jump("jump\\s*M\\((\\d+)\\)");			// jump M(X)		8
	regex jumpPlus("jump\\+\\s*M\\((\\d+)\\)"); 	// jump+ M(X)		9
	regex add("add\\s*M\\((\\d+)\\)");				// add M(X)			10
	regex addAbs("add\\s*\\|\\s*M\\((\\d+)\\)\\s*\\|");
													// add |M(X)|		11
	regex sub("sub\\s*M\\((\\d+)\\)");				// sub M(X)			12
	regex subAbs("sub\\s*\\|\\s*M\\s*\\((\\d+)\\)\\s*\\|");
													// sub |M(X)|		13
	regex mul("mul\\s*M\\((\\d+)\\)");				// mul M(X)			14
	regex div("div\\s*M\\((\\d+)\\)");				// div M(X)			15

	if (inst == "nop"
		|| regex_search(inst.c_str(), res, comment)
		|| regex_search(inst.c_str(), res, begin));
	else if (regex_search(inst.c_str(), res, halt))
		IR = "halt";
	else if (regex_search(inst.c_str(), res, loadMQMX))
		execute(2, stoi(res[1]));
	else if (regex_search(inst.c_str(), res, loadMQ))
		execute(1, 0);
	else if (regex_search(inst.c_str(), res, storMX))
		execute(3, stoi(res[1]));
	else if (regex_search(inst.c_str(), res, load))
		execute(4, stoi(res[1]));
	else if (regex_search(inst.c_str(), res, loadNeg))
		execute(5, stoi(res[1]));
	else if (regex_search(inst.c_str(), res, loadAbs))
		execute(6, stoi(res[1]));
	else if (regex_search(inst.c_str(), res, loadNabs))
		execute(7, stoi(res[1]));
	else if (regex_search(inst.c_str(), res, jump))
		execute(8, stoi(res[1]));
	else if (regex_search(inst.c_str(), res, jumpPlus))
		execute(9, stoi(res[1]));
	else if (regex_search(inst.c_str(), res, add))
		execute(10, stoi(res[1]));
	else if (regex_search(inst.c_str(), res, addAbs))
		execute(11, stoi(res[1]));
	else if (regex_search(inst.c_str(), res, sub))
		execute(12, stoi(res[1]));
	else if (regex_search(inst.c_str(), res, subAbs))
		execute(13, stoi(res[1]));
	else if (regex_search(inst.c_str(), res, mul))
		execute(14, stoi(res[1]));
	else if (regex_search(inst.c_str(), res, div))
		execute(15, stoi(res[1]));
	else if (inst == "lsh" || inst == "LSH")
		execute(16, -1);
	else if (inst == "rsh" || inst == "RSH")
		execute(17, -1);
	else
		IR = "broken";
}

void execute(int op, int val)
{
	string fromMemStr = "";
	int fromMem = 0;
	if (op != 1 && op != 3 && op != 8 && op !=9 && op != 16 && op != 17)
	{
		fromMemStr = memPool.get(val);
		if (fromMemStr.find_first_of("0123456789") != string::npos)
			fromMem = stoi(memPool.get(val));
	}

	switch (op)
	{
	case 1:
		AR = MQ;
		break;
	case 2:
		MQ = fromMem;
		break;
	case 3:
		memPool.set(val, to_string(AR));
		break;
	case 4:
		AR = fromMem;
		break;
	case 5:
		AR = -1 * fromMem;
		break;
	case 6:
		if (fromMem >= 0)
			AR = fromMem;
		else
			AR = -1 * fromMem;
		break;
	case 7:
		if (fromMem >= 0)
			AR = -1 * fromMem;
		else
			AR = fromMem;
		break;
	case 8:
		PC = val;
		break;
	case 9:
		if (AR >= 0)
			PC = val;
		break;
	case 10:
		AR += fromMem;
		break;
	case 11:
		if (fromMem < 0)
			AR -= fromMem;
		else
			AR += fromMem;
		break;
	case 12:
		fromMem *= -1;
		AR += fromMem;
		break;
	case 13:
		if (fromMem > 0)
			AR -= fromMem;
		else
			AR += fromMem;
		break;
	case 14:
		AR = fromMem * MQ;
		break;
	case 15:
		if (fromMem != 0)
		{
			MQ = AR / fromMem;
			AR /= fromMem;
		}
		else
			AR = 0;
		break;
	case 16:
		AR *= 2;
		break;
	case 17:
		AR /= 2;
		break;
	}
}

void waitForUser()
{
	cout << "Program exiting. Press return or enter any text to quit.\n";
	cin.clear();							// restores cin if bad
	cin.ignore(256, '\n');					// ignores anything left in cin
	cin.get();								// forces program to wait for user
}