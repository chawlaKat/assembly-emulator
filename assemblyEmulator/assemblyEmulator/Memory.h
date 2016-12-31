#pragma once
#include<string>

using namespace std;

//need private 1000-string array, default nop

//need constructor
	// add method
	// get method

class Memory
{
private:
	std::string memoryPool[1000];

public:
	Memory::Memory()
	{
		for (int i=0; i < 1000; i++)
		{
			memoryPool[i] = "nop";
		}
	}
	
	std::string Memory::get(int index)
	{
		return memoryPool[index];
	}

	void Memory::set(int index, std::string data)
	{
		memoryPool[index] = data;
	}

	void Memory::clear()
	{
		for (int i = 0; i < 1000; i++)
		{
			memoryPool[i] = "nop";
		}
	}
};