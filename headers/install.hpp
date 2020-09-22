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

	/* INSTALL :: -S :: npt -S <package> */

	if(!strcmp(argv[1], "-S") || !strcmp(argv[1], "--install") || !strcmp(argv[1], "install"))
	{
		if(isRoot())
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
								std::clog << "\33[2K\r" << col::y << ":: " << col::w << (argc > 3 ? "[" + col::wa + std::to_string(pkgNum - 1) + " of " + std::to_string(argc - 2) + col::w + "] " : "") << "Checking " << col::wa << packageBasename << col::w << " package info (at host " << col::wa << host << col::w << ")..." << col::e;

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
												else if(d == "packageVersion:")
												{
													getline(release, packageVersion);
												}
												else if(d == "packageLicense:")
												{
													getline(release, packageLicense);
												}
												else if(d == "packageDescription:")
												{
													getline(release, packageDescription);
												}
												else if(d == "packageCompression:")
												{
													getline(release, packageCompression);
												}
												else if(d == "packageDependencies:")
												{
													getline(release, packageDependencies);
												}
												else if(d == "makeCommand:")
												{
													getline(release, makeCommand);
												}
												else if(d == "binaries:")
												{
													getline(release, binaries);
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

					std::clog << col::e << col::g << ":: " << col::w << "Packages:     " << col::wa;
					std::string conInsChoice;

					for(int pkgNum = 0; pkgNum < packages.size(); pkgNum++)
					{
						if(pkgNum > 0) std::clog << col::w << ", " << col::wa;
						std::clog << packages[pkgNum].packageName;
					}
					std::clog << col::e;

					std::clog << std::endl << col::b << "[Y/n] " << col::w << "Continue installation? :: " << col::c;
					std::cin >> conInsChoice;

					if(conInsChoice == "Y" || conInsChoice == "y")
					{
						for(auto & pkg : packages)
						{
							if(pkg.packageName != "")
							{
								/*
								std::clog << col::e << col::g << ":: " << col::w << "Package:     " << col::wa << pkg.packageName << col::e;
								std::clog 			<< col::g << ":: " << col::w << "Version:     " << col::wa << pkg.packageVersion << col::e;
								if(pkg.packageCompression != "") std::clog 			<< col::g << ":: " << col::w << "Compression: " << col::wa << pkg.packageCompression << col::e;
								if(pkg.packageLicense != "") std::clog 			<< col::g << ":: " << col::w << "License:     " << col::wa << pkg.packageLicense << col::e;
								if(pkg.packageDescription != "") std::clog 			<< col::g << ":: " << col::w << "Description: " << col::wa << pkg.packageDescription << col::e;
								*/
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
									try { std::filesystem::create_directories(npt_dir + "pkgs/"); }
									catch(const std::exception& e) { std::cerr << "\33[2K\r" << col::r << ":: " << col::w << "Unable to create packages directory (" << col::r << e.what() << col::w << ")" << col::e; }
									release.open(std::string(npt_dir + "pkgs/" + pkg.packageName).c_str(), std::ios::out | std::ios::binary);
									if(release.good())
									{
										release << requestResponse -> body;
										std::clog << col::y << " :: " << col::g << "FINISHED" << col::e;

										if(pkg.packageCompression != "")
										{
											std::clog << col::y << ":: " << col::w << "Decompressing " << col::wa << pkg.packageBasename << col::w << "...";
											if(pkg.packageCompression == "gz" || pkg.packageCompression == "gzip")
											{
												int gzipOK = system(std::string("gzip -df " + npt_dir + "pkgs/" + pkg.packageName).c_str());
												if(!gzipOK)
												{
													pkg.packageName = pkg.packageName.substr(0, pkg.packageName.size() - 3);
													std::clog << col::g << " OK" << col::e;
												}
												else
												{
													std::cerr << "\r" << col::r << ":: " << col::w << "Decompression of " << col::wa << pkg.packageBasename << col::w << " failed (" << col::r << "error " << gzipOK << col::w << ")" << col::e;
													return -1;
												}
											}
											else if(pkg.packageCompression == "bz2" || pkg.packageCompression == "bzip2")
											{
												int bzip2OK = system(std::string("bzip2 -df " + npt_dir + "pkgs/" + pkg.packageName).c_str());
												if(!bzip2OK)
												{
													pkg.packageName = pkg.packageName.substr(0, pkg.packageName.size() - 4);
													std::clog << col::g << " OK" << col::e;
												}
												else
												{
													std::cerr << "\r" << col::r << ":: " << col::w << "Decompression of " << col::wa << pkg.packageName << col::w << " failed (" << col::r << "error " << bzip2OK << col::w << ")" << col::e;
													return -1;
												}
											}
											else if(pkg.packageCompression == "xz" || pkg.packageCompression == "xzip")
											{
												int xzOK = system(std::string("xz -df " + npt_dir + "pkgs/" + pkg.packageName).c_str());
												if(!xzOK)
												{
													pkg.packageName = pkg.packageName.substr(0, pkg.packageName.size() - 3);
													std::clog << col::g << " OK" << col::e;
												}
												else
												{
													std::cerr << "\r" << col::r << ":: " << col::w << "Decompression of " << col::wa << pkg.packageName << col::w << " failed (" << col::r << "error " << xzOK << col::w << ")" << col::e;
													return -1;
												}
											}
											else
											{
												std::cerr << "\r" << col::r << ":: " << col::w << "Decompression of " << col::wa << pkg.packageName << col::w << " failed (" << col::r << "Unknown compression format" << col::w << ")" << col::e;
												return -1;
											}
										}

										std::clog << col::y << ":: " << col::w << "Unpacking " << col::wa << pkg.packageBasename << col::w << "...";

										std::ifstream i;
										i.open(std::string(npt_dir + "pkgs/" + pkg.packageName).c_str(), std::ios::in | std::ios::binary);
										if(i.good())
										{
											std::string data = "";
											bool inHeader, inFileHeader, inFileContent = false;
											std::string format = "bad";

											while(getline(i, data))
											{
												if(data == ":::[[[")
												{
													inHeader = true;
													std::string d = "";
													do
													{
														getline(i, d);
														if(d == "format:")
														{
															getline(i, format);
															if(format != "nar2")
															{
																return -1;
															}
														}
													}
													while(d != "]]]:::");
													if(d == "]]]:::")
													{
														inHeader = false;
													}
												}
												else if(data == "F:")
												{
													inFileHeader = true;
													std::string path = "";
													getline(i, path);

													std::string dir, file = ""; int lastslash = 0;
													for(int i = 0; i < path.length(); i++)
													{
														if(path[i] == '/') lastslash = i;
													}
													for(int i = 0; i < lastslash; i++)
													dir += path[i];

													try { std::filesystem::create_directories(npt_dir + "cache/" + pkg.packageName + ".unpacked/" + dir); }
													catch(const std::exception& e) { std::cerr << "\33[2K\r" << col::r << ":: " << col::w << "Unable to create temporary package directory (" << col::r << e.what() << col::w << ")" << col::e; }
													std::ofstream o;
													o.open(npt_dir + "cache/" + pkg.packageName + ".unpacked/" + path, std::ios::out | std::ios::binary);
													if(o.good())
													{
														std::string fline = "";
														while(fline != std::string(1, char(28)))
														{
															getline(i, fline);
															if(fline != std::string(1, '\\') + std::string(1, char(28)))
															{
																if(fline != std::string(1, char(28))) o << fline << "\n";
															}
															else
															{
																o << char(28) << "\n";
															}
														}
														o.close();
													}
													else
													{
														return -1;
													}
												}
											}
											if(format == "bad") return -1;
											std::clog << col::g << " OK" << col::e;
											if(pkg.makeCommand != "")
											{
												std::clog << col::y << ":: " << col::w << "Making " << col::wa << pkg.packageBasename << col::w << "..." << col::e;
												int makingStatus = system(std::string("(cd " + npt_dir + "cache/" + pkg.packageName + ".unpacked/ && " + pkg.makeCommand + ")").c_str());
												if(!makingStatus)
												{
													std::clog << col::g << ":: " << col::w << "Making of " << col::wa << pkg.packageBasename << col::w << " finished!" << col::e;
												}
											}
											if(pkg.binaries != "")
											{
												std::vector<std::string> binaries = explode(pkg.binaries, ':');
												for(auto & binaryPath : binaries)
												{
                                                    std::vector<std::string> binaryPathVect = explode(binaryPath, '>');
                                                    std::string installDirectory;
                                                    std::string binary = binaryPathVect[0];
                                                    if(binaryPathVect.size() - 1) installDirectory = binaryPathVect[1]; else installDirectory = "/bin";
                                                    std::clog << col::y << ":: " << col::w << "Installing " << col::wa << pkg.packageBasename << col::w << " to " << col::wa << installDirectory << col::w << "..." << col::e;
													int installStatus = system(std::string("(cd " + npt_dir + "cache/" + pkg.packageName + ".unpacked/ && install -Dm 755 " + binary + " " + installDirectory + "/)").c_str());
													if(installStatus)
													{
														std::cerr << "\33[2K\r" << col::r << ":: " << col::w << "Installation of " << col::wa << binary << col::w << " binary failed." << col::e;
														return -1;
													}
												}
												std::clog << col::g << ":: " << col::w << "Installation of " << col::wa << pkg.packageBasename << col::w << " finished!" << col::e;
											}
										}
										else
										{
											std::cerr << "\33[2K\r" << col::r << ":: " << col::w << "Unpacking" << col::wa << pkg.packageBasename << col::w << " failed." << col::e;
										}
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
		else
		{
			std::cerr << col::r << ":: " << col::w << "You cannot perform this operation unless you are root" << col::e;
		}
	}
