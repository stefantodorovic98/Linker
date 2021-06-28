#include "SymbolTable.h"

void SymbolTable::insertSymbol(Row row)
{
	symbolList.push_back(row);
}

void SymbolTable::print(ofstream& outputFile)
{
	outputFile << "#" << " Symbol Table " << endl;
	outputFile << setw(20) << left << "name" << setw(10) << left << "section";
	outputFile << setw(10) << left << "value" << setw(20) << left << "visibility";
	outputFile << setw(10) << left << "ordinal" << setw(10) << left << "sectionSize" << endl;
	for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
	{
		(*it).print(outputFile);
	}
	outputFile << endl;
}

void SymbolTable::print()
{
	cout << "#" << " Symbol Table " << endl;
	cout << setw(20) << left << "name" << setw(10) << left << "section";
	cout << setw(10) << left << "value" << setw(20) << left << "visibility";
	cout << setw(10) << left << "ordinal" << setw(10) << left << "sectionSize" << endl;
	for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
	{
		(*it).print();
	}
	cout << endl;
}

void SymbolTable::rearrangeData()
{
	vector<Row> temp;
	for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
	{
		if ((*it).getSection() == (*it).getOrdinal())
		{
			temp.push_back((*it));
		}
	}
	for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
	{
		if ((*it).getSection() != (*it).getOrdinal())
		{
			temp.push_back((*it));
		}
	}
	int ordinal = 0;
	for (vector<Row>::iterator it = temp.begin(); it != temp.end(); ++it)
	{
		if ((*it).getSection() == (*it).getOrdinal())
		{
			for (vector<Row>::iterator itt = temp.begin(); itt != temp.end(); ++itt)
			{
				if ((*itt).getOrdinal() != (*it).getOrdinal() && (*itt).getSection() == (*it).getOrdinal())
				{
					(*itt).setSection(ordinal);
					(*itt).setOrdinal(-10);
				}

			}
			(*it).setSection(ordinal);
		}
		(*it).setOrdinal(ordinal);
		
		ordinal++;
	}
	symbolList = temp;
}


void SymbolTable::writeBinary(ofstream& outputBinary)
{
	unsigned int size = symbolList.size();
	outputBinary.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
	{
		(*it).writeBinary(outputBinary);
	}
}

void SymbolTable::readBinary(ifstream& inputBinary)
{
	unsigned int size = 0;
	inputBinary.read(reinterpret_cast<char*>(&size), sizeof(size));
	for (unsigned int i = 0; i < size; i++)
	{
		Row r;
		r.readBinary(inputBinary);
		symbolList.push_back(r);
	}
}

vector<Row>& SymbolTable::getSymbolList()
{
	return symbolList;
}

bool SymbolTable::existsSymbolByName(string name)
{
	for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
	{
		if ((*it).getName() == name)
		{
			return true;
		}
	}
	return false;
}

string SymbolTable::getSymbolNameByNumber(int ordinal)
{
	for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
	{
		if ((*it).getOrdinal() == ordinal)
		{
			return (*it).getName();
		}
	}
	return "";
}

int SymbolTable::getSymbolValueByNumber(int ordinal)
{
	for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
	{
		if ((*it).getOrdinal() == ordinal)
		{
			return (*it).getValue();
		}
	}
	return -1;
}

int SymbolTable::getSymbolValueByName(string name)
{
	for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
	{
		if ((*it).getName() == name)
		{
			return (*it).getValue();
		}
	}
	return -1;
}

int SymbolTable::getSymbolNumberByName(string name)
{
	for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
	{
		if ((*it).getName() == name)
		{
			return (*it).getOrdinal();
		}
	}
	return -1;
}

int SymbolTable::getSymbolSectionNumberByNumber(int ordinal)
{
	for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
	{
		if ((*it).getOrdinal() == ordinal)
		{
			return (*it).getSection();
		}
	}
	return -1;
}

bool SymbolTable::isSection(string name)
{
	for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
	{
		if ((*it).getName() == name)
		{
			if ((*it).getOrdinal() == (*it).getSection())
			{
				return true;
			}
		}
	}
	return false;
}

bool SymbolTable::isGlobalOrExtern(int ordinal)
{
	for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
	{
		if ((*it).getOrdinal() == ordinal)
		{
			if ((*it).getExt() == true || (*it).getGlb() == true)
			{
				return true;
			}
		}
	}
	return false;
}

bool SymbolTable::isAbsolute(int ordinal)
{
	for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
	{
		if ((*it).getOrdinal() == ordinal)
		{
			if ((*it).getSection() == 1)
			{
				return true;
			}
		}
	}
	return false;
}
