#ifndef OUTPUT_RELOC_H
#define OUTPUT_RELOC_H

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

class OutputReloc
{
private:
	int offset;
	string type;
	int value;
	string data_instr;
	int sectionAddr;
	bool section;
public:
	OutputReloc() :
		offset(0), type(""), value(0), data_instr(""), sectionAddr(0), section(false)
	{	}
	OutputReloc(int offset, string type, int value, string data_instr) : 
		offset(offset), type(type), value(value), data_instr(data_instr), sectionAddr(0), section(false)
	{	}

	int getOffset();
	void setOffset(int offset);

	string getType();

	int getValue();
	void setValue(int value);

	string getDataInstr();

	int getSectionAddr();
	void setSectionAddr(int sectionAddr);

	bool getSection();
	void setSection(bool section);

	void printOutputReloc(ofstream& outputFile);
	void printOutputReloc();

	void writeBinary(ofstream& outputBinary);
	void readBinary(ifstream& inputBinary);

};

#endif