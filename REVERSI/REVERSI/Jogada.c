
#include "Jogada.h"
#include <stdio.h>
#include <stdlib.h>
#include "Posicao.h"


/**
 * @brief Função que reinicia a jogada
 * Esta função torna a jogada inválida, o turno vazio, e iguala o número de peças a 0
 * @param jogada - apontador que se refere-se à jogada do jogador
 */

void resetJogada(Jogada* jogada)
{
    jogada->isvalid=false;
    jogada->turn=VAZIA;
    jogada->cima = jogada->baixo = jogada->esq = jogada->dir = 0;
    jogada->ce = jogada->cd = jogada->be = jogada->bd = 0;
    jogada->soma=0;
}

/**
 * @brief Função auxiliar utilizada para verificar, através de vetores se uma jogada é ou não possível
 * @param game - apontador para o estado atual do jogo
 * @param coords - apontador para a posição da peça
 * @param x - abcissa do vertor
 * @param y - ordendada do vetor
 * @return contador, número de peças do adversário que é possível virar
 */

int cercoByDir(ESTADO* game,Posicao* coords,int x,int y)
{
    int contador=0,l,c;
    VALOR enemy = game->peca==VALOR_X ? VALOR_O : VALOR_X;

    l = coords->linha  ;
    c = coords->coluna ;
    l += x;
    c += y;

    if(
            l<0 || l>7 || // coordenadas fora do mapa
            c<0 || c>7
            )
        return 0;

    for(  ; game->grelha[l][c]==enemy ; l+=x,c+=y ) // contando as pecas do inimigo direçoes dos vetor na funcao abaixo
        contador++; //importante para verificar o numero de pecas voltadas ao adversario

    if(game->grelha[l][c]==VAZIA) // caso peca vazia entao impossivel jogada
        return 0;

    return contador;
}

/**
 * @brief Esta função tem o objetivo de verificar se existem peças do jogador adverário à volta da peça do jogador atual, conforme
 * as direções dadas pelos vetores
 * Esta função vai-nos ajudar mais tarde a verificar se as jogadas são válidas
 * @param gameptr - apontador para o estado atual do jogo
 * @param jogadaptr - apontador que se refere-se à jogada do jogador atual
 */

void checkCerco(ESTADO* gameptr, Jogada* jogadaptr)
{

    jogadaptr->cima  = cercoByDir(gameptr,&(jogadaptr->coords),-1, 0);
    jogadaptr->baixo = cercoByDir(gameptr,&(jogadaptr->coords), 1, 0);
    jogadaptr->esq   = cercoByDir(gameptr,&(jogadaptr->coords), 0,-1);
    jogadaptr->dir   = cercoByDir(gameptr,&(jogadaptr->coords), 0, 1);
    jogadaptr->ce    = cercoByDir(gameptr,&(jogadaptr->coords),-1,-1);
    jogadaptr->cd    = cercoByDir(gameptr,&(jogadaptr->coords),-1, 1);
    jogadaptr->be    = cercoByDir(gameptr,&(jogadaptr->coords), 1,-1);
    jogadaptr->bd    = cercoByDir(gameptr,&(jogadaptr->coords), 1, 1);
    // fim contagens

    jogadaptr->soma = jogadaptr->cima + jogadaptr->baixo + jogadaptr->esq + jogadaptr->dir +
                      jogadaptr->ce + jogadaptr->cd + jogadaptr->be + jogadaptr->bd; // caso a soma de todos os contador para todas as direcao seja 0 entao nao e vaida a jogada

    if(jogadaptr->soma > 0)
        jogadaptr->isvalid=true;
}


/**
 * @brief Esta função verifica se a jogada efetuada é válida.
 * Se as coordenas escolhidas pelo jogador estiverem fora do tabuleiro, ou se já estiver uma peça nessa posição,
 * a função dá return à jogada como inválida.
 * Se estes últimos dois casos não se verificarem, chama-se a função checkCerco com o objetivo de verificar se a jogada é válida
 * @param gameptr - apontador para o estado do jogo
 * @param coords - apontador para as coordenadas
 * @return jogada, inválida caso não seja possível efetuar a jogada, ou válida caso contrário.
 */

