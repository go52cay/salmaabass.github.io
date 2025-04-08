#include <iostream>
#include <memory>
#include "system.hpp"
#include "controller.hpp"
#include "forwardEuler.hpp"
#include "helper.hpp"

int main(){

    std::cout << "Welcome to the Cruise Control Simulation!\n";

    System system{}; // also calls system.getUserInput();
    // Alternative: 
    // System system{1000, 50}; // does not call system.getUserInput();

    Controller controller{}; // also calls controller.getUserInput();
    // Alternative:
    // Controller controller{300, 50, 0}; // does not call controller.getUserInput();

    std::unique_ptr<Function> desiredVelocityFunction = getInputFunction();
    // Alternative:
    // ConstFunction constFunction(20);
    // Function* desiredVelocityFunction = &constFunction;
    // or
    // LinearFunction linearFunction(0.5, 0);
    // Function* desiredVelocityFunction = &linearFunction;

    ForwardEuler solver{controller, system, *desiredVelocityFunction}; // calls solver.getUserInput();
    // Alternative:
    // ForwardEuler solver{controller, system, *desiredVelocityFunction, 60, 0.1, 0, 0}; // does not call solver.getUserInput();

    solver.run();
    solver.exportToCSV(); // can be visualized with the Python script or Excel
    solver.printResults();

}