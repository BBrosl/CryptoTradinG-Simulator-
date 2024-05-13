#include "advisorbot.h"
#include "orderBook.h"
#include "CSVReader.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

AdvisorBot::AdvisorBot()
{

}

//program initialisation
void AdvisorBot::init()
{
	string userInput;
	vector<string> previousTimeFrames;

	currentTime = orderBook.getEarliestTime();
	timeframe = 1;

	for (string const& p : orderBook.getKnownProducts())
	{
		productsAvailable.push_back(p);
	}

	while (true)
	{
		mainMenu();
		userInput = getUserInput();
		if (processUserInput(userInput) == 0) {
			break;
		}
	}
}

void AdvisorBot::mainMenu()
{
	cout << "________________________________________" << endl;
	cout << "The time now is: " << currentTime << endl;
	cout << "Time frame: " << timeframe << "\n" << endl;

	//main commands
	cout << "List of commands available" << endl;
	cout << "1: help" << endl;
	cout << "2: help <cmd>" << endl;
	cout << "3: prod" << endl;
	cout << "4: min <prod> <bid/ask>" << endl;
	cout << "5: max <prod> <bid/ask>" << endl;
	cout << "6: avg <prod> <bid/ask> <timestamps>" << endl;
	cout << "7: predict <min/max> <prod> <bid/ask>" << endl;
	cout << "8: time" << endl;
	cout << "9: step" << endl;
	cout << "10: skip <timestamps>" << endl;
	cout << "11: diff <prod> <bid/ask>" << endl;
	cout << "12: exit" << endl;
	cout << "________________________________________" << endl;
}


string AdvisorBot::getUserInput()
{
	cout << "Type out the command: " << endl;

	string userInput;
	getline(cin, userInput);
	cout << "\n" << endl;

	return userInput;
}

//process userInput, checking user inputs and determining number of tokens based on inputs
int AdvisorBot::processUserInput(string userInput)
{
	vector<string> token = CSVReader::tokenise(userInput, ' ');

	//matching tokens to commands
	if (token.size() == 1)
	{
		if (token[0] == "help") {
			help();
			return 1;
		}
		else if (token[0] == "prod") {
			productList();
			return 1;
		}
		else if (token[0] == "time") {
			timeNow();
			return 1;
		}
		else if (token[0] == "step") {
			nextTimeStep();
			return 1;
		}
		else if (token[0] == "exit") {
			return 0;
		}
		else {
			cout << userInput << " is an invalid input. \n" << endl;
		}
	}
	//commands with 2 tokens
	else if (token.size() == 2) {
		if (token[0] == "help" && token[1] != "") {
			helpCmd(token[1]);
		}
		else if (token[0] == "skip") {
			skipTimeStep(token[1]);
		}
		else {
			cout << userInput << " is an invalid input. \n" << endl;
		}
	}
	//comands with 3 tokens
	else if (token.size() == 3) {
		if (contains(productsAvailable, token[1])) {
			if ((token[0] == "min") && (token[2] == "bid" || token[2] == "ask")) {
				getMin(token[1], token[2]);
			}
			else if ((token[0] == "max") && (token[2] == "bid" || token[2] == "ask")) {
				getMax(token[1], token[2]);
			}
			else if ((token[0] == "diff") && (token[2] == "bid" || token[2] == "ask")) {
				getPriceDiff(token[1], token[2]);
			}
			else {
				cout << userInput << " is an invalid input. \n" << endl;
			}
		}
		else {
			cout << token[1] << " is an invalid product. \n" << endl;
		}
	}
	//commands with 4 tokens
	else if (token.size() == 4) {
		if (token[0] == "avg") {
			if (contains(productsAvailable, token[1])) {
				if (token[2] == "bid" || token[2] == "ask") {
					getAvg(token[1], token[2], token[3]);
				}
				else {
					cout << "Invalid value for bid or ask: " << token[2] << endl;
				}
			}
			else {
				cout << "Invalid product: " << token[1] << endl;
			}
		}
		else if (token[0] == "predict") {
			if (token[1] == "min" || token[1] == "max") {
				if (contains(productsAvailable, token[2])) {
					if (token[3] == "bid" || token[3] == "ask") {
						predictMarket(token[1], token[2], token[3]);
					}
					else {
						cout << "Invalid value for bid or ask: " << token[3] << endl;
					}
				}
				else {
					cout << "Invalid product: " << token[2] << endl;
				}
			}
			else {
				cout << "Invalid value for min or max: " << token[1] << endl;
			}
		}
		else {
			cout << "Invalid function name: " << token[0] << endl;
		}
	}
	else {
		cout << "Invalid input: " << userInput << endl;
	}
	
}

//help
void AdvisorBot::help()
{
	cout << "The available commands are 'help', 'help <cmd>', 'prod', 'min', 'max', 'avg', 'predict', 'time', 'step', 'skip', 'diff'. \n" << endl;
}

