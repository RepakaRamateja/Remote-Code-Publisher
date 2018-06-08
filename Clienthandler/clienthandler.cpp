///////////////////////////////////////////////////////////////
// clienthandler.cpp - creates html files from given c++ source files//
// Language:    Visual C++ 2012                              //
// Platform:    Macbook pro  Windows 7                       //
// Application:  CSE687 - OOD, Pr#4, Spring 2017             //
// Author:      RamatejaRepaka,         Syracuse University  //
//              (315) 395-9575, rrepaka@syr.edu              //
///////////////////////////////////////////////////////////////


#include"clienthandler.h"
#include"../CommPrototype/HttpMessage/HttpMessage.h"
#include "../CommPrototype/Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../CommPrototype/Logger/Cpp11-BlockingQueue.h"
#include "../CommPrototype/Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include <string>
#include <iostream>
using namespace Utilities;
#include<set>

#include"../MockChannel/MockChannel.h"

//<---------used for reading the message ------------------------>
HttpMessage ClientmHandler::readcMessage(Socket& socket)
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
		connectionClosed_ = true;
		return msg;
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
			msg.removeAttribute("content-length");std::string bodyString = "<file>" + filename + "</file>";std::string sizeString = Converter<size_t>::toString(bodyString.size());
			msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));msg.addBody(bodyString);
		}
		else
		{
			size_t numBytes = 0;size_t pos = msg.findAttribute("content-length");
			if (pos < msg.attributes().size())
			{
				numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);Socket::byte* buffer = new Socket::byte[numBytes + 1];
				socket.recv(numBytes, buffer);buffer[numBytes] = '\0';std::string msgBody(buffer);msg.addBody(msgBody);delete[] buffer;
			}
		}
	}
	return msg;
}

//----< read a binary file from socket and save >--------------------
/*
* This function expects the sender to have already send a file message,
* and when this function is running, continuosly send bytes until
* fileSize bytes have been sent.
*/
//----< splits the given string with the  given delimiters and return vector<std::string> >--------------------
std::vector<std::string> splitpaths(
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

//<------------------used for reading a file----------------------------->
bool ClientmHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket)
{
	std::set<char> delims{ '/' };
	std::vector<std::string> result = splitpaths(filename, delims);
	std::string mresult = result.back();
	std::string fqname = "../clienthtmlfiles/"+ mresult;
	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{
		std::cout << "\n\n  can't open file" << fqname;
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
	std::cout << "\nsaving the obtained html file in the location\n"<< fqname;
	return true;
}

//----< receiver functionality is defined by this function >---------
void ClientmHandler::operator()(Socket socket)
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
		HttpMessage msg = readcMessage(socket);
		if (connectionClosed_ || msg.bodyString() == "quit")
		{
			std::cout<<"\n\n  clienthandler thread is terminating";
			break;
		}
		msgQ_.enQ(msg);
	}
}



//<-------test stub for the client handler----------------->
#ifdef testclient
int main()
{
	std::cout << "\nclient handler stub\n";
	std::cout << "\nstarted listening 7080\n";
	std::cout << "\nThis always waits for the message\n";
	std::cout << "\nwhen the msg comes then it interprets the message\n";
	std::cout << "\n does action basing on the interpretation\n";
	BlockingQueue<HttpMessage> msgQ;

	try
	{
		SocketSystem ss;
		SocketListener sl(7080, Socket::IP6);
		ClientmHandler cp(msgQ);
		sl.start(cp);
		/*
		* Since this is a server the loop below never terminates.
		* We could easily change that by sending a distinguished
		* message for shutdown.
		*/
		while (true)
		{
			HttpMessage msg = msgQ.deQ();
			std::cout<<"recieved message"<<msg.bodyString();
		}
	}
	catch (std::exception& exc)
	{
		std::cout << "\n  Exeception caught:";
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		
	}
	
	return true;
}
#endif
