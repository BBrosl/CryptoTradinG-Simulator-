#pragma once
#include "orderBookEntry.h"
#include "orderBook.h"
#include <string>
using namespace std;

class AdvisorBot
{
	public:
		AdvisorBot();
		void init();

	private:
		//main menu commands
		int processUserInput(string userInput);
		void mainMenu();
		void help();
		void helpCmd(string userInput);
		void productList();
		void getMin(string prod, string bidask);
		void getMax(string prod, string bidask);
		void getAvg(string prod, string bidask, string timeStepToken);
		void predictMarket(string minmax, string prod, string bidask);
		void timeNow();
		void nextTimeStep();
		void skipTimeStep(string skipToken);
		void exitMenu();
		bool contains(vector<string> availableProducts, string productToken);
		void getPriceDiff(string prod, string bidask);

		string getUserInput();
		string currentTime;
		int timeframe;
		vector<string> previousTimeFrames;
		vector<string> productsAvailable;

		OrderBook orderBook{"20200601.csv"};
};
