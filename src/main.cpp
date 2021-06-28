#include <iostream>
#include <fstream>
#include <vector>

#include "SymbolTable.h"
#include "OutputData.h"
#include "Linker.h"
#include "ServiceClass.h"
#include "ModifiedLinker.h"
#include "SectionAddress.h"

using namespace std;

int main(int argc, char** argv)
{
    vector<InputFileData> inputDataVector;
    vector<SectionAddress> sectionAddressVector;
    bool condition = true;
    string outputCode = "";
    string mode = "";

    ServiceClass::inputArgumentsProcessing(argc, argv, inputDataVector, outputCode,
        mode, condition, sectionAddressVector);

    if (!condition) cout << "Error in arguments!" << endl;
    //Linker l;
    SymbolTable symbolTableOut;
    OutputData outputDataOut;
    //l.linkerJob(inputDataVector, symbolTableOut, outputDataOut, condition);

    ModifiedLinker ml;
    if (outputCode == "")
    {
        outputCode = "izlazLinkera.txt";
    }

    ofstream outputFile(outputCode);
    if (condition)
    {
        ml.modifiedlinkerJob(mode, inputDataVector, symbolTableOut, sectionAddressVector, outputDataOut, condition, outputFile);
    }
    
    return 0;
}

