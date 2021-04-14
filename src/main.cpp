#include "tower_sim.hpp"

int main(int argc, char** argv)
{
    TowerSimulation simulation { argc, argv };
    simulation.launch("aircraft_type.txt");

    return 0;
}