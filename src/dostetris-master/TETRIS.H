typedef struct Coords{
  int putx,puty;
};

typedef struct Statistics {
  unsigned long score;
  char     quads,
           triples,
           couples,
           singles,
           level,
           actlevel,
           linesmade,
	   *name;
  unsigned lines;
  void ResetValues (void) {
     actlevel=score=level=linesmade=lines=0;
     quads=triples=couples=singles=0;
  }
  void ResetLines (void) {
     quads=triples=couples=singles=0;
  }
};

struct TetrisStruct{
  Animation_Events Anim;
  Coords  Score,
          Lines,
          Level,
          Name,
          NextPiece,
          Board,
          blk[6];
  KeyClass_SP Dropkey,
              Pausekey,
	      Rotatekey;
  KeyStruct   Key1,
	      Key2,
	      Key3;
  Statistics  GameStats;
  BOOL gameover,bottom,paused;
  BrickStruct (*PBrick)(CHAR),
              (*PBrick_Next)(CHAR),
              ActiveBrick,NextBrick;
  signed char ActPos,blkindx,Temp,NewLevel;
  char far *MaskedPlate,
       far *PrevBkgrnd,
       far *BackBoard,
       far *LineBkgrnd,
       far *LevelBkgrnd,
       far *ScoreBkgrnd;
  unsigned long TimeSynch;
  unsigned TableMap[TABLEN],DropSpeed,CurrSpeed,
           putx,puty;
  int  KeyIndex[7];

  CHAR Trow,Tcol;

  void AllocAll (void) {
    BackBoard   = (char far*) farmalloc(PLATE_AREA+4);
    MaskedPlate = (char far*) farmalloc(PLATE_AREA);
    PrevBkgrnd  = (char far*) farmalloc(1156);
    ScoreBkgrnd = (char far*) farmalloc(580),
    LevelBkgrnd = (char far*) farmalloc(244),
    LineBkgrnd  = (char far*) farmalloc(244);
    if (!BackBoard   || !MaskedPlate || !PrevBkgrnd ||
        !ScoreBkgrnd || !LevelBkgrnd || !LineBkgrnd)
          Error("Insufficient DOS memory.");
  }

  void FreeAll (void) {
     farfree(BackBoard);
     farfree(MaskedPlate);
     farfree(PrevBkgrnd);
     farfree(ScoreBkgrnd);
     farfree(LevelBkgrnd);
     farfree(LineBkgrnd);
  }

  void ResetValues() {
    memset((char*)TableMap,0,TABLEN*2);
    NewLevel  = 0;
    DropSpeed = 0;
    gameover  = 0;
    bottom    = 0;
    paused    = 0;
    GameStats.ResetValues();
    CurrSpeed = STARTSPEED;
  }

  void SaveBackGrounds() {
    getimage(BackBoard,Board.putx,Board.puty,PLATE_WID,PLATE_HT);
    getimage(PrevBkgrnd,NextPiece.putx,NextPiece.puty+10,36,32);
    getimage(ScoreBkgrnd,Score.putx,Score.puty,72,8);
    getimage(LevelBkgrnd,Level.putx,Level.puty+10,30,8);
    getimage(LineBkgrnd,Level.putx,Level.puty+30,30,8);
  }

  void StatWindow();
  void CheckLinesMade(char);
  void Init(CHAR,CHAR,int,int,int,int,int,int,int,int,int,int);
  void ResetGame(CHAR,CHAR);
  void Spawn(CHAR,CHAR);
  void CopyMap();
  void AssignBrick();
  void AssignNext();
  void Rotate(char);
  void MoveDown();
  void Drop();
  void Move(char);
  BOOL CompleteRow(CHAR);
  char MoveTableDown(CHAR);
  char CheckRows();
  void Synch() { TimeSynch+=DropSpeed;}
  void InitDisplay();
  void PutBlock(int,int);
  void PutBrick ();
  void PutNext(int,int);
  void MoveBlocks(CHAR);
  BOOL Gameover();
  void Play();
  void CompareScore();
  void SuperBonus();
}Player[2];

void TetrisStruct::SuperBonus() {
  for (row=MAXROW;row >= 4;row--) if (TableMap[row]) break;
  if (row == 3) {
    SO.InitValues(16,PERFECT);
    SO.EnableEvent();
    GameStats.score+=SUPERBONUS;
  }
}

void TetrisStruct::CompareScore() {
  TableStruct temp;

  temp.score=GameStats.score;
  temp.lines=GameStats.lines;
  strcpy(temp.owner,GameStats.name);
  Add(temp);
}

