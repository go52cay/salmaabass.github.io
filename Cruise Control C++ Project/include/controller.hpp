#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <limits>

/**
 * @brief Controller class that implements a PID controller.
 * 
 * This class provides functionality for setting PID gains, computing the output
 * of a PID controller, and adjusting output limits. It supports interactive user
 * input for tuning the controller and can calculate optimal PID gains based on
 * system parameters.
 */
class Controller {
private:
    double _K_P;                /**< @brief Proportional gain (K_P) */
    double _K_I;                /**< @brief Integral gain (K_I) */
    double _K_D;                /**< @brief Derivative gain (K_D) */
    double _maxOutput;          /**< @brief Maximum output limit */
    double _minOutput;          /**< @brief Minimum output limit */

public:
    /**
     * @brief Default constructor to initialize the PID controller with user input.
     * It calls the getUserInput() method to get the PID gains and output limits from the user.
     */
    Controller();

    /**
     * @brief Parameterized constructor to initialize the PID controller.
     * 
     * @param K_P Proportional gain value
     * @param K_I Integral gain value
     * @param K_D Derivative gain value
     * @param maxOutput Maximum output limit
     * @param minOutput Minimum output limit
     */
    Controller(double K_P, double K_I, double K_D, 
        double maxOutput = std::numeric_limits<double>::infinity(), 
        double minOutput = -std::numeric_limits<double>::infinity());

    /**
     * @brief Prints a message to the console indicating the successful creation of the controller.
     */
    void printCreationMessage() const;

    /**
     * @brief Function to get user input for the PID controller.
     */
    void getUserInput();

    /**
     * @brief Function to compute the PID output based on current velocity, desired velocity, and time step.
     * 
     * This function computes the PID control output based on the current velocity, the desired
     * velocity, and the time step (dt) between control iterations.
     * 
     * @param v The current velocity
     * @param v_des The desired velocity
     * @param dt The time step
     * 
     * @return The computed PID output
     */
    double computeOutput(double v, double v_des, double dt) const;

    /**
     * @brief Function to set optimal gains for the PID controller.
     * 
     * This method sets the optimal PID gains using default system values (m=1000 kg, b=50 Ns/m).
     * This could be based on a tuning method, such as Ziegler-Nichols.
     */
    void setOptimalGains();

    // Setter functions to modify the PID controller's gain and output limits

    /**
     * @brief Sets the proportional gain for the PID controller.
     * 
     * @param K_P The proportional gain value to set
     */
    void setKp(double K_P);

    /**
     * @brief Sets the integral gain for the PID controller.
     * 
     * @param K_I The integral gain value to set
     */
    void setKi(double K_I);

    /**
     * @brief Sets the derivative gain for the PID controller.
     * 
     * @param K_D The derivative gain value to set
     */
    void setKd(double K_D);

    /**
     * @brief Sets the maximum output limit for the PID controller.
     * 
     * @param maxOutput The maximum output value to set
     */
    void setMaxOutput(double maxOutput);

    /**
     * @brief Sets the minimum output limit for the PID controller.
     * 
     * @param minOutput The minimum output value to set
     */
    void setMinOutput(double minOutput);

    // Getter functions to access the current values of the PID gains and output limits

    /**
     * @brief Gets the proportional gain value.
     * 
     * @return The current proportional gain (K_P)
     */
    double getKp() const;

    /**
     * @brief Gets the integral gain value.
     * 
     * @return The current integral gain (K_I)
     */
    double getKi() const;

    /**
     * @brief Gets the derivative gain value.
     * 
     * @return The current derivative gain (K_D)
     */
    double getKd() const;

    /**
     * @brief Gets the maximum output limit.
     * 
     * @return The current maximum output limit
     */
    double getMaxOutput() const;

    /**
     * @brief Gets the minimum output limit.
     * 
     * @return The current minimum output limit
     */
    double getMinOutput() const;
};

#endif
