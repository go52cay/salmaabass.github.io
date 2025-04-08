#include "system.hpp"
#include <iostream>
#include "helper.hpp"       

// Constructor
System::System() {   
    System::getUserInput();
    System::printCreationMessage();
}

System::System(double m, double b) {
    if (m <= 0 || b < 0) {
        throw std::invalid_argument("Mass and damping coefficient must be positive!");
    }
    _m = m;
    _b = b;
    System::printCreationMessage();
}

void System::printCreationMessage() const {
    std::cout << "System was created successfully with parameters: m = " 
    << _m << ", b = " << _b << "\n\n";
}

// User input method
void System::getUserInput() {
    char choice = getInputVariable<char>("You have to choose the system parameters.\n"
    "Do you want to choose the default (enter d) or set them manually (enter m): ", 
    [](char choice){return choice=='d' || choice=='m';});

    switch (choice)
    {
    case 'd':
        // Default values
        _m = 1000;
        _b = 50;
        break;

    case 'm':
        _m = getInputVariable<double>("Enter mass m (in kg): ",
        [](const double& value) { return value > 0; });
        _b = getInputVariable<double>("Enter damping coefficient b (in N*s/m): ",
        [](const double& value) { return value >= 0; });
        break;

    default:  
        throw std::invalid_argument("Unallowed value for choice!");
        break;
    }
}

// System response computation method
double System::computeResponse(double u, double v) const {
    return (u - _b * v) / _m;
}