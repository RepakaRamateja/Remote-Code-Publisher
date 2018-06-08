#ifndef WINDOW_H
#define WINDOW_H
///////////////////////////////////////////////////////////////////////////
// Window.h - C++\CLI implementation of WPF Application                  //
//          - Demo for CSE 687 Project #4                                //
// ver 4.0                                                               //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015             //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package demonstrates how to build a C++\CLI WPF application.  It
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with three views, two
*  of which are provided with functionality that you may need for Project #4.
*  It loads the DLL holding MockChannel.  MockChannel hosts a send queue, a
*  receive queue, and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  The Client can post a message to the MockChannel's send queue.  It hosts
*  a receive thread that reads the receive queue and dispatches any message
*  read to a ListBox in the Client's FileList tab.  So this Demo simulates
*  operations you will need to execute for Project #4.
*
*  Required Files:
*  -----------------------------------
*  Window.h, Window.cpp, MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*  HttpMessage.h,Utilities.h,HttpMessage.cpp,Utilities.cpp
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
*  ver4.0  :3rdmay 2016
  --added functionality for uploading files
  --added functionality for downloading files
  --added functionality for lazy download
  --added functionality for satisying IIS requirements
  --added functionality for deleting a files in server repo

*  ver 3.0 : 22 Apr 2016
*  - added support for multi selection of Listbox items.  Implemented by
*    Saurabh Patel.  Thanks Saurabh.
*  ver 2.0 : 15 Apr 2015
*  - completed message passing demo with moch channel
*  - added FileBrowserDialog to show files in a selected directory
*  ver 1.0 : 13 Apr 2015
*  - incomplete demo with GUI but not connected to mock channel
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/
using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#include "../CommPrototype/MsgClient/MsgClient.h"
#include <iostream>

namespace CppCliWindows
{
	ref class WPFCppCliDemo : Window
	{
		// MockChannel references
		ISendr* pSendr_;
		IRecvr* pRecvr_;
		IMockChannel* pChann_;
		Iclient *client;
		// Controls for Window
		DockPanel^ hDockPanel = gcnew DockPanel();      
		// support docking statusbar at bottom
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();
		TabItem^ hSendMessageTab = gcnew TabItem();
		TabItem^ hFileListTab = gcnew TabItem();
		TabItem^ hFileManageTab = gcnew TabItem();
		TabItem^ hclientrepoview = gcnew TabItem();//for client repo
		TabItem^ hDownload = gcnew TabItem();
		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();
		// Controls for SendMessage View
		Grid^ hSendMessageGrid = gcnew Grid();
		Button^ hSendButton = gcnew Button();
		Button^ hClearButton = gcnew Button();
		TextBlock^ hTextBlock1 = gcnew TextBlock();
		ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
		StackPanel^ hStackPanel1 = gcnew StackPanel();

		String^ msgText
			= "Command:ShowMessage\n"   // command
			+ "Sendr:localhost@8080\n"  // send address
			+ "Recvr:localhost@8090\n"  // receive address
			+ "Content-length:44\n"     // body length attribute
			+ "\n"                      // end header
			+ "Hello World\nCSE687 - Object Oriented Design";  // message body

															   // Controls for FileListView View
		Grid^ hFileListGrid = gcnew Grid();
		Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
		ListBox^ hListBox = gcnew ListBox();
		Button^ hFolderBrowseButton = gcnew Button();
		Button^ hShowItemsButton = gcnew Button();
		Button^ uploadButton = gcnew Button();
	
		                                             //controls for file download view
		Grid^ hGrid2 = gcnew Grid();
		Forms::FolderBrowserDialog^ hFolderBrowserDialog2 = gcnew Forms::FolderBrowserDialog();
		ListBox^ repolistBox = gcnew ListBox();
		Button^ hgetrepocontents = gcnew Button();
		Button^ hShowselectedItemsButton = gcnew Button();
		Button^ downloadButton = gcnew Button();
		Button^ entiredownload = gcnew Button();

		                                          //controls for the  IIS  view
		Grid^ hGrid3 = gcnew Grid();
		Button^ hgetreposcontents = gcnew Button();
		ListBox^ reposlistBox = gcnew ListBox();
		Button^ hShowselItemsButton = gcnew Button();
		Button^ deletebutton = gcnew Button();

		                                          //controls for client repository view
		Grid^ hGrid4 = gcnew Grid();
		Button^ hclientreposcontents = gcnew Button();
		ListBox^ clientreposlistBox = gcnew ListBox();
		Button^ openfiles = gcnew Button();


		// receive thread
		Thread^ recvThread;
	public:
		WPFCppCliDemo();
		~WPFCppCliDemo();
		void setUpStatusBar();
		void setUpTabControl();
		void setUpSendMessageView();
		void setUpFileListView();
		void setUpdownloadListView();
		void setUpConnectionView();
		void downloadcssfiles();
		void setupfilemanageview();
		void setupclientrepoview();
		void sendMessage(Object^ obj, RoutedEventArgs^ args);
		void addText(String^ msg);
		void getMessage();
		void clear(Object^ sender, RoutedEventArgs^ args);
		void getItemsFromList(Object^ sender, RoutedEventArgs^ args);
		void uploadfilestoserver(Object^ sender, RoutedEventArgs^ args);
		void getrepocontents(Object^ sender, RoutedEventArgs^ args);
		void getreposcontents(Object^ sender, RoutedEventArgs^ args);//used for getting repository contents for file management view
		void browseForFolder(Object^ sender, RoutedEventArgs^ args);
		void OnLoaded(Object^ sender, RoutedEventArgs^ args);
		void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
		void getselectedfiles(Object^ sender, RoutedEventArgs^ args);
		void getonlyselectedfiles(Object^ sender, RoutedEventArgs^ args);
		void getallrepo(Object^ sender, RoutedEventArgs^ args);
		void openfilesthroughbrowser(Object^ sender, RoutedEventArgs^ args);
		void checkcompletemessage(std::string msg);
		void getlocalrepocontents(Object^ sender, RoutedEventArgs^ args);
		void openclientselectedfiles(Object^ sender, RoutedEventArgs^ args);
		void iisreposelecteditems(Object^ sender, RoutedEventArgs^ args);
	private:
		std::string toStdString(String^ pStr);
		String^ toSystemString(std::string& str);
		void setTextBlockProperties();
		void setButtonsProperties();
	};
}
#endif