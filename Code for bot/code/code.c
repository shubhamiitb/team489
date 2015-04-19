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

/*********************************************************************************
1. Make sure that in the configuration options following settings are
done for proper operation of the code

Microcontroller: atmega2560
Frequency: 14745600
Optimization: -O0  (For more information read section: Selecting proper optimization
options below figure 2.22 in the Software Manual)

2.	It is observed that external interrupts does not work with the optimization level -Os

3. Auxiliary power can supply current up to 1 Ampere while Battery can supply current up to
2 Ampere. When both motors of the robot changes direction suddenly without stopping,
it produces large current surge. When robot is powered by Auxiliary power which can supply
only 1 Ampere of current, sudden direction change in both the motors will cause current
surge which can reset the microcontroller because of sudden fall in voltage.
It is a good practice to stop the motors for at least 0.5seconds before changing
the direction. This will also increase the useable time of the fully charged battery.
the life of the motor.

*********************************************************************************/

//#define F_CPU 14745600        // uncomment  this if using atmel studio 6 
//otherwise for avr studoi 4 : define frequency in Project--> Configuratoin Options ---> "Frequency" Option 
// set it to 14745600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder
volatile unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
volatile unsigned int Degrees; //to accept angle in degrees for turning
unsigned char data; //to store received data from UDR1

void buzzer_pin_config (void)
{
    DDRC = DDRC | 0x08;		//Setting PORTC 3 as outpt
    PORTC = PORTC & 0xF7;		//Setting PORTC 3 logic low to turnoff buzzer
}

//Function to configure ports to enable robot's motion

//lcd interface
#define RS 0
#define RW 1
#define EN 2
#define lcd_port PORTC

#define sbit(reg,bit)	reg |= (1<<bit)			// Macro defined for Setting a bit of any register.
#define cbit(reg,bit)	reg &= ~(1<<bit)		// Macro defined for Clearing a bit of any register.

void init_ports();
void lcd_reset();
void lcd_init();
void lcd_wr_command(unsigned char);
void lcd_wr_char(char);
void lcd_line1();
void lcd_line2();
void lcd_string(char*);

unsigned int temp;
unsigned int unit;
unsigned int tens;
unsigned int hundred;
unsigned int thousand;
unsigned int million;

//Function to configure LCD port
void lcd_port_config (void)
{
    DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
    PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
}

//Function to Initialize PORTS
void port_init_lcd()
{
    lcd_port_config();
}

//Function to Reset LCD
void lcd_set_4bit()
{
    _delay_ms(1);

    cbit(lcd_port,RS);				//RS=0 --- Command Input
    cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
    lcd_port = 0x30;				//Sending 3
    sbit(lcd_port,EN);				//Set Enable Pin
    _delay_ms(5);					//Delay
    cbit(lcd_port,EN);				//Clear Enable Pin

    _delay_ms(1);

    cbit(lcd_port,RS);				//RS=0 --- Command Input
    cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
    lcd_port = 0x30;				//Sending 3
    sbit(lcd_port,EN);				//Set Enable Pin
    _delay_ms(5);					//Delay
    cbit(lcd_port,EN);				//Clear Enable Pin

    _delay_ms(1);

    cbit(lcd_port,RS);				//RS=0 --- Command Input
    cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
    lcd_port = 0x30;				//Sending 3
    sbit(lcd_port,EN);				//Set Enable Pin
    _delay_ms(5);					//Delay
    cbit(lcd_port,EN);				//Clear Enable Pin

    _delay_ms(1);

    cbit(lcd_port,RS);				//RS=0 --- Command Input
    cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
    lcd_port = 0x20;				//Sending 2 to initialise LCD 4-bit mode
    sbit(lcd_port,EN);				//Set Enable Pin
    _delay_ms(1);					//Delay
    cbit(lcd_port,EN);				//Clear Enable Pin


}

//Function to Initialize LCD
void lcd_init()
{
    _delay_ms(1);

    lcd_wr_command(0x28);			//LCD 4-bit mode and 2 lines.
    lcd_wr_command(0x01);
    lcd_wr_command(0x06);
    lcd_wr_command(0x0E);
    lcd_wr_command(0x80);

}


