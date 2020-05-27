// ========================================== max. 120 symbols in one line ============================================
/**
 * @par Project
 * Game_of_Life
 *
 * @file    visual2d.hpp
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

#ifndef HEADER_VISUAL2D_HPP_AP_19052020
#define HEADER_VISUAL2D_HPP_AP_19052020

#include <SFML/Graphics.hpp>


namespace GameOfLife
{
/** @class Visual2D
 *  @brief Visualization for the Game_of_Life project.
 *
 *  This class handles the visual output of the program.
 */
class Visual2D
{
public:
	Visual2D(int number_of_elements, std::string window_form);
	virtual ~Visual2D();				// Virtual Destructor.

	void WindowUpdater();

	/** @var sf::RenderWindow* window
	   *  @brief Object of the open window.
	   *
	   *  This object represents the visual output of the class.
	   */
	sf::RenderWindow* window;

private:
	/** @var sf::Event event
	   *  @brief Object of the event handler.
	   *
	   *  This object represents the user interface to the program.
	   */
	void Init();
	void WindowConfigurator();
	void GridConfigurator();
	void BiotopeConfigurator();
	
	inline int max_num_of_elem (int num_of_elem, int a, int b);
	inline int get_element_size (int num_of_elem, int a, int b);

	uint32_t window_height;
	uint32_t window_width;
	
	uint32_t grid_height;
	uint32_t grid_width;
	
	int _total_elements;
	int _number_of_elements;
	
        int _row_elements; 
        int _column_elements;
	
	int _element_size;
	
	std::string _window_form;

        sf::VideoMode desktop;

	sf::Event event;
	sf::RectangleShape* background;
	sf::VertexArray biotope_map;
	
        // Lookup table for common screen resolution (https://en.wikipedia.org/wiki/Display_resolution).
        // Because of not unique values, we cannot use "std::map" or "std::unordered_map" here.
        // Schema: {Width, Height}
	static int constexpr common_screen_res[19][2] =
	{
	{640, 360},
	{800, 600},
	{1024, 768},
	{1280, 720},
	{1280, 800},
	{1280, 1024},
	{1360, 768},
	{1366, 768},
	{1440, 900},
	{1536, 864},
	{1600, 900},
	{1680, 1050},
	{1920, 1080},
	{1920, 1200},
	{2048, 1152},
	{2560, 1080},
	{2560, 1440},
	{3440, 1440},
	{3840, 2160}
        };
};

}


#endif // HEADER_VISUAL2D_HPP_AP_19052020
