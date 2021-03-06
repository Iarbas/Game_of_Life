#include <stdio.h>
#include <chrono>
#include <thread>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "cmake_config.h"
#include "readConfig.hpp"
#include "visual2d.hpp"
#include "lifeCycle.hpp"
#include "python_wrapper.hpp"

#include <unistd.h>


using namespace std;

int main (int argc, char *argv[])
{
	// Output of the program version.
	fprintf(stdout,"%s -- Version %d.%d\n", GAME_PROJECT_NAME, GAME_VERSION_MAJOR, GAME_VERSION_MINOR);

        // Path to the configuration file. CONFIG_DIRECTORY - Variable from the CMakeLists.txt in the main project 
        // folder.
        std::string help_string = CONFIG_DIRECTORY;    
        help_string.append("/sim.config");

	// Read the configuration file and get the parameter.
	SPFR::ReadConfig read_config(help_string);

        int number_of_elements = 100;
        int live_rate = 10;
        std::string window_form = "square";
        std::string initial_placement = "empty";

        read_config.get_parameter("number_of_elements", number_of_elements);
        read_config.get_parameter("window_form", window_form);
        read_config.get_parameter("initial_placement", initial_placement);
        read_config.get_parameter("live_rate", live_rate);

	// Starting the 2D visualization.
	GameOfLife::Visual2D visualization(number_of_elements, window_form);
	
	// Start the Cycle of Life.
        GameOfLife::LifeCycle lifecycle(visualization.rows, visualization.columns, 
                                        visualization.res_num_elements, initial_placement, " ", live_rate);

	// Frequency control and execution time calculation.
	// Parameter. This will go into the configuration file later.
	int averge_depth_exe = 10;
	int averge_depth_fps = 10;

	float freqyency = 50;
	read_config.get_parameter("FPS", freqyency);
	
	int fps_in_us = (int) round(1000000.0 / freqyency);

	std::chrono::time_point<std::chrono::system_clock> fps_counter_start, fps_counter_end, execution_time_start;

	uint64_t fps;
	std::chrono::duration<int64_t,micro>::rep fps_array[averge_depth_fps] = {0LL};

	double fps_average;

	uint64_t duration;
	std::chrono::duration<int64_t,micro>::rep duration_array[averge_depth_exe] = {0LL};

	double duration_average;

	int iter_fps_calc = 0;
	int iter_exe_calc = 0;
	
        // Set the size and other parameter of the window.
        sf::RenderWindow window(sf::VideoMode(visualization.window_width, visualization.window_height), 
                                "Game_of_Life", sf::Style::Close);

    
        // Sets the position of the window on the screen.
        window.setPosition(sf::Vector2i(visualization.window_posx, visualization.window_posy));
        
        // Call the Python wrapper method to plot some statistics.
        GameOfLife::PythonWrapper pythonwrapper(8);
        
        sf::Event event;
                
        bool initialization = true;
        
        // For saving the window content to a file.
        /*sf::Vector2u windowSize = window.getSize();
        sf::Texture texture;
        texture.create(windowSize.x, windowSize.y);
        sf::Image screenshot;
        std::string folder_for_images= "";
        int iter = 0;*/
                 
	// Loop for the visualization.
	while (window.isOpen())
	{
            // Starting point of the loop. Measure the system time for FPS controlling and exact time for execution time analysis.
            fps_counter_start = std::chrono::system_clock::now();
            execution_time_start = std::chrono::high_resolution_clock::now();

            // Each iteration is one life cycle, but not in the first iteration.
            if (!initialization)
            {
                lifecycle.LifeRules();
            }
            else
            {
                initialization = false;
            }

            // Re-draw the scenery.
            visualization.GridUpdater(lifecycle.individuals);
		
            while (window.pollEvent(event))
            {
                   if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) 
                   {
                       window.close();
                   }
            }
        
            // Grey background
            window.clear(sf::Color(128,128,128));
            window.draw(visualization.biotope_map);
            window.display();
            
            // Save the window content to a file.
            /*iter++;
            folder_for_images = "a_seperate_folder/" + std::to_string(iter) + ".png";
            texture.update(window);
            texture.copyToImage().saveToFile(folder_for_images);*/

            // Calculation of the execution time average.
            duration_array[iter_exe_calc] = std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::high_resolution_clock::now() -
				execution_time_start ).count();

            if (iter_exe_calc == averge_depth_exe)
            {
			iter_exe_calc = 0;
			duration = 0LL;

			// Update the average execution time value.
			for (int i = 0; i < averge_depth_exe; i++)
			{
				duration = duration + duration_array[i];
			}

			duration_average = (double) (duration / (double) averge_depth_exe);

			fprintf(stdout,"\rAverage execution time: %.3f ms. Average FPS: %.3f", duration_average / 1000.0, fps_average);
                       fflush(stdout);
            }
            else
            {
			iter_exe_calc++;
            }

            // Sleep function to control the frequency in the program.
            fps_counter_end = fps_counter_start + std::chrono::microseconds(fps_in_us);
            this_thread::sleep_until(fps_counter_end);

            // Calculation of the FPS average. Just to check, if the "sleep_until" mechanism is working.
            fps_array[iter_fps_calc] = std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::system_clock::now() -
				fps_counter_start ).count();

            if (iter_fps_calc == averge_depth_fps)
            {
			iter_fps_calc = 0;
			fps = 0LL;

			// Update the average execution time value.
			for (int i = 0; i < averge_depth_fps; i++)
			{
				fps = fps + fps_array[i];
			}

			fps_average = (double) 1000000.0 / ((double) (fps / (double) averge_depth_fps));

			fprintf(stdout,"\rAverage execution time: %.3f ms. Average FPS: %.3f", duration_average / 1000.0, fps_average);
                       fflush(stdout);
            }
            else
            {
			iter_fps_calc++;
            }
        }

	return 0;
}
