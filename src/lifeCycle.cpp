// ========================================== max. 120 symbols in one line ============================================
/**
 * @par Project
 * Game_of_Life
 *
 * @file    lifeCycle.cpp
 * @author  Andre Alexander Pieper
 * @version 1.0
 * @date    2020-05-28
 *
 * @brief   Manager of the biotope and individuals.
 *
 * This program part is responsible for the life and dead cycles of each individual in the biotope. It contains the 
 * struct of the states of the individuals and the rules for live and die.
 */
// --------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <unordered_map>
#include "lifeCycle.hpp"
#include <stdlib.h>
#include <time.h>


namespace GameOfLife
{
/** @fn Visual2D::LifeCycle()
 *  @brief Constructor of the class LifeCycle.
 *
 */
LifeCycle::LifeCycle(int rows, int columns, int number_of_elements, std::string mode, std::string pattern)
{
    // Initialize variables
    this->_rows = rows;
    this->_columns= columns;
    this->_elements = number_of_elements;
    
    mode_enum = order_mode::random;
    LifeCycle::individuals.resize(number_of_elements);
    
    // initialize random seed
    seed = time(NULL);
    
    static std::unordered_map<std::string, order_mode> const table = 
        { {"random", order_mode::random}, 
        {"test_pattern",order_mode::test_pattern} };
        
    auto it = table.find(mode);
    
    if (it != table.end()) 
    {
        fprintf(stdout,"Population of the biotope will use the mode: %s.\n", mode.c_str());
        mode_enum = it->second;
    } 
    else 
    {  
        fprintf(stdout,"Cannot find the mode: %s. The program will continue with random pattern\n", mode.c_str());
    }
    
    this->Init(mode_enum);
}

/** @fn LifeCycle::~LifeCycle()
 *  @brief Destructor of the class LifeCycle.
 *
 *  This functions deletes all created objects.
 */
LifeCycle::~LifeCycle()
{
}

void LifeCycle::Init(GameOfLife::order_mode mode)
{
    switch(mode)
    {
        case order_mode::random :
            this->RandomPatternGenerator();
            break;
        case order_mode::test_pattern :
            this->SinglePatternGenerator();
            break;
        default:
            break;
    }
}

void LifeCycle::RandomPatternGenerator()
{
    // initialize random seed
    srand (seed);
    
    for (int i = 0; i < LifeCycle::individuals.size(); i++)
    {
        // percentage of living individuals.
        // TODO: In config file
        int live_rate = 30;
    
        // Decision of life and death. If the random number is bigger than a certain number, the individual will live.
        // Otherwise, it will be dead.
        if (rand() % 101 < live_rate)
        {
            if(!LifeCycle::individuals.at(i).alive)
            {
                LifeCycle::individuals.at(i).change = true;
                LifeCycle::individuals.at(i).alive = true;
            }
        }
        else
        {
            if(LifeCycle::individuals.at(i).alive)
            {
                LifeCycle::individuals.at(i).change = true;
                LifeCycle::individuals.at(i).alive = false;
            }
        }   
    }
    
    seed = rand();
}

void LifeCycle::SinglePatternGenerator()
{

}

void LifeCycle::LifeRules()
{
    int one, two, three, four, five, six, seven, eight;
    
    int living_neighbours;
    
    // Make a copy of the structure.
    std::vector<struct_individuals> copy_of_individuals = LifeCycle::individuals;

    // Iterate through all the individuals.
    for (int i = 0; i < LifeCycle::individuals.size(); i++)
    {
        living_neighbours = 0;
    
        // Check all 8 neighbours.
        // Here the control pattern:
        //
        //   1   2   3
        //   4   X   5
        //   6   7   8
        //
        // ---- 1 ----
        // If the individual is not located at the upper and left border, there is a neighbour.
        if (i > (_columns - 1) && i % _columns != 0)
        {
            one = i - _rows - 1;
                    
            if (LifeCycle::individuals.at(one).alive)
            {
                living_neighbours++;
            }
        }
        
        // ---- 2 ----
        // If the individual is not located at the upper border, there is a neighbour.
        if (i > (_columns - 1))
        {
            two = i - _rows;
                    
            if (LifeCycle::individuals.at(two).alive)
            {
                living_neighbours++;
            }
        }
        
        // ---- 3 ----
        // If the individual is not located at the upper and right border, there is a neighbour.
        if (i > (_columns - 1) && (i + 1) % _columns != 0)
        {
            three = i - _rows + 1;
                    
            if (LifeCycle::individuals.at(three).alive)
            {
                living_neighbours++;
            }
        }
        
        // ---- 4 ----
        // If the individual is not located at the left border, there is a neighbour.
        if (i > 0 && i % _columns != 0)
        {
            four = i - 1;
                        
            if (LifeCycle::individuals.at(four).alive)
            {
                living_neighbours++;
            }
        }
        
        // ---- 5 ----
        // If the individual is not located at the right border, there is a neighbour.
        if ((i + 1) % _columns != 0)
        {
            five = i + 1;
                        
            if (LifeCycle::individuals.at(five).alive)
            {
                living_neighbours++;
            }
        }
        
        // ---- 6 ----
        // If the individual is not located at the lower and left border, there is a neighbour.
        if (i < (_columns * (_rows - 1)) && i % _columns != 0)
        {
            six = i + _rows - 1;
                        
            if (LifeCycle::individuals.at(six).alive)
            {
                living_neighbours++;
            }
        }
        
        // ---- 7 ----
        // If the individual is not located at the lower border, there is a neighbour.
        if (i < (_columns * (_rows - 1)))
        {
            seven = i + _rows;
                        
            if (LifeCycle::individuals.at(seven).alive)
            {
                living_neighbours++;
            }
        }
        
        // ---- 8 ----
        // If the individual is not located at the lower and right border, there is a neighbour.
        if (i < (_columns * (_rows - 1)) && (i + 1) % _columns != 0)
        {
            eight = i + _rows + 1;
                        
            if (LifeCycle::individuals.at(eight).alive)
            {
                living_neighbours++;
            }
        }
        
        // Here come the rules.
    
        // If a living cell has 2 or 3 living neighbours, the cell continues to live.
        if (living_neighbours == 2 || living_neighbours == 3)
        {
            // If a dead individual has three living neighbours, the individual gets resurrected.
            if (!LifeCycle::individuals.at(i).alive && living_neighbours == 3)
            {
                copy_of_individuals.at(i).change = true;
                copy_of_individuals.at(i).alive = true;
            }
        }
        else if (LifeCycle::individuals.at(i).alive)
        {
            copy_of_individuals.at(i).change = true;
            copy_of_individuals.at(i).alive = false;
        }
    }
    
    // Copy back.
    LifeCycle::individuals = copy_of_individuals;
}


}
