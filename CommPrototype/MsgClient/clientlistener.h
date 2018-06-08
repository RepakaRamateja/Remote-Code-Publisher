#pragma once
/////////////////////////////////////////////////////////////////////////
// clientlistener.h - Demonstrates simple one-way HTTP style messaging //
//                 and file transfer                                   //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////
/* package operations
======================================================================
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
*
* It's purpose is to provide a very simple illustration of how to use
* the Socket Package provided for Project #4.
*/
/* Public Interface
============================================================================
* ClientHandler class
* readMessage(Socket& socket)                                            //used for reading message from socket
*readFile(const std::string& filename, size_t fileSize, Socket& socket)  //reading a file from the socket
*makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep)   //create a http customised message
*sendMessage(HttpMessage& msg, Socket& socket)                           //used for sending the messsage
* Required Files:
=======================================================================
*   MsgClient.cpp, MsgServer.cpp
*   HttpMessage.h, HttpMessage.cpp
*   Cpp11-BlockingQueue.h
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp
*   Utilities.h, Utilities.cpp
*/
/*Build Process:
==============================
*Build commands(either one)
 devenv MsgClient.dll 
*/
/*
* Maintenance History :
  ==============================================
* ver 1.0 : 3rd MAY 2017
* first release
* 
*
*
*/
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

using Show = StaticLogger<1>;
using namespace Utilities;

/////////////////////////////////////////////////////////////////////
// ClientHandler class
/////////////////////////////////////////////////////////////////////
// - instances of this class are passed by reference to a SocketListener
// - when the listener returns from Accept with a socket it creates an
//   instance of this class to manage communication with the client.
// - You need to be careful using data members of this class
//   because each client handler thread gets a reference to this 
//   instance so you may get unwanted sharing.
// - I may change the SocketListener semantics (this summer) to pass
//   instances of this class by value.
// - that would mean that all ClientHandlers would need either copy or
//   move semantics.
//
class ClientHandler
{
public:
	using EndPoint = std::string;
	ClientHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
	SocketConnecter sendfromserver;

private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
};



//----< this defines processing to frame messages>------------------
HttpMessage ClientHandler::readMessage(Socket& socket)
{
	connectionClosed_ = false;
	HttpMessage msg;
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
				return msg;readFile(filename, contentSize, socket);
		}

		if (filename != "")
		{
			msg.removeAttribute("content-length");std::string bodyString = "<file>" + filename + "</file>";
			std::string sizeString = Converter<size_t>::toString(bodyString.size());
			msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));msg.addBody(bodyString);
		}
		else
		{
			size_t numBytes = 0;size_t pos = msg.findAttribute("content-length");
			if (pos < msg.attributes().size())
			{
				numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
				Socket::byte* buffer = new Socket::byte[numBytes + 1];socket.recv(numBytes, buffer);
				buffer[numBytes] = '\0';std::string msgBody(buffer);msg.addBody(msgBody);delete[] buffer;
			}
		}
	}
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


//----< read a binary file from socket and save >--------------------
/*
* This function expects the sender to have already send a file message,
* and when this function is running, continuosly send bytes until
* fileSize bytes have been sent.
*/
bool ClientHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket)
{
	std::string name = getfilenamen(filename);
	std::string fqname = "../../ServerInputFiles/" + name;

	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{
		/*
		* This error handling is incomplete.  The client will continue
		* to send bytes, but if the file can't be opened, then the server
		* doesn't gracefully collect and dump them as it should.  That's
		* an exercise left for students.
		*/
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
}
//----< receiver functionality is defined by this function >---------

//<-----read the message and performs actions accordingly-------------------------->
void ClientHandler::operator()(Socket socket)
{
	/*
	* There is a potential race condition due to the use of connectionClosed_.
	* If two clients are sending files at the same time they may make changes
	* to this member in ways that are incompatible with one another.  This
	* race is relatively benign in that it simply causes the readMessage to
	* be called one extra time.
	*
	* The race is easy to fix by changing the socket listener to pass in a
	* copy of the clienthandler to the clienthandling thread it created.
	* I've briefly tested this and it seems to work.  However, I did not want
	* to change the socket classes this late in your project cycle so I didn't
	* attempt to fix this.
	*/
	while (true)
	{
		HttpMessage msg = readMessage(socket);
		if (connectionClosed_ || msg.bodyString() == "quit")
		{
			Show::write("\n\n  clienthandler thread is terminating");
			break;
		}
		if (msg.bodyString() == "retrivefilecontents")
		{
			std::cout << "\nplease wait iam giving the file contents\n";
			std::vector<std::string> files = FileSystem::Directory::getFiles("../../Serveroutputfiles/", "*");
			std::string filelist;
			for (int i = 0; i < files.size(); i++)
			{
				filelist = filelist + files[i];
				filelist = filelist + "\n";
			}


		}
		msgQ_.enQ(msg);
	}
}

//<----used for adding listening capability to the client---------->
void startlistening()
{
		Show::attach(&std::cout);
		Show::start();
		Show::title("\n  HttpMessage client listener started");
		BlockingQueue<HttpMessage> msgQ;
		try
		{
			SocketSystem ss;
			SocketListener sl(8090, Socket::IP6);
			ClientHandler cp(msgQ);
			sl.start(cp);
			/*
			* Since this is a server the loop below never terminates.
			* We could easily change that by sending a distinguished
			* message for shutdown.
			*/

			while (true)
			{
				HttpMessage msg = msgQ.deQ();
				Show::write("\n\n  client  recvd message contents:\n" + msg.bodyString());

			}

		}
		catch (std::exception& exc)
		{
			Show::write("\n  Exeception caught: ");
			std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
			Show::write(exMsg);
		}

	
}