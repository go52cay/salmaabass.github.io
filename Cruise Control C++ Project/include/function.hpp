#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <memory>

/**
 *  @brief Abstract class for a generic function.
 * 
 * This class provides functionality which is common for all concrete functions like 
 * getting user input, evaluating the function at a given time via the ()-operator.
 * It is designed to be inherited by specific function classes, such as ConstFunction or LinearFunction.
 */
class Function{
public:
    virtual void getUserInput() = 0;
    virtual double operator()(double t) const = 0;
};

/**
 * @brief Class for a constant function.
 * This class represents a constant function of the form f(t) = c, where c is a constant value.
 */
class ConstFunction : public Function {
private:
    double constant;
public:

    /**
     * @brief Construct a new Const Function object based on user input.
     * This constructor calls the getUserInput() method to get the constant value from the user. 
     */
    ConstFunction();
    
    ConstFunction(double constant);
    void getUserInput() override;
    double operator()(double t) const override;
};

/**
 * @brief Class for a linear function.
 * This class represents a linear function of the form f(t) = m*t + b, where m is the slope and b is the offset.
 */
class LinearFunction : public Function {
private:
    double slope;
    double offset;
public:

    /**
     * @brief Construct a new Linear Function object based on user input.
     * This constructor calls the getUserInput() method to get the slope and offset from the user. 
     */
    LinearFunction();

    LinearFunction(double slope, double offset);
    void getUserInput() override;
    double operator()(double t) const override;
};

/**
 * @brief Get the desired velocity function from the user.
 * This function asks the user to choose between a constant and a linear function and returns a pointer to the chosen function.
 * @return Function* Pointer to the desired velocity function.
 */
std::unique_ptr<Function> getInputFunction();

#endif