/*******************************************************************************************************************************************************************
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



***********************************************************  Title of the Project : Artistic Bot  *********************************************************************/
#include<simplecpp>
#include<fstream>
#include <windows.h>
#include <iostream>


using namespace std;

HANDLE hPort = CreateFile("COM3", GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
DCB dcb;
/*
Function : bool writebyte (unsigned char* data)
To write data on the xbee port
*/
bool writebyte (unsigned char* data)
{
    DWORD byteswritten;
    if (!GetCommState(hPort, &dcb))
    {
        cout<<"\n Serial port can't be opened";
        return false;
    }

    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    if (!SetCommState(hPort, &dcb))
    return false;

    bool retVal = WriteFile(hPort, data, 1, &byteswritten, NULL);
    return retVal;
}
/*
Function : int readByte ()
To read data on the xbee port
*/
int readByte()
{
    int Val;
    BYTE byte;
    DWORD dwBytesTransferred;
    DWORD dwCommModemStatus;
    if(!GetCommState(hPort, &dcb))
        return 0;

    SetCommMask(hPort, EV_RXCHAR|EV_ERR);
    WaitCommEvent(hPort, &dwCommModemStatus, 0);
    if (dwCommModemStatus& EV_RXCHAR)
        ReadFile(hPort, &byte, 1, &dwBytesTransferred, 0);
    Val = byte;
    return Val;
}


int main()
{
    unsigned char ch;                       //To store individual characters from the file.
    int distance_moved=10;                  //To store the unit of distance
    int distance=0;                         //To store the total distance moved//To store the total distance moved
    int angle=0;                            //To store the angle of the bot with respect to the horizontal
    int state=1;                            //To store penUp or penDown in integer format
    string state_1="Pen Down";              //To convert it to string format to print on the simplecpp interface
    ifstream fin("moves.txt",ios::in);
    fin.seekg(0);                           //To move the cursor to the start of the file
    initCanvas("HardwareTurtle4",700,700);
    Turtle t;
    while(fin)
    {
        fin>>ch;                            //Reads character into the character variable ch
        if(fin.eof())
             break;                         //Testing the end of file
        cout<<ch;                           //Printing on the terminal to check with the software turtle.
        if (writebyte(&ch)) cout<<"Data Sent";
        cout<<"\n Data Received::"<<readByte()<<endl;
        switch(ch)
        {
            case 'f'://forward
            {
                t.forward(distance_moved);
                distance+=distance_moved;
            }
            break;
            case 1://15 left
            {
                t.left(15*1);
                angle+=15;
            }
            break;
            case 2://30 left
            {
                t.left(15*2);
                angle+=30;
            }
            break;
            case 3://15 right
            {
                t.right(15*1);
                angle-=15;
            }
            break;
            case 4://60 left
            {
                t.left(15*4);
                angle+=60;
            }
            break;
            case 6://90 left
            {
                t.left(15*6);
                angle+=90;
            }
            break;
            case 8://120 left
            {
                t.left(15*8);
                angle+=120;
            }
            break;
            case 50://150 left
            {
                t.left(15*10);
                angle+=150;
            }
            break;
            case 52://180 left
            {
                t.left(15*12);
                angle+=180;
            }
            break;
            case 14://180 right
            {
                t.right(15*12);
                angle-=180;
            }
            break;
            case 16://150 right
            {
                t.right(15*10);
                angle-=150;
            }
            break;
            case 18://120 right
            {
                t.right(15*8);
                angle-=120;
            }
            break;
            case 20://90 right
            {
                t.right(15*6);
                angle-=90;
            }
            break;
            case 22://60 right
            {
                t.right(15*4);
                angle-=60;
            }
            break;
            case 24://30 right
            {
                t.right(15*2);
                angle-=30;
            }
            break;
            case 25://Buzzer on
                cout<<"\a";
            break;
            case 26://Buzzer off
                cout<<"\a";
            break;
            case 31:
                distance_moved=10;//Setting the unit of distance as 10
            break;
            case 42:
                distance_moved=20;//Setting the unit of distance as 20
            break;
            case 33:
                distance_moved=30;//Setting the unit of distance as 30
            break;
            case 34:
                distance_moved=40;//Setting the unit of distance as 40
            break;
            case 35:
                distance_moved=50;//Setting the unit of distance as 50
            break;
            case 36:
                distance_moved=60;//Setting the unit of distance as 60
            break;
            case 'u'://Pen Up
            {
                t.penUp();
                state=0;//penUp
            }
            break;
            case 'd'://Pen Down
            {
                t.penDown();
                state=1;//penDown
            }
            break;
        }
        if(state==0)
            state_1="Pen Up";
        if(state==1)
            state_1="Pen Down";
    }
    Text t9(1100,25,"Distance covered : ");
    Text t10(1100,45,"Angle with respect to horizontal : ");
    Text t14(1250,25,distance);
    Text t16(1250,45,angle);
    ch='s';
    if (writebyte(&ch)) cout<<"Data Sent";
        cout<<"\n Data Received::"<<readByte()<<endl;

    CloseHandle(hPort);
    fin.close();                                         //Closes the file "moves.txt"
    getClick();
    return(0);
}
