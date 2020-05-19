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
	inline int max_num_of_elem (int num_of_elem, int a, int b);
	inline int get_element_size (int num_of_elem, int a, int b);

	uint32_t screen_height;
	uint32_t screen_width;

	sf::Event event;
	sf::Texture* dead_box;
	sf::Texture* live_box;
};
}

#endif // HEADER_VISUAL2D_HPP_AP_19052020
