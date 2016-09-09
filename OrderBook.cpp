// OrderBook.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<fstream>
#include"order.h"
#include"message.h"
#include"structure.h"
using namespace std;



int _tmain(int argc, _TCHAR* argv[])
{
	structure bigcarl;
	message hhhh;
	ifstream infile;
	infile.open("orz_New.txt");
	vector<string> input_data;

	int mark = 1;
	string temp;
	int sep = 0;
	while (mark)
	{
		infile >> temp;
		if (infile)
		{
			input_data.push_back(temp);
			if ((input_data.size() > 0) && (input_data.size() % 4 == 0))
			{
				string trader_name = input_data[0];
		        string	side = input_data[1];
				string stock = input_data[2];
				int volume = atoi((input_data[3]).c_str());
				order carl ( trader_name, side, stock, volume);
				//system("cls");
				bigcarl.input_order(hhhh, carl);
				
				input_data.clear();
			}
		}
		else
		{
			mark = 0;
			if (infile.eof())
			{
				cout << endl;
				cout << '\t' << '\t' << '\t' << "---***End of The File***---" << endl<<endl;

			}
			else
			{
				cout << "Wrong file!" << endl;
				exit(0);// method to directly quit th eprogram
			}
		}
	}
	infile.close();
	bigcarl.display_sell();
	bigcarl.display_buy();

	return 0;
}

