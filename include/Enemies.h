/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */
#ifndef ENEMIES_H
#define ENEMIES_H

#include<CommonThings.h>
#include <time.h>
#include <thread>
#include <string>
#include <math.h>       /* ceil */

class Enemies
{
    public:
        Enemies();                          // constructor
        virtual ~Enemies();                 // DeConstructor
        void    initEnm(int, int, char *);  // Initialize Enemy
        void    freeEnum();

        void    placeEnemy(int,int);        // place enemy
        void    drawEnemy();                // Draw Enemy with Animation
        void    moveEnemy(int);          // move Enemy left,right,up,down
        void    animate();                  // Animate sprite
        GridLoc getEnemyLoc();              // Return Enemy current grid location
        loc     converter(int x, int y);

        void    chasingPlayer();            // chasing player
        bool    checkChasingPlayer();       // check to see player
        int     checkEmptyInNeiCell(int x, int y);      // In Neighbor Cell, empty cell count
        void    initMoveDirection();
        bool    isCollisionWall();
        int     gridSize;                   // Grid size of the maze
        float   unitWidth;                  // Unit width of the grid
        int     stepsPerMove;               // Step count for animation
        bool    live;                       // Status of the Enemy
        bool    bChase;                     // Status of chasing player
        int     nMoveDir;                   // move direction
        loc    enmLoc;                     // location of the enemy

    protected:

    private:
        int    frames;                     // number of frames generally
        float  t;                          // steps for animation count
        GLuint enmTex;                     // Image Texture

        float  xmax, xmin,ymax,ymin;       // Animated Texture mapping

        Timer *timer;                      // Set Timer for animation
        bool  isPassingBorder;             // enemy is passing between two cells
        loc   targCellLoc;    // target cell location
};

#endif // ENEMIES_H