//Function to Write Command on LCD
void lcd_wr_command(unsigned char cmd)
{
    unsigned char temp;
    temp = cmd;
    temp = temp & 0xF0;
    lcd_port &= 0x0F;
    lcd_port |= temp;
    cbit(lcd_port,RS);
    cbit(lcd_port,RW);
    sbit(lcd_port,EN);
    _delay_ms(5);
    cbit(lcd_port,EN);

    cmd = cmd & 0x0F;
    cmd = cmd<<4;
    lcd_port &= 0x0F;
    lcd_port |= cmd;
    cbit(lcd_port,RS);
    cbit(lcd_port,RW);
    sbit(lcd_port,EN);
    _delay_ms(5);
    cbit(lcd_port,EN);
}

//Function to Write Data on LCD
void lcd_wr_char(char letter)
{
    char temp;
    temp = letter;
    temp = (temp & 0xF0);
    lcd_port &= 0x0F;
    lcd_port |= temp;
    sbit(lcd_port,RS);
    cbit(lcd_port,RW);
    sbit(lcd_port,EN);
    _delay_ms(5);
    cbit(lcd_port,EN);

    letter = letter & 0x0F;
    letter = letter<<4;
    lcd_port &= 0x0F;
    lcd_port |= letter;
    sbit(lcd_port,RS);
    cbit(lcd_port,RW);
    sbit(lcd_port,EN);
    _delay_ms(5);
    cbit(lcd_port,EN);
}


//Function to bring cursor at home position
void lcd_home()
{
    lcd_wr_command(0x80);
}


//Function to Print String on LCD
void lcd_string(char *str)
{
    while(*str != '\0')
    {
        lcd_wr_char(*str);
        str++;
    }
}

//Position the LCD cursor at "row", "column".

void lcd_cursor (char row, char column)
{
    switch (row)
    {
    case 1:
        lcd_wr_command (0x80 + column - 1);
        break;
    case 2:
        lcd_wr_command (0xc0 + column - 1);
        break;
    case 3:
        lcd_wr_command (0x94 + column - 1);
        break;
    case 4:
        lcd_wr_command (0xd4 + column - 1);
        break;
    default:
        break;
    }
}

//Function To Print Any input value upto the desired digit on LCD
void lcd_print (char row, char coloumn, unsigned int value, int digits)
{
    unsigned char flag=0;
    if(row==0||coloumn==0)
    {
        lcd_home();
    }
    else
    {
        lcd_cursor(row,coloumn);
    }
    if(digits==5 || flag==1)
    {
        million=value/10000+48;
        lcd_wr_char(million);
        flag=1;
    }
    if(digits==4 || flag==1)
    {
        temp = value/1000;
        thousand = temp%10 + 48;
        lcd_wr_char(thousand);
        flag=1;
    }
    if(digits==3 || flag==1)
    {
        temp = value/100;
        hundred = temp%10 + 48;
        lcd_wr_char(hundred);
        flag=1;
    }
    if(digits==2 || flag==1)
    {
        temp = value/10;
        tens = temp%10 + 48;
        lcd_wr_char(tens);
        flag=1;
    }
    if(digits==1 || flag==1)
    {
        unit = value%10 + 48;
        lcd_wr_char(unit);
    }
    if(digits>5)
    {
        lcd_wr_char('E');
    }

}

// TO CONFIGURE ALL THE MOTION PINS
void motion_pin_config (void)
{
    DDRA = DDRA | 0x0F;
    PORTA = PORTA & 0xF0;
    DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
    PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}

//Function to configure INT4 (PORTE 4) pin as input for the left position encoder
void left_encoder_pin_config (void)
{
    DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input
    PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin
}

//Function to configure INT5 (PORTE 5) pin as input for the right position encoder
void right_encoder_pin_config (void)
{
    DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 4 pin as input
    PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 4 pin
}

