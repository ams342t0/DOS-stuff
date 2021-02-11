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
				mov al, 4ah
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
		t=((i&4096)?((i*(i^i%255)|(i>>4))>>1):(i>>3)|((i&8192)?i<<2:t));

		i++;
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
		sti
	}
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
			/*
	printf("\nMonotone sound.");
	playtone();

	for(int i=0;i<sizeof(notes);++i)
	{
		sound_on(tones[notes[i]]);
		delay(5);
	}
	sound_off();
			  */
	printf("\nMusical one-line C code.");
	onelinemusic();

	return 1;
}
