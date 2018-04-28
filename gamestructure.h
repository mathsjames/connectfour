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
  //char ppcount[2][2];
};

typedef struct gamestate gamestate;
struct gamestate {
  char state[7][6];
  char heights[7];
  history hist;
  char plys;
  traps traps;
  char killers[43];
};
