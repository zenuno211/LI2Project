
#include "Game.h"
#include "./Posicao.h"
#include "./SaveNLoad.h"
#include "./Jogada.h"
#include <stdlib.h>
#include <stdio.h>
#include "Game.h"


/**
 * @brief Função que reinicia o tabuleiro
 * A função limpa o tabuleiro deixando só as pecas pecas necessárias para iniciar o jogo
 * @param gameptr - apontador para o estado atual do jogo
 */
void resetTabuleiro(ESTADO* gameptr) // coloca tabuleiro em estado de inicio de jogo
{
    int ln,col;

    for(ln=0 ; ln<8 ; ln++)
        for(col=0 ; col<8 ; col++)
            gameptr->grelha[ln][col] = VAZIA;

    gameptr->grelha[3][3] = gameptr->grelha[4][4] = VALOR_O; // e colocando as pecas inicias nas posicoes centrais do tabuleiro
    gameptr->grelha[3][4] = gameptr->grelha[4][3] = VALOR_X; // de ambos os jogadores

}

/**
 * @brief Funcção que reinicia os pontos
 * A Funçao iguala os pontos dos jogadores a 2 (porque os jogadores comecam o jogo com duas peças)
 * @param pontos - apontador para os pontos dos jogadores
 */
void resetPoints(Pontos* pontos) // comeca de novo a pontuaçao dos jogarores
{
    pontos->player1 = pontos->player2 = 2; // ambos iniciam com duas pecas no tabuleiro dai a score inicial ser igual a 2 para ambos
}

/**
 * @brief Função que reinicia o jogo
 * Funcao que conta com a ajuda de resetTabuleiro e resestPoints,limpa o historico, establece o turno do
 * primeiro jogador e o modo do próximo jogo.
 * @param gameptr - apontador para o estado atual do jogo
 * @param histoptr - apontador para um apontador do histórico do jogo
 * @param pontosptr - apontador para os pontos dos jogadores
 * @param turn - turno do jogo, mostra que jogador é que está a jogar
 * @param mode - modo do jogo
 */
void resetGame(
        ESTADO* gameptr,
        Historico** histoptr,
        Pontos* pontosptr,
        VALOR turn,
        char mode
)
{
    resetTabuleiro(gameptr); // retorna um tabuleiro so com as pecas iniciais
    gameptr->peca=turn; // o jogador a jogar sera aquele dado
    gameptr->modo=mode; // tal como o modo

    resetPoints(pontosptr); // a contagem dos posntos recomeça
    *histoptr = freeAllHistorico(*histoptr); //o historico do jogo limpa se para comecar de novo para o novo jogo
    pushGame(gameptr,histoptr); // guarda-se no historico o primeiro estado atribuido
}

/**
 * @brief Função que contabliza os pontos dos jogadores
 * A Funçao percorre o Tabuleiro linha a linha contando as peças que pertencem a cada jogador
 * @param game - apontador para o estado atual do jogo
 * @return points, pontos dos jogadores conforme as condicoes do jogo atual
 */
Pontos getPoints(ESTADO* game)
{
    Pontos points;
    int ln,col;


    points.player1 = 0;
    points.player2 = 0;

    for( ln=0 ; ln<8 ; ln++ )
        for( col=0 ; col<8 ; col++ )
        {
            if ( game->grelha[ln][col] == VALOR_X )
                points.player1++;
            else if ( game->grelha[ln][col] == VALOR_O )
                points.player2++;
        }

    return points;
}


/**
 * @brief Função auxiliar utilizada para a atualização do tabuleiro por vetores
 * @param game - apontador para o estado atual do jogo
 * @param coords - apontador para a posição da peça
 * @param total - número de peças do adversário que vão ser viradas com a jogada
 * @param x - abcissa do vertor
 * @param y - ordenada do vetor
 */

void updateTabuByDir(ESTADO* game,Posicao* coords,const int total,const int x,const int y)
{
    int l,c;
    int contador=0;

    l = coords->linha;
    c = coords->coluna,
    l += x;
    c += y;

    for( ; contador<total ; l+=x,c+=y,contador++ )game->grelha[l][c] = game->peca;

    if(total>0)
    {
        game->grelha[coords->linha][coords->coluna] = game->peca;
        game->grelha[l][c] = game->peca;
    }
}

/**
 * @brief Função que, com a ajuda da função "updateTabuByDir", atualiza o tabuleiro
 * @param game - apontador para o estado atual do jogo
 * @param jogadaptr - apontador que se refere-se à jogada do jogador
 */
