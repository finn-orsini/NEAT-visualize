//
//  PopandSpecies.cpp
//  Capstone2
//
//  Created by School on 4/7/16.
//  Copyright © 2016 Seraphina Orsini. All rights reserved.
//

#include "PopandSpecies.hpp"




//ordering functions
bool innovationOrder(const connection * c1, const connection * c2)
{   return c1->innovation < c2->innovation;
    
}

bool fitnessOrder(const species * s1, const species * s2)
{   return s1->topFitness > s2->topFitness;
    
}

bool genomeFitnessOrder(const genome * g1, const genome * g2)
{   return g1->fitness > g2->fitness;
    
}


bool species::fitnessOrder(const species * s1, const species * s2)
{   return s1->topFitness > s2->topFitness;
    
}

//population constructor
population::population(int numInputs, int numOutputs)
{   generation = 0;
    //innovation = outputs?
    innovation = 0;
    currentGenome = 0;
    currentSpecies = 0;
    maxFitness = 0;
    currentPop = 0;
    popConstant = 150;
    inputs = numInputs;
    outputs = numOutputs;
    disjointConstant = 1.0;
    excessConstant = 1.0;
    weightConstant = .4;
    genomeId = 0;
    
    deltaThreshold = 1;
    staleSpecies = 10;
    maxNodes = 1000000;
    
    
};


//add genome g to species genome list
void species::addToSpecies(genome * g)
{   genomes.push_back(g);
    
};


//species constructor
species::species()
{   genomes = *new std::vector<genome *>();
    staleness = 0;
    averageFitness = 0;
    hasBestIndividual = false;
    canReproduce = true;
    age = 0;
    topFitness = -1;
    lastBestFitness = 0;
};

//if best fitness has not increased, increase stagnant coutn
void species::checkStagnant(double currentSpeciesBestFitness)
{   if(currentSpeciesBestFitness <= lastBestFitness)
    {   stagnant += 1;
    }
};

//calculate and set average fitness of species
void species::setAverageFitness()
{   int speciesSize = (int)genomes.size();
    double fitnessSum = 0;
    for(int i = 0;i<speciesSize;i++)
    {   fitnessSum += genomes[i]->fitness;
    }
    averageFitness = fitnessSum/speciesSize;
};


//calculate and set average adjusted fitness of species
void species::setAverageAdjustedFitness()
{   int speciesSize = (int)genomes.size();
   
    double fitnessSum = 0;
    for(int i = 0;i<speciesSize;i++)
    {   fitnessSum += genomes[i]->adjustedFitness;
        
    }
    averageAdjustedFitness = fitnessSum/speciesSize;
};



