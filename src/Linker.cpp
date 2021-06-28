#include "Linker.h"

void Linker::linkerJob(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut, OutputData& outputDataOut, bool& condition)
{
	int ordinal = 0;
	//Sections
	sectionProcessing(inputDataVector, symbolTableOut, ordinal);

	//Symbols
	symbolProcessing(inputDataVector, symbolTableOut, condition, ordinal);

	//Reloc data
	outputDataProcessing(inputDataVector, symbolTableOut, outputDataOut, condition);

	updateByteDataWithRelocData(symbolTableOut, outputDataOut, condition);

	symbolTableOut.print();
	outputDataOut.printRelocData();
	outputDataOut.printByteData();
}

void Linker::sectionProcessing(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut, int& ordinal)
{
	Row und("UND", 0, 0, "local", ordinal++, 0, false, false);
	symbolTableOut.insertSymbol(und);
	Row abs("ABS", 1, 0, "local", ordinal++, 0, false, false);
	symbolTableOut.insertSymbol(abs);
	
	int counter = 0;

	for (unsigned int i = 0; i < inputDataVector.size(); i++)
	{
		vector<Row>& symbolList = inputDataVector[i].getSymbolTable().getSymbolList();

		for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
		{
			if (((*it).getOrdinal() == (*it).getSection()) && ((*it).getName() != "UND" && (*it).getName() != "ABS")
				&& (!symbolTableOut.existsSymbolByName((*it).getName())))
			{
					(*it).setValue(counter);
					counter = counter + (*it).getSectionSize();
					string sectionName = (*it).getName();
					unsigned int fileIndex = i;
					int size = (*it).getSectionSize();

					processSameSectionInOtherFiles(inputDataVector, sectionName, fileIndex, counter, size);

					Row r = (*it);
					r.setSectionSize(size);
					r.setSection(ordinal);
					r.setOrdinal(ordinal++);
					symbolTableOut.insertSymbol(r);
			}
		}
	}

	for (unsigned int i = 0; i < inputDataVector.size(); i++)
	{
		SymbolTable& symbolTable = inputDataVector[i].getSymbolTable();
		OutputData& outputData = inputDataVector[i].getOutputData();
		symbolTable.print();
		outputData.printRelocData();
		outputData.printByteData();
	}
}

void Linker::processSameSectionInOtherFiles(vector<InputFileData>& inputDataVector, string sectionName,
	unsigned int fileIndex, int& counter, int& size)
{
	for (unsigned int j = fileIndex + 1; j < inputDataVector.size(); j++)
	{
		vector<Row>& symbolList = inputDataVector[j].getSymbolTable().getSymbolList();

		for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end(); ++it)
		{
			if (((*it).getOrdinal() == (*it).getSection()) && ((*it).getName() != "UND" && (*it).getName() != "ABS")
				&& ((*it).getName() == sectionName))
			{
				(*it).setValue(counter);
				counter = counter + (*it).getSectionSize();
				size += (*it).getSectionSize();
			}
		}
	}
}

void Linker::symbolProcessing(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut, bool& condition, int& ordinal)
{
	vector<string> undefinedSymbols;
	for (unsigned int i = 0; i < inputDataVector.size() && condition; i++)
	{
		SymbolTable& symbolTable = inputDataVector[i].getSymbolTable();
		vector<Row>& symbolList = symbolTable.getSymbolList();

		for (vector<Row>::iterator it = symbolList.begin(); it != symbolList.end() && condition; ++it)
		{
			if (((*it).getSection() != 0) && ((*it).getGlb()) && ((*it).getOrdinal() != (*it).getSection()))
			{
				if (!symbolTableOut.existsSymbolByName((*it).getName()))
				{
					string sectionName = symbolTable.getSymbolNameByNumber((*it).getSection());
					int sectionValue = symbolTable.getSymbolValueByNumber((*it).getSection());
					int sectionNumber = symbolTableOut.getSymbolNumberByName(sectionName);
					Row r = (*it);
					r.setSection(sectionNumber);
					r.setValue(r.getValue() + sectionValue);
					r.setOrdinal(ordinal++);
					symbolTableOut.insertSymbol(r);
				}
				else
				{
					condition = false;
					cout << "Symbol " << (*it).getName() << " has multiple definitions!" << endl;
				}
			}
			else if (((*it).getSection() == 0) && ((*it).getExt()) && ((*it).getOrdinal() != (*it).getSection()))
			{
				undefinedSymbols.push_back((*it).getName());
			}
		}
	}
	unsigned int counter = 0;
	for (vector<string>::iterator it = undefinedSymbols.begin(); it != undefinedSymbols.end(); ++it)
	{
		if (symbolTableOut.existsSymbolByName((*it)))
		{
			counter++;
		}
		else
		{
			cout << "Symbol " << (*it) << " is undefined" << endl;
		}
	}

	if (counter != undefinedSymbols.size())
	{
		condition = false;
		cout << "There are undefined symbols" << endl;
	}
}

