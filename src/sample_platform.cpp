#include <simgrid/s4u.hpp>

namespace sg4 = simgrid::s4u;

extern "C" void load_platform(const sg4::Engine &e);
void load_platform(const sg4::Engine &e)
{

    // Get the root zone (automatically created by the engine)
    auto* zone_world = sg4::Engine::get_instance()->get_netzone_root();
    auto* zw_l = zone_world->add_link("Internet", "100GBps")->set_latency("100ms")->seal();

    // Create Zone1
    auto* zone1 = zone_world->add_netzone_full("Zone1");
    auto* z1_h1 = zone1->add_host("H1.1", "10Gf");
    z1_h1->set_core_count(10)->seal();
    zone1->set_gateway(z1_h1->get_netpoint());
    zone1->seal();

    // Create Zone2
    auto* zone2 = zone_world->add_netzone_full("Zone2");
    auto* z2_h1 = zone2->add_host("H2.1", "10Gf");
    z2_h1->set_property("key", "12");
    z2_h1->set_property("stuff", "hello");
    auto* z2_h1_d1 = z2_h1->add_disk("mydisk", "100Mbps", "100Mbps");
    z2_h1_d1->set_property("whatever", "hello")->seal();
    z2_h1->set_core_count(10)->seal();
    auto* z2_h2 = zone2->add_host("H2.2", "10Gf");
    z2_h2->set_core_count(20)->seal();
    auto* z2_l = zone2->add_link("L2.1", "100MBps")->set_latency("1ns")->seal();
    zone2->add_route(z2_h1, z2_h2, {z2_l});
    auto* zone2_router = zone2->add_router("some_router");
    zone2->set_gateway(z2_h1->get_netpoint());
    zone2->seal();

    zone_world->add_route(zone1, zone2, {zw_l});
    zone_world->seal();
}


