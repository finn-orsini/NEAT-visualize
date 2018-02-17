//
//  main.cpp
//  Capstone2
//
//  Created by SRORS on 1/28/16.
//  Copyright (c) 2016 Seraphina Orsini. All rights reserved.
//

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include "math.h"
#include "PopandSpecies.hpp"



using namespace std;

double sigmoid(double input);

void displayAllXOR(genome* g);

double evaluateXOR(genome *g, int a, int b);

double evalAllXOR(genome* g);

void runXOROnce();

void run100XOR();


int main(int argc, const char * argv[]) {
    
    
    int option;
    
    do {
        
        cout << "Options:\n";
        cout << "1 - run NEAT on XOR once\n";
        cout << "2 - run NEAT on XOR 100 times, display data\n";
        cout << "-1 to quit\n";
        cout << "Select an option: ";
        cin >> option;
        
       if(option == 1)
       {    runXOROnce();
           
       }
       else if(option == 2)
       {    run100XOR();
            
       }
        
    }
    while(option != -1);
    return 0;
}






void runXOROnce()
{
    //seed rand number generator
    seedRand();
    
    //create new population
    population * pop = new population(3,1);
    //initialize population
    pop->initPop();
    //write population to file (ALL GENOMES)
    pop->writePopToFile();
    
    //generation counter
    int gen = 0;
    //top fitness
    double fitnessMet = 16;
    
    while(true)
    {
        std::cout << "\n" << "Generation: " << gen << "\n";
        //dont allow more than 300 generations
        if( gen > 300)
        {   std::cout << "did not find in 300 generations\n";
            // output << "did not find\n";
            goto fitnessWasNotMet;
        }
        //select each genome
        for(int i = 0; i<pop->speciesList.size();i++)
        {   species * s = pop->speciesList[i];
            for(int j = 0; j<s->genomes.size();j++)
            {
                genome * g = s->genomes[j];
                //EVALUATE ON XOR
                double fitness = evalAllXOR(g);
                g->setFitness(fitness);
                
                if(fitness >= fitnessMet)
                {
                    cout << "Species " << i << " Genome " << j << " fitness " << g->fitness << "\n";
                    cout << "Nodes: " << g->nodes.size() << "\n";
                    displayAllXOR(g);
                    cout << "FITNESS MET\n\n";
                    g->printGenome();
                    goto fitnessWasMet;
                }
            }
        }
        
        cout << "# Species: " << pop->speciesList.size() << "\n";
        cout << "Top Fitness: " << pop->topFitness << "\n";
        cout << "Avg Fitness: " << pop->avgFitness << "\n";
        
        
        gen ++;
        pop->newGeneration();
    }
    
    fitnessWasNotMet:
    return;
    
    fitnessWasMet:
    return;

}

void run100XOR()
{
    int total = 0;
    int sumGenerations = 0;
    int sumHidden = 0;
    int worstGenerations = 0;
    int bestGenerations = 300;

    int c = 100;
    while( c >= 0)
    {
        //setup
        seedRand();
        population * pop = new population(3,1);
        pop->initPop();
        pop->writePopToFile();
        
        //generation counter
         //top fitness
        int gen =0;
        double fitnessMet = 16;
        
        
        
        while(true)
        {
            //dont allow more than 300 generations
            if( gen > 300)
            {   std::cout << "Did not find in 300 generations\n";
                goto fitnessWasMet;
            }
            //select each genome
            for(int i = 0; i<pop->speciesList.size();i++)
            {   species * s = pop->speciesList[i];
                for(int j = 0; j<s->genomes.size();j++)
                {
                    genome * g = s->genomes[j];
                    //EVALUATE ON XOR
                    double fitness = evalAllXOR(g);
                    g->setFitness(fitness);
                    if(fitness >= fitnessMet)
                    {   total += 1;
                        sumGenerations += gen;
                        sumHidden += g->nodes.size()-4;
                        if(gen > worstGenerations)
                        {   worstGenerations = gen;
                            
                        }
                        if(gen < bestGenerations)
                        {   bestGenerations = gen;
                            
                        }
                        cout << "Solution found in gen: " << gen << "\n";
                        cout << "Number of hidden nodes: " << g->nodes.size()-4 << "\n\n";
                        goto fitnessWasMet;
                    }
                }
            }
            
            
            gen ++;
            pop->newGeneration();
        }
    
    
    fitnessWasMet:
        c--;
    
    }

    cout << "Average # Generations to Solution: " << sumGenerations/total << "\n";
    cout << "Best # Generations to Solution: " << bestGenerations << "\n";
    cout << "Worst # Generations to Solution: " << worstGenerations << "\n";
    cout << "Average # Hidden Nodes in Solution: " << sumHidden/total << "\n\n";
    
}







double sigmoid(double input)
{
    return 1.0 / (1.0 + exp(-4.9*input));
}