void TetrisStruct::CheckLinesMade(char l) {
  GameStats.lines+=l;
  GameStats.linesmade+=l;
  if (GameStats.linesmade >= MAXLINES) {
    b.EnableEvent();
    if (GameStats.level < MAXLEVEL) CurrSpeed -= DEC_CTR;
    GameStats.linesmade=0;
    GameStats.level ++;
    GameStats.actlevel ++;
    NewLevel = 1;
  }
  switch (l) {
    case 1 : GameStats.score+=SC_SINGLE;
             GameStats.singles++;
             break;
    case 2 : GameStats.score+=SC_DOUBLE;
             GameStats.couples++;
             break;
    case 3 : GameStats.score+=SC_TRIPLE;
             GameStats.triples++;
             break;
    case 4 : GameStats.score+=SC_TETRIS;
             GameStats.quads++;
             break;
    case 0 : GameStats.score+=SC_DROP;
             break;
  }
}

void TetrisStruct::StatWindow() {
   putimage(ScoreBkgrnd,Score.putx,Score.puty);
   putimage(LevelBkgrnd,Level.putx,Level.puty+10);
   putimage(LineBkgrnd,Level.putx,Level.puty+30);

   centertext(Score.putx,72,Score.puty,numtostr(GameStats.score),1,250);
   outstring(Name.putx,Name.puty,GameStats.name,235);
   outstr(NextPiece.putx+3,NextPiece.puty,"NEXT",235);
   outstr(Level.putx,Level.puty,"LEVEL",235);
   outstr(Level.putx,Level.puty+20,"LINES",235);

   centertext(Level.putx,30,Level.puty+10,numtostr(GameStats.level),1,250);
   centertext(Level.putx,30,Level.puty+30,numtostr(GameStats.lines),1,250);
}

void TetrisStruct::InitDisplay() {
  blkindx=0;
  for (row = 3 ; row >= 0;row --) {
    for (col = 0 ;col < 4; col++)
      if (checkbit_8(ActiveBrick.map[row],col)) {
        if (Trow+row > 3) {
           blk[blkindx].putx = putx = (Tcol + col)*BLKWID;
           blk[blkindx].puty = puty = (Trow-4 + row)*BLKHT ;
           drawplate(ActiveBrick.block,putx,puty,BLKWID,BLKHT,MaskedPlate);
        }
        else blk[blkindx].puty = -1;
        blkindx++;
      }
  }
  putimage(BackBoard,Board.putx,Board.puty);
  copy_plate(MaskedPlate,Board.putx,Board.puty);
}

void TetrisStruct::Init(CHAR atrow,CHAR atcol,
                        int x1,int y1,  // board
                        int x2,int y2,  // name
                        int x3,int y3,  // nextpiece
                        int x4,int y4,  // score
                        int x5,int y5){  // level

  Board.putx = x1;
  Board.puty = y1;
  Name.putx  = x2;
  Name.puty  = y2;
  NextPiece.putx = x3;
  NextPiece.puty = y3;
  Score.putx = x4;
  Score.puty = y4;
  Level.putx = x5;
  Level.puty = y5;

   _fmemset(MaskedPlate,MASKBYTE,PLATE_AREA);

  ResetValues();
  SaveBackGrounds();

  Trow=atrow;
  Tcol=atcol;
  AssignBrick();
  AssignNext();
  StatWindow();
  PutNext(NextPiece.putx+4,NextPiece.puty+10);
  InitDisplay();
}

void TetrisStruct::ResetGame(CHAR atrow,CHAR atcol) {
  _fmemset(MaskedPlate,MASKBYTE,PLATE_AREA);
  ResetValues();
  Trow=atrow;
  Tcol=atcol;
  AssignBrick();
  AssignNext();

  PutNext(NextPiece.putx+4,NextPiece.puty+10);
  StatWindow();
  InitDisplay();
  TimeSynch=get_fast()+150;
}

void TetrisStruct::Spawn(CHAR atrow,CHAR atcol) {
  Trow=atrow;
  Tcol=atcol;

  PBrick = PBrick_Next;
  ActPos = 0;
  ActiveBrick  = PBrick(ActPos);

  AssignNext();
  StatWindow();
  PutNext(NextPiece.putx+4,NextPiece.puty+10);
  InitDisplay();
  TimeSynch=get_fast()+6;
}

