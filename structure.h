#ifndef STRUCTURE_H
#define STRUCTURE_H
#include<string>
#include"message.h"
#include<map>
#include<vector>
class structure{
public:
	void input_order(message hhhh, order neworder);
	void display_sell();
	void display_buy();
private:
	map<double, vector<order>> sell_list;
	map<double, vector<order>> buy_list;
};
#endif