#include <stdio.h>
#include <chrono>
#include <thread>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "cmake_config.h"
#include "readConfig.hpp"
#include "visual2d.hpp"


using namespace std;

int main (int argc, char *argv[])
{
	// Output of the program version.
	fprintf(stdout,"%s -- Version %d.%d\n", GAME_PROJECT_NAME, GAME_VERSION_MAJOR, GAME_VERSION_MINOR);

        // Path to the configuration file.
        std::string help_string = CONFIG_DIRECTORY;    // CONFIG_DIRECTORY - Variable from the CMakeLists.txt in the main project folder.
        help_string.append("/sim.config");

	// Read the configuration file and get the parameter.
	GameOfLife::ReadConfig read_config(help_string);

        int number_of_elements = 100;
        std::string window_form = "square";

        read_config.get_parameter("number_of_elements", number_of_elements);
        read_config.get_parameter("window_form", window_form);

	// Starting the 2D visualization.
	GameOfLife::Visual2D visualization(number_of_elements, window_form);
/*
	// Frequency control and execution time calculation.
	// Parameter. This will go into the configuration file later.
	int averge_depth_exe = 10;
	int averge_depth_fps = 10;

	float freqyency = 60;
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

	// Loop for the visualization.
	while (visualization.window->isOpen())
	{
		// Starting point of the loop. Measure the system time for FPS controlling and exact time for execution time analysis.
		fps_counter_start = std::chrono::system_clock::now();
		execution_time_start = std::chrono::high_resolution_clock::now();

		// Re-draw the scenery.
		visualization.WindowUpdater();

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
	}*/

	return 0;
}
