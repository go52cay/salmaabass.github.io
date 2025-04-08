#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>
#include <functional>
#include <iostream>
#include <limits>

/**
 * @brief Prompts the user for input and validates it.
 * 
 * @tparam T The type of the input variable.
 * @param userMessage The message displayed to the user.
 * @param validator A function to validate the input. Defaults to a function that always returns true.
 * @return The validated input value.
 */
template <typename T>
T getInputVariable(const std::string& userMessage, std::function<bool(const T&)> validator = [](const T&){return true;}){
    T input_value;
    std::cout << userMessage;
    while (true){
        if (!(std::cin >> input_value)){
            std::cout << "Invalid input (wrong datatype)!\n" << userMessage;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (!validator(input_value)){
            std::cout << "Invalid input (validation failed)!\n" << userMessage;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        return input_value;
    }
}

#endif