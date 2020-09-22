/*
 * NPT is simple and powerful tool to get and management nar packages.
 * Copyright (C) 2020 Kocotian
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <pwd.h>
#include <unistd.h>

#include "config.h"

std::string version = "0.6";

std::string getUsername()
{
    struct passwd* pwd = getpwuid(getuid());
	if(pwd) return pwd -> pw_name; else return "unknown";
}

bool isRoot()
{
	return (getUsername() == "root");
}

std::vector<std::string> explode(std::string const & s, char delim)
{
	std::vector<std::string> result;
	std::istringstream iss(s);

	for(std::string token; std::getline(iss, token, delim); )
	{
		result.push_back(std::move(token));
	}

	return result;
}

#include "headers/httplib.h"
#include "headers/mirrorlist.hpp"

struct package
{
	std::string host = "";
	std::string packageBasename = "";
	std::string packageName = "";
	std::string packageVersion = "";
	std::string packageLicense = "";
	std::string packageDescription = "";
	std::string packageCompression = "";
	std::string packageDependencies = "";
	std::string makeCommand = "";
	std::string binaries = "";
};

int main(int argc, char** argv)
{
	if(argc > 1)
	{
		#include "headers/install.hpp"
		#include "headers/get.hpp"
		#include "headers/cache.hpp"
		#include "headers/hosts.hpp"
		#include "headers/help.hpp"

		else
		{
			std::cerr << col::r << ":: " << col::w << "Bad option -- " << col::r << argv[1] << col::w << " (Write " << col::wa << argv[0] << " -h" << col::w << ") for help" << col::e;
		}
	}
	else
	{
		std::cerr << col::r << ":: " << col::w << "Expected argument (Write " << col::wa << argv[0] << " -h" << col::w << ") for help" << col::e;
	}
}
