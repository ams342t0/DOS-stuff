#ifndef  __IO_H
  #include <io.h>
#endif
#ifndef  __BIOS_H
  #include <bios.h>
#endif
#ifndef  __STDLIB_H
  #include <stdlib.h>
#endif
#ifndef  __ALLOC_H
  #include <alloc.h>
#endif
#ifndef  __STDIO_H
  #include <stdio.h>
#endif
#ifndef  __CONIO_H
  #include <conio.h>
#endif
#ifndef  __MALLOC_H
  #include <stdio.h>
#endif
#ifndef  __STRING_H
  #include <string.h>
#endif
#ifndef  __DOS_H
  #include <dos.h>
#endif
#ifndef  __FCNTL_H
  #include <fcntl.h>
#endif
#ifndef  __CTYPE_H
  #include <CTYPE.h>
#endif

#define  ULONG   unsigned long
#define  CHAR    signed char
#define  BOOL    char

// Bit Assignments in ControlByte variable
#define  _2PLAYERS     0
#define  SNDCARD       1
#define  SNDABLE       2
#define  CHANGEBK      3

#define  TABWID  10
#define  TABLEN  24
#define  MAXROW  TABLEN-1
#define  MAXCOL  TABWID-1
#define  BLKWID  9
#define  BLKHT   8
#define  SUPERBONUS 10000
#define  STARTROW   0
#define  STARTCOL   4
#define  SC_SINGLE  75
#define  SC_DOUBLE  150
#define  SC_TRIPLE  300
#define  SC_TETRIS  750
#define  SC_DROP    45
#define  MUL_QUAD   1200
#define  MUL_TRIP   850
#define  MUL_COUP   300
#define  MUL_SING   150
#define  DEFHIGH    50000
#define  MAXLEVEL   12
#define  MAXLINES   30
#define  STARTSPEED 51
#define  DEC_CTR    4
#define  PLATE_WID  90
#define  PLATE_HT   160
#define  PLATE_AREA PLATE_WID*PLATE_HT
#define  MASKBYTE   0
#define  BITMAPS    23
#define  SCOREFILE  "DATA\\HISCORES.DAT"
#define  FILELIST   "DATA\\FILES.LST"

/***************************
   DEFAULT COORDINATES
****************************/
//  BOARD
#define  P1BOARDX   8
#define  P1BOARDY   30
//  NAME
#define  P1NAMEX    P1BOARDX
#define  P1NAMEY    10
//NEXTPIECE
#define  P1PIECEX   108
#define  P1PIECEY   P1BOARDY
//  SCORE
#define  P1SCOREX   P1BOARDX+65
#define  P1SCOREY   P1NAMEY
//  LINES & LEVEL
#define  P1LEVELX   P1PIECEX+2
#define  P1LEVELY   P1PIECEY+57

//  BOARD
#define  P2BOARDX   222
#define  P2BOARDY   P1BOARDY
//  NAME
#define  P2NAMEX    171
#define  P2NAMEY    P1NAMEY
//  NEXTPIECE
#define  P2PIECEX   180
#define  P2PIECEY   P1PIECEY
//  SCORE
#define  P2SCOREX   P2NAMEX+65
#define  P2SCOREY   P1SCOREY
//  LINES & LEVEL
#define  P2LEVELX   180
#define  P2LEVELY   P1LEVELY

unsigned ChkByte=1;
CHAR row,col,
     BackgrndNum=0,
     MaxBitmaps=BITMAPS,
     NumBricks=7;
ULONG start,
      WaitTime,
      GameHighScore;
int xmshandle,
    ti=0,tbuf[7];
char ControlByte=3,
	 *gamepal,
     far *AnimData,
     far *GameOver;
FILE *fp;

/**********************************
   KEYBOARD HANDLER AND CONSTANTS
***********************************/
#define         kESC                0x01
#define         _1                  0x02
#define         _2                  0x03
#define         _3                  0x04
#define         _4                  0x05
#define         _5                  0x06
#define         _6                  0x07
#define         _7                  0x08
#define         _8                  0x09
#define         _9                  0x0A
#define         _0                  0x0B
#define         MINUS               0x0C
#define         EQUAL               0x0D
#define         BKSPACE             0x0E
#define         TAB                 0x0F
#define         Q                   0x10
#define         W                   0x11
#define         E                   0x12
#define         R                   0x13
#define         T                   0x14
#define         Y                   0x15
#define         U                   0x16
#define         I                   0x17
#define         O                   0x18
#define         P                   0x19
#define         LBRACKET            0x1A
#define         RBRACKET            0x1B
#define         ENTER               0x1C
#define         LCTRL               0x1D
#define         A                   0x1E
#define         S                   0x1F
#define         D                   0x20
#define         F                   0x21
#define         G                   0x22
#define         H                   0x23
#define         J                   0x24
#define         K                   0x25
#define         L                   0x26
#define         SEMICOLON           0x27
#define         TICK                0x28
#define         APOSTROPHE          0x29
#define         LEFTSHIFT           0x2A
#define         BACKSLASH           0x2B
#define         Z                   0x2C
#define         X                   0x2D
#define         C                   0x2E
#define         V                   0x2F
#define         B                   0x30
#define         N                   0x31
#define         M                   0x32
#define         COMMA               0x33
#define         PERIOD              0x34
#define         SLASH               0x35
#define         RIGHTSHIFT          0x36
#define         KEYPADSTAR          0x37
#define         LEFTALT             0x38
#define         SPACE               0x39
#define         CAPSLOCK            0x3A
#define         F1                  0x3B
#define         F2                  0x3C
#define         F3                  0x3D
#define         F4                  0x3E
#define         F5                  0x3F
#define         F6                  0x40
#define         F7                  0x41
#define         F8                  0x42
#define         F9                  0x43
#define         F10                 0x44
#define         NUMLOCK             0x45
#define         SCROLLLOCK          0x46
#define         KP7                 0x47
#define         KP8                 0x48
#define         KP9                 0x49
#define         KPMINUS             0x4A
#define         KP4                 0x4B
#define         KP5                 0x4C
#define         KP6                 0x4D
#define         KPPLUS              0x4E
#define         KP1                 0x4F
#define         KP2                 0x50
#define         KP3                 0x51
#define         KP0                 0x52
#define         KPDEL               0x53
#define         SYSREQ              0x54
#define         F11                 0x57
#define         F12                 0x58
#define         RIGHTCTRL           0x9D
#define         RIGHTALT            0xB8
#define         CTRLPRTSC           0xB7
#define         SHIFTPRTSC          0xB7
#define         KPSLASH             0xB5
#define         INS                 0xD2
#define         DEL                 0xD3
#define         HOME                0xC7
#define         END                 0xCF
#define         PGUP                0xC9
#define         PGDN                0xD1
#define         LEFT                0x4B
#define         RIGHT               0x4D
#define         UP                  0x48
#define         DOWN                0x50
#define         KPENTER             0x9C

int Defaults[2][7]={{S,F,D,SPACE,K,L,P},
		    {LEFT,RIGHT,DOWN,KP0,KP1,KP3,KPMINUS}};
char keys[256],e0flag=0;
void interrupt (*OldInt9)(...);

inline void ResetKeys() {
  memset(keys,0,256);
}


void interrupt NewInt9(...) {
  asm {
     cli
     in      al,60h
     mov     bl,al
     and     bl,01111111b
     add     bl,[e0flag]
     xor     bh,bh
     and     al,10000000b
     xor     al,10000000b

     rol     al,1
     mov     [keys + bx],al
     mov     [e0flag],0

     mov     al,20h
     out     20h,al
     sti
  }
}

