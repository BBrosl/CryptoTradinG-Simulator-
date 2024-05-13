#pragma once
#include "orderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>
using namespace std;

class OrderBook
{
    public:
        /**construct, reading a csv data file*/
        OrderBook(string filename);

        /**return vvector of all known products in dataset*/
        vector<string> getKnownProducts();

        /**return vvector oforders according to sent filters*/
        vector<OrderBookEntry> getOrders(OrderBookType type,
                                         string product,
                                         string timestamp);

        /**return earliest time in orderbook*/
        string getEarliestTime();

        /**return the next time after sent time in orderbook
        * if there is no timestamp. wraps around to start
        */
        std::string getNextTime(std::string timestamp);

        void insertOrder();//////

        static double getLowPrice(vector<OrderBookEntry>& orders);
        static double getHighPrice(vector<OrderBookEntry>& orders);
        static double getAvgPrice(vector<OrderBookEntry>& orders);

    private:
        //storing of csv in in oderbookcpp
        vector<OrderBookEntry> orders;
};

