#include "aircraft_factory.hpp"


const std::string airlines[8] = {"AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY"};

std::unique_ptr<Aircraft> AircraftFactory::create_aircraft(Tower &tower, const AircraftType &type) {
    std::string flight_number;
    do {
        flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
    } while (created_aircrafts.find(flight_number) != created_aircrafts.end());
    created_aircrafts.insert(flight_number);

    const float angle = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
    const Point3D start = Point3D{std::sin(angle), std::cos(angle), 0} * 3 + Point3D{0, 0, 2};
    const Point3D direction = (-start).normalize();

    return std::make_unique<Aircraft>(type, flight_number, start, direction, tower);
}

std::unique_ptr<Aircraft> AircraftFactory::create_random_aircraft(Tower &tower) {
    return create_aircraft(tower, *(aircraft_types[rand() % 3]));
}

void AircraftFactory::init_aircraft_types() {
    aircraft_types[0] = new AircraftType{.02f, .05f, .02f, MediaPath{"l1011_48px.png"}};
    aircraft_types[1] = new AircraftType{.02f, .05f, .02f, MediaPath{"b707_jat.png"}};
    aircraft_types[2] = new AircraftType{.02f, .1f, .02f, MediaPath{"concorde_af.png"}};
}

int AircraftFactory::get_aircraft_count(int id) const {
    const std::string &airline = airlines[id];
    return (int) std::count_if(
            created_aircrafts.begin(),
            created_aircrafts.end(),
            [airline](const auto &item) {
                return item.rfind(airline, 0) == 0;
            }
    );
}

