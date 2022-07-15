#include "prepp.hpp"
#include <filesystem>

std::tuple<std::optional<std::string>,
		   std::optional<std::string>,
		   std::optional<std::string>>
read_toml(std::string ex) {
	toml::table exercises = toml::parse_file("info.toml");
	if (!exercises[ex].is_table()) {
		fmt::print(std::cerr, "Exercize {} dosent exist", ex);
		exit(1);
	}

	return {exercises[ex]["path"].value<std::string>(),
			exercises[ex]["mode"].value<std::string>(),
			exercises[ex]["hint"].value<std::string>()};
}

void compile_and_run(std::string path) {
	auto [result, exit_code] =
		exec(fmt::format("clang++ {0} -o {1}.exe 2>&1 && {1}.exe 2>&1",
						 path,
						 path.substr(0, path.size() - 3))
				 .c_str());
	fmt::print("Result: {}", result);
	fmt::print("Exit code: {}", exit_code);
}

void check(std::optional<std::string> path, std::optional<std::string> mode) {
	std::fstream file(fs::absolute(path.value()));

	std::string word = "I AM NOT DONE";
	std::string line;
	bool not_done = false;

	while (getline(file, line)) {
		size_t pos = line.find(word);
		if (pos != std::string::npos)
			not_done = true;
	}

	if (mode.value() == "compile" && not_done)
		compile_and_run(fs::absolute(path.value()).string());
}