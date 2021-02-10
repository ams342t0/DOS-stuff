<<<<<<< HEAD
/*
programming the PC speaker.
Referenced from:
		     ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
		     ³ Programming the PC Speaker ³
		     ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

	    Written for the PC-GPE by Mark Feldman
            e-mail address : u914097@student.canberra.edu.au
                             myndale@cairo.anu.edu.au


*/

#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <alloc.h>
#include <stdlib.h>

#define SPKPORT 0061H
#define PITCTRL 0043H
#define PITCHN2 0042H
#define PITFREQ 0x1234DD

char  portval;

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
        static char t,k=0;
        char tdel[256];
	static int sel=0;

	for(int n=0;n<255;++n)
                tdel[n]=n*54/255;

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
                /*
			Musical 1-line C codes:
                        https://github.com/kragen/viznut-music
		*/

		switch (sel)
		{
			case 0: t = i*((i>>12|i>>8)&63&i>>4); break;
			case 1: t = (i>>7|i|i>>6)*10+4*(i&i>>13|i>>6);break;
			case 2: t = i*(i>>11&i>>8&123*i>>3); break;
			case 3: t = i*(i>>8*(i>>15|i>>8)&(20|(i>>19)*5>>i|i>>3));break;
			case 4: t = (i|(i>>9|i>>7))*i&(i>>11|i>>9);break;
		}

		_asm {
			mov dx, PITCHN2
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
			cmp i, 0ffffh
			jb skip
			cmp sel, 4
			jae skip
			inc sel
		}
	skip:
		_asm {
                        mov ah, 2
                        int 16h
                        mov k, al
		}
        }
        _asm    sti
}

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

int main()
{
	int tones[]={C,Cs,D,Ds,E,F,Fs,G,Gs,A,As,B};

	printf("\nMonotone output\n");
	playtone();

	printf("\nFrequency tone output.\n");

	int i=0;
	while (!kbhit() && i<12)
	{
		sound_on(tones[i]);
		delay(300);
		i++;
	}

	printf("\nMusical one-line C code.");
	onelinemusic();

        return 1;
=======
/*
programming the PC speaker.
Referenced from:
		     ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
		     ³ Programming the PC Speaker ³
		     ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

	    Written for the PC-GPE by Mark Feldman
            e-mail address : u914097@student.canberra.edu.au
                             myndale@cairo.anu.edu.au


*/

#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <alloc.h>
#include <stdlib.h>

#define SPKPORT 0061H
#define PITCTRL 0043H
#define PITCHN2 0042H
#define PITFREQ 0x1234DD

char  portval;

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
        static char t,k=0;
        char tdel[256];
	static int sel=0;

	for(int n=0;n<255;++n)
                tdel[n]=n*54/255;

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
                /*
			Musical 1-line C codes:
                        https://github.com/kragen/viznut-music
		*/

		switch (sel)
		{
			case 0: t = i*((i>>12|i>>8)&63&i>>4); break;
			case 1: t = (i>>7|i|i>>6)*10+4*(i&i>>13|i>>6);break;
			case 2: t = i*(i>>11&i>>8&123*i>>3); break;
			case 3: t = i*(i>>8*(i>>15|i>>8)&(20|(i>>19)*5>>i|i>>3));break;
			case 4: t = (i|(i>>9|i>>7))*i&(i>>11|i>>9);break;
		}

		_asm {
			mov dx, PITCHN2
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
			cmp i, 0ffffh
			jb skip
			cmp sel, 4
			jae skip
			inc sel
		}
	skip:
		_asm {
                        mov ah, 2
                        int 16h
                        mov k, al
		}
        }
        _asm    sti
}

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

int main()
{
	int tones[]={C,Cs,D,Ds,E,F,Fs,G,Gs,A,As,B};

	printf("\nMonotone output\n");
	playtone();

	printf("\nFrequency tone output.\n");

	int i=0;
	while (!kbhit() && i<12)
	{
		sound_on(tones[i]);
		delay(300);
		i++;
	}

	printf("\nMusical one-line C code.");
	onelinemusic();

        return 1;
>>>>>>> 9fece98d53bf4693969f165d0f81ade22719d39d
}