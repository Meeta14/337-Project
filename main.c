#include <at89c5131.h>
#include "lcd.h"		//Header file with LCD interfacing functions
#include "serial.c"	//C file with UART interfacing functions

sbit LED=P1^7;

unsigned int score=0;
unsigned char overs=0;
unsigned char wickets=0;
unsigned char ballnum=0;
unsigned char val=0;
unsigned char temp=0;
unsigned char pos=0;
//Test function definitions

void lcd_start_scoring(void)
{  
	 lcd_cmd(0x01);	// LCD clear
	 msdelay(4);
	 lcd_cmd(0x83);
	 lcd_write_string("0/0 (0.0)");	
	 lcd_cmd(0xC0);
}	

void update_score(void){
	 lcd_cmd(0x83);
	 ////score
	 val=score;
	 val = 48+((val%1000)/100);
	 //lcd_write_char(val);
	 if(val!=48){lcd_write_char(val);}
	 
	 temp=score;
   temp=48+((temp%100)/10);
	 if(val!=48 || (val==48 && temp!=48)){lcd_write_char(temp);}
	// lcd_write_char(val);

	 val=score;
	 val=48+(val%10); 
	 lcd_write_char(val);
	 
	 lcd_write_string("/");	
	
	////wickets
	 if(wickets==10){lcd_write_string("10");}
	 else{lcd_write_char(wickets+48);}	 
	 lcd_write_string(" (");
	 
	 ///overs
	 val=overs;
   val=48+((val%100)/10);
	 if(val!=48){lcd_write_char(val);}
	 val=overs;
	 val=48+(val%10); 
	 lcd_write_char(val);
	 
	 lcd_write_char('.');
	 
	 ///balls
	 lcd_write_char(ballnum+48);
	 lcd_write_char(')');
	
}
/*
void end_innings(void)
{
		 update_score();
		 lcd_cmd(0xC0);
		 lcd_write_string("  Innings Ends  ");
		 transmit_string("\r\n");
		 transmit_string("Innings Ends\r\n");
		 msdelay(5000);
		 lcd_start_scoring();
		 score=0;
		 overs=0;
		 wickets=0;
		 ballnum=0;
		 val=0;
		 temp=0;
		 pos=0;
}	
*/
//Main function
void main(void)
{
	unsigned char ch=0;
	
	//Initialize port P1 for output from P1.7-P1.4
	P1 = 0x0F;
	
	//Call initialization functions
	lcd_init();
	uart_init();
	
	//These strings will be printed in terminal software
	transmit_string("************************\r\n");
	transmit_string("*****Cricket Score******\r\n");
	transmit_string("************************\r\n");
	//transmit_string("Press 1 for LCD test\r\n");
	//transmit_string("Press 2 for LED test\r\n");
	
	lcd_start_scoring();
	
	while(1)
	{
			//Receive a character
			ch = receive_char();
      temp = 0xC0;
		  temp = temp+pos;
		  lcd_cmd(temp);
			//Decide which test function to run based on character sent
      //Displays the string on the terminal software
			switch(ch)
			{
				case '.':
								ballnum=ballnum+1;
								lcd_write_char('.');
								transmit_string(".");
								break;				
				case 'w':
								ballnum=ballnum+1;
								wickets=wickets+1;
								lcd_write_char('W');
								transmit_string("W");
								break;
				case 'e':
								score=score+1;
								lcd_write_char('E');
								transmit_string("E");
								break;
				case 'n':
								score=score+1;
								lcd_write_char('N');
								transmit_string("N");
								break;
				case '1' :
								score=score+1;
								ballnum=ballnum+1;
				        lcd_write_char('1');
								transmit_string("1");
								break;	
				case '2' :
								score=score+2;
								ballnum=ballnum+1;
				        lcd_write_char('2');
								transmit_string("2");
								break;	
				case '3' :
								score=score+3;
								ballnum=ballnum+1;
				        lcd_write_char('3');
								transmit_string("3");
								break;	
				case '4' :
								score=score+4;
								ballnum=ballnum+1;
				        lcd_write_char('4');
								transmit_string("4");
								break;	
				case '5' :
								score=score+5;
								ballnum=ballnum+1;
				        lcd_write_char('5');
				        transmit_string("5");
								break;					
				case '6' :
								score=score+6;
								ballnum=ballnum+1;
				        lcd_write_char('6');
				        transmit_char('6');
								break;					
				default:transmit_string("Incorrect score\r\n");
				         pos--;
								 break;
				
			}
			pos=pos+1;
			msdelay(100);
			if(ballnum==6){
					overs++;
					ballnum=0;
					transmit_string("\r\n");
					msdelay(100);
				  lcd_cmd(0xC0);
					lcd_write_string("                ");
				  pos = 0;
					lcd_cmd(0xC0);
			}
			
		 if(overs==20 || wickets==10){
						//end_innings();
						 update_score();
						 lcd_cmd(0xC0);
						 lcd_write_string("  Innings Ends  ");
						 transmit_string("\r\n");
						 transmit_string("Innings Ends\r\n");
						 msdelay(5000);
						 lcd_start_scoring();
						 score=0;
						 overs=0;
						 wickets=0;
						 ballnum=0;
						 val=0;
						 temp=0;
						 pos=0; 
		 }
		 else{
		 update_score();
		 }
	}
}
