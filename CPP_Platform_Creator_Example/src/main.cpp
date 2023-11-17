#include <iostream>
#include <simgrid/s4u/Engine.hpp>
#include <simgrid/s4u/Host.hpp>

namespace sg4 = simgrid::s4u;

class PlatformCreator {

public:
    static void create_platform() {

        // Create the top-level zone
        auto zone_world = sg4::create_full_zone("World");
        auto zw_l = zone_world->create_link("Internet", "100GBps")->set_latency("100ms");

        // Create Zone1
        auto zone1 = sg4::create_full_zone("Zone1");
        zone1->set_parent(zone_world);
        auto z1_h1 = zone1->create_host("H1.1", "10Gf");
        z1_h1->set_core_count(10)->seal();
        zone1->set_gateway(z1_h1->get_netpoint());
        zone1->seal();

        // Create Zone2
        auto zone2 = sg4::create_full_zone("Zone2");
        zone2->set_parent(zone_world);
        auto z2_h1 = zone2->create_host("H2.1", "10Gf");
        auto z2_h1_d1 = z2_h1->create_disk("mydisk", "100Mbps", "100Mbps");
        z2_h1->set_core_count(10)->seal();
        auto z2_h2 = zone2->create_host("H2.2", "10Gf");
        z2_h2->set_core_count(20)->seal();
        auto z2_l = zone2->create_link("L2.1", "100MBps")->set_latency("1ns")->seal();
        zone2->add_route(z2_h1, z2_h2, {z2_l});
        auto zone2_router = zone2->add_router("some_router");
        zone2->set_gateway(z2_h1->get_netpoint());
        zone2->seal();

        zone_world->add_route(zone1, zone2, {zw_l});
        zone_world->seal();
    }
};

int main(int argc, char **argv) {
    PlatformCreator::create_platform();
    auto num_hosts = sg4::Engine::get_instance()->get_all_hosts().size();
    std::cerr << "Created a platform with " << num_hosts << " hosts\n";
    return 0;
}

