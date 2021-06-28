#include "OutputReloc.h"

int OutputReloc::getOffset()
{
	return offset;
}

void OutputReloc::setOffset(int offset)
{
	this->offset = offset;
}

string OutputReloc::getType()
{
	return type;
}

int OutputReloc::getValue()
{
	return value;
}

void OutputReloc::setValue(int value)
{
	this->value = value;
}

string OutputReloc::getDataInstr()
{
	return data_instr;
}

int OutputReloc::getSectionAddr()
{
	return sectionAddr;
}

void OutputReloc::setSectionAddr(int sectionAddr)
{
	this->sectionAddr = sectionAddr;
}

bool OutputReloc::getSection()
{
	return section;
}

void OutputReloc::setSection(bool section)
{
	this->section = section;
}

void OutputReloc::printOutputReloc(ofstream& outputFile)
{
	outputFile << setw(10) << left << hex << offset;
	outputFile << setw(20) << left << type;
	outputFile << setw(10) << left << hex << value << endl;
}

void OutputReloc::printOutputReloc()
{
	cout << setw(10) << left << hex << offset;
	cout << setw(20) << left << type;
	cout << setw(10) << left << hex << value << endl;
}

void OutputReloc::writeBinary(ofstream& outputBinary)
{
	outputBinary.write(reinterpret_cast<char*>(&offset), sizeof(offset));
	unsigned int typeSize = type.size();
	outputBinary.write(reinterpret_cast<char*>(&typeSize), sizeof(typeSize));
	outputBinary.write(type.c_str(), typeSize);
	outputBinary.write(reinterpret_cast<char*>(&value), sizeof(value));
	unsigned int data_instrSize = data_instr.size();
	outputBinary.write(reinterpret_cast<char*>(&data_instrSize), sizeof(data_instrSize));
	outputBinary.write(data_instr.c_str(), data_instrSize);
}

void OutputReloc::readBinary(ifstream& inputBinary)
{
	inputBinary.read(reinterpret_cast<char*>(&offset), sizeof(offset));
	unsigned int typeSize = 0;
	inputBinary.read(reinterpret_cast<char*>(&typeSize), sizeof(typeSize));
	char* typeBuffer = new char[typeSize + 1];
	inputBinary.read(typeBuffer, typeSize);
	typeBuffer[typeSize] = '\0';
	type = typeBuffer;
	delete[] typeBuffer;
	inputBinary.read(reinterpret_cast<char*>(&value), sizeof(value));
	unsigned int data_instrSize = 0;
	inputBinary.read(reinterpret_cast<char*>(&data_instrSize), sizeof(data_instrSize));
	char* data_instrBuffer = new char[data_instrSize + 1];
	inputBinary.read(data_instrBuffer, data_instrSize);
	data_instrBuffer[data_instrSize] = '\0';
	data_instr = data_instrBuffer;
	delete[] data_instrBuffer;
}