#ifndef SECTION_ADDRESS_H
#define SECTION_ADDRESS_H

#include <iostream>

using namespace std;

class SectionAddress
{
private:
	string sectionName;
	int addressValue;
public:
	SectionAddress(string sectionName, int addressValue) :
		sectionName(sectionName), addressValue(addressValue)
	{	}

	string getSectionName();
	
	int getAddressValue();
};

#endif

