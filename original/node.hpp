//
//  node.hpp
//  Capstone2
//
//  Created by School on 4/7/16.
//  Copyright © 2016 Seraphina Orsini. All rights reserved.
//

#ifndef node_hpp
#define node_hpp

#include <stdio.h>
#include "connection.hpp"
#include <vector>

class node
{   public:
    
    double value;                             //current value of this node, used in evaluation of network
    bool discovered;
    int numIncoming;
    int incomingCounter;
    
    std::vector<connection * > outgoingConnections;
    
    node();
    void addOutgoing(connection * c);           //add connection as outgoing
    bool containsConnection(int endNode);       //return true if this node contains a connection to endNode
    void clear();                               //'clear' node from previous value 
    node * copyNode();
};
#endif /* node_hpp */
