#ifndef MODIFIED_LINKER_H
#define MODIFIED_LINKER_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

#include "SymbolTable.h"
#include "OutputData.h"
#include "InputFileData.h"
#include "SectionAddress.h"

using namespace std;

class ModifiedLinker
{
public:
	void modifiedlinkerJob(string mode, vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut, 
		vector<SectionAddress>& sectionAddressVector, OutputData& outputDataOut, bool& condition, ofstream& outputFile);

	void linkeableSectionProcessing(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut, int& ordinal);
	void linkeableProcessSameSectionInOtherFiles(vector<InputFileData>& inputDataVector, string sectionName,
		unsigned int fileIndex, int& counter, int& size);

	void linkeableSymbolProcessing(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut, bool& condition, int& ordinal);

	void linkeableOutputDataProcessing(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut,
		OutputData& outputDataOut, bool& condition);
	void linkeableProcessOutputDataInOtherFiles(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut,
		OutputSection& outputSection, string sectionName, unsigned int fileIndex);

	void linkeableUpdateByteDataWithRelocData(SymbolTable& symbolTableOut, OutputData& outputDataOut, bool& condition);
	void updateDataWithAbsRelocData(SymbolTable& symbolTableOut, OutputReloc& outputReloc, vector<string>& byteVector);
	void updateInstrWithAbsRelocData(SymbolTable& symbolTableOut, OutputReloc& outputReloc, vector<string>& byteVector);
	void updateInstrWithRelRelocData(SymbolTable& symbolTableOut, OutputReloc& outputReloc, vector<string>& byteVector,
		string sectionName);
	void updateInstrWithRelRelocDataABS(SymbolTable& symbolTableOut, OutputReloc& outputReloc, vector<string>& byteVector,
		string sectionName);

	void hexSectionProcessing(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut,
		vector<SectionAddress>& sectionAddressVector, int& ordinal);
	void hexProcessSameSectionInOtherFiles(vector<InputFileData>& inputDataVector, string sectionName,
		unsigned int fileIndex, int& counter, int& size);
	void hexCheckSectionsOverlap(SymbolTable& symbolTableOut, bool& condition);

	void hexSymbolProcessing(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut, bool& condition, int& ordinal);

	void hexOutputDataProcessing(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut,
		OutputData& outputDataOut, bool& condition);
	void hexProcessOutputDataInOtherFiles(vector<InputFileData>& inputDataVector, SymbolTable& symbolTableOut,
		OutputSection& outputSection, string sectionName, unsigned int fileIndex);

	void hexUpdateByteDataWithRelocData(SymbolTable& symbolTableOut, OutputData& outputDataOut, bool& condition);
};

#endif

