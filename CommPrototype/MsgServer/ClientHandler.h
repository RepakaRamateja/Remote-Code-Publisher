#pragma once
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
/* Package operations
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
*
* It's purpose is to provide a very simple illustration of how to use
* the Socket Package provided for Project #4.
*/
/*
Public Interface :
=============================================================
*ClientmHandler cp(msgQ);
*sl.start(cp); --------                                                   -//it is used to create a new thread for handling request
*readcMessage----------                                                    //it is used for reading the given message
*readFile----------                                                        //it is used for reading the file
*sendFile(const std::string& fqname, Socket& socket)                       //it is used for sending the file
* makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep)    //used for making the http message
*checkinitialmessage(HttpMessage msg)                                      //used for checking the message body for performing operation
*checkdeletemessage(HttpMessage msg);                                      //used to check the delete message
*checkdownloadmessage(HttpMessage msg)                                     //used to check the download message
*checkvmessage(HttpMessage msg, std::string dir, std::string port, std::string pathvlocal)//checking the virtual directory message
*downloadcsstovirtualdirectory(std::string path)                          //download files to given virtual directory
*readMessage(Socket& socket)                                               //redaing the message from the socket
*checkmessage(HttpMessage msg);                                            //used for checking the message contents

*Build Process:
==============================
*Build commands(either one)
devenv MsgServer.sln

*Required files:
===============================================
MsgClient.cpp, MsgServer.cpp
HttpMessage.h, HttpMessage.cpp
Cpp11 - BlockingQueue.h
Sockets.h, Sockets.cpp
FileSystem.h, FileSystem.cpp
Logger.h, Logger.cpp
Utilities.h, Utilities.cpp

*Maintenance History :
==============================================
ver 1.0 : 3rd MAY 2017
first release
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
class ClientHandler
{
public:
	using EndPoint = std::string;
	ClientHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
	void checkvmessage(HttpMessage msg, std::string dir, std::string port, std::string pathvlocal);
	void downloadcsstovirtualdirectory(std::string path);
private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	void checkmessage(HttpMessage msg);
	void checkdeletemessage(HttpMessage msg);
	void checkdownloadmessage(HttpMessage msg);
	void checkinitialmessage(HttpMessage msg);
};
