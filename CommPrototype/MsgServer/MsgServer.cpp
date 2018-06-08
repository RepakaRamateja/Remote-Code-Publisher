/////////////////////////////////////////////////////////////////////////
// MsgServer.cpp - Demonstrates simple one-way HTTP style messaging    //
//                 and file transfer                                   //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////

#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include <string>
#include <iostream>
#include<set>
#include"../../Maindependencyanalysis/Maindependency.h"
#include"../../codepublisher/Publisher.h"
#include"ClientHandler.h"
#include"../../MockChannel/MockChannel.h"
#include<fstream>

using Show = StaticLogger<1>;
using namespace Utilities;

// -----< replace with old content with the replace content in the given string file_contents >------------
void replacesstring(std::string old, std::string rep, std::string &file_contents)
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

//--------<interprets the message and performs corresponding options------------------------------->
void ClientHandler::checkmessage(HttpMessage msg)
{
	if (msg.bodyString() =="cssfiles")
	{
		std::vector<std::string> fullpath;
		std::vector<std::string> files = FileSystem::Directory::getFiles("../Serveroutputfiles/", "*.css");//std::vector<std::string> files = FileSystem::Directory::getFiles("../../Serveroutputfiles/", "*.css");
		std::vector<std::string> dummy = FileSystem::Directory::getFiles("../Serveroutputfiles/", "*.js");//std::vector<std::string> dummy = FileSystem::Directory::getFiles("../../Serveroutputfiles/", "*.js");
		for (size_t i = 0; i < dummy.size(); i++)
		{
			files.push_back(dummy[i]);
		}

		std::cout << "the size of files is" << files.size();
		for (size_t i = 0; i < files.size(); ++i)
		{
			std::string name;
			name = "../Serveroutputfiles/" + files[i];//name = "../../Serveroutputfiles/" + files[i];
			fullpath.push_back(name);
		}
		
			SocketConnecter sendfromserver;
			while (!sendfromserver.connect("localhost", 8090))
			{
				Show::write("\n client waiting to connect");
				::Sleep(100);
			}
			std::cout << "Connected to port-----------------8090";
			for (size_t i = 0; i< fullpath.size(); i++)
			{
				std::cout<<"\n\n  sending file " + files[i];
				sendFile(fullpath[i], sendfromserver);
			}

	}

}
//--------<interprets the message and checks whether delete message recieved for not------------------>
void ClientHandler::checkdeletemessage(HttpMessage msg)
{
	std::string msgcontent = msg.bodyString();
	std::size_t found = msgcontent.find("delete");
	if (found != std::string::npos)
	{
		std::vector<std::string> fullpath;
		std::set<char> delims{ '\n' };
		std::vector<std::string> fileneedtobedeleted = splitpath(msgcontent, delims);
		for (size_t i = 0; i < fileneedtobedeleted.size(); i++)
		{
			std::string name; name = "../Serveroutputfiles/" + fileneedtobedeleted[i];
			//name = "../../Serveroutputfiles/" + fileneedtobedeleted[i];
			fullpath.push_back(name);
		}
		for (size_t i = 0; i < fullpath.size(); i++)
		{
			std::string deletefilename = fullpath[i];
			char *filecontent;
			filecontent = (char *)malloc(deletefilename.size() + 1);
			memcpy(filecontent, deletefilename.c_str(), deletefilename.size() + 1);
			if (remove(filecontent) != 0)
			{
				std::cout << fileneedtobedeleted[i] + " not deleted \n";
			}
			else
			{
				std::cout << fileneedtobedeleted[i] + "deleted successfully\n";
			}
		}}
}
//<--------interprets the message and check whether download message recieved or not----------------->
void ClientHandler::checkdownloadmessage(HttpMessage msg)
{
	std::string msgcontent = msg.bodyString();std::size_t found = msgcontent.find("downloadfiles");
	if (found != std::string::npos)
	{
		std::cout << "\nentered downloading of files function\n";std::set<char> delims{ '\n' };
		std::vector<std::string> fileneedtobesent = splitpath(msgcontent, delims);
		std::vector<std::string> finalfilestobesent;
		std::vector<std::string> files = FileSystem::Directory::getFiles("../ServerInputFiles", "*"); //std::vector<std::string> files = FileSystem::Directory::getFiles("../../ServerInputFiles", "*");
		for (size_t i = 0; i < files.size(); i++)
		{
			std::string name; name = "..\\ServerInputFiles\\" + files[i]; files[i] = name;//std::string name;name = "..\\..\\ServerInputFiles\\" + files[i];files[i] = name;
		}
		maindpanalysis analysis;analysis.setfileset(files);
		analysis.finddependencies();
		std::vector<std::string> fullpath;
		NoSQLDB::NoSqlDb<std::string> db;db = analysis.getdatabase();//this db object contain all the dependencies
		for (size_t i = 0; i < fileneedtobesent.size(); i++)
		{
			replacesstring(".html", "", fileneedtobesent[i]);
			NoSQLDB::Element<std::string> elem = db.value(fileneedtobesent[i]);std::vector<std::string> temp = elem.children;
			for (size_t i = 0; i < temp.size(); i++)
			{
				std::vector<std::string>::iterator it;it = find(finalfilestobesent.begin(), finalfilestobesent.end(),temp[i]);
				if (it != finalfilestobesent.end())
				{}	
				else
				{
                    if(temp[i]!="")
					finalfilestobesent.push_back(temp[i]);
				}
			}
			finalfilestobesent.push_back(fileneedtobesent[i]);
		}
		for (size_t i = 0; i < finalfilestobesent.size(); i++)
		{
			std::string name; name = "../Serveroutputfiles/" + finalfilestobesent[i] + ".html";
			fullpath.push_back(name);//name = "../../Serveroutputfiles/" + finalfilestobesent[i]+".html";
		}
		SocketConnecter sendfromserver;
		while (!sendfromserver.connect("localhost", 8090))
		{
			Show::write("\n client waiting to connect");::Sleep(100);
		};
		std::cout << "Connected to port-----------------8090";
		for (size_t i = 1; i< fullpath.size(); i++)
		{
			if (fullpath[i] != "../Serveroutputfiles/.html")//if (fullpath[i] != "../../Serveroutputfiles/.html")
			{
				std::cout << "\n\n  sending file " + fullpath[i];sendFile(fullpath[i], sendfromserver);
			}}
		msg = makeMessage(1, "completed", "toAddr:localhost:8090");
		sendMessage(msg, sendfromserver);
	}
}
//<--------used for exchnaging messages for initial communication------------------------------->
void ClientHandler::checkinitialmessage(HttpMessage msg)
{
	std::string msgcontent = msg.bodyString();
	std::size_t found = msgcontent.find("sending file");
	if (found != std::string::npos)
	{
		SocketConnecter sendfromserver;
		while (!sendfromserver.connect("localhost", 8090))
		{
			Show::write("\n client waiting to connect"); ::Sleep(100);
		}
		
		msg = makeMessage(1, "send the file content client", "toAddr:localhost:8090");
		sendMessage(msg, sendfromserver);
	}

}
//------------<used for downloading css files to virtual directory------------------------------>
void ClientHandler::downloadcsstovirtualdirectory(std::string path)
{
	std::cout << "\nentered the download files to virtual directory\n";
	std::vector<std::string> fullpath;
	std::vector<std::string> files = FileSystem::Directory::getFiles("../Serveroutputfiles/", "*.css"); //std::vector<std::string> files = FileSystem::Directory::getFiles("../../Serveroutputfiles/", "*.css");
	std::vector<std::string> dummy = FileSystem::Directory::getFiles("../Serveroutputfiles/", "*.js"); //std::vector<std::string> dummy = FileSystem::Directory::getFiles("../../Serveroutputfiles/", "*.js");
	for (size_t i = 0; i < dummy.size(); i++)
	{
		files.push_back(dummy[i]);
	}
	
	for (size_t i = 0; i < files.size(); ++i)
	{
		std::string name;
		name = "../Serveroutputfiles/" + files[i];//name = "../../Serveroutputfiles/" + files[i];
		fullpath.push_back(name);
	}
	std::vector<std::string> virtualdirectorycss;
	virtualdirectorycss.push_back(path + "project3.css");
	virtualdirectorycss.push_back(path + "collapsescript.js");
	char ch;
	for (size_t i = 0; i < fullpath.size(); i++)
	{
		std::ifstream fs;
		std::ofstream ft;
		fs.open(fullpath[i]);
		if (!fs)
		{
			std::cout << "error occured";
		}
		ft.open(virtualdirectorycss[i]);
		if (!ft)
		{
			std::cout << "Error in opening target file..!!";
			fs.close();
		}
		while (fs.eof() == 0)
		{
			fs >> ch;
			ft << ch;
		}
		std::cout<<files[i] << "    successfully saved to virtual directory" << path <<std::endl;
		std::cout << "\n";
	}
	
}
//<------------used for retriving virtual directory contents---------------------------------------->
void ClientHandler::checkvmessage(HttpMessage msg, std::string dir, std::string port, std::string pathvlocal)
{
	if (msg.bodyString() == "retrivevirtualfilecontents")
	{
		std::string path = "http://localhost:" + port + "/" + dir+"/";
		std::vector<std::string> files = FileSystem::Directory::getFiles(pathvlocal, "*");
		std::vector<std::string> fullpath;
		for (size_t i = 0; i < files.size(); i++)
		{
			std::string name;
			name = path + files[i];
			fullpath.push_back(name);
		}
		std::string filelist = "startvfile";
		filelist = filelist + "\n";
		for (size_t i = 0; i < fullpath.size(); i++)
		{
			filelist = filelist + fullpath[i];
			filelist = filelist + "\n";
		}
		SocketConnecter sendfromserver;
		while (!sendfromserver.connect("localhost", 8090))
		{
			Show::write("\n client waiting to connect");
			::Sleep(100);
		}
		std::cout << "Connected to port-----------------8090";
		msg = makeMessage(1, filelist, "toAddr:localhost:8090");
		sendMessage(msg, sendfromserver);
		std::cout << "\nmessage sent from server side\n";
	}

}
//<------------used for sending the file to client----------------------------->
bool ClientHandler::sendFile(const std::string& filename, Socket& socket)
{
	// assumes that socket is connected
	std::string fqname = filename;
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
	{
		std::cout << "\npath is incorrect\n";
		return false;

	}
	HttpMessage msg = makeMessage(1, "", "localhost::8080");
	msg.addAttribute(HttpMessage::Attribute("file", filename));
	msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
	sendMessage(msg, socket);
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	while (true)
	{
		FileSystem::Block blk = file.getBlock(BlockSize);
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];
		socket.send(blk.size(), buffer);
		if (!file.isGood())
			break;
	}
	file.close();
	return true;
}
//----< this defines processing to frame messages>------------------
HttpMessage ClientHandler::readMessage(Socket& socket)
{
  connectionClosed_ = false;HttpMessage msg;
  while (true)
  {
    std::string attribString = socket.recvString('\n');
    if (attribString.size() > 1)
    {
      HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);msg.addAttribute(attrib);
    }
    else
    {
      break;
    }
  }
  if (msg.attributes().size() == 0)
  {
    connectionClosed_ = true;return msg;
  }
  if (msg.attributes()[0].first == "POST")
  {
    std::string filename = msg.findValue("file");
    if (filename != "")
    {
      size_t contentSize;std::string sizeString = msg.findValue("content-length");
      if (sizeString != "")
        contentSize = Converter<size_t>::toValue(sizeString);
      else
        return msg;
      readFile(filename, contentSize, socket);
    }

    if (filename != "")
    {
      msg.removeAttribute("content-length"); std::string bodyString = "<file>" + filename + "</file>";
      std::string sizeString = Converter<size_t>::toString(bodyString.size());msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
      msg.addBody(bodyString);
    }
    else
    {
      size_t numBytes = 0;size_t pos = msg.findAttribute("content-length");
      if (pos < msg.attributes().size())
      {
        numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
        Socket::byte* buffer = new Socket::byte[numBytes + 1];socket.recv(numBytes, buffer);buffer[numBytes] = '\0';std::string msgBody(buffer);
        msg.addBody(msgBody);delete[] buffer;
      }}}
  return msg;
}

