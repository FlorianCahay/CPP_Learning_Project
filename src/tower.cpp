#include "tower.hpp"

#include "airport.hpp"
#include "terminal.hpp"

#include <cassert>

// On fait tourner l'avion au tour de l'aéroport.
WaypointQueue Tower::get_circle() const {
    return { { Point3D { -1.5f, -1.5f, .5f }, wp_air },
             { Point3D { 1.5f, -1.5f, .5f }, wp_air },
             { Point3D { 1.5f, 1.5f, .5f }, wp_air },
             { Point3D { -1.5f, 1.5f, .5f }, wp_air } };
}

WaypointQueue Tower::get_instructions(Aircraft& aircraft) {
    if (!aircraft.is_at_terminal) { // si l'avion n'est pas à un terminal
        if (aircraft.distance_to(airport.pos) < 5) { // si l'avion est loin, on le guide vers les environs de l'aéroport
            // on essaye de réserver un terminal pour que l'avion atterrisse
            const auto vp = airport.reserve_terminal(aircraft);
            if (!vp.first.empty()) {
                reserved_terminals.emplace(&aircraft, vp.second);
                return vp.first;
            } else {
                return get_circle();
            }
        } else {
            return get_circle();
        }
    } else {
        // on calcul un chemin pour que l'avion décolle
        const auto it = reserved_terminals.find(&aircraft);
        assert(it != reserved_terminals.end());
        const auto terminal_num = it->second;
        Terminal& terminal      = airport.get_terminal(terminal_num);

        // si l'avion a fini son service dans le terminal
        if (!terminal.is_servicing()) { 
            aircraft.is_service_done = true;
            terminal.finish_service();
            reserved_terminals.erase(it);
            aircraft.is_at_terminal = false;
            return airport.start_path(terminal_num);
        } else {
            return {};
        }
    }
}

void Tower::arrived_at_terminal(const Aircraft& aircraft) {
    const auto it = reserved_terminals.find(&aircraft);
    assert(it != reserved_terminals.end());
    airport.get_terminal(it->second).start_service(aircraft);
}

WaypointQueue Tower::reserve_terminal(Aircraft& aircraft) {
    const auto vp = airport.reserve_terminal(aircraft);
    if (!vp.first.empty()) {
        reserved_terminals.emplace(&aircraft, vp.second);
        return vp.first;
    } else {
        return {};
    }
}
