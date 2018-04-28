#include<stdlib.h>
#include "gamestructure.h"
#include "backendutilities.h"
//#include "frontendutilities.h"
#include "abagent.h"

extern const char MAXQUAL;

movet findabmove(gamestate* pgame, char depth, char a, char b,char maxdepth) {
  movet retval;
  char move,col,row,val;
  char qual[7], nextqual;
  char playerm1= (*pgame).plys%2;
  char opm1=1-playerm1;
  retval.move=0;
  retval.qual=-MAXQUAL-1;
  for (move=0; move<7;move++) {
    row=(*pgame).heights[move];
    if (row<6 && (*pgame).traps.active[move][row][playerm1]) {
      retval.move=move;
      retval.qual=MAXQUAL;
      return retval;
    }
  }
  char offset=rand()%7;
  offset=(*pgame).killers[(*pgame).plys];
  for (move=0; move<7;move++) {
    row=(*pgame).heights[move];
    if (row<6 && (*pgame).traps.active[move][row][opm1]) {
      offset=move;
    }
  }
  if (depth<maxdepth && (*pgame).plys<42) {
    for (char cmove=0;cmove<7;cmove++) {
      move=(cmove+offset)%7;
      if ((*pgame).heights[move]<6) {
	if (makemove(move,pgame)) {
	  nextqual=MAXQUAL;
	}
	else {
	  nextqual=findabmove(pgame,depth+1,-b,-a,maxdepth).qual;
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
	  (*pgame).killers[(*pgame).plys]=move;
	  break;
	}
      }
      else {
	qual[move]=-MAXQUAL-1;
      }
    }
  }
  else if (depth<maxdepth) {
    retval.qual=0;//drawn game
  }
  else {
    //retval.qual=(*pgame).traps.ppcount[playerm1][0]+(*pgame).traps.ppcount[playerm1][1];
    //retval.qual-=(*pgame).traps.ppcount[opm1][0]+(*pgame).traps.ppcount[opm1][1];
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
