

#ifndef REVERSI_JOGADA_H
#define REVERSI_JOGADA_H

#include "./Data.h"

void resetJogada(Jogada*);
int cercoByDir(ESTADO*,Posicao*,int,int);
void checkCerco(ESTADO*,Jogada*);
Jogada isValidPlay(ESTADO*,Posicao*);
int getValidPlays(ESTADO*,Posicao**);
Jogada maxPlay(ESTADO*);
Jogada minPlay(ESTADO*);

#endif //REVERSI_JOGADA_H
