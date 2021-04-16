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
        unsigned int crash_counter = 0;
    
    public:
        void add(std::unique_ptr<Aircraft> aircraft);

        bool update() override;

        int get_required_fuel() const;
        void counter(const std::string x) const;
        void print_nb_crash() const;
};