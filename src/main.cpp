#include <iostream>
#include <simgrid/s4u.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace sg4 = simgrid::s4u;

int main(int argc, char **argv) {

    std::string hostnames;
    std::string path_to_so_file;
    po::options_description desc("Usage");
    desc.add_options()
            ("help",
             "Show this help message\n")
            ("platform", po::value<std::string>(&path_to_so_file)->required()->value_name("<path to a .so or .xml file>"),
             "path to a .so file that implements the load_platform() function\n")
            ("show_hosts",
             "Show name and information for all the hosts in the platform\n")
            ("show_disks",
             "Show name and information for all the disks in the platform\n")
            ("show_links",
             "Show name and information for all the links platform\n")
            ("show_routes", po::value<std::string>(&hostnames)->value_name("<comma-separated list of host names>"),
             "Show routes between all specified hosts\n")
            ;

    // Parse command-line arguments
    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        // Print help message and exit if needed
        if (vm.count("help")) {
            std::cerr << desc;
            std::cerr << "Example: " << argv[0] << " --platform ./sample_platform.[so|xml] --show_hosts\n\n";
            exit(0);
        }
        // Throw whatever exception in case argument values are erroneous
        po::notify(vm);
    } catch (std::exception &e) {
        std::cerr << "Invalid arguments!" << "\n\n";
        std::cerr << desc;
        std::cerr << "Example: " << argv[0] << " --platform ./sample_platform.[so|xml] --show_hosts\n\n";
        exit(1);
    }

    sg4::Engine::get_instance()->load_platform(path_to_so_file);
    auto num_hosts = sg4::Engine::get_instance()->get_all_hosts().size();
    std::cerr << "Platform successfully created\n";

    if (vm.count("show_hosts")) {
        std::cout << "Hosts:\n";
        std::vector<simgrid::s4u::Host *> host_list = simgrid::s4u::Engine::get_instance()->get_all_hosts();
        for (auto const &h: host_list) {
            std::cout << "  - Host: " << h->get_name() << "(#cores=" << h->get_core_count() << ", speed=" << h->get_speed() << " fps)\n";
            if (not h->get_properties()->empty()) {
                std::cout << "    - Properties:\n";
                for (auto const &p: *h->get_properties()) {
                    std::cout << "       - " << p.first << ": " << p.second << "\n";
                }
            }
        }
    }

    if (vm.count("show_links")) {
        std::cout << "Links:\n";
        std::vector<simgrid::s4u::Link *> link_list = simgrid::s4u::Engine::get_instance()->get_all_links();
        for (auto const &l: link_list) {
            std::cout << "  - Link: " << l->get_name() << "(bandwidth=" << l->get_bandwidth() << " bps, latency=" << l->get_latency() << " s)\n";
            if (not l->get_properties()->empty()) {
                std::cout << "    - Properties:\n";
                for (auto const &p: *l->get_properties()) {
                    std::cout << "       - " << p.first << ": " << p.second << "\n";
                }
            }
        }
    }


    if (vm.count("show_disks")) {
        std::cout << "Disks:\n";
        std::vector<std::string> tokens;
        std::vector<simgrid::s4u::Host *> host_list = simgrid::s4u::Engine::get_instance()->get_all_hosts();
        for (auto const &h: host_list) {
            std::vector<simgrid::s4u::Disk *> disk_list = h->get_disks();
            for (auto const &d: disk_list) {
            	std::cout << "  - Disk: " << d->get_name() << " (host: " << h->get_name() << ", read bandwidth=" << d->get_read_bandwidth() << " bps, write bandwidth=" << d->get_write_bandwidth() << " bps)\n";
                if (not d->get_properties()->empty()) {
                    std::cout << "    - Properties:\n";
                    for (auto const &p: *d->get_properties()) {
                        std::cout << "       - " << p.first << ": " << p.second << "\n";
                    }
                }
            }
        }
    }


    if (vm.count("show_routes")) {
        std::vector<std::string> tokens;
        if (hostnames == "all") {
            std::vector<simgrid::s4u::Host *> host_list = simgrid::s4u::Engine::get_instance()->get_all_hosts();
            std::vector<std::string> hostname_list;
            for (auto h: host_list) {
                tokens.push_back(h->get_name());
            }
        } else {
            std::stringstream ss(hostnames);
            std::string item;
            while (getline(ss, item, ',')) {
                tokens.push_back(item);
            }
        }
        for (int i=0; i < tokens.size(); i++) {
            for (int j=i+1; j < tokens.size(); j++) {
                auto h1 = tokens.at(i);
                auto h2 = tokens.at(j);
                if (h1 != h2) {
                    auto host1 = simgrid::s4u::Host::by_name_or_null(h1);
                    auto host2 = simgrid::s4u::Host::by_name_or_null(h2);
                    if (host1 == nullptr) {
                        std::cerr << "Unknown host: " << h1 << "\n";
                        exit(1);
                    }
                    if (host2 == nullptr) {
                        std::cerr << "Unknown host: " << h2 << "\n";
                        exit(1);
                    }
                    std::vector<simgrid::s4u::Link*> links;
                    double latency = 0.0;
                    host1->route_to(host2, links, &latency);
                    std::cout << "Route from " << h1 << " to " << h2 << ":\n";
                    for (const auto &link : links) {
                        std::cout << "  Link: " << link->get_name() << "\n";
                    }
                    std::cout << "  Total latency: " << latency << " (sec)\n";
                }
            }
        }
    }

    //sg4::Actor::create("TestActor", simgrid::s4u::Host::by_name("host602"), []() {
        //auto host = simgrid::s4u::Host::by_name("host602");
        //std::cerr << "ACTOR!\n";
        //host->set_pstate(0);
        //std::cerr << "ACTOR!\n";
    //});
//
    //simgrid::s4u::Engine::get_instance()->run();




    return 0;
}

