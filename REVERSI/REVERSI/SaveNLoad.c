#include "SaveNLoad.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Jogada.h"
#include "Posicao.h"


/**
 * @brief Funçao que vai atualizando o historico apos cada jogada efetuada
 * Aqui ela vai guardadndo os diferentes estados do jogo ao longodo decorrer do mesmo. Sendo que guarda não só o tabuleiro
 * e as peças que lhe foram implemntadas através das jogdas como também guarda a peca do jogador a jogar e o modo do jogo.
 * @param gameptr - apontador para o estado do jogo
 * @param listptr - apontador para o histórico do jogo
 */
// historico -> implementacao LIFO
void pushGame(ESTADO* gameptr, Historico** listptr) // cria um historico, tal como a stack os estados vao se empilhando
{                                                   // sendo o mais recente encontrado no topo e o mais antigo (inicios de jogo)
    int ln,col;                                     // mais abaixo (tal como a stack)
    Historico* ptr=NULL;

    ptr = malloc(sizeof(Historico));

    if(*listptr==NULL) // caso historico esteja nulo entao nao havera nenhum estado anterior a este
    {
        *listptr=ptr; // sendo portanto a partir de agora o estado igual ao estado do jogo neste momento
        ptr->previous = NULL; // nao ha nenhum anterior pois novo
    }

    else
    {
        ptr->previous = *listptr; // caso o historico nao esteja vazio entao ligamos o novo estado ao estado anterior (historico previous)
        *listptr=ptr; // sendo que o historico agora passou a ser toodo ele (desde o anterior mais o novo estado guardado)
    }

// relativamente a primeira linha que diz M X ou A O 2
    (ptr->game).peca = gameptr->peca; // nessa nova "parte" do historico vamos guardadr o estado atual do jogo
    (ptr->game).modo = gameptr->modo; // como a peca que indica o jogador que joga e o modo do jogo
    (ptr->game).difficulty = gameptr->difficulty;

    for(ln=0 ; ln<8 ; ln++) // e ainda o tabuleiro com as pecas distribuidas conforme o estado do jogo
        for(col=0 ; col<8 ; col++)
            (ptr->game).grelha[ln][col] = gameptr->grelha[ln][col];
}

/**
 * @brief Funcao responsável pelo Undo
 * Usando o historico, temos a possibilidade de  voltar ao estado anterior, podendo assim retirar a última jogada realizada
 * @param gameptr - apontador para o estado do jogo
 * @param listptr - apontador para o histórico do jogo
 */
// pop de uma lista LIFO
void popGame(ESTADO* gameptr, Historico** listptr)
{
    int ln,col;
    Historico* ptr=NULL;  // Se o historico estiver nulo, ou seja quando se inicia um jogo, nao vai haver nenhum
                          // estado anterior uma vez que ainda nenhuma jogada foi feita, assim nao e possivel fazer
    if((*listptr)->previous==NULL) //undo nessas situacoes
        return; // nenhumas aleteraçoes sao feitas

    ptr = (*listptr)->previous; /*senao o estado do jogo volta ao anterior a jogada feita */
    free(*listptr); /*libertando assim o espaco ocupado por essa jogada */
    *listptr=ptr; // o historico passa a ser igual ao anterior a jogada

    *gameptr = (*listptr)->game; // o estado do jogo e alterado para o seu estado anterior a jogada feita
}

/**
 * @brief Funçao que limpa o historico inicializando-o
 * @param histoptr - apontador para o histórico do jogo
 * @return historico vazio
 */
Historico* freeAllHistorico(Historico* histoptr)
{
    Historico* previous;

    if(histoptr==NULL) // caso ja se encontre vazio
        return NULL;

    previous = histoptr->previous;
    while(1)
    {
        free(histoptr);

        if(previous==NULL)
            break;;

        histoptr = previous;
        previous = histoptr->previous;

    }

    return NULL;
}



/**
 * @brief Cria um ficheiro de texto onde se vai guardar o jogo com a ajuda do historico
 * @param histoptr - apontador para o histórico do jogo
 * @param filename - apontador para o nome do ficheiro
 * @return 0 se o ficheiro estiver vazio, ou 1 caso contrário (se for bem sucedido)
 */
