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
	
    // Adapt the window to the request form and number of elements.
    if (window_form.compare("square") == 0)
    {
        // This form uses equilateral side length.
        
        // Search for the smallest side.
        if (desktop.height <= desktop.width)
        {
            // Set a limit to the quantity of elements.
            number_of_elements = max_num_of_elem(number_of_elements, desktop.height, desktop.height);
            
            // Get the size of each element with respect to the screen size
            element_size = get_element_size(number_of_elements, desktop.height, desktop.height);
        }
        else
        {
            // Set a limit to the quantity of elements.
            number_of_elements = max_num_of_elem(number_of_elements, desktop.width, desktop.width);
            
            // Get the size of each element with respect to the screen size
            element_size = get_element_size(number_of_elements, desktop.width, desktop.width);
        }
    }
    else if (window_form.compare("rectangle") == 0)
    {
        // This option keeps the ratio of the screen resolution.
        
        // Set a limit to the quantity of elements.
        number_of_elements = max_num_of_elem(number_of_elements, desktop.height, desktop.width);
        
        // Get the size of each element with respect to the screen size
        element_size = get_element_size(number_of_elements, desktop.height, desktop.width);        
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

inline int Visual2D::max_num_of_elem (int num_of_elem, int a, int b)
{
    // Based on the requirement that each element needs to have 3 x 3 pixel, the max number of elements is 
    // also limited.
    if (num_of_elem > (0.95f * a * 0.95f * b) / 9)
    {
        num_of_elem = (0.95f * a * 0.95f * b) / 9;
        
        fprintf(stdout,"Too many elements! The number was restricted to %i.\n", num_of_elem);
    }
    
    return num_of_elem;
}

inline int Visual2D::get_element_size (int num_of_elem, int a, int b)
{
    int element_size, big_side, big_side_original;
    
    int help4_row_element, help5_column_element;
    
    int help6_total_elements; 
    
    float help1_big_element_size, help2_ratio, help3_small_element_size;
    
    int best_element_size, best_help4_row_element, best_help5_column_element;
    int best_help6_total_elements = 0;

    // Get the size of each element by the size of the window. We use the muliplicatior 0.95,
    // because of the Panel/Dock from the Ubuntu screen.
    a = a * 0.95f;
    b = b * 0.95f;
    
    if (a >= b)
    {
        big_side = a;
    }
    else
    {
        big_side = b;
    }
    
    big_side_original = big_side;
    
    // Create a loop which will adjust the size of the window with respect to the required 
    // number of elements (less than 10% of tolerance). We run the loop with respect to 10%
    // of the biggest side.
    // TODO: Tolerances in the config file.
    for (int i = 0; i <= (0.1 * big_side_original); i++ ) 
    {
        if (a == b)
        {
            a--;
            b--;
            big_side--;        
        }
        else if (a >= b)
        {
            a--;
            big_side--;
        }
        else
        {
            b--;
            big_side--;        
        }

        // Special algorithm for distributing multiple boxes with fixed size inside bigger box.
        help1_big_element_size = (float) (big_side) / sqrt( num_of_elem);
    
        help2_ratio = (a * b) / (big_side * big_side);
    
        help3_small_element_size = help1_big_element_size * help2_ratio;
    
        element_size = help1_big_element_size * (1 - help2_ratio) +  help3_small_element_size * help2_ratio;
        
        help4_row_element = (int) (a / element_size);
        help5_column_element = (int) (b / element_size);
        
        help6_total_elements = help4_row_element * help5_column_element;
        
        if (abs(help6_total_elements - num_of_elem) < abs(best_help6_total_elements - num_of_elem))
        {
            best_element_size = element_size;
            best_help4_row_element = help4_row_element;
            best_help5_column_element = help5_column_element;
            best_help6_total_elements = help6_total_elements;        
        }
        
        fprintf(stdout,"%i \t %i \t %i\n", big_side, help6_total_elements, best_help6_total_elements);
    
        // If the number of elements fit inside the tolerance of the requested number, the loop will stop.
        // TODO: Tolerances in the config file.
        if (help6_total_elements < (1.1 * num_of_elem) && help6_total_elements > (0.9 * num_of_elem))
        {
            break;
        }
    }
        
    // Control of the 
    // TODO: Tolerances in the config file.
    if (best_element_size < 3)
    {
        best_element_size = 3;
    
        fprintf(stdout,"Something totally went wrong. Set the element size to the minimum of 9 pixel.\n");
    }
    
    fprintf(stdout,"The edge length of one element was calculated to %i pixel on each side.\n", best_element_size);
    fprintf(stdout,"There are %i elements in the row and %i in the column. ", best_help4_row_element, 
                best_help5_column_element); 
            
    fprintf(stdout,"In total there are %i elements in the simulation from the requested %i.\n", 
                best_help6_total_elements, num_of_elem);
                
    // Calculate the resulting window size
    Visual2D::screen_height = best_help4_row_element * best_element_size;
    Visual2D::screen_width = best_help5_column_element * best_element_size;
    
    fprintf(stdout,"The height from the window is %i and the width is %i.\n", Visual2D::screen_height, 
                Visual2D::screen_width);
        
    return best_element_size;
}

}
