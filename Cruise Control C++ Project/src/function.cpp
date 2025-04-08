#include "function.hpp"
#include "helper.hpp"


ConstFunction::ConstFunction(){
    getUserInput();
}
ConstFunction::ConstFunction(double constant)
    : constant(constant){}

void ConstFunction::getUserInput() {
    constant = getInputVariable<double>("Enter constant value: ");
}
double ConstFunction::operator()(double t) const{
    return constant;
}

LinearFunction::LinearFunction(){
    getUserInput();
}
LinearFunction::LinearFunction(double slope, double offset)
    : slope(slope), offset(offset){}

void LinearFunction::getUserInput(){
    slope = getInputVariable<double>("Enter slope: ");
    offset = getInputVariable<double>("Enter offset: ");
}
double LinearFunction::operator()(double t) const{
    return slope * t + offset;
}

std::unique_ptr<Function> getInputFunction(){
    char choice = getInputVariable<char>("Choose the desired velocity function:\n"
        "Constant function (enter c)\n"
        "Linear function (enter l)\n", [](char choice){return choice=='c' || choice=='l';});

    std::unique_ptr<Function> desiredVelocityFunction = nullptr;
    switch (choice){
    case 'c':
        desiredVelocityFunction = std::make_unique<ConstFunction>();
        break;
    case 'l':
        desiredVelocityFunction = std::make_unique<LinearFunction>();
        break;
    default:
        throw std::invalid_argument("Unallowed value for choice!");
    }
    return desiredVelocityFunction;
}