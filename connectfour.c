#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//#include<math.h>

const char MAXDEPTH=12;
const char MAXQUAL=84;

typedef struct movet movet;
struct movet {
  char move;
  char qual;
};

typedef struct history history;
struct history {
  char hist[42];
  char length;
};

typedef struct traps traps;
struct traps {
  char active[7][6][2];
  char all[7][6][2];
  char histc[43];
  char histt[84][2];
};

typedef struct gamestate gamestate;
struct gamestate {
  char state[7][6];
  char heights[7];
  history hist;
  char plys;
  traps traps;
};

double mypow(double a,char b) {
  if (b<0) {
    b=-b;
    a=1/a;
  }
  double out=1;
  while (b!=0) {
    out=out*a;
    b--;
  }
  return out;
}

int hashstate(char** state) {
  
}
/*
char inaline(char state[7][6],char col,char row) {
  char player=state[col][row];
  char cc,cr;
  char len=1;
  //upright
  for (cc=col+1, cr=row+1;cc<7 && cr<6; cc++, cr++) {
    if (state[cc][cr]==player) {
      len++;
    }
    else {
      break;
    }
  }
  for (cc=col-1, cr=row-1;cc>=0 && cr>=0; cc--, cr--) {
    if (state[cc][cr]==player) {
      len++;
    }
    else {
      break;
    }
  }
  if (len>3) {return 1;}
  len=1;
  //downright
  for (cc=col+1, cr=row-1;cc<7 && cr>=0; cc++, cr--) {
    if (state[cc][cr]==player) {
      len++;
    }
    else {
      break;
    }
  }
  for (cc=col-1, cr=row+1;cc>=0 && cr<6; cc--, cr++) {
    if (state[cc][cr]==player) {
      len++;
    }
    else {
      break;
    }
  }
  if (len>3) {return 1;}
  len=1;
  //horizontal
  for (cc=col+1;cc<7; cc++) {
    if (state[cc][row]==player) {
      len++;
    }
    else {
      break;
    }
  }
  for (cc=col-1;cc>=0; cc--) {
    if (state[cc][row]==player) {
      len++;
    }
    else {
      break;
    }
  }
  if (len>3) {return 1;}
  len=1;
  //down
  return (row>2 && state[col][row-1]==player && state[col][row-2]==player && state[col][row-3]==player);
}
*/

void printtraps(traps traps);

void addtrap(gamestate* pgame,char col, char row, char player,char trapnum) {
  (*pgame).traps.histt[trapnum][0]=col;
  (*pgame).traps.histt[trapnum][1]=row;
  (*pgame).traps.all[col][row][player-1]=1;
  (*pgame).traps.active[col][row][player-1]=1;
}


void updatetraps(gamestate* pgame,char player) {
  char col=(*pgame).hist.hist[(*pgame).hist.length-1];
  char row=(*pgame).heights[col]-1;
  for (char pl=0; pl<2;pl++){
    (*pgame).traps.active[col][row][pl]=0;
  }
  char trapnum=(*pgame).traps.histc[(*pgame).plys-1];
  if (row>1 && row<5 && (*pgame).state[col][row-1]==player && (*pgame).state[col][row-2]==player) {
    addtrap(pgame,col,row+1,player,trapnum);
    trapnum++;
  }
  char otherplayer=3-player;
  char dirr;
  char ccol,crow;
  char f,b;
  char empc,empf,empb;
  for (dirr=-1; dirr<2; dirr++) {
    f=0;
    empc=0;
    crow=row+dirr;
    ccol=col+1;
    while (!(f>2 || crow>5 || crow<0  || ccol>6 || ccol<0 || (*pgame).state[ccol][crow]==otherplayer)) {

      if ((*pgame).state[ccol][crow]==0) {
	if (empc) {break;}
	empc=1;
	empf=f+1;
      }
      f++;
      ccol++;
      crow+=dirr;
    }
    if (empc==0) {
      empf=f+1;
    }
    b=0;
    empc=0;
    crow=row-dirr;
    ccol=col-1;
    while (!(b>2 || crow>5 || crow<0  || ccol>6 || ccol<0 || (*pgame).state[ccol][crow]==otherplayer)) {
      if ((*pgame).state[ccol][crow]==0) {
	if (empc) {break;}
	empc=1;
	empb=b+1;
      }
      b++;
      ccol--;
      crow-=dirr;
    }
    if (empc==0) {
      empb=b+1;
    }
    //printf("empb, empf, f, b %d %d %d %d\n",empb,empf,f,b);
    if (f+b>2) {
      if (empb+f>3 && empf<=f) {
	ccol=col+empf;
	crow=row+empf*dirr;
	//printf("%d %d %d\n",ccol,crow, (*pgame).traps.all[ccol][crow]);
	if ((*pgame).traps.all[ccol][crow][player-1]==0) {
	  //printf("%d %d\n",ccol,crow);
	  addtrap(pgame,ccol,crow,player,trapnum);
	  trapnum++;
	}
      }
      if (empf+b>3 && empb<=b) {
	ccol=col-empb;
	crow=row-empb*dirr;
	if ((*pgame).traps.all[ccol][crow][player-1]==0) {
	  addtrap(pgame,ccol,crow,player,trapnum);
	  trapnum++;
	}
      }
    }    
  }
  //printf("%d %d\n",(*pgame).plys, trapnum);
  (*pgame).traps.histc[(*pgame).plys]=trapnum;
}

