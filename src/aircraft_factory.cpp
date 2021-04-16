#include "aircraft_factory.hpp"

[[nodiscard]] std::unique_ptr<Aircraft> AircraftFactory::create_aircraft(const AircraftType& type, Airport* airport) {
    assert(airport); // make sure the airport is initialized before creating aircraft

    std::string flight_number;
    do {
        flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
    }
    while (!aircraft_initialized.emplace(flight_number).second);

    const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
    const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
    const Point3D direction = (-start).normalize();

    return std::make_unique<Aircraft>(type, flight_number, start, direction, airport->get_tower());
}

[[nodiscard]] std::unique_ptr<Aircraft> AircraftFactory::create_random_aircraft(Airport* airport) {
    assert(aircraft_types.size() > 0);
    return create_aircraft(*(aircraft_types[rand() % aircraft_types.size()]), airport);
}

std::string AircraftFactory::get_one_airline(const unsigned short index) const {
    assert(index >= 0 && index < NB_AIRLINES);
    return airlines[index];
}

void AircraftFactory::load_types(const MediaPath& path) {
    std::vector<std::unique_ptr<AircraftType>> types;
    std::ifstream input(path.get_full_path());
    if (!input.is_open()) {
        aircraft_types = init_default_aircraft_types();
    } else {
        for (std::string str; std::getline(input, str);) {
            auto tokens = split(str);
            aircraft_types.emplace_back(std::make_unique<AircraftType>(std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2]), MediaPath { tokens[3] } ));
        }
    }
}