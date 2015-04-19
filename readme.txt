********************************************************************************************************************************************************************
********************************************************************************************************************************************************************
                                                               **************Embedded systems IITB ***********************


									Year 		: 2015

									Course 		: CS 101

									Group No.	: 489



**************************************************************************  Roll No. & Names of Students ************************************************************

						
								140070028	: Shubham Yadav (Group Leader)

								14D070058	: Rashish Rajendra Shingi

								140040083	: Shubham Agrawal

								140040013	: Raghav Daga



***********************************************************  Title of the Project : Artistic Bot  *********************************************************************


                                                                        Hardware Requirements :- 
                                                                        ----------------------

1. FireBird V : Requires 1 bot

2. Gripper : To hold the pen in correct position 

3. Marker : To draw the image

4. Xbee : To maintain communication between the bot and Code::Blocks.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------

									Software Requirements :-
								       ------------------------- 


1. Atmel Studio : To write the program for the bot in Embedded C language

2. Code::Blocks : with Simplecpp

3. AVR studio : To program instruction onto a given bot (acts as a bootloader).

4. OpenCV : For processing images sent by camera.



**********************************************************  Instructions to setup the project  **********************************************************************


1. Firstly, burn the embedded C code given in folder "code for bot" into the Firebird V bot. This can be done by first building the code, then from the AVR Bootloader,
    Burn ( click on program)  the hex file into the bot.

2. Open folder "interface" and open "input_interface" in codeblocks , compile and run.

3. Draw any shape that you want the bot to reproduce on the paper (on larger scale).

3. Connect the x-bee transmitter to the laptop and the receiver to the bot.

4. Next open the software X-CTU and select PC Settings and press Test/Query. The COM Port will appear in the 'Select COM Port' dialog box.

5. Configure the Xbee ( see the video)

5. Goto "interface" --> open "main_code" in code::blocks--> write that COM Port 
   HANDLE hPort = CreateFile("<Your COM Port>", GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   just above the writebyte() function.

6. Now compile and run the "main_code.cpp", and the bot will draw the diagram on the paper.

------------------------------------------------------------------------------------------------------------------------------------------------------------------

 YouTube Link for the demo video : 
https://www.youtube.com/watch?v=6tRrkdVqEEU&feature=youtu.be **** For the video
https://www.youtube.com/watch?v=NMdcrJXzoCI********************** For the screencast

********************************************************************************************************************************************************************
*********************************************************************** ARTIST BOT *********************************************************************************

