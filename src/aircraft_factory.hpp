#pragma once

#include "airport.hpp"
#include "aircraft_manager.hpp"

#include <cassert>
#include <fstream>
#include <string_view>
#include <unordered_set>

class AircraftFactory {
    private:
        const std::string airlines[NB_AIRLINES] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
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
        [[nodiscard]] std::unique_ptr<Aircraft> create_aircraft(const AircraftType& type, Airport* airport);
        [[nodiscard]] std::unique_ptr<Aircraft> create_random_aircraft(Airport* airport);

        std::string get_one_airline(const unsigned short index) const;

        // Un type par avion avec un espace entre chaque valeur.
        void load_types(const MediaPath& path);
};