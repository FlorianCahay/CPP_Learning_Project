#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"

#include <algorithm>
#include <execution>
#include <memory>
#include <utility>
#include <vector>

class AircraftManager : public GL::DynamicObject {
    private:
        std::vector<std::unique_ptr<Aircraft>> aircrafts;
        int crash_counter = 0;
    
    public:
        void add(std::unique_ptr<Aircraft> aircraft);
        bool update() override;

        int get_required_fuel() const { 
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

        void counter(const std::string x) const {
            auto msg = x + " : ";
            msg += std::count_if(aircrafts.begin(), aircrafts.end(), [x](const std::unique_ptr<Aircraft> &a) { return a->get_flight_num().find(x) != std::string::npos; });
            std::cout << msg << std::endl;
        }
        void print_nb_crash() const {
            std::cout << crash_counter << " plane(s) have already crashed." << std::endl;
        }
};