void InitKB(void){
  char far *bios_key_state;

  bios_key_state=(char far*)MK_FP(0x040, 0x017);
  *bios_key_state&=(~(32 | 64));
  OldInt9=getvect(9);
  setvect(9,NewInt9);
}

void DeinitKB(void) {
  setvect(9,OldInt9);
}


/***************
   SOUNDCARD FUNCTIONS
****************/

#define ADDR 0x388  // Index port of sound card
#define DATA 0x389  // Data  port of sound card

void inreg(char regnum,char value) {
  asm {
    mov al, regnum
    mov dx, ADDR
    out dx, al
    mov cx, 6
    mov dx, 388h
  }
  wait1:
  asm {
    in  al, dx
    loop wait1
  }
//  delay(1);
  asm {
    mov al, value
    mov dx, DATA
    out dx, al
    mov cx, 35
    mov dx, 388h
  }
  wait2:
  asm {
    in al, dx
    loop wait2
  }
//  delay(1);
}

int detectboard() {
  char status,status2;

  inreg(4,0x60);
  inreg(4,0x80);
  status = inportb(0x388);
  inreg(2,0xff);
  inreg(4,0x21);
  asm mov dx, 388h
  asm mov cx, 146
  delay1:
  asm {
      in al, dx
      loop delay1
  }
  status2= inportb(0x388);
  inreg(4,0x60);
  inreg(4,0x80);

  if (!(status & 0x00E0) && (status2 & 0x00e0)==0xc0) return 1;
  else return 0;
}

void resetregs(void) {
  int ct=245;
  while (ct--) inreg(ct,0);
}

void initchannels() {
  int ct,offset;
  resetregs();

  offset = 0;
  inreg(0xbd,0xff);
  for (ct = 0; ct < 9; ct++) {
    inreg(0xa0+ct,0);
    inreg(0xb0+ct,0);
    inreg(0x20+offset,0x41);
    inreg(0x40+offset,0x0);
    inreg(0x60+offset,0xf0);
    inreg(0x80+offset,0xff);
    // Operator 2
    inreg(0x20+offset+3,0x41);
    inreg(0x40+offset+3,0);
    inreg(0x60+offset+3,0xf0);
    inreg(0x80+offset+3,0xff);
    offset++;
    if (offset == 3 || offset == 11) offset+=5;
  }
}

void snd(int channel,int freq,char oct) {
  inreg(0xa0+channel,freq);
  inreg(0xb0+channel,0x20|((oct << 2)+(freq>>8)));
}

void nosnd(int channel) {
  inreg(0xb0+channel,0);
}

/**************
   FUNCTIONS FOR MANIPULATING BITS ON BYTES AND WORDS
****************/

inline char checkbit_8(char byte,char col) {
  char mask=1;

  mask <<=(3-col);
  if (byte & mask) return 1;
  else return 0;
}

inline void biton_8(char  *val,char col) {
  char mask=1;

  mask <<=(3-col);
  *val |= mask;
}

inline void bitoff_8(char  *val,char col) {
  char mask=1;

  mask <<=(3-col);
  *val &= ~mask;
}

inline char checkbit_16(unsigned byte,char col) {
  int mask=1;

  mask <<=(15-col);
  if (byte & mask) return 1;
  else return 0;
}

/*
void turnbitoff_16(unsigned  *val,char col) {
  int mask=1;

  mask <<=(15-col);
  *val &= ~mask;
}
*/


inline void turnbiton_16(unsigned  *val,char col) {
  int mask=1;

  mask <<=(15-col);
  *val ^= mask;
}

/**********
   STRUCTURES DEFINING THE BRICKS
**********/

#include "blocks.h"

typedef struct BrickStruct{
  CHAR maxpos,position;
  char map[4],*block;
};

BrickStruct SquareBrick(CHAR position) {
  BrickStruct retval;

  memset((char*)retval.map,0,4);
  position ^= position;
//  retval.block=blueblk;
  retval.block=block1;
  retval.map[0]=0;    //  ....
  retval.map[1]=0;    //  ....
  retval.map[2]=12;   //  XX..
  retval.map[3]=12;   //  XX..
  retval.maxpos = 1;
  return retval;
}

BrickStruct Stick(CHAR position) {
  BrickStruct retval;
  memset((char*)retval.map,0,4);
//  retval.block=redblk;
  retval.block=block2;
  switch (position) {
    case 0 : retval.map[0]=8;       // X...
	     retval.map[1]=8;       // X...
	     retval.map[2]=8;       // X...
	     retval.map[3]=8;
	     break;
    case 1 : retval.map[0]=0;       // ....
	     retval.map[1]=15;      // XXXX
	     retval.map[2]=0;       // ....
	     retval.map[3]=0;       // ....
	     break;
  }
  retval.maxpos = 2;
  return retval;
}

BrickStruct Left_L(CHAR position) {
  BrickStruct retval;
  memset((char*)retval.map,0,4);
//  retval.block=yellowblk;
  retval.block=block3;
  switch (position) {
    case 0 : retval.map[0]=0;     // ....
	     retval.map[1]=4;     // .X..
	     retval.map[2]=4;     // .X..
	     retval.map[3]=12;    // XX..
	     break;
    case 1 : retval.map[0]=0;     // ....
	     retval.map[1]=8;     // X...
	     retval.map[2]=14;    // XXX.
	     retval.map[3]=0;     // ....
	     break;
    case 2 : retval.map[0]=0;    // ....
	     retval.map[1]=12;   // XX..
	     retval.map[2]=8;    // X...
	     retval.map[3]=8;    // X...
	     break;
    case 3 : retval.map[0]=0;   // ....
	     retval.map[1]=0;   // ....
	     retval.map[2]=14;  // XXX.
	     retval.map[3]=2;   // ..X.
  }
  retval.maxpos = 4;
  return retval;
}

BrickStruct Right_L(CHAR position) {
  BrickStruct retval;
  memset((char*)retval.map,0,4);
//  retval.block=purpleblk;
  retval.block=block4;
  switch (position) {
    case 0 : retval.map[0]=0;     // ....
	     retval.map[1]=8;     // X...
	     retval.map[2]=8;     // X...
	     retval.map[3]=12;    // XX..
	     break;
    case 1 : retval.map[0]=0;      // ....
	     retval.map[1]=0;      // ....
	     retval.map[2]=14;     // XXX.
	     retval.map[3]=8;      // X...
	     break;
    case 2 : retval.map[0]=0;    //  ....
	     retval.map[1]=12;   //  XX..
	     retval.map[2]=4;    //  .X..
	     retval.map[3]=4;    //  .X..
	     break;
    case 3 : retval.map[0]=0;   // ..X.
	     retval.map[1]=2;   // XXX.
	     retval.map[2]=14;  // ....
	     retval.map[3]=0;
  }
  retval.maxpos = 4;
  return retval;
}

BrickStruct Left_Sickle(CHAR position) {
  BrickStruct retval;
  memset((char*)retval.map,0,4);
//  retval.block=greenblk;
  retval.block=block5;
  switch (position) {
    case 0 : retval.map[0]=0;      // ....
	     retval.map[1]=0;      // ....
	     retval.map[2]=12;     // XX..
	     retval.map[3]=6;      // .XX.
	     break;
    case 1 : retval.map[0]=0;      // .X..
	     retval.map[1]=4;      // XX..
	     retval.map[2]=12;     // X...
	     retval.map[3]=8;
	     break;
  }
  retval.maxpos = 2;
  return retval;
}

