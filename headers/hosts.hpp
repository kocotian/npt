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

	/* LIST HOSTS :: -Hl :: npt -Hl */

	else if(!strcmp(argv[1], "-Hl") || !strcmp(argv[1], "--hosts-list"))
	{
		std::ifstream mirrorlist;
		mirrorlist.open(std::string(npt_dir + "mirrorlist").c_str(), std::ios::in);
		if(mirrorlist.good())
		{
			int createMirrorlistStatus = createMirrorlist();
			if(createMirrorlistStatus) return createMirrorlistStatus;
			mirrorlist.open(std::string(npt_dir + "mirrorlist").c_str(), std::ios::in);
		}

		if(mirrorlist.good())
		{
			std::string host;
			std::cout << col::g << ":: " << col::w << "Hosts:" << col::e;
			int i = 1;
			while(getline(mirrorlist, host))
			{
				if(host != "" && host[0] != '#' && !mirrorlist.eof())
				{
					std::cout << col::g << ":: " << col::w << (i < 10 ? "0" : "") << i << ". " << col::wa << host << col::e;
					i++;
				}
			}
			mirrorlist.close();
		}
	}

	/* ADD HOST :: -Ha :: npt -Ha <host> */

	else if(!strcmp(argv[1], "-Ha") || !strcmp(argv[1], "--host-add"))
	{
		if(isRoot())
		{
			if(argc >= 3)
			{
				std::ofstream outMirrorlist;
				outMirrorlist.open(std::string(npt_dir + "mirrorlist").c_str(), std::ios::out | std::ios::binary | std::ios::app);
				if(outMirrorlist.fail())
				{
					int createMirrorlistStatus = createMirrorlist();
					if(createMirrorlistStatus) return createMirrorlistStatus;
					outMirrorlist.open(std::string(npt_dir + "mirrorlist").c_str(), std::ios::out | std::ios::binary | std::ios::app);
				}

				if(outMirrorlist.good())
				{
					std::string host;
					for(int i = 2; i < argc; i++)
					{
						outMirrorlist << argv[i] << std::endl;
						std::cout << col::g << ":: " << col::w << "Added host " << col::wa << argv[i] << col::w << " to mirrorlist." << col::e;
					}
					outMirrorlist.close();
				}

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
					std::string host;
					std::cout << col::g << ":: " << col::w << "Hosts:" << col::e;
					int i = 1;
					while(getline(mirrorlist, host))
					{
						if(host != "" && host[0] != '#' && !mirrorlist.eof())
						{
							std::cout << col::g << ":: " << col::w << (i < 10 ? "0" : "") << i << ". " << col::wa << host << col::e;
							i++;
						}
					}
					mirrorlist.close();
				}
			}
			else
			{
				std::cerr << col::r << ":: " << col::w << "No host specified (write " << col::wa << argv[0] << " -h" << col::w << " for help)" << col::e;
			}
		}
		else
		{
			std::cerr << col::r << ":: " << col::w << "You cannot perform this operation unless you are root" << col::e;
		}
	}

	/* REMOVE HOSTS :: -Hr :: npt -Hr <host> */

	else if(!strcmp(argv[1], "-Hr") || !strcmp(argv[1], "--host-remove"))
	{
		if(isRoot())
		{
			if(argc >= 3)
			{
				std::stringstream hostlist;
				std::fstream mirrorlist;
				mirrorlist.open(std::string(npt_dir + "mirrorlist").c_str(), std::ios::in);
				if(mirrorlist.fail())
				{
					int createMirrorlistStatus = createMirrorlist();
					if(createMirrorlistStatus) return createMirrorlistStatus;
					mirrorlist.open(std::string(npt_dir + "mirrorlist").c_str(), std::ios::in);
				}

				if(mirrorlist.good())
				{
					std::string host;
					while(getline(mirrorlist, host))
					{
						hostlist << host << std::endl;
					}
					mirrorlist.close();
				}

				mirrorlist.open(std::string(npt_dir + "mirrorlist").c_str(), std::ios::out | std::ios::binary);
				if(mirrorlist.good())
				{
					std::string host;
					std::cout << col::g << ":: " << col::w << "Hosts:" << col::e;
					int i = 1;
					while(getline(hostlist, host))
					{
						bool removeHost = false;
						for(int j = 2; j < argc; j++) if(!removeHost) if(host == std::string(argv[j])) removeHost = true;

						if(!removeHost)
						{
							mirrorlist << host << std::endl;
							if(host != "" && host[0] != '#' && !mirrorlist.eof())
							{
								std::cout << col::g << ":: " << col::w << (i < 10 ? "0" : "") << i << ". " << col::wa << host << col::e;
								i++;
							}
						}
					}
					mirrorlist.close();
				}
				else
				{
					std::cerr << col::r << ":: " << col::w << "Unable to open " << col::wa << "mirrorlist" << col::w << " to write" << col::e;
					return -1;
				}
			}
			else
			{
				std::cerr << col::r << ":: " << col::w << "No host specified (write " << col::wa << argv[0] << " -h" << col::w << " for help)" << col::e;
			}
		}
		else
		{
			std::cerr << col::r << ":: " << col::w << "You cannot perform this operation unless you are root" << col::e;
		}
	}