//<-------------function for creating a http message------------------------->
HttpMessage ClientHandler::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	EndPoint myEndPoint = "localhost:8081";  // ToDo: make this a member of the sender
											 // given to its constructor.
	switch (n)
	{
	case 1:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("POST", "Message"));
		msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
		msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
		msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));
		msg.addBody(body);
		if (body.size() > 0)
		{
			attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
			msg.addAttribute(attrib);
		}
		break;
	default:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
	}
	return msg;
}

//<-------------function for sending a message to a client -------------------------------->
void ClientHandler::sendMessage(HttpMessage& msg, Socket& socket)
{
	std::string msgString = msg.toString();
	socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}

//----< used for calling the splitpath function to get the file name only >-------------
std::string  getfilenamen(std::string name)
{
	std::set<char> delims{ '\\' };
	std::vector<std::string> result = splitpath(name, delims);
	std::string mresult = result.back();
	return mresult;
}

//----< read a binary file from socket and save >--------------------
/*
 * This function expects the sender to have already send a file message, 
 * and when this function is running, continuosly send bytes until
 * fileSize bytes have been sent.
 */
bool ClientHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket)
{
  std::string name=getfilenamen(filename);
  std::string fqname = "../ServerInputFiles/" + name;// std::string fqname = "../../ServerInputFiles/"+ name;
  FileSystem::File file(fqname);
  file.open(FileSystem::File::out, FileSystem::File::binary);
  if (!file.isGood())
  {
    Show::write("\n\n  can't open file " + fqname);
    return false;
  }
  const size_t BlockSize = 2048;
  Socket::byte buffer[BlockSize];
  size_t bytesToRead;
  while (true)
  {
    if (fileSize > BlockSize)
      bytesToRead = BlockSize;
    else
      bytesToRead = fileSize;
    socket.recv(bytesToRead, buffer);
    FileSystem::Block blk;
    for (size_t i = 0; i < bytesToRead; ++i)
      blk.push_back(buffer[i]);

    file.putBlock(blk);
    if (fileSize < BlockSize)
      break;
    fileSize -= BlockSize;
  }
  file.close();
  return true;
}
//----< receiver functionality is defined by this function >---------
//<------------used for retriving the dependency filelist to server-------------------------->
std::string retrivedependencylist(std::string output, NoSQLDB::NoSqlDb<std::string> db)
{
	NoSQLDB::Element<std::string> elem = db.value(output);
	return NULL;
}
//<---------------interprets the message and perfoms corresponding actions-------------------->
void ClientHandler::operator()(Socket socket)
{
  while (true)
  {
    HttpMessage msg = readMessage(socket);
	std::cout << this << " : " << msg.bodyString() << std::endl;
    if (connectionClosed_ || msg.bodyString() == "quit")
    {
      Show::write("\n\n  clienthandler thread is terminating");
      break;
    }
	if (msg.bodyString() =="retrivefilecontents")
	{
		std::vector<std::string> files = FileSystem::Directory::getFiles("../Serveroutputfiles/", "*.html*");//std::vector<std::string> files = FileSystem::Directory::getFiles("../../Serveroutputfiles/", "*.html*");
		std::string filelist="startfile";filelist = filelist + "\n";
		for (size_t i = 0; i < files.size(); i++)
		{
			filelist = filelist + files[i];filelist = filelist + "\n";
		}
		SocketConnecter sendfromserver;msg = makeMessage(1, filelist, "toAddr:localhost:8090");
		while (!sendfromserver.connect("localhost", 8090))
		{
			Show::write("\n client waiting to connect");::Sleep(100);
		}
			std::cout << "Connected to port-----------------8090";
			msg = makeMessage(1, filelist, "toAddr:localhost:8090");sendMessage(msg, sendfromserver);
	}
	if (msg.bodyString() =="sendallfiles")
	{
		std::vector<std::string> fullpath;
		std::vector<std::string> files = FileSystem::Directory::getFiles("../Serveroutputfiles", "*.html");//std::vector<std::string> files = FileSystem::Directory::getFiles("../../Serveroutputfiles","*.html");
		for (size_t i = 0; i < files.size(); i++)
		{
			std::string name; name = "../Serveroutputfiles/" + files[i]; fullpath.push_back(name); //std::string name;name = "../../Serveroutputfiles/" + files[i];
		}
		SocketConnecter sendfromserver;
		while (!sendfromserver.connect("localhost", 8090))
		{
			Show::write("\n client waiting to connect");
			::Sleep(100);
		}
		std::cout << "Connected to port-----------------8090";
		for (size_t i= 0; i< fullpath.size(); i++)
		{
			Show::write("\n\n  sending file " + fullpath[i]);sendFile(fullpath[i], sendfromserver);
		}
	}
	checkmessage(msg);checkdeletemessage(msg);
	checkdownloadmessage(msg);checkinitialmessage(msg); msgQ_.enQ(msg);	
  }
}

