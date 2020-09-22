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

	/* VERSION :: -v :: npt -v */

	else if(!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))
	{
		std::cout << col::g << ":: " << col::w << "Nevada Packaging Tool " << col::wa << "v" << version << col::e;
		std::cout << col::y << ":: " << col::e;
		std::cout << col::y << ":: " << col::wa << "This program is free software: you can redistribute it and/or modify" << col::e;
		std::cout << col::y << ":: " << col::wa << "it under the terms of the GNU General Public License as published by" << col::e;
		std::cout << col::y << ":: " << col::wa << "the Free Software Foundation, either version 3 of the License, or" << col::e;
		std::cout << col::y << ":: " << col::wa << "(at your option) any later version." << col::e;
		std::cout << col::y << ":: " << col::wa << "" << col::e;
		std::cout << col::y << ":: " << col::wa << "This program is distributed in the hope that it will be useful," << col::e;
		std::cout << col::y << ":: " << col::wa << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << col::e;
		std::cout << col::y << ":: " << col::wa << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << col::e;
		std::cout << col::y << ":: " << col::wa << "GNU General Public License for more details." << col::e;
	}

	/* HELP :: -h :: npt -h */

	else if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
	{
		std::cout << col::g << ":: " << col::w << "Nevada Packaging Tool " << col::wa << "v" << version << col::e;
		std::cout << col::y << ":: " << col::w << "Usage:" << col::e;
		std::cout << col::y << ":: " << col::wa << "-S <package>    Get and install package" << col::e;
		std::cout << col::y << ":: " << col::wa << "-Cc             Clear cache" << col::e;
		std::cout << col::y << ":: " << col::wa << "-Hl             List hosts" << col::e;
		std::cout << col::y << ":: " << col::wa << "-Ha <host>      Add host" << col::e;
		std::cout << col::y << ":: " << col::wa << "-Hr <host>      Remove host" << col::e;
	}
