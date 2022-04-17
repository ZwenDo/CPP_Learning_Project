#pragma once


#include <map>
#include <string>
#include "aircraft.hpp"

class AircraftManager : public GL::DynamicObject {

private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts = {};
    int crashed_aircrafts = 0;
    void move(float delta) override;

public:
    void add_aircraft(std::unique_ptr<Aircraft>& aircraft);
    int get_required_fuel() const;
    int get_crashed_aircrafts() const;

};



