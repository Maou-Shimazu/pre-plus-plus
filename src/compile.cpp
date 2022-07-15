#include "prepp.hpp"

void read_toml(std::string ex){
	toml::table exercises = toml::parse_file("info.toml");
	if(!exercises[ex].is_table())
		fmt::print(std::cerr, "Exercize {} is not a table", ex); 
	
	std::cout << exercises[ex]["path"] << std::endl;
	std::cout << exercises[ex]["mode"] << std::endl;
	std::cout << exercises[ex]["hint"] << std::endl;
	
}