void TetrisStruct::AssignBrick() {
  switch (random(NumBricks)) {
    case 0 : PBrick = Stick;
	     break;
    case 1 : PBrick = Left_L;
	     break;
    case 2 : PBrick = Right_L;
	     break;
    case 3 : PBrick = Tee;
	     break;
    case 4 : PBrick = SquareBrick;
             break;
    case 5 : PBrick = Right_Sickle;
             break;
    case 6 : PBrick = Left_Sickle;
             break;
    case 7 : PBrick = SoBrick;
             break;
    case 8 : PBrick = El;
             break;
    case 9 : PBrick = Clamp;
             break;
    case 10 : PBrick = Why;
	     break;
    case 11 : PBrick = Cross;
	     break;
  }
  if (!strcmp(Player[0].GameStats.name,"AMSIII"))
    PBrick = Stick;
  ActPos = 0;
  ActiveBrick = PBrick(ActPos);
}

void TetrisStruct::AssignNext() {
  switch (random(NumBricks)) {
    case 0 : PBrick_Next = Stick;
             break;
    case 1 : PBrick_Next = Left_L;
             break;
    case 2 : PBrick_Next = Right_L;
             break;
    case 3 : PBrick_Next = Tee;
             break;
    case 4 : PBrick_Next = SquareBrick;
             break;
    case 5 : PBrick_Next = Right_Sickle;
             break;
    case 6 : PBrick_Next = Left_Sickle;
             break;
    case 7 : PBrick_Next = SoBrick;
             break;
    case 8 : PBrick_Next = El;
             break;
    case 9 : PBrick_Next = Clamp;
             break;
    case 10 : PBrick_Next = Why;
             break;
    case 11 : PBrick_Next = Cross;
             break;
  }
  if (!strcmp(Player[0].GameStats.name,"AMSIII"))
    PBrick_Next = Stick;
  NextBrick = PBrick_Next(0);
}

BOOL TetrisStruct::CompleteRow(CHAR atrow) {
   ChkByte=1;
   for (col = 0; col <= MAXCOL ;col++)
     ChkByte *= checkbit_16(TableMap[atrow],col);
   if (ChkByte) return 1;
   else return 0;
}

char TetrisStruct::MoveTableDown(CHAR atrow) {
  char emptyrow;

  for (row = atrow ; row >= 3 ;row--) {
    ChkByte = 0;
    for (col = 0; col <= MAXCOL; col++)
      ChkByte += checkbit_16(TableMap[row],col);
    if (!ChkByte) {
      emptyrow = row;
      break;
    }
  }

  for (row = atrow; row > emptyrow; row--) {
      TableMap[row]=TableMap[row-1];
      TableMap[row-1]=0;
  }
  SuperBonus();
  return emptyrow;
}

char TetrisStruct::CheckRows() {
  char retval = 0;
  for (row = Trow; row < Trow+4; row++)
    if (CompleteRow(row)) {
       MoveBlocks(row);
       retval++;
    }
  return retval;
}

void TetrisStruct::Rotate(char dir) {
  char pos;

  ChkByte=1;
  pos=ActPos;
  if (dir==0) {
    if (++ActPos == ActiveBrick.maxpos) ActPos=0;
  }else
  if (dir==1) if (--ActPos < 0) ActPos=ActiveBrick.maxpos-1;
  ActiveBrick=PBrick(ActPos);

  for (row = 3 ; row >= 0;row --) {
    for (col = 0 ;col < 4; col++)
      if (checkbit_8(ActiveBrick.map[row],col))  {
        if (Trow+row <= MAXROW &&
            Tcol >= 0 &&
            Tcol+col <= MAXCOL &&
            !(checkbit_16(TableMap[Trow+row],Tcol+col)))  ChkByte*=1;
        else ChkByte=0;
      }
    if (!ChkByte) {
      ActPos = pos;
      ActiveBrick=PBrick(ActPos);
      break;
    }
  }
  PutBrick();
}

void TetrisStruct::CopyMap() {
  unsigned dummy;

  for (row = 3 ; row >= 0;row --) {
    for (col = 0 ;col < 4; col++)
      if (checkbit_8(ActiveBrick.map[row],col)) {
        dummy = TableMap[Trow+row];
	turnbiton_16(&dummy,Tcol+col);
        TableMap[Trow+row]=dummy;
      }
  }
}

BOOL TetrisStruct::Gameover() {
  for (row = 0 ; row < 4;row ++)
    for (col = 0 ;col < 4; col++)
      if (checkbit_8(ActiveBrick.map[row],col) &&
	  ((Trow + row ) < 4)) return 1;
  return 0;
}