BrickStruct Right_Sickle(CHAR position) {
  BrickStruct retval;
  memset((char*)retval.map,0,4);
//  retval.block=orngblk;
  retval.block=block6;
  switch (position) {
    case 0 : retval.map[0]=0;      // ....
	     retval.map[1]=0;      // ....
	     retval.map[2]=6;      // .XX.
	     retval.map[3]=12;     // XX..
	     break;
    case 1 : retval.map[0]=0;      // X...
	     retval.map[1]=8;      // XX..
	     retval.map[2]=12;     // .X..
	     retval.map[3]=4;
	     break;
  }
  retval.maxpos = 2;
  return retval;
}

BrickStruct Tee (CHAR position) {
  BrickStruct retval;
  memset((char*)retval.map,0,4);
//  retval.block=maroonblk;
  retval.block=block0;
  switch (position) {
    case 0 : retval.map[0]=0;      //  ....
	     retval.map[1]=0;      //  ....
	     retval.map[2]=14;     //  XXX.
	     retval.map[3]=4;      //  .X..
	     break;
    case 1 : retval.map[0]=0;      //  ....
	     retval.map[1]=4;      //  .X..
	     retval.map[2]=12;     //  XX..
	     retval.map[3]=4;      //  .X..
	     break;
    case 2 : retval.map[0]=0;      // ....
	     retval.map[1]=4;      // .X..
	     retval.map[2]=14;     // XXX.
	     retval.map[3]=0;      // ....
	     break;
    case 3 : retval.map[0]=0;      //  ....
	     retval.map[1]=8;      //  X...
	     retval.map[2]=12;     //  XX..
	     retval.map[3]=8;      //  X...
	     break;
   }
   retval.maxpos = 4;
   return retval;
}

BrickStruct Clamp (CHAR position) {
  BrickStruct retval;
  memset((char*)retval.map,0,4);
//  retval.block=redblk;
  retval.block=block0;
  switch (position) {
    case 0 : retval.map[0]=0;      //  ....
	     retval.map[1]=0;      //  ....
	     retval.map[2]=14;     //  XXX.
	     retval.map[3]=10;     //  X.X.
	     break;
    case 1 : retval.map[0]=0;      //  ....
	     retval.map[1]=12;     //  XX..
	     retval.map[2]=4;      //  .X..
	     retval.map[3]=12;     //  XX..
	     break;
    case 2 : retval.map[0]=0;      // ....
	     retval.map[1]=10;     // X.X.
	     retval.map[2]=14;     // XXX.
	     retval.map[3]=0;      // ....
	     break;
    case 3 : retval.map[0]=0;      //  ....
	     retval.map[1]=12;     //  XX..
	     retval.map[2]=8;      //  X...
	     retval.map[3]=12;     //  XX..
	     break;
   }
   retval.maxpos = 4;
   return retval;
}

BrickStruct SoBrick(CHAR position) {
  BrickStruct retval;
  memset((char*)retval.map,0,4);
//  retval.block=greenblk;
  retval.block=block3;
  switch (position) {
    case 0 : retval.map[0]=0;      //  ....
	     retval.map[1]=14;     //  XXX.
	     retval.map[2]=4;      //  .X..
	     retval.map[3]=4;      //  .X..
	     break;
    case 3 : retval.map[0]=0;     //  ....
	     retval.map[1]=8;     //  X...
	     retval.map[2]=14;    //  XXX.
	     retval.map[3]=8;     //  X...
	     break;
    case 2 : retval.map[0]=4;      //  .X..
	     retval.map[1]=4;      //  .X..
	     retval.map[2]=14;     //  XXX.
	     retval.map[3]=0;      //  ....
	     break;
    case 1 : retval.map[0]=0;     //  ....
	     retval.map[1]=2;     //  ..X.
	     retval.map[2]=14;    //  XXX.
	     retval.map[3]=2;     //  ..X.
	     break;
   }
   retval.maxpos = 4;
   return retval;
}


BrickStruct Why(CHAR position) {
  BrickStruct retval;
  memset((char*)retval.map,0,4);
//  retval.block=purpleblk;
  retval.block=block2;
  switch (position) {
    case 0 : retval.map[0]=0;      //  ....
	     retval.map[1]=10;     //  X.X.
	     retval.map[2]=14;     //  XXX.
	     retval.map[3]=4;      //  .X..
	     break;
    case 1 : retval.map[0]=0;      //  ....
	     retval.map[1]=6;      //  .XX.
	     retval.map[2]=12;     //  XX..
	     retval.map[3]=6;      //  .XX.
	     break;
    case 2 : retval.map[0]=0;      //  ....
	     retval.map[1]=4;      //  .X..
	     retval.map[2]=14;     //  XXX.
	     retval.map[3]=10;     //  X.X.
	     break;
    case 3 : retval.map[0]=0;     //  ....
	     retval.map[1]=12;    //  XX..
	     retval.map[2]=6;     //  .XX.
	     retval.map[3]=12;    //  XX..
	     break;
   }
   retval.maxpos = 4;
   return retval;
}

BrickStruct El(CHAR position) {
  BrickStruct retval;
  memset((char*)retval.map,0,4);
//  retval.block=yellowblk;
  retval.block=block1;
  switch (position) {
    case 0 : retval.map[0]=0;      //  ....
	     retval.map[1]=0;     //  ....
	     retval.map[2]=12;     //  XX..
	     retval.map[3]=8;      //  X...
	     break;
    case 1 : retval.map[0]=0;      //  ....
	     retval.map[1]=0;      //  ....
	     retval.map[2]=12;     //  XX..
	     retval.map[3]=4;      //  .X..
	     break;
    case 2 : retval.map[0]=0;      //  ....
	     retval.map[1]=0;      //  ....
	     retval.map[2]=4;      //  .X..
	     retval.map[3]=12;     //  XX..
	     break;
    case 3 : retval.map[0]=0;     //  ....
	     retval.map[1]=0;    //  ....
	     retval.map[2]=8;     //  X...
	     retval.map[3]=12;    //  XX..
	     break;
   }
   retval.maxpos = 4;
   return retval;
}

BrickStruct Cross(CHAR position) {
  BrickStruct retval;
  memset((char*)retval.map,0,4);
  retval.block=block6;
  retval.map[0]=0;     //  ....
  retval.map[1]=4;     //  .X..
  retval.map[2]=14;    //  XXX.
  retval.map[3]=4;     //  .X..
  retval.maxpos=1;
  return retval;
}


/***************
   TIME HANDLER AND
   CLASSES FOR REAL-TIME OPERATIONS
****************/
#define ULONG unsigned long
#define TICKS (*(volatile unsigned long far *)(0x0040006CL))


unsigned Freq(char num) {
  switch (num) {
     case 0 : return 256;
     case 1 : return 288;
     case 2 : return 320;
     case 3 : return 340;
     case 4 : return 384;
     case 5 : return 427;
     case 6 : return 480;
     case 7 : return 512;
     case 8 : return 576;
     case 9: return 640;
     case 10: return 683;
     case 11: return 768;
     case 12: return 853;
     case 13: return 910;
     case 14: return 1024;
     case 15: return 1138;
     case 16: return 1213;
     case 17: return 1365;
     case 18: return 1535;
     case 19: return 1706;
     case 20: return 1820;
     case 21: return 2048;
     case 22: return 2184;
     case 23: return 2457;
     case 24: return 2730;
     case 25: return 2912;
     case 26: return 70;
     case 27: return 80;
     case 28: return 90;
  }
}

