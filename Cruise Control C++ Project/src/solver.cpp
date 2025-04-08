#include "solver.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include "helper.hpp"

Solver::Solver(const Controller& controller, const System& system, const Function& desiredVelocityFunction)
    : controller(controller), system(system), desiredVelocityFunction(desiredVelocityFunction){}

Solver::Solver(const Controller& controller, const System& system, const Function& desiredVelocityFunction, double t_end, double v0, double t0)
    : controller(controller), system(system), desiredVelocityFunction(desiredVelocityFunction){
    if (t_end <= t0) {
        throw std::invalid_argument("The end time must be greater than the initial time!");
    }
    timeStamps.push_back(t0);
    velocities.push_back(v0);
    this->t_end = t_end;
}

void Solver::getUserInitialConditions() {
    double t0 = getInputVariable<double>("Enter the initial time t0 (in s): ");
    timeStamps.push_back(t0);
    double v0 = getInputVariable<double>("Enter the initial velocity v0 (in m/s): ");
    velocities.push_back(v0);
}

void Solver::setDefaultInitialConditions() {
    timeStamps.push_back(0);
    velocities.push_back(0);
}

void Solver::getUserEndTime() {
    t_end = getInputVariable<double>("Enter the end time t_end (in s): ", 
    [t0 = timeStamps.back()](double t_end){return t_end > 0;});
}

void Solver::setDefaultEndTime() {
    t_end = 60;
}

bool Solver::validateInitial() const{
    return size(timeStamps) == 1
        && size(velocities) == 1
        && size(desiredVelocities) == 0
        && size(controllerOutputs) == 0
        && size(accelerations) == 0;
}

bool Solver::validateResult() const {
    return size(timeStamps) == size(velocities) 
        && size(velocities) == size(desiredVelocities) 
        && size(desiredVelocities) == size(controllerOutputs)
        && size(controllerOutputs) == size(accelerations);
}

void Solver::exportToCSV(std::string dirname, std::string filename) const {
    std::filesystem::path pathName = std::filesystem::current_path();
    if ((pathName.filename() == "build") || (pathName.filename() == "src")) {
        pathName = pathName.parent_path();
    }
    pathName = pathName.append(dirname);
    if (!std::filesystem::is_directory(pathName)) {
        std::cout << "The path: \n" << pathName << "\ndoes not exist. Nothing was written!\n"
            << "Please create it or change the dirname when calling this function.";
        return;
    }

    std::ofstream file(pathName.append(filename));

    if (!file.is_open()) {
        std::cout << "The file could not be open for some reason. Nothing was written!\n" << pathName;
        return;
    }

    // Write headers
    file << "TimeStamp,Velocity,DesiredVelocity,ControllerOutput,Acceleration\n";

    // Write vectors / data
    for (size_t i = 0; i < timeStamps.size(); i++) {
        file << timeStamps[i] << ','
            << velocities[i] << ','
            << desiredVelocities[i] << ','
            << controllerOutputs[i] << ','
            << accelerations[i] << '\n';
    }
    file.close();
    std::cout << "Export to CSV completed!" << std::endl;
}

void Solver::printResults() const {
    for (size_t i = 0; i <= size(timeStamps) - 1; ++i) {
        std::cout << "t:\t" << timeStamps.at(i) << "\t";
        std::cout << "v:\t" << velocities.at(i) << "\t";
        std::cout << "v_des:\t" << desiredVelocities.at(i) << "\t";
        std::cout << "u:\t" << controllerOutputs.at(i) << "\t";
        std::cout << "a:\t" << accelerations.at(i) << "\t";
        std::cout << std::endl;
    }
}
