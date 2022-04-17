#pragma once


#include "aircraft_types.hpp"
#include "aircraft.hpp"

class AircraftFactory {
public:
    std::unique_ptr<Aircraft> create_aircraft(Tower& tower, const AircraftType& type);
    std::unique_ptr<Aircraft> create_random_aircraft(Tower& tower);
    int get_aircraft_count(int id) const;

    AircraftFactory()
    {
        init_aircraft_types();
    }

private:
    static constexpr size_t NUM_AIRCRAFT_TYPES = 3;
    std::set<std::string> created_aircrafts;
    AircraftType* aircraft_types[NUM_AIRCRAFT_TYPES] {};

    void init_aircraft_types();
};



