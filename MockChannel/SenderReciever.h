/////////////////////////////////////////////////////////////////////////
// SenderReciever.cpp - Demonstrates simple one-way HTTP messaging    //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////

/* 
Package operations
==========================================================================
This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
*
* It's purpose is to provide a very simple illustration of how to use
* the Socket Package provided for Project #4.
Public Interface :
=============================================================
*Sendr
postMessage(const HttpMessage& msg);                           // from gui we are sending to the sender
EnqMessage(const HttpMessage& msg);                            // from the sender to the socket
*Recvr
*HttpMessage getMessage();                                     //help in getting the message
*HttpMessage DeqMessage();                                      //help in dequeing the message
*Mockchannel                                              
*start()                                                        //startng the mock channel
*stop()                                                         //stoping the mock channel
*Build Process:
==============================
*Build commands(either one)
devenv MockChannel.dll

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


#pragma once
#include "MockChannel.h"
#include "Cpp11-BlockingQueue.h"
#include <string>
#include <thread>
#include <iostream>
#include"../CommPrototype/HttpMessage/HttpMessage.h"

using BQueue = BlockingQueue <HttpMessage>;
using bq = BlockingQueue<HttpMessage>;

//----< Sendr class - sender >--------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
	void postMessage(const HttpMessage& msg);     // from gui we are sending to the sender
	void EnqMessage(const HttpMessage& msg);   // from the sender to the scoket
	BQueue& queue();
	Sendr() {}
	Sendr(BlockingQueue<HttpMessage>&bq) {}

private:
	BQueue sendQ_;
	bq ipQ_;

};


//----< Recvr class - Receiver >--------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
	HttpMessage getMessage();
	BQueue& queue();
	HttpMessage DeqMessage();
	
	void Recv();
	Recvr(size_t port) : sourcePort(port) {}
	Recvr(size_t port, BlockingQueue<HttpMessage>&bq) {}
private:
	BQueue recvQ_;
	BQueue  opQ_;
	size_t sourcePort;
};

////////////////////////////////////////////////////////////////////

//----< MockChannel >--------------------------------------------------

class MockChannel : public IMockChannel
{
public:
	MockChannel(ISendr* pSendr, IRecvr* pRecvr);
	void start();
	void stop();
private:
	std::thread thread_;
	ISendr* pISendr_;
	IRecvr* pIRecvr_;
	BlockingQueue<HttpMessage> msgQ;
	bool stop_ = false;
};




