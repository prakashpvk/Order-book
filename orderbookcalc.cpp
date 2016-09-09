#include <iostream> 
#include <string> 
#include <sstream> 
#include <vector> 
#include <map> 


//#include <SciDBAPI.h> 
//
//#include <boost/shared_ptr.hpp> 
//#include <boost/foreach.hpp> 
//
//
//#include "query/Operator.h" 
//#include "system/Exceptions.h" 
//#include "query/LogicalExpression.h" 
//
//
//#include <log4cxx/logger.h> 
//
//

//using namespace scidb; 


using namespace std; 

namespace orderbook 
{ 


struct Order 
{ 
  enum Side 
  { 
    NONE = 0, 
    BUY = 1, 
    SELL = 2 
  }; 


  Side side;
  char *trader;
  int stock;
  int quantity;

  Order(): 
     side(NONE), stock(0), quantity(0) ,trader(NULL)
  {} 


  void reset() 
  { 
     side = NONE;  
     stock = 0; 
     quantity = 0; 
	 trader = NULL;
  } 
};  




class orderbookCalculate 
{ 
private: 
    map<int, Order>    _idToOrder; 
    map<int, int>         _priceToBuyVolume; 
    map<int, int>         _priceToSellVolume; 
    string                _currentSymbol; 
    bool                  _flag; 
    map<int, int>::reverse_iterator _highestBuyIter; 
    map<int, int>::iterator         _lowestSellIter; 


public: 
    orderbookCalculate(const string& name, Type const& aggregateType): 
        Aggregate(name, aggregateType, aggregateType), 
        _flag(true) 
    { 
    } 


    virtual AggregatePtr clone() const 
    { 
         return AggregatePtr(new orderbookAggregate(getName(), getAggregateType())); 
     } 
 

     AggregatePtr clone(Type const& aggregateType) const 
     { 
         return AggregatePtr(new orderbookAggregate(getName(), aggregateType)); 
     } 
 

     bool ignoreNulls() const 
     { 
         return true; 
     } 
 

     Type getStateType() const 
     { 
         return getAggregateType(); 
     } 
 

     void initializeState(Value& state) 
     { 
         state.setNull(); 
     } 
      
     void innerIncorporateOrder(Order const& order, int const id, char action) 
     { 
         switch (action) 
         { 
         case 'A':   // Add an order 
         { 
            pair<map<int,Order>::iterator, bool> p = _idToOrder.insert(pair<int,Order>(id, order)); 
            if (p.second) 
            { 
                if (order.side == Order::BUY) 
                { 
                    int& volume = _priceToBuyVolume[order.price]; 
                    volume += order.volume; 
                    if (_highestBuyIter == _priceToBuyVolume.rend() || _highestBuyIter->first < order.price ) 
                    { 
                        _highestBuyIter = _priceToBuyVolume.rbegin(); 
                    } 
                } 
                else 
                { 
                    int& volume = _priceToSellVolume[order.price]; 
                    volume += order.volume;  
                    if (_lowestSellIter == _priceToSellVolume.end() || _lowestSellIter->first > order.price ) 
                    { 
                       _lowestSellIter = _priceToSellVolume.begin(); 
                    } 
                } 
            } 
            break; 
         } 
         case 'M':   // Modify an order 
         { 
           map<uint64_t, Order>::iterator orderIter = _idToOrder.find(id); 
           if ( orderIter == _idToOrder.end()) 
           { 
              break; 
           } 
           Order::Type type = orderIter->second.type; 
           int32_t oldPrice = orderIter->second.price; 
           int32_t oldVolume = orderIter->second.volume; 
           if (type == Order::BUY) 
           { 
                map<int,int>::iterator iter = _priceToBuyVolume.find(oldPrice); 
                iter->second -= oldVolume; 
                if (iter->second <= 0) 
                     _priceToBuyVolume.erase(iter); 
           } 
           else 
           { 
                map<int,int>::iterator iter = _priceToSellVolume.find(oldPrice); 
                iter->second -= oldVolume; 
                if (iter->second <= 0) 
                     _priceToSellVolume.erase(iter); 
           } 
           orderIter->second.type = order.type; 
 	  orderIter->second.price = order.price; 
           orderIter->second.volume = order.volume; 
           if (order.type == Order::BUY) 
           { 
               int& volume = _priceToBuyVolume[order.price]; 
               volume += order.volume; 
           } 
           else 
           { 
               int& volume = _priceToSellVolume[order.price]; 
               volume += order.volume; 
           } 
           _highestBuyIter = _priceToBuyVolume.rbegin(); 
           _lowestSellIter = _priceToSellVolume.begin(); 
           break; 
         } 
         case 'D':   // Delete an order 
         { 
           map<uint64_t, Order>::iterator orderIter = _idToOrder.find(id); 
           if ( orderIter == _idToOrder.end()) 
           { 
              break; 
           } 
           Order::Type type = orderIter->second.type; 
           int32_t price = orderIter->second.price; 
           int32_t volume = orderIter->second.volume; 
           if (type == Order::BUY) 
           { 
                map<int,int>::iterator iter = _priceToBuyVolume.find(price); 
                iter->second -= volume; 
                if (iter->second <= 0) 
                     _priceToBuyVolume.erase(iter); 
                _highestBuyIter = _priceToBuyVolume.rbegin(); 
           } 
           else 
           { 
                map<int,int>::iterator iter = _priceToSellVolume.find(price); 
                iter->second -= volume; 
                if (iter->second <= 0) 
                     _priceToSellVolume.erase(iter);  
                _lowestSellIter = _priceToSellVolume.begin(); 
           } 
           _idToOrder.erase(orderIter);   
           break; 
         } 
         default: 
           break; 
         } 
     } 
 

