#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <ctype.h>

 REGPACK regs;
 int 	x1,y1,x2,y2,HorBrds,VertBrds;
 char *fname;
 char *bmp;
 unsigned size;


typedef struct picsize
{
	char xsize,ysize;
};

 picsize pict;

int InitGRSystem()
{
	int Gd,Gm;
	Gd=DETECT;
	initgraph(&Gd,&Gm,"/lang/tcc/bgi");
	cleardevice();
	return 0;
}


char *StrInt(int I)
{
	char *Strn;
	itoa(I,Strn,10);
	return Strn;
}

void showcoords(int X,int Y)
{
	char *coords;
	setcolor(14);
	setfillstyle(1,0);
	outtextxy(5,31,"Coordinates");
	bar(5+textwidth(" Coordinates"),31,100+textwidth(" Coordinates"),38);
	strcpy(coords,StrInt(X));
	strcat(coords,":");
	strcat(coords,StrInt(Y));
	outtextxy(5+textwidth(" Coordinates"),31,coords);
}

void reset()
{
	asm mov ax ,5
	asm mov bx ,0
	asm int 0x33
	asm mov ax, 6
	asm mov bx, 0
	asm int 0x33
}

void initmouse()
{
	asm mov ax,0;
	asm int 0x33
}

void hidemouse()
{
	asm mov ax, 2
	asm int 0x33
}

void showmouse()
{
	asm mov ax, 1
	asm int 0x33
}

void keybox(int x1,int y1,int x2,int y2,char frame,char text,char *caption)
{
	int xcen,ycen,x;
	hidemouse();
	setcolor(frame);
	rectangle(x1,y1,x2,y2);
	xcen=x1 + ((x2-x1)/2);
	x=xcen-((textwidth(caption))/2);
	ycen=y1+(((y2-y1+1)-(textheight(caption)))/2);
	setcolor(text);
	outtextxy(x,ycen,caption);
	graphdefaults();
	showmouse();
}

void keyhit(int x1,int y1,int x2,int y2,char frame,char text,char *caption)
{
	int xcen,ycen,x;
	keybox(x1,y1,x2,y2,frame+8,text+8,caption);
	delay(250);
	hidemouse();
	setcolor(frame);
	rectangle(x1,y1,x2,y2);
	xcen=x1 + ((x2-x1)/2);
	x=xcen-((textwidth(caption))/2);
	ycen=y1+(((y2-y1+1)-(textheight(caption)))/2);
	setcolor(text);
	outtextxy(x,ycen,caption);
	graphdefaults();
	showmouse();
}

double computeradius(int x1,int y1,int x2,int y2)
{
	int radius,sqrsum,xsum,ysum;
	xsum=(x2-x1)*(x2-x1);
	ysum=(y2-y1)*(y2-y1);
	sqrsum=ceil(sqrt(xsum+ysum));
	radius=sqrsum;
	return radius;
}

void freehand(char Col)
{
	reset();
	while (1)
	{
		regs.r_ax=5;
		regs.r_bx=0;
		intr(0x33,&regs);
		regs.r_ax=3;
		regs.r_bx=0;
		intr(0x33,&regs);
		if (regs.r_dx < 43) break;
		if (kbhit()) break;
		if (regs.r_bx == 1)
		{
			while (regs.r_bx == 1)
			{
				x1=regs.r_cx;
				y1=regs.r_dx;
				delay(10);
				regs.r_ax=3;
				regs.r_bx=0;
				intr(0x33,&regs);
				x2=regs.r_cx;
				y2=regs.r_dx;
				hidemouse();
				setcolor(Col);
				line(x1,y1,x2,y2);
				showmouse();
				x1=x2;
				y2=y2;
				if (regs.r_bx == 1) break;
				if (regs.r_dx < 43) break;
				if (kbhit()) break;
			}
	 }
 }
 reset();
}