void updateTabuleiro(ESTADO* game, Jogada* jogadaptr)
{

    if(jogadaptr->isvalid==false)
        return;

    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->cima ,-1, 0);
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->baixo, 1, 0);
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->esq  , 0,-1);
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->dir  , 0, 1);
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->ce   ,-1,-1);
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->cd   ,-1, 1);
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->be   , 1,-1);
    updateTabuByDir(game,&(jogadaptr->coords),jogadaptr->bd   , 1, 1);
}

/**
 * @brief Funcao que atualiza os pontos dos jogadores
 * A funcao soma a peca jogada pelos jogadores com a quantidade de pecas que os mesmos conseguem virar nessa mesma jogada
 * @param jogadaptr - apontador para a jogada efetuada pelo jogador
 * @param pontosptr - apontador para os pontos dos jogadores
 */
void updatePoints(Jogada* jogadaptr, Pontos* pontosptr) // apos cada jogada feita vai atualizar os pontos
{
    if(jogadaptr->turn==VALOR_X)
    {
        pontosptr->player1 += 1 + jogadaptr->soma; // player1=VALOR_X
        pontosptr->player2 -= jogadaptr->soma; // player2=VALOR_O
    }

    else
    {
        pontosptr->player2 += 1 + jogadaptr->soma; // player1=VALOR_X
        pontosptr->player1 -= jogadaptr->soma; // player2=VALOR_O

    }
}

/**
 * @brief Função que muda o turno do jogo
 * @param gameptr - apontador para o estado do jogo
 */
void nextTurn(ESTADO* gameptr) // premite, controla mudança de vez (entre jogadores)
{
    int size=0;
    VALOR next;
    Posicao* listptr = NULL;

    next = (gameptr->peca==VALOR_X) ? VALOR_O : VALOR_X;
    gameptr->peca = next;

    size=getValidPlays(gameptr,&listptr);

    if(!size) // caso ainda haja jogadas possiveis para o jogador seguinte
        gameptr->peca = (next==VALOR_X) ? VALOR_O : VALOR_X; // entao a vez passa a ser dele

    freeAllPos(listptr);
}

/**
 * @brief Função que verifica se o jogo acabou
 * Utilizamos o "size1" guardar a quantidade de jogadas possíveis que o jogador atual tem, se não tiver nenhuma troca de turno,
 * também utiizamos o "size2" para guardar a quantidade de jogadas possíveis que o jogador tem. Se não existirem jogadas
 * válidas para nenhum dos jogadores significa que se chegou ao fim do jogo.
 * @param game - apontador para o estado do jogo
 * @return true se se tiver chegado ao fim do jogo, false caso contrário
 */
boolean isEndgame(ESTADO* game)
{
    Posicao* poslist = NULL;
    int size1,size2;

    size1 = getValidPlays(game,&poslist); // tamanho lista das posicoes possiveis de jogada
    poslist = freeAllPos(poslist);

    game->peca = (game->peca==VALOR_X) ? VALOR_O : VALOR_X;
    size2 = getValidPlays(game,&poslist);
    poslist = freeAllPos(poslist);

    game->peca = (game->peca==VALOR_X) ? VALOR_O : VALOR_X; // restaura turno original

    if( !size1 && !size2 )
        return true;
    return false;
}


/**
 * @brief Função que dá print ao tabuleiro
 * @param gameptr - apontador para o estado do jogo
 * @param pontos - apontador para os pontos dos jogadores
 * @param modo -
 */
