 #pragma once

#include "geometry.hpp"

#include <deque>

enum WaypointType
{
    wp_air,
    wp_circle,
    wp_ground,
    wp_terminal
};

class Waypoint : public Point3D
{
public:
    const WaypointType type;

    Waypoint(const Point3D& position, const WaypointType type_ = wp_air) :
        Point3D { position }, type { type_ }
    {}

    bool is_on_ground() const { return type == wp_ground || type == wp_terminal; }
    bool is_at_terminal() const { return type == wp_terminal; }
    bool is_circling() const { return type == wp_circle; }
};

using WaypointQueue = std::deque<Waypoint>;