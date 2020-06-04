// ========================================== max. 120 symbols in one line ============================================
/**
 * @par Project
 * Game_of_Life
 *
 * @file    readConfig.cpp
 * @author  Andre Alexander Pieper
 * @version 1.0
 * @date    2020-05-18
 *
 * @brief   Read the configuration file.
 *
 * This program part is responsible for reading the configuration file and returns the requested information.
 */
// --------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <experimental/filesystem>

#include "readConfig.hpp"


namespace GameOfLife
{

ReadConfig::ReadConfig(std::string config_file_name)
{
    this->_config_file_name = config_file_name;
    this->data_size_tested = false;
    this->data_size_ok = false;
    this->Init();
}

ReadConfig::~ReadConfig()
{

}

void ReadConfig::Init()
{
    std::cout << "File name of the configuration file: " << this->_config_file_name.c_str() << std::endl;

    // Check the size of the configuration file and if it is too big or a directory end the program.
    std::experimental::filesystem::path p = this->_config_file_name.c_str();

    try {
        std::experimental::filesystem::file_size(p);
    } catch(std::experimental::filesystem::filesystem_error& e) {
        std::cout << "Error while reading the configuration file. " << e.what() << '\n';
        return;
    } 

    std::cout << "Size of the configuration file: " << std::experimental::filesystem::file_size(p) << " Bytes." << '\n';

    if(std::experimental::filesystem::file_size(p) > 1048576)
    {
        std::cout << "Size exceeds the limit of 1 MB. Stop reading the file." << '\n';
        return;
    }

    // Read the text file.
    std::ifstream text_config(this->_config_file_name);

    text_config.seekg(0, std::ios::end);

    size_t size = text_config.tellg();

    std::string buffer(size, ' ');

    text_config.seekg(0);

    text_config.read(&buffer[0], size); 

    // Remove all comments from string buffer.
    std::string buffer_new = this->remove_comments(buffer);

    // Seperate the variables from the string.
    this->raw_config = split(buffer_new, ":", " ");

    if (this->raw_config.size()%2 != 0)
    {
        std::cout << std::endl;
        std::cout << "######### ERROR #########" << std::endl;
        std::cout << "The configuration file has one or more wrong parameter." << std::endl;
        std::cout << "Limitations: There are no spaces in the path string allowed. Comments are also not allowed." << std::endl;

        std::cout << std::endl;
        std::cout << "Here an example of an allowed format:" << std::endl;
        std::cout << std::endl;
        std::cout << "path: /home/USERNAME/workspace/blubb" << std::endl;
        std::cout << "number_int: 12" << std::endl;
        std::cout << "number_float: 4234.234234" << std::endl;
        std::cout << "number_bool_1: 1" << std::endl;
        std::cout << "number_bool_2: false" << std::endl;
        std::cout << "number_array: [2,3,4.5]" << std::endl;
        std::cout << "list: {Hello, World, !}" << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Success in reading the configuration file." << std::endl;
}

std::string ReadConfig::remove_comments(std::string &buffer)
{
    int n = buffer.length(); 
    std::string buffer_new; 
  
    // Flags to indicate that comments have started or not. 
    bool comment = false;
    bool new_line = true;
    bool whole_line_is_comment = false;
  
    for (int i=0; i<n; i++) 
    { 
        // If single line comment flag is on, then check for end of it.
        if (comment == true && buffer[i] == '\n') 
        {
            comment = false;

            // If the comments are at the end of a line with values, keep the "new line".
            if(!whole_line_is_comment)
            {
                buffer_new += '\n'; 
            }
        }   
        // If this character is in a comment, ignore it.
        else if (comment) 
        {
            continue; 
        }
        // Check for beginning of comments and set the approproate flags 
        else if (buffer[i] == '#') 
        {
            comment = true; 

            if (new_line)
            {
                whole_line_is_comment = true;
            }
            else
            {
                whole_line_is_comment = false;
            }
        }
        // If current character is a non-comment character, append it to res 
        else
        {  
            buffer_new += buffer[i]; 
        }

        // Check for new lines.
        if (buffer[i] == '\n')
        {
            new_line = true;
        }
        else
        {
            new_line = false;
        }
    } 

    return buffer_new; 
}

std::vector<std::string> ReadConfig::split(const std::string& s, std::string delimiter, std::string delete_chars)
{
    std::vector<std::string> wordVector;
    std::stringstream stringStream(s);
    std::string line;

    // Split the string according to the "limiter" string and after each new line.
    while(std::getline(stringStream, line)) 
    {
        std::size_t prev = 0, pos;
        while ((pos = line.find_first_of(delimiter, prev)) != std::string::npos)
        {
            if (pos > prev)
            {
                wordVector.push_back(line.substr(prev, pos-prev));
            }

            prev = pos+1;
        }

        if (prev < line.length())
        {
            wordVector.push_back(line.substr(prev, std::string::npos));
        }
    }

    // Remove characters from the sub strings according to the "delete_chars".
    // TODO: Spaces in path strings are NOT supported! I need to fix this.
    for (int j = 0; j < wordVector.size(); j++)
    {
        for (int k = 0; k < delete_chars.size(); k++) 
        {           
            wordVector.at(j).erase(std::remove(wordVector.at(j).begin(), wordVector.at(j).end(), delete_chars[k]), wordVector.at(j).end());
        }
    }

    return wordVector;
}

void ReadConfig::check_data_size()
{
    this->data_size_tested = true;

    // If the config file is empty, this gives the user a hint.
    if(this->raw_config.size() == 0)
    {
        std::cout << "Data invalid! Please read the previous output for further information. Using initial values..." << std::endl;
    }
    else
    {
        this->data_size_ok = true;
    }
}

void ReadConfig::get_parameter(std::string int_value_name, int &int_value)
{
    int result = -1;

    if(!this->data_size_tested) this->check_data_size();

    if (this->data_size_ok)
    {
        for (int i = 0; i < this->raw_config.size(); i=i+2)
        {
            result = this->raw_config.at(i).compare(int_value_name);

            if (result == 0)
            {
                int_value = std::stoi(this->raw_config.at(i+1));

                std::cout << "Parameter for " << int_value_name.c_str() << " is: " << int_value << std::endl; 

                return;
            }
        }
    }

    if (result != 0)
    {
        std::cout << "Could not find the parameter for " << int_value_name.c_str() << ". Using instead the initial value: " << int_value << std::endl; 
    }

    return;
}

void ReadConfig::get_parameter(std::string float_value_name, float &float_value)
{
    int result = -1;

    if(!this->data_size_tested) this->check_data_size();

    if (this->data_size_ok)
    {
        for (int i = 0; i < this->raw_config.size(); i=i+2)
        {
            result = this->raw_config.at(i).compare(float_value_name);

            if (result == 0)
            {
                float_value = std::stoi(this->raw_config.at(i+1));

                std::cout << "Parameter for " << float_value_name.c_str() << " is: " << float_value << std::endl; 

                return;
            }
        }
    }

    if (result != 0)
    {
        std::cout << "Could not find the parameter for " << float_value_name.c_str() << ". Using instead the initial value: " << float_value << std::endl; 
    }

    return;
}

void ReadConfig::get_parameter(std::string str_value_name, std::string &str_value)
{
    int result = -1;

    if(!this->data_size_tested) this->check_data_size();

    if (this->data_size_ok)
    {
        for (int i = 0; i < this->raw_config.size(); i=i+2)
        {
            result = this->raw_config.at(i).compare(str_value_name);

            if (result == 0)
            {
                str_value = this->raw_config.at(i+1);

                std::cout << "Parameter for " << str_value_name.c_str() << " is: " << str_value << std::endl; 

                return;
            }
        }
    }

    if (result != 0)
    {
        std::cout << "Could not find the parameter for " << str_value_name.c_str() << ". Using instead the initial value: " << str_value << std::endl; 
    }

    return;
}

}
