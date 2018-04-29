#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include "gamestructure.h"
#include "backendutilities.h"
#include "frontendutilities.h"

typedef struct mcnode mcnode;
struct mcnode {
  char depth;
  char filled;
  int counts[7];
  int sums[7];
  mcnode* children[7];
  mcnode* parent;
};

void printmctree(mcnode node,char depth) {
  for (char m=0;m<7;m++) {
    for (char i=0;i<depth;i++) {
      printf("  ");
    }
    printf("%d:%d\n",m,node.counts[m]);
    if (node.children[m]) {
      printmctree(*node.children[m],depth+1);
    }
  }
}


void wipemcnode(mcnode* node,mcnode* parent) {
  (*node).filled=0;
  for (char i=0;i<7;i++) {
    (*node).counts[i]=0;
    (*node).sums[i]=0;
    (*node).children[i]=NULL;
  }
  (*node).parent=parent;
  (*node).depth=0;
  if (parent) {
    (*node).depth=(*parent).depth+1;
  }
  //printf("new node depth:%d\n",(*node).depth);
}

char rollout(gamestate* pgame) {
  char startplyparity=(*pgame).plys%2;
  char move;
  while ((*pgame).plys<42) {
    move=rand()%7;
    while ((*pgame).heights[move]>5) {
      move=rand()%7;
    }
  
    if (makemove(move, pgame)) {
      if (startplyparity==(*pgame).plys%2) {
	return 1;
      }
      else {
	return -1;
      }
    }
  }
  return 0;
}

char choosechild(mcnode* node) {
  char move;
  float value=-1;
  float newvalue;
  float tot=0;
  for (char i=0;i<7;i++) {
    tot+=(*node).counts[i];
  }
  tot=2*log(tot);
  //printf("tot:%f\n",tot);
  for (char i=0;i<7;i++) {
    newvalue=((float) (*node).sums[i])/(*node).counts[i]+sqrt(tot/(*node).counts[i]);
    //if (!(*node).parent) {
    //  printf("choose %d?:%f\n",i,newvalue);
    //  printf("sum:%d count:%d\n",(*node).sums[i],(*node).counts[i]);
    //}
    if (value<newvalue) {
      value=newvalue;
      move=i;
    }
    //if (!(*node).parent) {
    //  printf("chose %d?:%f\n",move,value);
    //}
  }
  return move;
}

void backprop(mcnode* current, char result,char plys, char hist[42]) {
  if (!(*current).parent) {
    //printf("ply:%d col:%d count:%d\n",plys,hist[plys-1],(*current).counts[hist[plys-1]]);
  }
  if ((*current).parent) {
    plys--;
    result=-result;
    current=(*current).parent;
    backprop(current,result,plys,hist);
  }

  (*current).counts[hist[plys-1]]++;
  (*current).sums[hist[plys-1]]+=result;
  //printf("plys:%d depth:%d\n",plys,(*current).depth);


}

void freemctree(mcnode root) {
  for (char i=0;i<7;i++) {
    if (root.children[i]) {
      freemctree(*(root.children[i]));
    }
    free(root.children[i]);
  }
}

char findmcmove(gamestate* pgame, int rollouts) {
  mcnode root;
  mcnode* current;
  gamestate game, rootgame;
  rootgame=*pgame;
  char move,result,currplys;
  move=0;
  wipemcnode(&root,NULL);
  char done;
  int countdirectwins=0, countdirectdraws=0, countproperrollouts=0;
  while (rollouts>0) {
    done=0;

    game=*pgame; //mccopy(*pgame);
    //printstate(game.state);
    //printf("%d\n",rollouts);
    current=&root;
    while ((*current).filled==7 && !done) {
      move=choosechild(current);
      //printstate(game.state);
      //printtraps(game.traps);
      if (makemove(move,&game)) {
	(*current).counts[move]+=1;
	(*current).sums[move]+=10;
	backprop(current,1,game.plys,game.hist.hist);
	rollouts--;
	countdirectwins++;
	done=1;
      }
      else {
	if (game.plys==42) {
	  (*current).counts[move]++;
	  backprop(current,0,game.plys,game.hist.hist);
	  rollouts--;
	  countdirectdraws++;
	  done=1;
	}
      }
      if (!done) {
	if ((*current).children[move]) {
	  //printf("move:%d height:%d newpointer:%d\n",move,game.heights[move],(*current).children[move]);
	  current=((*current).children[move]);
	}
	else {
	  break;
	}
      }
    }

    
    if (!(*current).children[move]) {
      (*current).children[move]=malloc(sizeof(mcnode));
      if (!(*current).children[move]) {
	printf("ERROR: malloc failed!\n");
	break;
      }
      wipemcnode((*current).children[move],current);
      current=(*current).children[move];
    }


    if (!done) {    
      if (game.heights[(*current).filled]>5) {
	(*current).sums[(*current).filled]=-1000;
	(*current).counts[(*current).filled]+=1;
	(*current).filled++;
	done=1;
      }
    }
    if (!done) {
      move=(*current).filled;
      (*current).filled++;
      if (makemove(move,&game)) {
	(*current).counts[move]+=1;
	(*current).sums[move]+=10;
	backprop(current,1,game.plys,game.hist.hist);
	countdirectwins++;
	rollouts--;
      }
      else {
	if (game.plys==42) {
	  (*current).counts[move]++;
	  backprop(current,0,game.plys,game.hist.hist);
	  rollouts--;
	  countdirectdraws++;
	}
	else {
	  currplys=game.plys;
	  result=rollout(&game);
	  //printf("%d %d %d\n",move,root.counts[move],(*current).counts[move]);
	  //printstate(game.state);
	  (*current).counts[move]++;
	  (*current).sums[move]+=result;
	  backprop(current,result,currplys,game.hist.hist);
	  rollouts--;
	  countproperrollouts++;
	}
      }
    }
  }
  //printmctree(root,0);
  //printf("dw:%d dd:%d pr:%d\n",countdirectwins,countdirectdraws,countproperrollouts);
  float value=-2;
  float newvalue;
  for (char i=0;i<7;i++) {
    printf("for:%d count:%d sum:%d tobeatwas:%f",i,root.counts[i],root.sums[i],value);
    newvalue=((float)root.sums[i])/root.counts[i];
    printf("scoreis:%f\n",newvalue);
    if (newvalue>value) {
      value=newvalue;
      move=i;
    }
  }
  
  freemctree(root);
  return move;
}
