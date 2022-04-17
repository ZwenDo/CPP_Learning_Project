#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;

TowerSimulation::TowerSimulation(int argc, char** argv) :
    flop { argc, argv },
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) }
{
    create_keystrokes();
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}

void TowerSimulation::create_random_aircraft()
{
    auto aircraft = aircraft_factory.create_random_aircraft(airport->get_tower());
    aircraft_manager.add_aircraft(aircraft);
}

void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { create_random_aircraft(); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    GL::keystrokes.emplace('t', []() { GL::ticks_per_sec = std::max(1u, GL::ticks_per_sec - 1); });
    GL::keystrokes.emplace('y', []() { GL::ticks_per_sec++; });
    GL::keystrokes.emplace('b', []() { GL::framerate = std::max(1.f, GL::framerate - 1); });
    GL::keystrokes.emplace('n', []() { GL::framerate++; });
    GL::keystrokes.emplace('p', []() { GL::paused = !GL::paused; });
    GL::keystrokes.emplace('m',
                           [this]()
                           {
                               std::cout << aircraft_manager.get_crashed_aircrafts() << " aircraft(s) crashed"
                                         << std::endl;
                               ;
                           });
    GL::keystrokes.emplace('h', [this]() { display_help(); });
    for (int i = 0; i < 8; ++i)
    {
        GL::keystrokes.emplace('0' + i,
                               [this, i]()
                               {
                                   int count = aircraft_factory.get_aircraft_count(i);
                                   std::cout << "Airline n°" << i << " has " << count << " aircraft(s)."
                                             << std::endl;
                               });
    }
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    for (const auto& [first, _] : GL::keystrokes)
    {
        std::cout << first << ' ';
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    assert(!airport && "airport already initialized");
    airport = new Airport { aircraft_manager, one_lane_airport, Point3D { 0, 0, 0 },
                            new img::Image { one_lane_airport_sprite_path.get_full_path() } };
    assert(airport && "airport initialization failed");
    GL::move_queue.emplace(airport);
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();
    GL::move_queue.emplace(&aircraft_manager);
    GL::loop();
}
