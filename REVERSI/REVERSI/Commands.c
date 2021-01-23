#include <string.h>
#include <stdio.h>
#include "Commands.h"

/**
 * @brief Funcao que reinicia o comando
 * @param comando - apontador para o comando dado pelo utilizador
 */
void resetCommand(Command* comando)
{
    comando->isvalid = false;
    (comando->filename)[0] = '\0';
}


/**
 * @brief Funcao que le os comandos dados pelo utilizador
 * @param prompt - apontador para a string onde se vai encontrar o comando
 * @return retorna o comando depois de ler a string da instrucao dada pelo utilizador
 */
Command getCommand(char* prompt)
{
    Command comando;
    int args; //argumento
    char temp[31];
    char textarg[31];

    temp[0] = textarg[0] = '\0';

    resetCommand(&comando);

    args = sscanf(prompt,"%s",temp);

    if( args==EOF || strlen(temp)!=1 ) // commando inexistente (end of file) ou com mais de 1 char
        return comando;

    comando.command = temp[0];

    switch( temp[0] )
    {
        case 'N':
            args = sscanf(prompt,"%s %s %s",textarg,textarg,temp);

            if(
                    args != 2          ||  //Caso tamanho do comando maoir que dois (N e a PECA)
                    strlen(textarg)!=1 ||  //Caso o tamnaho de cada argumento ser maoir que 1
                    ( textarg[0]!='X' && textarg[0]!='O' ) //ou ainda ser diferente de qualquer uma das pecas possiveis
                    )
                return comando; // entao retorna comando como esta (invalido)

            comando.peca = textarg[0];

            if(comando.peca=='X' || comando.peca!='O');
            comando.isvalid = true;

            break;

        case 'L':
        case 'E':
            args = sscanf(prompt,"%s %s %s",textarg,textarg,temp);

            if(args!=2) // L/E
                return comando;

            strcpy(comando.filename,textarg); // Obtem nome do ficheiro
            comando.isvalid = true;

            break;

        case 'J':
            args = sscanf(prompt,"%s %d %d %s",textarg,&(comando.arg1int),&(comando.arg2int),temp);

            if( args!=3 )
                return comando;

            comando.isvalid = true;

            break;

        case 'A':
            args = sscanf(prompt,"%s %s %d %s",textarg,textarg,&(comando.arg2int),temp);

            if(
                    args!=3 ||
                    strlen(textarg)!=1
                    )
                return comando;

            comando.peca = textarg[0];

            if(
                    (comando.peca=='X' || comando.peca=='O') &&
                    (comando.arg2int>0 && comando.arg2int<4) //level
                    )
                comando.isvalid = true;

            break;

        case 'S':
        case 'H':
        case 'U':
        case 'Q':
            comando.isvalid = true;
            break;

        default:
            break;
    }

    return comando;
}

