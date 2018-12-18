//
//  Connctorslmpl.hpp
//  Bond Trading System
//
//  Created by 邓翔天 on 2018/12/7.
//  Copyright © 2018 Deng Xiangtian. All rights reserved.
//

#ifndef Connctorslmpl_h
#define Connctorslmpl_h
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <chrono>
#include <tuple>
#include <string>
#include "soa.hpp"
#include "tradebookingservice.hpp"
#include "FormatParser.h"
#include "ProductMap.h"
#include "pricingservice.hpp"
#include "streamingservice.hpp"
#include "marketdataservice.hpp"
#include "positionservice.hpp"
#include "riskservice.hpp"
#include "executionservice.hpp"
#include "inquiryservice.hpp"
using namespace std;
using namespace std::chrono;
template<typename V>
class TradeBookingConnector: public Connector<Trade<V> >
{
private:
    string file_name;
    TradeBookingService<V> * tradebookingservice;
public:
    explicit TradeBookingConnector(string _file_name, TradeBookingService<V> * _tradebookingservice): file_name(_file_name),
    tradebookingservice(_tradebookingservice){}
    virtual void Publish(const Trade<V> &data){}
    void TraverseTrades()
    {
        string trade_str_format;
        ifstream f(file_name);
        if (f.is_open()) {
            vector<string> line_elements;
            while(getline(f, trade_str_format))
            {
                cout << "New Trade~~~" << endl;
                cout << trade_str_format << endl;
                line_elements = FormatParser::ParseCommaSepLine(trade_str_format);
                unordered_map<string, Bond> product_map = ProductMap::GetProductMap();
                string productID = line_elements[0];
                Bond product = product_map[productID];
                string tradeID = line_elements[1];
                double price = atof(line_elements[4].c_str());
                string book = line_elements[2];
                long quantity = atol(line_elements[3].c_str());
                Side side;
                if(line_elements[5][0] == 'B')
                {
                    side = BUY;
                }
                else
                {
                    side = SELL;
                }
                Trade<Bond> trade(product, tradeID, price, book, quantity, side);
                tradebookingservice->OnMessage(trade);
            }
        } else{
            cout << "error!!! the file " << file_name << " can not be opened!" << endl;
        }
    }
};
template<typename V>
class PositionConnector: public Connector<Position<V> >
{
private:
    string file_name;
public:
    explicit PositionConnector(string file_name_):file_name(file_name_)
    {
        ofstream out(file_name, ios::trunc);
    }
    virtual void Publish(Position<V>& data)
    {
        
        ofstream out(file_name, ios::app);
        string book_name[3] = {"TRSY1", "TRSY2", "TRSY3"};
        long aggregate = 0;
        aggregate += data.GetPosition(book_name[0]) + data.GetPosition(book_name[1]) + data.GetPosition(book_name[2]);
        milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
        out << ms.count() << "," << data.GetProduct().GetTicker() << ","
        << data.GetPosition(book_name[0]) << ","
        << data.GetPosition(book_name[1]) << ","
        << data.GetPosition(book_name[2]) << ","
        << data.GetAggregatePosition() << endl;
    }
};
template<typename V>
class RiskConnector: public Connector<PV01<V> >
{
private:
    string file_name;
public:
    explicit RiskConnector(string file_name_):file_name(file_name_)
    {
        ofstream out(file_name, ios::trunc);
    }
    virtual void Publish(const PV01<V>& data)
    {
        ofstream out(file_name, ios::app);
        string product = data.GetProduct().GetTicker();\
        milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
        out << ms.count() << "," << data.GetProduct().GetTicker() << ","
        << data.GetTotalPV01() << endl;
    }
};
template<typename V>
class PricingConnector: public Connector<Price <V> >
{
private:
    string file_name;
    PricingService<V>* pricing_service;
public:
    explicit PricingConnector(string file_name_, PricingService<V>* pricing_service_):file_name(file_name_), pricing_service(pricing_service_){}
    virtual void Publish(const Price <V>& data){}
    void Subscribe()
    {
        string raw_line;
        ifstream f(file_name);
        int curr_line = 0;
        if (f.is_open())
        {
            cout << " prices from prices.txt loading" << endl;
            vector<string> line_elements;
            while(getline(f, raw_line))
            {
                curr_line++;
                if(curr_line%100000 == 0)
                    cout << curr_line << " prices processed" << endl;
                line_elements = FormatParser::ParseCommaSepLine(raw_line);
                unordered_map<string, Bond> product_map = ProductMap::GetProductMap();
                string productID = line_elements[0];
                Bond product = product_map[productID];
                double bid = FormatParser::ParsePriceFormat(line_elements[1]);
                double ask = FormatParser::ParsePriceFormat(line_elements[2]);
                double mid = (bid+ask)/2;
                double spread = ask - bid;
                Price<V> price(product, mid, spread);
                pricing_service->OnMessage(price);
            }
        } else{
            cout << "the file " << file_name << " can not be opened!" << endl;
        }
    }
};
template<typename V>
class GUIConnector: public Connector<Price<V> >
{
private:
    string file_name;
public:
    explicit GUIConnector(string file_name_):file_name(file_name_)
    {
        ofstream out(file_name, ios::trunc);
    }
    virtual void Publish(const Price<V>& data)
    {
        ofstream out(file_name, ios::app);
        milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
        out << ms.count() << "," << data.GetProduct().GetTicker()
        << "," << data.GetMid() << "," << data.GetBidOfferSpread() << endl;
    }
};
template<typename V>
class StreamingConnector: public Connector<PriceStream<V> >
{
private:
    string file_name;
public:
    explicit StreamingConnector(const string& file_name_):file_name(file_name_)
    {
        ofstream out(file_name, ios::trunc);
    }
    virtual void Publish(const PriceStream<V>& data)
    {
        ofstream out(file_name, ios::app);
        milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
        out << ms.count() << "," << data.GetProduct().GetTicker() << ","
        << data.GetBidOrder().GetPrice() << ","
        << data.GetOfferOrder().GetPrice() << endl;
    }
};
template<typename T>
class MrktDataConnector: public Connector<OrderBook <T> >
{
private:
    string file_name;
    MarketDataService<T>* marketdataservice;
public:
    explicit MrktDataConnector(const string& file_name_,
                               MarketDataService<T>* marketdataservice_):file_name(file_name_), marketdataservice(marketdataservice_){}
    virtual void Publish(const OrderBook <T>& data)
    {}
    
    
    void Subscribe()
    {
        string raw_line;
        ifstream f(file_name);
        int curr_line = 0;
        if (f.is_open())
        {
            cout << "order books loading "<< endl;
            vector<string> line_elements;
            int percentage_finished = 0;
            while(getline(f, raw_line))
            {
                curr_line++;
                if(curr_line == 100000)
                {
                    
                    cout << curr_line << " order books finished processing" << endl;
                }
                string ticker = std::get<0>(FormatParser::ParseOrderBook(raw_line));
                vector<double> prices = std::get<1>(FormatParser::ParseOrderBook(raw_line));
                
                vector<Order> bid_stack;
                vector<Order> offer_stack;
                
                for(int i = 0; i < 5; i++)
                {
                    bid_stack.push_back(Order(prices[i*2], 1000000*(i+1), BID));
                    offer_stack.push_back(Order(prices[i*2+1], 1000000*(i+1), OFFER));
                }
                unordered_map<string, Bond> product_map = ProductMap::GetProductMap();
                Bond product = product_map[ticker];
                
                OrderBook<Bond> order_book(product, bid_stack, offer_stack);
                marketdataservice->ProcessOrderBook(order_book);
            }
        } else {
            cout << "error!!! the file " << file_name << " can not be opened!" << endl;
        }
    }
};
template<typename V>
class ExecutionConnector: public Connector<ExecutionOrder<V> >
{
private:
    string file_name;
public:
    explicit ExecutionConnector(string file_name_):file_name(file_name_)
    {
        ofstream out(file_name, ios::trunc);
    }
    virtual void Publish(const ExecutionOrder<V>& data)
    {
        ofstream out(file_name, ios::app);
        milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
        string side;
        if(data.GetPricingSide() == BID)
            side = "BUY";
        else
            side = "SELL";
        out << ms.count() << "," << data.GetProduct().GetTicker()
        << "," << "TID_" << data.GetOrderId() << ",MarketOrder"
        << "," << side << "," << data.GetPrice()
        << "," << data.GetVisibleQuantity() << "," << data.GetHiddenQuantity()
        << endl;
    }
};
template<typename V>
class InquiryConnector: public Connector<Inquiry<V> >
{
private:
    string file_name;
    InquiryService<V>* inquiry_service;
public:
    explicit InquiryConnector(string file_name_, InquiryService<V>* inquiry_service_):file_name(file_name_), inquiry_service(inquiry_service_){}\
    virtual void Publish(const Inquiry<V>& data)
    {
        InquiryState state = data.GetState();
        if(state == RECEIVED)
        {
            Inquiry<V> new_state_inquiry = data;
            new_state_inquiry.SetState(QUOTED);
            inquiry_service->OnMessage(new_state_inquiry);
            
            new_state_inquiry.SetState(DONE);
            inquiry_service->OnMessage(new_state_inquiry);
        }
    }
    virtual void Subscribe()
    {
        string raw_line;
        ifstream f(file_name);
        if (f.is_open())
        {
            cout << "Inquiries reading from inquiries.txt is coming loading" << endl;
            vector<string> line_elements;
            string ticker;
            string quote_id;
            string side;
            Side side_enum;
            while(getline(f, raw_line))
            {
                cout << raw_line << endl;
                line_elements = FormatParser::ParseCommaSepLine(raw_line);
                
                quote_id = line_elements[0];
                ticker = line_elements[1];
                side = line_elements[2];
                if(side[0] == 'B')
                    side_enum = BUY;
                else
                    side_enum = SELL;
                unordered_map<string, Bond> product_map = ProductMap::GetProductMap();
                Bond product = product_map[ticker];
                Inquiry<Bond> inquiry(quote_id, product, side_enum, 1000000, -1, RECEIVED);
                inquiry_service->OnMessage(inquiry);
                Inquiry<V> new_state_inquiry = inquiry;
                new_state_inquiry.SetState(QUOTED);
                inquiry_service->OnMessage(new_state_inquiry);
                new_state_inquiry.SetState(DONE);
                inquiry_service->OnMessage(new_state_inquiry);
            }
        }
        else
        {
            cout << "error!!! the file " << file_name << " can not be opened!" << endl;
        }
    }
};
template<typename V>
class AllInquiriesConnector: public Connector<Inquiry<V> >
{
private:
    string file_name;
public:
    explicit AllInquiriesConnector(string file_name_):file_name(file_name_)
    {
        ofstream out(file_name, ios::trunc);
        out.close();
    }
    virtual void Publish(const Inquiry<V>& data)
    {
        ofstream out(file_name, ios::app);
        milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
        string state;
        InquiryState state_enum = data.GetState();
        if(state_enum == RECEIVED)
            state = "RECEIVED";
        else if(state_enum == QUOTED)
            state = "QUOTED";
        else if(state_enum == DONE)
            state = "DONE";
        string side;
        auto side_enum = data.GetSide();
        if(side_enum == BUY)
            side = "BUY";
        else
            side = "SELL";
        out << ms.count() << "," << "TID_" << data.GetInquiryId()
        << "," << data.GetProduct().GetTicker()<< "," << side
        << "," << data.GetPrice() << "," << state << endl;
        out.close();
    }
};
#endif /* Connctorslmpl_h */
