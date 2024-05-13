#pragma once
#include <string>
using namespace std;

enum class OrderBookType
{
	bid,
	ask,
	unknown,
	asksale,
	bidsale
};

class OrderBookEntry
{
public:
    OrderBookEntry(double _price,
        double _amount,
        string _timestamp,
        string _product,
        OrderBookType _orderType,
        string username = "dataset");

    static OrderBookType stringToOrderBookType(string s);

    static bool sortByTimestamp(OrderBookEntry& e1, OrderBookEntry& e2)
    {
        return e1.timestamp < e2.timestamp;
    }
    static bool sortPriceAsc(OrderBookEntry& e1, OrderBookEntry& e2)
    {
        return e1.price < e2.price;
    }
    static bool sortPriceDesc(OrderBookEntry& e1, OrderBookEntry& e2)
    {
        return e1.price > e2.price;
    }

    double price;
    double amount;
    string timestamp;
    string product;
    OrderBookType orderType;
    string username;
};