#include "stdafx.h"
#include <string>
#include"order.h"
using namespace std;
order::order(string Trader, string buyorsell, string inputstock, int inputquantity){
	Trader_name = Trader;
	side = buyorsell;
	stock = inputstock;
	quantity = inputquantity;
	
}
//other functions are too sample, so I defined them in the header file