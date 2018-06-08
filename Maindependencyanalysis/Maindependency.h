///////////////////////////////////////////////////////////////
// dependencies.h - Handles dependencies between files       //
// Language:    Visual C++ 2012                              //
// Platform:    Macbook pro  Windows 7                       //
// Application:  CSE687 - OOD, Pr#3, Spring 2017             //
// Author:      RamatejaRepaka,         Syracuse University  //
//              (315) 395-9575, rrepaka@syr.edu              //
///////////////////////////////////////////////////////////////

/*
Package Operations:
==================
This package defines all operations related to the dependency between the files

Public Interface:
=================
maindpanalysis v;
v.getfilesset                                                                   //function that returns instance variable files
v.setfileset(vector<fileset>)                                                   //set the fileset to the class instance variable
v.finddependencies()                                                            //find the dependies between the files
v.insert(std::string filename,std::vector<std::string>filenamelist)             // filename and corresponding filename list;
v.displaydependencies()                                                         // displaying the dependencies
v.getdatabase()                                                                 // returns the database instance
v. getonlyfilename(std::vector<std::string> completename)                       // it takes vector<complete filename> and returns vector<only filename>
v.splitpath(                                                                    //function that is useful for getting the filename from the complete path file name
		const std::string& str
		, const std::set<char> delimiters)
v. getfilenamen(std::string name)                                               // used for calling the splitpath function to get the file name only

Build Process:
==============
Required files
- Maindependency.h,depexecutive.cpp

Build commands (either one)
- devenv Maindependencyanalysis.sln

Maintenance History:
====================
ver 1.0 : 1st April 2017
-first release
*/

#pragma once
#include<vector>
#include"../NoSqlDb/NoSqlDb.h"
#include<set>
#include<fstream>
#include <algorithm>
#include"../Tokenizer/Tokenizer.h"
#include<string>



class maindpanalysis
{
	std::vector<std::string> files;
	NoSQLDB::NoSqlDb<std::string> db;

public:
	// <-------------function that returns instance variable files>-------------------
	std::vector<std::string> getfilesset()
	{
		return files;
	}

	//<------------- function that is used to set the file set>------------------------
	void setfileset(std::vector<std::string> fileset)
	{
		std::string name;
		for (size_t i = 0; i < fileset.size(); i++)
		{
			name = fileset[i];
			files.push_back(name);
		}

	}
	//<-----function that is useful for getting only file name from the complete file name>--------
	std::vector<std::string> getonlyfilename(std::vector<std::string> completename)
	{

		std::vector<std::string> onlyname;
		std::string name;
		std::set<char> delims{ '\\' };
		for (size_t i = 0; i < completename.size(); i++)
		{
			std::vector<std::string> path = splitpath(completename[i], delims);
			name = path.back();
			onlyname.push_back(name);
		}
		return onlyname;
	}
	//<------function that is useful for getting the filename from the complete path file name>------
	std::vector<std::string> splitpath(
		const std::string& str
		, const std::set<char> delimiters)
	{
		std::vector<std::string> result;

		char const* pch = str.c_str();
		char const* start = pch;
		for (; *pch; ++pch)
		{
			if (delimiters.find(*pch) != delimiters.end())
			{
				if (start != pch)
				{
					std::string str(start, pch);
					result.push_back(str);
				}
				else
				{
					result.push_back("");
				}
				start = pch + 1;
			}
		}
		result.push_back(start);

		return result;
	}

	//<-------used for calling the splitpath function to get the file name only>---------------------
	std::string  getfilenamen(std::string name)
	{
		std::set<char> delims{ '\\' };
		std::vector<std::string> result = splitpath(name, delims);
		std::string mresult = result.back();
		return mresult;
	}

	//<-------function that is useful for inserting data into the Nosqldatabase>---------------------
	void insert(std::string filename, std::vector<std::string>filenamelist)
	{
		NoSQLDB::Element<std::string> elem;
		bool val = db.hasKey(filename);
		if (val == false)
		{
			for (size_t i = 0; i < filenamelist.size(); i++)
			{
				elem.saveChild(filenamelist[i]);
			}

			db.saveRecord(filename, elem);
		}

		

	}

	//<--------function that is used to find the dependencies between the files>---------------------
	void finddependencies()
	{
		std::vector<std::string> filenames = getonlyfilename(files);
		for (size_t i = 0; i < files.size(); ++i)
		{
			std::vector<std::string> childlist;
			std::string fullfilename=files[i];
			std::string onlyfilename = getfilenamen(fullfilename);
			std::ifstream in(files[i]);
			if (in.is_open())
			{
				std::string str;
				std::string file_contents;
				while (std::getline(in, str))
				{
					std::size_t found = str.find("#");

					if (found != std::string::npos)
					{
						file_contents += str;
						file_contents.push_back('\n');
					}
				}

				for (size_t i = 0; i < filenames.size(); ++i)
				{
					std::size_t found = file_contents.find(filenames[i]);
					if (found != std::string::npos)
					{
						if (filenames[i] != onlyfilename)
							childlist.push_back(filenames[i]);

					}

				}

				insert(onlyfilename, childlist);
				in.close();
			}

			else
			{
				std::cout << "not able to open file";
			}
			
		}

		

	}


	//<--------function that is useful for displaying the dependencies between files>-----------------
	void  displaydependencies()
	{
		std::vector<std::string> keys = db.keys();
		for (std::string key : keys)
		{
			std::cout << "\n  " << key << ":";
			std::cout << db.value(key).show();
		}
	}

	//<----------- function that returns the database instance>--------------------------------	
	NoSQLDB::NoSqlDb<std::string> getdatabase()
	{
		return db;
	}
};