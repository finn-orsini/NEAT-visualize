//
//  genome.hpp
//  Capstone2
//
//  Created by School on 4/7/16.
//  Copyright © 2016 Seraphina Orsini. All rights reserved.
//

#ifndef genome_hpp
#define genome_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "node.hpp"
#include <stack>

class genome
{
public:
    int connections;
    std::vector<node * > nodes;                        //list of all nodes in genome
    double fitness;                                 //fitness of this genome
    double adjustedFitness;                            //adjusted fitness, used in comparing species
    int maxNode;                                    //number of nodes
       int ID;
    std::map<std::string,double> mutationRates;     //map of rate name to value(chance of mutation occuring)
    int numInputs;
    int numOutputs;
    int expectedOffspring;
    
    genome (int inputs,int outputs);
    
    void setFitness(double f);
    
    int randomNode(bool input);
    
    void printGenome();
    
    void writeGenomeToFile();
    
    void clearSearch();
    
    bool depthFirstSearch(int startIndex,int toFind);
    
    
};


#endif /* genome_hpp */