void drawline(char Col)
{
	reset();
	while (1)
	{
		regs.r_ax=5;
		regs.r_bx=0;
		intr(0x33,&regs);
		regs.r_ax=3;
		intr(0x33,&regs);
		y1=regs.r_dx;
		x1=regs.r_cx;
		if (kbhit()) break;
		if (regs.r_dx < 43) break;
		if (regs.r_bx == 1)
		{
			while (1)
			{
				regs.r_ax=6;
				regs.r_bx=0;
				intr(0x33,&regs);
				y2=regs.r_dx;
				x2=regs.r_cx;
				delay(10);
				if (regs.r_bx == 1)
				{
					hidemouse();
					setcolor(Col);
					line(x1,y1,x2,y2);
					showmouse();
					break;
				}
			}
		}
	}
	reset();
}

void drawcircle(char Col)
{
	int rad;
	reset();
	while (1)
	{
		regs.r_ax=5;
		regs.r_bx=0;
		intr(0x33,&regs);
		regs.r_ax=3;
		intr(0x33,&regs);
		y1=regs.r_dx;
		x1=regs.r_cx;
		if (kbhit()) break;
		if (regs.r_dx < 43) break;
		if (regs.r_bx == 1)
		{
			while (1)
			{
				regs.r_ax=6;
				regs.r_bx=0;
				intr(0x33,&regs);
				y2=regs.r_dx;
				x2=regs.r_cx;
        delay(10);
				if (regs.r_bx == 1)
				{
					hidemouse();
					setcolor(Col);
					rad=computeradius(x1,y1,x2,y2);
					circle(x1,y1,rad);
					showmouse();
					break;
				}
			}
		}
	}
	reset();
}

void drawrectangle(char Col)
{
	reset();
	while (1)
	{
		regs.r_ax=5;
		regs.r_bx=0;
		intr(0x33,&regs);
		regs.r_ax=3;
		intr(0x33,&regs);
		y1=regs.r_dx;
		x1=regs.r_cx;
		if (kbhit()) break;
		if (regs.r_dx < 43) break;
		if (regs.r_bx == 1)
		{
			while (1)
			{
				regs.r_ax=6;
				regs.r_bx=0;
				intr(0x33,&regs);
				y2=regs.r_dx;
				x2=regs.r_cx;
				delay(10);
				if (regs.r_bx == 1)
				{
					hidemouse();
					setcolor(Col);
					rectangle(x1,y1,x2,y2);
					showmouse();
					break;
				}
			}
		}
	}
	reset();
}

void drawellipse(char Col)
{
	int xrad,yrad;
	reset();
	while (1)
	{
		regs.r_ax=5;
		regs.r_bx=0;
		intr(0x33,&regs);
		regs.r_ax=3;
		intr(0x33,&regs);
		y1=regs.r_dx;
		x1=regs.r_cx;
		if (kbhit()) break;
		if (regs.r_dx < 43) break;
		if (regs.r_bx == 1)
		{
			while (1)
			{
				regs.r_ax=6;
				regs.r_bx=0;
				intr(0x33,&regs);
				y2=regs.r_dx;
				x2=regs.r_cx;
				delay(10);
				if (regs.r_bx == 1)
				{
					hidemouse();
					setcolor(Col);
					xrad=abs(x2-x1)+1;
					yrad=abs(y2-y1)+1;
					ellipse(x1,y1,0,360,xrad,yrad);
					showmouse();
					break;
				}
			}
		}
	}
	reset();
}

void colorfill(char FillCol,char Col)
{
	reset();
	while (1)
	{
		regs.r_ax=5;
		regs.r_bx=0;
		intr(0x33,&regs);
		regs.r_ax=3;
		intr(0x33,&regs);
		y1=regs.r_dx;
		x1=regs.r_cx;
		if (kbhit()) break;
		if (regs.r_dx < 43) break;
		if (regs.r_bx == 1)
		{
			setfillstyle(1,FillCol);
			hidemouse();
			floodfill(x1,y1,Col);
			showmouse();
		}
	}
	reset();
}

void eraser()
{
	reset();
	while (1)
	{
		regs.r_ax=5;
		regs.r_bx=0;
		intr(0x33,&regs);
		regs.r_ax=3;
		regs.r_bx=0;
		intr(0x33,&regs);
		if (regs.r_dx < 43) break;
		if (kbhit()) break;
		if (regs.r_bx == 1)
		{
			while (regs.r_bx == 1)
			{
				x1=regs.r_cx;
				y1=regs.r_dx;
				delay(15);
				regs.r_ax=3;
				regs.r_bx=0;
				intr(0x33,&regs);
				hidemouse();
				setfillstyle(1,0);
				bar(x1,y1,x1+8,y1+8);
				showmouse();
				x1=x2;
				y2=y2;
				if (regs.r_bx == 1) break;
			}
	 }
 }
	reset();
}

