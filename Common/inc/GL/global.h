/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */


#ifndef GLOBLA_H
#define GLOBLA_H

#include <Maze.h>
#include <player.h>
#include <Enemies.h>
#include <MazeMenu.h>

extern Maze     *g_Maze;
extern Player   *g_Player;
extern MazeMenu *g_menu;
extern Enemies  g_Enemy[10];
extern int      g_nEnemies;
extern Enemies     g_Enemy[10];                            // create number of enemies
extern int         g_nEnemies;

int  extern     g_nGameState;
int  extern     g_nLevel;
#endif // ENEMIES_H

