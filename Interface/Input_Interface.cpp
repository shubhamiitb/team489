/********************************************************************************************************************************************************************
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
class turtle_array
{
    public:
        char moves[500];        //array to store the character variables
        int count=0;            //Starting the position of the cursor
        int distance=0;         //To store the total distance moved from the start
        int distance_moved=10;  //To store the distance to be moved after pressing the distance_bar
        int angle=0;            //To store the angle of the bot with respect to the horizontal
        int state=1;            //To store penUp or penDown
        /*
        Function : void arr_fill(int xy,int &flag)
        To detect where the user has clicked and thereby fill the array with the correct character.
        */
        void arr_fill(int xy,int &flag)
        {
            int x,y;
            x=xy/65536;
            y=xy%65536;
            if((x>=300)&&(x<=400) && (y>=625)&&(y<=675))//Checking for stop button
                flag=0;
            else if((x>=300)&&(x<=400) && (y>=50)&&(y<=125))//Checking for forward button
            {
                moves[count]='f';
                count++;
            }
            else if((x>=150)&&(x<=250) && (y>=525)&&(y<=575))//Checking for penUp button
            {
                moves[count]='u';
                count++;
            }
            else if((x>=450)&&(x<=550) && (y>=525)&&(y<=575))//Checking for penDown button
            {
                moves[count]='d';
                count++;
            }
            else if((x>=150)&&(x<=225) && (y>=125)&&(y<=225))//Checking for 15 degree left button
            {
                moves[count]=1;//moving left
                count++;
            }
            else if((x>=475)&&(x<=550) && (y>=125)&&(y<=225))//Checking for 15 degree right button
            {
                moves[count]=3;//moving right
                count++;
            }
            else if((x>=525)&&(x<=625) && (y>=275)&&(y<=325))//Checking for Buzzer On button
            {
                moves[count]=25;//Buzzer on
                count++;
            }
            else if((x>=525)&&(x<=625) && (y>=375)&&(y<=425))//Checking for Buzzer Off button
            {
                moves[count]=26;//Buzzer off
                count++;
            }
            else if((x>=80)&&(x<=120) && (y>=250)&&(y<=283))
            {
                moves[count]=36;//Distance is 60
                count++;
            }
            else if((x>=80)&&(x<=120) && (y>=283)&&(y<=317))
            {
                moves[count]=35;//Distance is 50
                count++;
            }
            else if((x>=80)&&(x<=120) && (y>=317)&&(y<=350))
            {
                moves[count]=34;//Distance is 40
                count++;
            }
            else if((x>=80)&&(x<=120) && (y>=350)&&(y<=383))
            {
                moves[count]=33;//Distance is 30
                count++;
            }
            else if((x>=80)&&(x<=120) && (y>=383)&&(y<=417))
            {
                moves[count]=42;//Distance is 20
                count++;
            }
            else if((x>=80)&&(x<=120) && (y>=417)&&(y<=450))
            {
                moves[count]=31;//Distance is 10
                count++;
            }
            else if((x>=250)&&(x<=450) && (y>=(350-sqrt(10000-((x-350)*(x-350)))))&&(y<=(350+sqrt(10000-((x-350)*(x-350))))))
                                //Checking inside the circle
            {
                if((x>=350)&&(x<=450) && (y<=350)&&(y>=350-(x-350)/1.732))
                {
                    moves[count]=2;//moving left
                    count++;
                }
                else if((x>=350)&&(x<=450) && (y<=350-(x-350)/1.732)&&(y>=350-(x-350)*1.732))
                {
                    moves[count]=4;//moving left
                    count++;
                }
                else if((x>=350)&&(x<=450) && (y<=350-(x-350)*1.732))
                {
                    moves[count]=6;//moving left
                    count++;
                }
                else if((x<=350)&&(x>=250) && (y<=350-abs(x-350)*1.732))
                {
                    moves[count]=8;//moving left
                    count++;
                }
                else if((x<=350)&&(x>=250) && (y<=350-abs(x-350)/1.732)&&(y>=350-abs(x-350)*1.732))
                {
                    moves[count]=50;//moving left
                    count++;
                }
                else if((x<=350)&&(x>=250) && (y<=350)&&(y>=350-abs(x-350)/1.732))
                {
                    moves[count]=52;//moving left
                    count++;
                }
                else if((x<=350)&&(x>=250) && (y>=350)&&(y<=350+abs(x-350)/1.732))
                {
                    moves[count]=14;//moving right
                    count++;
                }
                else if((x<=350)&&(x>=250) && (y>=350+abs(x-350)/1.732)&&(y<=350+abs(x-350)*1.732))
                {
                    moves[count]=16;//moving right
                    count++;
                }
                else if((x<=350)&&(x>=250) && (y>=350+abs(x-350)*1.732))
                {
                    moves[count]=18;//moving right
                    count++;
                }
                else if((x>=350)&&(x<=450) && (y>=350+(x-350)*1.732))
                {
                    moves[count]=20;//moving right
                    count++;
                }
                else if((x>=350)&&(x<=450) && (y>=350+(x-350)/1.732)&&(y<=350+(x-350)*1.732))
                {
                    moves[count]=22;//moving right
                    count++;
                }
                else if((x>=350)&&(x<=450) && (y>=350)&&(y<=350+(x-350)/1.732))
                {
                    moves[count]=24;//moving right
                    count++;
                }
            }
        }
        /*
        Function : void menu()
        To provide the menu to the user on the canvas and to move the turtle around as per the instructions of the user.
        */
        void menu()
        {
            /*
            The circle angle_turn creates a beautiful way of providing the angle for turning as per the requirement
            The distance bar provides us with a choice of choosing our own unit of distance.
            */
            int xy,flag=1,i=0;
            unsigned char ch;
            string state_1="Pen Down";
            initCanvas("Turtle Menu...",1355,700);
            Line f1(300,125,400,125);
            Line f2(400,125,400,75);
            Line f3(400,75,350,50);
            Line f4(350,50,300,75);
            Line f5(300,75,300,125);//Above lines create the pentagon circumscribing the text
            Text t1(350,100,"Forward");
            Line L1(175,225,225,225);
            Line L2(225,225,225,125);
            Line L3(225,125,175,125);
            Line L4(175,125,150,175);
            Line L5(150,175,175,225);//Above lines create the pentagon circumscribing the text
            Text t2(200,175,"15 Degrees");
            Line R1(475,225,525,225);
            Line R2(525,225,550,175);
            Line R3(550,175,525,125);
            Line R4(525,125,475,125);
            Line R5(475,125,475,225);//Above lines create the pentagon circumscribing the text
            Text t3(500,175,"15 Degrees");
            Rectangle penup(200,550,100,50);
            Text t4(200,550,"Pen Up");
            Rectangle pendown(500,550,100,50);
            Text t5(500,550,"Pen Down");
            Rectangle buzzer_on(575,300,100,50);
            Text t11(575,300,"Buzzer On");
            Rectangle buzzer_off(575,400,100,50);
            Text t12(575,400,"Buzzer Off");
            Rectangle stop1(350,650,100,50);
            Rectangle stop2(350,650,80,35);
            Text t6(350,650,"STOP");
            t6.setColor(COLOR("red"));
            Rectangle distance_bar(100,350,40,200);
            Text t17(100,266.5,"60");
            Line d1(80,283,120,283);
            Text t18(100,300,"50");
            Line d2(80,317,120,317);
            Text t19(100,333.5,"40");
            Line d3(80,350,120,350);
            Text t20(100,366.5,"30");
            Line d4(80,383,120,383);
            Text t21(100,400,"20");
            Line d5(80,417,120,417);
            Text t22(100,433.5,"10");
            Text t7(100,470,"Distance");
            Circle angle_turn(350,350,100);
            Line l1(250,350,450,350);
            Line l2(263.4,400,436.6,300);
            Line l3(300,436.6,400,263.4);
            Line l4(350,450,350,250);
            Line l5(400,436.6,300,263.4);
            Line l6(436.6,400,263.4,300);
            Rectangle divide1(775,350,10,700);
            Text t8(350,470,"Angle");
            Text t9(1100,25,"Distance covered : ");
            Text t10(1100,45,"Angle with respect to horizontal : ");
            Text t24(1100,65,"Current Distance Unit : ");
            Text t25(1100,85,"Current State of Pen : ");
            t1.setColor(COLOR("blue"));//Below lines set the entities to the required colors.
            t2.setColor(COLOR("blue"));
            t3.setColor(COLOR("blue"));
            t4.setColor(COLOR("blue"));
            t5.setColor(COLOR("blue"));
            t7.setColor(COLOR("blue"));
            t8.setColor(COLOR("blue"));
            t9.setColor(COLOR("blue"));
            t10.setColor(COLOR("blue"));
            t11.setColor(COLOR("blue"));
            t12.setColor(COLOR("blue"));
            t17.setColor(COLOR("blue"));
            t18.setColor(COLOR("blue"));
            t19.setColor(COLOR("blue"));
            t20.setColor(COLOR("blue"));
            t21.setColor(COLOR("blue"));
            t22.setColor(COLOR("red"));
            t24.setColor(COLOR("blue"));
            t25.setColor(COLOR("blue"));
            angle_turn.setColor(COLOR(255,128,128));
            l1.setColor(COLOR(255,128,128));
            l2.setColor(COLOR(255,128,128));
            l3.setColor(COLOR(255,128,128));
            l4.setColor(COLOR(255,128,128));
            l5.setColor(COLOR(255,128,128));
            l6.setColor(COLOR(255,128,128));
            stop1.setFill(true);
            stop2.setFill(true);
            stop2.setColor(COLOR("white"));
            stop1.setColor(COLOR("red"));
            divide1.setFill(true);
            divide1.setColor(COLOR(255,128,128));
            Turtle t;
            t.penUp();
            t.forward(375);
            t.penDown();
            while(flag!=0)
            {
                Text t13(1100,25,"Distance covered : ");
                t13.setColor(COLOR("blue"));
                Text t14(1250,25,distance);
                t14.setColor(COLOR("blue"));
                Text t15(1100,45,"Angle with respect to horizontal : ");
                t15.setColor(COLOR("blue"));
                Text t16(1250,45,angle);
                t16.setColor(COLOR("blue"));
                Text t23(1250,85,state_1);
                t23.setColor(COLOR("blue"));
                Text t26(1250,65,distance_moved);
                t26.setColor(COLOR("blue"));
                xy=getClick();
                arr_fill(xy,flag);
                while(i<count)
                {
                    ch=moves[i];
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
                        case 31://Sets unit of distance as 10
                        {
                            distance_moved=10;
                            t17.setColor(COLOR("blue"));
                            t18.setColor(COLOR("blue"));
                            t19.setColor(COLOR("blue"));
                            t20.setColor(COLOR("blue"));
                            t21.setColor(COLOR("blue"));
                            t22.setColor(COLOR("red"));
                        }
                        break;
                        case 42://Sets unit of distance as 20
                        {
                            distance_moved=20;
                            t17.setColor(COLOR("blue"));
                            t18.setColor(COLOR("blue"));
                            t19.setColor(COLOR("blue"));
                            t20.setColor(COLOR("blue"));
                            t21.setColor(COLOR("red"));
                            t22.setColor(COLOR("blue"));
                        }
                        break;
                        case 33://Sets unit of distance as 30
                        {
                            distance_moved=30;
                            t17.setColor(COLOR("blue"));
                            t18.setColor(COLOR("blue"));
                            t19.setColor(COLOR("blue"));
                            t20.setColor(COLOR("red"));
                            t21.setColor(COLOR("blue"));
                            t22.setColor(COLOR("blue"));
                        }
                        break;
                        case 34://Sets unit of distance as 40
                        {
                            distance_moved=40;
                            t17.setColor(COLOR("blue"));
                            t18.setColor(COLOR("blue"));
                            t19.setColor(COLOR("red"));
                            t20.setColor(COLOR("blue"));
                            t21.setColor(COLOR("blue"));
                            t22.setColor(COLOR("blue"));
                        }
                        break;
                        case 35://Sets unit of distance as 50
                        {
                            distance_moved=50;
                            t17.setColor(COLOR("blue"));
                            t18.setColor(COLOR("red"));
                            t19.setColor(COLOR("blue"));
                            t20.setColor(COLOR("blue"));
                            t21.setColor(COLOR("blue"));
                            t22.setColor(COLOR("blue"));
                        }
                        break;
                        case 36://Sets unit of distance as 60
                        {
                            distance_moved=60;
                            t17.setColor(COLOR("red"));
                            t18.setColor(COLOR("blue"));
                            t19.setColor(COLOR("blue"));
                            t20.setColor(COLOR("blue"));
                            t21.setColor(COLOR("blue"));
                            t22.setColor(COLOR("blue"));
                        }
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
                    i++;
                }
            }
        }
        /*
        Function : void write_file()
        To write all the moves into a file "moves.txt" to store it into permanent memory.
        */
        void write_file()
        {
            int i;
            ofstream arr;
            arr.open("moves.txt",ios::out);//Opens the file in ios::out mode.
            for(i=0;i<count;i++)
                arr<<moves[i];
            arr.close();
        }
};
int main()
{
    turtle_array t1;
    t1.menu();
    t1.write_file();
}
