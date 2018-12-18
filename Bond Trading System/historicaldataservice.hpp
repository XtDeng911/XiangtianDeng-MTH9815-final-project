/**
 * historicaldataservice.hpp
 * historicaldataservice.hpp
 *
 * @author Breman Thuraisingham
 * Defines the data types and Service for historical data.
 *
 * @author Breman Thuraisingham
 */
#ifndef HISTORICAL_DATA_SERVICE_HPP
#define HISTORICAL_DATA_SERVICE_HPP

/**
 * Service for processing and persisting historical data to a persistent store.
 * Keyed on some persistent key.
 * Type T is the data type to persist.
 */
#include <tuple>
#include<string>
#include "soa.hpp"
#include "products.hpp"
#include "riskservice.hpp"
#include "positionservice.hpp"
#include "Connctorslmpl.hpp"
#include "streamingservice.hpp"

using std::string;
/**
 * Service for processing and persisting historical data to a persistent store.
 * Keyed on some persistent key.
 * Type T is the data type to persist.
 */
template<typename T>
class HistoricalDataService : Service<string,T>
{
    
public:
    virtual void PersistData(string persistKey, const T& data) {}
};

template<typename V>
class PositionHistoricalData: HistoricalDataService<Position<V> >
{
private:
    PositionConnector<V>* position_connector;
public:
    explicit PositionHistoricalData(PositionConnector<V>* position_connector_)
    {
        position_connector = position_connector_;
    }
    
    virtual void PersistData(string persistKey,Position<V>& data) 
    {
        position_connector->Publish(data);
    }
};

template<typename V>
class RiskHistoricalData: HistoricalDataService<PV01<V> >
{
private:
    RiskConnector<V>* position_connector;
public:
    explicit RiskHistoricalData(RiskConnector<V>* position_connector_)
    {
        position_connector = position_connector_;
    }
    
    void PersistData(string persistKey, const PV01<V>& data) override
    {
        position_connector->Publish(data);
    }
};
template<typename V>
class StreamingHistoricalDataService: HistoricalDataService<PriceStream <V> >
{
private:
    StreamingConnector<V>* streaming_connector;
public:
    explicit StreamingHistoricalDataService(StreamingConnector<V>* streaming_connector_): streaming_connector(streaming_connector_){}
    
    void PersistData(string persistKey, const PriceStream <V>& data) override
    {
        streaming_connector->Publish(data);
    }
};

template<typename V>
class ExecutionHistoricalService: HistoricalDataService<ExecutionOrder<V> >
{
private:
    ExecutionConnector<V>* execution_connector;
public:
    explicit ExecutionHistoricalService(ExecutionConnector<V>* execution_connector_):execution_connector(execution_connector_)
    {}
    
    void PersistData(string persistKey, const ExecutionOrder<V>& data) override
    {
        execution_connector->Publish(data);
    }
};

template<typename V>
class InquiryHistoricalService: HistoricalDataService<Inquiry<V> >
{
private:
    AllInquiriesConnector<V>* all_inquiries_connector;
public:
    explicit InquiryHistoricalService(AllInquiriesConnector<V>* all_inquiries_connector_):all_inquiries_connector(all_inquiries_connector_)
    {}
    
    void PersistData(string persistKey, const Inquiry<V>& data) override
    {
        all_inquiries_connector->Publish(data);
    }
};



#endif
