// ========================================== max. 120 symbols in one line ============================================
/**
 * @par Project
 * Game_of_Life
 *
 * @file    python_wrapper.cpp
 * @author  Andre Alexander Pieper
 * @version 1.0
 * @date    2020-06-16
 *
 * @brief   Wrapper to call Python code.
 *
 * This program part is responsible for calling the matplotlib code to plot a graph.
 */
// --------------------------------------------------------------------------------------------------------------------

#include <Python.h>

#include "python_wrapper.hpp"


namespace GameOfLife
{

PythonWrapper::PythonWrapper(int i)
{
    this->i_ = i;
    this->Init();
}

PythonWrapper::~PythonWrapper()
{    
    Py_Finalize();
}

void PythonWrapper::Init()
{
    // Initilize Python wrapper to call matplotlib
    Py_SetProgramName(L"GameOfLife");  /* optional but recommended */
    Py_Initialize();
    PyRun_SimpleString("print ('Initialization of the Python wrapper was successful.')\n");
}

}
