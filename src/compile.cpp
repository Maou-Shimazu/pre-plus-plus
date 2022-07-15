#include "prepp.hpp"
#include <glob/glob.hpp>

void glob_exercizes() {
	std::vector<std::string> files;
}

void compile() {
	auto [ex_code, sys_ex] = exec(
		R"(clang++ C:\Users\Maou\Documents\pre-plus-plus\include\ex1\main.cpp -o
    C:\Users\Maou\Documents\pre-plus-plus\include\ex1\main 2>&1 &&
    C:\Users\Maou\Documents\pre-plus-plus\include\ex1\main.exe 2>&1)");
	std::cout << "ex_code: " << ex_code << std::endl;
	std::cout << "sys_ex: " << sys_ex << std::endl;
}