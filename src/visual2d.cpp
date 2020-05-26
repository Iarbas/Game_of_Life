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
    
    // In case of a dual / tripple or even more monitor system, we only want to see the window on the first screen. 
    bool found = false;
    float factor = 1.0f;
    
    // Here the monitors are alongside...
    if (desktop.height <= desktop.width)
    {
        // Compare the resolution in a lookup table.
        for (int i = 0; i < sizeof common_screen_res / sizeof common_screen_res[0]; i++)
        {      
            // If we find the resolution in our list, we do not have to to anything.
            if (desktop.height == common_screen_res[i][1] && desktop.width == common_screen_res[i][0])
            {
                found = true;
                factor = 1.0f;
                break;
            }
            else if (desktop.height == common_screen_res[i][1])
            {
                factor = (float) (desktop.width) / (float) (common_screen_res[i][0]);
                                
                if (factor == (int) factor)
                {
                    // If we look in the table we will see, that there are some values not unique. 
                    // So, if we find a interger factor, we will have the right screen size.
                    found = true;
                    desktop.width = desktop.width / factor;
                    break;
                }
            }
        }        
    }
    // ... here there are on top of each other.
    else
    {
        for (int i = 0; i < sizeof common_screen_res / sizeof common_screen_res[0]; i++)
        {
            // If we find the resolution in our list, we do not have to to anything.
            if (desktop.width == common_screen_res[i][0] && desktop.height == common_screen_res[i][1])
            {
                found = true;
                factor = 1.0f;
                break;
            }
            else if (desktop.width == common_screen_res[i][0])
            {
                factor = (float) (desktop.height) / (float) (common_screen_res[i][1]);
                
                if (factor == (int) factor)
                {
                    // If we look in the table we will see, that there are some values not unique. 
                    // So, if we find a interger factor, we will have the right screen size.
                    found = true;
                    desktop.height = desktop.height / factor;
                    break;
                }
            }
        }     
    }
        
    // Check, if we have the right window size.
    if (found && factor == 1)
    {
        fprintf(stdout,"Single monitor system detected, no changes to the resolution.\n");
    }
    else if (found)
    {
        fprintf(stdout,"Multiple monitor system detected, adapt the window size to only one screen.\n");
    }
    else
    {
        fprintf(stdout,"\n\n+++++++++++ Something went wrong +++++++++++\n");
        fprintf(stdout,"Couldn't find the right resolution for your screen.\n");
        fprintf(stdout,"Height: %i \t and Width: %i\n", desktop.height, desktop.width);        
        fprintf(stdout,"Please send this to me with the corresponding config file.\n\n");    
        std::exit(0); 
    }
    
    
    // We use the muliplicatior 0.95, because of the Panel/Dock from the Ubuntu screen.
    Visual2D::screen_height = 0.95f * desktop.height;
    Visual2D::screen_width = 0.95f * desktop.width;
    
    fprintf(stdout,"Window size will be: %i x %i (W x H).\n", Visual2D::screen_width, Visual2D::screen_height);
	
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

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    Visual2D::window = new sf::RenderWindow(sf::VideoMode(Visual2D::screen_width, Visual2D::screen_height), "Game_of_Life", sf::Style::Close, settings);
    Visual2D::biotope = new sf::Texture;
    
    fprintf(stdout,"%i %i %i %i.\n", desktop.width, desktop.height, Visual2D::screen_width, Visual2D::screen_height);
    
    Visual2D::window->setPosition(sf::Vector2i(0.5 * (desktop.width - Visual2D::screen_width), 0.5 * (desktop.height - Visual2D::screen_height)));
	
    // Draw the whole scenery. (https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php)
    
    // Draw the background
    Visual2D::background = new sf::RectangleShape;
    background->setSize(sf::Vector2f(Visual2D::screen_width, Visual2D::screen_height));
    background->setPosition(sf::Vector2f(0.f, 0.f));
    background->setFillColor(sf::Color(128,128,128)); // grey background

    this->Init();
}

/** @fn Visual2D::~Visual2D()
 *  @brief Destructor of the class Visual2D.
 *
 *  This functions deletes all created objects.
 */
Visual2D::~Visual2D()
{
    delete Visual2D::window;
    delete Visual2D::biotope;
}

/** @fn Visual2D::Init()
 *  @brief Initial function of the class Visual2D.
 *
 *  This functions draws the first window.
 */
void Visual2D::Init()
{
    // Create the initial state.
    Visual2D::window->clear();
    // Visual2D::window->draw(*Visual2D::biotope);
    Visual2D::window->draw(*background);
    Visual2D::window->display();
}

/** @fn Visual2D::WindowUpdater()
 *  @brief Re-draws the window with new input.
 *
 *  This functions draws continuously windows with new input from other Game_of_Life parts.
 */
void Visual2D::WindowUpdater()
{
	if (Visual2D::window->isOpen())
	{
		if (Visual2D::window->pollEvent(Visual2D::event))
		{
			if (Visual2D::event.type == sf::Event::Closed) Visual2D::window->close();
		}
		else
		{
			Visual2D::window->clear();
			// Visual2D::window->draw(*Visual2D::biotope);
			Visual2D::window->draw(*background);
			Visual2D::window->display();
		}
	}
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
    float ratio = (float) (b_adjusted) / (float) (a_adjusted);
    
    // Distribute the elements according to the ratio of the screen.
            
    int x = sqrt((float) (num_of_elem) / ratio); // x * ratio = y | x * y = num_of_elem
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
        fprintf(stdout,"Please send this to me with the corresponding config file.\n\n");    
        std::exit(0);    
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