//Function to initialize ports
void port_initposition()
{
    motion_pin_config(); //robot motion pins config
    left_encoder_pin_config(); //left encoder pin config
    right_encoder_pin_config(); //right encoder pin config
    buzzer_pin_config();
}
//FUNCTION TO SWITCH ON BUZZER
void buzzer_on (void)
{
    unsigned char port_restore = 0;
    port_restore = PINC;
    port_restore = port_restore | 0x08;
    PORTC = port_restore;
}
//FUNTION TO SWITCH OFF BUZZER
void buzzer_off (void)
{
    unsigned char port_restore = 0;
    port_restore = PINC;
    port_restore = port_restore & 0xF7;
    PORTC = port_restore;
}

void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
{
    cli(); //Clears the global interrupt
    EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
    EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
    sei();   // Enables the global interrupt
}

void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
{
    cli(); //Clears the global interrupt
    EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
    EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
    sei();   // Enables the global interrupt
}

//ISR for right position encoder
ISR(INT5_vect)
{
    ShaftCountRight++;  //increment right shaft position count
}


//ISR for left position encoder
ISR(INT4_vect)
{
    ShaftCountLeft++;  //increment left shaft position count
}


//Function used for setting motor's direction

void init_ports()
{
    motion_pin_config();
}

// Timer 5 initialized in PWM mode for velocity control
// Prescale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz
void timer5_init()
{
    TCCR5B = 0x00;	//Stop
    TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
    TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
    OCR5AH = 0x00;	//Output compare register high value for Left Motor
    OCR5AL = 0xFF;	//Output compare register low value for Left Motor
    OCR5BH = 0x00;	//Output compare register high value for Right Motor
    OCR5BL = 0xFF;	//Output compare register low value for Right Motor
    OCR5CH = 0x00;	//Output compare register high value for Motor C1
    OCR5CL = 0xFF;	//Output compare register low value for Motor C1
    TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionality to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/

    TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

// Function for robot velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
    OCR5AL = (unsigned char)left_motor;
    OCR5BL = (unsigned char)right_motor;
}


void motion_set (unsigned char Direction)
{
    unsigned char PortARestore = 0;

    Direction &= 0x0F; 		// removing upper nibbel for the protection
    PortARestore = PORTA; 		// reading the PORTA original status
    PortARestore &= 0xF0; 		// making lower direction nibbel to 0
    PortARestore |= Direction; // adding lower nibbel for forward command and restoring the PORTA status
    PORTA = PortARestore; 		// executing the command
}

void forward (void) //both wheels forward
{
    motion_set(0x06);
}

void back (void) //both wheels backward
{
    motion_set(0x09);
}

void left (void) //Left wheel backward, Right wheel forward
{
    motion_set(0x05);
}

void right (void) //Left wheel forward, Right wheel backward
{
    motion_set(0x0A);
}

void soft_left (void) //Left wheel stationary, Right wheel forward
{
    motion_set(0x04);
}

void soft_right (void) //Left wheel forward, Right wheel is stationary
{
    motion_set(0x02);
}

void soft_left_2 (void) //Left wheel backward, right wheel stationary
{
    motion_set(0x01);
}

void soft_right_2 (void) //Left wheel stationary, Right wheel backward
{
    motion_set(0x08);
}

void stop (void)
{
    motion_set(0x00);
}


//Function used for turning robot by specified degrees
void angle_rotate(unsigned int Degrees)
{
    float ReqdShaftCount = 0;
    unsigned long int ReqdShaftCountInt = 0;

    ReqdShaftCount = (float) Degrees/ 4.090; // division by resolution to get shaft count
    ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
    ShaftCountRight = 0;
    ShaftCountLeft = 0;

    while (1)
    {
        if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))
            break;
    }
    stop(); //Stop robot
}

//Function used for moving robot forward by specified distance

