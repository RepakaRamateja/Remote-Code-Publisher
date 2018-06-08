/////////////////////////////////////////////////////////////////
// Publisher.h - creates html files from given c++ source files//
// Language:    Visual C++ 2012                                //
//  Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016  //
// Application: OOD Project #4                                 //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro//
///////////////////////////////////////////////////////////////
#include "Window.h"
#include<string>
#include <vector>
#include<set>
#include"../CommPrototype/HttpMessage/HttpMessage.h"
using namespace CppCliWindows;
//<----------constructor used for setting up the user interface at startup---------------->
WPFCppCliDemo::WPFCppCliDemo()
{
	// set up channel
	ObjectFactory* pObjFact = new ObjectFactory;
	pSendr_ = pObjFact->createSendr();
	pRecvr_ = pObjFact->createRecvr();
	pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
	pChann_->start();
	delete pObjFact;
	// client's receive thread
	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();
	// set event handlers
	this->Loaded +=
		gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing +=
		gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hSendButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendMessage);
	hClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
	hShowItemsButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getItemsFromList);
	uploadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::uploadfilestoserver);
	hgetrepocontents->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getrepocontents);
	downloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getselectedfiles);
	hShowselectedItemsButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getonlyselectedfiles);
	entiredownload->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getallrepo);
	hgetreposcontents->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getreposcontents);
	deletebutton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::openfilesthroughbrowser);
	hclientreposcontents->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getlocalrepocontents);
	openfiles->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::openclientselectedfiles);
	hShowselItemsButton->Click +=gcnew RoutedEventHandler(this, &WPFCppCliDemo::iisreposelecteditems);
	// set Window properties
	this->Title = "WPF C++/CLI Demo";
	this->Width = 800;
	this->Height = 600;
	// attach dock panel to Window
	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);
	// setup Window controls and views
	setUpTabControl();
	setUpStatusBar();
	setUpSendMessageView();
	setUpFileListView();
	setUpConnectionView();
	setUpdownloadListView();
	setupfilemanageview();
	setupclientrepoview();
	downloadcssfiles();//used for downloading css and java script files
}

//<------------used for setting up file management view----------------------------->
void CppCliWindows::WPFCppCliDemo::setupfilemanageview()
{
	Console::Write("\n  setting up FileM view");
	hGrid3->Margin = Thickness(20);
	hFileManageTab->Content = hGrid3;
	RowDefinition^ hRows1Def = gcnew RowDefinition();
	hRows1Def->Height = GridLength(100);
	hGrid3->RowDefinitions->Add(hRows1Def);
	Border^ hBorders1 = gcnew Border();
	hBorders1->BorderThickness = Thickness(1);
	hBorders1->BorderBrush = Brushes::Black;
	reposlistBox->SelectionMode = SelectionMode::Multiple;
	hBorders1->Child = reposlistBox;
	hGrid3->SetRow(hBorders1, 0);
	hGrid3->Children->Add(hBorders1);
	RowDefinition^ hRows2Def = gcnew RowDefinition();
	hRows2Def->Height = GridLength(75);
	RowDefinition^ hRows2Def2 = gcnew RowDefinition();
	hRows2Def2->Height = GridLength(75);
	RowDefinition^ hRows2Def3 = gcnew RowDefinition();
	hRows2Def3->Height = GridLength(75);
	hGrid3->RowDefinitions->Add(hRows2Def);
	hGrid3->RowDefinitions->Add(hRows2Def2);
	hGrid3->RowDefinitions->Add(hRows2Def3);
	hgetreposcontents->Content = "get vdir info";
	hgetreposcontents->Height = 20;
	hgetreposcontents->Width = 120;
	hgetreposcontents->BorderThickness = Thickness(2);
	hgetreposcontents->BorderBrush = Brushes::Black;
	hGrid3->SetRow(hgetreposcontents, 1);
	hGrid3->Children->Add(hgetreposcontents);
	hShowselItemsButton->Content = "Show Selected Items";
	hShowselItemsButton->Height = 20;
	hShowselItemsButton->Width = 120;
	hShowselItemsButton->BorderThickness = Thickness(2);
	hShowselItemsButton->BorderBrush = Brushes::Black;
	hGrid3->SetRow(hShowselItemsButton, 2);
	hGrid3->Children->Add(hShowselItemsButton);
	deletebutton->Content = "open files";
	deletebutton->Height = 20;
	deletebutton->Width = 120;
	deletebutton->BorderThickness = Thickness(2);
	deletebutton->BorderBrush = Brushes::Black;
	hGrid3->SetRow(deletebutton, 3);
	hGrid3->Children->Add(deletebutton);
}

