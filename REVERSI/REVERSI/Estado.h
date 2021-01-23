//
// Created by Utilizador on 27/02/2019.
//

#ifndef REVERSI_ESTADO_H
#define REVERSI_ESTADO_H

#define MAX_BUFFER 1024


/**
estado.h
DefiniÃ§Ã£o do estado i.e. tabuleiro. RepresentaÃ§Ã£o matricial do tabuleiro.
*/


// definiÃ§Ã£o de valores possiveis no tabuleiro
typedef enum {VAZIA, VALOR_X, VALOR_O} VALOR;

/**
Estrutura que armazena o estado do jogo
*/
typedef struct estado {
    int difficulty;
    VALOR peca; // peÃ§a do jogador que vai jogar!
    VALOR grelha[8][8];
    char modo; // modo em que se estÃ¡ a jogar! 0-> manual, 1-> contra computador
} ESTADO;


void printa(ESTADO);

ESTADO novo_jogo(ESTADO e, VALOR v);

#endif //REVERSI_ESTADO_H
