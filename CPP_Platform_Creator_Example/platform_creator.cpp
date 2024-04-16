#include <simgrid/s4u.hpp>

namespace sg4 = simgrid::s4u;

extern "C" void load_platform(const sg4::Engine &e);
void load_platform(const sg4::Engine &e)
{

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
    z2_h1->set_property("key", "12");
    z2_h1->set_property("stuff", "hello");
    auto z2_h1_d1 = z2_h1->create_disk("mydisk", "100Mbps", "100Mbps");
    z2_h1_d1->set_property("whatever", "hello");
    z2_h1->set_core_count(10)->seal();
    auto z2_h2 = zone2->create_host("H2.2", "10Gf");
    z2_h2->set_core_count(20)->seal();
    auto z2_l = zone2->create_link("L2.1", "100MBps")->set_latency("1ns")->seal();
    zone2->add_route(z2_h1, z2_h2, {z2_l});
    auto zone2_router = zone2->create_router("some_router");
    zone2->set_gateway(z2_h1->get_netpoint());
    zone2->seal();

    // Create Zone3 (a Cluster WITHOUT a backbone)
    // <cluster id="datacenter" prefix="c-"
    //                          suffix=".me"
    //                          radical="0-9"
    //                          speed="1Gf"
    //                          bw="125MBps"
    //                          lat="50us"
    //                          router_id="router1"/>
        auto zone3 = sg4::create_star_zone("Zone3");
    {
        zone3->set_parent(zone_world);
        auto router = zone3->create_router("router1");
        zone3->set_gateway(router);
        // If no 'router_id' spec in the xml, the router name
        // is ${prefix}${cluster_id}_router${suffix}
        //  ("c-datacenter_router.me")
        for (int i = 0; i < 10; i++) {
            std::string hostname = "c-" + std::to_string(i) + ".me";
            auto host = zone3->create_host(hostname, "1Gf");
            auto link = zone3->create_link(hostname + "-link", "125MBps")->set_latency("50us");
            sg4::LinkInRoute my_link(link);
            zone3->add_route(host, nullptr, {my_link}, true);
        }
    }
        zone3->seal();

    // Create Zone4 (a Cluster WITH a backbone)
    // <cluster id="datacenter" prefix="d-"
    //                          suffix=".me"
    //                          radical="0-9"
    //                          speed="1Gf"
    //                          bw="125MBps"
    //                          lat="50us"
    //                          bb_bw="10GBps"
    //                          bb_lat="100us"/>
    auto zone4 = sg4::create_star_zone("Zone4");
    {
        zone4->set_parent(zone_world);
        auto router = zone4->create_router("c-datacenter_router.me");
        auto l_bb = zone4->create_link("backbone.me", "10Gbps")->set_latency("100us");
        for (int i = 0; i < 10; i++) {
            std::string hostname = "d-" + std::to_string(i) + ".me";
            auto host = zone4->create_host(hostname, "1Gf");
            auto link = zone4->create_link(hostname + "-link", "125MBps")->set_latency("50us");
            sg4::LinkInRoute my_link(link);
            sg4::LinkInRoute bb_link(l_bb);
            zone4->add_route(host, nullptr, {my_link, bb_link}, true);
        }
        zone4->set_gateway(router);
        zone4->seal();
    }




    zone_world->add_route(zone1, zone2, {zw_l});
    zone_world->add_route(zone1, zone4, {zw_l});
    zone_world->add_route(zone2, zone4, {zw_l});
    zone_world->add_route(zone1, zone3, {zw_l});
    zone_world->add_route(zone3, zone4, {zw_l});
    zone_world->add_route(zone2, zone3, {zw_l});


    zone_world->seal();
}
