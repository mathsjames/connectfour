#include<stdio.h>
#include "gamestructure.h"

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
