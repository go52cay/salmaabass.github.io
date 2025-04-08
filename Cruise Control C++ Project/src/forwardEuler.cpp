#include "forwardEuler.hpp"
#include "helper.hpp"

ForwardEuler::ForwardEuler(const Controller& controller, const System& system, const Function& desiredVelocityFunction)
    : Solver(controller, system, desiredVelocityFunction){
    ForwardEuler::getUserInput();
    ForwardEuler::printCreationMessage();
    }

ForwardEuler::ForwardEuler(const Controller& controller, const System& system, const Function& desiredVelocityFunction, double t_end, double dt, double v0, double t0)
    : Solver(controller, system, desiredVelocityFunction, t_end, v0, t0){
    if (dt <= 0) {
        throw std::invalid_argument("The time step must be greater than zero!");
    }
    this->dt = dt;
    ForwardEuler::printCreationMessage();
}

void ForwardEuler::printCreationMessage() {
    std::cout << "ForwardEuler Solver was created successfully with parameters t0 = " << timeStamps[0]
    << ", v0 = " << velocities[0] << ", dt = " << dt << ", t_end = " << t_end << "\n\n";
}

void ForwardEuler::getUserInput(){

    char choice = getInputVariable<char>("You have to choose the solver parameters.\n"
    "Do you want to choose the default (enter d), set them manually (enter m): ", 
    [](char choice){return choice=='d' || choice=='m';});

    switch (choice){
    case 'd':
        Solver::setDefaultInitialConditions();
        Solver::setDefaultEndTime();
        dt = 0.1;
        break;

    case 'm':
        Solver::getUserInitialConditions();
        Solver::getUserEndTime();
        dt = getInputVariable<double>("Enter time step size dt (in s): ",
        [](double i){return i > 0;});
        break;

    default:  
        throw std::invalid_argument("Unallowed value for choice!");
        break;
    }
}

    /**
 * @brief Runs the complete simulation of the system and the controller.
 * This function uses the forward Euler method to fill the std::vectors of the Solver class.
 */
void ForwardEuler::run(){

    if (!Solver::validateInitial()){
        throw std::invalid_argument("The initial vectors are invalid!");
    }

    double v = velocities[0];
    for (double t = timeStamps[0]; t <= t_end + 1e-9; t += dt){
        if (t != timeStamps[0]){
            timeStamps.push_back(t);
        }
        double v_des = desiredVelocityFunction(t);
        desiredVelocities.push_back(v_des);

        double u = controller.computeOutput(v, v_des, dt);
        controllerOutputs.push_back(u);

        double a = system.computeResponse(u, v);
        accelerations.push_back(a);

        v += a * dt;
        velocities.push_back(v);
    }
    velocities.pop_back(); // To ensure equal lengths of all vectors

    if (!Solver::validateResult()){
        throw std::invalid_argument("The resulting vectors are invalid!");
    }   
} 