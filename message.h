#ifndef MESSAGE_H
#define MESSAGE_H
#include<string>
#include<iostream>
#include"order.h"
using namespace std;
class message{
public:
	message(){};
	void received(order hhh);
	void returned(order hhh);
	void transaction(order larger, order smaller);
	void fullinlist(order hhh);
	void partinlist(order hhh);
	void partreturn(order hhh);
};
#endif