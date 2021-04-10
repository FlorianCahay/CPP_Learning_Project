#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"

#include <memory>
#include <vector>

class AircraftManager : public GL::DynamicObject {
    private:
        std::vector<std::unique_ptr<Aircraft>> aircrafts;
        int crash_counter = 0;
    
    public:
        void add(std::unique_ptr<Aircraft> aircraft);
        bool update() override;
        void counter(const std::string x) const {
            std::cout << std::count_if(aircrafts.begin(), aircrafts.end(), [x](const std::unique_ptr<Aircraft> &a) { return a->get_flight_num().find(x) != std::string::npos; }) << std::endl;
        }
        void print_nb_crash() const {
            std::cout << crash_counter << " plane(s) have already crashed." << std::endl;
        }
};