//help cmd: lists all help commands and functions
void AdvisorBot::helpCmd(string userInput)
{
	if (userInput == "prod")
	{	
		//'prod' information
		cout << "prod: List all products.\n" << endl;
	}
	else if (userInput == "min") {
		//'min' information
		cout << "min <prod> <bid/ask>: Find minimum bid or ask for specified product in current time stamp.\n" << endl;
	}
	else if (userInput == "max") {
		
		cout << "max <prod> <bid/ask>: Find maximum bid or ask for specified product in current time stamp.\n" << endl;
	}
	else if (userInput == "avg") {
		//'avg' information
		cout << "max <prod> <bid/ask> <timestamps>: Find average bid or ask for specified product in given time stamp.\n" << endl;
	}
	else if (userInput == "predict") {
		//'predict' information
		cout << "predict <min/max> <prod> <bid/ask>: Predict max or min ask or bid for the sent product for the next time step.\n" << endl;	
	}
	else if (userInput == "time") {
		//'time' information
		cout << "time: Show current time frame.\n" << endl;
	}
	else if (userInput == "step") {
		//'step' information
		cout << "step: Move to the next timeframe.\n" << endl;
	}
	else if (userInput == "skip") {
		//'skip' information
		cout << "skip <timeSteps>: Move ahead by desired number of time steps. \n" << endl;
	}
	else if (userInput == "diff") {
		//'diff' information
		cout << "diff <prod> <bid/ask>: Shows the difference between the highest and lowest price for a given ask or bid product. \n" << endl;
	}
	else if (userInput == "exit") {
		//'exit' information
		cout << "exit: Exits the program. \n" << endl;
	}
	else {
		cout << userInput << " is an invalid help command.\n" << endl;
	}
}

//prod command: list all available products
void AdvisorBot::productList()
{
	for (string const& p : orderBook.getKnownProducts()) {
		cout << "List of available products: " << p << "\n" << endl;
	}
}

//min command
void AdvisorBot::getMin(string prod, string bidask)
{
	OrderBookType type;
	if (bidask == "bid") {
		type = OrderBookType::bid;
		cout << "The lowest bid for the product " << prod << " is $ ";
	}
	else if (bidask == "ask") {
		type = OrderBookType::ask;
		cout << "The lowest ask for the product " << prod << " is $ ";
	}
	else {
		throw invalid_argument("Invalid bidask value: " + bidask);
	}

	try {
		vector<OrderBookEntry> entries = orderBook.getOrders(type, prod, currentTime);
		double lowestPrice = orderBook.getLowPrice(entries);
		cout << lowestPrice << ".\n" << endl;
	}
	catch (...) {
		cout << "getMin error: lowest price not found" << endl;
	}
}

//max command
void AdvisorBot::getMax(string prod, string bidask)
{
	OrderBookType type;
	if (bidask == "bid") {
		type = OrderBookType::bid;
		cout << "The highest bid for the product " << prod << " is $ ";
	}
	else if (bidask == "ask") {
		type = OrderBookType::ask;
		cout << "The highest ask for the product " << prod << " is $ ";
	}
	else {
		throw invalid_argument("Invalid bidask value: " + bidask);
	}

	try {
		vector<OrderBookEntry> entries = orderBook.getOrders(type, prod, currentTime);
		double highestPrice = orderBook.getHighPrice(entries);
		cout << highestPrice << ".\n" << endl;
	}
	catch (...) {
		cout << "getMax error: highest price not found" << endl;
	}
}

//avg command
void AdvisorBot::getAvg(string prod, string bidask, string timestepToken) {
	string currentTimeframe = currentTime;
	int n = previousTimeFrames.size();
	int timesteps = stoi(timestepToken);
	double sum = 0.0;
	double average = 0.0;

	OrderBookType type;
	if (bidask == "bid") {
		type = OrderBookType::bid;
	}
	else if (bidask == "ask") {
		type = OrderBookType::ask;
	}
	else {
		return;
	}

	try {
		if (timesteps > timeframe) {
			cout << "Timestep Chosen: " << timesteps << ". Current time frame: " << timeframe << endl;
			cout << "Please enter a time step smaller than or equals to the current time frame.\n" << endl;
			return;
		}
		else {
			for (int i = 1; i <= timesteps; i++) {
				vector<OrderBookEntry> entries = orderBook.getOrders(type, prod, currentTimeframe);
				sum += orderBook.getAvgPrice(entries);

				cout << "Time frames used in calculations: " << currentTimeframe << "\n" << endl;

				// set current time frame to previous if user selected more than 1 time step
				if (n > 1) {
					currentTimeframe = previousTimeFrames[n - 1];
				}
			}

			// finding average
			average = sum / timesteps;
			cout << "The average of " << prod << " " << bidask << " over the last " << timesteps << " time steps is $" << average << ".\n" << endl;
		}
	}
	catch (...) {
		cout << "getAvg error, average could not be found";
	}
}

