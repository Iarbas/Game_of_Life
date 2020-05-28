// ========================================== max. 120 symbols in one line ============================================
/**
 * @par Project
 * Game_of_Life
 *
 * @file    lifeCycle.hpp
 * @author  Andre Alexander Pieper
 * @version 1.0
 * @date    2020-05-28
 *
 * @brief   Class to manage the biotope.
 *
 * This class encapsulates a structure of elements or rather life forms. The individuals live and die according to the 
 * set rules.
 */
// --------------------------------------------------------------------------------------------------------------------

#ifndef HEADER_LIFECYCLE_HPP_AP_28052020
#define HEADER_LIFECYCLE_HPP_AP_28052020

#include <vector>
#include <string>


namespace GameOfLife 
{
    struct struct_individuals
    {
        bool change, alive;
    };

    enum order_mode
    {
        random, 
        test_pattern
    };

/** @class LifeCycle
 *  @brief Class to manage the biotope.
 *
 *  This class encapsulates a structure of elements or rather life forms. The individuals live and die according to the 
 * set rules.
 */
class LifeCycle
{
public:
	LifeCycle(int rows, int columns, int number_of_elements, std::string mode, std::string pattern);
	virtual ~LifeCycle();				// Virtual Destructor.
	
	void LifeRules();
	
	std::vector<struct_individuals> individuals;
	order_mode mode_enum;
private:
        void Init(GameOfLife::order_mode mode);
        void RandomPatternGenerator();
        void SinglePatternGenerator();
        
        int _elements, _rows, _columns;
};

}


#endif // HEADER_LIFECYCLE_HPP_AP_28052020
