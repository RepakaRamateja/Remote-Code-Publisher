						Remote-Code-Publisher

</br>

							Purpose:

A Code Repository is a Program responsible for managing source code resources, e.g., files and documents. A fully developed Repository will support file persistance, managment of versions, and the acquisition and publication of source and document files. A Remote Repository adds the capability to access the Repository's functionality over a communication channel, e.g., interprocess communication, inter-network communication, and communication across the internet.

In this project we will focus on the publication functionality of a Remote Repository. We will develop a remote code publisher, local client, and communication channel that supports client access to the publisher from any internet enabled processor.

The communication channel will use sockets and support an HTTP like message structure. The channel will support:

HTTP style request/response transactions
One-way communication, allowing asynchronous messaging between any two endpoints that are capable of listening for connection requests and connecting to a remote listener.
Transmission of byte streams that are set up with one or more negotiation messages followed by transmission of a stream of bytes of specified stream size2.
The Remote Code Publisher will:

Support publishing web pages that are small wrappers around C++ source code files, just as we did in Project #3.
Accept source code text files, sent from a local client.
Support building dependency relationships between code files saved in specific repository folders, based on the functionality you provided in Project #2 and used in Project #3.
Support HTML file creation for all the files in a specified repository folder1, including linking information that displays dependency relationships, and supports and navigation based on dependency relationships.
Delete stored files, as requested by a local client.
Clients of the Remote Code Publisher will provide a Graphical User Interface (GUI) with means to:

Upload one or more source code text files to the Remote Publisher, specifying a category with which those files are associated1.
Display file categories, based on the directory structure supported by the Repository.
Display all the files in any category.
Display all of the files in any category that have no parents.
Display the web page for any file in that file list by clicking within a GUI control. This implies that the client will download the appropriate webpages, scripts, and style sheets and display, by starting a browser with a file cited on the command line2.
On starting, will download style sheet and JavaScript files from the Repository.
Note that your client does not need to supply the functionality to display web pages. It simply starts a browser to do that. Browsers will accept a file name, which probably includes a relative path to display a web page from the local directory.
You could also start IIS web server and provide an appropriate URL to the browser on startup. Either approach is acceptable. If you use IIS, you won't have to download files, but you are obligated to show that you can do that.


						   Requirements:

Your Remote Repository:

(1) Shall use Visual Studio 2015 and its C++ Windows console         projects, as provided in the ECS computer labs. You must also use Windows Presentation Foundation (WPF) to provide a required client Graphical User Interface (GUI).

(2) Shall use the C++ standard library's streams for all console I/O and new and delete for all heap-based memory management.


(3) Shall provide a Repository program that provides functionality to publish, as linked web pages, the contents of a set of C++ source code files.


(4) Shall, for the publishing process, satisfy the requirements of CodePublisher developed in Project #3.


(5) Shall provide a Client program that can upload files3, and view Repository contents, as described in the Purpose section, above.


(6) Shall provide a message-passing communication system, based on Sockets, used to access the Repository's functionality from another process or machine.


(7) The communication system shall provide support for passing HTTP style messages using either synchronous request/response or asynchronous one-way messaging.


(8) The communication system shall also support sending and receiving streams of bytes6. Streams will be established with an initial exchange of messages.


(9) Shall include an automated unit test suite that demonstrates you meet all the requirements of this project4 including the transmission of files.

(10)

(5 point bonus) Shall optionally use a lazy download strategy, that, when presented with a name of a source code web page, will download that file and all the files it links to. This allows you to demonstrate your project using local webpages instead of downloading the entire contents of the Code Publisher for demonstration.

(11)

(5 point bonus) Shall optionally have the publisher accept a path, on the commandline, to a virtual directory on the server. Then support browsing directly from the server by supplying a url to that path when you start a browser. This works only if you setup IIS on your machine and make the path a virtual directory. The TAs will do that on the grading machines.


						Execution Instructions:

