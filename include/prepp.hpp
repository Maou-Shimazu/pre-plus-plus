#pragma once

#ifndef PRE_PLUS_PLUS_HPP
#define PRE_PLUS_PLUS_HPP

#include <algorithm>
#include <argparse/argparse.hpp>
#include <array>
#include <cstdio>
#include <filesystem>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fstream>
#include <glob/glob.hpp>
#include <iostream>
#include <memory>
#include <src/format.cc>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#ifdef __WIN32__
#include <cppglob/fnmatch.cpp>
#include <cppglob/glob.cpp>
#include <cppglob/glob.hpp>
#include <cppglob/iglob.hpp>
#endif

namespace fs = std::filesystem;

std::pair<std::string, int> exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	auto exit_code = pclose(pipe.get());
	return std::make_pair(result, exit_code);
}

#endif