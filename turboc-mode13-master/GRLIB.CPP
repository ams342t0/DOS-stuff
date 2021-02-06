#include <stdio.h>
#include <io.h>
#include <mem.h>
#include <alloc.h>
#include <dos.h>

int ClipX1,ClipY1,ClipX2,ClipY2;
char  PALETTE[768];
char far *VGAMEM,
     far *BUFFER,
     far *Destination;

char far *LoadFileData(const char *fname) {
  FILE *fp;
  char far *p,far *tbuffer;
  char *buffer;
  unsigned long flen;

  tbuffer=NULL;
  fp = fopen(fname,"rb");
  if (fp) {
    flen = (unsigned long) filelength(fileno(fp));
    if (flen > 65535) flen = 65535;
    tbuffer = (char far*)farmalloc(flen);
    if (!tbuffer) return NULL;
    buffer = (char*)malloc(1024);
    if (!buffer) return NULL;
    p=tbuffer;
    while (flen >= 1024) {
      fread(buffer,1,1024,fp);
      _fmemcpy(p,buffer,1024);
      flen-=1024;
      p+=1024;
    }
    if (flen) {
      fread(buffer,1,flen,fp);
      _fmemcpy(p,buffer,flen);
    }
    fclose(fp);
    free(buffer);
    return tbuffer;
  }
  return NULL;
}

void LoadPalette(const char *file) {
  FILE *fp;

  fp = fopen(file,"rb");
  if (fp) {
    fread(PALETTE,1,768,fp);
    fclose(fp);
  }
}

void SetPalette() {
  asm {
    xor ax, ax
    mov dx, 3c8h
    out dx, al
    mov si, offset PALETTE
    inc dx
    mov cx, 768
    rep outsb
  }
}

void PutBitmap(char far *src,signed  x,signed  y,signed  wid,signed  ht) {
   asm {
     mov  ax, word ptr x
     cmp  ax, ClipX2
     jg   nodisplay

     add  ax, word ptr wid
     dec  ax
     cmp  ax, ClipX1
     jl   nodisplay

     mov  ax, word ptr y
     cmp  ax, ClipY2
     jg   nodisplay

     add  ax, word ptr ht
     dec  ax
     cmp  ax, ClipY1
     jl   nodisplay
     jmp  clip
   }
   nodisplay:  return;
   clip:

   signed  tw=wid;
   int px,py;
   px=0;
   py=0;
   if (x < ClipX1) {
      px = ClipX1-x;
      tw = wid - px;
      x  = ClipX1;
   }
   if ((x+tw-1) > ClipX2) tw = ClipX2-x+1;
   if (y < ClipY1) {
      py = ClipY1-y;
      ht = ht - py;
      y  = ClipY1;
   }
   if ((y+ht-1) > ClipY2) ht = ClipY2-y+1;

   asm {
     push  ds

     les   di, Destination
     lds   si, src

     mov   ax, word ptr py
     mul   word ptr wid
     add   ax, word ptr px
     add   si, ax

     mov   ax, y
     shl   ax, 8
     add   ax, x
     add   di, ax

     mov   dh, byte ptr ht
     mov   bx, word ptr tw
     mov   cx, bx
   }
   loopx:
   asm {
     mov  al,  [si]
     cmp  al,  0ffh
     je   skipmask
     mov  es:[di], al
   }
   skipmask:
   asm {
     inc  di
     inc  si
     dec  cx
     jnz  loopx
     add  di, 256
     sub  di, bx
     add  si, word ptr wid
     sub  si, bx
     mov  cx, bx
     dec  dh
     jnz  loopx
     pop  ds
   }
}

void VFlip(char far *src,signed  x,signed  y,signed  wid,signed  ht) {
   asm {
     mov  ax, word ptr x
     cmp  ax, ClipX2
     jg   nodisplay

     add  ax, word ptr wid
     dec  ax
     cmp  ax, ClipX1
     jl   nodisplay

     mov  ax, word ptr y
     cmp  ax, ClipY2
     jg   nodisplay

     add  ax, word ptr ht
     dec  ax
     cmp  ax, ClipY1
     jl   nodisplay
     jmp  clip
   }
   nodisplay:  return;
   clip:

   signed  tw=wid;
   int px,py,boff;

   boff=wid*(ht-1);

   px=0;
   py=0;
   if (x < ClipX1) {
      px = ClipX1-x;
      tw = wid - px;
      x  = ClipX1;
   }
   if ((x+tw-1) > ClipX2) tw = ClipX2-x+1;
   if (y < ClipY1) {
      py = ClipY1-y;
      ht = ht - py;
      y  = ClipY1;
   }
   if ((y+ht-1) > ClipY2) ht = ClipY2-y+1;

   asm {
     push  ds

     les   di, Destination
     lds   si, src

     add   si, boff

     mov   ax, word ptr py
     mul   word ptr wid
     sub   si, ax
     mov   ax, word ptr px
     add   si, ax

     mov   ax, y
     shl   ax, 8
     add   ax, x
     add   di, ax

     mov   dh, byte ptr ht
     mov   bx, word ptr tw
     mov   cx, bx
   }
   loopx:
   asm {
     mov  al,  [si]
     cmp  al,  0ffh
     je   skipmask
     mov  es:[di], al
   }
   skipmask:
   asm {
     inc  di
     inc  si
     dec  cx
     jnz  loopx
     add  di, 256
     sub  di, bx
     sub  si, word ptr wid
     sub  si, bx
     mov  cx, bx
     dec  dh
     jnz  loopx
     pop  ds
   }
}

