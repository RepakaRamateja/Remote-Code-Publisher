/////////////////////////////////////////////////////////////////////////
// MsgClient.cpp - Demonstrates simple one-way HTTP messaging          //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////
/* Package operations
=====================================================================
 * This package implements a client that sends HTTP style messages and
 * files to a server that simply displays messages and stores files.
 *
 * It's purpose is to provide a very simple illustration of how to use
 * the Socket Package provided for Project #4.
 */
/*
Public Interface :
=======================================================================
* MsgClient client;
* execute(std::vector<std::string> filelist);                           //this will be used by gui to send the vector of files
* sendmsgtoserver(std::string msg)                                      //used to send the message to server
* makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep) //used to make the customised message
* sendMessage(HttpMessage& msg, Socket& socket);                         //used for sending the message
* sendFile(const std::string& fqname, Socket& socket)                    //used for sending the file
* Required Files:
========================================================================
*   MsgClient.cpp, MsgServer.cpp
*   HttpMessage.h, HttpMessage.cpp
*   Cpp11-BlockingQueue.h
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp
*   Utilities.h, Utilities.cpp

*Build Process:
==============================
*Build commands(either one)
 devenv MsgClient.sln 
*/
/*
*Maintenance History :
==============================================
ver 1.0 : 3rd MAY 2017
first release

*/
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include <string>
#include <iostream>
#include <thread>
#include"MsgClient.h"

using Show = StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;

/////////////////////////////////////////////////////////////////////
// ClientCounter creates a sequential number for each client
//
class ClientCounter
{
public:
  ClientCounter() { ++clientCount; }
  size_t count() { return clientCount; }
private:
  static size_t clientCount;
};
size_t ClientCounter::clientCount = 0;
/////////////////////////////////////////////////////////////////////
// MsgClient class
// - was created as a class so more than one instance could be 
//   run on child thread
//
class MsgClient : public Iclient
{
public:
	using EndPoint = std::string;
	void execute(std::vector<std::string> filelist);
	void sendmsgtoserver(std::string msg);
	MsgClient();
private:
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket);
	SocketSystem ss;
	SocketConnecter si;
};

//<--------MSG CLIENT CONSTRUCTOR------------------------->
MsgClient::MsgClient()
{
	while (!si.connect("localhost", 8080))
	{
		Show::write("\n client waiting to connect");
		::Sleep(100);
	}
};
//------<send msg to server for retriving file contents>-----------------

//this is method that is exposed to the cpp cli app
void MsgClient::sendmsgtoserver(std::string rmsg)
{
	HttpMessage msg;
	msg = makeMessage(1, rmsg, "toAddr:localhost:8080");
	sendMessage(msg, si);
}

//----< factory for creating messages >------------------------------
/*
 * This function only creates one type of message for this demo.
 * - To do that the first argument is 1, e.g., index for the type of message to create.
 * - The body may be an empty string.
 * - EndPoints are strings of the form ip:port, e.g., localhost:8081. This argument
 *   expects the receiver EndPoint for the toAddr attribute.
 */
HttpMessage MsgClient::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
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
//----< send message using socket >----------------------------------
void MsgClient::sendMessage(HttpMessage& msg, Socket& socket)
{
  std::string msgString = msg.toString();
  socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}
//----< send file using socket >-------------------------------------
/*
 * - Sends a message to tell receiver a file is coming.
 * - Then sends a stream of bytes until the entire file
 *   has been sent.
 * - Sends in binary mode which works for either text or binary.
 */
bool MsgClient::sendFile(const std::string& filename, Socket& socket)
{
  std::string fqname =  filename;
  FileSystem::FileInfo fi(fqname);
  size_t fileSize = fi.size();
  std::string sizeString = Converter<size_t>::toString(fileSize);
  FileSystem::File file(fqname);
  file.open(FileSystem::File::in, FileSystem::File::binary);
  if (!file.isGood())
    return false;
  HttpMessage msg = makeMessage(1,"","localhost::8080");
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
//----< this defines the behavior of the client >--------------------
void MsgClient::execute(std::vector<std::string> filelist)
{
  // send NumMessages messages
  ClientCounter counter;
  size_t myCount = counter.count();
  std::string myCountString = Utilities::Converter<size_t>::toString(myCount);
  Show::attach(&std::cout);
  Show::start();
  Show::title(
    "Starting HttpMessage client" + myCountString + 
    " on thread " + Utilities::Converter<std::thread::id>::toString(std::this_thread::get_id())
  );
  try
  {
   SocketSystem ss;
    SocketConnecter si;
    while (!si.connect("localhost", 8080))
    {
      Show::write("\n client waiting to connect");
      ::Sleep(100);
    }
    // send a set of messages
    HttpMessage msg;
    for (size_t i = 0; i < filelist.size(); ++i)
    {
      Show::write("\n\n  sending file " + filelist[i]);
	  std::string name = "sending file" + filelist[i];
	  msg = makeMessage(1, name, "toAddr:localhost:8080");
	  std::cout << "sending message to client\n";
	  std::cout << "\n";
	  sendMessage(msg, si);
      sendFile(filelist[i], si);
    }
	msg = makeMessage(1, "publish", "toAddr:localhost:8080");
	sendMessage(msg, si);
    // shut down server's client handler
    msg = makeMessage(1, "quit", "toAddr:localhost:8080");
    sendMessage(msg, si);
    Show::write("\n\n  client" + myCountString + " sent\n" + msg.toIndentedString());
    Show::write("\n");
    Show::write("\n  All done folks");
  }
  catch (std::exception& exc)
  {
    Show::write("\n  Exeception caught: ");
    std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
    Show::write(exMsg);
  }
}
//<-----------used to get the client object when requested----------------->
Iclient*  clientFactory::getclient()
{
	return new MsgClient();
}

/*
//----< entry point - runs two clients each on its own thread >------
int main()
{
  ::SetConsoleTitle(L"Clients Running on Threads");
  Show::title("Demonstrating two HttpMessage Clients each running on a child thread");
  MsgClient c1;
  std::thread t1(
    [&]() { c1.execute(100, 20); } // 20 messages 100 millisec apart
  );

  MsgClient c2;
  std::thread t2(
    [&]() { c2.execute(120, 20); } // 20 messages 120 millisec apart
  );
  t1.join();
  t2.join();
}
*/
 
