#include "aircraft.hpp"
#include "GL/opengl_interface.hpp"


void Aircraft::turn_to_waypoint() {
    if (!waypoints.empty()) {
        Point3D target = waypoints[0];
        if (waypoints.size() > 1) {
            const float d   = (waypoints[0] - pos).length();
            const Point3D W = (waypoints[0] - waypoints[1]).normalize(d / 2.0f);
            target += W;
        }

        turn(target - pos - speed);
    }
}

void Aircraft::turn(Point3D direction) {
    (speed += direction.cap_length(type.max_accel)).cap_length(max_speed());
}

unsigned int Aircraft::get_speed_octant() const
{
    const float speed_len = speed.length();
    if (speed_len > 0) {
        const Point3D norm_speed { speed * (1.0f / speed_len) };
        const float angle =
            (norm_speed.y() > 0) ? 2.0f * 3.141592f - std::acos(norm_speed.x()) : std::acos(norm_speed.x());
        // partition into NUM_AIRCRAFT_TILES equal pieces
        return (static_cast<int>(std::round((angle * NUM_AIRCRAFT_TILES) / (2.0f * 3.141592f))) + 1) %
               NUM_AIRCRAFT_TILES;
    } else {
        return 0;
    }
}

// when we arrive at a terminal, signal the tower
void Aircraft::arrive_at_terminal() {
    // we arrived at a terminal, so start servicing
    control.arrived_at_terminal(*this);
    is_at_terminal = true;
}

// deploy and retract landing gear depending on next waypoints
void Aircraft::operate_landing_gear() {
    if (waypoints.size() > 1u) {
        const auto it            = waypoints.begin();
        const bool ground_before = it->is_on_ground();
        const bool ground_after  = std::next(it)->is_on_ground();
        // deploy/retract landing gear when landing/lifting-off
        if (ground_before && !ground_after) {
            std::cout << flight_number << " lift off" << std::endl;
        }
        else if (!ground_before && ground_after) {
            std::cout << flight_number << " is now landing..." << std::endl;
            landing_gear_deployed = true;
        }
        else if (!ground_before && !ground_after) {
            landing_gear_deployed = false;
        }
    }
}

template <bool front>
void Aircraft::add_waypoint(const Waypoint& wp) {
    if constexpr (front) {
        waypoints.push_front(wp);
    } else {
        waypoints.push_back(wp);
    }
}

bool Aircraft::update() {
    if (waypoints.empty()) { // si l'avion est en attente d'instructions
        if (is_service_done) { // si l'avion a déjà effectué son service dans un terminal
            return false; // on supprime l'avion
        }

        const auto front = false;
        for (const auto& wp: control.get_instructions(*this)) {
            add_waypoint<front>(wp);
        } 
    }

    if (is_circling()) { // si l'avion n'a toujours pas de terminal réservé
        const auto path = control.reserve_terminal(*this);
        if (!path.empty()) {
            waypoints.clear(); // on le redirige directement vers le terminal sans attendre la fin de son tour
            for (const auto wp : path) {
                add_waypoint<false>(wp);
            }
        }
    }

    // l'avion n'est pas au terminal
    if (!is_at_terminal) {
        turn_to_waypoint();
        // move in the direction of the current speed
        pos += speed;

        // if we are close to our next waypoint, stike if off the list
        if (!waypoints.empty() && distance_to(waypoints.front()) < DISTANCE_THRESHOLD) {
            if (waypoints.front().is_at_terminal()) {
                arrive_at_terminal();
            }
            else {
                operate_landing_gear();
            }
            waypoints.pop_front();
        }

        if (is_on_ground()) { // l'avion est au sol
            if (!landing_gear_deployed) {
                //using namespace std::string_literals;
                throw AircraftCrash { flight_number, pos, speed, "bad landing" };
            }
        } else { // l'avion est en vol
            // if we are in the air, but too slow, then we will sink!
            const float speed_len = speed.length();
            if (speed_len < SPEED_THRESHOLD) {
                pos.z() -= SINK_FACTOR * (SPEED_THRESHOLD - speed_len);
            }

            fuel--;
            if (fuel <= 0) { // s'il n'y a plus de carburant dans l'avion
                throw AircraftCrash { flight_number, pos, speed, "out of fuel" };
            }
        }

        // update the z-value of the displayable structure
        GL::Displayable::z = pos.x() + pos.y();
    }
    return true;
}

void Aircraft::display() const {
    type.texture.draw(project_2D(pos), { PLANE_TEXTURE_DIM, PLANE_TEXTURE_DIM }, get_speed_octant());
}

void Aircraft::refill(int& fuel_stock) {
    if (fuel_stock == 0) {
        return;
    }
    auto stock = 0;
    auto need = get_required_fuel();
    auto result = fuel_stock - need;
    if (result < 0) {
        stock = fuel_stock;
        fuel += fuel_stock;
        fuel_stock = 0;
    } else {
        stock = need;
        fuel = MAX_FUEL;
        fuel_stock -= need;
    }
    std::cout << "The tank of the " << flight_number << " aircraft has been filled with " << stock << " liters." << std::endl;
}

bool Aircraft::has_terminal() const {
    if (waypoints.empty()) {
        return is_at_terminal;
    }
    return waypoints.back().is_at_terminal();
}