double evalAllXOR(genome* g)
{
    
    //Evaluate network on all combinations of XOR problem
    //CLEAR all node values between evaluations
    double try1 = evaluateXOR(g,1,1);
    for(int i = 0; i<g->nodes.size();i++)
    {   g->nodes[i]->clear();
        
    }
    double try2 = evaluateXOR(g,1,0);
    for(int i = 0; i<g->nodes.size();i++)
    {   g->nodes[i]->clear();
        
    }
    double try3 = evaluateXOR(g,0,1);
    for(int i = 0; i<g->nodes.size();i++)
    {   g->nodes[i]->clear();
        
    }
    double try4 = evaluateXOR(g,0,0);
    for(int i = 0; i<g->nodes.size();i++)
    {   g->nodes[i]->clear();
        
    }
    
    
    
    double sumError = 0;
    if(try1 < .1)
    {   try1 = 0;
    }
    if(try4 <.1)
    {   try4 = 0;
    }
    if(try2 > .9)
    {   try2 = 1;
    }
    if(try3 > .9)
    {   try3 = 1;
    }
    
    //calculate error
    sumError += abs(try1 - 0);
    sumError += abs(try2 - 1);
    sumError += abs(try3 - 1);
    sumError += abs(try4 - 0);
    
    //return fitness
    return pow((4-sumError),2);
}


void displayAllXOR(genome* g)
{
   
    //Combinations
    //Same as EVAL, except prints results (Used when fitness is met, and for debugging)
    double try1 = evaluateXOR(g,1,1);
    for(int i = 0; i<g->nodes.size();i++)
    {   g->nodes[i]->clear();
        
    }
    double try2 = evaluateXOR(g,1,0);
    for(int i = 0; i<g->nodes.size();i++)
    {   g->nodes[i]->clear();
        
    }
    double try3 = evaluateXOR(g,0,1);
    for(int i = 0; i<g->nodes.size();i++)
    {   g->nodes[i]->clear();
        
    }
    double try4 = evaluateXOR(g,0,0);
    for(int i = 0; i<g->nodes.size();i++)
    {   g->nodes[i]->clear();
        
    }
 
    if(try1 < .1)
    {   try1 = 0;
    }
    if(try4 <.1)
    {   try4 = 0;
    }
    if(try2 > .9)
    {   try2 = 1;
    }
    if(try3 > .9)
    {   try3 = 1;
    }
    
    //     cout << "\n";
    cout << "1,1: " << try1 << " error: "<< pow((try1 - 0),2) <<"\n";
    cout << "1,0: " << try2 << " error: "<<  pow((try2 - 1),2)<< "\n";
    cout << "0,1: " << try3 << " error: "<<  pow((try3 - 1),2)<< "\n";
    cout << "0,0: " << try4 << " error: "<< pow((try4 - 0),2) << "\n";
    
}








//NETWORK EVALUATION
double evaluateXOR(genome *g, int a, int b)
{

    //set inputs and bias
    g->nodes[0]->value = a;
    g->nodes[1]->value = b;
    g->nodes[2]->value = 1;
    
    queue<int> workingQueue;
    for(int i = 0;i<g->numInputs;i++)
    {   node * n = g->nodes[i];
        
       //for each node in inputs
        //send data from outgoing connections
        for(int j= 0;j<n->outgoingConnections.size();j++)
        {
            connection * c = n->outgoingConnections[j];
            if(c->enabled)
            {   //update end node value and incoming counter
                g->nodes[c->endNode]->value += n->value*c->weight;
                g->nodes[c->endNode]->incomingCounter ++;
            
                //if incoming counter has reached total, add to working queue to eval
                if(g->nodes[c->endNode]->numIncoming ==  g->nodes[c->endNode]->incomingCounter)
                {   workingQueue.push(c->endNode);
                }
            }
            else
            {   g->nodes[c->endNode]->incomingCounter ++;
            
            }
        }
    }
    
    
    
    queue<int> nextQueue;
    
    //while working queue exsists
    while(true && !workingQueue.empty())
    {
        int index = workingQueue.front();

        node * n = g->nodes[index];
        for(int j= 0;j<n->outgoingConnections.size();j++)
        {
            connection * c = n->outgoingConnections[j];
            if(c->enabled)
            {
                 //update end node value and incoming counter
                g->nodes[c->endNode]->value += sigmoid(n->value)*n->outgoingConnections[j]->weight;
                g->nodes[c->endNode]->incomingCounter ++;
            
                //if incoming counter has reached total, add to next queue
                if(g->nodes[c->endNode]->numIncoming ==  g->nodes[c->endNode]->incomingCounter)
                {   nextQueue.push(c->endNode);
                }
            }
            else
            {   g->nodes[c->endNode]->incomingCounter ++;
            
            }
        }
        
        workingQueue.pop();
        
        //if no items left in working queue
        if(workingQueue.empty())
        {
            //get node indexes from next queue
            while(!nextQueue.empty())
            {   int toAdd = nextQueue.front();
                
                nextQueue.pop();
                
                //if it is not an output node, add to 'new' working queue
                if(g->nodes[toAdd]->outgoingConnections.size() != 0)
                {
                    workingQueue.push(toAdd);
                }
            }
            
            //if working queue is still empty -> all nodes on next queue were output nodes!
            if(workingQueue.empty())
            {   break;
            }
        }
    
    }
    
    //output node value
    return sigmoid(g->nodes[3]->value);
    
    
}