     void incorporateOrder (string const& orderString) 
     { 
         stringstream ss(orderString); 
         Order order; 
         string orderField; 
         uint64_t id = 0; 
         int counter = 0; 
         char action = 'A'; 
         while (getline(ss, orderField, ',')) 
         { 
           if (counter == 0) 
           { 
               action = orderField[0]; 
               if (action != 'A' && action != 'M' && action != 'D') 
               { 
                   ostringstream err;  
                   err<<"Encoutnered illegal action; action="<<action<<" orderField="<< orderField<< "orderString="<<orderString; 
                   throw SYSTEM_EXCEPTION(SCIDB_SE_INTERNAL, SCIDB_LE_ILLEGAL_OPERATION) << err.str(); 
               } 
           } 
           else if (counter == 1) 
           {  
              id = strtoull(orderField.c_str(), NULL, 10); 
           } 
           else if (counter == 2) 
           { 
              order.price = int32_t(1000*atof(orderField.c_str())); 
              if (order.price < 0) 
                   throw SYSTEM_EXCEPTION(SCIDB_SE_INTERNAL, SCIDB_LE_ILLEGAL_OPERATION) << "Encountered negative price"; 
           } 
           else if (counter == 3) 
           { 
              order.volume = atoi(orderField.c_str()); 
           } 
           else if (counter == 4)  
           {  //symbol 
           } 
           else if (counter == 5) 
           { 
              if (orderField[0] == 'B')   
                  order.type = Order::BUY; 
              else if (orderField[0] == 'S') 
                  order.type = Order::SELL; 
              else                              
                  throw SYSTEM_EXCEPTION(SCIDB_SE_INTERNAL, SCIDB_LE_ILLEGAL_OPERATION) << "Encountered illegal order type"; 
           }  
           else 
           { 
              throw SYSTEM_EXCEPTION(SCIDB_SE_INTERNAL, SCIDB_LE_ILLEGAL_OPERATION) << "Encountered an order with more than 6 fields"; 
           } 
           ++counter; 
         } 
         innerIncorporateOrder(order, id, action); 
     }    
 

     string getSymbol(string const& val) 
     { 
         stringstream ss(val); 
         string symbol; 
         int counter = 0; 
         while (getline(ss, symbol, ',')) 
         {     
            if (counter == 4) 
                return symbol; 
            ++counter; 
         } 
         return symbol; 
     } 
 