//<-----------------used for setting up the client repo view------------------------>
void CppCliWindows::WPFCppCliDemo::setupclientrepoview()
{
	Console::Write("\n  setting up clientrepo view");
	hGrid4->Margin = Thickness(20);
	hclientrepoview->Content = hGrid4;
	RowDefinition^ hRowss1Def = gcnew RowDefinition();
	hRowss1Def->Height = GridLength(100);
	hGrid4->RowDefinitions->Add(hRowss1Def);
	Border^ hBorderss1 = gcnew Border();
	hBorderss1->BorderThickness = Thickness(1);
	hBorderss1->BorderBrush = Brushes::Black;
	clientreposlistBox->SelectionMode = SelectionMode::Multiple;
	hBorderss1->Child = clientreposlistBox;
	hGrid4->SetRow(hBorderss1, 0);
	hGrid4->Children->Add(hBorderss1);

	RowDefinition^ hRowss2Def = gcnew RowDefinition();
	hRowss2Def->Height = GridLength(75);
	RowDefinition^ hRowss2Def2 = gcnew RowDefinition();
	hRowss2Def2->Height = GridLength(75);

	hGrid4->RowDefinitions->Add(hRowss2Def);
	hGrid4->RowDefinitions->Add(hRowss2Def2);

	// Show selected items button.
	hclientreposcontents->Content = "getlocalrepocontents";
	hclientreposcontents->Height = 20;
	hclientreposcontents->Width = 120;
	hclientreposcontents->BorderThickness = Thickness(2);
	hclientreposcontents->BorderBrush = Brushes::Black;
	hGrid4->SetRow(hclientreposcontents, 1);
	hGrid4->Children->Add(hclientreposcontents);

	//for the entire directorydownload

	openfiles->Content = "open files";
	openfiles->Height = 20;
	openfiles->Width = 120;
	openfiles->BorderThickness = Thickness(2);
	openfiles->BorderBrush = Brushes::Black;
	hGrid4->SetRow(openfiles, 2);
	hGrid4->Children->Add(openfiles);



}

//<----------------destructor------------------------>
WPFCppCliDemo::~WPFCppCliDemo()
{
	delete pChann_;
	delete pSendr_;
	delete pRecvr_;
}

//<---------------used for settingup status bar---------->
void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "very important messages will appear here";
	//status->FontWeight = FontWeights::Bold;
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

//<--------------used for setting up tab control---------->
void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hSendMessageTab->Header = "Send Message";
	hFileListTab->Header = " upload File List";
	hFileManageTab->Header = "IIS";
	hDownload->Header = "Download";
	hclientrepoview->Header = "ClientlocalRepo";
	//hTabControl->Items->Add(hSendMessageTab);
	hTabControl->Items->Add(hFileListTab);
	hTabControl->Items->Add(hDownload);
	hTabControl->Items->Add(hFileManageTab);
	hTabControl->Items->Add(hclientrepoview);
}

//<---------------used for seeting textblockproperties-------->
void WPFCppCliDemo::setTextBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock1;
	hTextBlock1->Padding = Thickness(15);
	hTextBlock1->Text = "";
	hTextBlock1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock1->FontWeight = FontWeights::Bold;
	hTextBlock1->FontSize = 16;
	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer1->Content = hBorder1;
	hSendMessageGrid->SetRow(hScrollViewer1, 0);
	hSendMessageGrid->Children->Add(hScrollViewer1);
}
//<---------------used for setting buttons properties--------->
void WPFCppCliDemo::setButtonsProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hSendMessageGrid->RowDefinitions->Add(hRow2Def);
	hSendButton->Content = "Send Message";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hClearButton->Content = "Clear";
	hBorder2->Child = hSendButton;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hClearButton;
	hStackPanel1->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel1->Children->Add(hSpacer);
	hStackPanel1->Children->Add(hBorder3);
	hStackPanel1->Orientation = Orientation::Horizontal;
	hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hSendMessageGrid->SetRow(hStackPanel1, 1);
	hSendMessageGrid->Children->Add(hStackPanel1);
}