void linear_distance_mm(unsigned int DistanceInMM)
{
    float ReqdShaftCount = 0;
    unsigned long int ReqdShaftCountInt = 0;

    ReqdShaftCount = DistanceInMM / 5.338; // division by resolution to get shaft count
    ReqdShaftCountInt = (unsigned long int) ReqdShaftCount;

    ShaftCountRight = 0;
    while(1)
    {
        if(ShaftCountRight > ReqdShaftCountInt)
        {
            break;
        }
    }
    stop(); //Stop robot
}

//TO MOVE BOT FORWARD BY PARTICULAR MM
void forward_mm(unsigned int DistanceInMM)
{
    forward();
    linear_distance_mm(DistanceInMM);
}

//TO MOVE BOT BACK BY PARTICULAR MM
void back_mm(unsigned int DistanceInMM)
{
    back();
    linear_distance_mm(DistanceInMM);
}

//TO TURN BOT LEFT BY PARTICULAR DEGREES WITH RESPECT TO CENTRE
void left_degrees(unsigned int Degrees)
{
    // 88 pulses for 360 degrees rotation 4.090 degrees per count
    left(); //Turn left
    angle_rotate(Degrees);
}


//TO TURN BOT RIGHT BY PARTICULAR DEGREES WITH RESPECT TO CENTRE
void right_degrees(unsigned int Degrees)
{
    // 88 pulses for 360 degrees rotation 4.090 degrees per count
    right(); //Turn right
    angle_rotate(Degrees);
}

//TO TURN BOT LEFT BY PARTICULAR DEGREES WITH RESPECT TO WHEEL 1
void soft_left_degrees(unsigned int Degrees)
{
    // 176 pulses for 360 degrees rotation 2.045 degrees per count
    soft_left(); //Turn soft left
    Degrees=Degrees*2;
    angle_rotate(Degrees);
}
//TO TURN BOT RIGHT BY PARTICULAR DEGREES WITH RESPECT TO WHEEL 2
void soft_right_degrees(unsigned int Degrees)
{
    // 176 pulses for 360 degrees rotation 2.045 degrees per count
    soft_right();  //Turn soft right
    Degrees=Degrees*2;
    angle_rotate(Degrees);
}
//TO TURN BOT LEFT BY PARTICULAR DEGREES WITH RESPECT TO WHEEL 2
void soft_left_2_degrees(unsigned int Degrees)
{
    // 176 pulses for 360 degrees rotation 2.045 degrees per count
    soft_left_2(); //Turn reverse soft left
    Degrees=Degrees*2;
    angle_rotate(Degrees);
}

//TO TURN BOT RIGHT BY PARTICULAR DEGREES WITH RESPECT TO WHEEL 1
void soft_right_2_degrees(unsigned int Degrees)
{
    // 176 pulses for 360 degrees rotation 2.045 degrees per count
    soft_right_2();  //Turn reverse soft right
    Degrees=Degrees*2;
    angle_rotate(Degrees);
}

void  turnbot_left(void) //Left wheel backward, right wheel stationary
{
    motion_set(0x06);
}


void turnbot_left_degrees(unsigned int Degrees)
{
    // 176 pulses for 360 degrees rotation 2.045 degrees per count

    velocity(255,95);
    turnbot_left();  //Turn reverse soft right
    Degrees=Degrees*3.16;
    angle_rotate(Degrees);
}

void  turnbot_right(void) //Left wheel backward, right wheel stationary
{
    motion_set(0x09);
}


void turnbot_right_degrees(unsigned int Degrees)
{
    // 176 pulses for 360 degrees rotation 2.045 degrees per count

    velocity(255,95);
    turnbot_right();  //Turn reverse soft right
    Degrees=Degrees*3.16;
    angle_rotate(Degrees);
}


//Configure PORTB 5 pin for servo motor 1 operation
void servo1_pin_config (void)
{
    DDRB  = DDRB | 0x20;  //making PORTB 5 pin output
    PORTB = PORTB | 0x20; //setting PORTB 5 pin to logic 1
}

//Initialize the ports
void port_initservo(void)
{
    servo1_pin_config(); //Configure PORTB 5 pin for servo motor 1 operation
}

