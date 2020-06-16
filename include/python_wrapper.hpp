// ========================================== max. 120 symbols in one line ============================================
/**
 * @par Project
 * Game_of_Life
 *
 * @file    python_wrapper.hpp
 * @author  Andre Alexander Pieper
 * @version 1.0
 * @date    2020-06-16
 *
 * @brief   Header file to read the configuration file.
 *
 * This program part is responsible for calling the matplotlib code to plot a graph.
 */
// --------------------------------------------------------------------------------------------------------------------

#ifndef HEADER_PYTHON_WRAPPER_AP_16062020
#define HEADER_PYTHON_WRAPPER_AP_16062020

#include <string>
#include <vector>


namespace GameOfLife
{
/** @class ReadConfig
 *  @brief Header file to read the configuration file.
 *
 *  This program part is responsible for reading the configuration file and returns
 *  the requested information.
 */
class PythonWrapper
{
public:
	PythonWrapper(int i);
	virtual ~PythonWrapper();				// Virtual Destructor.

private:
       void Init();
       int i_;
};
}

#endif // HEADER_PYTHON_WRAPPER_AP_16062020