//<---------------used for setting send message view----------->
void WPFCppCliDemo::setUpSendMessageView()
{
	Console::Write("\n  setting up sendMessage view");
	hSendMessageGrid->Margin = Thickness(20);
	hSendMessageTab->Content = hSendMessageGrid;
	setTextBlockProperties();
	setButtonsProperties();
}

//<--------------used to convert to standard string-------------->
std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}

//<--------------used for sending message------------------------->
void WPFCppCliDemo::sendMessage(Object^ obj, RoutedEventArgs^ args)
{
	HttpMessage msg;
	pSendr_->postMessage(msg);
	Console::Write("\n  sent message");
	hStatus->Text = "Sent message";
}

//<------------used to convert to cppcli string-------------------->
String^ WPFCppCliDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
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

//<----------when the download complete then automatically browser open the files in the client directory----->
void WPFCppCliDemo::checkcompletemessage(std::string msg)
{
	std::size_t found = msg.find("completed");
	if (found != std::string::npos)
	{
		array<String^>^ files = System::IO::Directory::GetFiles("../clienthtmlfiles/", L"*.html*");
		for (int i = 0; i < files->Length; ++i)
		{
			std::string temp = toStdString(files[i]);
			std::string command("start \"\" \"" + temp + "\"");
			std::system(command.c_str());
		}
		
	}

}
//<------------used for getting local repo contents------------------------------->
void CppCliWindows::WPFCppCliDemo::getlocalrepocontents(Object ^ sender, RoutedEventArgs ^ args)
{
	String^ path = "../clienthtmlfiles/";
	array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.html*");
	for (int i = 0; i < files->Length; ++i)
	{
		clientreposlistBox->Items->Add(files[i]);
	}
	hStatus->Text = "\ndisplaying local repo contents\n";

}
//<--------------used to open selected files through browser------------------------>
void CppCliWindows::WPFCppCliDemo::openclientselectedfiles(Object ^ sender, RoutedEventArgs ^ args)
{
	int count = clientreposlistBox->SelectedItems->Count;  //get the count list of items in the repolistbox
	int index = 0;
	array<System::String^>^ items = gcnew array<String^>(count);
	std::string val;
	std::vector<std::string> openfilelist;
	if (count > 0)
	{
		for each (String^ item in clientreposlistBox->SelectedItems)
		{
			items[index++] = item;
			val = toStdString(item);
			openfilelist.push_back(val);
		}
	}
	for (size_t i = 0; i < openfilelist.size(); i++)
	{
		std::string path = openfilelist[i];
			std::string command("start \"\" \"" + path + "\"");
			std::system(command.c_str());
	}
	hStatus->Text = "opening files";
}
//<----------used for selecting only few contents out of all client repo contents-------------->
void CppCliWindows::WPFCppCliDemo::iisreposelecteditems(Object ^ sender, RoutedEventArgs ^ args)
{
	int index = 0;
	int count = reposlistBox->SelectedItems->Count;
	hStatus->Text = "Show Selected Items";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in reposlistBox->SelectedItems)
		{
			items[index++] = item;
		}
	}
	reposlistBox->Items->Clear();
	if (count > 0) {
		for each (String^ item in items)
		{
			reposlistBox->Items->Add(item);
		}
	}

	hStatus->Text = "displaying selected items";
}
//<-------------used for adding text recieved dynamically through gui-------------------------->
void WPFCppCliDemo::addText(String^ msg)
{
	std::string msgs = toStdString(msg);
	std::size_t found = msgs.find("startfile");
	if (found != std::string::npos)
	{
		repolistBox->Items->Clear();
		std::string filenames = toStdString(msg);
		std::set<char> delims{ '\n' };
		std::vector<std::string> filelist = splitpath(filenames, delims);
		for (size_t i = 1; i < filelist.size(); i++)
		{
			String^ results = toSystemString(filelist[i]);
			repolistBox->Items->Add(results);
		}
	}

	std::size_t found1 = msgs.find("startvfile");
	if (found1 != std::string::npos)
	{
		repolistBox->Items->Clear();
		std::string filenames = toStdString(msg);
		std::set<char> delims{ '\n' };
		std::vector<std::string> filelist = splitpath(filenames, delims);
		for (size_t i = 1; i < filelist.size(); i++)
		{
			String^ results = toSystemString(filelist[i]);
			reposlistBox->Items->Add(results);
		}
	}
	
	checkcompletemessage(msgs);
      
}
//<-------------used for getting message---------------------------------------->
void WPFCppCliDemo::getMessage()
{
	// recvThread runs this function
	while (true)
	{
		HttpMessage msg = pRecvr_->getMessage();
		//std::cout << msg.bodyString();
		String^ sMsg = toSystemString(msg.bodyString());
		array<String^>^ args = gcnew array<String^>(1);
		args[0] = sMsg;
		Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
		Dispatcher->Invoke(act, args);  // must call addText on main UI thread
	}
}
//<--------------used to clear the gui--------------------------------------------->
void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hTextBlock1->Text = "";
}