//TIMER1 initialization in 10 bit fast PWM mode
//prescale:256
// WGM: 7) PWM 10bit fast, TOP=0x03FF
// actual value: 52.25Hz
void timer1_init(void)
{
    TCCR1B = 0x00; //stop
    TCNT1H = 0xFC; //Counter high value to which OCR1xH value is to be compared with
    TCNT1L = 0x01;	//Counter low value to which OCR1xH value is to be compared with
    OCR1AH = 0x03;	//Output compare Register high value for servo 1
    OCR1AL = 0xFF;	//Output Compare Register low Value For servo 1
    ICR1H  = 0x03;
    ICR1L  = 0xFF;
    TCCR1A = 0xAB; /*{COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0; COM1C1=1 COM1C0=0}
 					For Overriding normal port functionality to OCRnA outputs.
				  {WGM11=1, WGM10=1} Along With WGM12 in TCCR1B for Selecting FAST PWM Mode*/
    TCCR1C = 0x00;
    TCCR1B = 0x0C; //WGM12=1; CS12=1, CS11=0, CS10=0 (Prescaler=256)
}

void uart0_init(void)
{
    UCSR0B = 0x00; //disable while setting baud rate
    UCSR0A = 0x00;
    UCSR0C = 0x06;
    // UBRR0L = 0x47; //11059200 Hz
    UBRR0L = 0x5F; // 14745600 Hzset baud rate lo
    UBRR0H = 0x00; //set baud rate hi
    UCSR0B = 0x98;
}

//Function to initialize all the peripherals
void init_devices(void)
{
    cli(); //disable all interrupts
    port_initservo();
    timer5_init();
    init_ports();
    port_initposition();
    port_init_lcd();
    timer1_init();
    left_position_encoder_interrupt_init();
    right_position_encoder_interrupt_init();
    uart0_init(); //Initailize UART1 for serial communiaction
    sei(); //re-enable interrupts
}


//Function to rotate Servo 1 by a specified angle in the multiples of 1.86 degrees
void servo_1(unsigned char degrees)
{
    float PositionPanServo = 0;
    PositionPanServo = ((float)degrees / 1.86) + 35.0;
    OCR1AH = 0x00;
    OCR1AL = (unsigned char) PositionPanServo;
}

/*
FUNCTION::pen_Down()
input::void
output::void
Work::to move gripper down
*/
void pen_Down()
{
    servo_1(180);
}
/*
FUNCTION::pen_Up()
input::void
output::void
Work::to move gripper up
*/
void pen_Up()
{
    servo_1(108);//proper angle hamlog bad m dal lenge daga
}


//Function To Initialize UART0
// desired baud rate:9600
// actual baud rate:9600 (error 0.0%)
// char size: 8 bit
// parity: Disabled

//simdata to store the ascii values given from simple cpp
unsigned char simdata[100];
int x=0;
/*
FUNCTION::signal()
input::unsigned char
output::void
Work::to store values from xbee
*/
SIGNAL(SIG_USART0_RECV) 		// ISR for receive complete interrupt
{
    data = UDR0; 				//making copy of data from UDR0 in 'data' variable

    UDR0 = data; 				//echo data back to PC

    simdata[x]=data;
    x++;
    /*if(data == 0x38) //ASCII value of 8
    {
        PORTA=0x06;  //forward
    }

    if(data == 0x32) //ASCII value of 2
    {
        PORTA=0x09; //back
    }

    if(data == 0x34) //ASCII value of 4
    {
        PORTA=0x05;  //left
    }

    if(data == 0x36) //ASCII value of 6
    {
        PORTA=0x0A; //right
    }

    if(data == 0x35) //ASCII value of 5
    {
        PORTA=0x00; //stop
    }*/

    if(data == 0x37) //ASCII value of 7
    {
        buzzer_on();
    }

    if(data == 0x39) //ASCII value of 9
    {
        buzzer_off();
    }
    
}
/*
signal interupt ends
*/

//to store forward scale
int distance=10;
// to check whether pen is up or down
int checkpen = 1;

