#include "prepp.hpp"

#ifdef __unix__
std::vector<std::string> glob_exercizes() {
	std::vector<std::string> files;
	auto glob = glob::rglob({"exercizes/*.hpp", "exercizes/*.cpp"});
	std::for_each(glob.begin(), glob.end(), [&files](fs::path& p) {
		files.push_back(p.string());
	});
	return files;
}
#elifdef __WIN32__
std::vector<std::string> glob_exercizes() {
	std::vector<fs::path> fs_files = cppglob::glob("exercizes/**/*.cpp", true);
	std::vector<std::string> files;
	std::for_each(fs_files.begin(), fs_files.end(), [&files](fs::path& p) {
		files.push_back(p.string());
	});
	return files;
}
#endif

void compile(std::vector<std::string> files) {
	for (auto& i : files) {
		std::string abs = fs::absolute(i).string();
		std::string result;
		int exit_code;
		std::thread([&abs, &result, &exit_code]() {
			std::tie(result, exit_code) =
				exec(fmt::format("clang++ {0} -o {1}.exe 2>&1 && {1}.exe 2>&1",
								 abs,
								 abs.substr(0, abs.size() - 3))
						 .c_str()); // add unix specific execution
		}).join();
		std::cout << "ex_code: " << result << std::endl;
		std::cout << "sys_ex: " << exit_code << std::endl;
	}
}

void watch_current_ex(fs::path current_ex) {
	std::fstream oc(current_ex, std::ifstream::in);
	std::string original_content((std::istreambuf_iterator<char>(oc)),
								 std::istreambuf_iterator<char>());
	while (true) {
		std::fstream nc(current_ex, std::ifstream::in);
		std::string new_content((std::istreambuf_iterator<char>(nc)),
								std::istreambuf_iterator<char>());
		if (original_content != new_content) {
            original_content = new_content;
			compile(glob_exercizes());
			// break;
		}
	}
}