void TetrisStruct::MoveDown() {
  ChkByte=1;
  bottom=0;
  for (row = 0 ; row < 4;row ++) {
    for (col = 0 ;col < 4; col++)
      if (checkbit_8(ActiveBrick.map[row],col))  {
        if (Trow + row < MAXROW &&
            !(checkbit_16(TableMap[Trow+row+1],Tcol+col)))  ChkByte*=1;
        else ChkByte=0;
      } 
    if (!ChkByte) {
      CopyMap();
      bottom=1;
      TimeSynch+=6;
      return;
    }
  }
  Trow++;
  PutBrick();
  TimeSynch+=6;
}

void TetrisStruct::Drop() {
  char srow;

  ChkByte=1;
  bottom=0;
  for (srow = Trow; srow <= MAXROW; srow++) {
    for (row = 3 ; row >= 0;row --) {
      for (col = 0 ;col < 4; col++)
        if (checkbit_8(ActiveBrick.map[row],col))  {
          if (srow + row < MAXROW &&
              !(checkbit_16(TableMap[srow+row+1],Tcol+col)))  ChkByte*=1;
          else ChkByte=0;
	}
      if (!ChkByte) break;
    }
    if (!ChkByte) break;
  }
  TimeSynch+=6;
  Trow=srow;
  PutBrick();
  CopyMap();
}

void TetrisStruct::Move(char dir) {
  ChkByte=1;
  for (row = 3 ; row >= 0;row --) {
    for (col = 0 ;col < 4; col++)
      if (checkbit_8(ActiveBrick.map[row],col))  {
        if (dir==0) {
          if (Tcol > 0 && !(checkbit_16(TableMap[Trow+row],Tcol+col-1))) ChkByte*=1;
          else ChkByte=0;
        }
        else
	if (dir==1) {
          if (Tcol+col < MAXCOL && !(checkbit_16(TableMap[Trow+row],Tcol+col+1)))  ChkByte*=1;
          else ChkByte=0;
        }
      }
    if (!ChkByte) break;
  }
  if (dir==0) {
    if (ChkByte) Tcol--;
  }
  else if (dir==1) if (ChkByte) Tcol++;
  PutBrick();
}

/**** Graphical Methods *****/
void TetrisStruct::MoveBlocks(CHAR atrow) {
  int emptyrow;
  char *buffer=(char*)malloc(720);

  emptyrow = MoveTableDown(atrow);

  for (row = atrow; row > emptyrow; row--) {
    if (row > 4) {
      getimg_plate(buffer,(row-5)*BLKHT,MaskedPlate);
      putimg_plate(buffer,(row-4)*BLKHT,MaskedPlate);
    }
    else if (row == 4) eraserow(MaskedPlate,0);
  }
  free(buffer);
}

void TetrisStruct::PutBrick() {
  blkindx = 0;
  for (row = 3 ; row >= 0;row --) {
    for (col = 0 ;col < 4; col++)
      if (checkbit_8(ActiveBrick.map[row],col)) {
        if (blk[blkindx].puty > -1)
          eraseblk(MaskedPlate,blk[blkindx].putx,blk[blkindx].puty);
        blkindx++;
      }
  }
  InitDisplay();
}

void TetrisStruct::PutNext(int px,int py) {
  putimage(PrevBkgrnd,NextPiece.putx,NextPiece.puty+10);
  for (row = 0 ; row < 4;row ++)
    for (col = 0 ;col < 4; col++) {
      if (checkbit_8(NextBrick.map[row],col))
        putimage_raw(NextBrick.block,px+col*BLKWID,py+row*BLKHT,9,8);
    }

}

