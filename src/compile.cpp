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
		auto [ex_code, sys_ex] =
			exec(fmt::format("clang++ {0} -o {1}.exe 2>&1 && {1}.exe 2>&1",
							 fs::absolute(i).string(),
							 fs::absolute(i).string().substr(
								 0, fs::absolute(i).string().size() - 3))
					 .c_str());
		std::cout << "ex_code: " << ex_code << std::endl;
		std::cout << "sys_ex: " << sys_ex << std::endl;
	}
}