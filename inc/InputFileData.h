#ifndef INPUT_FILE_DATA_H
#define INPUT_FILE_DATA_H

#include <iostream>

#include "SymbolTable.h"
#include "OutputData.h"

using namespace std;

class InputFileData
{
private:
	string fileName;
	SymbolTable symbolTable;
	OutputData outputData;
public:
	InputFileData(string fileName, SymbolTable symbolTable, OutputData outputData) :
		fileName(fileName), symbolTable(symbolTable), outputData(outputData)
	{	}
	
	string getFileName();
	SymbolTable& getSymbolTable();
	OutputData& getOutputData();
};

#endif
