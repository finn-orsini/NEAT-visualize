//
//  genome.cpp
//  Capstone2
//
//  Created by School on 4/7/16.
//  Copyright © 2016 Seraphina Orsini. All rights reserved.
//

#include "genome.hpp"


genome::genome (int inputs,int outputs)
{ 
    connections = 0;
    maxNode = 0;
    numInputs = inputs;
    numOutputs = outputs;
    //  globalRank = 0;
    
    //CONNECTION MUTATIONS
    mutationRates.insert(std::pair<std::string,double>("weightMutate",0.860));                    //mutate weight chance
    mutationRates.insert(std::pair<std::string,double>("weightMutatePower",1));
    mutationRates.insert(std::pair<std::string,double>("weightReplace",0.0245));                  //weight replace chance
    mutationRates.insert(std::pair<std::string,double>("weightStdDev", 1.0));                     //std dev for replace
    mutationRates.insert(std::pair<std::string,double>("toggleConnection", 0.00138));              //enable/disable switch chance
    
    
    //TOPOLOGY MUTATIONS
    mutationRates.insert(std::pair<std::string,double>("addConnection",0.09));                   //mutate add connection
    mutationRates.insert(std::pair<std::string,double>("addNode",0.0009));
    mutationRates.insert(std::pair<std::string,double>("perturbChance",0.09));
    
    
};

void genome::setFitness(double f)
{   fitness = f;
    
};


int genome::randomNode(bool input)
{   if(input)
    {   //return an input node
        int index =  randomInt(0,  numInputs-1);
        return index;
    }
    else
    {   //return index which is not input, could be output or hidden
        return randomInt(0, maxNode-1);
    }
    
};


//clear discovery booleans from depth first search
void genome::clearSearch()
{   for(int i = 0; i< nodes.size();i++)
    {   nodes[i]->discovered = false;
    
    }
};

//search for toFind from node @ startIndex
bool genome::depthFirstSearch(int startIndex,int toFind)
{   bool found = false;
   // printGenome();
    //init stack
    //push start on stack
    std::stack<int> s;
    s.push(startIndex);
    
    int currentIndex;
    
    while(!s.empty() && !found)
    {  // std::cout << "stuck in search\n";
        currentIndex = s.top();
        s.pop();
        
        //if current index is not discovered, label as discovered
        
        if(! nodes[currentIndex]->discovered)
        {
             nodes[currentIndex]->discovered = true;
        }
        
        //for all edges leaving current
        for(int i = 0; i< nodes[currentIndex]->outgoingConnections.size(); i++)
        {   connection * c = nodes[currentIndex]->outgoingConnections[i];
            if(c->endNode == toFind)
            {   found = true;
            }
           // std::cout << "endnode: " << c->endNode<<"\n";
            s.push(c->endNode);
        }
    }
    
    
    //while stack is not empy
    //current v = stack.pop()
    //if v is not discovered, label as discovered
    //for all edges w leaving from v
    //push(w)
    
    return found;
    
};


void genome::printGenome()
{   std::cout  << "Fitness: "<<  fitness << "\n\t";
    std::cout  << "# Nodes: " <<  maxNode << "\n\n\t";
    std::cout  << "Genes" << "\n";
    for(int k = 0;k< nodes.size();k++)
    {   for(int l=0;l< nodes[k]->outgoingConnections.size();l++)
    {    connection * c =  nodes[k]->outgoingConnections[l];
        std::cout << "\t\tStart Node:" << c->startNode  << "\n";
        std::cout << "\t\tEnd Node:" << c->endNode  << "\n";
        std::cout << "\t\tWeight:" << c->weight  << "\n";
        std::cout << "\t\tInnovation:" << c->innovation  << "\n";
        if(c->enabled)
        {   std::cout << "\t\tEnabled \n";
            
        }
        else
        {   std::cout << "\t\tDisabled \n";
            
        }
        std::cout << "\n";
        
    }
    }
    
    std::cout  << "\n";
    
    std::cout   << "\n";
    
    std::cout  << "-----------------------------------\n";
    
};




void genome::writeGenomeToFile()
{    std::ofstream output ("genome.txt",std::fstream::in | std::fstream::out | std::fstream::trunc);
    if(output.is_open())
    {
        output << "Fitness: "<<   fitness << "\n\t";
        output << "# Nodes: " <<   maxNode << "\n\n\t";
        output << "Genes" << "\n";
        for(int k = 0;k<  nodes.size();k++)
        {   for(int l=0;l<  nodes[k]->outgoingConnections.size();l++)
        {    connection * c =   nodes[k]->outgoingConnections[l];
            output << "\t\tStart Node:" << c->startNode  << "\n";
            output << "\t\tEnd Node:" << c->endNode  << "\n";
            output << "\t\tWeight:" << c->weight  << "\n";
            output << "\t\tInnovation:" << c->innovation  << "\n";
            if(c->enabled)
            {   output << "\t\tEnabled \n";
                
            }
            else
            {   output << "\t\tDisabled \n";
                
            }
            output << "\n";
            
        }
        }
        
        output << "\n";
        
        output  << "\n";
        
        output << "-----------------------------------\n";
        output.close();
    }
    else {
        std::cout << "Unable to open file";
    }
    
};

