#pragma once
/////////////////////////////////////////////////////////////////////
// ClientmHandler class
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

/* package operations
===================
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
* It's purpose is to provide a very simple illustration of how to use
* the Socket Package provided for Project #4.
*
* Public Interface:
=====================
*     ClientmHandler cp(msgQ);
*     sl.start(cp); ---------//it is used to create a new thread for handling request
*     readcMessage  ----------//it is used for reading the given message
*     readFile      ----------//it is used for reading the file

*    Build Process:
==============================
* Build commands (either one)
devenv Clienthandler.sln
*   Required files:
================================
MsgClient.cpp, MsgServer.cpp
HttpMessage.h, HttpMessage.cpp
Cpp11-BlockingQueue.h
Sockets.h, Sockets.cpp
FileSystem.h, FileSystem.cpp
Logger.h, Logger.cpp
Utilities.h, Utilities.cpp

*  Maintenance History:
=================================
ver 1.0 : 3rd MAY 2017
first release
*
*
*/
#include"../CommPrototype/HttpMessage/HttpMessage.h"
#include "../CommPrototype/HttpMessage/HttpMessage.h"
#include "../CommPrototype/Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../CommPrototype/Logger/Cpp11-BlockingQueue.h"
#include "../CommPrototype/Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include <string>
#include <iostream>
class ClientmHandler
{
public:
	using EndPoint = std::string;
	ClientmHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
	bool connectionClosed_;
	HttpMessage readcMessage(Socket& socket);
	
private:
	
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;
	
};

