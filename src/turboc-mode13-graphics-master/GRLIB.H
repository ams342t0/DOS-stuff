#ifndef __GRLIB_H
#define __GRLIB_H


#define FONTCHAR   0x20

extern int ClipX1,ClipY1,ClipX2,ClipY2;
extern char far *VGAMEM,far *BUFFER,PALETTE[768],far *Destination;
extern char far* LoadFileData(const char *);
extern void LoadPalette(const char *);
extern void DirectMem(int);
extern void PutBitmap(char far*,int,int,int,int);
extern void HFlip(char far*,int,int,int,int);
extern void VFlip(char far*,int,int,int,int);
extern void HVFlip(char far*,int,int,int,int);
extern void DrawTile(char far*,int,int,int,int);
extern void HorFlip(char far*,char far*,int,int,int,int,char);
extern void PutBitmapPlane(char far*,char far*,int,int,int,int);
extern void Blitz();
extern void SetPalette();
extern int InitGraphics();
extern void CloseGraphics();
extern "C" void ShowFrame();
extern "C" void ClrScreen(unsigned long);

#ifndef __DOS_H
 #include <dos.h>
#endif
void TweakVideo() {
  char v;
  // enable write mode to ctr control registers 00h-07h, clear bit 7
  outportb(0x3d4,0x11);
  v=inportb(0x3d5)&0x7f;
  outportb(0x3d4,0x11);
  outportb(0x3d5,v);

  outportb(0x03c2,0xe3);  //

  outport(0x03d4,0x5f00);
  outport(0x03d4,0x3f01);
  outport(0x03d4,0x4002);
  outport(0x03d4,0x8203);
  outport(0x03d4,0x4a04);
  outport(0x03d4,0x9a05);
  outport(0x03d4,0x2306);
  outport(0x03d4,0xb207);
  outport(0x03d4,0x0008);
  outport(0x03d4,0x6109);
  outport(0x03d4,0x0a10);
  outport(0x03d4,0xac11);
  outport(0x03d4,0xff12);
  outport(0x03d4,0x2013);
  outport(0x03d4,0x4014);
  outport(0x03d4,0x0715);
  outport(0x03d4,0x1a16);
  outport(0x03d4,0xa317);

  outport(0x03c4,0x0101);
  outport(0x03c4,0x0e04);
  outport(0x03ce,0x4005);
  outport(0x03ce,0x0106);

  inportb(0x3da);
  outportb(0x03c0,0x10|0x20);
  outportb(0x03c0,0x41);
  inportb(0x3da);
  outportb(0x03c0,0x13|0x20);
  outportb(0x03c0,0x00);
}

#endif

