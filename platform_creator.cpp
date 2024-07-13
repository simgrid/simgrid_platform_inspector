#include <simgrid/s4u.hpp>
#include <iostream>

namespace sg4 = simgrid::s4u;
extern "C" void load_platform(const sg4::Engine &e);
void load_platform(const sg4::Engine &e) {
    auto world = sg4::create_full_zone("world");
    auto WIFI_zone = sg4::create_wifi_zone("WIFI zone");
    WIFI_zone->set_parent(world);
    WIFI_zone->set_property("access_point", "WIFI router");
    auto Station_1 = WIFI_zone->create_host("Station 1", {"100.0Mf","50.0Mf","20.0Mf"});
    Station_1->set_core_count(1);
    auto Station_2 = WIFI_zone->create_host("Station 2", {"100.0Mf","50.0Mf","20.0Mf"});
    Station_2->set_core_count(1);
    auto AP1 = WIFI_zone->create_link("AP1", {"54Mbps","36Mbps","24Mbps"})->set_latency("0ms");
    std::cerr << "HERE1\n";
    AP1->set_sharing_policy(simgrid::s4u::Link::SharingPolicy::WIFI);
    std::cerr << "HERE2\n";
    AP1->seal();
    auto WIFI_router = WIFI_zone->create_router("WIFI router");
    auto Wired_zone = sg4::create_full_zone("Wired zone");
    Wired_zone->set_parent(WIFI_zone);
    auto node1 = Wired_zone->create_host("node1", {"100.0Mf","50.0Mf","20.0Mf"});
    node1->set_core_count(1);
    Wired_zone->set_gateway(node1->get_netpoint());
    Wired_zone->seal();
    auto Collector = WIFI_zone->create_link("Collector", "100Mbps")->set_latency("0ms");
    Collector->set_sharing_policy(simgrid::s4u::Link::SharingPolicy::SHARED);
    Collector->seal();
    WIFI_zone->add_route(WIFI_zone, Wired_zone, {Collector});
    WIFI_zone->seal();
    world->seal();
};