char NEWLEVEL[6]={7,9,7,9,7,9},
     OK[8]={8,25,8,10,8,10,11,30},
     PERFECT[16]={4,10,5,10,6,11,7,26,4,15,7,42,7,42,11,54},
     NEW[24]={16,30,16,30,16,30,17,30,16,30,17,30,18,4,18,13,18,13,18,12,18,13,18,40},
     sd1[10]={28,3,27,3,26,1,28,2,27,2},
     sd2[20]={0,2,1,3,2,4,1,2,2,3,1,4,2,2,4,3,0,4,3,2},
     sd3[20]={3,1,4,2,5,3,6,2,5,1,6,2,5,3,6,2,7,1,8,2},
     sd4[20]={3,4,4,4,2,4,3,2,5,4,1,4,14,3,12,3,13,3,18,4},
     Quad[6]={0,15,1,13,8,46},
     NewHigh[18]={7,12,7,12,7,70,7,14,7,14,8,34,7,24,8,25,9,153};

volatile unsigned long fast_tick;
static void interrupt (far *oldtimer)(...);
unsigned long get_fast();
void deinit_timer(void);

class KeyClass_SP {
  char key_index;
  public:
    BOOL Ready;
    KeyClass_SP() {
     Ready = 1;
    }
    void Wait();
    void Enable(int);
};

void KeyClass_SP::Enable(int i) {
  Ready = 0;
  key_index=i;
}

void KeyClass_SP::Wait() {
  if (!keys[key_index]) Ready = 1;
}

class KeyStruct {
  public:
    unsigned long _delay;
    BOOL Ready;
    unsigned presscount;
    KeyStruct();
    void Delay(void);
    void Enable(int,int);
};

KeyStruct::KeyStruct() {
  Ready = 1;
  presscount = 0;
}

void KeyStruct::Enable(int w,int del) {
  presscount++;
  _delay = get_fast()+w;
  if (presscount==1) _delay+=del;
  Ready = 0;
}

void KeyStruct::Delay() {
  if (Ready) presscount=0;
  if (get_fast() <= _delay);
  else {
    Ready = 1;
  }
}

class Timed_Events {
  protected:
    char *raw_data;
    signed char mixing;
    char index,items,ctr;
    unsigned long local_t;

  public:
    Timed_Events();
    void Mix();
    void InitValues(char,char*);
    void EnableEvent();
}SO;

void Timed_Events::EnableEvent() {
  mixing = 1;
  index  = 0;
  ctr    = 0;
  local_t = get_fast();
}

void Timed_Events::InitValues(char numitems,char *s_data) {
  items = numitems;
  raw_data  = s_data;
}

Timed_Events::Timed_Events() {
  index  = 0;
  mixing = 0;
  ctr    = 0;
}

void Timed_Events::Mix() {
  if ( (get_fast() >= local_t)  && mixing) {
    if (!((ctr++) % 2)) {
      if (checkbit_8(ControlByte,SNDABLE))
        if (checkbit_8(ControlByte,SNDCARD)){
          snd(0,Freq(raw_data[index]),4);
          snd(1,Freq(raw_data[index++]),3);
      } else sound(Freq(raw_data[index++]));
      local_t+=raw_data[index++];
    }
    else {
      nosound();
      snd(0,0,4);
      snd(1,0,3);
      local_t+=3;
    }
    if (index > items) {
      mixing = 0;
      nosnd(0);
      nosnd(1);
      nosound();
    }
  }
}

class Sound_Events : public Timed_Events {
  public:
    void Play();
}LAP;

void Sound_Events::Play() {
  if ( (get_fast() >= local_t)  && mixing) {
    if (checkbit_8(ControlByte,SNDABLE))
      if (checkbit_8(ControlByte,SNDCARD)){
        snd(2,Freq(raw_data[index]),4);
        snd(3,Freq(raw_data[index++]),3);
      }
      else sound(Freq(raw_data[index++]));

    local_t+=raw_data[index++];

    if (index > items) {
      mixing = 0;
      nosnd(2);
      nosnd(3);
      nosound();
    }
  }
}

class Beeper : public Timed_Events {
  int freq;
  public:
   Beeper();
    void Beep();
}b;

Beeper::Beeper() {
  items = 50;
  freq = 650;
}

void Beeper::Beep() {
  if ( (get_fast() >= local_t)  && mixing) {
    if (checkbit_8(ControlByte,SNDABLE))
    if (checkbit_8(ControlByte,SNDCARD)) {
      snd(4,freq,5);
      snd(5,freq,3);
    }else  sound(freq);
    if (index % 2)
      if (checkbit_8(ControlByte,SNDCARD)) {
        nosnd(4);
        nosnd(5);
      }else nosound();
    local_t+=3;
    index++;
    if (index == items) {
      freq = 650;
      mixing = 0;
      index = 0;
    }
  }
}

/*
class Music : public Timed_Events {
  int * data;
  public :
    void Play();
    void init(int,int *);
}m;

void Music::init(int numitems,int * s_data) {
  items = numitems;
  data  = s_data;
}

void Music::Play() {
  if ( (get_fast() >= local_t)  && mixing) {
    snd(5,data[index],6);
    snd(6,data[index],4);
    snd(7,data[index],1);
    index++;
    local_t+=data[index++];
    if (index == items) {
      index = 0;
      nosnd(5);
      nosnd(6);
      nosnd(7);
    }
  }
} */

void delay_t(unsigned l) {
  unsigned long t=get_fast()+l;
  while (get_fast() < t);
}

void interrupt new_timer(...) {
   LAP.Play();
   SO.Mix();
   b.Beep();

   fast_tick ++;
   if (!(fast_tick & 7)) oldtimer();
   else asm {
       mov al, 20h
       out 20h, al
     }
}

void init_timer(void) {
   fast_tick=0l;
   oldtimer=getvect(8);
   asm cli
   asm {
     mov     bx,  8172  // 1/145 s
     mov     al,  00110110b
     out     43h, al
     mov     al,  bl
     out     40h, al
     mov     al,  bh
     out     40h, al
   }
   setvect(8, new_timer);
   asm sti
}

void deinit_timer(void) {
   asm cli
   asm {
      xor bx,  bx
      mov al,  00110110b
      out 43h, al
      mov al,  bl
      out 40h, al
      mov al,  bh
      out 40h, al
   }
   setvect(8, oldtimer);
   asm sti
}

ULONG get_fast(void) {
  return(fast_tick);
}


/***********************
   GRAPHIC FUNCTIONS
*************************/
#define INPUT_STATUS_0 0x3da
#define SCR_WID 320
#define SCR_HT  200

unsigned char far *screen,
              far *off_screen;

#define putpixel(x,y,colour) *(off_screen+y*SCR_WID+x)=colour

void initmode13h (void) {
  asm {
    mov ax, 0013h
    int 10h
  }
}

void closemode13h (void) {
  asm {
    mov ax, 0003h
    int 10h
  }
}

typedef struct{
   unsigned int bx;
   int height;
}font_init_t;

char far *font, far *t1;
int fheight;

void InitFont(int fontheight) {
   unsigned int segment, offset;
   struct REGPACK regs;
   font_init_t font_init[]=
      {
      {0x0200, 14},  // value of BX, and the height of the font
      {0x0300, 8},
      {0x0400, 8},
      {0x0500, 0},   // height 0, because wierd 9 wide font
      {0x0600, 16},
      {0x0700, 0}
      };

   fheight=font_init[fontheight].height;

   // call the bios int to get the address of the desired font
   regs.r_ax = 0x1130;
   regs.r_bx = font_init[fontheight].bx;
   intr(0x10, &regs);

   // make a far pinter to the font with info returned by the bios call
   offset = regs.r_bp;
   segment = regs.r_es;
   font = (char far*) MK_FP(segment, offset);
}

