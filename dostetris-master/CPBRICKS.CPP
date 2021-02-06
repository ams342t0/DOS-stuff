extern unsigned _stklen=0x100u;

#include "MISC.H"
#include "TETRIS.H"
#include <process.h>

void main() {
  char playing=1;
  ULONG blitz;

  init_timer();
  LoadTable();
  CheckFiles();
  InitializeGame();
  ShowTitle();
  Repeat :
    clearscr();
    _setcursortype(_NOCURSOR);

    Menu(15,10,50,11);

    fp = fopen("GAME.PAL","rb");
    gamepal=loadfilepal("game.pal",63);
    fclose(fp);
    fp = fopen("ANIM1.DAT","rb");
    AnimData=loadfile(fp);
    fclose(fp);
    fp = fopen("ENDING.PIC","rb");
    GameOver=loadfile(fp);
    fclose(fp);


    Player[0].AllocAll();
    Player[1].AllocAll();

    Player[0].Anim.InitValues(10,AnimData,P1BOARDX+28,P1BOARDY+5,40,68,10,0xc0,0);
    Player[1].Anim.InitValues(10,AnimData,P2BOARDX+28,P2BOARDY+5,40,68,10,0xc0,0);

    if (!initvideo()) Error ("Insufficient DOS memory.");
    InitKB();
    setpalette_part(gamepal,0,235,21);
    free(gamepal);

    BackgrndNum=random(MaxBitmaps);
    changebkground(BackgrndNum);
    show();

    Player[0].Init(STARTROW,STARTCOL,P1BOARDX,P1BOARDY,
		   P1NAMEX,P1NAMEY,P1PIECEX,P1PIECEY,
		   P1SCOREX,P1SCOREY,P1LEVELX,P1LEVELY);
    if (checkbit_8(ControlByte,_2PLAYERS)) {
       Player[1].Init(STARTROW,STARTCOL,P2BOARDX,P2BOARDY,
		      P2NAMEX,P2NAMEY,P2PIECEX,P2PIECEY,
		      P2SCOREX,P2SCOREY,P2LEVELX,P2LEVELY);
    }

    SO.InitValues(24,NEW);
    SO.EnableEvent();

    for (row=0;row < 7;row++) {
      Player[0].KeyIndex[row]=Defaults[0][row];
      Player[1].KeyIndex[row]=Defaults[1][row];
    }

    delay_t(70);

    ResetKeys();

    Player[0].TimeSynch=get_fast()+Player[0].CurrSpeed;
    Player[1].TimeSynch=get_fast()+Player[1].CurrSpeed;

    blitz = get_fast();

    if (!checkbit_8(ControlByte,_2PLAYERS))
	while (1) {
           if (Player[0].gameover && playing) {
             playing=0;
             WaitTime=get_fast()+700;
           }
           if (!Player[0].gameover) playing=1;
           if (get_fast() >= WaitTime && !playing) break;

	   Player[0].Play();
	   if (get_fast() >= blitz) {
	     show();
	     blitz+=6;
	   }

	   if (Player[0].NewLevel) {
               SO.InitValues(24,NEW);
               SO.EnableEvent();
	       Player[0].NewLevel = 0;
	       putimage(Player[0].BackBoard,Player[0].Board.putx,Player[0].Board.puty);
	       BonusScreen(P1BOARDX,P1BOARDY,&(Player[0].GameStats));
	       show();
               delay_t(500);

	       if (checkbit_8(ControlByte,CHANGEBK)) {
		   if (++BackgrndNum == MaxBitmaps) BackgrndNum = 0;
 	           changebkground(BackgrndNum);
	           Player[0].SaveBackGrounds();
                   Player[0].StatWindow();
       	           Player[0].PutNext(P1PIECEX+4,P1PIECEY+10);
                   Player[0].PutBrick();
	       }
	       Player[0].TimeSynch = get_fast()+150;
	   }
	}
    else
       while (1) {
          if (Player[0].gameover && Player[1].gameover && playing) {
              playing=0;
              WaitTime=get_fast()+700;
          }
          if (!Player[0].gameover || !Player[1].gameover) playing=1;
          if (get_fast() >= WaitTime && !playing) break;

	  Player[0].Play();
	  Player[1].Play();
	  if (get_fast() >= blitz) {
	    show();
	    blitz+=6;
	  }
	  if (Player[0].NewLevel && Player[0].GameStats.actlevel > Player[1].GameStats.actlevel ||
              Player[1].NewLevel && Player[1].GameStats.actlevel > Player[0].GameStats.actlevel) {
                 SO.InitValues(24,NEW);
                 SO.EnableEvent();
	         Player[0].NewLevel = 0;
                 Player[1].NewLevel = 0;

		 if (!Player[0].gameover && !Player[0].paused) {
	            putimage(Player[0].BackBoard,Player[0].Board.putx,Player[0].Board.puty);
	            BonusScreen(P1BOARDX,P1BOARDY,&(Player[0].GameStats));
                    Player[0].Trow=0;
                 }
                 if (!Player[1].gameover && !Player[1].paused) {
	            putimage(Player[1].BackBoard,Player[1].Board.putx,Player[1].Board.puty);
	            BonusScreen(P2BOARDX,P2BOARDY,&(Player[1].GameStats));
                    Player[1].Trow=0;
                 }
                 show();
		 delay_t(500);

                 if (checkbit_8(ControlByte,CHANGEBK)) {
                    if (++BackgrndNum == BITMAPS-1) BackgrndNum = 0;
                    changebkground(BackgrndNum);
                    Player[0].SaveBackGrounds();
                    Player[1].SaveBackGrounds();
                    Player[0].PutNext(P1PIECEX+4,P1PIECEY+10);
                    Player[0].PutBrick();
		    Player[1].PutNext(P2PIECEX+4,P2PIECEY+10);
                    Player[1].PutBrick();
                    if (Player[0].paused) ShowPause(P1BOARDX+17,P1BOARDY+75);
                    if (Player[1].paused) ShowPause(P2BOARDX+17,P2BOARDY+75);
                 }
                 Player[1].StatWindow();
                 Player[0].StatWindow();
                 show();
	         if (!Player[0].gameover && !Player[0].paused)
		    Player[0].TimeSynch = get_fast()+150;
                 if (!Player[1].gameover && !Player[1].paused)
		    Player[1].TimeSynch = get_fast()+150;
           }
         }
    DeinitKB();
    restore();
    nosound();
    if (AnimData) farfree(AnimData);
    Player[0].FreeAll();
    Player[1].FreeAll();
    if (RecTable[0].score > GameHighScore) {
       GameHighScore=RecTable[0].score;
       ShowNewHigh();
    }
    goto Repeat;
}





