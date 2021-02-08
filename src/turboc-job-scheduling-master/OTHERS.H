#include <dos.h>
#include <graphics.h>

REGPACK regs;

void far reset()
{
	asm mov ax ,5
	asm mov bx ,0
	asm int 0x33
	asm mov ax, 6
	asm mov bx, 0
	asm int 0x33
}

void far initmouse()
{
	asm mov ax,0;
	asm int 0x33
}

void far hidemouse()
{
	asm mov ax, 2
	asm int 0x33
}

void far showmouse()
{
	asm mov ax, 1
	asm int 0x33
}

void far mousearea(int x1, int y1,int x2,int y2)
{
		asm mov ax,7
		asm mov cx,x1
		asm mov dx,x2
		asm int 33h
		asm mov ax,8
		asm mov cx,y1
		asm mov dx,y2
		asm int 33h
}

void far rowcol(int x,int y)
{
	asm mov ax, 4
	asm mov bx, x
	asm mov dx, y
	asm int 33h
}

void far initgraphics()
{
	int Gd=DETECT,Gm;
	initgraph(&Gd,&Gm,"/lang/tcc/bgi");
	cleardevice();
}

void far newbutton(int x1,int y1, int x2,int y2,char col,char tcol,char caption[])
{
	int y,x,xcenter;
	graphdefaults();
	setfillstyle(1,col);
	bar(x1,y1,x2,y2);
	setcolor(col+4);
	rectangle(x1+2,y1+2,x2-2,y2-2);
	xcenter=((x2-x1+1)-textwidth(caption))/2;
	x=x1+xcenter;
	y=y1+(((y2-y1+1)-textheight(caption))/2);
	setcolor(tcol);
	outtextxy(x+1,y+1,caption);
}

void far buttondown(int x1,int y1, int x2,int y2,char col,char tcol,char caption[])
{
	int y,x,xcenter;
	graphdefaults();
	setfillstyle(1,col);
	bar(x1,y1,x2,y2);
	setcolor(col+4);
	rectangle(x1+2,y1+2,x2-2,y2-2);
	xcenter=((x2-x1+1)-textwidth(caption))/2;
	x=x1+xcenter;
	y=y1+(((y2-y1+1)-textheight(caption))/2);
	setcolor(tcol);
	outtextxy(x+2,y+2,caption);
}

void far button(int x1,int y1, int x2,int y2,char col,char tcol,char caption[],char status)
{
	if (status==0) newbutton(x1,y1,x2,y2,col,tcol,caption);
	else if (status != 0) buttondown(x1,y1,x2,y2,col+8,tcol+8,caption);
}

void far push(int X1,int Y1,int X2,int Y2,char col,char tcol,char Heading[],int *RetVal,int State)
{
	if (State==0)
	{
		hidemouse();
		button(X1,Y1,X2,Y2,col,tcol,Heading,1);
		showmouse();
		while (regs.r_bx==1 || regs.r_bx==2)
		{
			 regs.r_ax=3;
			 intr(0x33,&regs);
			 if (((regs.r_cx > X1) &&(regs.r_cx < X2)) &&
					((regs.r_dx > Y1) && (regs.r_dx < Y2))) *RetVal=1;
			 else
			 {
					hidemouse();
					button(X1,Y1,X2,Y2,col,tcol,Heading,0);
					showmouse();
					*RetVal=0;
					break;
			 }
		}
	 }
	 else if (State==1)
	 {
		 hidemouse();
		 button(X1,Y1,X2,Y2,col,tcol,Heading,0);
		 showmouse();
		 delay(200);
		 *RetVal=1;
	 }
	 if (*RetVal==1)
	 {
		 hidemouse();
		 button(X1,Y1,X2,Y2,col,tcol,Heading,0);
		 showmouse();
		 delay(20);
	 }
}

void far popbox(int x,int y)
{
	setcolor(15);
	hidemouse();
	rectangle(x,y,x+100,y+125);
	showmouse();
}

