//
//  ProductMap.h
//  Bond Trading System
//
//  Created by 邓翔天 on 2018/12/5.
//  Copyright © 2018 Deng Xiangtian. All rights reserved.
//

//this is to give a map to map different products
#ifndef ProductMap_h
#define ProductMap_h

#include <vector>
#include <unordered_map>
#include "products.hpp"
class ProductMap
{
public:
    static unordered_map<string, Bond> GetProductMap()
    {
        std::unordered_map<string, Bond> product_map;
        vector<Bond> bonds;
        bonds.push_back( Bond("B02y", CUSIP, "B02y", 0.05, date(2020,11,30)));
        bonds.push_back( Bond("B03y", CUSIP, "B03y", 0.05, date(2021,11,30)));
        bonds.push_back( Bond("B05y", CUSIP, "B05y", 0.05, date(2023,11,30)));
        bonds.push_back( Bond("B07y", CUSIP, "B07y", 0.05, date(2025,11,30)));
        bonds.push_back( Bond("B10y", CUSIP, "B10y", 0.05, date(2028,11,30)));
        bonds.push_back( Bond("B30y", CUSIP, "B30y", 0.05, date(2048,11,30)));
        
        for (auto bond: bonds)
        {
            auto pair=make_pair(bond.GetProductId(),bond);
            product_map.insert(pair);
        }
        return product_map;
    }
    
    static vector<Bond> GetProducts(){
        vector<Bond> bonds;
        bonds.push_back( Bond("B02y", CUSIP, "B02y", 0.05, date(2020,11,30)));
        bonds.push_back( Bond("B03y", CUSIP, "B03y", 0.05, date(2021,11,30)));
        bonds.push_back( Bond("B05y", CUSIP, "B05y", 0.05, date(2023,11,30)));
        bonds.push_back( Bond("B07y", CUSIP, "B07y", 0.05, date(2025,11,30)));
        bonds.push_back( Bond("B10y", CUSIP, "B10y", 0.05, date(2028,11,30)));
        bonds.push_back( Bond("B30y", CUSIP, "B30y", 0.05, date(2048,11,30)));
        return bonds;
    }
    
    static vector<string> GetTickers()
    {
        return vector<string>{"B02y","B03y","B05y","B07y","B10y","B30y"};
    }
};

#endif /* ProductMap_h */
