//
//  random.cpp
//  Capstone2
//
//  Created by School on 4/7/16.
//  Copyright © 2016 Seraphina Orsini. All rights reserved.
//

#include "random.hpp"

//seed random gen
void seedRand()
{   time_t timev;
    srand((unsigned int) time(&timev));
}

//return INCLUSIVE rand int min to max
int randomInt(int min, int max)
{   return (rand()+min) % (max+1);
}

//return INCLUSIVE rand double min to max
double randomDouble(double min,double max)
{   double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}
