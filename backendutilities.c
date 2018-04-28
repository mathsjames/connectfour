#include "gamestructure.h"

void addtrap(gamestate* pgame,char col, char row, char pm1,char trapnum) {
  (*pgame).traps.histt[trapnum][0]=col;
  (*pgame).traps.histt[trapnum][1]=row;
  (*pgame).traps.all[col][row][pm1]=1;
  (*pgame).traps.active[col][row][pm1]=1;
  //(*pgame).traps.ppcount[pm1][row%2]++;
}


void updatetraps(gamestate* pgame,char player) {
  char pm1=player-1;
  char col=(*pgame).hist.hist[(*pgame).hist.length-1];
  char row=(*pgame).heights[col]-1;
  for (char pl=0; pl<2;pl++){
    (*pgame).traps.active[col][row][pl]=0;
  }
  char trapnum=(*pgame).traps.histc[(*pgame).plys-1];
  if (row>1 && row<5 && (*pgame).state[col][row-1]==player && (*pgame).state[col][row-2]==player) {
    addtrap(pgame,col,row+1,pm1,trapnum);
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
	  addtrap(pgame,ccol,crow,pm1,trapnum);
	  trapnum++;
	}
      }
      if (empf+b>3 && empb<=b) {
	ccol=col-empb;
	crow=row-empb*dirr;
	if ((*pgame).traps.all[ccol][crow][player-1]==0) {
	  addtrap(pgame,ccol,crow,pm1,trapnum);
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
  char pm1=1-((*pgame).plys%2);
  //printf("%d\n",(*pgame).plys);
  for (trap=(*pgame).traps.histc[(*pgame).plys-1];trap<(*pgame).traps.histc[(*pgame).plys];trap++) {
    col=(*pgame).traps.histt[trap][0];
    row=(*pgame).traps.histt[trap][1];
    (*pgame).traps.all[col][row][pm1]=0;
    (*pgame).traps.active[col][row][pm1]=0;
    //(*pgame).traps.ppcount[pm1][row%2]--;
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
