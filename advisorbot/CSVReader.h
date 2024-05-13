#pragma once
#include "orderBookEntry.h"
#include <vector>
#include <string>
using namespace std;

class CSVReader
{
    public:
	    CSVReader();
	
        static vector<OrderBookEntry> readCSV(string csvFile);
        static vector<string> tokenise(string csvLine, char separator);
        static OrderBookEntry stringsToOBE(string price,
                                           string amount,
                                           string timestamp,
                                           string product,
                                           OrderBookType OrderBookType);
        static vector<OrderBookEntry> readNewEntry(string csvFile, double size);

    private:
        static OrderBookEntry stringsToOBE(vector<string> strings);
};

