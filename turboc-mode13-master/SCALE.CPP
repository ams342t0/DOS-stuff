#include <alloc.h>
#include <mem.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "grlib.h"

#define putpixel(x,y,c) *(videoram+x+y*320)=c
char far*videoram=(char far*)0xa0000000;
char far *dblbuffer=(char far*)farmalloc(64000);

#define ClipX1 0
#define ClipX2 320
#define ClipY1 0
#define ClipY2 200

void scaleput(int x,int y,int sw,int sh,int dwid,int dht,char far *bitmap) {
   char far *p;


   p=bitmap;
   int xc,yc;
   int decx,decy;

   decx = -dwid;
   decy = -dht;
   yc=y;
   while (1) {
      p=bitmap;
      xc=x;
      while (1){
        putpix:
          if (xc == (x+dwid)) break;
          if (xc < 0 || xc > 319) break;
          putpixel(xc,yc,*p);
          xc++;
          decx += sw;
          if (decx < 0) goto putpix;
       skipcols:
          p++;
          decx -= dwid;
          if (decx >=0) goto skipcols;
      }
      yc++;
      if (yc == (y+dht)) break;
      decy+=sh;
      if (decy < 0) continue;
      skiprows:
        bitmap+=sw;
        decy-=dht;
        if (decy >=0)goto skiprows;
   }

}

void screen_blur()  {
  asm {
    push ds
    lds  di, videoram
    add  di, 321
    xor  bx, bx//xorl %%ebx, %%ebx;
    mov  cx, 63678
  }
  top2:
  asm {
    xor ax, ax
    mov al, [di-321]   //movb (%%edi), %%al;
    mov bl, [di-320] //movb 2(%%edi), %%bl;
    add ax, bx//addl %%ebx, %%eax;
    mov  bl, [di-319]//movb 321(%%edi), %%bl;
    add  ax, bx//addl %%ebx, %%eax;
    mov  bl, [di-1]//movb 641(%%edi), %%bl;
    add  ax, bx//addl %%ebx, %%eax;
    mov  bl, [di+1]//movb 641(%%edi), %%bl;
    add  ax, bx//addl %%ebx, %%eax;
    mov  bl, [di+319]//movb 641(%%edi), %%bl;
    add  ax, bx//addl %%ebx, %%eax;
    mov  bl, [di+320]//movb 641(%%edi), %%bl;
    add  ax, bx//addl %%ebx, %%eax;
    mov  bl, [di+321]//movb 641(%%edi), %%bl;
    add  ax, bx//addl %%ebx, %%eax;

    shr  ax, 3//shrl $2, %%eax;
    jz zero2
//    dec ax//decl %%eax;
  }
  zero2:
  asm {
    mov  [di], al//movb %%al, (%%esi);
    inc  di //incl %%edi;

    dec  cx//decl %%ecx;
    jnz  top2
    pop ds
  }
}



void scaleput_asm(int x,int y,int sw,int sh,int dwid,int dht,char far *bitmap) {
   int decx,decy;

   asm {
        push    ds
        les     di, videoram
        lds     si, bitmap

        mov     ax, 320
        mul     y
        add     ax, x
        add     di, ax

        mov     ax, dwid
        neg     ax
        mov     decx, ax
        mov     ax, dht
        neg     ax
        mov     decy, ax
        mov     ah, byte ptr dht
        mov     dx, decx
        mov     bx, decy
   }
toploop://@2@58:
   asm {
        mov     cx, dwid
        push    di
        push    si
   }
columnloop://@2@86:
   asm {
        dec     cx
        js      columndone//@2@282
        mov     al, [si]
        cmp     al, 0ffh
        je      skippix
        mov     es:[di], al
   }
   skippix:
   asm {
        inc     di
        add     dx, sw
        js      columnloop
    }
skipcol://@2@198:
    asm {
        inc     si
        sub     dx, dwid
	jns     skipcol
        jmp     columnloop
     }
columndone:
     asm {
        pop     si
        pop     di
        add     di, 320
        dec     ah
	jz	alldone//short @2@422
        add     bx, sh
	js	toploop//short @2@58
    }
skiprow://2@338:
    asm {
        add     si, sw
        sub     bx, dht
        jns     skiprow
	jmp	toploop//@2@58
    }
alldone://@2@422:
    asm{
        pop     ds
    }
}

void main() {
    char far *test;
    test=LoadFileData("sarrah.raw");
    LoadPalette("pal.13");
    asm {
       mov ax, 13h
       int 10h
    }
    SetPalette();
    int dx,dy;
    int stepx,stepy;
    stepx=-3;
    stepy=-2;
    dx=319;
    dy=199;
    _fmemset(dblbuffer,0,64000);
    while (!kbhit()) {
       dx+=stepx;
       dy+=stepy;
       if (dx >= 319 || dx <= 0) stepx=-stepx;
       if (dy >= 199 || dy <= 0) stepy=-stepy;
       asm mov dx, 3dah
       v0:
       asm {
          in al, dx
          test al,8
          jz v0
       }
       v1:
       asm {
          in al, dx
          test al,8
          jnz v1
       }
       scaleput_asm(159-(dx/2),99-(dy/2),72,104,dx,dy,test);
//       _fmemcpy(videoram,dblbuffer,64000);
    }
    getch();
    screen_blur();
//   _fmemcpy(videoram,dblbuffer,64000);
    getch();
    asm {
       mov ax, 3h
       int 10h
    }

}