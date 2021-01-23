//
// Created by Utilizador on 27/02/2019.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./Data.h"
#include "./Posicao.h"
#include "./Jogada.h"
#include "./Game.h"
#include "./Commands.h"
#include "./SaveNLoad.h"

#define CLEARMYSCREEN (system("cls"))

void clearBuffer()
{
    while(getchar()!='\n');
}


int main()
{
    ESTADO game; // jogo
    Historico* histoptr=NULL; // apontador LIFO historico de estados
    Pontos pontos; // pontos actuais do jogo
    Posicao coords; // cordenadas inseridas pelo humano
    Command comando; // construÃ§ao do comando
    int printMode=0; // modo de impressao onde 0 e o normal,   1 e com pos validas,   2 e a sugestao (HINT "?")
    char prompt[52]={'\0'}; // string que recebe comando inserido
    Jogada tempPlay; // jogada temporaria para dificuldade 2
    VALOR bot=VALOR_O; // inicializacao peca bot
    VALOR turn=VALOR_X; // inicializacao turno inicial
    Posicao* validPosList = NULL, *tempPos=NULL;
    int sizePosList,randPos;
    int saveload; //controla validade de save e load

    srand(time(NULL)); // inicializacao para uso do rand

    game.difficulty=1;
    resetGame(&game,&histoptr,&pontos,VALOR_X,'A'); // inicializacao do estado (comeca peca x e modo automatico nivel de dificuldade 1)

    GOTO_INICIO_JOGO: //label para salto incondicional
    CLEARMYSCREEN;

    printTabuleiro(&game,&pontos,printMode); // inicializamos printmode como 0 no inicio
    printMode=0;

    if( isEndgame(&game) )
    {
        if( pontos.player1 > pontos.player2 )
            printf("\n\t\tJOGADOR 'X' SAIU VENCEDOR COM:\n\t\t  %d vs %d pontos\n\n", pontos.player1,pontos.player2);
        else if( pontos.player2 > pontos.player1 )
            printf("\n\t\tJOGADOR 'O' SAIU VENCEDOR COM:\n\t\t  %d vs %d pontos\n\n", pontos.player2,pontos.player1);
        else
            printf("\n\t\tJOGO EMPATADO:\n\t\t  %d vs %d pontos\n\n", pontos.player1,pontos.player2);

        resetGame(&game,&histoptr,&pontos,VALOR_X,game.modo); // novo jogo
        getchar();

        goto GOTO_INICIO_JOGO;
    }

    // turno bot
    if( game.peca==bot )
    {
        printf("BOT TURN...ANY KEY TO CONTINUE OR 'U' TO UNDO!\n");
        if( getchar()=='U' )
        {
            clearBuffer();
            goto GOTO_UNDO;
        }
        clearBuffer();

       // jogada minima
        if( game.difficulty==1 )
        {
            tempPlay = minPlay(&game);
            play(&game,&(tempPlay.coords),&pontos,&histoptr);

        }
             //jogada random
        else if( game.difficulty==2 ) //como com a hint
         {
                sizePosList = getValidPlays(&game,&validPosList);
                randPos = 1 + rand() % sizePosList; //rand dara um numero aleatorio a partir de 0 que ao dividir pelo tamAnho da lista das possicoes possiveis dara uma posicao

                tempPos = validPosList;
                for( ; randPos!=1 ; randPos-- )
                    tempPos = tempPos->next;

                play(&game,tempPos,&pontos,&histoptr);

                validPosList = freeAllPos(validPosList);
                tempPos = NULL;
            }


        else // difficulty == 3// jogada maxima
            tempPlay = maxPlay(&game);
            play(&game,&(tempPlay.coords),&pontos,&histoptr);

        goto GOTO_INICIO_JOGO;
    }

    // obtencao de comando
    printf(" COMMAND LINE:$ ");
    fgets(prompt,51,stdin);

    comando = getCommand(prompt);

    // avaliacao de comando
    if(comando.isvalid==false)
    {
        printf("INVALID COMMAND...PRESS TO CONTINUE");
        getchar();
        goto GOTO_INICIO_JOGO;
    }

    // executa comando se for valido
    switch(comando.command)
    {
        case 'N': // human vs human
            turn = ( comando.peca=='X' ) ? VALOR_X : VALOR_O;
            bot = VAZIA;

            game.difficulty=0;
            resetGame(&game,&histoptr,&pontos,turn,'M');

            goto GOTO_INICIO_JOGO;
        case 'L': // load
            saveload = loadGame(&histoptr,&game,comando.filename);

            if(!saveload)
            {
                printf("\n!FALHA LEITURA DE JOGO! VERIFIQUE NOME DO FICHEIRO!\n");
                getchar();
                goto GOTO_INICIO_JOGO;
            }
            else
            {
                printf("\nLOAD COM SUCESSO!\n");
                getchar();
            }

            if(game.difficulty==0)
                bot=VAZIA;
            else
                bot=(game.peca==VALOR_X) ? VALOR_O : VALOR_X;

            pontos = getPoints(&game);

            goto GOTO_INICIO_JOGO;
        case 'E': // save
            saveload = saveGame(&histoptr,comando.filename);

            if(!saveload)
            {
                printf("\nFALHA ESCRITA DE JOGO!\n");
                getchar();
            }
            else
            {
                printf("\nJOGO GUARDADO!\n");
                getchar();
            }

            goto GOTO_INICIO_JOGO;

        case 'H': // hint
            printMode = 2; // print do tabuleiro com modo 2
            goto GOTO_INICIO_JOGO;

        case 'A': // vs Bot
            bot = (comando.peca == 'X') ? VALOR_X : VALOR_O;

            game.difficulty = comando.arg2int; // (level entre 1 a 3) ,level 0 e o jogo normal
            resetGame(&game,&histoptr,&pontos,VALOR_X,'A');

            goto GOTO_INICIO_JOGO;

        case 'J': // play
            coords.linha  = comando.arg1int - 1; // linhas dadas no comando sao de 1 a 8 no entanto as cordenadas da grelha do tabulero sao cordenadas de 0 a 7
            coords.coluna = comando.arg2int - 1;

            play(&game,&coords,&pontos,&histoptr);
            goto GOTO_INICIO_JOGO;

        case 'S': // valid plays
            printMode = 1;              // feito em game.c no printT
            // Tabuleiro
            goto GOTO_INICIO_JOGO;

        case 'U': // undo
        GOTO_UNDO:
            popGame(&game,&histoptr);
            pontos = getPoints(&game);
            goto GOTO_INICIO_JOGO;

        case 'Q': // quit
            break;

        default:
            printf("\n\nCOMMAND VALIDATION FAIL ");
            getchar();
            exit(1);
    }

    getchar();
    return 0;
}
