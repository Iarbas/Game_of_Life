// ========================================== max. 120 symbols in one line ============================================
/**
 * @par Project
 * Game_of_Life
 *
 * @file    visual2d.cpp
 * @author  Andre Alexander Pieper
 * @version 1.0
 * @date    2020-05-19
 *
 * @brief   Visualization of the habitat and individuals.
 *
 * This program part is responsible for the visualization of the habitat and individuals. This includes all
 * information regarding the environment and test subject.
 */
// --------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include <stdlib.h> 
#include "visual2d.hpp"

namespace GameOfLife
{

/** @fn Visual2D::Visual2D()
 *  @brief Constructor of the class Visual2D.
 *
 *  Initialization of the window object.
 */
Visual2D::Visual2D(int number_of_elements, std::string window_form)
{
    // Get screen resolution.
    sf::VideoMode desktop = sf::VideoMode().getDesktopMode();
    
    fprintf(stdout,"Create window as a %s.\n", window_form.c_str());
    
    // Declare variables
    int element_size = 3;   // 3 is minimum.
    
    // We use the muliplicatior 0.95, because of the Panel/Dock from the Ubuntu screen.
    Visual2D::screen_height = 0.95f * desktop.height;
    Visual2D::screen_width = 0.95f * desktop.width;
	
    // Adapt the window to the request form and number of elements.
    if (window_form.compare("square") == 0)
    {
        // This form uses equilateral side length.
        
        // Search for the smallest side.
        if (Visual2D::screen_height <= Visual2D::screen_width)
        {
            // Set a limit to the quantity of elements.
            number_of_elements = max_num_of_elem(number_of_elements, Visual2D::screen_height, Visual2D::screen_height);
            
            // Get the size of each element with respect to the screen size
            element_size = get_element_size(number_of_elements, Visual2D::screen_height, Visual2D::screen_height);
        }
        else
        {
            // Set a limit to the quantity of elements.
            number_of_elements = max_num_of_elem(number_of_elements, Visual2D::screen_width, Visual2D::screen_width);
            
            // Get the size of each element with respect to the screen size
            element_size = get_element_size(number_of_elements, Visual2D::screen_width, Visual2D::screen_width);
        }
    }
    else if (window_form.compare("rectangle") == 0)
    {
        // This option keeps the ratio of the screen resolution.
        
        // Set a limit to the quantity of elements.
        number_of_elements = max_num_of_elem(number_of_elements, Visual2D::screen_height, Visual2D::screen_width);
        
        // Get the size of each element with respect to the screen size
        element_size = get_element_size(number_of_elements, Visual2D::screen_height, Visual2D::screen_width);        
    }
    else
    {
        fprintf(stdout,"No form with the name %s exists. You should exit the program.\n", window_form.c_str());
    }


    // Visual2D::window = new sf::RenderWindow(desktop, "Game_of_Life");
    // Visual2D::dead_box = new sf::Texture;
    // Visual2D::live_box = new sf::Texture;
	
    // Assign colors to the textures.
    // if (!dead_box->create(30,30))

    // this->Init();
}

/** @fn Visual2D::~Visual2D()
 *  @brief Destructor of the class Visual2D.
 *
 *  This functions deletes all created objects.
 */
Visual2D::~Visual2D()
{
    /*delete Visual2D::window;
    delete Visual2D::dead_box;
    delete Visual2D::live_box;
*/
}

/** @fn Visual2D::Init()
 *  @brief Initial function of the class Visual2D.
 *
 *  This functions draws the first window.
 */
void Visual2D::Init()
{
    // Create the initial state.
    

    // Visual2D::window->clear();
    // Visual2D::window->draw();
    // Visual2D::window->display();
}

/** @fn Visual2D::WindowUpdater()
 *  @brief Re-draws the window with new input.
 *
 *  This functions draws continuously windows with new input from other Game_of_Life parts.
 */
void Visual2D::WindowUpdater()
{
	/*
	if (Visual2D::window->isOpen())
	{
		if (Visual2D::window->pollEvent(Visual2D::event))
		{
			if (Visual2D::event.type == sf::Event::Closed) Visual2D::window->close();
		}
		else
		{
			Visual2D::window->clear();
			Visual2D::window->draw(*Visual2D::sprite_street);
			Visual2D::window->display();
		}
	}
	*/
}

/** @fn Visual2D::max_num_of_elem(int num_of_elem, int a, int b)
 *  @brief Function to check, if the size of the screen can display the requested number of elements.
 *
 *  This functions checks, if the requested number of elements fits in the screen. To see an element,
 *  is would be benificial to have one element at the size of 9 pixel at least.
 */
inline int Visual2D::max_num_of_elem (int num_of_elem, int a, int b)
{
    // Based on the requirement that each element needs to have 3 x 3 pixel, the max number of elements is 
    // also limited.
    if (num_of_elem > (a * b) / 9)
    {
        num_of_elem = (a * b) / 9;
        
        fprintf(stdout,"Too many elements! The number was restricted to %i.\n", num_of_elem);
    }
    
    return num_of_elem;
}

/** @fn Visual2D::WindowUpdater()
 *  @brief Re-draws the window with new input.
 *
 *  This functions draws continuously windows with new input from other Game_of_Life parts.
 */
inline int Visual2D::get_element_size (int num_of_elem, int a, int b)
{
    fprintf(stdout,"\n\n---------- Calculation of the Grid Size ----------\n");
    
    // Variable Definition.
    int element_size = 0;
    int row_elements = 0; 
    int column_element = 0;
    int total_elements = 0;
    
    // Re-ajust the window size in case of rounding issues in the calculation step.
    int a_adjusted = 0.95f * a;
    int b_adjusted = 0.95f * b;
    
    // Calculate ratio of the screen resolution.
    int ratio = b_adjusted / a_adjusted;
    
    // Distribute the elements according to the ratio of the screen.
    int x = sqrt(num_of_elem / ratio); // x * ratio = y | x * y = num_of_elem
    int y = num_of_elem / x;
    total_elements = y * x;
    
    // Calculate the element size with respect to the screen resolution.
    int element_size_a = a_adjusted / x;
    int element_size_b = b_adjusted / y;
    
    // Check, if the the element sizes are similar.
    float diff = abs((float)(a_adjusted / x) - (float)(b_adjusted / y)); 
    
    if (diff > (0.1 * (float)(a_adjusted / x)) || diff > (0.1 * (float)(b_adjusted / y)))
    {
        fprintf(stdout,"\n\n+++++++++++ Something went wrong +++++++++++\n");
        fprintf(stdout,"Difference of the element size in a and b: %.3f\n", diff);
        fprintf(stdout,"Element size a: %i \t and b: %i\n", element_size_a, element_size_b);        
        fprintf(stdout,"Please send this to the developer with the corresponding config file.\n\n");        
    }
    else
    {
        if (diff != 0)
        {
            if (element_size_a < element_size_b)
            {
                element_size = element_size_a + 0.5 * diff;
            }
            else
            {
                element_size = element_size_b + 0.5 * diff;
            } 
        }
        else
        {
            element_size = element_size_a;
        }
        
        row_elements = y;
        column_element = x;
    }
        
    // Control of the element size to fulfill the requirements.
    // TODO: Tolerances in the config file.
    if (element_size < 3)
    {
        element_size = 3;
    
        fprintf(stdout,"Something totally went wrong. Set the element size to the minimum of 9 pixel.\n");
    }
    
    fprintf(stdout,"The edge length of one element was calculated to %i pixel on each side.\n", element_size);
    fprintf(stdout,"There are %i elements in the row and %i in the column. ", row_elements, column_element); 
            
    fprintf(stdout,"In total there are %i elements in the simulation from the requested %i.\n", 
                total_elements, num_of_elem);
                
    // Calculate the resulting window size
    Visual2D::grid_height = column_element * element_size;
    Visual2D::grid_width = row_elements * element_size;
    
    fprintf(stdout,"The height of the grid is %i and the width is %i Pixel.\n", Visual2D::grid_height, 
                Visual2D::grid_width);
                
    fprintf(stdout,"The window size is %i in height and %i in width.\n", a, b);
                
    fprintf(stdout,"---------- End of the Grid Size Calculation ----------\n\n");
        
    return element_size;
}

}
