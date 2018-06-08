///////////////////////////////////////////////////////////////////////////////
// depexecutive.cpp - find the dependencies among given set of files         //
// this package contains all the operations for the finding the dependencies //
//-------------------------------------------------------------------------  //
//                                                                           //
//---------------------------------------------------------------------------//
// Language:    C++, Visual Studio 2015                                      //
// Platform:    Dell XPS 8900, Windows 7                                     //
// Application: Project #3, CSE687 - Object Oriented Design, S2017           //
// Author:      ramteja repaka, Syracuse University                          //
//              rrepaka@syr.edu                                              //
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include"Maindependency.h"
#include"vector"
#include<string>
#include<fstream>

//--------<TEST STUB>----------------------
#ifdef DEP_EXECUTIVE
int main()
{

	maindpanalysis analysis;
	std::vector<std::string> fileset;
	fileset.push_back("..\\ServerInputFiles\\A0.h");
	fileset.push_back("..\\ServerInputFiles\\A1.h");
	fileset.push_back("..\\ServerInputFiles\\A2.h");
	fileset.push_back("..\\ServerInputFiles\\A3.h");
	fileset.push_back("..\\ServerInputFiles\\A4.h");
	std::cout << "\nafter setting the file set\n";
	analysis.setfileset(fileset);
	std::cout << "\nbefore calling display dependencies\n";
	analysis.finddependencies();
	std::cout << "\nafter calling display dependencies\n";
	analysis.displaydependencies();
}
#endif