void ToolBar()
{
	int Cter;
	int Gap,X;
	Gap=20;
	X=30;
	setcolor(15);
	rectangle(0,0,639,50);
	outtextxy(7,7,"FG");
	outtextxy(7,19,"BG");
	for (Cter=0;Cter<=15;Cter++)
	{
		setfillstyle(1,Cter);
		bar(X,5,X+19,15);
		X=X+Gap;
	}
	Gap=20;
	X=30;
	for (Cter=0;Cter<=15;Cter++)
	{
		setfillstyle(1,Cter);
		bar(X,17,X+Gap,27);
		X=X+Gap;
	}
	setcolor(14);
	outtextxy(10,35,"NONAME.IMG");
	keybox(354,2,399,22,14,15,"CIRC");
	keybox(404,2,449,22,14,15,"FH");
	keybox(454,2,499,22,14,15,"LINE");
	keybox(504,2,544,22,14,15,"ELPS");
	keybox(549,2,589,22,14,15,"FILL");
	keybox(594,2,634,22,14,15,"QUIT");
	keybox(354,25,399,45,14,15,"ERASE");
	keybox(404,25,449,45,14,15,"RECT");
	keybox(454,25,499,45,14,15,"SAVE");
	keybox(504,25,544,45,14,15,"OPEN");
	keybox(549,25,589,45,14,15,"CLR");
}

void centertext(int x1,int x2,int y,char col,char *text)
{
	int xcen,x;
	xcen=x1 + ((x2-x1+1)/2);
	x=xcen-((textwidth(text))/2);
	graphdefaults();
	setcolor(col);
	outtextxy(x,y,text);
}

char *getfilename(int X,int Y,int *saved)
{
	char *Input,key;
	int count=0,size=1,xpos=X;
	int xwidth=textwidth("X");
	while (1)
	{
		while (1)
		{
			 regs.r_ax=5;
			 regs.r_bx=0;
			 intr(0x33,&regs);
			 setcolor(0);
			 outtextxy(xpos,Y,"Ý");
			 delay(25);
			 setcolor(10);
			 outtextxy(xpos,Y,"Ý");
			 delay(15);
			 if (kbhit()) break;
			 if (regs.r_bx !=0) break;
		}
		if (regs.r_dx >=228 && regs.r_dx <=248)
		{
			if (regs.r_cx >=255 && regs.r_cx <=315)
			{
				 keyhit(255,228,315,248,15,14,"OK");
				 key=13;
				 *saved=0;
			}
			if (regs.r_cx >=325 && regs.r_cx <=385)
			{
				 keyhit(325,228,385,248,15,14,"CANCEL");
				 key=13;
				 *saved=1;
			}
		}
		if (kbhit())
		{
			 key=getch();
			 switch (key)
			 {
					case 13: *saved=0;
									 break;
					case  8: if (count > 0 && xpos > X && size >1)
									 {
										 Input[count]='\0';
										 count--;
										 xpos-=xwidth;
										 size--;
										 setfillstyle(1,0);
										 bar(xpos,Y,400,Y+9);
									 }
									 break;
					default: if (size <=12)
									 {
											 if (!isascii(key)) continue;
											 Input[count]=key;
											 setfillstyle(1,0);
											 bar(X,Y,400,Y+9);
											 setcolor(15);
											 outtextxy(X,Y,Input);
											 size++;
											 count++;
											 xpos+=xwidth;
									 }
									 break;
			 }
		}
	if (key==13) break;
	}
	Input[count]='\0';
	return  Input;
}

void savetofile(int Hor,int Vert)
{
	int x,y,filehandle;
	char cter1,cter2;
	filehandle=open(fname, O_CREAT | O_BINARY);
	x=6;
	y=55;
	for (cter1=1;cter1<=Vert;cter1++)
	{
		for (cter2=1;cter2<=Hor;cter2++)
		{
			getimage(x,y,x+79,y+59,&bmp);
			write(filehandle,bmp,sizeof(bmp));
			x+=80;
		}
		y+=60;
		x=6;
	}
	pict.xsize=HorBrds;
	pict.ysize=VertBrds;
	write(filehandle,&pict,sizeof(pict));
	close(filehandle);
}

