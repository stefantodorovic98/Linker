#ifndef SERVICE_CLASS_H
#define SERVICE_CLASS_H

#include <iostream>
#include <vector>
#include <regex>

#include "SymbolTable.h"
#include "OutputData.h"
#include "InputFileData.h"
#include "SectionAddress.h"

using namespace std;

class ServiceClass
{
private:

public:
	static regex PLACE;
	static regex FILE_NAME;
	static void inputArgumentsProcessing(int argc, char** argv, vector<InputFileData>& inputDataVector,
		string& outputFile, string& mode, bool& condition, vector<SectionAddress>& sectionAddressVector);

	static string trimString(string arg);
};

#endif

