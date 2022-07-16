#include "compile.cpp"
#include "prepp.hpp"

int main(int argc, char** argv) {
	argparse::ArgumentParser prepp(
		"pre++", VERSION, argparse::default_arguments::version);
	prepp.add_argument("--run", "-r")
		.nargs(1)
		.help("Run a pre++ exercize")
		.action([&](const std::string& ex) {
			check(std::get<0>(read_toml(ex)),
				  std::get<1>(read_toml(ex)),
				  std::get<3>(read_toml(ex)));
		});

	prepp.add_argument("--hint")
		.help("Run pre++ in watch mode.")
		.nargs(1)
		.action([&](const std::string& ex) {
			hint(std::get<2>(read_toml(ex)));
		});

	try {
		prepp.parse_args(argc, argv);
	} catch (const std::runtime_error& err) {
		fmt::print(std::cerr, "{} {}", err.what(), prepp);
		std::exit(2);
	}

	return 0;
}