Jogada isValidPlay(ESTADO* gameptr, Posicao* coords)
{
    Jogada jogada;

    resetJogada(&jogada);

    jogada.coords.linha  = coords->linha;
    jogada.coords.coluna = coords->coluna;
    jogada.turn          = gameptr->peca;

    // coordenadas fora do mapa
    if(
            coords->linha<0  || coords->linha>7 ||
            coords->coluna<0 || coords->coluna>7
            )
        return jogada;

    // caso na posicao escolhida ja exista jogada
    if(gameptr->grelha[coords->linha][coords->coluna]!=VAZIA)
        return jogada;

    // contagens a converter em cada direcao. Caso nenhuma conversao->jogada invalida
    checkCerco(gameptr,&jogada);

    return jogada;
}

/**
 * @brief com auxilio das funções anteriores vai criar uma lista das posições para jogadas possíveis
 * @param gameptr - apontador para o estado do jogo
 * @param listptr - apontar para uma lista de posições
 * @return size, tamanho da lista
 */

int getValidPlays(ESTADO* gameptr, Posicao** listptr)
{
    Posicao coords;
    Jogada jogada;
    Posicao** nextPos=NULL;
    Posicao* newPos=NULL;
    int size=0;

    if( (*listptr) != NULL )
        *listptr=freeAllPos(*listptr);

    for(coords.linha=0 ; coords.linha<8 ; coords.linha++)
        for(coords.coluna=0 ; coords.coluna<8 ; coords.coluna++)
        {
            jogada=isValidPlay(gameptr,&coords);

            if(jogada.isvalid==true)
            {
                // pointer inicio da lista
                if(*listptr==NULL)
                {
                    *listptr = malloc(sizeof(Posicao));
                    (*listptr)->linha  = coords.linha;
                    (*listptr)->coluna = coords.coluna;
                    (*listptr)->next   = NULL;
                    nextPos = &( (*listptr)->next );
                }
                else
                {
                    newPos = malloc(sizeof(Posicao));
                    newPos->linha  = coords.linha;
                    newPos->coluna = coords.coluna;
                    newPos->next   = NULL;
                    *nextPos = newPos;
                    nextPos = &( newPos->next );
                }
                size++;
            }
        }
    return size;
}

/**
 * @brief Função que verifica qual é a melhor jogada (jogada que ganha mais pontos)
 * @param gameptr - apontador para o estado do jogo
 * @return max, jogada que ganha mais pontos (vira mais peças do adversário)
 */

Jogada maxPlay(ESTADO* gameptr)
{
    Jogada jogada;
    Jogada max;
    Posicao* poslist=NULL;
    Posicao* next=NULL;

    getValidPlays(gameptr,&poslist);

    next=poslist;

    resetJogada(&max); // soma de max vai estar a 0
    resetJogada(&jogada);

    if( poslist==NULL )
        return jogada;

    while(1)
    {
        resetJogada(&jogada);

        jogada = isValidPlay(gameptr,next);

        if(jogada.soma>max.soma)
            max = jogada;

        next = next->next;

        if(next==NULL)
            break;

    }

    freeAllPos(poslist);

    return max;
}

/**
 * @brief Função que verifica qual é a pior jogada (jogada que ganha menos pontos)
 * @param gameptr - apontador para o estado do jogo
 * @return min, jogada que ganha menos pontos (vira menos peças do adversário)
 */
Jogada minPlay(ESTADO* gameptr)
{
    Jogada jogada;
    Jogada min;
    Posicao* poslist=NULL;
    Posicao* next=NULL;

    getValidPlays(gameptr,&poslist);

    next=poslist;

    resetJogada(&min);
    resetJogada(&jogada);

    if( poslist==NULL )
        return jogada;
    min.soma= 8;
    while(1)
    {
        resetJogada(&jogada);

        jogada = isValidPlay(gameptr,next);

        if(jogada.soma<min.soma) // soma ( numero de pecas voltadas )
            min = jogada;

        next = next->next;


        if(next==NULL)
            break;

    }

    freeAllPos(poslist);

    return min;
}

