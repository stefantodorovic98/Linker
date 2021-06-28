#include "InputFileData.h"

string InputFileData::getFileName()
{
    return fileName;
}

SymbolTable& InputFileData::getSymbolTable()
{
    return symbolTable;
}

OutputData& InputFileData::getOutputData()
{
    return outputData;
}
