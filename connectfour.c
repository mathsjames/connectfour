#include<stdio.h>

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
    if (state[cc][cr]==player) {
      len++;
    }
    else {
      break;
    }
  }
  for (cc=col-1;cc>=0; cc--) {
    if (state[cc][cr]==player) {
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

char makemove(char move, char state[7][6], char heights[7], history hist, char plys) {
  char player=1+plys%2;
  state[move][heights[move]]=player;
  heights[move]++;
  hist.hist[hist.length]=move;
  hist.length++;
  return inaline(state,move,heights[move]-1); // should return haswon
}

movet findmove(char state[7][6], char* heights, history hist) {
  movet retval;
  char move;
  for (move=0;move<7;move++) {
    if (1) {
      retval.move=move;
      retval.qual=21;
      return retval;
    }
  }
  for (move=0;move<7;move++) {
    
  }
}

void printstate(char state[7][6]) {
  char row,col;
  printf("1 2 3 4 5 6 7 \n");
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
  char state[7][6];
  char heights[7];
  for (char i=0;i<7;i++) {
    for (char j=0;j<6;j++) {
      state[i][j]=0;
    }
    heights[i]=0;
  }
      

  char automated[3]={0,0,0};
  char player=1;
  char gameover=0;
  char move;
  char plys=0;
  history proj, gamehist;
  movet tempmovet;
  printstate(state);
  while (!gameover) {
    player=1+plys%2;
    if (automated[player]==1) {
      proj.length=0;
      move=findmove(state,heights,proj).move;
    }
    else {
      move=getmove(player);
    }
    char iswon=makemove(move,state,heights,gamehist,plys);
    printstate(state);
    if (iswon) {
      printf("Player %d wins.\n",(int) player);
      gameover=1;
    }
    else if (plys==41) {
      printf("The game is a draw.\n");
      gameover=1;
    }
    plys++;
  }
  return 0;
}
