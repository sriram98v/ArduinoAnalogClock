/*********************************************************************
Arduino OLED Analog Clock / Philip Vallone

Uses Adafruit_GFX && Adafruit_SSD1306 Libraries
 https://github.com/adafruit/Adafruit_SSD1306
 https://github.com/adafruit/Adafruit-GFX-Library
This code is derived from equations found in Project: UTFT Analog Clock http://www.henningkarlsen.com/electronics/p_utft_analog_clock.php

Hook up 5v(or 3v), GND, SDA to A4 and SCL to A5 
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

uint8_t CENTER_X;
uint8_t CENTER_Y;
uint8_t INNER_R = 30;
uint8_t OUTTER_R = 27;
uint8_t seconds = 0;
uint8_t minutes = 18;
uint8_t hours = 5;

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {              
	
    display.clearDisplay();
	 // initialize with the I2C addr 0x3C (for the 128x64) - if no display, then run I2C scanner to find correct address
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 

    CENTER_X = display.width()/2;
    CENTER_Y = display.height()/2;
	
	addNumbers();
	drawHourMarks(CENTER_X, CENTER_Y, OUTTER_R, CENTER_X, CENTER_Y, INNER_R);	
    display.display();
}

void loop() {

	drawHour(hours);
	drawHands(minutes, 0);
	drawHands(seconds, 1);
	addNumbers();// redraw the numbers as the second hand will wash them out
	display.display();
	delay(1000);
	
	seconds++;
	if(seconds == 61){
			
		seconds = 1;
		minutes++;

		if(minutes == 61){
				
			minutes = 1;
			hours++;

			if(hours == 13){
					
				hours == 0;
			}
		}
	}
}

void addNumbers(){
	
	display.setTextColor(WHITE);
	display.setTextSize(1);
	display.drawCircle(CENTER_X, CENTER_Y, INNER_R, WHITE);
	display.setCursor( CENTER_X+20, CENTER_Y-4);
	display.print("3");
	display.setCursor( CENTER_X-2, CENTER_Y+18);
	display.print("6");
	display.setCursor(CENTER_X-22, CENTER_Y-4);
	display.print("9");
	display.setCursor(CENTER_X-5, CENTER_Y-23);
	display.print("12");
	
}

void drawHourMarks(int16_t x0, int16_t y0, int16_t r0, int16_t x1, int16_t y1, int16_t r1){
	
	// inner xy	
	int16_t f0 = 1 - r0;
	int16_t ddF_x0 = 1;
	int16_t ddF_y0 = -2 * r0;
	int16_t x = 0;
	int16_t y = r0;	

	// draw marks at 12,3,6,9    			  
	display.drawLine(x0, y0 + r0,x1, y1+r1, WHITE);//6
	display.drawLine(x0, y0 - r0,x1, y1-r1, WHITE);//12
	display.drawLine(x0+r0, y0,x1+r1, y1, WHITE);//3
	display.drawLine(x0-r0, y0,x1-r1, y1, WHITE);//9
	
	for (int i=0; i<12; i++)
	{
		drawMark(i);	  
		
	}
	
}
void drawMark(int h)
{
	float x1, y1, x2, y2;
	
	h=h*30;
	h=h+270;
	x1=OUTTER_R*cos(h*0.0175);
	y1=OUTTER_R*sin(h*0.0175);
	x2=INNER_R*cos(h*0.0175);
	y2=INNER_R*sin(h*0.0175);

    display.drawLine(x1+CENTER_X, y1+CENTER_Y, x2+CENTER_X, y2+CENTER_Y, WHITE);
}
void drawHands(int t, uint8_t isSecond)
{
	uint8_t len = 25;
	
	if(isSecond == 1){
		len =8;
	}
	float x1, y1, x2, y2;
	int ps = t-1;
	
	if (ps==-1){
		
		ps=59;			
		
	}
	ps=ps*6;
	ps=ps+270;
	
	x1=len*cos(ps*0.0175);
	y1=len*sin(ps*0.0175);
	x2=1*cos(ps*0.0175);
	y2=1*sin(ps*0.0175);
	
	display.drawLine(x1+CENTER_X, y1+CENTER_Y, x2+CENTER_X, y2+CENTER_Y, BLACK);
	if(isSecond == 0){
		display.fillCircle(x1+CENTER_X, y1+CENTER_Y, 1, BLACK);
	}
	t=t*6;
	t=t+270;
	
	x1=len*cos(t*0.0175);
	y1=len*sin(t*0.0175);
	x2=1*cos(t*0.0175);
	y2=1*sin(t*0.0175);
	
	display.drawLine(x1+CENTER_X, y1+CENTER_Y, x2+CENTER_X, y2+CENTER_Y, WHITE);
	if(isSecond == 0){
		display.fillCircle(x1+CENTER_X, y1+CENTER_Y, 1, WHITE);	
	}
	
}
void drawHour(int h)
{
	uint16_t m = minutes;
	float x1, y1, x2, y2;
	int ph = h;

	if (m==0)
	{
		ph=((ph-1)*30)+((m+59)/2);
	}
	else
	{
		ph=(ph*30)+((m-1)/2);
	}
	ph=ph+270;
	
	x1=15*cos(ph*0.0175);
	y1=15*sin(ph*0.0175);
	x2=1*cos(ph*0.0175);
	y2=1*sin(ph*0.0175);
	
	display.drawLine(x1+CENTER_X, y1+CENTER_Y, x2+CENTER_X, y2+CENTER_Y, BLACK);
	display.fillCircle(x1+CENTER_X, y1+CENTER_Y, 1, BLACK);

	h=(h*30)+(m/2);
	h=h+270;
	
	x1=15*cos(h*0.0175);
	y1=15*sin(h*0.0175);
	x2=1*cos(h*0.0175);
	y2=1*sin(h*0.0175);
	 
   display.drawLine(x1+CENTER_X, y1+CENTER_Y, x2+CENTER_X, y2+CENTER_Y, WHITE);
   display.fillCircle(x1+CENTER_X, y1+CENTER_Y, 1, WHITE);
}


  