void savediagbox()
{
	int retstat;
	void *image;
	unsigned size;
	size=imagesize(200,150,439,255);
	image=malloc(size);
	getimage(200,150,439,255,image);
	setfillstyle(1,0);
	bar(200,150,439,255);
	setcolor(15);
	rectangle(200,150,439,255);
	centertext(200,439,160,14,"SAVING DRAWING");
	setcolor(15);
	rectangle(225,200,404,216);
	setcolor(7);
	outtextxy(225,190,"Save image as :");
	keybox(255,228,315,248,15,14,"OK");
	keybox(325,228,385,248,15,14,"CANCEL");
	fname=getfilename(230,204,&retstat);
	hidemouse();
	putimage(200,150,image,COPY_PUT);
	showmouse();
	free(image);
	if (retstat==0)
	{
		setfillstyle(1,0);
		bar(10,35,10+textwidth("NONAME00.IMG"),45);
		setcolor(14);
		outtextxy(10,35,fname);
		savetofile(HorBrds,VertBrds);
	}
}


void getinfofirst()
{
	clrscr();
	gotoxy(5,5);
	cputs("Number of horizontal boards : ");
	gotoxy(5,6);
	cputs("Number of vertical boards   : ");
	gotoxy(35,5); scanf("%d",&HorBrds);
	gotoxy(35,6); scanf("%d",&VertBrds);
}

void canvas(int Hor,int Vert)
{
	int Xlen,YLen;
	Xlen=80*Hor;
	YLen=60*Vert;
	rectangle(5,54,6+Xlen,55+YLen);
}


void clear(char col)
{
	 hidemouse();
	 setfillstyle(1,0);
	 bar(0,52,639,479);
	 setcolor(15);
	 rectangle(0,52,639,479);
	 setcolor(col);
	 canvas(HorBrds,VertBrds);
	 showmouse();
}

