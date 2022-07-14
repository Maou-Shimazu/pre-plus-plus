#pragma once

#ifndef PRE_PLUS_PLUS_HPP
#define PRE_PLUS_PLUS_HPP

#include <iostream>
#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple> 

std::tuple<std::string, int> exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
    auto rc = pclose(pipe.get());
	return std::make_tuple(result, rc);
}

#endif