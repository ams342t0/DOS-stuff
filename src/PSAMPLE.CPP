/*
PC speaker output at 8khz using 8-bit samples.
Works great when compiled for DOS4GW, 
gets rid of the 64k alloc limit.
Only tested on DOSBox. Timing will vary
with different configurations and may not work the same way.
*/

#include <iostream.h>
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <mem.h>

#define SPKPORT 0X0061
#define PITCTRL 0X0043
#define PITCHN2 0X0042
#define PITFREQ 0x1234DD

void playsample(unsigned char *sample,long slen)
{
        static unsigned long i;
        static unsigned char t,k=0;
        static long q = slen;

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
        i = 0;
        while (!k)
        {
                t=sample[i];
                i++;
                if (i>=q) i = 0;
                _asm {
                        mov dx, PITCHN2
                        mov al, t
                        out dx, al
                        xor bx, bx
                }

                _asm
                {
                loopA:
                        inc bx
                        cmp bx,150
                        jbe loopA
                        mov al, t
                        out dx, al
                        xor bx, bx
                }

                _asm 
                {
                loopB:
                        inc bx
                        cmp bx, 150
                        jbe loopB
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


unsigned char *loadsamplefile(char *f,long *len)
{
        FILE*fp;
        char buffer[64];
        char riff[4];
        long cn;
        unsigned char *pv, *v;
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

        *len = n;

        v = (unsigned char *) malloc(n);

        if (!v)
        {
                fclose(fp);
                return NULL;
        }

        cn = n;
        pv = v;
        while(cn>=64)
        {
                fread(buffer,64,1,fp);
                memcpy(pv,buffer,64);
                cn -=64;
                pv+=64;
        }

        if (cn)
        {
                fread(buffer,cn,1,fp);
                memcpy(pv,buffer,cn);
        }

        printf("\n%lu bytes read",n);

        fclose(fp);

        return v;
}

int main(int argc,char**argv)
{
        unsigned char *p;
        long n;
        char iswav;

        if      (argc == 1) return 0;

        p = loadsamplefile(argv[1],&n);

        if (p)
        {
                playsample(p,n);
                free(p);
        }
        return 1;
}