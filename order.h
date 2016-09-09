#ifndef ORDER_H
#define ORDER_H
#include<string>
using namespace std;
class order{
public:
	order(){};
	order(string Trader, string buyorsell, string stock, int quantity);
	void change_quantity(int x){ quantity = quantity - x; }
	string get_name(){ return Trader_name; }
	string get_side(){ return side; }
	int get_quantity(){ return quantity; }
	string get_stock(){ return stock; }
	void set_Trader_name(string a){ Trader_name = a; }
	void set_side(string a){ side = a; }
	void set_stock(string a){ stock = a; }
	void set_quantity(int a){ quantity = a; }


private:
	string Trader_name;
	string side;
	string stock;
	int quantity;
	
};





#endif