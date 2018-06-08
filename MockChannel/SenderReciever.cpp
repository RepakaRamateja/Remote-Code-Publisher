/////////////////////////////////////////////////////////////////////////
// SenderReciever.cpp - Demonstrates simple two-way HTTP messaging     //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////

#pragma once
#define IN_DLL
#include"SenderReciever.h"
#include"../CommPrototype/Sockets/Sockets.h"
#include"../CommPrototype/HttpMessage/HttpMessage.h"
#include"../Clienthandler/clienthandler.h"
using namespace std;

//<----------used for posting the message----------------------->
void Sendr::postMessage(const HttpMessage& msg)
{
	sendQ_.enQ(msg);
}
//<----------used for enqueing the message to queue-------------------->
void Sendr::EnqMessage(const HttpMessage &msg)
{
	ipQ_.enQ(msg);
}
//<--------------used for returning a queue------------------------------->
BQueue& Sendr::queue() { return sendQ_; }

//----< Recvr class - receiver >--------------------------------------------------
HttpMessage Recvr::getMessage()
{
	//recvQ_.enQ(opQ_.deQ());
	return recvQ_.deQ();
}

//<---------used to return the queue---------------->
BQueue& Recvr::queue()
{
	return recvQ_;
}

//<------used for dequing the message-------------->
HttpMessage Recvr::DeqMessage()
{
	return opQ_.deQ();
}

//<------------recv class constructor--------------->
void Recvr::Recv()
{
	try
	{
		/*
		SocketSystem ss;
		SocketListener sl(sourcePort, Socket::IP6);
		ClientHandler cp(opQ_);
		sl.start(cp);*/
	}
	catch (std::exception& ex)
	{
		std::cout << "  Exception caught:" << ex.what() << "\n\n";
	}
}



//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {}

//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
	Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
	Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
	if (pSendr == nullptr || pRecvr == nullptr)
	{
		std::cout << "\n  failed to start Mock Channel\n\n";
		return;
	}
	BQueue& sendQ = pSendr->queue();
	BQueue& recvQ = pRecvr->queue();
	std::cout << "\n  MockChannel starting up";
	std::thread recvThread = std::thread(
		[&] {
		SocketSystem ss;
		SocketListener sl(8090, Socket::IP6);
		ClientmHandler cp(recvQ);
		HttpMessage msg;
		sl.start(cp);
		while (true) {
			//std::cout << "Listening.." << std::endl;
			msg = recvQ.deQ();
			if ( msg.bodyString() == "quit")
			{
				std::cout << "Stopped LIstening";
				break;
			}
			//std::cout << "Received on client :" << msg.bodyString() << std::endl;
				recvQ.enQ(msg);
		}
		
	});
	recvThread.detach();
	std::cout << "Detached" << std::endl;
}

//<-----------------used for stopping the mock channel------------------------------->
void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

//<--------used for creating sender---------------------------------->
ISendr* ObjectFactory::createSendr() { return new Sendr; }

//<-----------used to create reciever-------------------->
IRecvr* ObjectFactory::createRecvr() { return new Recvr(8090); }

//<----------used for creating mock channel--------------------->
IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr)
{
	return new MockChannel(pISendr, pIRecvr);
}

