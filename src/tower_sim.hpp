#pragma once

#include "aircraft_manager.hpp"
#include "aircraft_factory.hpp"

#include <memory>

class Aircraft;
class Airport;
struct AircraftType;

class TowerSimulation {
private:
    bool help        = false;
    Airport* airport = nullptr;
    AircraftManager manager;
    AircraftFactory factory;

    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    void create_keystrokes();
    void display_help() const;

    void init_airport();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch(std::string_view path = "empty.txt");
};
