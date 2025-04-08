#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "system.hpp"
#include "controller.hpp"
#include "function.hpp"
#include <vector>
#include <string>


/**
 * @brief Abstract class for a generic solver.
 * 
 * This class provides functionality which is common for all concrete solvers like 
 * validating initial conditions, exporting results to CSV, etc.
 * It is designed to be inherited by specific solver classes, such as ForwardEuler or RungeKutta.
 */

class Solver {
protected:
    const Controller& controller; // reference to the controller object
    const System& system; // reference to the system object
    const Function& desiredVelocityFunction; // reference to the desired velocity function
    std::vector<double> timeStamps;
    std::vector<double> velocities;
    std::vector<double> desiredVelocities;
    std::vector<double> controllerOutputs;
    std::vector<double> accelerations;
    double t_end; // end time of the simulation

public:

    /**
     * @brief Construct a new Solver object without full initialization.
     * Complete initialization is managed by derived classes.
     * 
     * @param controller Controller object
     * @param system System object
     * @param desiredVelocityFunction Function object
     */
    Solver(const Controller& controller, const System& system, const Function& desiredVelocityFunction);
    
    /**
     * @brief Construct a new Solver object with specified parameters.
     * 
     * @param controller Controller object
     * @param system System object
     * @param desiredVelocityFunction Function object
     * @param t_end End time of the simulation
     * @param v0 Initial velocity
     * @param t0 Initial time
     */
    Solver(const Controller& controller, const System& system, const Function& desiredVelocityFunction, 
        double t_end, double v0 = 0, double t0 = 0);
    
    /**
     * @brief Function to get user input for the initial conditions of the simulation.
     */
    void getUserInitialConditions();

    /**
     * @brief Function to set default initial conditions for the simulation.
     */
    void setDefaultInitialConditions();

    /**
     * @brief Function to get user input for the end time of the simulation.
     */
    void getUserEndTime();

    /**
     * @brief Function to set default end time for the simulation.
     */
    void setDefaultEndTime();

    /**
     * @brief Function to validate the initial state of the std::vectors.
     * 
     * @return true if the initial conditions are valid
     * @return false if the initial conditions are invalid
     */
    bool validateInitial() const;

    /**
     * @brief Function to run the complete simulation of the system and the controller.
     * 
     * This function is implemented by derived classes.
     */
    virtual void run() = 0;

    /**
     * @brief Function to validate the resulting state of the std::vectors.
     * 
     * @return true if the results are valid
     * @return false if the results are invalid
     */
    bool validateResult() const;

    /**
     * @brief Function to export the simulation results to a CSV file.
     * 
     * @param dirname The name of the directory to export the file to
     * @param filename The name of the file to export the data to
     */
    void exportToCSV(std::string dirname = "csv_exports", std::string filename = "output.csv") const;
    
    /**
     * @brief Function to display the simulation results to the console.
     */
    void printResults() const;
};


#endif