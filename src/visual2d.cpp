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
 */
Visual2D::Visual2D(int number_of_elements, std::string window_form)
{
    // Initialize variables
    res_num_elements = 0;
    this->_window_form = window_form;
    this->_number_of_elements = number_of_elements;
    this->_element_size = 3;   // 3 is the minimum. TODO: in config file
    this->rows = 0; 
    this->columns = 0;
    
    // Create the window and background
    Visual2D::window = new sf::RenderWindow;
    Visual2D::background = new sf::RectangleShape;
    
    // Get screen resolution.
    desktop = sf::VideoMode().getDesktopMode();

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
    delete Visual2D::background;
}

/** @fn Visual2D::Init()
 *  @brief Initial function of the class Visual2D.
 *
 *  This functions draws the first window.
 */
void Visual2D::Init()
{
    // Calculate the window size.
    this->WindowConfigurator();
    
    // Calculate the grid size.
    this->GridConfigurator();

    // Prepare vertices or rather the biotope.
    this->BiotopeConfigurator();

    // Draw the first scenery.
    Visual2D::window->clear();
    Visual2D::window->draw(*background);
    Visual2D::window->draw(Visual2D::biotope_map);
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
            if (Visual2D::event.type == sf::Event::Closed) 
            {
                Visual2D::window->close();
            }
            else
            {
                Visual2D::window->clear();
                Visual2D::window->draw(*background);
                Visual2D::window->draw(Visual2D::biotope_map);
                Visual2D::window->display();
            }
        }
    }
}

/** @fn Visual2D::WindowConfigurator()
 *  @brief Calculates the size of the window.
 *
 *  This functions draws the window in one screen and it fits the size to the panel and task bar.
 */
void Visual2D::WindowConfigurator()
{
    fprintf(stdout,"Create window as a %s.\n", this->_window_form.c_str());
    
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
    Visual2D::window_height = 0.95f * desktop.height;
    Visual2D::window_width = 0.95f * desktop.width;
    
    fprintf(stdout,"Window size will be: %i x %i (W x H).\n", Visual2D::window_width, Visual2D::window_height);
}

/** @fn Visual2D::GridConfigurator()
 *  @brief Calculates the grid/map size in the window.
 *
 *  This functions calculates the grid/map size in the window with respect to the desired number of elements,
 *  window size and window form.
 */
void Visual2D::GridConfigurator()
{
    // Adapt the grid to the request form and number of elements.
    if (this->_window_form.compare("square") == 0)
    {
        // This form uses equilateral side length.
        
        // Search for the smallest side.
        if (Visual2D::window_height <= Visual2D::window_width)
        {
            // Set a limit to the quantity of elements.
            _number_of_elements = max_num_of_elem(_number_of_elements, Visual2D::window_height, Visual2D::window_height);
            
            // Get the size of each element with respect to the screen size
            _element_size = get_element_size(_number_of_elements, Visual2D::window_height, Visual2D::window_height);
        }
        else
        {
            // Set a limit to the quantity of elements.
            _number_of_elements = max_num_of_elem(_number_of_elements, Visual2D::window_width, Visual2D::window_width);
            
            // Get the size of each element with respect to the screen size
            _element_size = get_element_size(_number_of_elements, Visual2D::window_width, Visual2D::window_width);
        }
    }
    else if (this->_window_form.compare("rectangle") == 0)
    {
        // This option keeps the ratio of the screen resolution.
        
        // Set a limit to the quantity of elements.
        _number_of_elements = max_num_of_elem(_number_of_elements, Visual2D::window_height, Visual2D::window_width);
        
        // Get the size of each element with respect to the screen size
        _element_size = get_element_size(_number_of_elements, Visual2D::window_height, Visual2D::window_width);        
    }
    else
    {
        fprintf(stdout,"No form with the name %s exists. You should exit the program.\n", this->_window_form.c_str());
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
    
    // Re-ajust the window size in case of rounding issues in the calculation step.
    int a_adjusted = 0.95f * a;
    int b_adjusted = 0.95f * b;
    
    // Calculate ratio of the screen resolution.
    float ratio = (float) (b_adjusted) / (float) (a_adjusted);
    
    // Distribute the elements according to the ratio of the screen.
            
    float x = sqrt((float) (num_of_elem) / ratio); // x * ratio = y | x * y = num_of_elem
    float y = (float) (num_of_elem) / x;
    res_num_elements = (int) (y) * (int) (x);
    
    // Calculate the element size with respect to the screen resolution.
    int element_size_a = a_adjusted / x;
    int element_size_b = b_adjusted / y;
    
    // Check, if the the element sizes are similar.
    float diff = abs((float)(a_adjusted / x) - (float)(b_adjusted / y)); 
    
    if (diff > (0.1 * (float)(a_adjusted) / x) || diff > (0.1 * (float)(b_adjusted / y)))
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
                _element_size = element_size_a + 0.5 * diff;
            }
            else
            {
                _element_size = element_size_b + 0.5 * diff;
            } 
        }
        else
        {
            _element_size = element_size_a;
        }
        
        columns = y;
        rows = x;
    }
        
    // Control of the element size to fulfill the requirements.
    // TODO: Tolerances in the config file.
    if (_element_size < 3)
    {
        _element_size = 3;
    
        fprintf(stdout,"Something totally went wrong. Set the element size to the minimum of 9 pixel.\n");
    }
    
    fprintf(stdout,"The edge length of one element was calculated to %i pixel on each side.\n", _element_size);
    fprintf(stdout,"There are %i elements in the row and %i in the column. ", rows, columns); 
            
    fprintf(stdout,"In total there are %i elements in the simulation from the requested %i.\n", 
                res_num_elements, num_of_elem);
                
    // Calculate the resulting window size
    Visual2D::grid_height = rows * _element_size;
    Visual2D::grid_width = columns * _element_size;
    
    fprintf(stdout,"The height of the grid is %i and the width is %i Pixel.\n", Visual2D::grid_height, 
                Visual2D::grid_width);
                
    fprintf(stdout,"The window size is %i in height and %i in width.\n", a, b);
                
    fprintf(stdout,"---------- End of the Grid Size Calculation ----------\n\n");
        
    return _element_size;
}

