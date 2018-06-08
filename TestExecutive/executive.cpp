#include<iostream>
#include<string>
#include<vector>


//<----starting note------------------->
void startingnote()
{
	std::cout << "\n------------------NOTE-----------------------\n";
	std::cout << "\n WHAT EVER CLIENT SENDS  SERVER SAVES IN---> ServerInputFiles FOLDER\n";
	std::cout << "\n OUTPUT HTML FILES WILL BE GENERATED IN --->Serveroutputfiles  FOLDER\n";
	std::cout << "\n WHAT EVER SERVER SENDS CLIENT SAVES IN -----> clienthtmlfiles FOLDER\n";
	std::cout << "\n PUBLISHER  ASLO SAVES THE FILES IN GIVEN VIRTUAL DIRECTORY LOCATION\n";
	std::cout << "\n";

}

//<-------------requirement9output function------------->
void requirement9()
{
	std::cout << "\n------------DEMONSTRATION OF THE REQUIREMENTS--(REQ 9)-------------\n";
	std::cout << "\n";
	std::cout << "ON START UP CSS FILES AND JAVA SCRIPT FILES \n";
	std::cout << "\n";
	std::cout << "ARE SENT FROM SERVER TO CLIENT AND ALSO SAVED IN VIRTUAL DIRECTORY\n";
	std::cout << "\n";
	std::cout << "IF YOU WANT TO OBSERVE PLEASE LOOK AT CLIENT AND SERVER CONSOLE\n";
	std::cout << "\n";
	std::cout << "OTHERWISE CHECK THE TIME STAMP IN clienthtmlfiles FOLDER \n";
	std::cout << "\nGIVEN VIRTUAL DIRECTORY FOLDER\n";
	std::cout << "\n";
	std::cout << "\n----------------- DEMONSTRATION FOR REQUIREMENT 5-----------------------\n";
	std::cout << "\nGO TO THE TAB----->FILE UPLOAD LIST\n";
	std::cout << "\n";
	std::cout << "NOW CLICK ON BUTTON---->SELECT DIRECTORY\n";
	std::cout << "\n";
	std::cout << "NOW YOU CAN SEE THE DIRECTORY CONTENTS\n";
	std::cout << "\nNOW SELECT ANY NUMBER OF FILES YOU WANT\n";
	std::cout << "\n";
	std::cout << "CLICK -----> SHOW SELECTED FILES BUTTON TO UPLOAD ONLY SELECTED FILES\n";
	std::cout << "\n";
	std::cout << "TO UPLOAD THE FILES NOW CLICK ON UPLOAD BUTTON\n";
	std::cout << "\n";
}
//<-------------requirement8 output function------------->
void requirement8()
{
	std::cout << "NOW FILES ARE BEING TRANSFERRED FROM CLIENT TO SERVER\n";
	std::cout << "\n";
	std::cout << "PLEASE OBSERVE THE CONSOLES WHICH CLEARLY DEMONSTRATES THIS REQUIREMENT\n";
	std::cout << "\n----------------- IT ALSO SATISFIES REQUIREMENT 8------------------------\n";
	std::cout << "\nINPUT FILES ARE SAVED IN THE LOCATION ../ServerInputFiles folder\n";
	std::cout << "\nPLEASE VERIFY TIME STAMP IN ORDER TO CONFORM\n";
	std::cout << "\n";
	std::cout << "IF YOU SEE THE CONSOLE-- CLIENT IS SENDING MESSAGE BEFORE SENDING FILE \n";
	std::cout << "\n";
	std::cout << "WHICH IS THE NAME OF THE FILE TO BE SEND\n";
	std::cout << "\n";
	std::cout << "SEE THE SERVER CONSOLE WHICH RECIEVED THE MESSAGE FIRST THAN THE FILE\n";
	std::cout << "\n";
	std::cout << "SERVER ALSO SENDS THE MESSAGE AND CLIENT RECIEVES IT \n";

}
//<-------------requirement3 and 4 output function------------->
void requirement3and4()
{
	std::cout << "\n---------------DEMONTRATION FOR REQUIREMENT 3 AND 4-------------------\n";

	std::cout << "\nCODE FILES ARE PUBLISHED IN THE LOCATION ../Serveroutputfiles\n";

	std::cout << "\nAND GENERATED HTML FILES WHICH SATISFIES PROJECT 3 REQUIREMENTS\n";
	std::cout << "\n------GENERATED HTML CONTAINS ALL DEPENDDENCY FILE LINKS\n";
	std::cout << "\n------ USING EXTERNAL CSS AND JAVA SCRIPT FILES ETC\n";
}
//<-------------requirement7 output function------------->
void requirement7()
{
	std::cout << "\n---------------- DEMONSTRATION FOR REQUIREMENT 7--------------------------\n";
	std::cout << "\nIN THE DOWNLOAD FILE TAB WHEN YOU CLICK ----------->GET CONTENTS BUTTON\n";
	std::cout << "\nCLIENT SENDS MESSAGE TO------>SERVER AND SERVER SENDS MESSGAE TO CLIENT\n";
	std::cout << "\n CLIENT POPULATES IT IN THE UI\n";
	std::cout << "\nTHIS IS A TWO WAY COMMUNICATION\n";
	std::cout << "\n";
	std::cout << "SIMILARLY  IT HAPPENS WHEN YOU CLICK----> DOWNLOAD ENTIRE REPO\n";
	std::cout << "\nSIMILARLY IN THE TAB ------->IIS\n";
	std::cout << "\nWHEN YOU CLICK ON BUTTON------>GET VDIR INFO\n";
	std::cout << "\nSIMILAR TYPE OF COMMUNICATION TAKES PLACE\n";
	std::cout << "\nMOSTLY WHEN YOU CLICK ANY BUTTON MOSTLY SYNCHRONOUS OR SYNCHRONOUS IS HAPPENING\n";
	std::cout << "\nLIKE GETTING IIS FOLDER DETAILS ETC\n";
	std::cout << "\n";
}
//<---requirement10 output function------------->
void requirement10()
{
	std::cout << "\n----------------DEMONSTRATION FOR REQUIREMENT 10-------------------->\n";
	std::cout << "\n";
	std::cout << "NOTE:FILES WILL DOWNLOAD TO LOACTION ../clienthtmlfiles/";
	std::cout << "\n";
	std::cout << "\n   IAM OPENING ALL THE FILES THROUGH BROWSER\n";
	std::cout << "\n   YOU CAN SEE THE TIME STAMP OF THE FILES TO VERFIY WHICH FILES ARE DOWNLOADED\n";
	std::cout << "\n   THEN YOU CAN SEE LAZY DOWNLOAD WORKS\n";
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "IN THE DOWNLOAD TAB CLICK ON BUTTON --------->GET REPOSITIRY CONTENTS\n";
	std::cout << "\n";
	std::cout << "SELECT ANY ITEMS AND CLICK ON ------->LAZY DOWNLOAD\n";
	std::cout << "\n";
	std::cout << "NOW AFTER THAT FILES DOWNLOADED AND ARE OPENED THROUGH BROWSER\n";
	std::cout << "\n";
	std::cout << "ALTERNATE WAY  IS  GO TO THE TAB--------> CLIENT REPO VIEW \n";
	std::cout << "\n";
	std::cout << "\nAND  CLICK----------> GET CONTENTS\n";
	std::cout << "\nNOW SELECT ANY FILE AND CLICK ON BUTTON ------>OPEN FILES \n";
	std::cout << "\n";
	std::cout << "NOW FILES WILL BE OPENED THROUGH BROWSER\n";
	std::cout << "\n";
}
//<---requirement6 output function------------->
void requirement6()
{
	std::cout << "\n<------------DEMONSTRATION FOR REQUIREMENT 6------------------------>\n";
	std::cout << "\n GO TO THE TAB--->DOWNLOAD\n";
	std::cout << "\n CLICK ON BUTTON ---->get contents\n";
	std::cout << "\n NOW WHATEVER CONTENTS PRESENT IN SERVER REPO ARE DISPLAYED IN LISTBOX\n";
	std::cout << "\n---------------------------------------------------------------------\n";
	std::cout << "\n CREATED A TAB FOR VIEWING ALL DOWNLOADED FILES\n";
	std::cout << "\nGO TO THE TAB--->CLIENT LOCAL REPO\n";
	std::cout << "\nCLICK ON BUTTON --->GET LOCAL REPO CONTENTS\n";
	std::cout << "\nNOW SELECT SOME FILES\n";
	std::cout << "\n NOW CLICK---> OPEN FILES BUTTON\n";
	std::cout << "\n NOW FILES WILL BE OPENED THROUGH BROWSER\n";
	std::cout << "\n";
}
//test stub for the test executive
//<----------used for demonstrating the project requirements-------------------------->
#ifdef testmain
int main()
{
	startingnote();
	requirement9();
	requirement8();
	requirement3and4();
	requirement6();
	requirement7();
	requirement10();
	std::cout << " TO DOWNLOAD ALL CONTENTS THEN CLICK-------> DOWNLOAD ENTIRE REPO BUTTON\n";
	std::cout << "\n";
	std::cout << " FILES DOWNLOADED WILL BE OPENED THROUGH BROWSER\n";
	std::cout << "\n";
	std::cout << "---------------------DELETING A FILE REQUIREMENT---------------------------\n";
	std::cout << "\nPLEASE FOLLOW BELOW STEPS\n";
	std::cout << "\n------->GO TO THE TAB DOWNLOAD\n";
	std::cout << "\nGET THE REPO CONTENTS BY CLICKING BUTTON -------->GET CONTENTS\n";
	std::cout << "\n SELECT THE FILES YOU WANT TO DELETE\n";
	std::cout << "\nNOW CLICK ON------------> DELETE BUTTON\n";
	std::cout << "\nTO verify CLICK ON ----->GET CONTENTS\n";
	std::cout << "\nTHAT IS FILES ARE DELETED\n";
	std::cout << "\nTO CONFIRM CHECK ../Serveroutputfiles DIRECTORY\n";
	std::cout << "\n";
	std::cout << "THIS SHOWS THAT FILE IS DELETED\n";
	std::cout << "\n";
	std::cout << "------------------DEMONSTRATION FOR IIS REQUIREMENT(REQUIREMENT 11)------------\n";
	std::cout << "\nGIVING THE FOLLOWING COMMAND LINE ARGUMENTS FOR MSG SERVER PROJECT\n";
	std::cout << "\nVIRTUAL DIRECTORY PATH WHERE CODE FILES TO BE PUBLISHED\n";
	std::cout << "\nAND APPLICATION NAME(DUMMY NAME) THAT POINTS TO YOUR REAL DIRECTORY NAME\n";
	std::cout << "\nPORT NUMBER\n";
	std::cout << "\n";
	std::cout << "NOW GO TO THE IIS TAB\n";
	std::cout << "\nCLICK ON----------->GET VDIR INFO  \n";
	std::cout << "\nSELECT ANY FILES AND CLICK ON -------> OPEN FILES BUTTON\n";
	std::cout << "\nNOW FILES WILL BE OPENED THROUGH BROWSER\n";
	std::cout << "\n YOU CAN ALSO FILTER THE FILES USING BUTTON------->SHOW SELECTED FILES\n";
	return 0;
}
#endif