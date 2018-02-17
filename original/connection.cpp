//
//  connection.cpp
//  Capstone2
//
//  Created by School on 4/7/16.
//  Copyright © 2016 Seraphina Orsini. All rights reserved.
//

#include "connection.hpp"

//create new connection
connection::connection()
{
    weight = 0;
    enabled = true;
    innovation = 0;
};

//create new connection with specified start and end
connection::connection(int start,int end)
{   startNode = start;
    endNode = end;
    weight = 0;
    enabled = true;
    innovation = 0;
};

//create new connection with specified start,end, and weight
connection::connection(int start,int end,double weight1)
{   startNode = start;
    endNode = end;
    weight = weight1;
    enabled = true;
    innovation = 0;
};


//create a copy of a connection
connection * connection::copyConnection()
{   connection * c = new connection(startNode,endNode,weight);
    c->enabled = enabled;
    c->innovation = innovation;
    return c;
};


//mutate weight of connection
void connection::connectionMutate(bool replace)
{   if(replace)
    {   weight =  randomDouble(-1,1);
    }
    else
    {  weight = weight + randomDouble(-1,1);
    
    }
};

//order connections by innovation
bool connectionOrder(const connection  c1, const connection  c2)
{   return c1.innovation < c2.innovation;
    
};


//order connections by innovation
bool connectionOrderPointer(const connection*  c1, const connection*  c2)
{   return c1->innovation < c2->innovation;
    
};
