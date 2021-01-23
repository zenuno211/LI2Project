//
// Created by Utilizador on 27/02/2019.
//


#include "./Estado.h"
#ifndef REVERSI_DATA_H
#define REVERSI_DATA_H


#define true 1
#define false 0
typedef int boolean;


typedef struct Historicoo
{
    ESTADO game;
    struct Historicoo* previous;
} Historico;

typedef struct Posicaoo
{
    int linha,coluna;
    struct Posicaoo* next;
} Posicao;

typedef struct
{
    Posicao coords;
    boolean isvalid;
    VALOR turn;
    int cima,baixo,esq,dir;
    int ce,cd,be,bd;
    int soma;
} Jogada;

typedef struct
{
    int player1; // jogador X
    int player2; // jogador O
} Pontos;


typedef struct
{
    boolean isvalid;
    char command;
    int arg1int,arg2int;
    char peca;
    char filename[31];
} Command;


#endif //REVERSI_DATA_H