//<--------used for getting selected items from the list---------------------------->
void WPFCppCliDemo::getItemsFromList(Object^ sender, RoutedEventArgs^ args)
{
	int index = 0;
	int count = hListBox->SelectedItems->Count;
	hStatus->Text = "Show Selected Items";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in hListBox->SelectedItems)
		{
			items[index++] = item;
		}
	}

	hListBox->Items->Clear();
	if (count > 0) {
		for each (String^ item in items)
		{
			hListBox->Items->Add(item);
		}
	}
}

//<-----------used for uploading files to the server---------------------------->
void WPFCppCliDemo::uploadfilestoserver(Object^ sender,RoutedEventArgs^ args)
{

	int count = hListBox->Items->Count;
	int index = 0;
	array<System::String^>^ items = gcnew array<String^>(count);
	std::string val;
	std::vector<std::string> vect;
	if (count > 0) {
		for each (String^ item in hListBox->Items)
		{
			items[index++] = item;
			val=toStdString(item);
			vect.push_back(val);
		}
	}
	Console::WriteLine("\nhey i reached this function\n");
	clientFactory *fact = new clientFactory();
	client = fact->getclient();
	client->execute(vect);
	hStatus->Text = "fileuploaddone";
}


//<----used for sending message to the server to get the repository contents----------->
void CppCliWindows::WPFCppCliDemo::getrepocontents(Object ^ sender, RoutedEventArgs ^ args)
{
	clientFactory *fact = new clientFactory();
	client = fact->getclient();
	client->sendmsgtoserver("retrivefilecontents");
	hStatus->Text = "displaying repository contents";
}
//<-----------------------used for getting repository contents for file management view---------------->
void CppCliWindows::WPFCppCliDemo::getreposcontents(Object ^ sender, RoutedEventArgs ^ args)
{
	reposlistBox->Items->Clear();
	clientFactory *fact = new clientFactory();
	client = fact->getclient();
	client->sendmsgtoserver("retrivevirtualfilecontents");
	hStatus->Text = "getting virtual directory contents";
}