void HVFlip(char far *src,signed  x,signed  y,signed  wid,signed  ht) {
   asm {
     mov  ax, word ptr x
     cmp  ax, ClipX2
     jg   nodisplay

     add  ax, word ptr wid
     dec  ax
     cmp  ax, ClipX1
     jl   nodisplay

     mov  ax, word ptr y
     cmp  ax, ClipY2
     jg   nodisplay

     add  ax, word ptr ht
     dec  ax
     cmp  ax, ClipY1
     jl   nodisplay
     jmp  clip
   }
   nodisplay:  return;
   clip:

   signed  tw=wid;
   int px,py,boff;

   boff=wid*ht;

   px=0;
   py=0;
   if (x < ClipX1) {
      px = ClipX1-x;
      tw = wid - px;
      x  = ClipX1;
   }
   if ((x+tw-1) > ClipX2) tw = ClipX2-x+1;
   if (y < ClipY1) {
      py = ClipY1-y;
      ht = ht - py;
      y  = ClipY1;
   }
   if ((y+ht-1) > ClipY2) ht = ClipY2-y+1;

   asm {
     push  ds

     les   di, Destination
     lds   si, src

     add   si, boff
     dec   si

     mov   ax, word ptr py
     mul   word ptr wid
     sub   si, ax
     mov   ax, word ptr px
     sub   si, ax

     mov   ax, y
     shl   ax, 8
     add   ax, x
     add   di, ax

     mov   dh, byte ptr ht
     mov   bx, word ptr tw
     mov   cx, bx
   }
   loopx:
   asm {
     mov  al,  [si]
     cmp  al,  0ffh
     je   skipmask
     mov  es:[di], al
   }
   skipmask:
   asm {
     inc  di
     dec  si
     dec  cx
     jnz  loopx
     add  di, 256
     sub  di, bx
     sub  si, word ptr wid
     add  si, bx
     mov  cx, bx
     dec  dh
     jnz  loopx
     pop  ds
   }
}

void HFlip(char far *src,signed  x,signed  y,signed  wid,signed  ht) {
   asm {
     mov  ax, word ptr x
     cmp  ax, ClipX2
     jg   nodisplay

     add  ax, word ptr wid
     dec  ax
     cmp  ax, ClipX1
     jl   nodisplay

     mov  ax, word ptr y
     cmp  ax, ClipY2
     jg   nodisplay

     add  ax, word ptr ht
     dec  ax
     cmp  ax, ClipY1
     jl   nodisplay
     jmp  clip
   }
   nodisplay:  return;
   clip:

   signed  tw=wid;
   int px,py;
   px=0;
   py=0;
   if (x < ClipX1) {
      px = ClipX1-x;
      tw = wid - px;
      x  = ClipX1;
   }
   if ((x+tw-1) > ClipX2) tw = ClipX2-x+1;
   if (y < ClipY1) {
      py = ClipY1-y;
      ht = ht - py;
      y  = ClipY1;
   }
   if ((y+ht-1) > ClipY2) ht = ClipY2-y+1;

   asm {
     push  ds

     les   di, Destination
     lds   si, src

     mov   ax, word ptr py
     mul   word ptr wid
     add   ax, wid
     sub   ax, word ptr px
     add   si, ax
     dec   si

     mov   ax, y
     shl   ax, 8
     add   ax, x
     add   di, ax

     mov   dh, byte ptr ht
     mov   bx, word ptr tw
     mov   cx, bx
   }
   loopx:
   asm {
     mov  al,  [si]
     cmp  al,  0ffh
     je   skipmask
     mov  es:[di], al
   }
   skipmask:
   asm {
     inc  di
     dec  si
     dec  cx
     jnz  loopx
     add  di, 256
     sub  di, bx
     add  si, wid
     add  si, bx
     mov   cx, bx
     dec  dh
     jnz  loopx
     pop  ds
   }
}

int InitGraphics() {
  BUFFER=(char far*)farmalloc(0x10000);
  if (!BUFFER) return 0;
  asm {
    mov ax, 13h
    int 10h
  }
  VGAMEM =(char far*)(0xA0000000);
  Destination = VGAMEM;
  ClipX1=0;
  ClipY1=0;
  ClipX2=255;
  ClipY2=255;
  return 1;
}

void DirectMem(int i) {
   if (i) Destination=VGAMEM;
   else Destination=BUFFER;

}

void CloseGraphics() {
  asm {
    mov ax, 3
    int 10h
  }
  farfree(BUFFER);
}