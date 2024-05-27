#include "lexer/lexer.hpp"

#include <iostream>
#include <string>

/// <summary>
/// Main will only be included in a direct build. 
/// When included in the full compiler this file will not be added.
/// </summary>
int main(int argc, char *argv[])
{
	std::cout << "[Warning] This is the frontend only build, this is not the full compiler!" << std::endl;

	std::string infile;
	std::string outfile;

	if (argc < 3) {
		std::cout << "What file to use as input file? ";
		std::cin >> infile;
		std::cout << "What file to use as output file? ";
		std::cin >> outfile;
	}
	else {	
		infile = argv[1];
		outfile = argv[2];
	}

	std::cout << infile << " -> " << outfile;

	std::cin;

	return 0;
}
	