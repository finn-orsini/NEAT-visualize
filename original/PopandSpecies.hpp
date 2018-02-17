//
//  PopandSpecies.hpp
//  Capstone2
//
//  Created by School on 4/7/16.
//  Copyright © 2016 Seraphina Orsini. All rights reserved.
//

#ifndef PopandSpecies_hpp
#define PopandSpecies_hpp


#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "genome.hpp"




class population;


class species
{
    
public:
    
    std::vector<genome * > genomes;                     //list of genomes in this species
    double topFitness;                                  //value of genome with highest fitness in this species
    int staleness;                                      //tracks progress of species over generations
    double averageFitness;                              //average fitness of all genomes in species
    double averageAdjustedFitness;
    genome * mostFitGenome;
    
    int totalOffspring;
    bool hasBestIndividual;
    bool canReproduce;
    
    int age;
    int stagnant;
    double lastBestFitness;
    
    
    
    species();
    void addToSpecies(genome * g);
    void checkStagnant(double currentSpeciesBestFitness);
    void setAverageFitness();
    bool reproduce(population * p);
    bool fitnessOrder(const species * s1, const species * s2);
    void setAverageAdjustedFitness();
    
};




class population
{
    
public:
    std::vector<species * > speciesList;               //list of all species in this population
    int generation;                                 //int to track # of generations
    int innovation;                                 //"global" innovation counter for this population
    int currentSpecies;                             //counter for current species being evaluated
    int currentGenome;                              //counter for current genome being evaluated
    double maxFitness;                                 //current top fitness of the population
    int popConstant;
    int currentPop;
    int inputs;
    int outputs;
    int genomeId;
    double disjointConstant;
    double excessConstant;
    double weightConstant;
    double deltaDisjoint;
    double deltaWeights;
    double deltaThreshold;
    double staleSpecies;
    double topFitness;
    double avgFitness;
    int maxNodes;
    bool reproduce();
    std::vector<std::pair<int,int>> innovations;
    
    population(int numInputs, int numOutputs);
    
    genome * startingGenome();
    
    int innovationExists(int start, int end);
    
    void initPop();
    
    void writePopToFile();
    
    void mutate(genome * g);
    
    void addNode(genome *g);
    
    void addConnection(genome * g);
    
    void addToSpecies(genome * g, std::vector<species*> * specieList);
    
    bool isSameSpecies(genome * g1, genome *g2);
    
    void newGeneration();
    
    genome * breedChild(species * s);
    
    genome * crossoverGenome(genome * g1, genome *g2);
    
    genome * copyGenome(genome * toCopy);

    void setAdjustedFitness();
    
};


bool innovationOrder(const connection * c1, const connection * c2);

bool fitnessOrder(const species * s1, const species * s2);

bool genomeFitnessOrder(const genome * g1, const genome * g2);

#endif /* PopandSpecies_hpp */
