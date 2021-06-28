#include "OutputSection.h"

string OutputSection::getName()
{
	return name;
}

int OutputSection::getLc()
{
	return lc;
}

void OutputSection::setLc(int lc)
{
	this->lc = lc;
}

void OutputSection::printByteData(ofstream& outputFile)
{
	int cnt = 0;
	outputFile << "#" << name << " " << hex << lc << endl;
	for (vector<string>::iterator it = byteData.begin(); it != byteData.end(); ++it)
	{
		outputFile << (*it) << " ";
		cnt++;
		if (cnt == 8)
		{
			outputFile << endl;
			cnt = 0;
		}
	}
	outputFile << endl;
}

void OutputSection::printByteData()
{
	int cnt = 0;
	cout << "#" << name << " " << hex << lc << endl;
	for (vector<string>::iterator it = byteData.begin(); it != byteData.end(); ++it)
	{
		cout << (*it) << " ";
		cnt++;
		if (cnt == 8)
		{
			cout << endl;
			cnt = 0;
		}
	}
	cout << endl;
}

void OutputSection::printByteDataHex(ofstream& outputFile, SymbolTable& symbolTable)
{
	vector<Row> symbolList = symbolTable.getSymbolList();
	int cnt = 0;
	int index = 0;
	for (unsigned int i = 0; i < symbolList.size(); i++)
	{
		Row& symbol = symbolList[i];
		if (symbol.getName() == name)
		{
			int sectionAddress = symbol.getValue();
			int sectionSize = symbol.getSectionSize();
			if (sectionSize > 0)
			{
				outputFile << "       " << "00" << " " << "01" << " " << "02" << " " << "03";
				outputFile << " " << "04" << " " << "05" << " " << "06" << " " << "07" << endl << endl;
				outputFile << setfill('0') << setw(4) << right << hex << sectionAddress << ":  ";
				for (vector<string>::iterator it = byteData.begin(); it != byteData.end(); ++it)
				{
					outputFile << (*it) << " ";
					cnt++;
					index++;
					if (cnt == 8)
					{
						outputFile << endl;
						if (index < sectionSize)
						{
							sectionAddress += 8;
							outputFile << setfill('0') << setw(4) << right << hex << sectionAddress << ":  ";
						}
						cnt = 0;
					}
				}
			}
			
			outputFile << endl;
		}
	}
}

void OutputSection::printRelocData(ofstream& outputFile)
{
	outputFile << "#" << " rel " << name << endl;
	outputFile << setw(10) << left << "offset";
	outputFile << setw(20) << left << "type";
	outputFile << setw(10) << left << "value" << endl;
	for (vector<OutputReloc>::iterator it = relocVector.begin(); it != relocVector.end(); ++it)
	{
		(*it).printOutputReloc(outputFile);
	}
	outputFile << endl;
}

void OutputSection::printRelocData()
{
	cout << "#" << " rel " << name << endl;
	cout << setw(10) << left << "offset";
	cout << setw(20) << left << "type";
	cout << setw(10) << left << "value" << endl;
	for (vector<OutputReloc>::iterator it = relocVector.begin(); it != relocVector.end(); ++it)
	{
		(*it).printOutputReloc();
	}
	cout << endl;
}

void OutputSection::insertByteData(string byte)
{
	byteData.push_back(byte);
	lc += 1;
}

void OutputSection::insertRelocData(string type, int value, string data_instr)
{
	OutputReloc reloc(lc, type, value, data_instr);
	relocVector.push_back(reloc);
}

void OutputSection::insertRelocData(OutputReloc outputReloc)
{
	relocVector.push_back(outputReloc);
}

int OutputSection::getOrdinal()
{
	return ordinal;
}

void OutputSection::writeBinary(ofstream& outputBinary)
{
	unsigned int nameSize = name.size();
	outputBinary.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
	outputBinary.write(name.c_str(), nameSize);
	outputBinary.write(reinterpret_cast<char*>(&ordinal), sizeof(ordinal));
	outputBinary.write(reinterpret_cast<char*>(&lc), sizeof(lc));

	unsigned int byteDataSize = byteData.size();
	outputBinary.write(reinterpret_cast<char*>(&byteDataSize), sizeof(byteDataSize));
	for (vector<string>::iterator it = byteData.begin(); it != byteData.end(); ++it)
	{
		unsigned int size = (*it).size();
		outputBinary.write(reinterpret_cast<char*>(&size), sizeof(size));
		outputBinary.write((*it).c_str(), size);
	}

	unsigned int relocVectorSize = relocVector.size();
	outputBinary.write(reinterpret_cast<char*>(&relocVectorSize), sizeof(relocVectorSize));
	for (vector<OutputReloc>::iterator it = relocVector.begin(); it != relocVector.end(); ++it)
	{
		(*it).writeBinary(outputBinary);
	}
}

void OutputSection::readBinary(ifstream& inputBinary)
{
	unsigned int nameSize = 0;
	inputBinary.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
	char* nameBuffer = new char[nameSize + 1];
	inputBinary.read(nameBuffer, nameSize);
	nameBuffer[nameSize] = '\0';
	name = nameBuffer;
	delete[] nameBuffer;
	inputBinary.read(reinterpret_cast<char*>(&ordinal), sizeof(ordinal));
	inputBinary.read(reinterpret_cast<char*>(&lc), sizeof(lc));

	unsigned int byteDataSize = 0;
	inputBinary.read(reinterpret_cast<char*>(&byteDataSize), sizeof(byteDataSize));
	for (unsigned int i = 0; i < byteDataSize; i++)
	{
		string data;
		unsigned int size = 0;
		inputBinary.read(reinterpret_cast<char*>(&size), sizeof(size));
		char* dataBuffer = new char[size + 1];
		inputBinary.read(dataBuffer, size);
		dataBuffer[size] = '\0';
		data = dataBuffer;
		byteData.push_back(data);
		delete[] dataBuffer;
	}

	unsigned int relocVectorSize = 0;
	inputBinary.read(reinterpret_cast<char*>(&relocVectorSize), sizeof(relocVectorSize));
	for (unsigned int i = 0; i < relocVectorSize; i++)
	{
		OutputReloc r;
		r.readBinary(inputBinary);
		relocVector.push_back(r);
	}
}

vector<string>& OutputSection::getByteData()
{
	return byteData;
}

vector<OutputReloc>& OutputSection::getRelocVector()
{
	return relocVector;
}

void OutputSection::setRelocVector(vector<OutputReloc> relocVector)
{
	this->relocVector = relocVector;
}


