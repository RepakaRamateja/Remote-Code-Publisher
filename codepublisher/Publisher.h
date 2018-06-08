///////////////////////////////////////////////////////////////
// Publisher.h - creates html files from given c++ source files//
// Language:    Visual C++ 2012                              //
// Platform:    Macbook pro  Windows 7                       //
// Application:  CSE687 - OOD, Pr#3, Spring 2017             //
// Author:      RamatejaRepaka,         Syracuse University  //
//              (315) 395-9575, rrepaka@syr.edu              //
///////////////////////////////////////////////////////////////

/*
Package Operations:
==================
This package defines all operations related to the publishing of the c++ source files

Public Interface:
=================
publisher v;
v.setfileset(std::vector<std::string> filesets)                              //used to initialize fileset variable
v.replacestring(std::string old,std::string rep,std::string &file_contents)  //used to replace old part of the string(file_contents) with given rep 
v.std::vector<std::string> splitpath(
		const std::string& str
		, const std::set<char> delimiters)                                   // return vector<std::string> by splitting given string with given delimiter
v. createfile(std::string filename,std::string filecontent)                  // create html file with the given filename and given filecontent
v.addbuttonlogic(int i)                                                      // returns string after adding logic required for implementing collapse and expand
v.filecontent(std::string content, std::string delimiter)                    // add string(it contains tags) returned by a addbuttonlogic method to the html content
v.readfromfile(std::string fullname)                                         //read all the content from the file and append to the string
v.publishfiles(NoSQLDB::NoSqlDb<std::string> dbase)                          // main  starting point of code for publishing files to the default specified folder location
createindexpage()                                                            // creates index page (called inside publish files function)
openfilesthroughbrowser(std::string filepath)                                // function used to open the specified filepath through browser(called inside publish files function)

Build Process:
==============
Required files
-Publisher.h,publisher.cpp

Build commands (either one)
- devenv codepublisher.sln


Maintenance History:
====================
ver 1.0 : 1st april 2017
-first release
*/
#pragma once
#include<vector>
#include<iostream>
#include<string>
#include"../XmlDocument/XmlDocument/XmlDocument.h"
#include"../XmlDocument/XmlElement/XmlElement.h"
#include<fstream>
#include<algorithm>
#include<set>
#include"../NoSqlDb/NoSqlDb.h"
#include"../DateTime/DateTime.h"
#include<Windows.h>
#include<shellapi.h>
#include"../FileSystem/FileSystem.h"
#include"../Maindependencyanalysis/Maindependency.h"



using namespace XmlProcessing;
using SPtr = std::shared_ptr<AbstractXmlElement>;
class publisher
{
	std::vector<std::string> fileset;
	std::vector<std::string> filenamelist;
	NoSQLDB::NoSqlDb<std::string> db;
	std::vector<std::string> outputfileset;
	std::string indexfilepath;
	std::string virtualdirectory;
	std::string local;

public:
	publisher(std::string vdir, std::string localdirectory)
	{
		virtualdirectory = vdir;
		local = localdirectory;

	}

	//-----< set the given fileset to the  class instance variable fileset >-----------
	void setfileset(std::vector<std::string> filesets)
	{
		std::string name;
		for (size_t i = 0; i < filesets.size(); i++)
		{
			name = filesets[i];
			fileset.push_back(name);
		}
	}
	// -----< replace with old content with the replace content in the given string file_contents >------------
	void replacestring(std::string old,std::string rep,std::string &file_contents)
	{
		for (int pos = 0;
			(pos = file_contents.find(old, pos)) != std::string::npos;
			pos += rep.length())
		{
			file_contents.replace(pos, old.length(), rep);		
		}
	}

	//----< splits the given string with the  given delimiters and return vector<std::string> >--------------------
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

	//----< used for calling the splitpath function to get the file name only >-------------
	std::string  getfilenamen(std::string name)
	{
		std::set<char> delims{ '\\' };
		std::vector<std::string> result = splitpath(name, delims);
		std::string mresult = result.back();
		return mresult;
	}

	//----< used to split the string from the given delimiter >--------------------
	std::vector<std::string> split_string(const std::string& str,
		const std::string& delimiter)
	{
		std::vector<std::string> strings;

		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		while ((pos = str.find(delimiter, prev)) != std::string::npos)
		{
			strings.push_back(str.substr(prev, pos - prev));
			prev = pos + 1;
		}

		// To get the last substring (or only, if delimiter is not found)
		strings.push_back(str.substr(prev));

		return strings;
	}