//----< test stub >--------------------------------------------------
int main(int argc, char *argv[])
{
  ::SetConsoleTitle(L"HttpMessage Server - Runs Forever");
  Show::attach(&std::cout);Show::start();
  Show::title("\n  HttpMessage Server started");
  BlockingQueue<HttpMessage> msgQ;
  int i = 0;
  try
  {
    SocketSystem ss;
    SocketListener sl(8080, Socket::IP6);
    ClientHandler cp(msgQ);sl.start(cp);
	while (true)
	{
		HttpMessage msg = msgQ.deQ();
		if (msg.bodyString() != "publish")
		{
			Show::write("\n\n  server recvd message contents:\n" + msg.bodyString());
		}
		if (msg.bodyString() == "publish")
		{
			std::vector<std::string> fullpath;
			std::vector<std::string> files = FileSystem::Directory::getFiles("../ServerInputFiles", "*");//std::vector<std::string> files = FileSystem::Directory::getFiles("../../ServerInputFiles", "*");
			for (size_t i = 0; i < files.size(); i++)
			{
				std::string name = files[i]; name = "..\\ServerInputFiles\\" + name;
				fullpath.push_back(name);//name = "..\\..\\ServerInputFiles\\" + name;
			}
			NoSQLDB::NoSqlDb<std::string> db;maindpanalysis analysis;
			publisher pub(argv[1], "..\\Serveroutputfiles\\"); //publisher pub(argv[1], "..\\..\\Serveroutputfiles\\");
			analysis.setfileset(fullpath);analysis.finddependencies();db = analysis.getdatabase();
			pub.setfileset(fullpath);pub.publishfiles(db);
		}
		cp.checkvmessage(msg, argv[2], argv[3], argv[1]);
		if (i == 0)
		{
			cp.downloadcsstovirtualdirectory(argv[1]);
			i++;
		}
	}
    }
  
  catch (std::exception& exc)
  {
    std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
    Show::write(exMsg);
  }
}