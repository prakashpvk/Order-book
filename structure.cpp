
#include"stdafx.h"
#include"structure.h"

void structure::input_order(message hhhh, order neworder){
	int starmark = 0;
	if (neworder.get_side() == "B")
	{
		hhhh.received(neworder);
		//buy side

		if ((sell_list.empty()))
		{
			if (neworder.get_stock() == "S" || neworder.get_stock() == "G" || neworder.get_stock() == "H")
			{
				buy_list[neworder.get_quantity()].push_back(neworder);
				starmark = 1; 
			}

			else
				hhhh.returned(neworder);
		}
		else{
			//~~~~~~~~~~~~~
			if (neworder.get_stock() == "S")
			{
				while (1)
				{
					map<double, vector<order>>::iterator sell_iter = sell_list.begin();
					if (sell_iter == sell_list.end())
						break;
					else
					{
						//****************************
						for (int i = 0; i <= (sell_iter->second).size() - 1; i++)
						{
							if (((sell_iter->second)[i]).get_quantity() >= neworder.get_quantity())
							{

								hhhh.transaction(((sell_iter->second)[i]), neworder);
								
								((sell_iter->second)[i]).change_quantity(neworder.get_quantity());
								neworder.change_quantity(neworder.get_quantity());


							}
							else  
							{
								hhhh.transaction(neworder, ((sell_iter->second)[i]));
								neworder.change_quantity(((sell_iter->second)[i]).get_quantity());
								((sell_iter->second)[i]).change_quantity(((sell_iter->second)[i]).get_quantity());
							}
						}
						
						//*******************************
						while (1)
						{
							vector<order>::iterator temp = (sell_iter->second).begin();
							if (temp == (sell_iter->second).end())
								break;
							else if ((*temp).get_quantity() == 0)
								(sell_iter->second).erase(temp);
							else
								break;
						}

						if ((sell_iter->second).size() == 0)
							sell_list.erase(sell_iter);
						else
							break;
						//*******************************
					}
				}
			}
			if (neworder.get_stock() == "G")
			{
				while (1)
				{
					map<double, vector<order>>::iterator sell_iter = sell_list.begin();
					if (sell_iter == sell_list.end())
						break;
					else
					{
						//****************************
						for (int i = 0; i <= (sell_iter->second).size() - 1; i++)
						{
							if (((sell_iter->second)[i]).get_quantity() >= neworder.get_quantity())
							{

								hhhh.transaction(((sell_iter->second)[i]), neworder);
								
								((sell_iter->second)[i]).change_quantity(neworder.get_quantity());
								neworder.change_quantity(neworder.get_quantity());


							}
							else  
							{
								hhhh.transaction(neworder, ((sell_iter->second)[i]));
								neworder.change_quantity(((sell_iter->second)[i]).get_quantity());
								((sell_iter->second)[i]).change_quantity(((sell_iter->second)[i]).get_quantity());
							}
						}
						
						//*******************************
						while (1)
						{
							vector<order>::iterator temp = (sell_iter->second).begin();
							if (temp == (sell_iter->second).end())
								break;
							else if ((*temp).get_quantity() == 0)
								(sell_iter->second).erase(temp);
							else
								break;
						}

						if ((sell_iter->second).size() == 0)
							sell_list.erase(sell_iter);
						else
							break;
						//*******************************
					}
				}
			}
			if (neworder.get_stock() == "H")
			{
				while (1)
				{
					map<double, vector<order>>::iterator sell_iter = sell_list.begin();
					if (sell_iter == sell_list.end())
						break;
					else
					{
						//****************************
						for (int i = 0; i <= (sell_iter->second).size() - 1; i++)
						{
							if (((sell_iter->second)[i]).get_quantity() >= neworder.get_quantity())
							{

								hhhh.transaction(((sell_iter->second)[i]), neworder);
								
								((sell_iter->second)[i]).change_quantity(neworder.get_quantity());
								neworder.change_quantity(neworder.get_quantity());


							}
							else  
							{
								hhhh.transaction(neworder, ((sell_iter->second)[i]));
								neworder.change_quantity(((sell_iter->second)[i]).get_quantity());
								((sell_iter->second)[i]).change_quantity(((sell_iter->second)[i]).get_quantity());
							}
						}
						
						//*******************************
						while (1)
						{
							vector<order>::iterator temp = (sell_iter->second).begin();
							if (temp == (sell_iter->second).end())
								break;
							else if ((*temp).get_quantity() == 0)
								(sell_iter->second).erase(temp);
							else
								break;
						}

						if ((sell_iter->second).size() == 0)
							sell_list.erase(sell_iter);
						else
							break;
						//*******************************
					}
				}
			}
		}

	}
	else
	{
		//sell side// order="S";
		if ((buy_list.empty()))
		{
			if (neworder.get_stock() == "S" || neworder.get_stock() == "G" || neworder.get_stock() == "H")
			{
				sell_list[neworder.get_quantity()].push_back(neworder);
				hhhh.fullinlist(neworder);
				
				starmark = 1;
			}
			else
				hhhh.returned(neworder);
		}
		else{
			//~~~~~~~~~~~~~
			if (neworder.get_stock() == "S")
			{
				while (1)
				{
					map<double, vector<order>>::iterator buy_iter = buy_list.end();
					if (buy_list.empty())
						break;
					else
					{
						//****************************
						buy_iter--;
						int xx = 0;
						for (int i = 0; i <= (buy_iter->second).size() - 1; i++, xx++)
						{
							if (((buy_iter->second)[i]).get_stock()==neworder.get_stock())
							{
								if (((buy_iter->second)[i]).get_quantity() >= neworder.get_quantity())
								{
									hhhh.transaction(((buy_iter->second)[i]), neworder);
									((buy_iter->second)[i]).change_quantity(neworder.get_quantity());
									neworder.change_quantity(neworder.get_quantity());
								}
								else  
								{
									hhhh.transaction(neworder, ((buy_iter->second)[i]));
									neworder.change_quantity(((buy_iter->second)[i]).get_quantity());
									((buy_iter->second)[i]).change_quantity(((buy_iter->second)[i]).get_quantity());
								}
							}
						}
						
						while (1)
						{

							vector<order>::iterator temp = (buy_iter->second).begin();
							if (temp == (buy_iter->second).end())
								break;
							else if ((*temp).get_quantity() == 0)
								(buy_iter->second).erase(temp);// act like a stack 
							else
								break;
						}
						//*******************************
						if ((buy_iter->second).size() == 0) 
							buy_list.erase(buy_iter);
						else
							break;
					}
				}
			}
			
			if (neworder.get_stock() == "G")
			{
				while (1)
				{
					map<double, vector<order>>::iterator buy_iter = buy_list.end();
					if (buy_list.empty())
						break;
					else
					{
						//****************************
						buy_iter--;
						int xx = 0;
						for (int i = 0; i <= (buy_iter->second).size() - 1; i++, xx++)
						{
							if (((buy_iter->second)[i]).get_stock()==neworder.get_stock())
							{
								if (((buy_iter->second)[i]).get_quantity() >= neworder.get_quantity())
								{
									hhhh.transaction(((buy_iter->second)[i]), neworder);	
									((buy_iter->second)[i]).change_quantity(neworder.get_quantity());
									neworder.change_quantity(neworder.get_quantity());
								}
								else 
								{
									hhhh.transaction(neworder, ((buy_iter->second)[i]));
									neworder.change_quantity(((buy_iter->second)[i]).get_quantity());
									((buy_iter->second)[i]).change_quantity(((buy_iter->second)[i]).get_quantity());
								}
							}
						}
						
						while (1)
						{

							vector<order>::iterator temp = (buy_iter->second).begin();
							if (temp == (buy_iter->second).end())
								break;
							else if ((*temp).get_quantity() == 0)
								(buy_iter->second).erase(temp);// act like a stack 
							else
								break;
						}
						//*******************************
						if ((buy_iter->second).size() == 0) 
							buy_list.erase(buy_iter);
						else
							break;
					}
				}
			}
			if (neworder.get_stock() == "H")
			{
				while (1)
				{
					map<double, vector<order>>::iterator buy_iter = buy_list.end();
					if (buy_list.empty())
						break;
					else
					{
						//****************************
						buy_iter--;
						int xx = 0;
						for (int i = 0; i <= (buy_iter->second).size() - 1; i++, xx++)
						{
							if (((buy_iter->second)[i]).get_stock()==neworder.get_stock())
							{
								if (((buy_iter->second)[i]).get_quantity() >= neworder.get_quantity())
								{
									hhhh.transaction(((buy_iter->second)[i]), neworder);	
									((buy_iter->second)[i]).change_quantity(neworder.get_quantity());
									neworder.change_quantity(neworder.get_quantity());
								}
								else  
								{
									hhhh.transaction(neworder, ((buy_iter->second)[i]));
									neworder.change_quantity(((buy_iter->second)[i]).get_quantity());
									((buy_iter->second)[i]).change_quantity(((buy_iter->second)[i]).get_quantity());
								}
							}
						}
					
						while (1)
						{

							vector<order>::iterator temp = (buy_iter->second).begin();
							if (temp == (buy_iter->second).end())
								break;
							else if ((*temp).get_quantity() == 0)
								(buy_iter->second).erase(temp);// act like a stack 
							else
								break;
						}
						//*******************************
						if ((buy_iter->second).size() == 0) 
							buy_list.erase(buy_iter);
						else
							break;
					}
				}
			}
		}
	}
	if (neworder.get_quantity() != 0)
	{
		if (neworder.get_stock() == "S"|| neworder.get_stock() == "G" || neworder.get_stock() == "H")

		{
			if ((neworder.get_side() == "B") && starmark == 0)
				buy_list[neworder.get_quantity()].push_back(neworder);
			else if ((neworder.get_side() == "S") && starmark == 0)
				sell_list[neworder.get_quantity()].push_back(neworder);
			hhhh.partinlist(neworder);
		}
	}
	else{

		//asdsadsadsad
	}

}
void structure::display_sell(){
	map<double, vector<order>>::iterator iter = sell_list.begin();
	cout << '\t' << "       " << "Offer List" << endl;
	cout << "      " << "Offer Price ($) " << '\t' << "quantity " << endl;
	for (; iter != sell_list.end(); iter++)
	{

		cout << '\t' << "   " << iter->first << "   ";
		int sum_quantity = 0;
		for (vector<order>::iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2)
		{
			sum_quantity += (*iter2).get_quantity();
		}
		cout << "         " << sum_quantity << endl;
	}
	cout << "****************************************" << endl;
	cout << "****************************************" << endl;

}
void structure::display_buy(){
	cout << '\t' << '\t' << "Bid List" << endl;
	cout << '\t' << "Bid Price ($) " << '\t' << "quantity " << endl;
	if (buy_list.size() != 0)
	{
		map<double, vector<order>>::iterator iter = buy_list.end();
		--iter;
		int x = buy_list.size();
		for (; x > 0; x--, iter--)
		{
			cout << '\t' << "   " << iter->first << "   ";
			int sum_quantity = 0;
			for (vector<order>::iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2)
			{
				sum_quantity += (*iter2).get_quantity();
			}
			cout << '\t' << "  " << sum_quantity << endl;
		}

	}
	cout << endl << endl;
}