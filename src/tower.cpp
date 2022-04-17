#include "tower.hpp"

#include "airport.hpp"
#include "terminal.hpp"

#include <cassert>

WaypointQueue Tower::get_circle() const {
    return {{Point3D{-1.5f, -1.5f, .5f}, wp_circle},
            {Point3D{1.5f, -1.5f, .5f},  wp_circle},
            {Point3D{1.5f, 1.5f, .5f},   wp_circle},
            {Point3D{-1.5f, 1.5f, .5f},  wp_circle}};
}

WaypointQueue Tower::get_instructions(Aircraft &aircraft) {
    if (!aircraft.is_at_terminal) {
        // if the aircraft is far, then just guide it to the airport vicinity
        //if (aircraft.distance_to(airport.pos) < 5) {
        return get_circle();
        //}
    } else {
        // get a path for the craft to start
        unsigned long terminal_num = reserved_terminals[&aircraft];
        Terminal &terminal = airport.get_terminal(terminal_num);
        if (!terminal.is_servicing()) {
            terminal.finish_service();
            reserved_terminals.erase(&aircraft);
            aircraft.is_at_terminal = false;
            aircraft.service_done();
            return airport.start_path(terminal_num);
        } else {
            return {};
        }
    }
}

void Tower::arrived_at_terminal(const Aircraft &aircraft) {
    unsigned long number = reserved_terminals[&aircraft];
    airport.get_terminal(number).start_service(aircraft);
}

WaypointQueue Tower::reserve_terminal(Aircraft &aircraft) {
    // try and reserve a terminal for the craft to land
    const auto vp = airport.reserve_terminal(aircraft);
    if (!vp.first.empty()) {
        reserved_terminals[&aircraft] = vp.second;
        return vp.first;
    }
    return {};
}

void Tower::release_terminal(const Aircraft &aircraft) {
    auto it = reserved_terminals.find(&aircraft);
    if (it == reserved_terminals.end()) {
        return;
    }
    reserved_terminals.erase(it);
    airport.get_terminal(it->second).release();
}
