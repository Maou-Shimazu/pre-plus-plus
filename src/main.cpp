#include "compile.cpp"
#include "prepp.hpp"

int main(int argc, char** argv) {
	argparse::ArgumentParser prepp(
		"pre++", VERSION, argparse::default_arguments::version);
	prepp.add_argument("--run", "-r")
		.nargs(1)
		.help("Run a pre++ exercize")
		.action([](const std::string& ex) { read_toml(ex); });

	try {
		prepp.parse_args(argc, argv);
	} catch (const std::runtime_error& err) {
		std::cerr << err.what() << std::endl;
		std::cerr << prepp;
		std::exit(2);
	}

	return 0;
}
