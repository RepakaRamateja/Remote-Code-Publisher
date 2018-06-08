////////////////////////////////////////////////////////////////////////////
// Publisher.cpp - creation of html files  demo from given source c++ files//
// this package contains all the operations for the creation of html files//
//-------------------------------------------------------------------------//
//                                                                         //
//-------------------------------------------------------------------------//
// Language:    C++, Visual Studio 2015                                    //
// Platform:    Dell XPS 8900, Windows 7                                   //
// Application: Project #3, CSE687 - Object Oriented Design, S2017         //
// Author:      ramteja repaka, Syracuse University                        //
//              rrepaka@syr.edu                                            //
/////////////////////////////////////////////////////////////////////////////
#include<vector>
#include"Publisher.h"
#include"../Maindependencyanalysis/Maindependency.h"


//----< TEST STUB >------------------------------
#ifdef PUB_EXECUTIVE
int main(int argc, char *argv[])
{
	
	publisher pub(argv[1],"..\\Serveroutputfiles\\");
	NoSQLDB::NoSqlDb<std::string> db;
	std::vector<std::string> fileset;
	fileset.push_back("..\\ServerInputFiles\\A1.h");
	fileset.push_back("..\\ServerInputFiles\\A2.h");
	maindpanalysis analysis;
	analysis.setfileset(fileset);
	analysis.finddependencies();
	analysis.displaydependencies();
	db=analysis.getdatabase();
	pub.setfileset(fileset);
	pub.publishfiles(db);
	pub.openfilesthroughbrowser("..\\Serveroutputfiles\\index.html");
	return 0;
}
#endif