	//----< create html file from the given file >----------------------------------
	void createfile(std::string filename,std::string filecontent)
	{	
		std::string path = local + filename;
		path += ".html";
		std::string vpath = virtualdirectory + filename;
		vpath += ".html";
		if (filename == "index")
		{
			indexfilepath = path;
		}
		std::string temp;
		temp = temp + "<!----------------------------------------------------------------------------";
		temp = temp + "\n";
		temp = temp + filename;
		temp = temp + "- generated html file for Project #3 demo";
		temp = temp + "\n";
		temp = temp + "RamaTejaRepaka, Syracuse University rrepaka@syr.edu";
		temp = temp + "\n";
		temp = temp + "----------------------------------------------------------------------------->";
		temp = temp + "\n";
		filecontent = temp + filecontent;
		std::ofstream myfile;
		std::ofstream myfile2;
		myfile.open(path);
		myfile2.open(vpath);
		myfile << filecontent;
		myfile2 << filecontent;
		myfile.close();
		myfile2.close();
		if (filename != "index")
		{
			filenamelist.push_back(filename);
			outputfileset.push_back(path);
		}
	}

	//----< add + and - logic >----------------------------------------------
	std::string addbuttonlogic(int i)
	{
		std::string temp;
		temp = temp + "{";
		temp = temp + "\n";
		temp = temp + "<button onclick=";
		temp = temp + "\"myfunc(";
		temp = temp + std::to_string(i) + ")\"";
		temp = temp + "id=\"";
		temp = temp + "plus";
		temp = temp + std::to_string(i);
		temp = temp + "\"";
		temp = temp + ">";
		temp = temp + "-";
		temp = temp + "</button>";
		temp = temp + "\n";
		temp = temp + "<button onclick=";
		temp = temp + "\"myfunc(";
		temp = temp + std::to_string(i) + ")\"";
		temp = temp + "style=";
		temp = temp + "\"display:none\"";
		temp = temp + " id=\"";
		temp = temp + "minus";
		temp = temp + std::to_string(i);
		temp = temp + "\"";
		temp = temp + ">";
		temp = temp + "+";
		temp = temp + "</button>";
		temp = temp + "\n";
		temp = temp + "<div";
		temp = temp + "  id=\"";
		temp = temp + std::to_string(i);
		temp = temp + "\"";
		temp = temp + ">";
		return temp;
	}

	//----< adding java script tags to the html content >------------------------
	std::string filecontent(std::string content, std::string delimiter)
	{
		std::vector<std::string> ouput = split_string(content, "\n");
		for (size_t i = 0; i < ouput.size(); i++)
		{
			std::size_t found = ouput[i].find("{");
			if (found != std::string::npos)
			{
				std::string temp = addbuttonlogic(i);
				replacestring("{", temp, ouput[i]);
			}
			std::size_t found2 = ouput[i].find("}");
			if (found2 != std::string::npos)
			{
				std::string temp;
				temp = "</div>";
				temp = temp + "\n";
				temp = temp + "}";
				replacestring("}", temp, ouput[i]);
			}
		}
		std::string newstring = "";
		for (size_t i = 0; i < ouput.size(); i++)
		{
			newstring = newstring + ouput[i];
			newstring.push_back('\n');
		}
		return newstring;
	}

	//----< read all the contents from the file and append to string >------------------
	std::string readfromfile(std::string fullname)
	{
		std::ifstream file(fullname);
		std::string file_contents;
		file_contents.push_back('\n');
		std::string str;
		std::string divcon;
		int count = 0;
		while (std::getline(file, str))
		{
			file_contents += str;
			replacestring("<", "&lt;", file_contents);
			replacestring(">", "&gt;", file_contents);
			file_contents.push_back('\n');
		}
		return file_contents;
	}