//predict command: predict downtrend or uptrend based on product, bid/ask, min/max
void AdvisorBot::predictMarket(string minmax, string prod, string bidask)
{
		vector<double> price;
		vector<double> movingAvg;
		string currentTimeframe = currentTime;
		int n = previousTimeFrames.size();
		int length = 5;
		double sum = 0;
		double mavg = 0;
		double diff;

		if (timeframe < 11) {
			cout << "Insufficient time steps! Please jump to the 11th time step or further. \n";
			return;
		}
	try{
		
		//determining orderBookType
		OrderBookType obt;
		if (bidask == "bid") {
			obt = OrderBookType::bid;
		}
		else {
			obt = OrderBookType::ask;
		}

		for (int i = 1; i < 11; i++) {

			//getting orders from order book
			vector<OrderBookEntry> entries = orderBook.getOrders(obt, prod, currentTimeframe);
			double priceVal;

			//determining min or max
			if (minmax == "min") {
				priceVal = orderBook.getLowPrice(entries);
			}
			else {
				priceVal = orderBook.getHighPrice(entries);
			}
			price.push_back(priceVal);
			currentTimeframe = previousTimeFrames[n - i];
		}

		//looping through price vector
		for (int i = 0; i <= (10 - length); i++)
		{
			sum = 0;
			for (int j = i; j < i + length; j++)
			{
				sum += price[j];
			}
			//moving average calculation
			mavg = sum / length;
			movingAvg.push_back(mavg);
			cout << "Moving average " << ": " << mavg << "\n" << endl;
		}
		diff = movingAvg.front() - movingAvg.back();
		cout << "Price difference within the previous time steps are " << diff << "\n" << endl;

		//output results
		if (diff > 0)
		{
			cout << prod << " " << minmax << " " << bidask << " is in an uptrend and is likely to increase.\n" << endl;
		}
		else if (diff < 0) {
			cout << prod << " " << minmax << " " << bidask << " is in a downtrend and is likely to decrease.\n" << endl;
		}
		else {
			cout << prod << " " << minmax << " " << bidask << " is neutral and likely to remain constant.\n" << endl;
		}
	}
	catch (...) {
		cout << "Prediction error, could not get prediction" << endl;
	}
}

//current time command: show time now
void AdvisorBot::timeNow() 
{
	cout << "The time now is: " << currentTime << ".\n" << endl;
}

//step command: go to next step
void AdvisorBot::nextTimeStep()
{
	timeframe++;
	//pushing currentTime into the array so other functions have access to timeframes
	previousTimeFrames.push_back(currentTime);
	currentTime = orderBook.getNextTime(currentTime);
	orderBook.insertOrder();
}

//skip command: go through multiple time step
void AdvisorBot::skipTimeStep(string skipToken) 
{
	try {
		int i, amountSkipped = stoi(skipToken);

		for (i = 0; i < amountSkipped; i++) {
			timeframe++;
			//pushing currentTime into the array so other functions have access to timeframes
			previousTimeFrames.push_back(currentTime);
			currentTime = orderBook.getNextTime(currentTime);
			orderBook.insertOrder();
		}
	}
	catch (...) {
		cout << skipToken << "is an invalid input. Only input integers. \n" << endl;
	}
}

//exit command: exit program safely
void AdvisorBot::exitMenu()
{
	return;
}

bool AdvisorBot::contains(vector<string> productsAvailable, string productToken)
{
	bool found = false;

	if (find(productsAvailable.begin(), productsAvailable.end(), productToken) != productsAvailable.end())
	{
		found = true;
	}

	return found;
}

//diff command: returns the difference between min and max price for ask or bid
void AdvisorBot::getPriceDiff(string prod, string bidask)
{
	OrderBookType type;

	if (bidask == "bid") {
		type = OrderBookType::bid;
		cout << "The difference between the highest and lowest bid prices for the product " << prod << " is $ ";
	}

	else if (bidask == "ask") {
		type = OrderBookType::ask;
		cout << "The difference between the highest and lowest ask prices for the product " << prod << " is $ ";
	}

	else {
		throw invalid_argument("Invalid bidask value: " + bidask);
	}

	try {
		vector<OrderBookEntry> entries = orderBook.getOrders(type, prod, currentTime);
		//obtaining highPrice and lowPrice from functions in orderBook
		double highestPrice = orderBook.getHighPrice(entries);
		double lowestPrice = orderBook.getLowPrice(entries);
		double priceDiff = highestPrice - lowestPrice;
		cout << priceDiff << ".\n" << endl;
	}
	catch (...) {
		cout << "getPriceDiff error: price difference not found" << endl;
	}
}