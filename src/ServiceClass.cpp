#include "ServiceClass.h"

regex ServiceClass::PLACE("^-place=[a-zA-Z_][a-zA-Z0-9_]*@0[xX][0-9a-fA-F]+$");
regex ServiceClass::FILE_NAME("^tests/[a-zA-Z][a-zA-Z0-9_]*\\.txt$");

void ServiceClass::inputArgumentsProcessing(int argc, char** argv, vector<InputFileData>& inputDataVector,
    string& outputCode, string& mode, bool& condition, vector<SectionAddress>& sectionAddressVector)
{
    vector<string> inputArguments;
    bool modeLinkeable = false;
    bool modeHex = false;

    for (int i = 1; i < argc; i++)
    {
        string arg = string(argv[i]);
        inputArguments.push_back(arg);
    }

    for (unsigned int i = 0; i < inputArguments.size() && condition; i++)
    {
        if (inputArguments[i] == "-o")
        {
            if ((i + 1) < inputArguments.size())
            {
                outputCode = inputArguments[++i];
            }
            else
            {
                condition = false;
            }
        }
        else if (inputArguments[i] == "-hex")
        {
            modeHex = true;
        }
        else if (inputArguments[i] == "-linkeable")
        {
            modeLinkeable = true;
        }
        //Ovo na kraju
        else if (regex_match(inputArguments[i], PLACE))
        {
            string arg = inputArguments[i];
            string delimeter = "=";
            arg.erase(0, arg.find(delimeter)+1);
            arg = trimString(arg);
            delimeter = "@";
            string section = arg.substr(0, arg.find(delimeter));
            section = trimString(section);
            arg.erase(0, arg.find(delimeter) + 1);
            string addressString = trimString(arg);
            int address = stoi(addressString, nullptr, 16);

            SectionAddress value(section, address);
            sectionAddressVector.push_back(value);
        }
        else if (regex_match(inputArguments[i], FILE_NAME))
        {
            SymbolTable symbolTable;
            OutputData outputData;
            
            string file = inputArguments[i];

            ifstream inFile(file, ios::out | ios::binary);
            symbolTable.readBinary(inFile);
            outputData.readBinary(inFile);
            InputFileData inputData(file, symbolTable, outputData);

            inputDataVector.push_back(inputData);
        }
        else
        {
            cout << "Error! False argument!" << endl;
            condition = false;
        }
    }

    if (modeHex && modeLinkeable && condition)
    {
        mode = "";
        condition = false;
    }
    else if (modeHex && condition)
    {
        mode = "hex";
        condition = true;
    }
    else if (modeLinkeable && condition)
    {
        mode = "linkeable";
        condition = true;
    }

}

string ServiceClass::trimString(string arg)
{
    if (arg.find_first_not_of(" \t\n\v\f\r") != std::string::npos)
    {
        arg.erase(0, arg.find_first_not_of(" \t"));
        arg.erase(arg.find_last_not_of(" \t") + 1);
    }
    return arg;
}
