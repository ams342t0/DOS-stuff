 #include <others.h>
 #include <graphics.h>
 #include <stdio.h>
 #include <conio.h>
 #include <stdlib.h>

 void *bmp2;
 char bmp[100];
 unsigned size;
 int pendingjobs=0,jobsdone=0;
 int xbufpos=107 ,ybufpos=365;
 float totaltime=0,avewait=0;
 char sounds=0;

 void job_in_buffer();
 void job_in_cpu();
 void job_out_cpu();
 void processing();
 void generatejob();
 void shownumjobs();
 void showwaittime();
 void menubox(char text[]);
 void callmenu();

 typedef struct cpujob
 {
		int timealloc;
 }jobs;

 void centertext(int x1,int x2,int y,char FG,char size,char text[])
 {
	 settextstyle(0,0,size);
	 int xcen=((x2-x1+1)-textwidth(text))/2;
	 setcolor(FG);
	 outtextxy(x1+xcen,y,text);
	 graphdefaults();
 }

 void shadow(int x1,int y1,int x2,int y2)
 {
	 graphdefaults();
	 setfillstyle(1,0);
	 bar(x1,y1,x2,y2);
 }

 void window1(int x1,int y1,int x2,int y2,char FG,char BG,char shad)
 {
	 if (shad==1) shadow(x1+10,y1+10,x2+10,y2+10);
	 setcolor(FG);
	 setlinestyle(0,0,THICK_WIDTH);
	 rectangle(x1,y1,x2,y2);
	 setfillstyle(1,BG);
	 floodfill(x1+5,y1+5,FG);
 }


 void soundfx()
 {
	 if (sounds==0) return;
	 sound(250);
	 delay(20);
	 nosound();
 }

 void soundpush()
 {
	 if (sounds==0) return;
	 sound(1005);
	 delay(10);
	 sound(700);
	 delay(20);
	 sound(600);
	 delay(10);
	 sound(500);
	 delay(5);
	 nosound();
 }

 void soundpop()
 {
	 if (sounds==0) return;
	 sound(500);
	 delay(10);
	 sound(600);
	 delay(20);
	 sound(700);
	 delay(10);
	 sound(1005);
	 delay(5);
	 nosound();
 }

 void reducebuffer()
 {
	 ybufpos+=15;
	 putimage(xbufpos,ybufpos,bmp,XOR_PUT);
 }

 void job_in_cpu()
 {
	 int ctr,xpos=150,ypos=385;
	 if (pendingjobs > 0)
	 {
		 reducebuffer();
		 pendingjobs--;
		 shownumjobs();
		 soundpop();
		 for (ctr=ypos;ctr <= 420; ctr +=10)
		 {
			 soundfx();
			 putimage(xpos,ctr,bmp,XOR_PUT);
			 delay(30);
			 putimage(xpos,ctr,bmp,XOR_PUT);
			 callmenu();
		 }
		 ypos=ctr;
		 for (ctr=xpos;ctr <= 320; ctr+=10,xpos+=10)
		 {
			 soundfx();
			 putimage(ctr,ypos,bmp,XOR_PUT);
			 delay(30);
			 putimage(ctr,ypos,bmp,XOR_PUT);
			 callmenu();
		 }
		 for (ctr=ypos;ctr >= 274; ctr-=10)
		 {
			 soundfx();
			 putimage(xpos,ctr,bmp,XOR_PUT);
			 delay(30);
			 putimage(xpos,ctr,bmp,XOR_PUT);
			 callmenu();
		 }
		 soundpush();
		 processing();
		 }
 }

 void fillbuffer()
 {
	 putimage(xbufpos,ybufpos,bmp,COPY_PUT);
	 ybufpos-=15;
	 if (ybufpos <= 160)
	 {
		 xbufpos+=15;
		 ybufpos=365;
	 }
 }

 void job_out_cpu()
 {
	 int ctr,xpos=320,ypos=146;
	 pendingjobs++;
	 soundpop();
	 for (ctr=ypos; ctr >= 80;ctr-=10)
	 {
		 soundfx();
		 putimage(xpos,ctr,bmp,XOR_PUT);
		 delay(30);
		 putimage(xpos,ctr,bmp,XOR_PUT);
		 callmenu();
	 }
	 ypos=ctr;
	 for (ctr=xpos;ctr >= 170; ctr-=10)
	 {
		 soundfx();
		 putimage(ctr,ypos,bmp,XOR_PUT);
		 delay(30);
		 putimage(ctr,ypos,bmp,XOR_PUT);
		 callmenu();
	 }
	 xpos=ctr;
	 for (ctr=ypos;ctr <= 146; ctr+=10)
	 {
		 soundfx();
		 putimage(xpos,ctr,bmp,XOR_PUT);
		 delay(30);
		 putimage(xpos,ctr,bmp,XOR_PUT);
		 callmenu();
	 }
	 fillbuffer();
 }


 void job_in_buff()
 {
	 int ctr,xpos=130,ypos=60;
	 shownumjobs();
	 soundpop();
	 for (ctr=ypos;ctr <= 150; ctr+=10)
	 {
		 soundfx();
		 putimage(xpos,ctr,bmp,XOR_PUT);
		 delay(30);
		 putimage(xpos,ctr,bmp,XOR_PUT);
		 callmenu();
	 }
	 soundpush();
	 fillbuffer();
 }

 void generatejob()
 {
	 if (random(1000) <= 5)
	 {
		 pendingjobs++;
		 job_in_buff();
	 }
 }

 void showcputime(int timer)
 {
	 char time[5];
	 itoa(timer/10,time,10);
	 setcolor(LIGHTCYAN);
	 setfillstyle(1,7);
	 bar(580,230,600,240);
	 outtextxy(580,230,time);
 }

 void shownumjobs()
 {
	 char numjobs[5];
	 itoa(pendingjobs,numjobs,10);
	 setcolor(LIGHTCYAN);
	 setfillstyle(1,7);
	 bar(580,325,620,335);
	 outtextxy(580,325,numjobs);
 }

 void showwaittime()
 {
 }

 void processing()
 {
	 int ctr,timer;
	 char done[4];
	 setcolor(10);
	 setlinestyle(0,0,3);
	 rectangle(490,197,615,209);
	 setlinestyle(0,0,1);
	 setcolor(4);
	 setfillstyle(1,7);
	 bar(568,220,633,228);
	 outtextxy(570,220,"BUSY... ");
	 for (ctr=1,timer=151;ctr <=152;ctr++,timer--)
	 {
		 showcputime(timer);
		 generatejob();
		 putimage(295,180,bmp2,XOR_PUT);
		 if (sounds !=0) sound(500);
		 delay(5);
		 putimage(295,180,bmp2,XOR_PUT);
		 delay(5);
		 nosound();
		 callmenu();
	 }
	 jobsdone++;
	 itoa(jobsdone,done,10);
	 setcolor(LIGHTCYAN);
	 setfillstyle(1,7);
	 bar(605,240,630,248);
	 outtextxy(605,240,done);
	 setcolor(9);
	 setlinestyle(0,0,3);
	 rectangle(490,197,615,209);
	 setlinestyle(0,0,1);
	 setcolor(0);
	 setfillstyle(1,7);
	 bar(568,220,633,228);
	 outtextxy(580,220,"NONE");
	 if (random(100) <=25)  job_out_cpu();
	}

 void drawcircuit()
 {
	 setcolor(9);
	 setlinestyle(0,0,3);
	 rectangle(280,160,390,170);
	 rectangle(280,172,390,272);
	 rectangle(100,160,200,380);
	 setcolor(LIGHTMAGENTA);
	 settextstyle(0,0,2);
	 outtextxy(400,180,"C");
	 outtextxy(400,210,"P");
	 outtextxy(400,240,"U");
	 outtextxy(210,190,"B");
	 outtextxy(210,220,"U");
	 outtextxy(210,250,"F");
	 outtextxy(210,280,"F");
	 outtextxy(210,310,"E");
	 outtextxy(210,340,"R");
	 settextstyle(0,0,1 );
 }

 void screenout()
 {
	 window1(5,5,635,50,15,2,0);
	 centertext(5,635,15,1,2,"CPU JOB SIMULATION");
	 centertext(5,635,38,0,1,"Project in CEST 149 Submitted by Armando M. So III ");
	 window1(5,60,460,470,1,0,0);
	 window1(470,60,635,160,15,GREEN,0);
	 window1(470,170,635,470,15,7,0);
	 centertext(470,635,370,0,1,"* OPTIONS *");
	 centertext(470,635,180,0,1,"STATUS BOX");
	 centertext(470,635,200,1,1,"*** CPU ***");
	 centertext(470,635,260,1,1,"*** JOB ***");
	 centertext(470,635,305,1, 1,"*** BUFFER ***");
	 setcolor(9);
	 setlinestyle(0,0,3);
	 rectangle(490,197,615,209);
	 rectangle(490,257,615,269);
	 rectangle(490,302,615,314);
	 setlinestyle(0,0,1);

	 setcolor(0);
	 outtextxy(480,220,"Job in CPU:  NONE");
	 outtextxy(480,230,"Time left :");
	 outtextxy(480,240,"Jobs processed:");

	 outtextxy(480,275,"Time reg'd : ");
	 outtextxy(480,285,"Time left :");
	 outtextxy(480,325,"Queued Jobs:");
	 outtextxy(480,335,"Average");
	 outtextxy(480,345,"Wait Time:");
	 setcolor(15);
	 outtextxy(478,385,"F1 - Start/Resume");
	 outtextxy(478,395,"F2 - Enable sounds");
	 outtextxy(478,405,"F3 - Disable sounds");
	 outtextxy(478,415,"ESC- Exit/Sibat");
	 drawcircuit();
 }

 void initializeimages()
 {
	 size=imagesize(100,100,180,180);
	 bmp2=malloc(sizeof(size));
	 setfillstyle(1,10);
	 bar(200,200,210,210);
	 getimage(200,200,210,210,&bmp);
	 putimage(200,200,&bmp,XOR_PUT);
	 setfillstyle(1,4);
	 bar(100,100,180,180);
	 getimage(100,100,180,180,bmp2);
	 putimage(180,180,bmp2,XOR_PUT);
	 cleardevice();
 }

 void callmenu()
 {
	 int row,col,val,state=0;
	 char key;
	 regs.r_ax=5;
	 regs.r_bx=0;
	 intr(0x33,&regs);
	 if (regs.r_bx !=0 || kbhit())
	 {
		 hidemouse();
		 button(502,70,602,90,1,7,"RESUME",0);
		 button(502,95,602,115,1,7,"SUSPEND",0);
		 button(502,120,602,140,1,7,"EXIT",0);
		 showmouse();
		 while (1)
		 {
			 while (1)
			 {
				 regs.r_ax=5;
				 regs.r_bx=0;
				 intr(0x33,&regs);
				 regs.r_ax=3;
				 intr(0x33,&regs);
				 row=regs.r_dx;
				 col=regs.r_cx;
				 if (kbhit()) break;
				 if (regs.r_bx==1 || regs.r_bx==2) break;
			 }
			 if (kbhit())
			 {
				 key=getch();
				 if (key==27)
				 {
					 closegraph();
					 free(bmp2);
					 exit(0);
				 }
				 else if (key==59) break;
				 else if (key==60)
				 {
					 sounds=1;
					 break;
				 }
				 else if (key==61)
				 {
					 sounds=0;
					 break;
				 }
				 else continue;
			 }
			 state=0;
			 if (regs.r_bx==1)
			 {
				 if ((row >=70 && row <=90) && (col >=502 && col <= 602))
				 {
					 push(502,70,602,90,1,7,"RESUME",&val,state);
					 if (val==1) break;
					 else continue;
				 }else
				 if ((row >=95 && row <=115) && (col >=502 && col <= 602))
				 {
					 push(502,95,602,115,1,7,"SUSPEND",&val,state);
					 if (val==1) continue;
					 else continue;
				 }else
				 if ((row >=120 && row <=140) && (col >=502 && col <= 602))
				 {
					 push(502,120,602,140,1,7,"EXIT",&val,state);
					 if (val==1)
					 {
						 closegraph();
						 free(bmp2);
						 exit(0);
					 }
					 else continue;
				 }
				 else continue;
			 }
		 }
	 }
 }

 void menubox(char text[])
 {
	 int row,col,val,state=0;
	 char key;
	 button(502,70,602,90,1,7,text,0);
	 button(502,95,602,115,1,7,"EXIT",0);
	 while (1)
	 {
		 while (1)
		 {
			 regs.r_ax=5;
			 regs.r_bx=0;
			 intr(0x33,&regs);
			 regs.r_ax=3;
			 intr(0x33,&regs);
			 row=regs.r_dx;
			 col=regs.r_cx;
			 if (kbhit()) break;
			 if (regs.r_bx==1 || regs.r_bx==2) break;
		 }
		 if (kbhit())
		 {
			 key=getch();
			 if (key==27)
			 {
				 closegraph();
				 free(bmp2);
				 exit(0);
			 }
				 else if (key==60)
				 {
					 sounds=1;
					 break;
				 }
				 else if (key==61)
				 {
					 sounds=0;
					 break;
				 }
			 else if (key==59) break;
			 else continue;
		 }
		 state=0;
		 if (regs.r_bx==1)
		 {
			 if ((row >=70 && row <=90) && (col >=502 && col <= 602))
			 {
				 push(502,70,602,90,1,7,text,&val,state);
				 if (val==1) break;
				 else continue;
			 }else
			 if ((row >=95 && row <=115) && (col >=502 && col <= 602))
			 {
				 push(502,95,602,115,1,7,"EXIT",&val,state);
				 if (val==1)
				 {
					 closegraph();
					 free(bmp2);
					 exit(0);
				 }
				 else continue;
			 }
			 else continue;
		 }
	 }
 }

 void main()
 {
	 initgraphics();
	 initializeimages();
	 screenout();
	 initmouse();
	 showmouse();
	 mousearea(475,65,630,155);
	 rowcol(500,80);
	 menubox("START");
	 do
	 {
			shownumjobs();
			generatejob();
			job_in_cpu();
	 } while (!kbhit());
	 free(bmp2);
	 closegraph();
 }