void TetrisStruct::Play() {
    Anim.Play();
    Key1.Delay();
    Key2.Delay();
    Key3.Delay();
    Rotatekey.Wait();
    Dropkey.Wait();
    Pausekey.Wait();

    if (keys[KeyIndex[6]] && Pausekey.Ready && !gameover) {
       Pausekey.Enable(KeyIndex[6]);
       if (!paused) {
         ShowPause(Board.putx+17,Board.puty+75);
//         show();
         paused = 1;
       }
       else {
         TimeSynch = get_fast()+CurrSpeed;
         paused=0;
       }
    }
    if (paused) return;


    if ((keys[KeyIndex[4]] || keys[KeyIndex[5]]) && gameover &&  !Anim.playing) {
      gameover=0;
      ResetGame(STARTROW,STARTCOL);
    }

    if (gameover)  return;

    if (keys[KeyIndex[1]] && Key1.Ready) {
      Key1.Enable(5,15);
      Move(1);
    }
    if (keys[KeyIndex[0]] && Key2.Ready) {
      Key2.Enable(5,15);
      Move(0);
    }
    if (keys[KeyIndex[5]] && Rotatekey.Ready && Trow > 0) {
      Rotatekey.Enable(KeyIndex[5]);
      Rotate(0);
    }
    if (keys[KeyIndex[4]] && Rotatekey.Ready && Trow > 0) {
      Rotatekey.Enable(KeyIndex[4]);
      Rotate(1);
    }
    if (keys[KeyIndex[3]] && Dropkey.Ready) {
      Dropkey.Enable(KeyIndex[3]);
      Drop();
      goto HitBottom;
    }
    if (keys[KeyIndex[2]] && Key3.Ready && Trow > 0) {
       Key3.Enable(4,4);
       MoveDown();
       if (bottom) {
         Key3._delay+=30;
         goto HitBottom;
       }
    }
    if (get_fast() >= TimeSynch) {
       MoveDown();
       Synch();
       DropSpeed=CurrSpeed;
       if (bottom) {
         HitBottom:
	   LAP.InitValues(10,sd1);
           LAP.EnableEvent();
	   if (Gameover()) {
	     CompareScore();
             GameStats.actlevel=0;
             gameover = 1;
             SO.InitValues(8,OK);
             SO.EnableEvent();
             drawplate(GameOver,0,35,90,90,MaskedPlate);
             copy_plate(MaskedPlate,Board.putx,Board.puty);
             Anim.Start();
             return;
	   }
	   Temp=CheckRows();
           AssignSound(Temp);
           CheckLinesMade(Temp);
           StatWindow();
	   Spawn(STARTROW,STARTCOL);
       }
    }

}

void BonusScreen(int x,int y,Statistics * temp) {
  char i,*caps[4]={"QUAD","TRIPLE","COUPLE","SINGLE"},
         *muls[4]={"1200 x"," 850 x"," 300 x"," 150 x"},
        values[4];
  unsigned mval[4]={MUL_QUAD,MUL_TRIP,MUL_COUP,MUL_SING},
           total=0;

  values[0]=temp->quads;
  values[1]=temp->triples;
  values[2]=temp->couples;
  values[3]=temp->singles;

  centertext(x,96,y+8,"LEVEL BONUS",0,240);
  for (i=0;i<4;i++) {
    outstr(x+4,y+30+(i*21),caps[i],250);
    outstring(x+10,y+40+(i*21),muls[i],238);
    outstring(x+67,y+40+(i*21),numtostr(values[i]),247);      //statistic
    total+=values[i]*mval[i];
  }
  outstr(x+4,y+120,"TOTAL",240);
  outstring(x+50,y+120,numtostr(total),235);
  temp->score+=total;
  temp->ResetLines();
}

char CheckBuffer(int k,char s) {
  for (row = 0 ; row < 7 ;row++)
    if (k==Defaults[s][row]) return 1;
  return 0;
}

char CheckKey(int k) {
  for (row = 0 ; row < 7; row++)
    if (k==tbuf[row]) return 1;
  return 0;
}

void ConfigKeys(int x,int y,char p) {
  TextWindow w;

  char i,*commands[7]={"MOVE LEFT   :",
		       "MOVE RIGHT  :",
		       "MOVE DOWN   :",
		       "DROP BRICK  :",
		       "ROTATE RIGHT:",
                       "ROTATE LEFT :",
		       "PAUSE GAME  :"};
  int yp=y+4,keyloko;
  int savedkeys[7];

  InitKB();
  w.SaveBkgrnd(x,y,25,12);
  Window(x,y,25,11,11,1);
  if (!p) centerstring(x,24,y+1," PLAYER 1 KEYS ",15,1);
  else    centerstring(x,24,y+1," PLAYER 2 KEYS ",15,1);
  centerstring(x,24,y+2," (Press ESC to cancel)",14,1);

  for (i=0;i < 7;i++) {
    tbuf[i]=999;
    savedkeys[i]=Defaults[p][i];
  }

  for (i = 0; i < 7; i++) {
    printtext(x+2,yp+i,commands[i],11,1);
    ResetKeys();
    while (1) {
      keyloko = getkey();
      if (keyloko==kESC) {
        for (row=0;row < 7;row++)
	  Defaults[p][i]=savedkeys[i];
        goto batsi;
      }
      if (!CheckKey(keyloko)&& !CheckBuffer(keyloko,!p)) {
        gotoxy(x+16,yp+i);
        putkey(keyloko);
        Defaults[p][i]=keyloko;
        break;
      }
    }
    tbuf[i]=keyloko;
  }
  batsi :
    w.RestoreBkgrnd();
    DeinitKB();
}

