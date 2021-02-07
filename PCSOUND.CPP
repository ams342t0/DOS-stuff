#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <alloc.h>
#include <stdlib.h>


char  portval;


void soundoff()
{
	_asm {
		mov dx, 0061h
		in  al, dx
		mov portval, al
		and al, 0fch
		out dx, al
	}

	while (!kbhit())
	{
		_asm {
			mov dx, 0061h
			mov al, portval
			xor al, 2
			out dx, al
		}
		delay(3);
		_asm {
			mov dx, 0061h
			mov al, portval
			out dx, al
		}
		delay(3);
	}

}

#define SPKPORT	0061H
#define	PITCTRL	0043H
#define PITCHN2 0042H
#define PITFREQ 0x1234DD

void sound_on(unsigned int freq)
{
	unsigned int counter;

	counter = PITFREQ/freq;

	_asm {
		mov dx, PITCTRL
		mov al, 0b6h
		out dx, al
		mov dx, PITCHN2
		mov ax, counter
		out dx, al
		shr ax, 8
		out dx, al

		mov dx, SPKPORT
		in  al, dx
		or  al, 3
		out dx, al
	}
}

void sound_off()
{
	_asm {
		mov dx, SPKPORT
		in  al, dx
		and al, 0fch
		out dx, al
	}
}



void soundoff2()
{
	static unsigned i=0;
	static char t,k=0;
	char tdel[256];

	for(int n=0;n<255;++n)
		tdel[n]=n*54/255;

	_asm {
		mov dx, 043h
		mov al, 90h
		out dx, al
		mov dx, 61h
		in  al, dx
		or  al, 3
		out dx, al
		cli
		}

	while (!k)
	{
		t = ((i>>6|i|i>>(i>>16))*10+((i>>11)&7));
		//t=i*(i>>8*(i>>15|i>>8)&(20|(i>>19)*5>>t|t>>3));
		//t=i*((i>>12|i>>8)&63&i>>4);
		_asm {
			mov dx, 42h
			mov al, t
			out dx, al
			xor bx, bx
		}
		loopdel1:
		_asm{
			inc bx
			cmp bx,100
			jbe loopdel1
			mov al, t
			out dx, al
			xor bx, bx
		}
		loopdel2:
		_asm {
			inc bx
			cmp bx, 100
			jbe loopdel2
			inc i
			mov ah, 2
			int 16h
			mov k, al
		}
	}
	_asm 	sti
}


int main()
{
	soundoff2();

	return 1;
}