bool population::reproduce()
{
    

    std::vector<species *> newSpecieList;
    std::vector<genome *> offspring;

    //create new species list
    //sort genomes in each species by fitness
    //add most fit individual to new species list
    for(int i = 0; i<speciesList.size();i++)
    {   newSpecieList.push_back(new species());
        std::sort(speciesList[i]->genomes.begin(),speciesList[i]->genomes.end(),genomeFitnessOrder);
        newSpecieList[i]->genomes.push_back(speciesList[i]->genomes[0]);
        newSpecieList[i]->stagnant = speciesList[i]->stagnant;
    }
    
    for(int j = 0; j<speciesList.size();j++)
    {
        //cull species into a third of original size (after sorting - cuts least fit 2/3rds)
    speciesList[j]->genomes.resize(speciesList[j]->genomes.size()/3);
    
    //TODO change to be a percent based on a config rate
    int survivors = 1;
    int i = 0;
      
        
        //copy most fit genome to new species list.
    while(survivors > 0)
    {   addToSpecies(speciesList[j]->genomes[i],&newSpecieList);
        offspring.push_back(speciesList[j]->genomes[i]);
        i++;
        speciesList[j]->totalOffspring --;
        survivors--;
    }
    
    //if species is stagnant, create offspring from only most fit individual mutations
    if(speciesList[j]->stagnant >= 15)
    {   while(speciesList[j]->totalOffspring > 0)
        {
            genome * child;
            child = copyGenome(speciesList[j]->genomes[0]);
            child->ID =  genomeId;
            genomeId ++;
            mutate(child);
            addToSpecies(child,&newSpecieList);
            offspring.push_back(child);
            speciesList[j]->totalOffspring--;
        }
        speciesList[j]->stagnant = 0;
    }

    //else species is not stagnant
    else{
        //find number to mutate only (not crossover)
        int numMutateOnly = round(speciesList[j]->totalOffspring * .25);
        
        //mutate genomes
        while(numMutateOnly > 0)
        {   genome * child =copyGenome(speciesList[j]->genomes[randomInt(0,(int)speciesList[j]->genomes.size())]);
            child->ID =  genomeId;
            genomeId ++;
            mutate(child);
            addToSpecies(child,&newSpecieList);
            offspring.push_back(child);
            numMutateOnly --;
        }
        speciesList[j]->totalOffspring -= numMutateOnly;
        
        //breed
        while(speciesList[j]->totalOffspring > 0)
        {
            genome * child;
           
            if(speciesList[j]->genomes.size() > 1)
            {   //breed from species
                child = breedChild(speciesList[j]);
                child->ID =  genomeId;
                genomeId ++;
                mutate(child);
              
                
            }
            //else only one individual in species
            else
            {   //copy child if only
                child = copyGenome(speciesList[j]->genomes[0]);
                child->ID =  genomeId;
                 genomeId ++;
                 mutate(child);
            }
            
            addToSpecies(child,&newSpecieList);
            offspring.push_back(child);
            speciesList[j]->totalOffspring--;

        
        }
    }
    }
    this->speciesList = newSpecieList;

    return true;
    
};




//Copy genome
genome * population::copyGenome(genome * toCopy)
{   genome * g = new genome(toCopy->numInputs,toCopy->numOutputs);
    g->ID = genomeId;
    genomeId++;
    
    for(int j=0;j<toCopy->nodes.size();j++)
    {   g->nodes.insert(g->nodes.end(),toCopy->nodes[j]->copyNode());
                      
        g->nodes[j]->numIncoming = toCopy->nodes[j]->numIncoming;
        
    }
    
    g->fitness = toCopy->fitness;
    g->maxNode = toCopy->maxNode;
    g->connections = toCopy->connections;
    
    for(auto const& iter : toCopy->mutationRates)
    {   g->mutationRates.insert(std::map<std::string,double>::value_type(iter.first,iter.second));
        
    }
    return g;
    
};



//create a starting genome
//connects all inputs to outputs, mutates once
genome * population::startingGenome()
{
    genome * g = new genome(inputs,outputs);
    g->ID = genomeId;
    genomeId++;
    g->maxNode = inputs+outputs;
    
    for(int i = 0;i<g->maxNode;i++)
    {   g->nodes.push_back(new node());
    }
    
    
    //XOR ONLY
    //UPDATE FOR ANY OTHER USE, CONNECT ALL INPUTS TO ALL OUTPUTS
    //UPDATE NUM INCOMING FOR EACH
    connection * c1 = new connection(0,3);
    connection * c2 = new connection(1,3);
    
    g->nodes[0]->addOutgoing(c1);
    g->nodes[1]->addOutgoing(c2);
    
    g->nodes[3]->numIncoming = 2;
    
    mutate(g);
    return g;
}


//check if innovation exists in population already
int population::innovationExists(int start, int end)
{   std::pair<int,int> toFind = std::make_pair(start,end);
    
    
    bool found = false;
    int retVal = -1;
    
    //if none yet, add
    if(innovations.size() == 0)
    {   innovations.push_back(toFind);
        
    }
    
    //else, search for innovation
    for(int i = 0; i<innovations.size();i++)
    {   if(toFind.first == innovations[i].first && toFind.second == innovations[i].second)
        {   found = true;
            retVal = i;
        }
    }
    
    //if not found, add
    if(!found)
    {   innovations.push_back(toFind);
        innovation+=1;
        retVal = innovation;
    }
    
    return retVal;
}



