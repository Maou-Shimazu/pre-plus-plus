#include "prepp.hpp"
#include <tuple>

int main() {
	std::string ex_code;
	int sys_ex;
	std::tie(ex_code, sys_ex) = exec(R"(clang++ 2>&1)");
	std::cout << "ex_code: " << ex_code << std::endl;
	std::cout << "sys_ex: " << sys_ex << std::endl;
	return 0;
}
