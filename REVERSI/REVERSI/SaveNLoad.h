
#ifndef REVERSI_SAVENLOAD_H
#define REVERSI_SAVENLOAD_H

#include "./Data.h"

#define FILENAME "./SavedGames/mysave.txt"

void pushGame(ESTADO*,Historico**);
void popGame(ESTADO*,Historico**);
Historico* freeAllHistorico(Historico*);
int saveGame(Historico**,char*);
int loadGame(Historico**,ESTADO*,char*);

#endif //REVERSI_SAVENLOAD_H