	// ----< main code for publishing files into the folder location >---------------------
	void publishfiles(NoSQLDB::NoSqlDb<std::string> dbase)
	{
		std::cout << "\n files published in  ../Serveroutputfiles/ \n";db = dbase;
		for (size_t i = 0; i < fileset.size(); i++)
		{
			XmlDocument doc;SPtr pRoot = makeTaggedElement("html");doc.docElement() = pRoot;
			SPtr phead = makeTaggedElement("head");
			SPtr plink= makeTaggedElement("link");
			plink->addAttrib("rel","stylesheet");
			plink->addAttrib("href","project3.css");
			plink->addAttrib("type", "text/css");phead->addChild(plink);
			SPtr pscript = makeTaggedElement("script");pscript->addAttrib("text", "text/javascript");
			pscript->addAttrib("src","collapsescript.js");
			phead->addChild(pscript);pRoot->addChild(phead);
			SPtr pNameElem = makeTaggedElement("body");
			pRoot->addChild(pNameElem);SPtr heading = makeTaggedElement("h3");
			std::string onlyname = getfilenamen(fileset[i]);
			SPtr headingtext = makeTextElement(onlyname);
			heading->addChild(headingtext);
			pNameElem->addChild(heading);SPtr mdiv = makeTaggedElement("div");
			mdiv->addAttrib("class", "indent");
			SPtr linebreakst = makeTaggedElement("hr");mdiv->addChild(linebreakst);
			SPtr heading4 = makeTaggedElement("h4");mdiv->addChild(heading4);
			SPtr heading4text = makeTextElement("Dependencies:");heading4->addChild(heading4text);
			NoSQLDB::Element<std::string> elem=db.value(onlyname);std::vector<std::string> childlist = elem.children;
			for (size_t i = 0; i < childlist.size(); i++)
			{
			   SPtr ma = makeTaggedElement("a");
			   std::string htmlname = childlist[i]+".html";
			   ma->addAttrib("href", htmlname);
			   SPtr linktext = makeTextElement(childlist[i]);
			   ma->addChild(linktext);mdiv->addChild(ma);
			   SPtr brs = makeTaggedElement("br");
			   mdiv->addChild(brs);
			}
			SPtr linebreakend = makeTaggedElement("hr");mdiv->addChild(linebreakend);
			pNameElem->addChild(mdiv);SPtr pretag = makeTaggedElement("pre");
			pNameElem->addChild(pretag);std::string file_contents = readfromfile(fileset[i]);
			std::string newstring = filecontent(file_contents, "\n");		
			SPtr ptext = makeTextElement(newstring);pretag->addChild(ptext);
			std::string temp3 = doc.toString();createfile(onlyname, temp3);	
		}
		if (fileset.size() > 0)
		{
			createindexpage();
		}
		
	}

	//----< creation of index page >-----------------------
	void createindexpage()
	{
		XmlDocument doc;
		SPtr pRoot = makeTaggedElement("html");
		doc.docElement() = pRoot;
		SPtr phead = makeTaggedElement("head");
		SPtr plink = makeTaggedElement("link");
		plink->addAttrib("rel", "stylesheet");
		plink->addAttrib("href", "project3.css");
		plink->addAttrib("type", "text/css");
		phead->addChild(plink);pRoot->addChild(phead);
		SPtr pNameElem = makeTaggedElement("body");
		pRoot->addChild(pNameElem);
		SPtr heading = makeTaggedElement("h3");
		SPtr headingtext = makeTextElement("list of all generated files");
		heading->addChild(headingtext);
		pNameElem->addChild(heading);
		SPtr mdiv = makeTaggedElement("div");
		mdiv->addAttrib("class", "indent");
		for (size_t i = 0; i < outputfileset.size(); i++)
		{
			std::size_t found2 = outputfileset[i].find("index");
			if (found2 == std::string::npos)
			{
				SPtr ma = makeTaggedElement("a");
				std::string relname = filenamelist[i];
				relname = relname + ".html";
				ma->addAttrib("href",relname);
				SPtr linktext = makeTextElement(filenamelist[i]);
				ma->addChild(linktext); mdiv->addChild(ma);
				SPtr brs = makeTaggedElement("br");
				mdiv->addChild(brs);
			}
		}
		pNameElem->addChild(mdiv);
		std::string temp = doc.toString();
		createfile("index", temp);
	}


    //----< opening files in browser >----------------
	void openfilesthroughbrowser(std::string path)
	{
		std::cout << "\n opening the given command line argument filename through default browser\n";
		std::string command("start \"\" \"" + path + "\"");
		std::system(command.c_str());
		std::cout << "\n";
	}


};