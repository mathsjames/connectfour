#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//#include<math.h>
#include "gamestructure.h"
#include "frontendutilities.h"
#include "backendutilities.h"
#include "abagent.h"
#include "mcagent.h"

const char MAXQUAL=84;

int main (int argc,char *argv[]) {
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
    game.killers[i]=rand()%7;
  }
  for (char i=0;i<84;i++) {
    game.traps.histt[i][0]=0;
    game.traps.histt[i][1]=0;
  }

  char player;  
  char automated[3]={0,-1,-1};
  if (argc>2) {
    for (player=1;player<3;player++) {
      automated[player]=(argv[player])[0]-'0';
      if ((argv[player])[1]!='\0') {
	automated[player]=automated[player]*10+(argv[player])[1]-'0';
      }
    }
  }
  

  char move;
  srand(time(NULL));

  printstate(game.state);
  while (1) {
    player=1+game.plys%2;
    if (automated[player]) {
      //printstate(game.state);
      if (automated[player]>0) {
	move=findabmove(&game,0,-MAXQUAL-1,MAXQUAL+1,automated[player]).move;
      }
      else {
	move=findmcmove(&game,10000);
      }
      //printstate(game.state);
      //printf("move %d player %d\n",move,player);

    }
    else {
      move=getmove(player);
      while (game.heights[move]>5) {
	printf("That column is already full.\n");
	move=getmove(player);
      }
    }
    char iswon=makemove(move,&game);
    printf("Player %d plays in column %d",player,move+1);
    printstate(game.state);
    //printf("\n heights %d %d %d %d %d %d %d\n \n",game.heights[0],game.heights[1],game.heights[2],game.heights[3],game.heights[4],game.heights[5],game.heights[6]);
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
