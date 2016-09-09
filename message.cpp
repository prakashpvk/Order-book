#include "stdafx.h"
#include"message.h"
using namespace std;

void message::received(order hhh){
	string side;
	string price;
	if (hhh.get_side() == "S")
		side = "sell";
	else
		side = "buy";
	if (hhh.get_stock() == "S")
		cout << "[Stock S Message-Received]: " << hhh.get_name() << " , your order to " << side << " with quantity" << hhh.get_quantity() << " has been received" << endl;
	if (hhh.get_stock() == "G")
		cout << "[Stock G Message-Received]: " << hhh.get_name() << " , your order to " << side << " with quantity" << hhh.get_quantity() << " has been received" << endl;
	if (hhh.get_stock() == "H")
		cout << "[Stock H Message-Received]: " << hhh.get_name() << " , your order to " << side << " with quantity" << hhh.get_quantity() << " has been received" << endl;
	
}
void message::returned(order hhh){
	string side;
	if (hhh.get_side() == "S")
		side = "sell";
	else
		side = "buy";
	cout << "[Message-No counterparty]: " << hhh.get_name() << " , we can not find counterparty for your order, the order is being returned" << endl;
}
void message::transaction(order larger, order smaller){
	if ((smaller.get_quantity())*(larger.get_quantity()) != 0)
	{
		cout << "[Message-Transaction]: " << smaller.get_name() << ", Your order is fully completed with the counterparty " << larger.get_name() << endl;
		cout << "[Message-Transaction]: " << larger.get_name() << ", Your order is partially completed with the counterparty " << smaller.get_name() << " with volume of " << smaller.get_quantity() << endl;
	}
}
void message::fullinlist(order hhh){

	cout << "Message-No counterparty]: " << hhh.get_name() << " , No available counterparty founded, your order has been added in the order list " << endl;
}
void message::partinlist(order hhh){

	cout << "[Message-Status]: " << hhh.get_name() << " , your order has been partially fuifiled, the rest of your order ( " << hhh.get_quantity() << " ) is now in the list" << endl;
}
void message::partreturn(order hhh){

	cout << "[Message-Status]: " << hhh.get_name() << " , your order has been partially fuifiled, the rest of your order (" << hhh.get_quantity() << " )has been returned " << endl;
}