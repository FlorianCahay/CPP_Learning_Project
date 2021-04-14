#pragma once

#include "airport.hpp"
#include "aircraft_manager.hpp"

#include <cassert>
#include <fstream>
#include <string_view>
#include <unordered_set>

class AircraftFactory {
    private:
        const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
        std::vector<std::unique_ptr<AircraftType>> aircraft_types;
        std::unordered_set<std::string_view> aircraft_initialized;

        // Pour pouvoir faire des split de string...
        std::vector<std::string> split(const std::string& s) {
            std::stringstream ss(s);
            std::vector<std::string> words;
            for (std::string w; ss>>w; ) words.push_back(w);
            return words;
        }

        // Liste par défaut des types des avions
        std::vector<std::unique_ptr<AircraftType>> init_default_aircraft_types() {
            std::vector<std::unique_ptr<AircraftType>> types;
            types.emplace_back(std::make_unique<AircraftType>(.02f, .05f, .02f, MediaPath { "l1011_48px.png" }));
            types.emplace_back(std::make_unique<AircraftType>(.02f, .05f, .02f, MediaPath { "b707_jat.png" }));
            types.emplace_back(std::make_unique<AircraftType>(.02f, .07f, .02f, MediaPath { "concorde_af.png" }));
            return types;
        }
    
    public:
        [[nodiscard]] std::unique_ptr<Aircraft> create_aircraft(const AircraftType& type, Airport* airport) {
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

        [[nodiscard]] std::unique_ptr<Aircraft> create_random_aircraft(Airport* airport) {
            return create_aircraft(*(aircraft_types[rand() % aircraft_types.size()]), airport);
        }

        std::string get_one_airline(const unsigned short index) const {
            return airlines[index];
        }

        // Un type par avion avec un espace entre chaque valeur.
        void load_types(const MediaPath& path) {
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

        

};