void Linker::outputDataProcessing(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut,
	OutputData& outputDataOut, bool& condition)
{
	for (unsigned int i = 0; i < inputDataVector.size() && condition; i++)
	{
		vector<OutputSection>& outputData = inputDataVector[i].getOutputData().getDataList();
		SymbolTable& symbolTable = inputDataVector[i].getSymbolTable();

		for (vector<OutputSection>::iterator it = outputData.begin(); it != outputData.end(); ++it)
		{
			if (!outputDataOut.existsSectionByName((*it).getName()))
			{
				string sectionName = (*it).getName();
				unsigned int fileIndex = i;
				vector<OutputReloc>& relocVector = (*it).getRelocVector();
				vector<string>& byteData = (*it).getByteData();

				OutputSection outputSection(sectionName, symbolTableOut.getSymbolNumberByName(sectionName));

				for (vector<OutputReloc>::iterator itt = relocVector.begin(); itt != relocVector.end(); ++itt)
				{
					OutputReloc outputReloc = (*itt);
					int valueOfCurrentSection = symbolTable.getSymbolValueByName(sectionName);
					int addrOfSection = symbolTable.getSymbolValueByNumber(outputReloc.getValue());
					int valueOfOutSection = symbolTableOut.getSymbolValueByName(sectionName);
					outputReloc.setOffset(valueOfCurrentSection - valueOfOutSection + outputReloc.getOffset());
					string symbolName = symbolTable.getSymbolNameByNumber(outputReloc.getValue());
					int symbolNumber = symbolTableOut.getSymbolNumberByName(symbolName);
					outputReloc.setValue(symbolNumber);

					outputReloc.setSectionAddr(addrOfSection);
					if (symbolTableOut.isSection(symbolName))
					{
						outputReloc.setSection(true);
					}

					outputSection.insertRelocData(outputReloc);
				}

				for (vector<string>::iterator itt = byteData.begin(); itt != byteData.end(); ++itt)
				{
					outputSection.insertByteData((*itt));
				}

				processOutputDataInOtherFiles(inputDataVector, symbolTableOut, outputSection, sectionName, fileIndex);
				outputDataOut.insertSection(outputSection);
			}
		}
	}
}

void Linker::processOutputDataInOtherFiles(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut, OutputSection& outputSection,
	string sectionName, unsigned int fileIndex)
{
	for (unsigned int j = fileIndex + 1; j < inputDataVector.size(); j++)
	{
		vector<OutputSection>& relocSectionVector = inputDataVector[j].getOutputData().getDataList();
		SymbolTable& symbolTable = inputDataVector[j].getSymbolTable();

		for (vector<OutputSection>::iterator it = relocSectionVector.begin(); it != relocSectionVector.end(); ++it)
		{
			if ((*it).getName() == sectionName)
			{
				string sectionName = (*it).getName();
				vector<OutputReloc>& relocVector = (*it).getRelocVector();
				vector<string>& byteData = (*it).getByteData();

				for (vector<OutputReloc>::iterator itt = relocVector.begin(); itt != relocVector.end(); ++itt)
				{
					OutputReloc outputReloc = (*itt);
					int valueOfCurrentSection = symbolTable.getSymbolValueByName(sectionName);
					int addrOfSection = symbolTable.getSymbolValueByNumber(outputReloc.getValue());
					int valueOfOutSection = symbolTableOut.getSymbolValueByName(sectionName);
					outputReloc.setOffset(valueOfCurrentSection - valueOfOutSection + outputReloc.getOffset());
					string symbolName = symbolTable.getSymbolNameByNumber(outputReloc.getValue());
					int symbolNumber = symbolTableOut.getSymbolNumberByName(symbolName);
					outputReloc.setValue(symbolNumber);

					outputReloc.setSectionAddr(addrOfSection);
					if (symbolTableOut.isSection(symbolName))
					{
						outputReloc.setSection(true);
					}

					outputSection.insertRelocData(outputReloc);
				}

				for (vector<string>::iterator itt = byteData.begin(); itt != byteData.end(); ++itt)
				{
					outputSection.insertByteData((*itt));
				}
			}
		}
	}
}