//<---------------------download the files using lazy download option---------------------------------->
void CppCliWindows::WPFCppCliDemo::getselectedfiles(Object ^ sender, RoutedEventArgs ^ args)
{
	int count = repolistBox->SelectedItems->Count;  //get the count list of items in the repolistbox
	int index = 0;
	array<System::String^>^ items = gcnew array<String^>(count);
	std::string val;
	std::vector<std::string> downloadfilelist;
	if (count > 0)
	{
		for each (String^ item in repolistBox->SelectedItems)
		{
			items[index++] = item;
			val = toStdString(item);
			downloadfilelist.push_back(val);
		}
	}
	std::string filenames="downloadfiles";
	filenames = filenames + "\n";

	for (size_t i = 0; i < downloadfilelist.size(); i++)
	{
		filenames = filenames + downloadfilelist[i];
		filenames = filenames + "\n";
	}

	clientFactory *fact = new clientFactory();
	client = fact->getclient();
	client->sendmsgtoserver(filenames);

	hStatus->Text = "lazy download";
	
}

//<-------used for settingupfilelistview---------------------------------------->
void WPFCppCliDemo::setUpFileListView()
{
	Console::Write("\n  setting up FileList view");
	hFileListGrid->Margin = Thickness(20);
	hFileListTab->Content = hFileListGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	//hRow1Def->Height = GridLength(75);
	hFileListGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hListBox->SelectionMode = SelectionMode::Multiple;
	hBorder1->Child = hListBox;
	hFileListGrid->SetRow(hBorder1, 0);
	hFileListGrid->Children->Add(hBorder1);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	RowDefinition^ hRow2Def2 = gcnew RowDefinition();
	hRow2Def2->Height = GridLength(75);
	RowDefinition^ hRow2Def3 = gcnew RowDefinition();
	hRow2Def3->Height = GridLength(75);
	hFileListGrid->RowDefinitions->Add(hRow2Def);
	hFileListGrid->RowDefinitions->Add(hRow2Def2);
	hFileListGrid->RowDefinitions->Add(hRow2Def3);
	hFolderBrowseButton->Content = "Select Directory";
	hFolderBrowseButton->Height = 20;
	hFolderBrowseButton->Width = 120;
	hFolderBrowseButton->BorderThickness = Thickness(2);
	hFolderBrowseButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hFolderBrowseButton, 1);
	hFileListGrid->Children->Add(hFolderBrowseButton);
	hShowItemsButton->Content = "Show Selected Items";
	hShowItemsButton->Height = 20;hShowItemsButton->Width = 120;
	hShowItemsButton->BorderThickness = Thickness(2);
	hShowItemsButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hShowItemsButton, 2);
	hFileListGrid->Children->Add(hShowItemsButton);uploadButton->Content = "upload files";
	uploadButton->Height = 20;
	uploadButton->Width = 120;
	uploadButton->BorderThickness = Thickness(2);
	uploadButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(uploadButton, 3);
	hFileListGrid->Children->Add(uploadButton);
	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

//<-------------------used for setting up download listview---------------------->
void WPFCppCliDemo::setUpdownloadListView()
{
	hGrid2->Margin = Thickness(20);hDownload->Content = hGrid2;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hGrid2->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);hBorder1->BorderBrush = Brushes::Black;
	repolistBox->SelectionMode = SelectionMode::Multiple;
	hBorder1->Child = repolistBox;hGrid2->SetRow(hBorder1, 0);
	hGrid2->Children->Add(hBorder1);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	RowDefinition^ hRow2Def2 = gcnew RowDefinition();hRow2Def2->Height = GridLength(75);
	RowDefinition^ hRow2Def3 = gcnew RowDefinition();
	hRow2Def3->Height = GridLength(75);
	RowDefinition^ hRow2Def4 = gcnew RowDefinition();hRow2Def4->Height = GridLength(75);
	hGrid2->RowDefinitions->Add(hRow2Def);
	hGrid2->RowDefinitions->Add(hRow2Def2);
	hGrid2->RowDefinitions->Add(hRow2Def3);
	hGrid2->RowDefinitions->Add(hRow2Def4);
	hgetrepocontents->Content = "get contents";
	hgetrepocontents->Height = 20;
	hgetrepocontents->Width = 120;
	hgetrepocontents->BorderThickness = Thickness(2);
	hgetrepocontents->BorderBrush = Brushes::Black;
	hGrid2->SetRow(hgetrepocontents, 1);hGrid2->Children->Add(hgetrepocontents);
	hShowselectedItemsButton->Content = "delete Items";
	hShowselectedItemsButton->Height = 20;
	hShowselectedItemsButton->Width = 120;
	hShowselectedItemsButton->BorderThickness = Thickness(2);
	hShowselectedItemsButton->BorderBrush = Brushes::Black;
	hGrid2->SetRow(hShowselectedItemsButton, 2);
	hGrid2->Children->Add(hShowselectedItemsButton);
	downloadButton->Content = "lazy download";
	downloadButton->Height = 20;
	downloadButton->Width = 120;
	downloadButton->BorderThickness = Thickness(2);
	downloadButton->BorderBrush = Brushes::Black;
	hGrid2->SetRow(downloadButton, 3);
	hGrid2->Children->Add(downloadButton);
	entiredownload->Content = "dwnload entire repo ";
	entiredownload->Height = 20;
	entiredownload->Width = 120;
	entiredownload->BorderThickness = Thickness(2);
	entiredownload->BorderBrush = Brushes::Black;
	hGrid2->SetRow(entiredownload, 4);
	hGrid2->Children->Add(entiredownload);
}

