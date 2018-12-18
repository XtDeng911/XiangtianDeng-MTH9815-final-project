#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Dec  4 22:27:45 2018

@author: dengxiangtian
"""

import random

# ticker name
tickers = ["B02y", "B03y", "B05y", "B07y", "B10y", "B30y"]

# function to generate market prices file
def price_generator():
    #function to gengerate market price, to simpllify the spread will be 2 or 4
    def random_price():
        integer_=random.randint(99,100)
        decimal_32=random.randint(10,31)
        decimal_8=random.randint(0,3)
        spread_random=random.randint(1,2)
        bid_string=str(integer_)+'-'+str(decimal_32)+str(decimal_8)
        if spread_random % 2 == 0:
            ask_string=str(integer_)+"-"+str(decimal_32)+str(decimal_8+2)
        else:
            ask_string=str(integer_)+"-"+str(decimal_32)+str(decimal_8+4)
        
        return bid_string, ask_string
    
    print("generating prices.txt")
    f=open("prices.txt","w")
    price_num=1000000
    for i in range(price_num):
        if i % 100000 == 0:
            print("%d prices generated" %i)
        for ele in tickers:
            bid,ask=random_price()
            # string to put in to 
            s_to_file=ele+','+bid+','+ask+'\n'
            f.write(s_to_file)
    f.close()
    print("finished generating prices.txt")
# generating trades.txt
def trade_generator():
    print("generating trades.txt")
    f=open("trades.txt",'w')
    books=["TRSY1", "TRSY2", "TRSY3"]
    side=["BUY","SELL"]
    trade_id = 0

    for ele in tickers:
        for i in range(10):
            trade_id+=1
            trade_str="TradeId"+str(trade_id)
            
            s_to_file=ele+','+trade_str+','+books[i % 3]+','+str(random.randint(1,5))+'000000'+','+'99.0'+','+side[i % 2]+'\n'
            f.write(s_to_file)
    f.close()
    print("finished generating trades.txt")
    
#format of line of market data:
#bond_name + bid1 + ask1 + ...
def market_data_generator():
    print("generating marketdata.txt")
    f=open("marketdata.txt",'w')
    number_orderbooks=1000000
    cnt=-1
    spread=[2,4,6,8,6,4]
    for i in range(number_orderbooks):
        if i%100000==0:
            print("%d oderbooks generated" %i)
        for ele in tickers:
            cnt+=1
            s_to_file=ele
            integer_part=random.randint(99,100)
            decimal_32=random.randint(12,29)
            decimal_8=random.randint(4,7)
            half_spread=spread[cnt%6]/2
            bid_decimal_8 = decimal_8 - half_spread
            bid_decimal_32 = decimal_32
            ask_decimal_8 = decimal_8 + half_spread
            ask_decimal_32 = decimal_32
            for k in range(5):
                ask_decimal_8+=1
                bid_decimal_8-=1
                if ask_decimal_8>=8:
                    ask_decimal_8 = ask_decimal_8 - 8
                    ask_decimal_32 += 1
                if bid_decimal_8<0:
                    bid_decimal_8 += 8
                    bid_decimal_32 -= 1
                bid = str(integer_part) + '-' + str(bid_decimal_32) + str(int(bid_decimal_8))
                ask = str(integer_part) + '-' + str(ask_decimal_32) + str(int(ask_decimal_8))
                s_to_file+=','+bid+','+ask
            f.write(s_to_file+'\n')
    f.close()
    print("finished generating marketdata.txt")
    
    
def inquiries_generator():
    directions=["BUY","SELL"]
    f=open("inquiries.txt","w")
    cnt=0
    for i in range(10):
        for ele in tickers:
            cnt+=1
            f.write(",".join([str(cnt),ele,directions[cnt % 2],"RECEIVED\n"]))
    f.close()
    print("finished generating inquires.txt")
#generate prices
price_generator()

# generate trades
trade_generator()

#generate marketdata
market_data_generator()

#generate inquiries
inquiries_generator()

