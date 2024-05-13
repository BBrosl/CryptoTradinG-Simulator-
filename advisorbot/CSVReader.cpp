#include "CSVReader.h"
#include <iostream>
#include <fstream>
using namespace std;

CSVReader::CSVReader()
{
}

//read data from CSV and store in vector
vector<OrderBookEntry> CSVReader::readCSV(string csvFilename) {
    //empty vector
    vector<OrderBookEntry> entries;
    //opening CSVFile by name
    ifstream csvFile{ csvFilename };
    //declaring a string to store each line
    string line;
    int unreadCSVEntries = 0;

    if (csvFile.is_open()) {
        //keep reading until end of file or 1000 lines reached
        while (getline(csvFile, line) && unreadCSVEntries < 1000) {
            try {
                //passing resulting vector to create OrderBookEntry object
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));

                entries.push_back(obe);

            }
            catch (const exception& e) {

                cout << "CSVReader::readCSV bad data" << endl;
            }
            unreadCSVEntries++;
        }
    }
    //cout << "Entries read in readCSV: " << entries.size() << endl;
    return entries;
}

vector<OrderBookEntry> CSVReader::readNewEntry(string csvFilename, double size) 
{
    vector<OrderBookEntry> entries;
    ifstream csvFile{ csvFilename };
    string line;

    // read CSV entries is equal to the size of orders vector
    int entriesRead = size;
    int entriesUnread = 0;

    if (csvFile.is_open()) {
        while (getline(csvFile, line)) {
            try {
                //only tokenise entries that are between entries read and entries read + 1000
                if (entriesUnread > entriesRead && entriesUnread <= entriesRead + 1000) {

                    OrderBookEntry obe = stringsToOBE(tokenise(line, ','));

                    entries.push_back(obe);

                }
                else if (entriesUnread > entriesRead + 1000) {

                    // exiting the loop after tokenising 1000 entries or end of file
                    break;
                }
                entriesUnread++;
            }

            catch (const exception& e) {
                cout << "CSVReader::readCSV bad data" << endl;
            }
        }
    }

    return entries;
}

vector<string> CSVReader::tokenise(string csvLine, char separator)
{
    vector<string> tokens;
    signed int start, end;
    string token;

    start = csvLine.find_first_not_of(separator, 0);
    do
    {
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end)
            break;
        if (end >= 0)
            token = csvLine.substr(start, end - start);
        else
            token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end > 0);

    return tokens;
}

OrderBookEntry CSVReader::stringsToOBE(vector<string> tokens) {
    double price, amount;

    //bad entry
    if (tokens.size() != 5) {
        cout << "Bad line" << endl;
        throw exception{};
    }

    //we have 5 tokens
    try {
        price = stod(tokens[3]);
        amount = stod(tokens[4]);
    }
    catch (const exception& e) {
        cout << "CSVReader::stringsToOBE Bad float! " << tokens[3] << endl;
        cout << "CSVReader::stringsToOBE Bad float! " << tokens[4] << endl;
        throw;
    }

    OrderBookEntry obe{ price, 
                        amount, 
                        tokens[0], 
                        tokens[1], 
                        OrderBookEntry::stringToOrderBookType(tokens[2]) };

    return obe;
}

OrderBookEntry CSVReader::stringsToOBE(string priceString,
                                       string amountString,
                                       string timestamp,
                                       string product,
                                       OrderBookType orderType)
{
    double price, amount;
    try
    {
        price = stod(priceString);
        amount = stod(amountString);
    }
    catch (const exception& e)
    {
        std::cout << "CSVReader::stringsToOBE Bad float! " << priceString << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << amountString << std::endl;
        throw;
    }

    OrderBookEntry obe{ price,
                       amount,
                       timestamp,
                       product,
                       orderType };

    return obe;
}