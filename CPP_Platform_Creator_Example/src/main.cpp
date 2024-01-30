#include <iostream>
#include <simgrid/s4u/Engine.hpp>
#include <simgrid/s4u/Host.hpp>
#include <boost/program_options.hpp>

#include "PlatformCreator.hpp"

namespace po = boost::program_options;
namespace sg4 = simgrid::s4u;

int main(int argc, char **argv) {

    std::string hostnames;
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help",
             "Show this help message\n")
            ("show_routes", po::value<std::string>(&hostnames)->value_name("<list of host names>"),
             "comma-separated list of hostnames (no white space)\n")
            ;

    // Parse command-line arguments
    po::variables_map vm;
    po::store(
            po::parse_command_line(argc, argv, desc),
            vm
    );

    try {
        // Print help message and exit if needed
        if (vm.count("help")) {
            std::cerr << desc << "\n";
            exit(0);
        }
        // Throw whatever exception in case argument values are erroneous
        po::notify(vm);
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        exit(1);
    }

    PlatformCreator::create_platform();
    auto num_hosts = sg4::Engine::get_instance()->get_all_hosts().size();
    std::cerr << "Created a platform with " << num_hosts << " hosts\n";

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

    return 0;
}

