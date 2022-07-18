#include "compile.cpp"
#include "prepp.hpp"

int main(int argc, char** argv) {
	argparse::ArgumentParser prepp(
		"pre++", VERSION, argparse::default_arguments::all);
	prepp.add_argument("--run", "-r")
		.nargs(1)
		.help("Run a pre++ exercise")	
		.action([&](const std::string& ex) {
			check(std::get<0>(read_toml(ex)), std::get<2>(read_toml(ex)));
		});

	prepp.add_argument("--hint", "-ht")
		.help("Get a hint about an+ exercise.")
		.nargs(1)
		.action(
			[&](const std::string& ex) { hint(std::get<1>(read_toml(ex))); });

	try {
		prepp.parse_args(argc, argv);
	} catch (const std::runtime_error& err) {
		fmt::print(std::cerr, "{} {}", err.what(), prepp);
		std::exit(2);
	}

	return 0;
}
