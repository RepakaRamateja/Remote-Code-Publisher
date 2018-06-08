/////////////////////////////////////////////////////////////////////////
//MsgClient.h - used by the gui to send files and messages to the server//
// Ramatejarepaka, CSE687 - Object Oriented Design, Spring 2016        //
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
* execute(std::vector<std::string> filelist)                       //used for sending the vector<filelist> to the server
*sendmsgtoserver(std::string msg)                                  //used for sending the message to the server
*getclient()                                                       //used to get the instnace of a client

*REQUIRED FILES
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
devenv MsgClient.sln
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
#pragma warning( disable : 4273)
#pragma once
#include<vector>


#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif  

struct Iclient
{
	virtual void execute(std::vector<std::string> filelist) = 0;
	virtual void sendmsgtoserver(std::string msg) = 0;
};

extern "C" {
	struct clientFactory
	{
		DLL_DECL Iclient* getclient();
	};
}


	

