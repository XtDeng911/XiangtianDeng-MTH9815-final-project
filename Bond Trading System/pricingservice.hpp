/**
 * pricingservice.hpp
 * Defines the data types and Service for internal prices.
 *
 * @author Breman Thuraisingham
 */
#ifndef PRICING_SERVICE_HPP
#define PRICING_SERVICE_HPP

#include <string>
#include "soa.hpp"
using namespace std;
/**
 * A price object consisting of mid and bid/offer spread.
 * Type T is the product type.
 */
template<typename T>
class Price
{

public:

  // ctor for a price
  Price(const T &_product, double _mid, double _bidOfferSpread);

  // Get the product
  const T& GetProduct() const;

  // Get the mid price
  double GetMid() const;

  // Get the bid/offer spread around the mid
  double GetBidOfferSpread() const;

private:
  const T& product;
  double mid;
  double bidOfferSpread;

};

/**
 * Pricing Service managing mid prices and bid/offers.
 * Keyed on product identifier.
 * Type T is the product type.
 */
template<typename T>
class PricingService : public Service<string,Price <T> >
{
private:
    std::map<string,Price<T> > prices;
public:
    //getting data
    virtual Price <T>& GetData(string key)
    {

 
            return prices.find(key)->second;

    }

    virtual void OnMessage(Price<T> &data_)
    {
        prices.erase(data_.GetProduct().GetTicker());
        
        
        
        prices.insert(make_pair(data_.GetProduct().GetTicker(),data_));
        
        Service<string,Price<T> >::Notify(data_);
    }
};

template<typename T>
Price<T>::Price(const T &_product, double _mid, double _bidOfferSpread) :
  product(_product)
{
    bidOfferSpread = _bidOfferSpread;
    
  mid = _mid;
  
}

template<typename T>
const T& Price<T>::GetProduct() const
{
  return product;
}

template<typename T>
double Price<T>::GetMid() const
{
  return mid;
}

template<typename T>
double Price<T>::GetBidOfferSpread() const
{
  return bidOfferSpread;
}

#endif
