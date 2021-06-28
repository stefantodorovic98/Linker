#ifndef LINKER_H
#define LINKER_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

#include "SymbolTable.h"
#include "OutputData.h"
#include "InputFileData.h"

using namespace std;

class Linker
{
public:
	void linkerJob(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut, OutputData& outputDataOut, bool& condition);

	void sectionProcessing(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut, int& ordinal);
	void processSameSectionInOtherFiles(vector<InputFileData>& inputDataVector, string sectionName,
		unsigned int fileIndex, int& counter, int& size);

	void symbolProcessing(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut, bool& condition, int& ordinal);

	void outputDataProcessing(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut,
		OutputData& outputDataOut, bool& condition);
	void processOutputDataInOtherFiles(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut, OutputSection& outputSection,
		string sectionName, unsigned int fileIndex);

	void updateByteDataWithRelocData(SymbolTable& symbolTableOut, OutputData& outputDataOut, bool& condition);
	void updateDataWithAbsRelocData(SymbolTable& symbolTableOut, OutputReloc& outputReloc, vector<string>& byteVector);
	void updateInstrWithAbsRelocData(SymbolTable& symbolTableOut, OutputReloc& outputReloc, vector<string>& byteVector);
	void updateInstrWithRelRelocData(SymbolTable& symbolTableOut, OutputReloc& outputReloc, vector<string>& byteVector, string sectionName);
};

#endif

