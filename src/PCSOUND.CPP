/*
programming the PC speaker.
Referenced from:
		     旼컴컴컴컴컴컴컴컴컴컴컴컴컴커
		     � Programming the PC Speaker �
		     읕컴컴컴컴컴컴컴컴컴컴컴컴컴켸

	    Written for the PC-GPE by Mark Feldman
	    e-mail address : u914097@student.canberra.edu.au
			     myndale@cairo.anu.edu.au


*/

#include <iostream.h>
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <alloc.h>
#include <stdlib.h>
#include <mem.h>

#define SPKPORT 0061H
#define PITCTRL 0043H
#define PITCHN2 0042H
#define PITFREQ 0x1234DD

char  portval;
char  far *v=(char far*)farmalloc(64000);
long  n;

void playtone()
{
	_asm {
                mov dx, SPKPORT
                in  al, dx
                mov portval, al
                and al, 0fch
		out dx, al
        }

        while (!kbhit())
        {
                _asm {
                        mov dx, SPKPORT
                        mov al, portval
                        xor al, 2
                        out dx, al
                }
		delay(5);
		_asm {
			mov dx, SPKPORT
			mov al, portval
			out dx, al
		}
		delay(5);
	}

	getch();
}


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


void onelinemusic()
{
	static unsigned i=0;
	static char     t,k=0;

	_asm {
				mov dx, PITCTRL
				mov al, 90h
				out dx, al
				mov dx, SPKPORT
				in  al, dx
				or  al, 3
				out dx, al
				cli
		 }

	k = 0;
	while (!k)
	{
		t=v[i];
		i++;
		if (i==n) i = 0;


		_asm {
			mov dx, PITCHN2
			mov al, t
			out dx, al
			xor bx, bx
		}
		loopdel1:
		_asm{
			inc bx
			cmp bx,250
			jbe loopdel1
			mov al, t
			out dx, al
			xor bx, bx
		}
		loopdel2:
		_asm {
			inc bx
			cmp bx, 250
			jbe loopdel2
			mov ah, 2
			int 16h
			mov k, al
		}
	}
	_asm    {
		popa
	}
}

#define NOTES  13
#define C      261
#define Cs     277
#define D      293
#define Ds     311
#define E      329
#define F      349
#define Fs     370
#define G      392
#define Gs     415
#define A      440
#define As     466
#define B      494
#define C2     523

int main(int argc,char**argv)
{
	FILE*fp;
	char buffer[64];
	long cn;
	char far*pv=v;

	if (argc < 3) return 1;

	fp=fopen(argv[1],"rb");

	if (argv[2][0] == 'w')
	{
		printf("\nWave file");
		fseek(fp,40,SEEK_SET);
		fread(&n,4,1,fp);
		printf("\nLength %lu",n);
	}
	else
	{
		printf("\nRaw file.");
		fseek(fp,0,SEEK_END);
		n = ftell(fp);
		fseek(fp,0,SEEK_SET);
	}

	if (n > 65536) n = 65536;

	printf("\nSize %lu",n);
	cn = n;
	while(cn>=64)
	{
		fread(buffer,64,1,fp);
		_fmemcpy(pv,buffer,64);
		cn -=64;
		pv+=64;
	}

	if (cn)
	{
		fread(buffer,cn,1,fp);
		_fmemcpy(pv,buffer,cn);
	}

	fclose(fp);

	int tones[]={C,Cs,D,Ds,E,F,Fs,G,Gs,A,As,B,C2};

/*	printf("\nMonotone output\n");
	playtone();

	printf("\nFrequency tone output.\n");

	int i=0;

	while (!kbhit() && i<NOTES)
	{
		sound_on(tones[i]);
		delay(300);
		i++;
	}
	*/

	printf("\nMusical one-line C code.");
	onelinemusic();

	farfree(v);
	return 1;
}