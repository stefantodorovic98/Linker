#ifndef ROW_H
#define ROW_H

#include <iostream>
#include <string>
#include <fstream>

#include <iomanip>

using namespace std;

class Row
{
private:
	string name;
	int section;
	int value;
	string visibility;
	int ordinal;
	int sectionSize;
	bool ext;
	bool glb;
public:
	Row() :
		name(""), section(0), value(0), visibility(""), ordinal(0), sectionSize(0), ext(false), glb(false)
	{	}
	Row(string name, int section, int value, string visibility, int ordinal, int sectionSize, bool ext, bool glb) : 
		name(name), section(section), value(value), visibility(visibility), ordinal(ordinal), sectionSize(sectionSize), ext(ext), glb(glb)
	{	}

	string getName();

	int getSection();
	void setSection(int section);

	int getValue();
	void setValue(int value);

	string getVisibility();
	void setVisibility(string visibility);

	int getOrdinal();
	void setOrdinal(int ordinal);

	int getSectionSize();
	void setSectionSize(int sectionSize);

	bool getExt();
	bool getGlb();

	void print(ofstream& outputFile);
	void print();
	
	void writeBinary(ofstream& outputBinary);
	void readBinary(ifstream& inputBinary);
};

#endif