/** @fn Visual2D::BiotopeConfigurator()
 *  @brief Prepare the vertices of the scenery.
 *
 *  This functions creates the vertices and configures the states of each individual in the biotope. The whole map
 *  will be contained in a single vertex array, therefore it  will be super fast to draw.
 */
void Visual2D::BiotopeConfigurator()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    // Set the size and other parameter of the window.
    Visual2D::window->create(sf::VideoMode(Visual2D::window_width, Visual2D::window_height), "Game_of_Life", sf::Style::Close, settings);
    
    // Sets the position of the window on the screen.
    Visual2D::window->setPosition(sf::Vector2i(0.5 * (desktop.width - Visual2D::window_width), 0.5 * (desktop.height - Visual2D::window_height)));
	
    // Draw the whole scenery. (https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php)    
    // Create the biotope
    biotope_map.setPrimitiveType(sf::Quads);
    biotope_map.resize(res_num_elements * 4);    
    
    // Start Point Width
    int spw = 0.5 * (Visual2D::window_width - Visual2D::grid_width);
    
    // Start Point Height
    int sph = 0.5 * (Visual2D::window_height - Visual2D::grid_height);
    
    int particle = 0;
    
    int ax, bx, cx, dx, ay, by, cy, dy;
    
    // Give each individual a position and color.
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)      
        {
            particle = (j + i * columns) * 4;
            
            ax = spw + j * _element_size;
            ay = sph + i * _element_size;
            
            bx = spw + (j + 1) * _element_size;
            by = sph + i * _element_size;
            
            cx = spw + (j + 1) * _element_size;
            cy = sph + (i + 1) * _element_size;
            
            dx = spw + j * _element_size;
            dy = sph + (i + 1) * _element_size; 
        
            biotope_map[particle    ].position = sf::Vector2f(ax, ay);
            biotope_map[particle + 1].position = sf::Vector2f(bx, by);
            biotope_map[particle + 2].position = sf::Vector2f(cx, cy);
            biotope_map[particle + 3].position = sf::Vector2f(dx, dy);

            biotope_map[particle    ].color    = sf::Color::White;
            biotope_map[particle + 1].color    = sf::Color::White;
            biotope_map[particle + 2].color    = sf::Color::White;
            biotope_map[particle + 3].color    = sf::Color::White;
        }
    }
        
    background->setSize(sf::Vector2f(Visual2D::window_width, Visual2D::window_height));
    background->setPosition(sf::Vector2f(0.f, 0.f));
    background->setFillColor(sf::Color(128,128,128)); // grey background
}

/** @fn Visual2D::GridUpdater()
 *  @brief Changes the color of the individual with respect to its status.
 *
 *  This functions checks continuously the biotope and change the color to the status of each individual.
 */
void Visual2D::GridUpdater(std::vector<struct_individuals> &individuals)
{
    int particle = 0;

    // Get an update on the status of the individuals
    for (int i = 0; i < individuals.size(); i++)
    {
        // If there were changes, we manipulate the color in the grid.
        if (individuals.at(i).change)
        {
            if (individuals.at(i).alive)
            {
                particle = i * 4;
            
                biotope_map[particle    ].color    = sf::Color::Black;
                biotope_map[particle + 1].color    = sf::Color::Black;
                biotope_map[particle + 2].color    = sf::Color::Black;
                biotope_map[particle + 3].color    = sf::Color::Black;
            }
            else
            {
                particle = i * 4;
            
                biotope_map[particle    ].color    = sf::Color::White;
                biotope_map[particle + 1].color    = sf::Color::White;
                biotope_map[particle + 2].color    = sf::Color::White;
                biotope_map[particle + 3].color    = sf::Color::White;
            }
            
            // Set the notifier back to false.
            individuals.at(i).change = false;
        }
    }
}


}
