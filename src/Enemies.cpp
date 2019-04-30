/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <Enemies.h>
#include <iostream>
#include <GL/global.h>

using namespace std;

Enemies::Enemies()
{
    //ctor
    enmLoc.x=0;
    enmLoc.y=0;

    xmax = 1;
    ymax = 0.25;
    xmin = 0;
    ymin = 0;
    nMoveDir = MOVE_DIRECTION_UP;
    live = true;
    isPassingBorder = false;
}

Enemies::~Enemies()
{
    if( g_nEnemies < 1 )
        return;
    // dtor
    timer->Stop();
    free(timer);
}

void Enemies::initEnm(int grid,int numFrames, char * FileName)
{
    gridSize = grid;
    unitWidth = 2.0/gridSize;
    frames = numFrames;
    xmax =1/(float)frames;
    timer = new Timer();
    timer->Start();
    t = (float)(2.0/grid)/frames/2;
    enmTex = TextureLoader(FileName);
}

void Enemies::freeEnum() {
    if( timer != nullptr ) {
        timer->Stop();
        free( timer );
        timer = nullptr;
    }
}

void Enemies::initMoveDirection(){
    int x = getEnemyLoc().x;
    int y = getEnemyLoc().y;
    enmLoc = converter(x,y);

    int candDir[4], nCand=0;

    if( y > 0 && g_Maze->pMazeData[y-1][x] != CELL_STATE_WALL ) {
        candDir[nCand++] = MOVE_DIRECTION_DOWN;
    }
    if ( y < 9 && g_Maze->pMazeData[y+1][x] != CELL_STATE_WALL) {
        candDir[nCand++] = MOVE_DIRECTION_UP;
    }
    if( x > 0 && g_Maze->pMazeData[y][x-1] != CELL_STATE_WALL ) {
        candDir[nCand++] = MOVE_DIRECTION_LEFT;
    }
    if ( x < 9 && g_Maze->pMazeData[y][x+1] != CELL_STATE_WALL) {
        candDir[nCand++] = MOVE_DIRECTION_RIGHT;
    }

    if ( nCand != 0 )
        nMoveDir = candDir[rand()%nCand];

    if ( nMoveDir == MOVE_DIRECTION_UP )    targCellLoc = converter(x,y+1);
    else if ( nMoveDir == MOVE_DIRECTION_DOWN )    targCellLoc = converter(x,y-1);
    else if ( nMoveDir == MOVE_DIRECTION_LEFT )    targCellLoc = converter(x-1,y);
    else if ( nMoveDir == MOVE_DIRECTION_RIGHT )   targCellLoc = converter(x+1,y);

    isPassingBorder = true;
}

void Enemies::drawEnemy()
{
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D,enmTex);

    glPushMatrix();
    animate();
    glTranslatef(enmLoc.x,enmLoc.y,0.0);

    glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);

    glBegin(GL_POLYGON);
    glTexCoord2f(xmin,ymin);
    glVertex3f(1,1,0.0f);

    glTexCoord2f(xmax,ymin);
    glVertex3f(-1,1,0.0f);

    glTexCoord2f(xmax,ymax);
    glVertex3f(-1,-1,0.0f);

    glTexCoord2f(xmin,ymax);
    glVertex3f(1,-1,0.0f);
    glEnd();
    glPopMatrix();
}

void Enemies::placeEnemy(int x, int y)
{
    x+=1;
    y+=1;
    enmLoc.x =  -1-unitWidth/2+(unitWidth)*x;
    enmLoc.y =  -1-unitWidth/2+(unitWidth)*y;

    initMoveDirection();
}

void Enemies::moveEnemy(int dir)
{
    nMoveDir = dir;
    if( isCollisionWall() ) {
        initMoveDirection();
    }
}

bool Enemies::isCollisionWall() {
    int x = getEnemyLoc().x;
    int y = getEnemyLoc().y;
    if( ( nMoveDir == MOVE_DIRECTION_UP && ( y == 9 || g_Maze->pMazeData[y+1][x] == CELL_STATE_WALL )) ||
        ( nMoveDir == MOVE_DIRECTION_DOWN && ( y == 0 || g_Maze->pMazeData[y-1][x] == CELL_STATE_WALL )) ||
        ( nMoveDir == MOVE_DIRECTION_RIGHT && ( x == 9 || g_Maze->pMazeData[y][x+1] == CELL_STATE_WALL )) ||
        ( nMoveDir == MOVE_DIRECTION_LEFT && ( x == 0 || g_Maze->pMazeData[y][x-1] == CELL_STATE_WALL )) ) {
            return true;
    }

    return false;
}

