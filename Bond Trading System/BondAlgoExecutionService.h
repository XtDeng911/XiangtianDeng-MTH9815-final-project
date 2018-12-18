//
//  BondAlgoExecutionService.h
//  Bond Trading System
//
//  Created by 邓翔天 on 2018/12/7.
//  Copyright © 2018 Deng Xiangtian. All rights reserved.
//

#ifndef BondAlgoExecutionService_h
#define BondAlgoExecutionService_h
#include<string>
#include "soa.hpp"
#include "marketdataservice.hpp"
#include <iostream>
#include "executionservice.hpp"
#include <tuple>
using namespace std;

template <class T>
class AlgoExecution{
private:
    ExecutionOrder<T>*order;
    Market market;
    static int counter;
public:
    AlgoExecution(OrderBook<T> data_):market(CME)
    {
        counter++;
        PricingSide side=PricingSide(counter % 2);
        string orderID=std::to_string(counter);
        T product=data_.GetProduct();
        double price;
        long quantity;
        double hidden_ratio=0.9;
        
        if(side==BID){
            price=data_.GetBidStack()[0].GetPrice();
            quantity=data_.GetOfferStack()[0].GetQuantity();
        }
        order = new ExecutionOrder<T>(product, side, orderID, MARKET,price,
                                      quantity, quantity*hidden_ratio, orderID, false);

    }
    ExecutionOrder<T> GetOrder()
    {
        return *order;
    }
    Market GetMarket(){
        return market;
    }
    ~AlgoExecution(){
        delete order;
    }
};
template <class T>
int AlgoExecution<T>::counter=0;
template <class T>
class BondAlgoExecutionService: public Service<string, AlgoExecution<T> > {
private:
    
public:
    void Execute(OrderBook<T> data_){
        double best_bid=data_.GetBidStack()[0].GetPrice();
        double best_offer=data_.GetOfferStack()[0].GetPrice();
        double spread=best_offer-best_bid;
        double tol=1./127;
    
    if (spread>tol){
        return;
    }else{
        AlgoExecution<T> algo_execution(data_);
        Service<string,AlgoExecution<T> >::Notify(algo_execution);
    }
    }
};




#endif /* BondAlgoExecutionService_h */