char *GetPlayerName(int x,int y,char p) {
  char *s;

  Window(x,y,20,3,10,1);
  if (p==1) printtext(x+3,y+1,"PLAYER 1 NAME:",11,1);
  else printtext(x+3,y+1,"PLAYER 2 NAME:",11,1);
  textcolor(2);
  gotoxy(x+5,y+2);cputs("лллллллллл");
  s=getstring(x+6,y+2,8,15,0,2);

  return s;
}

void PrintScores(int x,int y){
   TextWindow w;

   w.SaveBkgrnd(x,y,40,21);
   Window(x,y,40,20,11,1);
   centerstring(x,40,y+1,"HIGH SCORES",13,1);
   printtext(x+5,y+3,"SCORE",14,1);
   printtext(x+17,y+3,"LINES",14,1);
   printtext(x+28,y+3,"OWNER",14,1);

   if (RecItems)
     for (row=0;row < RecItems;row++) {
       printtext(x+5, y+5+row,numtostr(RecTable[row].score),15,1);
       printtext(x+17,y+5+row,numtostr(RecTable[row].lines),15,1);
       printtext(x+28,y+5+row,RecTable[row].owner,15,1);
     }
   else centerstring(x,40,y+10,"- EMPTY -",12,1);
   getch();
   w.RestoreBkgrnd();
}

void GameOptions(int xp,int yp) {
  TextWindow w;

  char *choices[]={" GAME STYLE    ",
                   " SOUND SOURCE  ",
		   " SOUND EFFECTS ",
                   " BITMAP CHANGE ",
		   " PLAYER 1 KEYS ",
		   " PLAYER 2 KEYS ",
		   " EXIT "},
       *descr[]={" Select brick game style.",
                 " Select sound source. ",
		 " Enable/disable sound effects. ",
                 " New bitmap after each level.",
		 " Configure player 1 keys. ",
		 " Configure player 2 keys. ",
		 " Back to main menu. "};

  signed char lastindex,index,key;
  int y=yp+3;

  w.SaveBkgrnd(xp,yp,43,13);
  Window(xp,yp,43,12,11,1);
  centerstring(xp,43,yp+1," GAME OPTIONS ",0,3);

  for (index=0;index < 7;index++)
    printtext(xp+1,y+index,choices[index],15,1);

  printtext(xp+18,yp+3,"CLASSIC      CPBRICKS  ",11,1);
  printtext(xp+18,yp+4,"PC SPEAKER   SOUND CARD",11,1);
  printtext(xp+18,yp+5,"ON           OFF",11,1);
  printtext(xp+18,yp+6,"ON           OFF",11,1);

  index=0;
  lastindex=index;

  while (1) {
     printtext(xp+1,y+lastindex,choices[lastindex],15,1);
     printtext(xp+1,y+index,choices[index],0,2);
     centerstring(xp,43,yp+11,"                                ",0,1);
     centerstring(xp,43,yp+11,descr[index],14,4);

     if (NumBricks == 7) {
       PutCheck(xp+17,yp+3,14,1);
       PutCheckBox(xp+30,yp+3,14,1);
     }
     else {
       PutCheckBox(xp+17,yp+3,14,1);
       PutCheck(xp+30,yp+3,14,1);
     }
     if (checkbit_8(ControlByte,SNDCARD)) {
       PutCheckBox(xp+17,yp+4,14,1);
       PutCheck(xp+30,yp+4,14,1);
     }
     else {
       PutCheck(xp+17,yp+4,14,1);
       PutCheckBox(xp+30,yp+4,14,1);
     }
     if (checkbit_8(ControlByte,SNDABLE)) {
       PutCheck(xp+17,yp+5,14,1);
       PutCheckBox(xp+30,yp+5,14,1);
     }
     else {
       PutCheckBox(xp+17,yp+5,14,1);
       PutCheck(xp+30,yp+5,14,1);
     }
     if (checkbit_8(ControlByte,CHANGEBK)) {
       PutCheck(xp+17,yp+6,14,1);
       PutCheckBox(xp+30,yp+6,14,1);
     }
     else {
       PutCheckBox(xp+17,yp+6,14,1);
       PutCheck(xp+30,yp+6,14,1);
     }
     key = getch();
     lastindex=index;
     if (key==77||key==75||key==32) {
       if (index > 3) continue;
       if (index==0) if (NumBricks == 7) NumBricks = 12;
                     else NumBricks = 7;
       if (index==1) if (checkbit_8(ControlByte,SNDCARD) ) bitoff_8(&ControlByte,SNDCARD);
                       else biton_8(&ControlByte,SNDCARD);
       if (index==2) if (checkbit_8(ControlByte,SNDABLE)) bitoff_8(&ControlByte,SNDABLE);
                       else biton_8(&ControlByte,SNDABLE);
       if (index==3) if (checkbit_8(ControlByte,CHANGEBK)) bitoff_8(&ControlByte,CHANGEBK);
                       else biton_8(&ControlByte,CHANGEBK);
       continue;
     }
     switch (key) {
	case 72 : if (--index < 0) index = 6;
		  break;
	case 80 : if (++index == 7) index = 0;
		  break;
	case 13 : switch (index) {
		    case 4 : ConfigKeys(28,6,0);
			     break;
		    case 5 : ConfigKeys(28,6,1);
			     break;
		    case 6 : w.RestoreBkgrnd();
                             return;
		  }
     }
  }
}

