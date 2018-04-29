#include "gamestructure.h"

history mccopyhist(history hist) {
  history newhist;
  for (char i=0;i<42;i++) {
    newhist.hist[i]=hist.hist[i];
  }
  newhist.length=hist.length;
  return newhist;
}

traps mccopytraps(traps trap) {
  traps newtrap;
  for (char i=0;i<7;i++) {
    for (char j=0;j<6;j++) {
      newtrap.active[i][j][0]=trap.active[i][j][0];
      newtrap.active[i][j][1]=trap.active[i][j][1];
    }
  }
  for (char i=0;i<43;i++) {
    newtrap.histc[i]=0;
  }
}

gamestate mccopy(gamestate game) {
  gamestate newgame;
  for (char i=0;i<7;i++) {
    for (char j=0;j<6;j++) {
      newgame.state[i][j]=game.state[i][j];
    }
    newgame.heights[i]=game.heights[i];
  }
  newgame.hist=mccopyhist(game.hist);
  newgame.plys=game.plys;
  newgame.traps=mccopytraps(game.traps);
  return newgame;
}