void Enemies::animate()
{
    if( g_nGameState != GAME_STATE_PLAYING || timer->GetTicks() < 1000/frames )
        return;

    timer->Reset();
    // ----- animate ------------
    ymin = (nMoveDir-1) * 0.25;
    ymax = ymin + 0.25;
    xmin +=1/(float)frames;
    xmax +=1/(float)frames;
    if(xmax > 1){
        xmax = 1/(float)frames;
        xmin = 0;
    }
    // -------------------------

    if( !g_Player->livePlayer )
        return;

    int x = getEnemyLoc().x;
    int y = getEnemyLoc().y;
    // ------- enemy is passing between two cells ----------------
    if( isPassingBorder ) {
        if( nMoveDir == MOVE_DIRECTION_UP ) {
            enmLoc.y += t;
            if( targCellLoc.y - enmLoc.y < t ) {
                isPassingBorder = false;
            }
        } else if( nMoveDir == MOVE_DIRECTION_DOWN ) {
            enmLoc.y -= t;
            if( enmLoc.y - targCellLoc.y < t ) {
                isPassingBorder = false;
            }
        } else if( nMoveDir == MOVE_DIRECTION_LEFT ) {
            enmLoc.x -= t;
            if( enmLoc.x - targCellLoc.x < t ) {
                isPassingBorder = false;
            }
        } else if( nMoveDir == MOVE_DIRECTION_RIGHT ) {
            enmLoc.x += t;
            if( targCellLoc.x - enmLoc.x < t ) {
                isPassingBorder = false;
            }
        }
        return;
    }
    // -----------------------------------------------------------

    isPassingBorder = true;
    // ------- enemy collision against walls or border ---------------
    if( isCollisionWall() )
    {
        initMoveDirection();
        return;
    }
    // ---------------------------------------------------------------

    int dis_pX = g_Player->getPlayerLoc().x - x;
    int dis_pY = g_Player->getPlayerLoc().y - y;
    if( abs(dis_pX) + abs(dis_pY) < 5 )
        bChase = true;
    else
        bChase = false;

    // ------ enemy is chasing player --------
    if( bChase ) {
        if( abs(dis_pY) > abs(dis_pX) ) {
            if( dis_pY > 0 ) {
                moveEnemy( MOVE_DIRECTION_UP );
            } else {
                moveEnemy( MOVE_DIRECTION_DOWN );
            }
        } else {
            if( dis_pX > 0 ) {
                moveEnemy( MOVE_DIRECTION_RIGHT );
            } else {
                moveEnemy( MOVE_DIRECTION_LEFT);
            }
        }
    }
    // -------------------------------------------

    // ----- enemy go continuously ------------------
    if( nMoveDir == MOVE_DIRECTION_UP ) {
        enmLoc.y += t;
        targCellLoc = converter(x, y+1);
    } else if(nMoveDir == MOVE_DIRECTION_DOWN) {
        enmLoc.y -= t;
        targCellLoc = converter(x, y-1);
    } else if(nMoveDir == MOVE_DIRECTION_LEFT) {
        enmLoc.x -= t;
        targCellLoc = converter(x-1, y+1);
    } else if(nMoveDir == MOVE_DIRECTION_RIGHT) {
        enmLoc.x += t;
        targCellLoc = converter(x+1, y);
    }
    // -----------------------------------------------
}


GridLoc Enemies::getEnemyLoc()
{
    GridLoc val;
    val.x = (int)(ceil((enmLoc.x +(1-unitWidth))/unitWidth));
    val.y = (int)(ceil((enmLoc.y +(1-unitWidth))/unitWidth));

    return val;
}

loc Enemies::converter(int x, int y)
{
    loc val;
    x+=1;
    y+=1;

    val.x = -1-unitWidth/2+(unitWidth)*x;
    val.y = -1-unitWidth/2+(unitWidth)*y;
    return val;
}

