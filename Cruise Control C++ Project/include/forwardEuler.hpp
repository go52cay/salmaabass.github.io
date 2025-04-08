#ifndef FORWARDEULER_HPP
#define FORWARDEULER_HPP

#include "solver.hpp"
#include "controller.hpp"
#include "system.hpp"
#include "function.hpp"


/**
 * @brief Forward Euler solver class that implements the forward Euler method.
 * 
 * This class provides functionality for running a simulation using the forward Euler method.
 * It inherits from the Solver class and implements the run() method to fill the std::vectors
 * with the simulation data using the forward Euler method.
 */

class ForwardEuler : public Solver {
private:
    double dt; // time step size

public:

    /**
     * @brief Construct a new Forward Euler object with user input.
     * It calls the getUserInput() method to get the simulation parameters from the user.
     * 
     * @param controller Controller object
     * @param system System object
     * @param desiredVelocityFunction Function object
     */
    ForwardEuler(const Controller& controller, const System& system, const Function& desiredVelocityFunction);
    
    /**
     * @brief Construct a new Forward Euler object with specified parameters.
     * 
     * @param controller Controller object
     * @param system System object
     * @param desiredVelocityFunction Function object
     * @param t_end End time of the simulation
     * @param dt Time step size
     * @param v0 Initial velocity
     * @param t0 Initial time
     */
    ForwardEuler(const Controller& controller, const System& system, const Function& desiredVelocityFunction, 
    double t_end, double dt, double v0 = 0, double t0 = 0);

    /**
     * @brief Prints a message to the console indicating the successful creation of the Forward Euler solver.
     */
    void printCreationMessage();
    
    /**
     * @brief Function to get user input for the simulation parameters.
     */
    void getUserInput();

    /**
     * @brief Runs the complete simulation of the system and the controller.
     * This function uses the forward Euler method to fill the std::vectors with the simulation data.
     */
    void run();
};

#endif
