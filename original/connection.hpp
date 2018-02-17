//
//  connection.hpp
//  Capstone2
//
//  Created by School on 4/7/16.
//  Copyright © 2016 Seraphina Orsini. All rights reserved.
//

#ifndef connection_hpp
#define connection_hpp

#include <stdio.h>
#include "random.hpp"

class connection
{
    
public:
    
    int startNode;
    int endNode;                                    //int id of ending node - i.e. this connects from node start to end
    double weight;                                  //weight of connection
    bool enabled;                                   //if true, connection can be used
    int innovation;                                 //tracks innovation
    
    connection();
    
    connection(int start,int end);
    
    connection(int start,int end,double weight1);
    
    connection * copyConnection();
    
    void connectionMutate(bool replace);
};


bool connectionOrder(const connection  c1, const connection  c2);

bool connectionOrderPointer(const connection*  c1, const connection*  c2);

#endif /* connection_hpp */
