cmake_minimum_required(VERSION 3.13)
project(REVERSI C)

set(CMAKE_C_STANDARD 99)

add_executable(REVERSI main.c Commands.c Commands.h Posicao.h Posicao.c Estado.c Estado.h SaveNLoad.c SaveNLoad.h Game.h Game.c Jogada.c Jogada.h Data.c Data.h)