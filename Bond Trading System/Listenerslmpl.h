//
//  Listenerslmpl.h
//  Bond Trading System
//
//  Created by 邓翔天 on 2018/12/6.
//  Copyright © 2018 Deng Xiangtian. All rights reserved.
//

/*
 This file is to give derive class of listener which will be used to link different class.
 */
#ifndef Listenerslmpl_h
#define Listenerslmpl_h
#include "positionservice.hpp"
#include "BondAlgoExecutionService.h"
#include "executionservice.hpp"
#include "riskservice.hpp"
#include "GUIService.h"
#include "BondAlgoStreamingService.h"
#include "historicaldataservice.hpp"
#include <tuple>
template<typename T>
    class PositionServiceListener : public ServiceListener<Trade<T> >
    {
    private:
        PositionService<T>* service_;
    public:
        
        explicit PositionServiceListener(PositionService<T>* ser) : service_(ser){}
        virtual void ProcessAdd(Trade<T> &data)
        {
            service_->AddTrade(data);
        }
        virtual void ProcessRemove(Trade<T> &data){}
        virtual void ProcessUpdate(Trade<T> &data){}
    };
    
    template<typename T>
    class RiskServiceListener : public ServiceListener<Position<T> >
    {
    private:
        RiskService<T>* service_;
    public:
        explicit RiskServiceListener(RiskService<T>* ser): service_(ser){}
        virtual void ProcessAdd(Position<T> &data)
        {
            service_->AddPosition(data);
        }
        virtual void ProcessRemove(Position<T> &data){}
        virtual void ProcessUpdate(Position<T> &data){}
    };
    
    template<typename T>
    class HistPositionListener: public ServiceListener<Position<T> >
    {
    private:
        PositionHistoricalData<T>* service_;
    public:
        
        explicit  HistPositionListener(PositionHistoricalData<T>* ser): service_(ser){}
        virtual void ProcessAdd(Position<T> &data_)
        {
            service_->PersistData("key", data_);
        }
        virtual void ProcessRemove(Position<T> &data){}
        virtual void ProcessUpdate(Position<T> &data){}
    };
    template<typename T>
    class HistRiskListener: public ServiceListener<PV01<T> >
    {
    private:
        RiskHistoricalData<T>* service_;
    public:
        explicit HistRiskListener(RiskHistoricalData<T>* ser): service_(ser){}
        virtual void ProcessAdd(PV01<T> &data_)
        {
            service_->PersistData("key", data_);
        }
        virtual void ProcessRemove(PV01<T> &data){}
        virtual void ProcessUpdate(PV01<T> &data){}
    };
    template<typename T>
    class HistStreamingListener:public ServiceListener<PriceStream<T> >
    {
    private:
        StreamingHistoricalDataService<T>* service_;
    public:
        explicit HistStreamingListener(StreamingHistoricalDataService<T>* ser): service_(ser){}
        virtual void ProcessAdd(PriceStream<T> &data_)
        {
            service_->PersistData("key", data_);
        }
        virtual void ProcessRemove(PriceStream<T> &data){}
        virtual void ProcessUpdate(PriceStream<T> &data){}
    };
    
    
    
    template<typename T>
    class GUIListener: public ServiceListener<Price<T> >
    {
    private:
        GUIService<T>* service_;
    public:
        explicit GUIListener(GUIService<T>* ser): service_(ser){}
        virtual void ProcessAdd(Price<T> &data_)
        {
            service_->ProvideData(data_);
        }
        virtual void ProcessRemove(Price<T> &data){}
        virtual void ProcessUpdate(Price<T> &data){}
    };
    
    template<typename T>
    class AlgoStreamingListener:public ServiceListener<Price<T> >
    {
    private:
        BondAlgoStreamingService<T>* service_;
    public:
        explicit AlgoStreamingListener(BondAlgoStreamingService<T>* ser): service_(ser){}
        virtual void ProcessAdd(Price<T> &data)
        {
            service_->PublishPrice(data);
        }
        virtual void ProcessRemove(Price<T> &data){}
        virtual void ProcessUpdate(Price<T> &data){}
    };
    
    template<typename T>
    class StreamingListener:public ServiceListener<PriceStream <T> >
    {
    private:
        StreamingService<T>* service_;
    public:
        explicit StreamingListener(StreamingService<T>* ser): service_(ser){}
        virtual void ProcessAdd(PriceStream <T> &data)
        {
            service_->PublishPrice(data);
        }
        virtual void ProcessRemove(PriceStream <T> &data){}
        virtual void ProcessUpdate(PriceStream <T> &data){}
    };
    
    
    template<typename T>
    class BondAlgoExecutionListener:public ServiceListener<OrderBook <T> >
    {
    private:
        BondAlgoExecutionService<T>* service_;
    public:
        explicit BondAlgoExecutionListener(BondAlgoExecutionService<T>* ser): service_(ser){}
        virtual void ProcessAdd(OrderBook<T> &data)
        {
            service_->Execute(data);
        }
        virtual void ProcessRemove(OrderBook<T> &data){}
        virtual void ProcessUpdate(OrderBook<T> &data){}
    };
    
    template<typename T>
    class ExecutionServiceListener:public ServiceListener<AlgoExecution<T> >
    {
    private:
        ExecutionService<T>* service;
    public:
        explicit ExecutionServiceListener(ExecutionService<T>* service_): service(service_){}
        virtual void ProcessAdd(AlgoExecution<T> &algo_execution)
        {
            service->ExecuteOrder(algo_execution.GetOrder(), algo_execution.GetMarket());
        }
        virtual void ProcessRemove(AlgoExecution<T> &data){}
        virtual void ProcessUpdate(AlgoExecution<T> &data){}
    };
    
    
    template<typename T>
    class TradeBookingServiceListener:public ServiceListener<ExecutionOrder <T> >
    {
    private:
        TradeBookingService<T>* service;
    public:
        explicit TradeBookingServiceListener(TradeBookingService<T>* service_): service(service_){}
        
        virtual void ProcessAdd(ExecutionOrder <T> &algo_execution)
        {
            T product = algo_execution.GetProduct();
            string trade_id = algo_execution.GetOrderId();
            double price = algo_execution.GetPrice();
            
            int id = std::stoi(trade_id);
            vector<string> books;
            books.push_back("TRSY1");
            books.push_back("TRSY2");
            books.push_back("TRSY3");
            string book = books[id%3];
            
            long quantity = algo_execution.GetVisibleQuantity();
            
            PricingSide side = algo_execution.GetPricingSide();
            Side order_side;
            if(side == BID)
                order_side = BUY;
            else
                order_side = SELL;
            
            Trade<T> trade(product, trade_id, price, book, quantity, order_side);
            service->BookTrade(trade);
        }
        
        // Listener callback to process a remove event to the Service
        virtual void ProcessRemove(ExecutionOrder <T> &data)
        {}
        
        // Listener callback to process an update event to the Service
        virtual void ProcessUpdate(ExecutionOrder <T> &data)
        {}
    };
    
    
    template<typename T>
    class ExecutionHistoricalDataServiceListener:public ServiceListener<ExecutionOrder<T> >
    {
    private:
        ExecutionHistoricalService<T>* service_;
    public:
        explicit ExecutionHistoricalDataServiceListener(ExecutionHistoricalService<T>* ser): service_(ser)
        {}
        
        // Listener callback to process an add event to the Service
        virtual void ProcessAdd(ExecutionOrder<T> &algo_execution_)
        {
            service_->PersistData("key", algo_execution_);
        }
        virtual void ProcessRemove(ExecutionOrder<T> &data){}
        virtual void ProcessUpdate(ExecutionOrder<T> &data){}
    };
    
    template<typename T>
    class AllInquiryHistoricalDataServiceListener:public ServiceListener<Inquiry<T> >
    {
    private:
        InquiryHistoricalService<T>* service_;
    public:
        explicit AllInquiryHistoricalDataServiceListener(InquiryHistoricalService<T>* ser_): service_(ser_){}
        virtual void ProcessAdd(Inquiry<T> &data)
        {
            service_->PersistData("key", data);
        }
        virtual void ProcessRemove(Inquiry<T> &data){}
        virtual void ProcessUpdate(Inquiry<T> &data){}
    };
#endif /* Listenerslmpl_h */
