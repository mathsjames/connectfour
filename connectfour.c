#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//#include<math.h>

const char MAXDEPTH=14;

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

typedef struct gamestate gamestate;
struct gamestate {
  char state[7][6];
  char heights[7];
  history hist;
  char plys;
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

char makemove(char move, gamestate* pgame) {
  char player=1+(*pgame).plys%2;
  (*pgame).plys++;
  (*pgame).state[move][(*pgame).heights[move]]=player;
  (*pgame).heights[move]++;
  (*pgame).hist.hist[(*pgame).hist.length]=move;
  (*pgame).hist.length++;
  //printstate((*pgame).state);
  return inaline((*pgame).state,move,(*pgame).heights[move]-1); // returns whether or not player has won
}

void undomove(gamestate* pgame) {
  (*pgame).hist.length--;
  char move=(*pgame).hist.hist[(*pgame).hist.length];
  (*pgame).heights[move]--;
  (*pgame).state[move][(*pgame).heights[move]]=0;
  (*pgame).plys--;
}

movet findmove(gamestate* pgame, char depth, char a, char b) {
  movet retval;
  char move;
  char qual[7], nextqual;
  retval.move=0;
  retval.qual=-43;
  char offset=rand()%7;
  if (depth<MAXDEPTH && (*pgame).plys<42) {
    for (char cmove=0;cmove<7;cmove++) {
      move=(cmove+offset)%7;
      if ((*pgame).heights[move]<6) {
	if (makemove(move,pgame)) {
	  nextqual=42;
	}
	else {
	  nextqual=findmove(pgame,depth+1,-b,-a).qual;
	  nextqual=-nextqual;
	}
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
    retval.qual=0;//heuristic value
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

char getmove(char player) {
  printf("Player %d, please enter the column number you want to play in (1 to 7):\n",(int) player);
  while (1) {
    char move=getchar()-'1';
    if (getchar()=='\n') {return move;}
  }
}

int main () {
  gamestate game;
  for (char i=0;i<7;i++) {
    for (char j=0;j<6;j++) {
      game.state[i][j]=0;
    }
    game.heights[i]=0;
  }
  game.hist.length=0;
  game.plys=0;

  
  char automated[3]={0,1,0};
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
      //printf("heights %d %d %d %d %d %d %d\n",game.heights[0],game.heights[1],game.heights[2],game.heights[3],game.heights[4],game.heights[5],game.heights[6]);
    }
    else {
      move=getmove(player);
    }
    char iswon=makemove(move,&game);
    printstate(game.state);
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
