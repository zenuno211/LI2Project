#include "Posicao.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Função que dá free a todas as posições
 * @param listptr - apontador para uma lista de posições
 * @return NULL, isto porque, depois de dar free a todas as posições, o apontador aponta para um local que não tem nada
 */

Posicao* freeAllPos(Posicao* listptr)
{
    Posicao* next;

    if(listptr==NULL)
        return NULL;

    next=listptr->next;

    do
    {
        free(listptr);
        listptr=next;

        if(next==NULL)
            break;

        next=listptr->next;

    } while(1);

    return NULL;
}


/**
 * @brief Função que verifica se existe a posição "coords" na lista de posições "listptr"
 * @param listptr - apontador para uma lista de posições
 * @param coords - refere-se à posição no tabuleiro
 * @return True se existir alguma posição na "listptr" igual à posição "coords"; False caso contrário
 */

boolean existePos(Posicao* listptr,Posicao coords)
{

    if(listptr==NULL)
        return false;

    do
    {
        if(coords.linha==listptr->linha && coords.coluna==listptr->coluna)
            return true;

        if(listptr->next==NULL)
            break;

        listptr = listptr->next;

    } while (1);

    return false;
}
