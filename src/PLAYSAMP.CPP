#include <iostream.h>
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <alloc.h>
#include <stdlib.h>
#include <mem.h>

#define SPKPORT 0X0061
#define PITCTRL 0X0043
#define PITCHN2 0X0042
#define PITFREQ 0x1234DD

void playsample(char far* sample,long n)
{
	static unsigned i=0;
	static char     t,k=0;

	_asm {
				pusha
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
		t=sample[i]<<1;
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
		sti
		popa
	}
}


char far *loadsamplefile(char *f,long *len)
{
	FILE*fp;
	char buffer[64];
	char riff[4];
	long cn;
	char far *pv, far *v;
	long n;

	fp=fopen(f,"rb");

	if (!fp) return NULL;

	fread(&riff,4,1,fp);

	if (riff[0] == 'R' &&
		riff[1] == 'I' &&
		riff[2] == 'F' &&
		riff[3] == 'F')
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

	*len = n;

	v = (char far*)	farmalloc(n);

	if (!v)
	{
		fclose(fp);
		return NULL;
	}

	printf("\nSize %lu",n);
	cn = n;
	pv = v;
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

	return v;
}

int main(int argc,char**argv)
{
	char far *p;
	long n;
	char iswav;

	if      (argc == 1) return 0;

	p = loadsamplefile(argv[1],&n);

	if (p)
	{
		playsample(p,n);
		farfree(p);
	}
	return 1;
}