     void accumulate(Value& state, Value const& input) 
     { 
       string val = input.getString(); 
       string symbol = getSymbol(val); 
       if(symbol!=_currentSymbol) 
       { 
           _priceToSellVolume.clear(); 
           _priceToBuyVolume.clear(); 
           _idToOrder.clear(); 
           _currentSymbol = symbol; 
           _flag = true; 
           _highestBuyIter = _priceToBuyVolume.rend(); 
           _lowestSellIter = _priceToSellVolume.end(); 
       } 
       if (_flag == true) 
       { 
           _flag = false; 
       } 
       else 
       { 
           _flag =true; 
           return; 
       } 
       stringstream ss(val); 
       string item; 
       vector<string> adds; 
       vector<string> mods; 
       vector<string> dels; 
       while (getline(ss, item, '|')) 
       { 
           if (item[0]=='A') 
               adds.push_back(item); 
           else if (item[0] =='M') 
               mods.push_back(item); 
           else if (item[0] == 'D' ) 
               dels.push_back(item); 
           else 
               throw SYSTEM_EXCEPTION(SCIDB_SE_INTERNAL, SCIDB_LE_ILLEGAL_OPERATION) << "Encountered a record with improper starting field"; 
       } 
       for (size_t i =0; i<adds.size(); ++i) 
       { 
           incorporateOrder(adds[i]); 
       } 
       for (size_t i =0; i<mods.size(); ++i) 
       { 
           incorporateOrder(mods[i]); 
       } 
       for (size_t i =0; i<dels.size(); ++i) 
       { 
           incorporateOrder(dels[i]); 
       } 
     } 
 

     void merge(Value& dstState, Value const& srcState) 
     { 
       throw SYSTEM_EXCEPTION(SCIDB_SE_INTERNAL, SCIDB_LE_ILLEGAL_OPERATION) << "All time must be in one chunk!"; 
     } 
 

     void finalResult(Value& result, Value const& state) 
     { 
         unsigned int j,k; 
         unsigned int depth = 10;  // XXX FIXED DEPTH--find a way to parameterize this? 
         char buf[128];  
         string ans = ""; 
         map<int,int>::iterator i; 
 

         memset(buf, 0, 128); 
 /* Write out the formatted bid prices */ 
         j = 0; 
         k = 0; 
         for(i = _priceToBuyVolume.begin(); i != _priceToBuyVolume.end(); ++i) 
         { 
           if(_priceToBuyVolume.size() - j > depth) 
           { 
             ++j; 
             continue; 
           } 
           if(k==0) 
           { 
             snprintf(buf, 128, "%.3f,%.0f", ((double)i->first)/1000.0, (double)i->second); 
             k = 1; 
           } else 
           { 
             snprintf(buf, 128, ", %.3f,%.0f", ((double)i->first)/1000.0, (double)i->second); 
           } 
           ans = ans + buf; 
           ++j; 
         } 
         ans = ans + " | "; 
 /* Write out the formatted ask prices */ 
         memset(buf, 0, 128); 
         j = 0; 
         for(i = _priceToSellVolume.begin(); i != _priceToSellVolume.end(); ++i) 
         { 
           if(j==0) 
           { 
             snprintf(buf, 128, "%.3f, %.0f", ((double)i->first)/1000.0, (double)i->second); 
           } else 
           { 
             snprintf(buf, 128, ", %.3f, %.0f", ((double)i->first)/1000.0, (double)i->second); 
           } 
           ans = ans + buf; 
           ++j; 
           if(j>depth) break; 
         } 
         result.setString(ans.c_str()); 
     } 
 }; 
 

 

 

 vector<AggregatePtr> _aggregates; 
 EXPORTED_FUNCTION const vector<AggregatePtr>& GetAggregates() 
 { 
     return _aggregates; 
 } 
 

 class orderbookAggregateGeneratorInstance 
 { 
 public: 
     orderbookAggregateGeneratorInstance() 
     { 
         //Add new aggregates here: 
         _aggregates.push_back(AggregatePtr(new orderbookAggregate("orderbook", TypeLibrary::getType(TID_VOID)))); 
 

     } 
 } _aggregateGeneratorInstance; 
 

 } 


