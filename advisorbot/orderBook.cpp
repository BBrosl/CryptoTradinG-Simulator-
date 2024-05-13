#include "orderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>
using namespace std;

OrderBook::OrderBook(string filename)
{
    //READ CSV and store
    orders = CSVReader::readCSV(filename);
}

/**return vector of all known products in dataset*/
vector<string> OrderBook::getKnownProducts() 
{
    vector<string> products;
    map<string, bool> prodMap;

    for (OrderBookEntry& e : orders) {
        prodMap[e.product] = true;
    }

    // flatten the map to a vector of strings
    for (auto const& e : prodMap) {
        products.push_back(e.first);
    }

    return products;
}

/**return vvector oforders according to sent filters*/
vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                            string product,
                                            string timestamp)
{
    vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e : orders)
    {
        if (e.orderType == type &&
            e.product == product &&
            e.timestamp == timestamp)
        {
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}

//get the earliest time stamp
string OrderBook::getEarliestTime() 
{
    return orders[0].timestamp;
}

string OrderBook::getNextTime(string timestamp)
{
    string next_timestamp = "";

    for (OrderBookEntry& e : orders)
    {
        if (e.timestamp > timestamp)
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}

//function to obtain highest price
double OrderBook::getHighPrice(vector<OrderBookEntry>& orders) 
{
    double max = orders[0].price;

    for (OrderBookEntry& e : orders) 
    {
        if (e.price > max) {
            max = e.price;
        }
    }
    return max;
}

//function to get lowest price
double OrderBook::getLowPrice(vector<OrderBookEntry>& orders) 
{
    double min = orders[0].price;

    for (OrderBookEntry& e : orders) 
    {
        if (e.price < min) {
            min = e.price;
        }
    }

    return min;
}

//function to obtain the average price
double OrderBook::getAvgPrice(vector<OrderBookEntry>& orders)
{
    double sum = 0.0, avg;

    for (OrderBookEntry& e : orders)
    {
        sum += e.price;
    }

    avg = sum / orders.size();
    return avg;
}

void OrderBook::insertOrder()
{
    vector<OrderBookEntry> entry = CSVReader::readNewEntry("20200601.csv", orders.size());
    orders.insert(end(orders), begin(entry), end(entry));
    //cout << "Total number of orders: " << orders.size() << endl;
}
