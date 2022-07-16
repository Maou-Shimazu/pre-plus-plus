#include "prepp.hpp"
#include <FileWatcher/FileWatcher.h>
#include <filesystem>
#include <string>
#include <string_view>
#include <toml++/impl/array.h>

std::tuple<std::optional<std::string>,
		   std::optional<std::string>,
		   std::optional<std::string>,
		   std::string>
read_toml(std::string ex) {
	toml::table exercises = toml::parse_file("info.toml");
	if (!exercises[ex].is_table()) {
		fmt::print(fg(fmt::color::red), "Exercise `{}` dosent exist", ex);
		std::exit(2);
	}

	return {exercises[ex]["path"].value<std::string>(),
			exercises[ex]["mode"].value<std::string>(),
			exercises[ex]["hint"].value<std::string>(),
			ex};
}

void compile_and_run(
	std::string path,
	std::string ex) { /// todo: update toml value mode from compile to complete
					  /// when done, do when implimenting watch
	auto [result, exit_code] =
		exec(fmt::format("clang++ {0} -o {1}.exe 2>&1 && {1}.exe 2>&1",
						 path,
						 path.substr(0, path.size() - 3))
				 .c_str());
	if (exit_code != 0) {
		fmt::print(fg(fmt::color::red),
				   "! Compilation of {} failed! Compiler Error message: \n",
				   ex);
		fmt::print("{}\n", result);
		std::exit(2);
	} else {
		fmt::print("{}\n", result);
		fmt::print(
			fg(fmt::color::lime_green), "✔ Sucessfully ran exercise: {}", ex);
		fs::remove(fmt::format("{}.exe", path.substr(0, path.size() - 3)));
	}
}

bool done(std::optional<std::string> path) {
	std::fstream f(fs::absolute(path.value()));
	std::string word = "I AM NOT DONE", line;
	bool done		 = false;

	while (getline(f, line)) {
		size_t pos = line.find(word);
		if (pos != std::string::npos)
			done = true;
	}
	return done;
}

void compile_to_complete() {}

void check(std::optional<std::string> path,
		   std::optional<std::string> mode,
		   std::string ex) {
	std::fstream f(fs::absolute(path.value()));

	if (mode.value() == "compile" && !done(path))
		compile_and_run(fs::absolute(path.value()).string(), ex);

	if (mode.value() == "compile" && !done(path)) {
		toml::table exercises = toml::parse_file("info.toml");
		if (!exercises[ex].is_table()) {
			fmt::print(fg(fmt::color::red), "Exercise `{}` dosent exist", ex);
			std::exit(2);
		}
		exercises.insert(ex, toml::array{
			"mode = complete"
		});

		std::cout << "\n\n" << exercises << std::endl;
	}
}

void hint(std::optional<std::string> hint) {
	fmt::print("{}\n", hint.value());
}