//<-----------------used to browse for the folder----------------------------->
void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hListBox->Items->Add(files[i]);
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i)
			hListBox->Items->Add(L"<> " + dirs[i]);
	}
}

//<-----------------used for the setting up connection view-------------------->
void WPFCppCliDemo::setUpConnectionView()
{
	Console::Write("\n  setting up Connection view");
}

//<---------used for downloading css files------------------------------->
void CppCliWindows::WPFCppCliDemo::downloadcssfiles()
{
	std::cout << "\nstarting of download css files\n";
	clientFactory *fact = new clientFactory();
	client = fact->getclient();
	client->sendmsgtoserver("cssfiles");
}

//<-------when ui loaded this function call takes place---------------------->
void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}

//<--------when ui is closing this function call take splace------------------>
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}

//<------------------used for getting only selected files-------------------------->
void  WPFCppCliDemo::getonlyselectedfiles(Object^ sender, RoutedEventArgs^ args)
{
	int index = 0;
	int count = repolistBox->SelectedItems->Count;
	hStatus->Text = "Deleting Selected items";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in repolistBox->SelectedItems)
		{
			items[index++] = item;
		}
	}

	/**/
	std::string deletefiles = "delete";
	deletefiles = deletefiles + "\n";
	if (count > 0) {
		for each (String^ item in items)
		{
			std::string temp = toStdString(item);
			deletefiles = deletefiles + temp;
			deletefiles = deletefiles + "\n";
		}
	}

	std::cout << "\nthe contentents going to be deleted is\n" << deletefiles;

	clientFactory *fact = new clientFactory();
	client = fact->getclient();
	client->sendmsgtoserver(deletefiles);
	hStatus->Text = "deleting files";

}

//<-------USED FOR GETTING ALL THE REPOSITORY CONTENTS----------------->
void WPFCppCliDemo::getallrepo(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "entered start of download all files function";
	clientFactory *fact = new clientFactory();
	client = fact->getclient();
	client->sendmsgtoserver("sendallfiles");
	hStatus->Text = "files downloaded";

}

//<-------------used for opening files through browser--------------------->
void CppCliWindows::WPFCppCliDemo::openfilesthroughbrowser(Object ^ sender, RoutedEventArgs ^ args)
{
	std::cout << "\n opening the selected files through default browser\n";
	int index = 0;
	int count = reposlistBox->SelectedItems->Count;
	hStatus->Text = "Deleting Selected items";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
	for each (String^ item in reposlistBox->SelectedItems)
	{
	items[index++] = item;
	}
	}

	if (count > 0) {
	for each (String^ item in items)
	{
	std::string temp = toStdString(item);
	std::string command("start \"\" \"" + temp + "\"");
	std::system(command.c_str());
	}

	}
	
	hStatus->Text = "opening files through browser";
}

//<----------------main function used for launching the application----------------------------->
[STAThread]
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting WPFCppCliDemo");
	Application^ app = gcnew Application();
	app->Run(gcnew WPFCppCliDemo());

	Console::WriteLine(L"\n\n");
}