//
//  node.cpp
//  Capstone2
//
//  Created by School on 4/7/16.
//  Copyright © 2016 Seraphina Orsini. All rights reserved.
//

#include "node.hpp"

//node constructor
node::node()
{
    discovered = false;
    numIncoming =0;
    incomingCounter =0;
};

//add an outgoing connection
void node::addOutgoing(connection * c)
{   outgoingConnections.push_back(c);
    
};


//return true if this node contains a connection to endNode
bool node::containsConnection(int endNode)
{
    for(int i = 0; i<outgoingConnections.size();i++)
    {   if(outgoingConnections[i]->endNode == endNode)
        {   return true;
        }
    }
    return false;
    
};

//clear value and incoming connection counter
void node::clear()
{   incomingCounter = 0;
    value = 0;
};

//copy node, and all connections
node * node::copyNode()
{    node * n = new node();
        
        for(int i = 0;i<outgoingConnections.size();i++)
        {   n->outgoingConnections.push_back(outgoingConnections[i]->copyConnection());
            
            
        }
    return n;

}
