#include "controller.hpp"
#include <iostream>
#include <algorithm>
#include "helper.hpp"


Controller::Controller()
    :_maxOutput(std::numeric_limits<double>::infinity()), 
    _minOutput(-std::numeric_limits<double>::infinity()){
    Controller::getUserInput();
    Controller::printCreationMessage();
}

Controller::Controller(double K_P, double K_I, double K_D, double maxOutput, double minOutput)
    :_K_P(K_P), _K_D(K_D), _K_I(K_I), 
    _maxOutput(maxOutput), _minOutput(minOutput)
    {Controller::printCreationMessage();}

void Controller::printCreationMessage() const {
    std::cout << "Controller was created successfully with parameters: K_P = " << _K_P 
    << ", K_I = " << _K_I << ", K_D = " << _K_D << ", maxOutput = " << _maxOutput 
    << ", minOutput = " << _minOutput << "\n\n";
}

void Controller::getUserInput() {
    char choice = getInputVariable<char>("You have to choose the controller gains.\n"
    "Do you want to choose the precomputed optimal values (enter o) "
    "or set them manually (enter m): ", [](char choice){return choice=='m' || choice=='o';});

    switch (choice){
    case 'm':
        _K_P= getInputVariable<double>("Enter the Proportional gain K_P: ");
        _K_D= getInputVariable<double>("Enter the Derivative gain K_D: ");
        _K_I= getInputVariable<double>("Enter the Integral gain K_I: ");
        _maxOutput= getInputVariable<double>("Enter the Maximum output limit (in N): ");
        _minOutput= getInputVariable<double>("Enter the Minimum output limit (in N): ");
        break;
    case 'o':
        Controller::setOptimalGains();
        break;
    default:  
        throw std::invalid_argument("Unallowed value for choice!");
        break;
    }
}

double Controller::computeOutput(double v, double v_des, double dt) const {
    double error = v_des - v;
    double u = 0;

    static double integralError = 0;
    static double lastError = error;
    static double lastDt = dt;

    u += _K_P * error;

    u += _K_I * integralError;
    integralError += error * dt;

    u += _K_D * (error - lastError) / lastDt;
    lastError = error;
    lastDt = dt;

    // Clamp the output to max/min output limits
    u = std::clamp(u, _minOutput, _maxOutput);

    return u;
}

void Controller::setOptimalGains() {
    _K_P = 246.3;
    _K_I = 28.3;
    _K_D = 266.6;
}

void Controller::setKp(double K_P) {
    _K_P = K_P;
}

void Controller::setKi(double K_I) {
    _K_I = K_I;
}

void Controller::setKd(double K_D) {
    _K_D = K_D;
}

void Controller::setMaxOutput(double maxOutput) {
    _maxOutput = maxOutput;
}

void Controller::setMinOutput(double minOutput) {
    _minOutput = minOutput;
}

double Controller::getKp() const {
    return _K_P;
}

double Controller::getKi() const {
    return _K_I;
}

double Controller::getKd() const {
    return _K_D;
}

double Controller::getMaxOutput() const {
    return _maxOutput;
}

double Controller::getMinOutput() const {
    return _minOutput;
}
