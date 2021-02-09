#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <alloc.h>

int adlib_check()
{
	#define ADDR	0388H
	#define	DATA	0389H
	static unsigned char retval;

	_asm {
		MOV DX, ADDR   // ADDR
		MOV AL, 4
		OUT DX, AL
		INC DX         // DATA
		MOV AL, 60H
		OUT DX, AL
		DEC DX         // ADDR
		MOV AL, 4
		OUT DX, AL
		INC DX         // DATA
		MOV AL, 80H
		OUT DX, AL
		DEC DX         // ADDR
		IN  AL, DX
		MOV AH, AL
		MOV AL, 2
		OUT DX, AL
		INC DX         // DATA
		MOV AL, 0FFH
		OUT DX, AL
		INC DX         // ADDR
		MOV AL, 4
		OUT DX, AL
		INC DX         // DATA
		MOV AL, 21H
		OUT DX, AL
		mov cx, 1000
	}
       _delay:
	_asm {
		dec cx
		jnz _delay
		mov dx, ADDR
		in  al, dx
		mov bx, ax
		mov al, 4
		out dx, al
		inc dx
		mov al, 60h
		out dx, al
		dec dx
		mov al, 4
		out dx, al
		inc dx
		mov al, 80h
		out dx, al
		and bl, 0e0h
		and bh, 0e0h
		or  bh, bl
		mov retval, bh
	}

	if (retval == 0xc0) return 1;
	else return 0;
}


void adlib_init()
{
	for (int i=1;i<245;++i)
	{
		outp(0x388,i);
		outp(0x389,0);
	}
}


void adlib_sound(int freq,int c)
{
	int channel;
	switch (c)
	{
	case 0: case 1: case 2: channel = c; 	  break;
	case 3:	case 4:	case 5: channel = c + 5;  break;
	case 6:	case 7:	case 8: channel = c + 10; break;
	}
	outp(0x388,0x20+channel);
	outp(0x389,0x01);
	outp(0x388,0x40+channel);
	outp(0x389,0x10);
	outp(0x388,0x60+channel);
	outp(0x389,0xF0);
	outp(0x388,0x80+channel);
	outp(0x389,0x77);
	outp(0x388,0xA0+c);
	outp(0x389,(unsigned char)(freq & 0x00FF));
	outp(0x388,0x23+channel);
	outp(0x389,0x01);
	outp(0x388,0x43+channel);
	outp(0x389,0x00);
	outp(0x388,0x63+channel);
	outp(0x389,0xF0);
	outp(0x388,0x83+channel);
	outp(0x389,0x77);
	outp(0x388,0xB0+c);
	outp(0x389,0x30 | (unsigned char)((freq>>8)&0x03));

}

void adlib_nosound(int c)
{
	outp(0x388,0xb0+c);
	outp(0x389,0x11);
}


#define C      261
#define D      293
#define E      329
#define F      349
#define G      392
#define A      440
#define B      494
#define C2     523
#define D2     587
#define E2     659
#define F2     698
#define G2     784
#define A2     880
#define NOTES  13

int main()
{
	int tones[]={C,D,E,F,G,A,B,C2,D2,E2,F2,G2,A2};
	char notes[]={
	0,0,0,3,3,4,4,7,5,3,3,5,3,1,6,4,2,3,3,4,
	5,5,5,6,5,5,4,4,5,6,6,6,7,6,5,7,7,7,5,3,5,3,
	0,0,0,2,4,6,4,2,4,3,2,1,2,2,2,1,2,4,4,
	0,0,0,2,4,6,4,2,4,3,2,1,3,3,3,2,3,4,4,
	4,4,4,4,5,6,4,5,4,3,3,3,5,4,3,3,2,7,
	7,7,7,5,3,4,7,7,7,7,5,3,4,0,3,4,
	5,6,7,8,4,8,7,7,5,6,4,3
	};
	int n=sizeof(notes);

	if (adlib_check()) printf("adlib card present.");
	else               printf("adlib card not found.");

	adlib_init();

	for (int i=0;i<n;++i)
	{
		adlib_sound(tones[notes[i]],0);
		adlib_sound(tones[notes[i]]>>1,1);
		adlib_sound(tones[notes[n-i-1]],2);
		delay(200);
		adlib_sound(0,0);
		delay(10);
	}
	for (i=0;i<9;i++)
		adlib_nosound(i);
	return 1;
}