void outchar(int x,int y,char character,char color) {
   unsigned char mask;
   register int i, j;

   t1=font + (character*fheight);
   for ( i=y; i < y+fheight; i++ ) {
      mask=128; // 10000000 binary - this was main your bug - missing this
      if (i < 0 || i > 199) continue;
      for ( j=x; j < x+8; j++ ) {
          if (j <0 || j > 319) {
            mask >>=1;
            continue;
          }
          if (((*t1) & mask) ) putpixel(j,i,color);
          mask >>=1;  // move mask to next bit to the right
      }
      t1++;  // t1 points to the next scanline of the current character
   }
}

void outstring(int x,int y,char *string, char color) {
  int len=strlen(string);
  register int ct;
  for (ct=0;ct < len;ct++) {
     outchar(x,y,string[ct],color);
     x+=8;
  }
}

char initvideo(void) {
   screen=(char far*)MK_FP(0xa000, 0);
   off_screen=(char far*)farmalloc(64000u);
   if (!off_screen) return 0;
   initmode13h();
   InitFont(1);
   _fmemset(off_screen,0,64000u);
   _fmemset(screen,0,64000u);
   return 1;
}

void bar(int x, int y, int width, int height, int colour) {
   unsigned char far *p;

   p=off_screen + y*SCR_WID+x;
   while ( height--)	{
     _fmemset(p, colour, width);
     p+=SCR_WID;
   }
}

void show () {
  while ( inportb(INPUT_STATUS_0)&8);
  while ( !(inportb(INPUT_STATUS_0)&8));
  _fmemcpy(screen,off_screen,64000u);
}

void setpalette(unsigned char *p) {
   asm {   /* set palette, taking advantage of the auto-increment feature */
         xor   al, al	     // set al to zero, the first colour to set
         mov   dx, 03c8h	 // port for the vga palette control
         out   dx, al	     // tell the vga we want to change colours, starting with zero
         mov   cx, 768	   // number of bytes to send
         mov   si, p	     // si points to the colours to copy over
         mov   dx, 03c9h   // port to send the colours to
         rep   outsb     	 //  send the palette values over
   }
}

void setpalette_part(char *p, short offs,short start, short num) {
   p+=offs*3;
   num*=3;
   asm   {
	 mov   ax, start
         mov   dx, 03c8h
         out   dx, al
         mov   cx, num
         mov   si, p
         mov   dx, 03c9h
         rep   outsb
   }
}

void cyclepal(char *palette, short start, short num) {
   char t[3];
   short x1, x2, len;

   num--;
   len=num*3;

   x1=start*3;
   x2=len + x1;

   memcpy(t, palette+x1, 3);
   memmove(palette+x1, palette+x1+3, len);
   memcpy(palette+x2, t, 3);

   num++;
   setpalette_part(palette, start, start,num);
}

void fastput_raw(char far* buffer, short x, short y,
              short width, short height){
  asm   {
         push  ds
         push  di

	 mov   ax, y
         mov   bx, x
         xchg  ah, al
         add   bx, ax
         shr   ax, 2
         add   bx, ax

         les   di, off_screen // es:di points to start dest
         add   di, bx    // make es:di point to x,y in dest

         lds   si, buffer

         mov   dx, width
         shr   dx, 1
         mov   bx, 320
         sub   bx, width
         mov   ax, height
         }
l1:
   asm   {
	 mov   cx, dx
         rep   movsw
         add   di, bx
         dec   ax
         jnz   l1

         pop   di
         pop   ds
         }
}

void putimage(unsigned char far *spr, int x, int y) {
  unsigned char far *p;
  int width, height;

  _fmemcpy(&width, spr, 2);
  spr+=2;
  _fmemcpy(&height, spr, 2);
  spr+=2;
  fastput_raw(spr,x,y,width,height);
}

// get a patch of screen from off screen and save it as a sprite
void getimage(unsigned char far *spr, int x, int y, int width, int height) {
  unsigned char far *p;

  _fmemcpy(spr, &width, 2);
  spr+=2;
  _fmemcpy(spr, &height, 2);
  spr+=2;

  p=off_screen + y*SCR_WID + x;
  while ( height-- ){
    _fmemcpy(spr, p, width);
    spr+=width;
    p+=SCR_WID;
  }
}

void getbackboard(unsigned char far *spr, int x, int y) {
  unsigned char far *p;
  int height = PLATE_HT;

  p=off_screen + y*SCR_WID + x;
  while ( height-- ){
    _fmemcpy(spr, p, PLATE_WID);
    spr+=PLATE_WID;
    p+=SCR_WID;
  }
}

void draw_anim(char far *spr,int x,int y,int wid,int ht,char m){
  char far *p;
  int w,yc;

  yc=y;
  while (ht-- ) {
    w=wid;
    p = off_screen+yc*320+x;
    while (w--) {
       if (*spr != m) *p=*spr;
       p++;
       spr++;
    }
    yc++;
  }
}

char far *loadfile(FILE *f) {
  int handle;
  unsigned int size;
  char far *imagebuffer,
           *temp,
       far *retaddr;

  retaddr=NULL;
  if (f) {
     size=(unsigned int)filelength(fileno(f));
     imagebuffer=(char far*)farmalloc(size);
     if (!imagebuffer) return retaddr;
     retaddr=imagebuffer;
     temp=(char*)malloc(1024);
     while (size >= 1024) {
       fread(temp,1024,1,f);
       _fmemcpy(imagebuffer,temp,1024);
       imagebuffer+=1024;
       size-=1024;
     }
     if (size) {
       fread(temp,size,1,f);
       _fmemcpy(imagebuffer,temp,size);
     }
     free(temp);
  }
  return retaddr;
}

char *loadfilepal(char *fname,int size) {
   int handle;
   char *buffer;

   buffer=NULL;
   handle=open(fname,O_RDONLY | O_BINARY);
   if (handle > 0) {
      buffer=(char*)malloc(size);
      read(handle,buffer,size);
      close(handle);
   }
   return buffer;
}

void putimage_raw(unsigned char far *spr, int x, int y,int width,int height){
  unsigned char far *p;

  p=off_screen + y*SCR_WID + x;
  while ( height-- ) {
    _fmemcpy(p,spr,width);
    spr+=width;
    p+=SCR_WID;
  }
}

void restore() {
  asm {
    mov ax, 3h
    int 10h
  }
  farfree(off_screen);
}

void drawplate(char far *spr,int x, int y,int w,int h,char far* dest){
  char far *p;

  p= dest + y*PLATE_WID + x;
  while ( h-- ) {
    _fmemcpy(p,spr,w);
    p+=PLATE_WID;
    spr+=w;
  }
}
          /*
void drawblock(char far *spr,int x, int y,char far* dest){
  char far *p;
  int height=BLKHT;

  p= dest + y*PLATE_WID + x;
  while ( height-- ) {
    _fmemcpy(p,spr,BLKWID);
    p+=PLATE_WID;
    spr+=BLKWID;
  }
}           */

void copy_plate(char far *spr,int x,int y){
  char far *p;
  int height=PLATE_HT,w,yc;

  yc=y;
  while (height-- ) {
    w=PLATE_WID;
    p = off_screen+yc*320+x;
    while (w--) {
       if (*spr != MASKBYTE) *p=*spr;
       p++;
       spr++;
    }
    yc++;
  }
}

