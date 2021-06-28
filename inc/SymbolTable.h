#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <iomanip>

#include "Row.h"

using namespace std;

class SymbolTable
{
private:
	vector<Row> symbolList;
public:
	void insertSymbol(Row row);
	void print(ofstream& outputFile);
	void print();
	void rearrangeData();
	void writeBinary(ofstream& outputBinary);
	void readBinary(ifstream& inputBinary);

	vector<Row>& getSymbolList();

	bool existsSymbolByName(string name);
	string getSymbolNameByNumber(int ordinal);
	int getSymbolValueByNumber(int ordinal);
	int getSymbolValueByName(string name);
	int getSymbolNumberByName(string name);

	int getSymbolSectionNumberByNumber(int ordinal);

	bool isSection(string name);

	bool isGlobalOrExtern(int ordinal);

	bool isAbsolute(int ordinal);
	
};

#endif