/*
FUNCTION::bot_turn()
input::int
output::void
Work::to turn the bot by specified angle
*/
void bot_turn(int ang)
{
	//left turn with pen down
    if(ang>0&&ang<=180&&checkpen==1)
    {
        pen_Up();
        _delay_ms(1500);
        soft_left_degrees(ang);
        _delay_ms(2000);
        PORTA=0x00; //stop
        pen_Down();
        _delay_ms(1500);
    }
	
	//left turn with pen up
    else if(ang>0&&ang<=180&&checkpen==0)
    {
        soft_left_degrees(ang);
        _delay_ms(2000);
    }
	
	//right turn with pen down
    else if(ang<0&&checkpen==1)
    {
        pen_Up();
        _delay_ms(1500);
        soft_right_2_degrees(ang*-1);
        _delay_ms(2000);
        PORTA=0x00; //stop
        pen_Down();
        _delay_ms(1500);
    }
	
	//right turn with pen up
    else if(ang<0&&checkpen==0)
    {
        soft_right_2_degrees(ang*-1);
        _delay_ms(2000);
    }
}
/*
bot_turn() ends
*/

//Main function
void main(void)
{
    unsigned char i = 0;
    init_devices();
    lcd_set_4bit();
    lcd_init();

    pen_Up();
    _delay_ms(2000);
    pen_Down();
    _delay_ms(2000);

    pen_Up();
    _delay_ms(2000);
    pen_Down();
    _delay_ms(2000);

    while(data!='s')
    {
        //to tell how much time it is inside void
		lcd_cursor(2,1);
        lcd_string("inswhl");

    }//while 1 ends
    lcd_cursor(2,1);
    lcd_string("whlend");

    
	int chcount=0;
    for(chcount=0; simdata[chcount]!='s'; chcount++)
    {
        //to move bot forward by scale 
		if(simdata[chcount] == 0x66) //ASCII value of f
        {
			lcd_cursor(2,1);
			lcd_string(" forward ");
            forward_mm(distance*5);
            _delay_ms(2000);
        }
        
		//to move pen up
		else if(simdata[chcount]== 0x75) //ASCII value of u
        {
			lcd_cursor(2,1);
			lcd_string("  penup  ");
            pen_Up();
            _delay_ms(1500);
            checkpen=0;
        }
		
		//to move pen down
        else if(simdata[chcount]== 0x64) //ASCII value of d
        {
			lcd_cursor(2,1);
			lcd_string(" pendown ");

            pen_Down();
            _delay_ms(2000);
            checkpen=1;
        }
		
		//to turn bot left 30 degrees
        else if(simdata[chcount]== 0x6C) //ASCII value of l
        {
			lcd_cursor(2,1);
			lcd_string(" left    ");

            pen_Up();
            _delay_ms(2000);
            soft_left_degrees(30);
            _delay_ms(4000);
            PORTA=0x00; //stop
            pen_Down();
            _delay_ms(2000);
        }
		
		//to turn bot right 30
        else if(simdata[chcount]== 0x72) //ASCII value of r
        {
			lcd_cursor(2,1);
			lcd_string(" right   ");

            pen_Up();
            _delay_ms(2000);
            soft_right_degrees(30);
            _delay_ms(4000);
            PORTA=0x00; //stop
            pen_Down();
            _delay_ms(2000);
        }
		
		//turn bot left:: 15 degrees
        else if(simdata[chcount]== 0x01)
        {
			lcd_cursor(2,1);
			lcd_string(" left 15  ");
            bot_turn(15);
        }
		//turn bot left:: 30 degrees
        else if(simdata[chcount]== 0x02)
        {
			
			lcd_cursor(2,1);
			lcd_string(" left 30  ");
            bot_turn(30);
        }
		
		//turn bot right:: 15 degrees
        else if(simdata[chcount]== 0x03)
        {
			
			lcd_cursor(2,1);
			lcd_string("right 15  ");
            bot_turn(-15);
        }
        //turn bot left:: 60 degrees
		else if(simdata[chcount]== 0x04)
        {
			
			lcd_cursor(2,1);
			lcd_string(" left 60  ");
            bot_turn(59);
        }
        //turn bot left:: 90 degrees
		else if(simdata[chcount]== 0x06)
        {
			
			lcd_cursor(2,1);
			lcd_string(" left 90  ");
            bot_turn(86);
        }
		
		//turn bot left:: 120 degrees
        else if(simdata[chcount]== 0x08)
        {
			
			lcd_cursor(2,1);
			lcd_string(" left 120 ");
            bot_turn(116);
        }
		
		//turn bot left:: 150 degrees
        else if(simdata[chcount]== 0x32)
        {
			
			lcd_cursor(2,1);
			lcd_string(" left 150 ");
            bot_turn(146);
        }
		
		//turn bot left:: 180 degrees
        else if(simdata[chcount]== 0x34)
        {
			
			lcd_cursor(2,1);
			lcd_string(" left 180 ");
            bot_turn(176);
        }
		
		//turn bot right:: 180 degrees
        else if(simdata[chcount]== 0x0E)
        {
			
			lcd_cursor(2,1);
			lcd_string(" right 180");
            bot_turn(-184);
        }
		
		//turn bot right:: 150 degrees
        else if(simdata[chcount]== 0x10) 
        {
			
			lcd_cursor(2,1);
			lcd_string(" right 150");
            bot_turn(-154);
        }
		
		//turn bot right:: 120 degrees
        else if(simdata[chcount]== 0x12)
        {
			
			lcd_cursor(2,1);
			lcd_string(" right 120");
            bot_turn(-124);
        }
		
		//turn bot right::90 degrees
        else if(simdata[chcount]== 0x14)
        {
			
			lcd_cursor(2,1);
			lcd_string(" right 90 ");
            bot_turn(-94);
        }
		
		//turn bot right:: 60 degrees
        else if(simdata[chcount]== 0x16) 
        {
			
			lcd_cursor(2,1);
			lcd_string(" right 60 ");
            bot_turn(-62);
        }
		
		//turn bot right:: 30 degrees
        else if(simdata[chcount]== 0x18)
        {
			
			lcd_cursor(2,1);
			lcd_string(" right 30 ");
            bot_turn(-30);
        }
		
		//switch buzzer on
        else if(simdata[chcount]== 0x19)
        {
			
			lcd_cursor(2,1);
			lcd_string("buzzeron");
            buzzer_on();
        }
		
		//switch buzzer off
        else if(simdata[chcount]== 0x1A)
        {
			
			lcd_cursor(2,1);
			lcd_string("buzzeroff");
            buzzer_off();
        }
		
		//to select forward scale 10
        else if(simdata[chcount]== 0x1F)
        {
			
			lcd_cursor(2,1);
			lcd_string("dist -5");
            distance=10;
        }
		
		//to select forward scale 20
        else if(simdata[chcount]== 0x2A)
        {
			
			lcd_cursor(2,1);
			lcd_string("dist -10");
            distance=20;
        }
		
		//to select forward scale 30
        else if(simdata[chcount]== 0x21)
        {
			
			lcd_cursor(2,1);
			lcd_string("dist -15");
            distance=30;
        }
		
		//to select forward scale 40
        else if(simdata[chcount]== 0x22)
        {
			
			lcd_cursor(2,1);
			lcd_string("dist -20");
            distance=40;
        }
		
		//to select forward scale 50
        else if(simdata[chcount]== 0x23)
        {
			
			
			lcd_cursor(2,1);
			lcd_string("dist -25");
            distance=50;
        }
		
		//to select forward scale 60
        else if(simdata[chcount]== 0x24)
        {
			
			
			lcd_cursor(2,1);
			lcd_string("dist -30");
            distance=60;
        }
    }
	
	//bot stops with a buzzer
    if(simdata[chcount]== 0x73) //ASCII value of s
    {
		
		lcd_cursor(2,1);
		lcd_string("   stop   ");
        buzzer_on();
        _delay_ms(2000);
        buzzer_off();
    }

}

