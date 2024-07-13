# SimGrid platform inspector

This is a simple command-line tool that can be used to find out information about a SimGrid platform description (provided as a .so file or a .xml file), with the following usage:

```bash
./simgrid_platform_inspector --platform ../small_platform.xml --show_hosts --help       
Usage:
  --help                                Show this help message
                                        
  --platform <path to a .so or .xml file>
                                        path to a .so file that implements the 
                                        load_platform() function
                                        
  --show_hosts                          Show name and information for all the 
                                        hosts in the platform
                                        
  --show_disks                          Show name and information for all the 
                                        disks in the platform
                                        
  --show_links                          Show name and information for all the 
                                        links platform
                                        
  --show_routes <comma-separated list of host names>
                                        Show routes between all specified hosts
```


