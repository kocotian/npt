#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>

#include "HTTPRequest.hpp"
#include "config.h"

std::string version = "0.1";

int main(int argc, char** argv)
{
	if(argc > 1)
	{
		/* GET :: -g :: npt -g <package> */
		if(!strcmp(argv[1], "-g") || !strcmp(argv[1], "--get") || !strcmp(argv[1], "-S") || !strcmp(argv[1], "install"))
		{
			if(argc >= 3)
			{
				std::ifstream mirrorlist;
				mirrorlist.open(std::string(npt_dir + "mirrorlist").c_str(), std::ios::in);
				if(mirrorlist.good())
				{
					std::string host = "";
					std::string packageName = "";
					std::string packageVersion = "";
					std::string packageLicense = "";
					std::string packageDescription = "";

					std::string installCommand = "make && make install";

					while(packageName == "" && !mirrorlist.eof())
					{
						getline(mirrorlist, host);
						if(host != "" && host[0] != '#' && !mirrorlist.eof())
						{
							std::clog << "\33[2K\r" << col::y << ":: " << col::w << "Checking " << col::wa << argv[2] << col::w << " package version (at host " << col::wa << host << col::w << ")...";
							try
							{
								http::Request request("http://" + host + "/release/" + argv[2]);

								const http::Response response = request.send("GET");
								if(response.status == 200)
								{
									std::clog << col::g << " OK" << col::e;
									std::stringstream release;
									release << std::string(response.body.begin(), response.body.end());
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
												else if(d == "installCommand:")
												{
													getline(release, installCommand);
												}
											}
											while(d != "]]]:::");
										}
									}
								}
								else
								{
									// std::cerr << "\r" << col::r << ":: " << col::w << "Checking " << col::wa << argv[2] << col::w << " package version (at host " << col::wa << host << col::w << ") failed. (" << col::r << "Server responsed with status " << response.status << col::w << ")" << col::e;
								}
							}
							catch(const std::exception& e)
							{
								// std::cerr << "\r" << col::r << ":: " << col::w << "Checking " << col::wa << argv[2] << col::w << " package version (at host " << col::wa << host << col::w << ") failed. (" << col::r << e.what() << col::w << ")" << col::e;
							}
						}
					}

					mirrorlist.close();

					if(packageName != "")
					{
						std::string conInsChoice;
						std::clog << col::e << col::g << ":: " << col::w << "Package: " << col::wa << packageName << col::e;
						std::clog 			<< col::g << ":: " << col::w << "Version: " << col::wa << packageVersion << col::e;
						if(packageLicense != "") std::clog 			<< col::g << ":: " << col::w << "License: " << col::wa << packageLicense << col::e;
						if(packageDescription != "") std::clog 			<< col::g << ":: " << col::w << "Description: " << col::wa << packageDescription << col::e;
						std::clog << std::endl << col::b << "[Y/n] " << col::w << "Continue installation? :: " << col::c;
						std::cin >> conInsChoice;
						if(conInsChoice == "Y" || conInsChoice == "y" || conInsChoice == "Yes" || conInsChoice == "yes" || conInsChoice == "YES")
						{
							std::clog << col::y << ":: " << col::w << "Getting " << col::wa << packageName << col::w << " package from host " << col::wa << host << col::w << "..."; 
							try
							{
								http::Request request("http://" + host + "/pkgs/" + packageName);

								const http::Response response = request.send("GET");
								if(response.status == 200)
								{
									std::clog << col::g << " OK" << col::e;
									std::clog << col::y << ":: " << col::w << "Unpacking " << col::wa << packageName << col::w << "..."; 

									std::stringstream i;
									i << std::string(response.body.begin(), response.body.end());
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

											std::filesystem::create_directories(npt_dir + "cache/" + packageName + ".unpacked/" + dir);
											std::ofstream o;
											o.open(npt_dir + "cache/" + packageName + ".unpacked/" + path, std::ios::out | std::ios::binary);
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
									std::clog << col::y << ":: " << col::w << "Installing " << col::wa << packageName << col::w << "..."; 
									int installStatus = system(std::string("(cd " + npt_dir + "cache/" + packageName + ".unpacked/ && " + installCommand + ") > /dev/null").c_str());
									if(!installStatus)
									{
										std::clog << col::g << " OK" << col::e;
										std::clog << col::y << ":: " << col::w << "Installation of " << col::wa << packageName << col::w << " package finished!" << col::e; 
									}
									else
									{
										std::clog << col::y << ":: " << col::w << "Installation of " << col::wa << packageName << col::w << " package finished!" << col::e; 
										std::cerr << "\r" << col::r << ":: " << col::w << "Installation of " << col::wa << packageName << col::w << " package failed." << col::e;
									}
								}
								else
								{
									std::cerr << "\r" << col::r << ":: " << col::w << "Getting " << col::wa << packageName << col::w << " package from host " << col::wa << host << col::w << " failed. (" << col::r << "Server responsed with status " << response.status << col::w << ")" << col::e;
								}
							}
							catch(const std::exception& e)
							{
								std::cerr << "\r" << col::r << ":: " << col::w << "Checking " << col::wa << argv[2] << col::w << " package version (at host " << col::wa << host << col::w << ") failed. (" << col::r << e.what() << col::w << ")" << col::e;
							}
						}
						else
						{
							std::clog << col::r << ":: " << col::w << "Aborted by user" << col::e;
							return 0;
						}
					}
					else
					{
						std::cerr << "\33[2K\r" << col::r << ":: " << col::w << "Getting package " << col::wa << argv[2] << col::w << " failed. (" << col::r << "Package not found" << col::w << ")" << col::e;
					}

				}
				else
				{
					std::cerr << col::r << ":: " << col::w << "Failed to open mirror list" << col::e;
				}
			}
			else
			{
				std::cerr << col::r << ":: " << col::w << "No target specified (write " << col::wa << argv[0] << " -h" << col::w << " for help)" << col::e;
			}
		}

		/* HELP :: -h :: npt -h */
		else if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
		{
			std::cout << "Next Packaging Tool v" << version << '\n';
			std::cout << "Usage:\n    -g <package>    Get and install package\n    -r <package>    Remove package\n\nExamples:\n";
			std::cout << argv[0] << " -g nar - Getting and installing latest nar\n";
			std::cout << argv[0] << " -r npart - Removing npart package\n";
			std::cout << argv[0] << " -s - Syncing database\n\r";
		}
		else
		{
			std::cerr << argv[0] << ": bad option -- '" << argv[1] << "'\nWrite \"" << argv[0] << " -h\" for help\n\r";
		}
	}
	else
	{
		std::cerr << argv[0] << ": expected argument\nWrite \"" << argv[0] << " -h\" for help\n\r";
	}
}
