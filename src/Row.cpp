#include "Row.h"

string Row::getName()
{
	return name;
}

int Row::getSection()
{
	return section;
}

void Row::setSection(int section)
{
	this->section = section;
}

int Row::getValue()
{
	return value;
}

void Row::setValue(int value)
{
	this->value = value;
}

string Row::getVisibility()
{
	return visibility;
}

void Row::setVisibility(string visibility)
{
	this->visibility = visibility;
}

int Row::getOrdinal()
{
	return ordinal;
}

void Row::setOrdinal(int ordinal)
{
	this->ordinal = ordinal;
}

int Row::getSectionSize()
{
	return sectionSize;
}

void Row::setSectionSize(int sectionSize)
{
	this->sectionSize = sectionSize;
}

bool Row::getExt()
{
	return ext;
}

bool Row::getGlb()
{
	return glb;
}


void Row::print(ofstream& outputFile)
{
	outputFile << setw(20) << left << name << setw(10) << left << hex << section;
	outputFile << setw(10) << left << hex << value << setw(20) << left << visibility;
	outputFile << setw(10) << left << hex << ordinal << setw(10) << left << hex << sectionSize << endl;
}

void Row::print()
{
	cout << setw(20) << left << name << setw(10) << left << hex << section;
	cout << setw(10) << left << hex << value << setw(20) << left << visibility;
	cout << setw(10) << left << hex << ordinal << setw(10) << left << hex << sectionSize << endl;
}

void Row::writeBinary(ofstream& outputBinary)
{
	unsigned int nameSize = name.size();
	outputBinary.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
	outputBinary.write(name.c_str(), nameSize);
	outputBinary.write(reinterpret_cast<char*>(&section), sizeof(section));
	outputBinary.write(reinterpret_cast<char*>(&value), sizeof(value));
	unsigned int visibilitySize = visibility.size();
	outputBinary.write(reinterpret_cast<char*>(&visibilitySize), sizeof(visibilitySize));
	outputBinary.write(visibility.c_str(), visibilitySize);
	outputBinary.write(reinterpret_cast<char*>(&ordinal), sizeof(ordinal));
	outputBinary.write(reinterpret_cast<char*>(&sectionSize), sizeof(sectionSize));
	outputBinary.write(reinterpret_cast<char*>(&ext), sizeof(ext));
	outputBinary.write(reinterpret_cast<char*>(&glb), sizeof(glb));
}

void Row::readBinary(ifstream& inputBinary)
{
	unsigned int nameSize = 0;
	inputBinary.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
	char* nameBuffer = new char[nameSize + 1];
	inputBinary.read(nameBuffer, nameSize);
	nameBuffer[nameSize] = '\0';
	name = nameBuffer;
	delete[] nameBuffer;
	inputBinary.read(reinterpret_cast<char*>(&section), sizeof(section));
	inputBinary.read(reinterpret_cast<char*>(&value), sizeof(value));
	unsigned int visibilitySize = 0;
	inputBinary.read(reinterpret_cast<char*>(&visibilitySize), sizeof(visibilitySize));
	char* visibilityBuffer = new char[visibilitySize + 1];
	inputBinary.read(visibilityBuffer, visibilitySize);
	visibilityBuffer[visibilitySize] = '\0';
	visibility = visibilityBuffer;
	delete[] visibilityBuffer;
	inputBinary.read(reinterpret_cast<char*>(&ordinal), sizeof(ordinal));
	inputBinary.read(reinterpret_cast<char*>(&sectionSize), sizeof(sectionSize));
	inputBinary.read(reinterpret_cast<char*>(&ext), sizeof(ext));
	inputBinary.read(reinterpret_cast<char*>(&glb), sizeof(glb));
}