// modo=1 imprime . (pontos) jogadas validas
void printTabuleiro(ESTADO* gameptr,Pontos* pontos,int modo)
{
    int linhaReal=0;
    Posicao coords;
    Posicao *listptr=NULL;
    Jogada max;

    coords.linha = coords.coluna = 0;

    printf("\n         1       2       3       4       5       6       7       8\n");

    if(modo==1)
        getValidPlays(gameptr,&listptr);
    if(modo==2)
        max = maxPlay(gameptr);

    for(coords.linha=0 ; coords.linha<8 ; coords.linha++)
    {
        printf("     -----------------------------------------------------------------");
        linhaReal++;

        if(linhaReal==1 || linhaReal==5 || linhaReal==9 || linhaReal==33)
            printf(" #######################");
        if(linhaReal==13)
            printf("                       #");

        if(linhaReal==17)
            printf(" L <filename> (load)   #");
        else if(linhaReal==21)
            printf(" H (hint)              #");
        else if(linhaReal==25)
            printf(" J <l> <c> (jogar)     #");
        else if(linhaReal==29)
            printf(" U (undo)              #");

        printf("\n");
        printf("     |       |       |       |       |       |       |       |       |");
        linhaReal++;

        if(linhaReal==2)
            printf("\t     -> SCORE <-     #");
        else if(
                linhaReal==6  || linhaReal==10 || linhaReal==14 ||
                linhaReal==18 || linhaReal==22 || linhaReal==26 ||
                linhaReal==30
                )
            printf("                       #");

        printf("\n");
        linhaReal++;
        for(coords.coluna=0 ; coords.coluna<8 ; coords.coluna++)
        {
            if(coords.coluna==0)
                printf("  %d  ",coords.linha+1);

            if(modo==1 && existePos(listptr,coords)==true)
            {
                printf("|   .   "); // jogada valida
                continue;
            }

            if(modo==2 && max.coords.linha==coords.linha && max.coords.coluna==coords.coluna)
            {
                printf("|   ?   "); // hint
                continue;
            }

            if(gameptr->grelha[coords.linha][coords.coluna]==VALOR_X)
                printf("|   X   ");
            else if(gameptr->grelha[coords.linha][coords.coluna]==VALOR_O)
                printf("|   O   ");
            else
                printf("|       "); // casa vazia


        }

        printf("|");

        if(linhaReal==3)
            printf("                       #");
        else if(linhaReal==7)
        {
            if(gameptr->peca==VALOR_X && gameptr->difficulty==0)
                printf("\tTurno: X             #");
            else if (gameptr->peca==VALOR_O && gameptr->difficulty==0)
                printf("\tTurno: O             #");
            else if(gameptr->peca==VALOR_X)
                printf("\tTurno: X | Lvl: %d    #",gameptr->difficulty);
            else
                printf("\tTurno: O | Lvl: %d    #",gameptr->difficulty);

        }
        else if(linhaReal==11)
            printf("\t   -> COMMANDS <-    #");
        else if(linhaReal==15)
            printf(" N <peca> (1vs1)       #");
        else if(linhaReal==19)
            printf(" E <filename> (save)   #");
        else if(linhaReal==23)
            printf(" A <peca> <nivel> (bot)#");
        else if(linhaReal==27)
            printf(" S (jogadas possiveis) #");
        else if(linhaReal==31)
            printf(" Q (quit)              #");

        printf("\n");
        linhaReal++;
        printf("     |       |       |       |       |       |       |       |       |");

        if(linhaReal==4)
        {
            printf("\t  X: %d   |   O: %d",pontos->player1,pontos->player2);
            if(pontos->player1>=10 && pontos->player2>=10)
                printf("  #");
            else if(pontos->player1>=10 || pontos->player2>=10)
                printf("   #");
            else
                printf("    #");
        }
        else if (
                linhaReal==8  || linhaReal==12 || linhaReal==16 ||
                linhaReal==20 || linhaReal==24 || linhaReal==28 ||
                linhaReal==32
                )
            printf("                       #");

        printf("\n");
    }

    printf("     -----------------------------------------------------------------");
    linhaReal++;

    if(linhaReal==33)
        printf(" #######################");

    printf("\n\n");
    freeAllPos(listptr);
}


/**
 * @brief Função que executa os passos necessários entre as jogadas
 * Se a jogada for inválida dá return false;
 * Se a jogada for válida, chama diferentes funções de forma a atualizar o tabuleiro, os pontos, muda de turno e atualiza também o histórico
 * @param gameptr - apontador para o estado do jogo
 * @param coords - apontador que se refere à posição no tabuleiro
 * @param pontosptr - apontador para os pontos dos jogadores
 * @param histoptr - apontador para um apontador do histórico do jogo
 * @return false se a jogada for inválida, true se a jogada for válida
 */

boolean play(
        ESTADO* gameptr,
        Posicao* coords,
        Pontos* pontosptr,
        Historico** histoptr
)
{
    Jogada jogada;

    jogada=isValidPlay(gameptr,coords); // caso jogada seja valida

    if(jogada.isvalid==false)
        return false;

    updateTabuleiro(gameptr,&jogada); // retornamos o tabuleiro atualizado

    updatePoints(&jogada,pontosptr); // os pontos atualizados

    nextTurn(gameptr); // passamos a vez ao proximo jogador

    pushGame(gameptr,histoptr); // e guardamos a "jogada" no historico

    return true;
}
