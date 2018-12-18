//
//  GUIService.h
//  Bond Trading System
//
//  Created by 邓翔天 on 2018/12/8.
//  Copyright © 2018 Deng Xiangtian. All rights reserved.
//

#ifndef GUIService_h
#define GUIService_h
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <chrono>
#include <tuple>

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
#include "Connctorslmpl.hpp"
using std::cout;
using std::endl;
using namespace std::chrono;
template<typename T>
class GUIService : public Service<string,Price<T> >
{
private:
    long long int service_start_time;
    long long int last_quote_time;
    GUIConnector<T>* gui_connector;
    
public:
    explicit GUIService(GUIConnector<T>* gui_connector_):gui_connector(gui_connector_)
    {
        service_start_time = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
        last_quote_time = service_start_time;
    }
    void ProvideData(Price<T> data)
    {
        long long int current_epic = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
        if(current_epic-last_quote_time > 300)
        {
            last_quote_time = current_epic;
            gui_connector->Publish(data);
        }
    }
};
#endif /* GUIService_h */