void eraseblk(char far *spr,int x, int y){
  char far *p;
  int height=BLKHT;

  p= spr + y*PLATE_WID + x;
  while ( height-- ) {
    _fmemset(p,MASKBYTE,BLKWID);
    p+=PLATE_WID;
  }
}

void eraserow(char far *spr,int y){
  char far *p;
  int height=BLKHT;

  p= spr + y*PLATE_WID;
  while ( height-- ) {
    _fmemset(p,MASKBYTE,BLKWID*10);
    p+=PLATE_WID;
  }
}

void getimg_plate(char far *spr,int y,char far*plate) {
  char far *p;

  p=plate + y*PLATE_WID;
  _fmemcpy(spr,p,720);
}

void putimg_plate(char far *spr,int y,char far*plate) {
  unsigned char far *p;

  p=plate + y*PLATE_WID;
  _fmemcpy(p,spr,720);
}


/*******************
  XMS DRIVER INTERFACE
*********************/

static int (far * XMSDriver)(void);
static int initFlag = 0;

int XMSinit(void) {
      asm {
	mov ax, 4300h
	int 2fh
      }
      if(!_AL) return 0;

      asm {
	 mov ax,4310h             /* Get XMS manager entry point */
	 int 2fh
      }
      XMSDriver = (int (_far *)(void)) MK_FP(_ES,_BX);
      return (initFlag = 1);
}

/*
int XMSversion(void) {
    if(!initFlag) return 0;
    asm mov ax, 0
    return XMSDriver();
}

*/

long  xcoreleft(void){
    if(!initFlag) return 0L;
    asm mov ax,800h
    return 1024L * (long) XMSDriver();
}

unsigned int xalloc(int size) {
   int rval = (int) (size / 1024L);
   if (size % 1024L) rval++;
   if(!initFlag) return 0;

   asm {
     mov dx, size
     mov ax, 0x0900
   }
   if (XMSDriver()) asm mov rval, dx
   else  rval = 0;
   return rval;
}

int xfree(unsigned int handle) {
   if(!initFlag) return 0;
   asm  {
     mov dx, handle
     mov ax, 0x0a00
   }
   return XMSDriver();
}

typedef struct {
   long         nbytes;     /* Number of bytes to move */
   unsigned int shandle;    /* Handle of source memory */
   long         soffset;    /* Offset of source in handle's memory area */
   unsigned int dhandle;    /* Handle of destination memory */
   long         doffset;    /* Offset of destination in memory */
} XMSRequestBlock;

static XMSRequestBlock bd;

static long XMSMove(long n) {
   long rval;
   unsigned int segm, offs;
   XMSRequestBlock far *fptr = (XMSRequestBlock far *) &bd;

   if(!initFlag) return 0L;

   bd.nbytes = n;

   offs = FP_OFF(fptr);
   segm = FP_SEG(fptr);

   asm  {
     push ds      /* Save DS */
     mov ds, segm
     mov si, offs
     mov ax, 0x0b00
   }
   rval = (XMSDriver() == 0) ? 0L : n;
   asm pop ds    /* Restore DS since we changed it to make this call */
   return rval;
}

/*
long xmemcpy(unsigned int desthandle, long destoff,
	     unsigned int srchandle, long srcoff, long n) {
    bd.shandle = srchandle;
    bd.soffset = srcoff;
    bd.dhandle = desthandle;
    bd.doffset = destoff;
    return XMSMove(n);
} */

long DOStoXMS(unsigned int desthandle, long destoff,
	     char far *src, long n) {
   bd.shandle = 0;
   bd.soffset = (long) ((char far *) src);
   bd.dhandle = desthandle;
   bd.doffset = destoff;
   return (XMSMove(n));
}

long XMStoDOS(char far *dest, unsigned int srchandle, long srcoff, long n) {
   bd.shandle = srchandle;
   bd.soffset = srcoff;
   bd.dhandle = 0;
   bd.doffset = (long) ((char far *) dest);
   return XMSMove(n);
}

/****************
   EXTRA FONTS
****************/
char fonts[]={4,10,17,31,17,17,17,  //A
	      30,17,17,30,17,17,30,
	      14,17,16,16,16,17,14,
	      30,17,17,17,17,17,30,
	      31,16,16,30,16,16,31,
	      31,16,16,30,16,16,16,
	      14,17,16,23,17,17,14,
	      17,17,17,31,17,17,17,
	      14,4,4,4,4,4,14,
	      1,1,1,1,17,17,14,
	      17,18,20,28,18,17,17,
	      16,16,16,16,16,16,31,
	      17,27,21,21,17,17,17,
	      17,25,21,19,17,17,17,
	      14,17,17,17,17,17,14,
	      30,17,17,30,16,16,16,
	      14,17,17,17,17,21,14,
	      30,17,17,30,17,17,17,
	      14,17,16,14,1,17,14,
	      31,4,4,4,4,4,4,
	      17,17,17,17,17,17,14,
	      17,17,17,17,17,10,4,
	      17,17,17,21,21,27,17,
	      17,17,10,4,10,17,17,
	      17,17,10,4,4,4,4,
	      31,1,2,4,8,16,31},
    *font_p;


void charout(int x,int y,char character,char color) {
   char mask;
   register int i, j;

   if (isalpha(toupper(character))) character-=65;
   else return;
   font_p=fonts + (character*7);
   for ( i=y; i < y+7; i++ ) {
      mask=16;
      for ( j=x; j < x+5; j++ ) {
         if (((*font_p) & mask) ) putpixel(j,i,color);
         mask >>=1;
      }
      font_p++;
   }
}

void outstr(int x,int y,char *string,char color) {
  int len=strlen(string),
      count;

  for (count = 0; count < len;count++) {
    charout(x,y,string[count],color);
    x+=6;
  }
}

char *numtostr(unsigned long num) {
  char *string=(char*)malloc(11);
  ltoa(num,string,10);
  free(string);
  return string;
}

/***********************************
   ROUTINES FOR CUSTOMIZING KEYBOARD
************************************/
int getkey() {
   while (1) {
     if (keys[kESC]) return  kESC;
     if (keys[A])    return  A;
     if (keys[S])    return  S;
     if (keys[D])    return  D;
     if (keys[F])    return  F;
     if (keys[G])    return  G;
     if (keys[H])    return  H;
     if (keys[J])    return  J;
     if (keys[K])    return  K;
     if (keys[L])    return  L;
     if (keys[Q])    return  Q;
     if (keys[W])    return  W;
     if (keys[E])    return  E;
     if (keys[R])    return  R;
     if (keys[T])    return  T;
     if (keys[Y])    return  Y;
     if (keys[U])    return  U;
     if (keys[I])    return  I;
     if (keys[O])    return  O;
     if (keys[P])    return  P;
     if (keys[Z])    return  Z;
     if (keys[X])    return  X;
     if (keys[C])    return  C;
     if (keys[V])    return  V;
     if (keys[B])    return  B;
     if (keys[N])    return  N;
     if (keys[M])    return  M;
     if (keys[KP0])  return KP0;
     if (keys[KPMINUS]) return KPMINUS;
     if (keys[LBRACKET]) return LBRACKET;
     if (keys[RBRACKET]) return RBRACKET;
     if (keys[ENTER]||keys[KPENTER])return ENTER;
     if (keys[RIGHTCTRL]) return LCTRL;
     if (keys[SEMICOLON]) return SEMICOLON;
     if (keys[RIGHTSHIFT]) return RIGHTSHIFT;
     if (keys[BACKSLASH]) return BACKSLASH;
     if (keys[SLASH]||keys[KPSLASH]) return SLASH;
     if (keys[COMMA]) return COMMA;
     if (keys[PERIOD]) return PERIOD;
     if (keys[RIGHTALT]) return LEFTALT;
     if (keys[SPACE]) return SPACE;
     if (keys[CAPSLOCK]) return CAPSLOCK;
     if (keys[NUMLOCK]) return NUMLOCK;
     if (keys[KP7]) return KP7;
     if (keys[KP9]) return KP9;
     if (keys[KP5]) return KP5;
     if (keys[KPPLUS]) return KPPLUS;
     if (keys[KP1]) return KP1;
     if (keys[KP3]) return KP3;
     if (keys[KPDEL]) return KPDEL;
     if (keys[KP4]||keys[LEFT]) return LEFT;
     if (keys[KP6]||keys[RIGHT]) return RIGHT;
     if (keys[KP2]||keys[DOWN]) return DOWN;
     if (keys[KEYPADSTAR]) return KEYPADSTAR;
     if (keys[KP8]||keys[UP]) return UP;
   }
}