char makemove(char move, gamestate* pgame) {
  char player=1+(*pgame).plys%2;
  (*pgame).plys++;
  (*pgame).state[move][(*pgame).heights[move]]=player;
  char won=(*pgame).traps.active[move][(*pgame).heights[move]][player-1];
  (*pgame).heights[move]++;
  (*pgame).hist.hist[(*pgame).hist.length]=move;
  (*pgame).hist.length++;
  updatetraps(pgame, player);
  //printstate((*pgame).state);
  return won;// inaline((*pgame).state,move,(*pgame).heights[move]-1); // returns whether or not player has won
}

void reverttraps(gamestate* pgame) {
  char trap, col, row;
  char player=2-((*pgame).plys%2);
  //printf("%d\n",(*pgame).plys);
  for (trap=(*pgame).traps.histc[(*pgame).plys-1];trap<(*pgame).traps.histc[(*pgame).plys];trap++) {
    col=(*pgame).traps.histt[trap][0];
    row=(*pgame).traps.histt[trap][1];
    (*pgame).traps.all[col][row][player-1]=0;
    (*pgame).traps.active[col][row][player-1]=0;
  }
  col=(*pgame).hist.hist[(*pgame).hist.length-1];
  row=(*pgame).heights[col]-1;
  (*pgame).traps.active[col][row][0]=(*pgame).traps.all[col][row][0];
  (*pgame).traps.active[col][row][1]=(*pgame).traps.all[col][row][1];
}

void undomove(gamestate* pgame) {
  reverttraps(pgame);
  (*pgame).hist.length--;
  char move=(*pgame).hist.hist[(*pgame).hist.length];
  (*pgame).heights[move]--;
  (*pgame).state[move][(*pgame).heights[move]]=0;
  (*pgame).plys--;
}

