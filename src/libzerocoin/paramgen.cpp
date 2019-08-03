/**
 * @file       paramgen.cpp
 *
 * @brief      Parameter generation utility for Zerocoin.
 *
 * @author     Ian Miers, Christina Garman and Matthew Green
 * @date       June 2013
 *
 * @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
 * @license    This project is released under the MIT license.
 **/
// Copyright (c) 2017-2018 The PIVX developers

#include <string>
#include <iostream>
#include <fstream>
//#include <curses.h>
#include <exception>
#include "Zerocoin.h"

#define DEFAULT_MODULUS_SIZE    3072
#define MIN_MODULUS_SIZE        1026

using namespace libzerocoin;

void
PrintWarning()
{
	std::cout << "Zerocoin parameter generation utility" << std::endl;
	std::cout << "-------------------------------------" << std::endl << std::endl;
	std::cout << "This utility generates an l-bit modulus N as the product of" << std::endl;
	std::cout << "two safe primes p, q. The values p and q are not stored." << std::endl;
	std::cout << "Call this program with no arguments to see usage options." << std::endl;
	std::cout << std::endl;
	std::cout << "SECURITY WARNING: ZEROCOIN PARAMETERS MUST BE GENERATED BY" << std::endl;
	std::cout << "A TRUSTED PARTY WHO DOES NOT STORE THE FACTORS. WHILE WE MAKE" << std::endl;
	std::cout << "A BEST EFFORT TO DESTROY THIS INFORMATION WE DO NOT TAKE" << std::endl;
	std::cout << "SPECIAL PRECAUTIONS TO ENSURE THAT THEY ARE DESTROYED." << std::endl;
	std::cout << std::endl;
	std::cout << "USE THIS UTILITY AT YOUR OWN RISK" << std::endl << std::endl;
}

void usage()
{
	printf("Usage:\n");
	printf(" -b <numbits>\n");
	printf(" -o <output file>\n");

	exit (8);
}

int main(int argc, char **argv)
{
	static CBigNum resultModulus(0);
	uint32_t numBits = DEFAULT_MODULUS_SIZE;
	ofstream outfile;
	char* outfileName;
	bool writeToFile = false;

	while ((argc > 1) && (argv[1][0] == '-'))
	{
		switch (argv[1][1])
		{
		case 'b':
			numBits = atoi(argv[2]);
			++argv;
			--argc;
			break;

		case 'o':
			outfileName = argv[2];
			writeToFile = true;
			break;

		case 'h':
			usage();
			break;

		default:
			printf("Wrong Argument: %s\n", argv[1]);
			usage();
			break;
		}

		++argv;
		--argc;
	}

	if (numBits < MIN_MODULUS_SIZE) {
		std::cout << "Modulus is below minimum length (" << MIN_MODULUS_SIZE << ") bits" << std::endl;
		return(0);
	}

	PrintWarning();

	std::cout << "Modulus size set to " << numBits << " bits." << std::endl;
	std::cout << "Generating parameters. This may take a few minutes..." << std::endl;

	// Generate two safe primes "p" and "q"
	CBigNum *p, *q;
	p = new CBigNum(0);
	q = new CBigNum(0);
	*p = CBigNum::generatePrime(numBits / 2, true);
	*q = CBigNum::generatePrime(numBits / 2, true);

	// Multiply to compute N
	resultModulus = (*p) * (*q);

	// Wipe out the factors
	delete p;
	delete q;

	// Convert to a hexidecimal string
	std::string resultHex = resultModulus.ToString(16);

	std::cout << std::endl << "N = " << std::endl << resultHex << std::endl;

	if (writeToFile) {
		try {
			outfile.open (outfileName);
			outfile << resultHex;
			outfile.close();
			std::cout << std::endl << "Result has been written to file '" << outfileName << "'." << std::endl;
		} catch (std::runtime_error &e) {
			std::cout << "Unable to write to file:" << e.what() << std::endl;
		}
	}
}