void MainScreen()
{
	char buff,key,DrawMode=1;
	char FG=15,BG=0;
	initmouse();
	showmouse();
	FG=15;
	BG=0;
	setcolor(15);
	rectangle(0,52,639,479);
	while (1)
	{
		 setcolor(FG);
		 canvas(HorBrds,VertBrds);
		 while (1)
		 {
				regs.r_ax=5;
				regs.r_bx=0;
				intr(0x33,&regs);
				if (regs.r_bx !=0) break;
				if (kbhit()) break;
		 }
		 if (regs.r_dx >=5 && regs.r_dx <= 15)
		 {
				if (regs.r_cx >= 30  && regs.r_cx <=49)  FG=0;
				else if (regs.r_cx >= 50  && regs.r_cx <=69)  FG=1;
				else if (regs.r_cx >= 70  && regs.r_cx <=89)  FG=2;
				else if (regs.r_cx >= 90  && regs.r_cx <=109) FG=3;
				else if (regs.r_cx >= 110 && regs.r_cx <=129) FG=4;
				else if (regs.r_cx >= 130 && regs.r_cx <=149) FG=5;
				else if (regs.r_cx >= 150 && regs.r_cx <=169) FG=6;
				else if (regs.r_cx >= 170 && regs.r_cx <=189) FG=7;
				else if (regs.r_cx >= 190 && regs.r_cx <=209) FG=8;
				else if (regs.r_cx >= 210 && regs.r_cx <=229) FG=9;
				else if (regs.r_cx >= 230 && regs.r_cx <=249) FG=10;
				else if (regs.r_cx >= 250 && regs.r_cx <=269) FG=11;
				else if (regs.r_cx >= 270 && regs.r_cx <=289) FG=12;
				else if (regs.r_cx >= 290 && regs.r_cx <=309) FG=13;
				else if (regs.r_cx >= 310 && regs.r_cx <=329) FG=14;
				else if (regs.r_cx >= 330 && regs.r_cx <=349) FG=15;
		 } else
		 if (regs.r_dx >=17 && regs.r_dx <=27)
		 {
				if (regs.r_cx >= 30  && regs.r_cx <=49)  BG=0;
				else if (regs.r_cx >= 50  && regs.r_cx <=69)  BG=1;
				else if (regs.r_cx >= 70  && regs.r_cx <=89)  BG=2;
				else if (regs.r_cx >= 90  && regs.r_cx <=109) BG=3;
				else if (regs.r_cx >= 110 && regs.r_cx <=129) BG=4;
				else if (regs.r_cx >= 130 && regs.r_cx <=149) BG=5;
				else if (regs.r_cx >= 150 && regs.r_cx <=169) BG=6;
				else if (regs.r_cx >= 170 && regs.r_cx <=189) BG=7;
				else if (regs.r_cx >= 190 && regs.r_cx <=209) BG=8;
				else if (regs.r_cx >= 210 && regs.r_cx <=229) BG=9;
				else if (regs.r_cx >= 230 && regs.r_cx <=249) BG=10;
				else if (regs.r_cx >= 250 && regs.r_cx <=269) BG=11;
				else if (regs.r_cx >= 270 && regs.r_cx <=289) BG=12;
				else if (regs.r_cx >= 290 && regs.r_cx <=309) BG=13;
				else if (regs.r_cx >= 310 && regs.r_cx <=329) BG=14;
				else if (regs.r_cx >= 330 && regs.r_cx <=349) BG=15;
		 }
		 if (regs.r_dx >= 2 && regs.r_dx <=22)
		 {
				if (regs.r_cx >=354 && regs.r_cx <=399)
				{
					 keyhit(354,2,399,22,14,15,"CIRC");
					 DrawMode=5;
				} else
				if (regs.r_cx >=404 && regs.r_cx <=449)
				{
					 keyhit(404,2,449,22,14,15,"FH");
					 DrawMode=1;
				}else
				if (regs.r_cx >=454 && regs.r_cx <=499)
				{
					 keyhit(454,2,499,22,14,15,"LINE");
					DrawMode=2;
				} else
				if (regs.r_cx >=504 && regs.r_cx <=544)
				{
					 keyhit(504,2,544,22,14,15,"ELPS");
					 DrawMode=3;
				} else
				if (regs.r_cx >=549 && regs.r_cx <=589)
				{
					 keyhit(549,2,589,22,14,15,"FILL");
					 DrawMode=4;
				} else
				if (regs.r_cx >=594 && regs.r_cx <=634)
				{
					 keyhit(594,2,634,22,14,15,"QUIT");
					 break;
				}
		 } else
		 if (regs.r_dx >=25 && regs.r_dx <=45)
		 {
				if (regs.r_cx >=354 && regs.r_cx <=399)
				{
					keyhit(354,25,399,45,14,15,"ERASE");
					DrawMode=6;
				}
				if (regs.r_cx >= 404 && regs.r_cx <=449)
				{
					keyhit(404,25,449,45,14,15,"RECT");
					DrawMode=7;
				}
				if (regs.r_cx >= 549 && regs.r_cx <=589)
				{
					keyhit(549,25,589,45,14,15,"CLR");
					DrawMode=8;
				}
				if (regs.r_cx >= 454 && regs.r_cx <=499)
				{
					keyhit(454,25,499,45,14,15,"SAVE");
					buff=DrawMode;
					DrawMode=9;
				}
		 }
		 if (kbhit())
		 {
				key=getch();
				switch (key)
				{
					case 27: keyhit(594,2,634,22,14,15,"QUIT");
									 closegraph();
									 return;
					default: break;
				}
		 }
		 switch (DrawMode)
		 {
				case 1: freehand(FG);
								break;
				case 2: drawline(FG);
								break;
				case 3: drawellipse(FG);
								break;
				case 4: colorfill(BG,FG);
								break;
				case 5: drawcircle(FG);
								break;
				case 6: eraser();
								break;
				case 7: drawrectangle(FG);
								break;
				case 8: clear(FG);
								break;
				case 9: savediagbox();
								DrawMode=buff;
								break;
				default:break;
		 }
	}
}


void main()
{
	getinfofirst();
	InitGRSystem();
	ToolBar();
	MainScreen();
	closegraph();
}
