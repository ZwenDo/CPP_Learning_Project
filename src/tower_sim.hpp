#pragma once

#include "aircraft_manager.hpp"
#include "aircraft_factory.hpp"

class Airport;
struct AircraftType;

class TowerSimulation
{
private:
    struct ContextInitializer {
        ContextInitializer(int argc, char **argv) {
            MediaPath::initialize(argv[0]);
            std::srand(static_cast<unsigned int>(std::time(nullptr)));
            GL::init_gl(argc, argv, "Airport Tower Simulation");
        }
    };

    const ContextInitializer flop;
    bool help        = false;
    Airport* airport = nullptr;
    AircraftManager aircraft_manager;
    AircraftFactory aircraft_factory;

    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    void create_keystrokes();
    void display_help() const;

    void init_airport();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();

    void create_random_aircraft();
};
