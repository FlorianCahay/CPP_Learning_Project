#include "aircraft_manager.hpp"


void AircraftManager::add(std::unique_ptr<Aircraft> aircraft) {
    aircrafts.emplace_back(std::move(aircraft));
}

bool AircraftManager::update() {

    std::sort(aircrafts.begin(), aircrafts.end(), [](std::unique_ptr<Aircraft>& first, std::unique_ptr<Aircraft>& second) {
        if (first->has_terminal() == second->has_terminal()) {
            return first->get_fuel() < second->get_fuel();
        }
        return first->has_terminal();
    });

    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(), [this](std::unique_ptr<Aircraft>& x) { 
        try {
            return !(x->update());
        } catch (const AircraftCrash& e) {
            std::cerr << e.what() << std::endl;
            crash_counter++;
            return true;
        }
    }), aircrafts.end());

    
    /* for (auto aircraft_it = aircrafts.begin(); aircraft_it != aircrafts.end();)
    {
        // On doit déréférencer 2x pour obtenir une référence sur l'Aircraft : une fois pour déréférencer
        // l'itérateur, et une deuxième fois pour déréférencer le unique_ptr.
        auto& aircraft = **aircraft_it;
        if (aircraft.update())
        {
            ++aircraft_it;
        }
        else
        {
            aircraft_it = aircrafts.erase(aircraft_it);
        }
    } */

    return true;
}