int saveGame(Historico** histoptr,char* filename)
{
    Posicao coords;
    char peca;
    FILE* myfile=NULL;
    char finalName[61];

    finalName[0] = '\0';

    strcat(finalName,"../SavedGames/"); //pasta onde guardamos os jogos
    strcat(finalName,filename); // nome dado a esse ficheiro
    strcat(finalName,".txt"); // do tipo texto

    myfile = fopen(finalName,"w");

    if(myfile==NULL)
        return 0;

    while(*histoptr!=NULL)
    {
        peca = ( ((*histoptr)->game).peca == VALOR_X ) ? 'X' : 'O';

        fprintf(myfile,"%c %c",((*histoptr)->game).modo,peca); // faz o print do modo (A OU M) e da peca a jogar
        if( (*histoptr)->game.difficulty !=0 ) // no estado temos uma variavel difficulty associado ao nivel de dificuldade do jogo, caso este exista, (modo automatico)
            fprintf( myfile, " %d\n", (*histoptr)->game.difficulty );
        else
            fprintf(myfile,"\n"); // relativamente a primeira linha a escrever

        for(coords.linha=0 ; coords.linha<8 ; coords.linha++) // comeca o print do estado do tabuleiro
        {
            for(coords.coluna=0 ; coords.coluna<8; coords.coluna++)
            {
                if ( ((*histoptr)->game).grelha[coords.linha][coords.coluna] == VALOR_X )
                    peca = 'X';
                else if (  ((*histoptr)->game).grelha[coords.linha][coords.coluna] == VALOR_O  )
                    peca = 'O';
                else
                    peca = '-';

                fprintf(myfile,"%c ",peca); // print das pecas
            }

            fprintf(myfile,"\n");
        }
        fprintf(myfile,"\n");

        histoptr = & ( (*histoptr)->previous );

    }

    fclose(myfile);

    return 1;
}


/**
 * @brief funçao que faz load de um jogo colocando o assim no Historico (atual)
 * @param histoptr - apontador para o histórico do jogo
 * @param gameptr - apontador para o estado do jogo
 * @param filename - apontador para o nome do ficheiro
 * @return 0 se o ficheiro estiver vazio, ou 1 caso contrário (se for bem sucedido)
 */

int loadGame(Historico** histoptr,ESTADO* gameptr,char* filename) {
    int ln, col;
    char ch;
    FILE *myfile = NULL;
    VALOR peca;
    Historico *temp = NULL, *currentHisto = NULL;
    char finalName[61];

    finalName[0] = '\0';

    strcat(finalName, "../SavedGames/");
    strcat(finalName, filename);
    strcat(finalName, ".txt");

    myfile = fopen(finalName, "r");

    if (myfile == NULL)
        return 0;

    *histoptr = freeAllHistorico(*histoptr);

    ch = (char) fgetc(myfile); // capta modo
    while (1) {
        // setup nova estrutura
        temp = malloc(sizeof(Historico));

        if (*histoptr == NULL) {
            *histoptr = temp;
            currentHisto = temp;
        } else {
            currentHisto->previous = temp;
            currentHisto = temp;
        }

        currentHisto->previous = NULL;
        // fim setup

        currentHisto->game.modo = ch;

        fgetc(myfile); // espaco entre modo e peca;
        ch = (char) fgetc(myfile); // peca

        currentHisto->game.peca = (ch == 'X') ? VALOR_X : VALOR_O;

        ch = (char) fgetc(myfile); // le enter ou espaço
        if (ch == '\n')
            currentHisto->game.difficulty = 0;
        else {
            ch = (char) fgetc(myfile); // le numero
            currentHisto->game.difficulty = (char) ch - 48; // conversao para num
            fgetc(myfile);// consome \n
        }

        //inicio leitura tabuleiro
        for (ln = 0; ln < 8; ln++) {
            for (col = 0; col < 8; col++) {
                ch = (char) fgetc(myfile); // char
                fgetc(myfile); // espaco entre chars

                switch (ch) {
                    case '-':
                        peca = VAZIA;
                        break;
                    case 'X':
                        peca = VALOR_X;
                        break;
                    case 'O':
                        peca = VALOR_O;
                        break;
                }
                currentHisto->game.grelha[ln][col] = peca;
            }
            fgetc(myfile); // \n fim linha
        }
        //fim leitura tabuleiro

        while (
                (ch = (char) fgetc(myfile)) != 'A' &&
                ch != 'M' &&
                ch != EOF
                );

        if (ch == EOF)
            break;
    }

    fclose(myfile);

    *gameptr = (*histoptr)->game;

    return 1;
}