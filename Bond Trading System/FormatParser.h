//
//  Header.h
//  Bond Trading System
//
//  Created by 邓翔天 on 2018/12/7.
//  Copyright © 2018 Deng Xiangtian. All rights reserved.
//

#ifndef Header_h
#define Header_h

#include <iostream>
#include <tuple>
#include <string>
#include <vector>
using namespace std;
class FormatParser{
public:
    static vector<string> Parse(const string & text){
        vector<string> res;
        int i=0;
        istringstream istst(text);
        string token;
        while (getline(istst,token,',')){
            istst.ignore();
            res.push_back(token);
            i++;
        }
        return res;
    }
    
    static vector<string> ParseCommaSepLine(const string & text){
        vector<string> res;
        int i=0;
        istringstream istst(text);
        string token;
        while (getline(istst,token,',')){
            res.push_back(token);
            i++;
        }
        return res;
    }
    static double ParsePriceFormat(const string & price_string){
        const char* prchar = price_string.c_str();
        int priceps[3];
        int part2_start_pos;
        if(prchar[0] == '9')
        {
            priceps[0] = 99;
            part2_start_pos = 3;
        }
        else
        {
            priceps[0] = 100;
            part2_start_pos = 4;
        }
        priceps[1] = (prchar[part2_start_pos]-'0') * 10 + (prchar[part2_start_pos+1] - '0');
        priceps[2] = (prchar[part2_start_pos+2]-'0');
        return priceps[0] + priceps[1] / 32.0 + priceps[2] / 256.0;
    }
    
    static tuple<string,vector<double> > ParseOrderBook(const string&text){
        vector<string> string_res=ParseCommaSepLine(text);
        vector<double> prices;
        string ticker=string_res[0];
        for (int i=1;i<=10;i++){
            prices.push_back(ParsePriceFormat(string_res[i]));
        }
        return std::make_tuple(ticker,prices);
        
    }
    
};

#endif /* Header_h */