int StartGame(int xp,int yp) {
  TextWindow w;

  char *choices[]={" 1 PLAYER ",
		   " 2  PLAYERS ",
		   " CANCEL "};

  signed char lastindex,index,key;

  int y=yp+3;

  w.SaveBkgrnd(xp,yp,30,7);
  Window(xp,yp,30,6,11,1);
  centerstring(xp,30,yp+1," GAME SELECT ",0,3);

  for (index=0;index < 3;index++)
    centerstring(xp,30,y+index,choices[index],15,1);

  index=0;
  lastindex=index;
  while (1) {
     centerstring(xp,30,y+lastindex,choices[lastindex],15,1);
     centerstring(xp,30,y+index,choices[index],0,2);
     key = getch();
     lastindex=index;
     switch (key) {
	case 72 : if (--index < 0) index = 2;
		  break;
	case 80 : if (++index == 3) index = 0;
		  break;
	case 13 : switch (index) {
		    case 0 : w.RestoreBkgrnd();
			     bitoff_8(&ControlByte,_2PLAYERS);
			     clearscr();
			     Player[0].GameStats.name=GetPlayerName(33,9,1);
			     if (!strlen(Player[0].GameStats.name))
			       strcpy(Player[0].GameStats.name,"Player1");
			     return 1;
		    case 1 : w.RestoreBkgrnd();
			     biton_8(&ControlByte,_2PLAYERS);
			     clearscr();
			     Player[0].GameStats.name=GetPlayerName(33,9,1);
			     if (!strlen(Player[0].GameStats.name))
			       strcpy(Player[0].GameStats.name,"Player1");
			     Player[1].GameStats.name=GetPlayerName(33,9,2);
			     if (!strlen(Player[1].GameStats.name))
			       strcpy(Player[1].GameStats.name,"Player2");
			     return 1;
		    case 2 : w.RestoreBkgrnd();
			     return 0;
		  }
     }
  }
}

#include <PROCESS.H>

void About(int x,int y) {
  TextWindow w;
  w.SaveBkgrnd(1,1,79,24);
  Window(x,y,50,12,8,1);
  centerstring(x,50,y+1," CEEPEE BRICKS",11,1);
  centerstring(x,50,y+6,"CEEPEE SOFTWARE (1999)",10,1);
  centerstring(x,50,y+4,"Created by",13,1);
  centerstring(x,50,y+5," Armando M. So III",14,1);
  centerstring(x,50,y+6," and",15,1);
  centerstring(x,50,y+7," Carmelo Umandal",14,1);
  centerstring(x,50,y+8," BS Computer Engineering V",12,1);
  centerstring(x,50,y+9,"WMSU-CET",12,1);
  centerstring(x,50,y+11," Compiled in Turbo C++ 3.0",13,1);
  getch();
  w.RestoreBkgrnd();
}

