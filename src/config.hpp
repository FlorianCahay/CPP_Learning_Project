#pragma once

#include "img/media_path.hpp"
#include "geometry.hpp"

#include <sstream>
#include <stdexcept>


const MediaPath one_lane_airport_sprite_path = { "airport_1lane.png" };
const MediaPath two_lane_airport_sprite_path = { "airport_2lane.png" };

// number of cycles needed to service an aircraft at a terminal
constexpr unsigned int SERVICE_CYCLES = 40u;
// speeds below the threshold speed loose altitude linearly
constexpr float SPEED_THRESHOLD = 0.05f;
// this models the speed with wich slow (speed < SPEED_THRESHOLD) aircrafts sink
constexpr float SINK_FACTOR = 0.1f;
// distances below this distance are considered equal (planes crash, waypoints
// are reached, etc)
constexpr float DISTANCE_THRESHOLD = 0.05f;
// each aircraft sprite has 8 tiles
constexpr unsigned char NUM_AIRCRAFT_TILES = 8;
// size of the plane-sprite on screen
constexpr float PLANE_TEXTURE_DIM = 0.2f;
// default number of ticks per second
constexpr unsigned int DEFAULT_TICKS_PER_SEC = 16u;
// default zoom factor
constexpr float DEFAULT_ZOOM = 2.0f;
// default window dimensions
constexpr size_t DEFAULT_WINDOW_WIDTH  = 800;
constexpr size_t DEFAULT_WINDOW_HEIGHT = 600;
// max fuel in one aircraft
constexpr unsigned short MAX_FUEL = 3000;
// number of different airlines
constexpr unsigned char NB_AIRLINES = 8;

//using AircraftCrash = std::runtime_error;
class AircraftCrash : public std::runtime_error {
    private:
        static std::string message(const std::string flight_number, const Point3D pos, const Point3D speed, const std::string cause) {
            std::string msg;
            msg += "Plane " + flight_number ;
            msg += " crashed at position (" + std::to_string(pos.x()) + ", " + std::to_string(pos.y()) + ", " + std::to_string(pos.z());
            msg += ") and speed ("+ std::to_string(speed.x()) + ", " + std::to_string(speed.y()) + ", " + std::to_string(speed.z()) + ")  because : ";
            msg +=  cause + ".";
            return msg.c_str();
        }

    public:
        AircraftCrash(const std::string flight_number, const Point3D pos, const Point3D speed, const std::string cause) : 
            std::runtime_error { message(flight_number, pos, speed, cause) }
        { };

        
};