#include "aircraft_manager.hpp"


void AircraftManager::add(std::unique_ptr<Aircraft> aircraft) {
    aircrafts.emplace_back(std::move(aircraft));
}

int AircraftManager::get_required_fuel() const { 
    return std::transform_reduce(
        aircrafts.begin(), 
        aircrafts.end(), 
        0, 
        [] (unsigned short x, unsigned short y) { return x + y; }, 
        [] (const std::unique_ptr<Aircraft>& aircraft) { 
            if (aircraft->need_refill()) {
                return aircraft->get_required_fuel(); 
            }
            return (unsigned short) 0;
        }
    );
}

void AircraftManager::print_nb_crash() const {
    std::cout << crash_counter << " plane(s) have already crashed." << std::endl;
}

void AircraftManager::counter(const std::string x) const {
    auto msg = x + " : ";
    msg += std::count_if(aircrafts.begin(), aircrafts.end(), [x](const std::unique_ptr<Aircraft> &a) { return a->get_flight_num().find(x) != std::string::npos; });
    std::cout << msg << std::endl;
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