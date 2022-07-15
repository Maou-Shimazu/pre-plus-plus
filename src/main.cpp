#include "prepp.hpp"

int main(int argc, char** argv) {
	// Note:(s)
	// -[ ] Alright this works, returns the error in ex_code. with this, we need
	// to glob the files and deal with the issue of absolute paths.
	// -[ ] set a check if the exit from the first globbed file is 0, if it
	// isnt, keep running that file untill it is. otherwise find a way to record
	// it being completed.
	// -[ ] check if rustlings removes executables or compiles them to a
	// specific place.

	argparse::ArgumentParser prepp("pre++", VERSION, argparse::default_arguments::version);


	try {
		prepp.parse_args(argc, argv);
	}
	catch (const std::runtime_error& err) {
		std::cerr << err.what() << std::endl;
		std::cerr << prepp;
		std::exit(2);
	}

	// auto [ex_code, sys_ex] = exec(
	// 	R"(clang++ C:\Users\Maou\Documents\pre-plus-plus\include\ex1\main.cpp -o
	// C:\Users\Maou\Documents\pre-plus-plus\include\ex1\main 2>&1 &&
	// C:\Users\Maou\Documents\pre-plus-plus\include\ex1\main.exe 2>&1)");
	// std::cout << "ex_code: " << ex_code << std::endl;
	// std::cout << "sys_ex: " << sys_ex << std::endl;
	return 0;
}
