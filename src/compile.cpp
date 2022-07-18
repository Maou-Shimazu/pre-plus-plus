#include "prepp.hpp"
#include <FileWatcher/FileWatcher.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <toml++/impl/array.h>

std::tuple<std::optional<std::string>, std::optional<std::string>, std::string>
read_toml(std::string ex) {
	toml::table exercises = toml::parse_file("info.toml");
	if (!exercises[ex].is_table()) {
		fmt::print(fg(fmt::color::red), "Exercise `{}` dosent exist", ex);
		std::exit(2);
	}

	return {exercises[ex]["path"].value<std::string>(),
			exercises[ex]["hint"].value<std::string>(),
			ex};
}

bool done(std::optional<std::string> path) {
	std::fstream f;
	f.open(fs::absolute(path.value()), std::ios_base::in);
	std::string word = "I AM NOT DONE", line;
	bool done		 = false;
	while (getline(f, line)) {
		size_t pos = line.find(word);
		if (pos != std::string::npos) 
			done = true;
	}
	return done;
}

bool is_complete(std::string path) {
	std::ifstream file;
	std::string word = "COMPLETE", line;
	bool contains;

	file.open(path, std::ios_base::app | std::ios_base::in);
	while (getline(file, line)) {
		size_t pos = line.find(word);
		if (pos != std::string::npos)
			contains = true;
	}

	std::string text = "\n// COMPLETE";
	while (file.is_open())
		if (!contains) {
			std::ofstream f;
			f.open(path, std::ios_base::app);
			f.write(text.data(), static_cast<int>(text.size()));
			fmt::print(fg(fmt::color::lime_green),
					   "\n✔ This Exercise is complete! Move on to the next exercise.");

			file.close();
			f.close();
			return true;
		} else if (contains) {
			fmt::print(fg(fmt::color::lime_green),
					   "\n✔ This Exercise is complete! Move on to the next exercise. If you wish to continue, remove \"// COMPLETE\" from your file.\n");
			file.close();
			return true;
		}
	return false;
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
				   "\n! Compilation of {} failed! Compiler Error message: \n",
				   ex);
		fmt::print("{}\n", result);
		std::exit(2);
	} else {
		fs::remove(fmt::format("{}.exe", path.substr(0, path.size() - 3)));
		if (is_complete(path))
			std::exit(0);
		fmt::print("\n{}\n", result);
		fmt::print(
			fg(fmt::color::lime_green), "✔ Sucessfully ran exercise: {}", ex);
	}
}

void check(std::optional<std::string> path, std::string ex) {
	if (done(path)) {
		fmt::print(fg(fmt::color::dark_cyan), "\nCompiling... {}", ex);
		compile_and_run(fs::absolute(path.value()).string(), ex);
	} else if (!done(path)) {
		fmt::print(fg(fmt::color::lime_green),
				   "✔ {} is complete! If you wish to continue then put \"// I "
				   "AM NOT DONE \" In your file.",
				   ex);
	}
}

void hint(std::optional<std::string> hint) {
	fmt::print("{}\n", hint.value());
}