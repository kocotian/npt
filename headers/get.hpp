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

	/* GET :: -g :: npt -g <package> */
	else if(!strcmp(argv[1], "-G") || !strcmp(argv[1], "--get") || !strcmp(argv[1], "get"))
	{
		if(argc >= 3)
		{
			std::ifstream mirrorlist;
			mirrorlist.open(std::string(npt_dir + "mirrorlist").c_str(), std::ios::in);
			if(mirrorlist.fail())
			{
				int createMirrorlistStatus = createMirrorlist();
				if(createMirrorlistStatus) return createMirrorlistStatus;
				mirrorlist.open(std::string(npt_dir + "mirrorlist").c_str(), std::ios::in);
			}

			if(mirrorlist.good())
			{
				std::stringstream mirrorlist_ss;
				std::string host_ss;
				while(getline(mirrorlist, host_ss))
				{
					if(host_ss != "" && host_ss[0] != '#' && !mirrorlist.eof())
					mirrorlist_ss << host_ss << std::endl;
				}
				mirrorlist.close();

				std::vector<package> packages;

				for(int pkgNum = 2; pkgNum < argc; pkgNum++)
				{
					std::stringstream mirrorlist_loop;
					mirrorlist_loop << mirrorlist_ss.str();

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


					packageBasename = std::string(argv[pkgNum]);
					while(packageName == "" && !mirrorlist_loop.eof())
					{
						getline(mirrorlist_loop, host);
						if(host != "" && host[0] != '#' && !mirrorlist_loop.eof())
						{
							std::clog << "\33[2K\r" << col::y << ":: " << col::w << (argc > 3 ? "[" + col::wa + std::to_string(pkgNum - 1) + " of " + std::to_string(argc - 2) + col::w + "] " : "") << "Checking " << col::wa << packageBasename << col::w << " package version (at host " << col::wa << host << col::w << ")";

							httplib::Client request(host, 80);
							auto requestResponse = request.Get(std::string("/release/" + packageBasename).c_str(),
								[packageBasename](uint64_t current, uint64_t total)
								{
									std::clog << "\33[2K\r" << col::y << ":: " << col::w << "Downloading " << col::wa << packageBasename << col::w << " package info: " << col::wa << current << col::w << " bytes of " << col::wa << total << col::y << " :: " << col::w << int(current * 100 / total) << "%";
									return true;
								}
							);

							if(requestResponse && requestResponse -> status == 200)
							{
								std::clog << col::y << " :: " << col::g << "FINISHED" << col::e;
								std::stringstream release;
								release << requestResponse -> body;
								std::string releaseData;

								while(getline(release, releaseData))
								{
									if(releaseData == ":::[[[")
									{
										std::string d = "";
										do
										{
											getline(release, d);
											if(d == "packageName:")
											{
												getline(release, packageName);
											}
										}
										while(d != "]]]:::");
									}
								}
							}
							else
							{
								std::cerr << "\r" << col::r << ":: " << col::w << "Checking " << col::wa << packageBasename << col::w << " package version (at host " << col::wa << host << col::w << ") failed. (" << col::r << (requestResponse ? ("Server responsed with status " + std::to_string(requestResponse -> status)) : "Host not found") << col::w << ")" << col::e;
							}
						}
					}

					if(packageDependencies != "")
					{
						//
					}

					packages.push_back({
						host,
						packageBasename,
						packageName,
						packageVersion,
						packageLicense,
						packageDescription,
						packageCompression,
						packageDependencies,
						makeCommand,
						binaries
					});
				}

				if(true)
				{
					for(auto & pkg : packages)
					{
						if(pkg.packageName != "")
						{
							std::clog << col::y << ":: " << col::w << "Getting " << col::wa << pkg.packageBasename << col::w << " package from host " << col::wa << pkg.host << col::w << "...";

							httplib::Client request(pkg.host, 80);
							auto requestResponse = request.Get(std::string("/pkgs/" + pkg.packageName).c_str(),
								[](uint64_t current, uint64_t total)
								{
									std::clog << "\33[2K\r" << col::y << ":: " << col::w << "Downloading package: " << col::wa << (total > 1024 ? (total > 1024 * 1024 ? current / (1024 * 1024) : current / 1024) : current) << col::w << (total > 1024 ? (total > 1024 * 1024 ? " MiB of " : " KiB of ") : " bytes of ") << col::wa << (total > 1024 ? (total > 1024 * 1024 ? total / (1024 * 1024) : total / 1024) : total) << col::y << " :: " << col::w << int(current * 100 / total) << "% " << col::y << "[" << col::g;
									for(int n = 0; n < int(current * 20 / total); n++)
									{
										std::clog << ":";
									}
									std::clog << col::r;
									for(int n = 0; n < 20 - int(current * 20 / total); n++)
									{
										std::clog << ".";
									}
									std::clog << col::y << "]" << col::w;
									return true;
								}
							);

							if(requestResponse && requestResponse -> status == 200)
							{
								std::ofstream release;
								release.open(std::string(pkg.packageName).c_str(), std::ios::out | std::ios::binary);
								if(release.good())
								{
									std::clog << col::g << " :: " << col::g << "FINISHED" << col::e;
									std::clog << col::y << ":: " << col::w << "Saving " << col::wa << pkg.packageBasename << col::w << " package data...";
									release << requestResponse -> body;
									std::clog << col::g << " :: " << col::g << "FINISHED" << col::e;
								}
								else
								{
									std::cerr << "\33[2K\r" << col::r << ":: " << col::w << "Writing " << col::wa << pkg.packageBasename << col::w << " to file failed." << col::e;
								}
							}
							else
							{
								std::cerr << "\33[2K\r" << col::r << ":: " << col::w << "Checking " << col::wa << pkg.packageBasename << col::w << " package version (at host " << col::wa << pkg.host << col::w << ") failed. (" << col::r << (requestResponse ? ("Server responsed with status " + std::to_string(requestResponse -> status)) : "Host not found") << col::w << ")" << col::e;
							}
						}
						else
						{
							std::cerr << "\33[2K\r" << col::r << ":: " << col::w << "Getting package " << col::wa << pkg.packageBasename << col::w << " failed. (" << col::r << "Package not found" << col::w << ")" << col::e;
						}
					}
				}
				else
				{
					std::clog << col::r << ":: " << col::w << "Aborted by user" << col::e;
					return 0;
				}
			}
		}
		else
		{
			std::cerr << col::r << ":: " << col::w << "No target specified (write " << col::wa << argv[0] << " -h" << col::w << " for help)" << col::e;
		}
	}
