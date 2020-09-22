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

	/* CLEAR CACHE :: -cc :: npt -cc */

	else if(!strcmp(argv[1], "-Cc") || !strcmp(argv[1], "--cache-clear"))
	{
		if(isRoot())
		{
			std::clog << col::y << ":: " << col::w << "Removing cache...";
			if(!system(std::string("rm " + npt_dir + "cache " + npt_dir + "pkgs -rf 2> /dev/null").c_str()))
			{
				std::clog << col::g << " OK" << col::e;
			}
			else
			{
				std::cerr << "\33[2K\r" << col::r << ":: " << col::w << "Unable to remove cache" << col::e;
				return -1;
			}
		}
		else
		{
			std::cerr << col::r << ":: " << col::w << "You cannot perform this operation unless you are root" << col::e;
		}
	}