void Linker::updateByteDataWithRelocData(SymbolTable& symbolTableOut, OutputData& outputDataOut, bool& condition)
{
	vector<OutputSection>& relocSectionVector = outputDataOut.getDataList();
	for (vector<OutputSection>::iterator it = relocSectionVector.begin(); it != relocSectionVector.end() && condition; ++it)
	{
		vector<OutputReloc>& relocVector = (*it).getRelocVector();
		vector<OutputReloc> tempVector;
		vector<string>& byteVector = (*it).getByteData();
		string sectionName = (*it).getName();
		for (unsigned int i = 0; i < relocVector.size(); i++)
		{
			OutputReloc& outputReloc = relocVector[i];

			if (outputReloc.getType() == "R_386_PC16")
			{
				if (outputReloc.getDataInstr() == "data")
				{
				}
				else if (outputReloc.getDataInstr() == "instr")
				{
					updateInstrWithRelRelocData(symbolTableOut, outputReloc, byteVector, sectionName);
					int sectionNumber = symbolTableOut.getSymbolNumberByName(sectionName);
					int symbolSectionNumber = symbolTableOut.getSymbolSectionNumberByNumber(outputReloc.getValue());
					if (sectionNumber != symbolSectionNumber)
					{
						tempVector.push_back(outputReloc);
					}
				}
			}
			else if (outputReloc.getType() == "R_386_16")
			{
				if (outputReloc.getDataInstr() == "data")
				{
					tempVector.push_back(outputReloc);
					updateDataWithAbsRelocData(symbolTableOut, outputReloc, byteVector);
				}
				else if (outputReloc.getDataInstr() == "instr")
				{
					tempVector.push_back(outputReloc);
					updateInstrWithAbsRelocData(symbolTableOut, outputReloc, byteVector);
				}
			}
		}

		(*it).setRelocVector(tempVector);
	}
}

void Linker::updateDataWithAbsRelocData(SymbolTable& symbolTableOut, OutputReloc& outputReloc, vector<string>& byteVector)
{
	int offset = outputReloc.getOffset();
	int ordinal = outputReloc.getValue();
	int symbolValue = symbolTableOut.getSymbolValueByNumber(ordinal);

	int value = 0;
	if (outputReloc.getSection())
	{
		value = outputReloc.getSectionAddr();
	}
	else
	{
		value = symbolValue;
	}

	int index = offset;
	string lowByte = byteVector[index];
	short int low = stoi(lowByte, nullptr, 16);
	string highByte = byteVector[index + 1];
	short int high = stoi(highByte, nullptr, 16);

	int number = ((high << 8) & 0xFF00) | (low & 0x00FF);
	number = number + value;

	stringstream ss;
	low = number & 0x00FF;
	ss << setfill('0') << setw(2) << right << hex << low;
	lowByte = ss.str();

	ss.str(string());
	ss.clear();

	high = ((number & 0xFF00) >> 8) & 0x00FF;
	ss << setfill('0') << setw(2) << right << hex << high;
	highByte = ss.str();

	byteVector[index] = lowByte;
	byteVector[index + 1] = highByte;
}

void Linker::updateInstrWithAbsRelocData(SymbolTable& symbolTableOut, OutputReloc& outputReloc, vector<string>& byteVector)
{
	int offset = outputReloc.getOffset();
	int ordinal = outputReloc.getValue();
	int symbolValue = symbolTableOut.getSymbolValueByNumber(ordinal);

	int value = 0;
	if (outputReloc.getSection())
	{
		value = outputReloc.getSectionAddr();
	}
	else
	{
		value = symbolValue;
	}

	int index = offset;
	string highByte = byteVector[index];
	short int high = stoi(highByte, nullptr, 16);
	string lowByte = byteVector[index + 1];
	short int low = stoi(lowByte, nullptr, 16);

	int number = ((high << 8) & 0xFF00) | (low & 0x00FF);
	number = number + value;

	stringstream ss;
	low = number & 0x00FF;
	ss << setfill('0') << setw(2) << right << hex << low;
	lowByte = ss.str();

	ss.str(string());
	ss.clear();

	high = ((number & 0xFF00) >> 8) & 0x00FF;
	ss << setfill('0') << setw(2) << right << hex << high;
	highByte = ss.str();

	byteVector[index] = highByte;
	byteVector[index + 1] = lowByte;
}

void Linker::updateInstrWithRelRelocData(SymbolTable& symbolTableOut, OutputReloc& outputReloc, vector<string>& byteVector, string sectionName)
{
	int offset = outputReloc.getOffset();
	int ordinal = outputReloc.getValue();
	int symbolValue = symbolTableOut.getSymbolValueByNumber(ordinal);
	int sectionValue = symbolTableOut.getSymbolValueByName(sectionName);

	int value = 0;
	if (outputReloc.getSection())
	{
		value = outputReloc.getSectionAddr();
	}
	else
	{
		value = symbolValue;
	}

	int index = offset;
	string highByte = byteVector[index];
	short int high = stoi(highByte, nullptr, 16);
	string lowByte = byteVector[index + 1];
	short int low = stoi(lowByte, nullptr, 16);

	int number = ((high << 8) & 0xFF00) | (low & 0x00FF);
	number = (number + value) - (sectionValue + offset);

	stringstream ss;
	low = number & 0x00FF;
	ss << setfill('0') << setw(2) << right << hex << low;
	lowByte = ss.str();

	ss.str(string());
	ss.clear();

	high = ((number & 0xFF00) >> 8) & 0x00FF;
	ss << setfill('0') << setw(2) << right << hex << high;
	highByte = ss.str();

	byteVector[index] = highByte;
	byteVector[index + 1] = lowByte;
}