#define PUT(s)    cputs(s);

void putkey(int i) {
   if (i==kESC ) PUT("ESC");
   if (i==A )    PUT("A");
   if (i==S )    PUT("S");
   if (i==D )    PUT("D");
   if (i==F )    PUT("F");
   if (i==G )    PUT("G");
   if (i==H )    PUT("H");
   if (i==J )    PUT("J");
   if (i==K )    PUT("K");
   if (i==L )    PUT("L");
   if (i==Q )    PUT("Q");
   if (i==W )    PUT("W");
   if (i==E )    PUT("E");
   if (i==R )    PUT("R");
   if (i==T )    PUT("T");
   if (i==Y )    PUT("Y");
     if (i==U )    PUT("U");
     if (i==I )    PUT("I");
     if (i==O )    PUT("O");
     if (i==P )    PUT("P");
     if (i==Z )    PUT("Z");
     if (i==X )    PUT("X");
     if (i==C )    PUT("C");
     if (i==V )    PUT("V");
     if (i==B )    PUT("B");
     if (i==N )    PUT("N");
     if (i==M )    PUT("M");
     if (i==KP0 )  PUT("KP 0");
     if (i==KPMINUS ) PUT("KP -");
     if (i==LBRACKET ) PUT("[");
     if (i==RBRACKET ) PUT (" ");
     if (i==ENTER ||i==KPENTER )PUT ("ENTER");
     if (i==RIGHTCTRL ) PUT ("CONTROL");
     if (i==SEMICOLON ) PUT (";");
     if (i==RIGHTSHIFT ) PUT ("SHIFT");
     if (i==BACKSLASH ) PUT ("\\");
     if (i==SLASH ||i==KPSLASH ) PUT ("/");
     if (i==COMMA ) PUT (",");
     if (i==PERIOD ) PUT (".");
     if (i==RIGHTALT ) PUT ("ALT");
     if (i==SPACE ) PUT ("SPACE");
     if (i==CAPSLOCK ) PUT ("CAPSLOCK");
     if (i==NUMLOCK ) PUT ("NUMLOCK");
     if (i==KP7 ) PUT ("KP 7");
     if (i==KP9 ) PUT ("KP 9");
     if (i==KP5 ) PUT ("KP 5");
     if (i==KPPLUS ) PUT ("KP +");
     if (i==KP1 ) PUT ("KP 1");
     if (i==KP3 ) PUT ("KP 3");
     if (i==KPDEL ) PUT ("DEL");
     if (i==KP4 ||i==LEFT ) PUT ("LEFT");
     if (i==KP6 ||i==RIGHT ) PUT ("RIGHT");
     if (i==KP2 ||i==DOWN ) PUT ("DOWN");
     if (i==KEYPADSTAR ) PUT ("*");
     if (i==KP8 ||i==UP) PUT ("UP");
}

 void centertext(int x1,int width,int y,char *text,char type,char FG){
   int xcen;

   switch (type) {
      case 0 : xcen=(width-(strlen(text)*8))/2;
	       outstr(x1+xcen,y,text,FG);
	       break;
      case 1 : xcen=(width-(strlen(text)*9))/2;
	       outstring(x1+xcen,y,text,FG);
	       break;
   }

 }

class TextWindow {
   char * buffer;
   int putx,puty,
       width,height;

   public:
     void SaveBkgrnd(int x1,int y1,int w,int h) {
	putx=x1;
	puty=y1;
	width=w;
	height=h;
	buffer = (char *)malloc(2*width*height);
	gettext(x1,y1,x1+w-1,y1+h-1,buffer);
     }
     void RestoreBkgrnd() {
	puttext(putx,puty,putx+width-1,puty+height-1,buffer);
	free(buffer);
     }
};

void Border(int x,int y,int width,int height,char FG,char BG) {
   int xc,yc;

   textattr((BG<<4)+FG);
   for (xc=x+1;xc < x+width-1;xc++) {
     gotoxy(xc,y);putch('ß');
     gotoxy(xc,y+height);putch('Ü');
   }

   for (yc=y;yc <= y+height;yc++) {
     gotoxy(x,yc);putch('Û');
     gotoxy(x+width-1,yc);putch('Û');
   }
}

void Window(int x,int y,int width,int height,char color,char bor) {
   int xc,yc;

   Border(x,y,width,height,color,bor);
   textcolor(bor);
   for (yc=y+1;yc < y+height;yc++)
     for (xc=x+1;xc < x+width-1;xc++) {
       gotoxy(xc,yc);putch('Û');
     }
}

void PutCheck(int x,int y,char FG,char BG) {
  textattr((BG<<4)+FG);
  gotoxy(x,y);putch(251);
}

void PutCheckBox(int x,int y,char FG,char BG) {
  textattr((BG<<4)+FG);
  gotoxy(x,y);putch(' ');
}


void centerstring(int x1,int width,int y,char *text,char FG,char BG){
   int xcen=(width-strlen(text))/2;

   textattr((BG << 4)+FG);
   gotoxy(x1+xcen,y);cputs(text);
}

void printtext(int x1,int y,char *text,char FG,char BG){
   textattr((BG << 4)+FG);
   gotoxy(x1,y);cputs(text);
}

void clearscr() {
   textbackground(0);
   clrscr();
}

char *getstring(int x,int y,int len,char cursor,char FG,char BG) {
  char *string=(char*)malloc(len+1),
	key;
  int count=0,xpos=x;
  int xwidth=1;

  while (1) {
    textattr((BG<<4)+cursor);
    gotoxy(xpos,y);
    putch('Ý');
    key=getch();
    if (key==13) {
      string[count]='\0';
      break;
    }
    switch (key) {
      case  8: if (count == 0) continue;
	       string[count]='\0';
	       count--;
	       textattr((BG<<4)+FG);
	       gotoxy(xpos,y);
	       putch(' ');
	       xpos-=xwidth;
	       break;
      default: if (count == len) continue;
	       if (!isprint(key)) continue;
	       string[count]=toupper(key);
	       count++;
	       textattr((BG<<4)+FG);
	       gotoxy(xpos,y);
	       putch(key);
	       xpos+=xwidth;
	       break;
    }
  }
  return string;
}

/*****************************
   SCORE TABLE
******************************/
#define ITEMS 15
int RecItems=0;

typedef struct TableStruct{
  unsigned long score;
  unsigned lines;
  char owner[10];
};

