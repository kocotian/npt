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

int createMirrorlist()
{
	std::cerr << col::r << ":: " << col::w << "Failed to open mirror list" << col::e;
	std::string createMirrorList;
	std::clog << col::b << "[Y/n] " << col::w << "Create mirror list? :: " << col::c;
	std::cin >> createMirrorList;
	if(createMirrorList == "Y" || createMirrorList == "y")
	{
		if(!std::filesystem::exists(npt_dir))
		{
			std::clog << col::y << ":: " << col::w << "Creating npt directory...";
			try { std::filesystem::create_directories(npt_dir); std::clog << col::g << " OK" << col::e; }
			catch(const std::exception& e) { std::cerr << "\33[2K\r" << col::r << ":: " << col::w << "Unable to create npt directory (" << col::r << e.what() << col::w << ")" << col::e; return -3; }
		}
		std::clog << col::y << ":: " << col::w << "Creating mirrorlist...";
		std::ofstream mirrorOut;
		mirrorOut.open(std::string(npt_dir + "mirrorlist").c_str(), std::ios::out);
		if(mirrorOut.good())
		{
			mirrorOut << "# Nevada Packaging Tool Mirrorlist";
			std::clog << col::g << " OK" << col::e;
			std::clog << col::y << ":: " << col::w << "Adding main " << col::wa << "npt.kocotian.pl" << col::w << " mirror...";
			mirrorOut << "\nnpt.kocotian.pl\n";
			std::clog << col::g << " OK" << col::e;
			std::clog << col::g << ":: " << col::w << "Creating mirrorlist finished" << col::e;
			mirrorOut.close();
			return 0;
		}
		else
		{
			std::cerr << "\33[2K\r" << col::r << ":: " << col::w << "Unable to create mirrorlist" << col::e;
			return -2;
		}
	}
	else return -1;
	return 0;
}