movet findmove(gamestate* pgame, char depth, char a, char b) {
  movet retval;
  char move,col,row,val;
  char qual[7], nextqual;
  char playerm1= (*pgame).plys%2;
  char opm1=1-playerm1;
  retval.move=0;
  retval.qual=-43;
  for (move=0; move<7;move++) {
    row=(*pgame).heights[move];
    if (row<6 && (*pgame).traps.active[move][row][playerm1]) {
      retval.move=move;
      retval.qual=MAXQUAL;
      return retval;
    }
  }
  char offset=rand()%7;
  for (move=0; move<7;move++) {
    row=(*pgame).heights[move];
    if (row<6 && (*pgame).traps.active[move][row][opm1]) {
      offset=move;
    }
  }
  if (depth<MAXDEPTH && (*pgame).plys<42) {
    for (char cmove=0;cmove<7;cmove++) {
      move=(cmove+offset)%7;
      if ((*pgame).heights[move]<6) {
	if (makemove(move,pgame)) {
	  nextqual=MAXQUAL;
	}
	else {
	  nextqual=findmove(pgame,depth+1,-b,-a).qual;
	  nextqual=-nextqual;
	}
	//printtraps((*pgame).traps);
	undomove(pgame);
	if (nextqual>retval.qual) {
	  retval.qual=nextqual;
	  retval.move=move;
	}
	a=(retval.qual>a)?retval.qual:a;

	if (b<=a) {
	  break;
	}
      }
      else {
	qual[move]=-100;
      }
    }
  }
  else if (depth<MAXDEPTH) {
    retval.qual=0;//drawn game
  }
  else {
    retval.qual=0;
    for (col=0;col<7;col++) {
      for (row=0;row<6;row++) {
	retval.qual+=(*pgame).traps.active[col][row][playerm1]-(*pgame).traps.active[col][row][opm1];
      }
    }
    //heuristic value
  }

  
  /*if (depth==0) {
    char probs[7];
    double sum;
    for (move=0;move<7;move++) {
      probs[move]=(qual[move]==retval.qual);
      sum=sum+probs[move];
    }
    double u=sum*rand()/((double) RAND_MAX);
    double tot=0;
    for (move=0;move<7;move++) {
      tot=tot+probs[move];
      if (tot>u) {
	retval.move=move;
	break;
      }
    }
    }*/
  return retval;
}

void printstate(char state[7][6]) {
  char row,col;
  printf("\n1 2 3 4 5 6 7 \n");
  for (row=5;row>=0;row--) {
    for (col=0;col<7;col++) {
      printf("%d ",(int) state[col][row]);
    }
    printf("\n");
  }
}

void printtraps(traps traps) {
  char row,col,val;
  printf("\n1 2 3 4 5 6 7 \n");
  for (row=5;row>=0;row--) {
    for (col=0;col<7;col++) {
      val=0;
      val+=traps.all[col][row][0];
      val+=traps.all[col][row][1]+traps.all[col][row][1];
      printf("%d ",(int) val);
    }
    printf("\n");
  }

  for (char ply=0;ply<43;ply++) {
    printf("%d ",traps.histc[ply]);
  }
  printf("\n");
  for (char ply=0;ply<10;ply++) {
    printf("%d %d   ",traps.histt[ply][0],traps.histt[ply][1]);
  }
  printf("\n");
}


char getmove(char player) {
  printf("Player %d, please enter the column number you want to play in (1 to 7):\n",(int) player);
  char move=getchar()-'1';
  char done=1;
  while (getchar()!='\n') { done=0; }
  if (done && move>-1 && move<7) { return move; }
  printf("Just put a digit from 1 to 7 and press return");
  return getmove(player);
}

int main () {
  gamestate game;
  for (char i=0;i<7;i++) {
    for (char j=0;j<6;j++) {
      for (char k=0;k<2;k++) {
	game.traps.all[i][j][k]=0;
	game.traps.active[i][j][k]=0;
      }
      game.state[i][j]=0;
    }
    game.heights[i]=0;
  }
  game.hist.length=0;
  game.plys=0;
  game.traps.histc[0]=0;
  for (char i=0;i<43;i++) {
    game.traps.histc[i]=0;
  }
  for (char i=0;i<84;i++) {
    game.traps.histt[i][0]=0;
    game.traps.histt[i][1]=0;
  }

  
  char automated[3]={0,0,1};
  char player=1;
  char move;
  srand(time(NULL));

  printstate(game.state);
  while (1) {
    player=1+game.plys%2;
    if (automated[player]==1) {
      //printstate(game.state);
      move=findmove(&game,0,-50,50).move;
      //printstate(game.state);
      //printf("move %d player %d\n",move,player);

    }
    else {
      move=getmove(player);
    }
    char iswon=makemove(move,&game);
    printstate(game.state);
    //printf("heights %d %d %d %d %d %d %d\n",game.heights[0],game.heights[1],game.heights[2],game.heights[3],game.heights[4],game.heights[5],game.heights[6]);
    //printtraps(game.traps);
    if (iswon) {
      printf("Player %d wins.\n",(int) player);
      break;
    }
    else if (game.plys==42) {
      printf("The game is a draw.\n");
      break;
    }
  }
  return 0;
}
