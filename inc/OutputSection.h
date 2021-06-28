#ifndef OUTPUT_SECTION_H
#define OUTPUT_SECTION_H

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

#include "OutputReloc.h"
#include "SymbolTable.h"

using namespace std;

class OutputSection
{
private:
	string name;
	int ordinal;
	int lc = 0;
	vector<string> byteData;
	vector<OutputReloc> relocVector;
public:
	OutputSection() :
		name(""), ordinal(0)
	{	}
	OutputSection(string name, int ordinal) :
		name(name), ordinal(ordinal)
	{	}
	
	string getName();
	int getLc();
	void setLc(int lc);
	void printByteData(ofstream& outputFile);
	void printByteData();

	void printByteDataHex(ofstream& outputFile, SymbolTable& symbolTable);

	void printRelocData(ofstream& outputFile);
	void printRelocData();
	void insertByteData(string byte);
	void insertRelocData(string type, int value, string data_instr);
	void insertRelocData(OutputReloc outputReloc);

	int getOrdinal();

	void writeBinary(ofstream& outputBinary);
	void readBinary(ifstream& inputBinary);

	vector<string>& getByteData();
	vector<OutputReloc>& getRelocVector();
	void setRelocVector(vector<OutputReloc> relocVector);
};

#endif

