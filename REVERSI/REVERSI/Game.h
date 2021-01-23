
#ifndef REVERSI_GAME_H
#define REVERSI_GAME_H

#include "./Data.h"

void resetTabuleiro(ESTADO*);
void resetPoints(Pontos*);
void resetGame(ESTADO*,Historico**,Pontos*,VALOR,char);

Pontos getPoints(ESTADO*);

void updateTabuByDir(ESTADO*,Posicao*,const int,const int,const int);
void updateTabuleiro(ESTADO*,Jogada*);
void updatePoints(Jogada*,Pontos*);

void nextTurn(ESTADO*);
boolean isEndgame(ESTADO*);

void printTabuleiro(ESTADO*,Pontos*,int);

boolean play(ESTADO*,Posicao*,Pontos*,Historico**);

#endif //REVERSI_GAME_H