//create initial population of starting genomes
//add all to species
//speciated due to first mutation
void population::initPop()
{
    //init species list
    for(int i = 0; i<popConstant; i++)
    {   genome * start = startingGenome();
        addToSpecies(start, &speciesList);
        
    }
    
};






//write population to file
void population::writePopToFile()
{   std::ofstream output ("gen" + std::to_string(   generation) + ".txt",std::fstream::in | std::fstream::out | std::fstream::trunc);
    //std::ofstream output ("gen.txt",std::fstream::in | std::fstream::out | std::fstream::trunc);
    if(output.is_open())
    {
        
        output << "Gen: "<<    generation << "\n";
        output << "Max Fit: "<<   maxFitness << "\n";
        output << "Species: "<<    speciesList.size() << "\n\n";
        
        for(int i = 0;i<   speciesList.size();i++)
        {   output << "Species: " + std::to_string(i) << "\n";
            output << "Top Fitness: " <<    speciesList[i]->topFitness << "\n";
            //   output << "Sum Adjusted Fitness: " <<    speciesList[i]->sumAdjustedFitness << "\n";
            output << "Staleness: " <<    speciesList[i]->staleness << "\n";
            output << "# Genomes: " <<    speciesList[i]->genomes.size() << "\n\n";
            for(int j = 0;j<   speciesList[i]->genomes.size();j++)
            {   genome g =*   speciesList[i]->genomes[j];
                output << "\tGenome: " + std::to_string(j) << "\n\t";
                output << "Global ID: "<< g.ID << "\n\t";
                output << "Fitness: "<< g.fitness << "\n\t";
                //  output << "Adjusted Fitness: "<< g.adjustedFitness << "\n\t";
                output << "# Nodes: " << g.maxNode << "\n\n\t";
                output << "Genes" << "\n";
                for(int k = 0;k<g.nodes.size();k++)
                {   for(int l=0;l<g.nodes[k]->outgoingConnections.size();l++)
                {    connection * c = g.nodes[k]->outgoingConnections[l];
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
            }
            output  << "\n";
            
            output << "-----------------------------------\n";
        }
        
        
        output.close();
    }
    else {
        std::cout << "Unable to open file";
    }
    
};


// mutate a genome
void population::mutate(genome * g)
{   //CONNECTION MUTATIONS
    for(int i = 0;i<g->nodes.size();i++)
    {
        for(int j = 0; j<g->nodes[i]->outgoingConnections.size();j++)
        {
            connection * c = g->nodes[i]->outgoingConnections[j];
            if(randomDouble(0,1) < g->mutationRates["weightMutate"])
            {
                if( randomDouble(0,1) < g->mutationRates["weightReplace"])
                {   //replace weight
                    c->connectionMutate(true);
                    
                }
                else
                {   //change weight
                    c->connectionMutate(false);
                }
            }
            
            if(randomDouble(0,1) < g->mutationRates["toggleConnection"])
            {   //change enabled to disabled
                c->enabled = !c->enabled;
            }
        }
    }
    

    
    //TOPOLOGY MUTATIONS
    if(randomDouble(0,1) < g->mutationRates["addNode"])
    {   addNode(g);
     
        
    }
    
    if(randomDouble(0,1) < g->mutationRates["addConnection"])
    {
        addConnection(g);
        
    }


};

void population::addNode(genome * g)
{     //ADD CHECK
        //if there are no connections to add a node between
        //    if(g->connections.size() == 0)
        //    {   return;
        //    }
        //
        int randNode = randomInt(0, (int)(g->nodes.size()-1));
        while(g->nodes[randNode]->outgoingConnections.size() == 0)
        {   randNode = randomInt(0, (int)(g->nodes.size()-1));
        }
        
        int randConnection = randomInt(0, (int)(g->nodes[randNode]->outgoingConnections.size()-1));
        connection * c = g->nodes[randNode]->outgoingConnections[randConnection];
        
        
        
        
        //if not enabled, do nothing
        if(!c->enabled)
        {   return;
        }
        
        //if enabled, set to false b/c we are going to split it
        c->enabled = false;
        
        
        //increase node counter
        g->maxNode += 1;
        
        //copy connection, connect first half
        connection * c2 = new connection();
        c2->endNode = g->maxNode-1;
        c2->startNode = c->startNode;
        c2->weight = c->weight;
        
        node * newNode = new node();
        g->nodes.push_back(newNode);
        
        //INNOVATION
        c2->innovation = innovationExists(c2->startNode,c2->endNode);
        g->nodes[c2->startNode]->addOutgoing(c2);
        g->nodes[c2->endNode]->numIncoming++;
        
        
        
        connection * c3 = new connection();
        c3->startNode = g->maxNode-1;
        c3->endNode = c->endNode;
        c3->weight = c->weight;;
        //INNOVATION
        c3->innovation = innovationExists(c3->startNode,c3->endNode);
        

        g->nodes[c3->startNode]->addOutgoing(c3);
        g->nodes[c3->endNode]->numIncoming++;
        g->connections += 2;
        
    
};


void population::addConnection(genome * g)
{
    //find random nodes to connect
    int n1;
    int n2;
    //if there are no connections yet
    if(g->connections == 0)
    {   n1 =  g->randomNode(true);
        n2 =  g->randomNode(false);
    }
    else
    {   n1 =  g->randomNode(false);
        n2 =  g->randomNode(false);
    }
    
    
    connection * c = new connection();
    
    //if both input nodes, return
    //no reason to connect
    if(n1 < g->numInputs && n2 < g->numInputs)
    {
        return;
    }
    
    //if both output nodes, return
    //TODO THIS CHECK IS INACCURATE
    if(n1 >= g->maxNode - g->numOutputs  && n2 >= g->maxNode -  g->numOutputs)
    {
        
        return;
    }
    
    //start is an output node
    if(n1  >= g->numInputs && n1 < g->numInputs+g->numOutputs)
    {
        return;
        
    }
    
    if(n1 == n2)
    {   return;
        
    }
    
    
    //if n2 is an input, swap
    if(n2 < g->numInputs)
    {   int temp = n1;
        n1 = n2;
        n2 = temp;
    }
    
    c->startNode = n1;
    c->endNode = n2;
    
    //if connection already exists return
    if(g->nodes[n1]->containsConnection(n2))
    {   return;
    }
    
    //if loop will form, return
    if(g->nodes[n2]->containsConnection(n1))
    {   return;
    }
    

    //if will create a cycle
    if(g->depthFirstSearch(n2,n1))
    {   g->clearSearch();
        return;
    }
    
    g->clearSearch();

    //passed all tests
    //assign innovation number and rand weight, add to network
    c->innovation = innovationExists(c->startNode,c->endNode);
    c->weight = randomDouble(0,1);
    g->nodes[c->startNode]->outgoingConnections.push_back(c);
    g->nodes[c->endNode]->numIncoming++;
    g->connections ++;
 
    
    
};


void population::addToSpecies(genome * g, std::vector<species*> * specieList)
{   bool found = false;
    

    for(int i =0;i<specieList->size();i++)
    {
        species * s = specieList->at(i);
        
        if(s->genomes.size() > 0)
        {   //if not found, and is same species, add
            if(!found &&  isSameSpecies(g,s->genomes[0]))
            {   found = true;
                s->addToSpecies(g);
            }
        }
    }
    
    //not found, create new species
    if(!found)
    {
        species * s = new species();
        s->genomes.push_back(g);
        specieList->push_back(s);
    }
    currentPop++;
};



bool population::isSameSpecies(genome * g1, genome *g2)
{   double totalMatchingGenes = 0;
    double totalWeightDifferences = 0.0;
    double avgWeightDifference;
    double delta;
    double numDisjointGenes = 0;
    double numExcessGenes = 0;
    
    
    std::vector<connection> g1Con;
    std::vector<connection> g2Con;
    
    
    //create list of connections
    for(int x = 0; x<g1->nodes.size();x++)
    {   for(int m = 0; m<g1->nodes[x]->outgoingConnections.size();m++)
    {   g1Con.push_back(*g1->nodes[x]->outgoingConnections[m]);
    }
    }
    
    
    //create list of connections
    for(int x = 0; x<g2->nodes.size();x++)
    {   for(int m = 0; m<g2->nodes[x]->outgoingConnections.size();m++)
    {   g2Con.push_back(*g2->nodes[x]->outgoingConnections[m]);
    }
    }
    
    
    //sort connections
    std::sort(g1Con.begin(),g1Con.end(),connectionOrder);
    std::sort(g2Con.begin(),g2Con.end(),connectionOrder);
    
    //find limits of innovations (for disjoint v. excess)
    int g1InvLimit = g1Con[g1Con.size()-1].innovation;
    int g2InvLimit = g2Con[g2Con.size()-1].innovation;
    
    
    int i = 0;
    int j = 0 ;
    while(i < g1Con.size() && j < g2Con.size())
    {   //match
        if(g1Con[i].innovation == g2Con[j].innovation)
        {   totalMatchingGenes++;
            totalWeightDifferences += std::abs(g1Con[i].weight - g2Con[j].weight);
            i++;
            j++;
        }
    else if(g1Con[i].innovation < g2Con[j].innovation)
    {   //disjoint
        if(g1Con[i].innovation < g2InvLimit)
        {   numDisjointGenes++;
        }
        i++;
        
    }
        
    else if(g1Con[i].innovation > g2Con[j].innovation)
    {   //disjoint
        if(g2Con[j].innovation < g1InvLimit)
        {   numDisjointGenes++;
        }
        j++;
    }
    }
    
    while(i < g1Con.size())
    {   //disjoint
        if(g1Con[i].innovation < g2InvLimit)
        {   numDisjointGenes++;
        }
        //excess
        else{
            numExcessGenes++;
        }
        i++;
    }
    while(j < g2Con.size())
    {   //disjoint
        if(g2Con[j].innovation < g1InvLimit)
        {   numDisjointGenes++;
        }
        //excess
        else{
        numExcessGenes++;
        }
        j++;
    }
    
    
    avgWeightDifference = totalWeightDifferences/totalMatchingGenes;
    delta = ((numDisjointGenes*disjointConstant))+ ((numExcessGenes*excessConstant)) + avgWeightDifference*weightConstant;
    return delta < deltaThreshold;

};



void population::newGeneration()
{
    
    
    //-1 is flag value b/c XOR fitness min is -1
    double bestFitness = -1;
    double speciesBest = -1;
    species * sWithBestIndividual;
    genome * topGenome;
    double popSumFitness = 0;
    double popAvgFitness = 0;
    double totalSpeciesAvgFitness = 0;
    
    //set adjusted fitness of each species/individual
    setAdjustedFitness();
    
    
    
    //get best genome, get populations average fitness
    //SAVE SPECIES with best individual - FLAG within species
    for(int i = 0;i< speciesList.size();i++)
    {   speciesBest = -1;
        //Set avg fitness of species
        //get total of these averages
        speciesList[i]->setAverageFitness();
        speciesList[i]->setAverageAdjustedFitness();
        
    
        totalSpeciesAvgFitness +=  speciesList[i]->averageAdjustedFitness;
     
        
        for(int j = 0; j<speciesList[i]->genomes.size();j++)
        {   genome * g = speciesList[i]->genomes[j];
            popSumFitness += g->fitness;
            
            if(g->fitness > bestFitness)
            {   bestFitness = g->fitness;
                topGenome = g;
                sWithBestIndividual =  speciesList[i];
                 speciesList[i]->hasBestIndividual = true;
            }
            if(g->fitness >speciesBest)
            {   speciesBest = g->fitness;
                
            }
        }
         speciesList[i]->checkStagnant(speciesBest);
    }
    

    
    
    popAvgFitness = popSumFitness/ popConstant;
    topFitness = bestFitness;
    avgFitness = popAvgFitness;
    this->writePopToFile();
    
   //get num offspring for each species
    //Offspring = (AverageSpeciesFitness / Total_of_AverageSpeciesFitnesss)* PopulationSize

    std::vector<species * > newSpeciesList;
    std::vector<species*>::iterator iter;
    for (iter =  speciesList.begin(); iter !=  speciesList.end();)
    {
        (*iter)->totalOffspring = ((*iter)->averageAdjustedFitness/totalSpeciesAvgFitness)* popConstant;
     
       
        if((*iter)->totalOffspring > 1 )//&& (*iter)->stagnant < 20)
        {   newSpeciesList.push_back(*iter);
            ++iter;
        }
        else
        {
            ++iter;
        }
    }

    
    
     speciesList = newSpeciesList;

    //create new pop by in reproduce function
    reproduce();
    
     generation++;


};




genome * population::breedChild(species * s)
{    genome * child;
    
    //select random genome in species
    //breed
    int int1 = randomInt(0, ((int)s->genomes.size()/2));
    int int2 = randomInt(0, ((int)s->genomes.size()/2));

    genome * gA = s->genomes[int1];
    genome * gB = s->genomes[int2];
  
    child = crossoverGenome(gA,gB);
   
    return child;
    
};

void population::setAdjustedFitness()
{
    
    //outer loop for each genome
    for(int k = 0;k<speciesList.size();k++)
    {   for(int z = 0;z<speciesList[k]->genomes.size();z++)
        {
            speciesList[k]->genomes[z]->adjustedFitness = speciesList[k]->genomes[z]->fitness/(speciesList[k]->genomes.size());
       
        }
    }
    
}



genome * population::crossoverGenome(genome * g1, genome *g2)
{   if(g2->fitness > g1->fitness)
    {   genome* temp = g1;
        g1 = g2;
        g2 = temp;
    }
    
    bool thresholdFitness = false;
    double lessFitError = g1->fitness - .05;
    double greaterFitError = g1->fitness + .05;
    
    std::vector<connection*> matchingGenes;
    std::vector<connection*> g1DisjointExcess;
    std::vector<connection*> g2DisjointExcess;
    std::vector<connection*> g1Con;
    std::vector<connection*> g2Con;
    
    
    //lines from here to 915 very similar to speciation
    //decide which genes are matching, disjoint, excess.
    for(int x = 0; x<g1->nodes.size();x++)
    {   for(int m = 0; m<g1->nodes[x]->outgoingConnections.size();m++)
        {   g1Con.push_back(g1->nodes[x]->outgoingConnections[m]);
        }
    }

    for(int x = 0; x<g2->nodes.size();x++)
    {   for(int m = 0; m<g2->nodes[x]->outgoingConnections.size();m++)
        {   g2Con.push_back(g2->nodes[x]->outgoingConnections[m]);
        }
    }
    

    
    std::sort(g1Con.begin(),g1Con.end(),connectionOrderPointer);
    std::sort(g2Con.begin(),g2Con.end(),connectionOrderPointer);
    
    int g1InvLimit = g1Con[g1Con.size()-1]->innovation;
    int g2InvLimit = g2Con[g2Con.size()-1]->innovation;
    
    int i = 0;
    int j = 0 ;
    while(i < g1Con.size() && j < g2Con.size())
    {   if(g1Con[i]->innovation == g2Con[j]->innovation)
        {   matchingGenes.push_back(g1Con[i]);
            i++;
            j++;
        }
        else if(g1Con[i]->innovation < g2Con[j]->innovation)
        {   if(g1Con[i]->innovation < g2InvLimit)
            {   g1DisjointExcess.push_back(g1Con[i]);
            }
            i++;
            
        }
            
        else if(g1Con[i]->innovation > g2Con[j]->innovation)
        {   if(g2Con[j]->innovation < g1InvLimit)
            {   g2DisjointExcess.push_back(g2Con[j]);
            }
            j++;
        }
    }

    
    
    while(i < g1Con.size())
    {   g1DisjointExcess.push_back(g1Con[i]);
        i++;
    }
    while(j < g2Con.size())
    {   g2DisjointExcess.push_back(g2Con[j]);
        j++;
    }

    
    
    
    
    //CREATE CHILD
    genome * child = new genome(g1->numInputs,g1->numOutputs);
    child->maxNode = std::max(g1->maxNode,g2->maxNode);
    
    
    //if within threshold (+-.25 of g1 fitness)
    if(g2->fitness > lessFitError && g2->fitness < greaterFitError)
    {
        thresholdFitness = true;
    }
    
    for(int h = 0; h<child->maxNode;h++)
    {   child->nodes.push_back(new node());
        
    }
    
    
    int numGenes = 0;
    
    
    //add genes (connections) to child.
    while(numGenes == 0)
    {
        for(int k=0; k<matchingGenes.size();k++)
        {   //if(randomDouble(0,1) < .8)
            {   connection *  c = matchingGenes[k];
                connection * toAdd = new connection();
                toAdd->startNode = c->startNode;
                toAdd->endNode = c->endNode;
                toAdd->weight = c->weight;
                toAdd->innovation = c->innovation;
                toAdd->enabled = c->enabled;
             
                if(child->depthFirstSearch(toAdd->endNode,toAdd->startNode))
                {    child->clearSearch();
                     break;
                    
                }
                 child->clearSearch();
                if(child->nodes.size() == toAdd->startNode)
                {   child->nodes.push_back(new node());
                    
                }
                child->nodes[toAdd->startNode]->outgoingConnections.push_back(toAdd);
                numGenes ++;
            }
        }
        
        
        if(thresholdFitness)
        {
            for(int g = 0; g<g1DisjointExcess.size();g++)
            {   //if(randomDouble(0,1) < .8)
                {
                    connection* c = g1DisjointExcess[g];
                    connection * toAdd = new connection();
                    toAdd->startNode = c->startNode;
                    toAdd->endNode = c->endNode;
                    toAdd->weight = c->weight;
                    toAdd->innovation = c->innovation;
                    toAdd->enabled = c->enabled;
                    if(child->depthFirstSearch(toAdd->endNode,toAdd->startNode))
                    {   child->clearSearch();
                        break;
                        
                    }
                    child->clearSearch();
                    child->nodes[toAdd->startNode]->outgoingConnections.push_back(toAdd);
                     numGenes ++;
                }
                
            }
            for(int h = 0; h<g2DisjointExcess.size();h++)
            {   if(randomDouble(0,1) < .5)
                {
                    connection* c = g2DisjointExcess[h];
                    connection * toAdd = new connection();
                    toAdd->startNode = c->startNode;
                    toAdd->endNode = c->endNode;
                    toAdd->weight = c->weight;
                    toAdd->innovation = c->innovation;
                    toAdd->enabled = c->enabled;
                    if(child->depthFirstSearch(toAdd->endNode,toAdd->startNode))
                    {   child->clearSearch();
                        break;
                        
                    }
                     child->clearSearch();
                    child->nodes[toAdd->startNode]->outgoingConnections.push_back(toAdd);
                     numGenes ++;
                }
            }
            
        }
        
        else
        {   for(int f = 0;f<g1DisjointExcess.size();f++)
            {   connection * c = g1DisjointExcess[f];
                connection * toAdd = new connection();
                toAdd->startNode = c->startNode;
                toAdd->endNode = c->endNode;
                toAdd->weight = c->weight;
                toAdd->innovation = c->innovation;
                toAdd->enabled = c->enabled;
                if(child->depthFirstSearch(toAdd->endNode,toAdd->startNode))
                {   child->clearSearch();
                    break;
                }
                 child->clearSearch();
                child->nodes[toAdd->startNode]->outgoingConnections.push_back(toAdd);
                 numGenes ++;
            }
        }
    }
    
    
    for(int i = 0; i<child->nodes.size();i++)
    {   for(int j = 0;j<child->nodes[i]->outgoingConnections.size();j++)
        {   child->nodes[child->nodes[i]->outgoingConnections[j]->endNode]->numIncoming++;
        
        }
        
    }

    
    std::map<std::string,double>::iterator iter3;
  
    for (iter3 = g1->mutationRates.begin(); iter3 != g1->mutationRates.end();++iter3)
    {   child->mutationRates[iter3->first] = iter3->second;
        
    }
    
    return child;

};