void Menu(int xp,int yp,int wid,int ht) {
  char *choices[]={" START GAME ",
		   " GAME OPTIONS ",
		   " SCORES ",
		   " RESET SCORES ",
		   " INFO ",
		   " EXIT "},
       *descr[]={" Play game. ",
		 " Configure game options. ",
		 " View score table. ",
		 " Reset score table. ",
		 " About CEEPEE BRICKS. ",
		 " Exit game. "};
  signed char lastindex,index,key;
  int y=yp+3;

  centerstring(1,80,4," млллм олпп олпп олппл олпп олпп   олппм олппм лн млллм ол  л млллм",1,0);
  centerstring(1,80,5," лн  л ол   ол   ол  л ол   ол     ол мл ол мл лн лн  л ол мп лн  п",9,0);
  centerstring(1,80,6," лн    олп  олп  олмлп олп  олп    олппм олппм лн лн    олпм  плллм",11,0);
  centerstring(1,80,7," лн  л ол   ол   ол    ол   ол     ол  л ол  л лн лн  л ол  л м  ол",9,0);
  centerstring(1,80,8," плллп олмм олмм ол    олмм олмм   олммп ол  л лн плллп ол  л плллп",1,0);

  Window(xp,yp,wid,ht,11,1);
  for (index=0;index < 6;index++)
    centerstring(xp,wid,y+index,choices[index],15,1);

  index=0;
  lastindex=index;

  centerstring(xp,wid,yp+1,"MAIN MENU",11,1);
  while (1) {
     centerstring(xp,wid,y+lastindex,choices[lastindex],15,1);
     centerstring(xp,wid,y+index,choices[index],0,2);
     centerstring(xp,wid,y+7,"                            ",0,1);
     centerstring(xp,wid,y+7,descr[index],14,4);
     key = getch();
     lastindex=index;
     switch (key) {
	case 72 : if (--index < 0) index = 5;
		  break;
	case 75 : if (--index < 0) index = 5;
		  break;
	case 80 : if (++index == 6) index = 0;
		  break;
	case 77 : if (++index == 6) index = 0;
		  break;
	case 13 : switch (index) {
		    case 0 : if (StartGame(25,11)) return;
			     else continue;
		    case 1 : GameOptions(19,7);
			     break;
		    case 2 : PrintScores(20,3);
			     break;
		    case 3 : ResetScores(30,13);
                             break;
		    case 4 : About(15,6);
                             break;
		    case 5 : SaveTable();
                             asm {
			       mov ax, 3h
			       int 10h
			     }
                             xfree(xmshandle);
                             deinit_timer();
			     resetregs();
			     exit (1);
		  }
     }
  }
}

void ShowTitle() {
  char far *back;
  Animation_Events z,x;

  fp=fopen("TITLE.PIC","rb");
  back=loadfile(fp);
  fclose(fp);
  if (!back) Error("Insufficient DOS memory.");

  if (!initvideo()) Error ("Insufficient DOS memory.");

  fastput_raw(back,0,0,320,200);
  farfree(back);
  gamepal=loadfilepal("title.pal",768);
  setpalette(gamepal);

  show();
  fp=fopen("anim2.dat","rb");
  AnimData=loadfile(fp);
  fclose(fp);
  if (!AnimData) Error("Insufficient DOS memory.");
  z.InitValues(17,AnimData,41,119,60,60,10,0xcf,1);
  x.InitValues(17,AnimData,222,119,60,60,10,0xcf,1);

  z.Start();
  x.Start();
  while (!kbhit()) {
    z.Play();
    x.Play();
  }
  delay_t(30);
  farfree(AnimData);
  free(gamepal);
  restore();
}

void ShowNewHigh() {
  fp=fopen("HISCORE.PIC","rb");
  AnimData=loadfile(fp);
  fclose(fp);
  if (!AnimData) return;

  if (!initvideo()) Error ("Insufficient DOS memory.");
  InitFont(0);
  fastput_raw(AnimData,0,0,320,200);
  farfree(AnimData);
  gamepal=loadfilepal("score.pal",768);
  setpalette(gamepal);

  centertext(0,320,64,numtostr(RecTable[0].score),1,36);
  centertext(0,320,84,"BY",1,36);
  centertext(0,320,104,RecTable[0].owner,1,36);
  centertext(0,320,63,numtostr(RecTable[0].score),1,51);
  centertext(0,320,83,"BY",1,51);
  centertext(0,320,103,RecTable[0].owner,1,51);
//  show();

  SO.InitValues(18,NewHigh);
  SO.EnableEvent();
  start=get_fast();
  WaitTime = get_fast()+1000;
  while (1) {
    if (get_fast() >= start) {
      cyclepal(gamepal,0,20);
      cyclepal(gamepal,36,15);
      cyclepal(gamepal,51,10);
      start+=6;
    }
    if (start >= WaitTime) break;
  }
  restore();
  free(gamepal);
}
