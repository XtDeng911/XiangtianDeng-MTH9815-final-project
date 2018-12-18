//
//  main.cpp
//  Bond Trading System
//
//  Created by 邓翔天 on 2018/12/5.
//  Copyright © 2018 Deng Xiangtian. All rights reserved.
//

#include<iostream>
#include "products.hpp"
#include "positionservice.hpp"
#include "tradebookingservice.hpp"
#include "soa.hpp"
#include "Listenerslmpl.h"
#include "pricingservice.hpp"
#include "riskservice.hpp"
#include "Connctorslmpl.hpp"
#include "GUIService.h"
#include "historicaldataservice.hpp"
#include "BondAlgoExecutionService.h"
#include "executionservice.hpp"
#include "inquiryservice.hpp"

using std::cout;
using std::endl;
// this file is used to test
int main()
{
    cout<<"this test takes about 5mins to 15mins depending on the platform, thank you for your time~~~~\n"<<endl;
    
    /*
     This part is for the data flow started with trades.txt
     Two branches:
     a. trades.txt -> trade booking service -> position service -> risk service -> historical data service -> risk.txt
     
     b. trades.txt -> trade booking service -> position service -> historical data service -> position.txt
     */

    TradeBookingService<Bond> trade_booking_service;
    PositionService<Bond> pos_service;
    PositionServiceListener<Bond> position_listener(&pos_service);
    trade_booking_service.AddListener(&position_listener);
    PositionConnector<Bond> position_connector_instance("output/position_first60.txt");
    PositionHistoricalData<Bond> position_history(&position_connector_instance);
    HistPositionListener<Bond> history_position_listener(&position_history);
    pos_service.AddListener(&history_position_listener);
    RiskService<Bond> risk_service_instance;
    RiskServiceListener<Bond> risk_service_listener(&risk_service_instance);
    pos_service.AddListener(&risk_service_listener);
    RiskConnector<Bond> risk_connector("output/risk_first60.txt");
    RiskHistoricalData<Bond> risk_h(&risk_connector);//risk history
    HistRiskListener<Bond> history_risk_listener(&risk_h);
    risk_service_instance.AddListener(&history_risk_listener);
    TradeBookingConnector<Bond> trade_connector("data_generators/trades.txt", &trade_booking_service);
    trade_connector.TraverseTrades();
    
    /*
     This part is for the data flow started with prices.txt
     Two branches:
     1. prices.txt-> bond pricing service -> gui service -> output gui.txt
     2. prices.txt-> bond pricing service -> algo streaming service -> streaming service-> historical data service -> streaming.txt
     */
    
    GUIConnector<Bond> gui_connector("output/gui.txt");
    GUIService<Bond> gui_service(&gui_connector);
    GUIListener<Bond> gui_listener(&gui_service);
    PricingService<Bond> pricing_service;
    pricing_service.AddListener(&gui_listener);
    BondAlgoStreamingService<Bond> bond_algo_streaming_service;
    AlgoStreamingListener<Bond> algo_streaming_listener(&bond_algo_streaming_service);
    pricing_service.AddListener(&algo_streaming_listener);
    StreamingService<Bond> streaming_service;
    StreamingListener<Bond> streaming_listener(&streaming_service);
    bond_algo_streaming_service.AddListener(&streaming_listener);\
    StreamingConnector<Bond> streaming_connector("output/streaming.txt");
    StreamingHistoricalDataService<Bond> streaming_hist_service(&streaming_connector);
    HistStreamingListener<Bond> hist_streaming_listener(&streaming_hist_service);
    streaming_service.AddListener(&hist_streaming_listener);
    PricingConnector<Bond> pricing_connector("data_generators/prices.txt", &pricing_service);
    pricing_connector.Subscribe();
    
    
    
    /*
     * This part is for the data flow started with marketdata.txt
     * Data flow is as following
     *     1. marketdata.txt-> market data service -> Bond algo execution service
     *        -> bond execution service -> trade booking service
     *     2. after reaching trade booking service, the data flow is exactly the same as
     *        in the TradesFlow() function described
     */
    BondAlgoExecutionService<Bond> bond_algo_execution_service_ins;
    BondAlgoExecutionListener<Bond> bond_algo_execution_listener_ins(&bond_algo_execution_service_ins);
    MarketDataService<Bond> mrkt_data_service_ins;
    mrkt_data_service_ins.AddListener(&bond_algo_execution_listener_ins);
    ExecutionService<Bond> execution_service_ins;
    ExecutionServiceListener<Bond> execution_service_listener_ins(&execution_service_ins);
    bond_algo_execution_service_ins.AddListener(&execution_service_listener_ins);
    TradeBookingService<Bond> trade_booking_service_ins;
    TradeBookingServiceListener<Bond> trade_booking_listener_ins(&trade_booking_service_ins);
    execution_service_ins.AddListener(&trade_booking_listener_ins);
    PositionService<Bond> pos_service_ins;
    PositionServiceListener<Bond> position_listenerins(&pos_service_ins);
    trade_booking_service_ins.AddListener(&position_listenerins);
    PositionConnector<Bond> position_connector_ins("output/positions.txt");
    PositionHistoricalData<Bond> position_history_ins(&position_connector_ins);
    HistPositionListener<Bond> history_position_listener_ins(&position_history_ins);
    pos_service_ins.AddListener(&history_position_listener_ins);
    RiskService<Bond> risk_service;
    RiskServiceListener<Bond> risk_service_listener_ins(&risk_service);
    pos_service_ins.AddListener(&risk_service_listener_ins);
    RiskConnector<Bond> risk_connector_ins("output/risk.txt");
    RiskHistoricalData<Bond> risk_history(&risk_connector_ins);
    HistRiskListener<Bond> history_risk_listener_ins(&risk_history);
    risk_service.AddListener(&history_risk_listener_ins);
    ExecutionConnector<Bond> execution_connector("output/executions.txt");
    ExecutionHistoricalService<Bond> execution_hist_service(&execution_connector);
    ExecutionHistoricalDataServiceListener<Bond> execution_hist_service_listener(&execution_hist_service);
    execution_service_ins.AddListener(&execution_hist_service_listener);
    MrktDataConnector<Bond> mrkt_data_connector("data_generators/marketdata.txt", &mrkt_data_service_ins);
    mrkt_data_connector.Subscribe();
    
    
    /* inquiry flow
    */

    AllInquiriesConnector<Bond> all_inquiries_connector("output/allinquiries.txt");
    InquiryHistoricalService<Bond> inquiry_hist_service(&all_inquiries_connector);
    AllInquiryHistoricalDataServiceListener<Bond> all_inquiry_hist_listner(&inquiry_hist_service);
    InquiryService<Bond> inquiry_service;
    inquiry_service.AddListener(&all_inquiry_hist_listner);
    InquiryConnector<Bond> inquiry_connector("data_generators/inquiries.txt", &inquiry_service);
    inquiry_connector.Subscribe();
    cout << "Testing of Bond Trading system finished" << endl;
    
    cout << "Thank you for your time!!" << endl;
}