Prerequisites:

 Microsoft Visual Studio 2015 0r 2017

 IIS installed and then create virtual directory in IIS which points to a local system directory

 Finally in the run.bat 

 change the name of directory under command start MsgServer.exe  (local system directory pointed to IIS)  coderepo  9090 


  Now follow the below steps


               open developer command prompt for vs2017 by administrator mode

               Navigate to the downloaded project location   

               Run compile.bat

               Run run.bat



 User Interface:


 ![alt text](https://github.com/RepakaRamateja/Remote-Code-Publisher/blob/master/images/1.PNG)

</br>

 ![alt text](https://github.com/RepakaRamateja/Remote-Code-Publisher/blob/master/images/2.PNG)
 
</br>

 ![alt text](https://github.com/RepakaRamateja/Remote-Code-Publisher/blob/master/images/3.PNG)

</br>

 ![alt text](https://github.com/RepakaRamateja/Remote-Code-Publisher/blob/master/images/4.PNG)

</br>
 
 ![alt text](https://github.com/RepakaRamateja/Remote-Code-Publisher/blob/master/images/5.PNG)

</br>

 ![alt text](https://github.com/RepakaRamateja/Remote-Code-Publisher/blob/master/images/6.PNG)


</br>

 ![alt text](https://github.com/RepakaRamateja/Remote-Code-Publisher/blob/master/images/7.PNG)



 Created Html files with dependencies

</br>

 ![alt text](https://github.com/RepakaRamateja/Remote-Code-Publisher/blob/master/images/code1.png)

</br>

 ![alt text](https://github.com/RepakaRamateja/Remote-Code-Publisher/blob/master/images/code2.png)


				   Demonstration of Requirements:



--------------------------------------------- NOTE -------------------------------------------------------------

</br>

 WHAT EVER CLIENT SENDS  SERVER SAVES IN---> ServerInputFiles FOLDER

 OUTPUT HTML FILES WILL BE GENERATED IN --->Serveroutputfiles  FOLDER

 WHAT EVER SERVER SENDS CLIENT SAVES IN -----> clienthtmlfiles FOLDER

 PUBLISHER  ASLO SAVES THE FILES IN GIVEN VIRTUAL DIRECTORY LOCATION

</br>

--------------------------------  DEMONSTRATION OF THE REQUIREMENTS--(REQ 9) -----------------------------------

</br>

ON START UP CSS FILES AND JAVA SCRIPT FILES

ARE SENT FROM SERVER TO CLIENT AND ALSO SAVED IN VIRTUAL DIRECTORY

IF YOU WANT TO OBSERVE PLEASE LOOK AT CLIENT AND SERVER CONSOLE

OTHERWISE CHECK THE TIME STAMP IN clienthtmlfiles FOLDER

GIVEN VIRTUAL DIRECTORY FOLDER

</br>

---------------------------------  DEMONSTRATION FOR REQUIREMENT 5 ---------------------------------------------
</br>

GO TO THE TAB----->FILE UPLOAD LIST

NOW CLICK ON BUTTON---->SELECT DIRECTORY

NOW YOU CAN SEE THE DIRECTORY CONTENTS

NOW SELECT ANY NUMBER OF FILES YOU WANT

CLICK -----> SHOW SELECTED FILES BUTTON TO UPLOAD ONLY SELECTED FILES

TO UPLOAD THE FILES NOW CLICK ON UPLOAD BUTTON

NOW FILES ARE BEING TRANSFERRED FROM CLIENT TO SERVER

PLEASE OBSERVE THE CONSOLES WHICH CLEARLY DEMONSTRATES THIS REQUIREMENT

</br>

----------------------------------- IT ALSO SATISFIES REQUIREMENT 8 ----------------------------------------------

</br>

INPUT FILES ARE SAVED IN THE LOCATION ../ServerInputFiles folder

PLEASE VERIFY TIME STAMP IN ORDER TO CONFORM

IF YOU SEE THE CONSOLE-- CLIENT IS SENDING MESSAGE BEFORE SENDING FILE

WHICH IS THE NAME OF THE FILE TO BE SEND

SEE THE SERVER CONSOLE WHICH RECIEVED THE MESSAGE FIRST THAN THE FILE

SERVER ALSO SENDS THE MESSAGE AND CLIENT RECIEVES IT

</br>

---------------------------------- DEMONTRATION FOR REQUIREMENT 3 AND 4 ----------------------------------------

</br>

CODE FILES ARE PUBLISHED IN THE LOCATION ../Serveroutputfiles

AND GENERATED HTML FILES WHICH SATISFIES PROJECT 3 REQUIREMENTS

------GENERATED HTML CONTAINS ALL DEPENDDENCY FILE LINKS

------ USING EXTERNAL CSS AND JAVA SCRIPT FILES ETC

</br>

---------------------------------  DEMONSTRATION FOR REQUIREMENT 6 ----------------------------------------------

 GO TO THE TAB--->DOWNLOAD

 CLICK ON BUTTON ---->get contents

 NOW WHATEVER CONTENTS PRESENT IN SERVER REPO ARE DISPLAYED IN LISTBOX

</br>

 CREATED A TAB FOR VIEWING ALL DOWNLOADED FILES

GO TO THE TAB--->CLIENT LOCAL REPO

CLICK ON BUTTON --->GET LOCAL REPO CONTENTS

 NOW SELECT SOME FILES

 NOW CLICK---> OPEN FILES BUTTON

 NOW FILES WILL BE OPENED THROUGH BROWSER

</br>


----------------------------------- DEMONSTRATION FOR REQUIREMENT 7 ---------------------------------------------

</br>

IN THE DOWNLOAD FILE TAB WHEN YOU CLICK ----------->GET CONTENTS BUTTON

CLIENT SENDS MESSAGE TO------>SERVER AND SERVER SENDS MESSGAE TO CLIENT

 CLIENT POPULATES IT IN THE UI

THIS IS A TWO WAY COMMUNICATION

SIMILARLY  IT HAPPENS WHEN YOU CLICK----> DOWNLOAD ENTIRE REPO

SIMILARLY IN THE TAB ------->IIS

WHEN YOU CLICK ON BUTTON------>GET VDIR INFO

SIMILAR TYPE OF COMMUNICATION TAKES PLACE

MOSTLY WHEN YOU CLICK ANY BUTTON MOSTLY SYNCHRONOUS OR SYNCHRONOUS IS HAPPENING

LIKE GETTING IIS FOLDER DETAILS ETC

</br>


----------------------------------- DEMONSTRATION FOR REQUIREMENT 10 --------------------------------------------

</br>

NOTE:FILES WILL DOWNLOAD TO LOACTION ../clienthtmlfiles/

   IAM OPENING ALL THE FILES THROUGH BROWSER

   YOU CAN SEE THE TIME STAMP OF THE FILES TO VERFIY WHICH FILES ARE DOWNLOADED

   THEN YOU CAN SEE LAZY DOWNLOAD WORKS


IN THE DOWNLOAD TAB CLICK ON BUTTON --------->GET REPOSITIRY CONTENTS

SELECT ANY ITEMS AND CLICK ON ------->LAZY DOWNLOAD

NOW AFTER THAT FILES DOWNLOADED AND ARE OPENED THROUGH BROWSER

ALTERNATE WAY  IS  GO TO THE TAB--------> CLIENT REPO VIEW


AND  CLICK----------> GET CONTENTS

NOW SELECT ANY FILE AND CLICK ON BUTTON ------>OPEN FILES

NOW FILES WILL BE OPENED THROUGH BROWSER

 TO DOWNLOAD ALL CONTENTS THEN CLICK-------> DOWNLOAD ENTIRE REPO BUTTON

 FILES DOWNLOADED WILL BE OPENED THROUGH BROWSER

---------------------DELETING A FILE REQUIREMENT---------------------------

PLEASE FOLLOW BELOW STEPS

------->GO TO THE TAB DOWNLOAD

GET THE REPO CONTENTS BY CLICKING BUTTON -------->GET CONTENTS

 SELECT THE FILES YOU WANT TO DELETE

NOW CLICK ON------------> DELETE BUTTON

TO verify CLICK ON ----->GET CONTENTS

THAT IS FILES ARE DELETED

TO CONFIRM CHECK ../Serveroutputfiles DIRECTORY

THIS SHOWS THAT FILE IS DELETED

</br>

-------------------------------- DEMONSTRATION FOR IIS REQUIREMENT(REQUIREMENT 11) ------------------------

</br>

GIVING THE FOLLOWING COMMAND LINE ARGUMENTS FOR MSG SERVER PROJECT

VIRTUAL DIRECTORY PATH WHERE CODE FILES TO BE PUBLISHED

AND APPLICATION NAME(DUMMY NAME) THAT POINTS TO YOUR REAL DIRECTORY NAME

PORT NUMBER

NOW GO TO THE IIS TAB

CLICK ON----------->GET VDIR INFO

SELECT ANY FILES AND CLICK ON -------> OPEN FILES BUTTON

NOW FILES WILL BE OPENED THROUGH BROWSER

YOU CAN ALSO FILTER THE FILES USING BUTTON------->SHOW SELECTED FILES

 
















               