TableStruct RecTable[ITEMS];

void Sort(){
  int i,j;
  TableStruct temp;

  for(i=1 ; i < RecItems ; i++)
    for (j=RecItems-1; j >=1 ;j--)
      if(RecTable[j].score > RecTable[j-1].score) {
	temp=RecTable[j-1];
	RecTable[j-1]=RecTable[j];
	RecTable[j]=temp;
      }
}

void Add(TableStruct val) {
  int i,i2;

  if (RecItems < ITEMS) {
    if (val.score < DEFHIGH) return;
    RecTable[RecItems]=val;
    RecItems++;
    Sort();
  }
  else {
    for (i = 0; i < ITEMS; i++) {
      if (val.score >= RecTable[i].score) {
	for (i2=ITEMS-1; i2 > i;i2--)
	  RecTable[i2]=RecTable[i2-1];
	RecTable[i]=val;
	break;
      }
    }
  }
}

void LoadTable() {
  fp = fopen(SCOREFILE,"rb");
  if (!fp) {
    RecItems=0;
    GameHighScore=DEFHIGH;
  }
  else {
    RecItems=filelength(fileno(fp))/sizeof(TableStruct);
    if (RecItems) {
      for (row = 0; row < RecItems ; row++)
	fread(&RecTable[row],sizeof(TableStruct),1,fp);
      GameHighScore=RecTable[0].score;
    }
    else {
      RecItems=0;
      GameHighScore=DEFHIGH;
    }
    fclose(fp);
  }
}

void SaveTable(){
  if (!RecItems) return;
  fp = fopen(SCOREFILE,"w+b");
  for (row = 0; row < RecItems ; row++)  fwrite(&RecTable[row],sizeof(TableStruct),1,fp);
  fclose(fp);
}

void ResetScores(int x,int y) {
  TextWindow w;

  if (!RecItems) return;
  for (row = 0; row < RecItems ; row++)
     RecTable[row].score=0;
  RecItems=0;
  GameHighScore=DEFHIGH;
  unlink(SCOREFILE);
  w.SaveBkgrnd(x,y,20,3);
  Window(x,y,20,2,12,1);
  centerstring(x,20,y+1,"SCORES RESET",15,1);
  getch();
  w.RestoreBkgrnd();
}

/*********************************8
   GAME BITMAPS
**********************************/

int loadbitmaps() {
  FILE *binf;
  char far * buffer,
	   * binfile=(char*)malloc(14);
  ULONG offset=0;

  xmshandle = xalloc(2000);
  if (!xmshandle) return 2;

  fp = fopen(FILELIST,"rb");
  fseek(fp,91,0);
  printf("\nLoading bitmaps...");
  for (row = 0; row < MaxBitmaps ;row++) {
    fread(binfile,13,1,fp);
    binf   = fopen(binfile,"rb");
    if (!binf) continue;
    buffer = loadfile(binf);
    if (!buffer) {
      fclose(fp);
      fclose(binf);
      free(binfile);
      return 3;
    }
    DOStoXMS(xmshandle,offset,buffer,64768l);
    offset+=64768l;
    farfree(buffer);
    fclose(binf);
  }
  free(binfile);
  fclose(fp);
  return 1;
}

void changebkground(int num) {

   char far *background=(char far*)farmalloc(64000l);
   char	*_pal=(char*)malloc(768);
   ULONG offset;

   if (background && _pal) {
     offset = num*64768l;
     XMStoDOS(background,xmshandle,offset,64000l);
     offset+=64000l;
     XMStoDOS(_pal,xmshandle,offset,768);
     setpalette_part(_pal,0,0,235);
     fastput_raw(background,0,0,320,200);
     free(_pal);
     farfree(background);
   }
}

class Animation_Events{
  int  xput,yput,width,height;
  char far *raw_data,
       far *p,tdel,
       far *back,
       mask,inf;

  public:
    signed char playing;
    int index,items;
    unsigned long local_t;

    Animation_Events();
    void FirstFrame();
    void Play();
    void InitValues(char,char far *,int,int,int,int,char,char,char);
    void Start();
};

void Animation_Events::FirstFrame() {
  if (!raw_data) return;
  p = raw_data;
  putimage(back,xput,yput);
  draw_anim(p,xput,yput,width,height,mask);
  show();
}

void Animation_Events::Start() {
  if (!raw_data && !back) return;
  if (index > 0 && index < items) return;
  getimage(back,xput,yput,width,height);
  playing = 1;
  index  = 0;
  local_t = get_fast();
}

void Animation_Events::InitValues(char numitems,char far * s_data,
                              int x,int y,int w,int h,char t_delay,char m,char i) {

  inf    = i;
  mask   = m;
  width  = w;
  height = h;
  xput   = x;
  yput   = y;
  items  = numitems;
  tdel   = t_delay;
  if (s_data) {
    raw_data  = s_data;
    p      = raw_data;
    back = (char far*)farmalloc((width*height)+4);
  }
}

Animation_Events::Animation_Events() {
  index   = 0;
  playing = 0;
}

void Animation_Events::Play() {
  if (get_fast() >= local_t  && playing) {
    if (!(index % 3)) putimage(back,xput,yput);
    draw_anim(p,xput,yput,width,height,mask);
    show();
    p += (width*height);
    local_t+=tdel;
    index++;
    if (index == items) {
      if (inf) {
        local_t+=3*tdel;
        index=0;
        p=raw_data;
      }
      else {
        playing = 0;
        index = 0;
        FirstFrame();
      }
    }
  }
}

void Error(char * errmess) {
  printf("\n%s \n\rError running program.",errmess);
  deinit_timer();
  exit (-1);
}

void CheckFiles() {
   char * tname=(char*)malloc(14);
   FILE * tf;

   fp = fopen(FILELIST,"rb");
   if (!fp) {
      printf("\nFile missing : Files.lst");
      Error("");
   }
   while (!feof(fp)) {
     fread (tname,13,1,fp);
     tf = fopen(tname,"rb");
     if (!tf) {
       printf("\nFile missing : %s",tname);
       Error("");
     }
     fclose(tf);
   }
   free(tname);
   fclose(fp);
}

void AssignSound(char i) {
   switch (i) {
     case 1 : LAP.InitValues(20,sd2);
              break;
     case 2 : LAP.InitValues(20,sd3);
              break;
     case 3 : LAP.InitValues(20,sd4);
              break;
     case 4 : LAP.InitValues(6,Quad);
              break;
   }
   LAP.EnableEvent();
}

int ControlBreak(void) {
  asm {
    mov ax, 3h
    int 10h
  }
  xfree(xmshandle);
  deinit_timer();
  resetregs();
  exit (1);
}

void InitializeGame() {
  int retval;

  ctrlbrk(ControlBreak);
  if (!XMSinit()) Error("XMS not available.");
  else {
    printf("\nXMS driver detected.");
    printf("\nXMS Core memory : %lu bytes free",xcoreleft());
    if (xcoreleft() < 2097152) {
       Error("Insufficient free XMS core memory");
    }
    retval = loadbitmaps();
    switch (retval) {
      case 2 : Error("Error allocating XMS memory.");
      case 3 : Error("Insufficient DOS memory.");
    }
  }
  if (detectboard) {
    printf("\nAdlib-compatible sound card detected.");
    initchannels();
  }
  else printf("\nNo sound card detected.");
  randomize();
}

void ShowPause(int x,int y) {
  bar(x,y,59,16,250);
  outstring(x+7,y+4,"PAUSED",238);
}




