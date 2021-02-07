#define ON  1
#define OFF 0

#include <alloc.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <mem.h>

void redo_palette(char on_off);
void make_cos_table();
void do_plasma_c();
void do_plasma_asm();
void do_plasma_asm2();
void Copy_to_Video();
void set_mode(unsigned int mode);

unsigned char far *video_mem=(char far*)(0xa0000000);
unsigned char far *double_buffer=(char far*)farmalloc(64000);
unsigned char old_pal[256*3];
unsigned char cos_table[256];

unsigned char p1=0, p2=0, p3=0, p4=0;
unsigned long x_count=320, y_count=200;

char p1i=1, p2i=2, p3i=1, p4i=2;

//////////////////////////////////////////////////////////////////////////
void main()
  {
  clock_t start_time, end_time;
  long frame_count=0;
  int a, done=0, mode=1, key=0;

  set_mode(0x13);
  redo_palette(ON);
  make_cos_table();

  start_time=clock();

  while(!done)
    {
    if(kbhit())
      {
      key=getch();
      if(key==27)
        done=1;
      if(key==32)//space
        mode=-mode;
      }
    if(mode==1)
      do_plasma_c();
    else
      do_plasma_asm();
    Copy_to_Video();
    frame_count++;
    }
  end_time=clock();

  _fmemset(video_mem,0,64000);
  redo_palette(OFF);
  set_mode(0x03);

  printf("Average Frames per Second = %f\n\n", (float)(frame_count)/((float)(end_time-start_time)/18.2));

  }
//////////////////////////////////////////////////////////////////////////
void redo_palette(char on_off)
  {
  int a=0;
  int index=0;
  if(on_off==ON)//make new palette
    {
    //save old_palette();
    for(a=0; a<256; a++)
      {
      outportb(0x3c6, 0xff);
      outportb(0x3c7, a);
      old_pal[index++]=inportb(0x3c9);
      old_pal[index++]=inportb(0x3c9);
      old_pal[index++]=inportb(0x3c9);
      }
    //make new palette
    outportb(0x3c6, 0xff);
    for(a=0; a<32; a++)
      {
      outportb(0x3c8, a);
      outportb(0x3c9, a*2);
      outportb(0x3c9, 0);
      outportb(0x3c9, 0);

      outportb(0x3c8, a+32);
      outportb(0x3c9, 63-a*2);
      outportb(0x3c9, a*2);
      outportb(0x3c9, 0);

      outportb(0x3c8, a+64);
      outportb(0x3c9, 0);
      outportb(0x3c9, 63-a*2);
      outportb(0x3c9, a*2);

      outportb(0x3c8, a+64+32);
      outportb(0x3c9, a*2);
      outportb(0x3c9, a*2);
      outportb(0x3c9, 63);

      outportb(0x3c8, a+128);
      outportb(0x3c9, 63-a*2);
      outportb(0x3c9, 63);
      outportb(0x3c9, 63-a*2);

      outportb(0x3c8, a+32+128);
      outportb(0x3c9, a*2);
      outportb(0x3c9, 63-a*2);
      outportb(0x3c9, 0);

      outportb(0x3c8, a+64+128);
      outportb(0x3c9, 63-a*2);
      outportb(0x3c9, 0);
      outportb(0x3c9, a*2);

      outportb(0x3c8, a+64+32+128);
      outportb(0x3c9, 0);
      outportb(0x3c9, 0);
      outportb(0x3c9, 63-a*2);
      }
    }
  else//restore old one
    {
    //restore old_palette();
    for(a=0; a<256; a++)
      {
      outportb(0x3c6, 0xff);
      outportb(0x3c8, a);
      outportb(0x3c9, old_pal[index++]);
      outportb(0x3c9, old_pal[index++]);
      outportb(0x3c9, old_pal[index++]);
      }
    }
  }
//////////////////////////////////////////////////////////////////////////
void make_cos_table()
  {
  int a;
  for(a=0; a<256; a++)
    cos_table[a]=(unsigned char)((cos((double)a*360/255*3.141592/180)*32)+32);
  }
//////////////////////////////////////////////////////////////////////////
void set_mode(unsigned int mode)
  {
  asm  {
     mov ax, mode
     int 10h
  }
  }
//////////////////////////////////////////////////////////////////////////
void Copy_to_Video()
  {
  _fmemcpy(video_mem, double_buffer, 64000);
  }
//////////////////////////////////////////////////////////////////////////
void do_plasma_c()
  {
  int x=0, y=0;
  unsigned int where=0;
  unsigned char vptr1, vptr2, hptr1, hptr2;

  vptr1=p3;
  vptr2=p4;
  do
    {
    vptr1+=p2i;
    vptr2+=p1i;
    hptr1=p2;
    hptr2=p1;
    x=0;
    do
      {
      hptr1+=p4i;
      hptr2+=p3i;
      double_buffer[where++]=cos_table[hptr1]+cos_table[hptr2]+
                         cos_table[vptr1]+cos_table[vptr2];
      }while(x++<319);
    }while(y++<199);
  p1-=1;
  p2+=2;
  p3-=3;
  p4+=4;
  }
//////////////////////////////////////////////////////////////////////////
void do_plasma_asm()  {
  asm {
    push ds
    les  di, double_buffer
    mov  ch, p3
    mov  cl, p4
    xor  bx, bx
    xor  ax, ax
    mov  ah, 200
  }
  next_y:
  asm {
    add ch, p1i
    add cl, p2i
    mov dh, p1
    mov dl, p2
    mov si, 320
  }
  next_x:
  asm {
    add  dh, p3i
    add  dl, p4i
    mov  bl, dh
    mov  al, cos_table[bx]
    mov  bl, dl
    add  al, cos_table[bx]
    mov  bl, ch
    add  al, cos_table[bx]
    mov  bl, cl
    add  al, cos_table[bx]
    mov  es:[di], al
    inc  di
    dec  si
    jnz  next_x
    dec ah
    jnz next_y
    pop ds
  }
  p1-=1;
  p2+=2;
  p3-=3;
  p4+=4;
}




