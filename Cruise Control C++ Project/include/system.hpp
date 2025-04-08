#ifndef SYSTEM_HPP
#define SYSTEM_HPP

/**
 * @brief Represents a simple physical system with mass and damping coefficient.
 */
class System {
private:
    double _m; ///< Mass of the system (default: 1000 kg)
    double _b; ///< Damping coefficient (default: 50 Ns/m)

public:
    /**
     * @brief Default constructor to initialize the system with user input.
     * It calls the getUserInput() method to get the system parameters from the user.
     */
    System();

    /**
     * @brief Parameterized constructor.
     * 
     * @param m Mass of the system.
     * @param b Damping coefficient.
     */
    System(double m, double b);

    /**
     * @brief Prints a message to the console indicating the successful creation of the system.
     */
    void printCreationMessage() const;

    /**
     * Prompts the user to decide whether to use default parameters or enter custom ones.
     * Initializes the system parameters accordingly.
     */
    void getUserInput();

    /**
     * @brief Computes the system's response (new acceleration).
     * 
     * Calculates the acceleration of the system based on the input force and current velocity.
     * @param u Input force (e.g., output of a controller).
     * @param v Current velocity of the system.
     * @return Computed acceleration.
     */
    double computeResponse(double u, double v) const;
};

#endif // SYSTEM_HPP