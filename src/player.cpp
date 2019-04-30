/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <Player.h>
#include <GL/global.h>

Timer *T = new Timer();                             // Set Timer for animation

Player::Player()
{
    //ctor
    playerDir = MOVE_DIRECTION_DOWN;
    arrowLoc.x = 0.2;
    arrowLoc.y =0.5;
    arrAngle =0;
    arrowStatus = false;
    livePlayer = true;
}

Player::~Player()
{
    //dtor
}

void Player::drawArrow()
{
    if(arrowStatus)
    {
        glColor3f(1.0,1.0,1.0);

        if(T->GetTicks()>10)
        {
            if(arrowLoc.x>=-1 && arrowLoc.x<=1)
                arrowLoc.x += t*arrXdir;
            else
                arrowStatus = false;
            if(arrowLoc.y>=-1 && arrowLoc.y<=1)
                arrowLoc.y += t*arrYdir;
            else
                arrowStatus = false;
            T->Reset();
        }

        glTranslatef(arrowLoc.x ,arrowLoc.y,0.0);
        glRotated(arrAngle,0,0,1);
        glBindTexture(GL_TEXTURE_2D,arrowTex);
        glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);

        glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(1,0);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(1,1);
        glVertex3f(-1,-1,0.0f);

        glTexCoord2f(0,1);
        glVertex3f(1,-1,0.0f);
        glEnd();
    }
}

void Player::shootArrow()
{
    if(livePlayer)
    {
        if(!arrowStatus)
        {
            arrowStatus=true;

            if(playerDir == MOVE_DIRECTION_LEFT)
            {
                arrAngle = -90;
                arrXdir=-1;
                arrYdir=0;
                arrowLoc.x = plyLoc.x-unitWidth/2;
                arrowLoc.y = plyLoc.y;
            }
            if(playerDir == MOVE_DIRECTION_RIGHT)
            {
                arrAngle =90;
                arrXdir=1;
                arrYdir=0;
                arrowLoc.x = plyLoc.x+unitWidth/2;
                arrowLoc.y = plyLoc.y;
            }
            if(playerDir == MOVE_DIRECTION_UP)
            {
                arrXdir=0;
                arrYdir=1;
                arrAngle =180;

                arrowLoc.y = plyLoc.y+unitWidth/2;
                arrowLoc.x = plyLoc.x;
            }
            if(playerDir == MOVE_DIRECTION_DOWN)
            {
                arrXdir=0;
                arrYdir=-1;
                arrAngle= 0;

                arrowLoc.y = plyLoc.y-unitWidth/2;
                arrowLoc.x = plyLoc.x;
            }
        }
    }
}

void Player::loadArrowImage(char* fileName)
{
    arrowTex = TextureLoader(fileName);
}

GridLoc Player::getArrowLoc()
{
    GridLoc val;

    val.x = (int)(ceil((arrowLoc.x +(1-unitWidth))/unitWidth));
    val.y = (int)(ceil((arrowLoc.y +(1-unitWidth))/unitWidth));

    return val;
}

void Player::drawplayer()
{
    if(g_nGameState != GAME_STATE_MENU )
    {
        glColor3f(1.0,1.0,1.0);

        glTranslatef(plyLoc.x ,plyLoc.y,0.0);

        glBindTexture(GL_TEXTURE_2D,plyTex);
        glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);


        glBegin(GL_QUADS);
        glTexCoord2f(xmin,ymin);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(xmax,ymin);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(xmax,ymax);
        glVertex3f(-1,-1,0.0f);

        glTexCoord2f(xmin,ymax);
        glVertex3f(1,-1,0.0f);
        glEnd();

    }
}

void Player::initPlayer(int gSize,int frams, char *FileName)
{
    frames = frams;
    gridSize = gSize;

    xmax =1/(float)frames;
    ymax =0.25;
    xmin =0;
    ymin =0;

    unitWidth = (float)2/gridSize;
    stepsPerMove = frames;

    T->Start();
    plyLoc.x= -unitWidth;
    plyLoc.y= -unitWidth;

    t= unitWidth/stepsPerMove;

    plyTex = TextureLoader(FileName);
}

void Player::placePlayer(int x, int y)
{
    plyLoc.x =  converter(x,y).x;
    plyLoc.y =  converter(x,y).y;
}

void Player::movePlayer(int dir)
{
    if( !livePlayer ) return;
    if( T->GetTicks() < 10 )
        return;

    int x = getPlayerLoc().x;
    int y = getPlayerLoc().y;

    if( ( (playerDir == MOVE_DIRECTION_LEFT || playerDir == MOVE_DIRECTION_RIGHT) &&
          (dir == MOVE_DIRECTION_UP || dir == MOVE_DIRECTION_DOWN) ) ||
        ( (playerDir == MOVE_DIRECTION_UP || playerDir == MOVE_DIRECTION_DOWN) &&
          (dir == MOVE_DIRECTION_LEFT || dir == MOVE_DIRECTION_RIGHT) ) )
    {
        plyLoc = converter( x, y );
    }

    playerDir = dir;

    float fMax = 1.0f;
    if( playerDir == MOVE_DIRECTION_LEFT ) {
        if( x > 0 && g_Maze->pMazeData[y][x-1] == CELL_STATE_WALL ) {
            fMax = 1-x*unitWidth;
        }

        if(plyLoc.x>-fMax+unitWidth/2)
            plyLoc.x -= t;
    } else if( playerDir == MOVE_DIRECTION_RIGHT ) {
        if( x < 9 && g_Maze->pMazeData[y][x+1] == CELL_STATE_WALL ) {
            fMax = 1-(9-x)*unitWidth;
        }

        if(plyLoc.x<fMax-unitWidth/2)
            plyLoc.x += t;

    } else if( playerDir == MOVE_DIRECTION_UP ) {
        if( x < 9 && g_Maze->pMazeData[y+1][x] == CELL_STATE_WALL ) {
            fMax = 1-(9-y)*unitWidth;
        }

        if(plyLoc.y< fMax-unitWidth/2)
            plyLoc.y += t;
    } else if( playerDir == MOVE_DIRECTION_DOWN ) {
        if( x > 0 && g_Maze->pMazeData[y-1][x] == CELL_STATE_WALL ) {
            fMax = 1-y*unitWidth;
        }

        if(plyLoc.y > -fMax+unitWidth/2)
            plyLoc.y -= t;
    }

    xmin += 1/(float)frames;
    xmax += 1/(float)frames;
    ymin = (playerDir-1) * 0.25;
    ymax = ymin + 0.25;

    if( xmax >= 1 ){
        xmax = 1 /(float)frames;
        xmin = 0;
    }

    T->Reset();
}

GridLoc Player::getPlayerLoc()
{
   GridLoc val;

   val.x = (int)(ceil((plyLoc.x +(1-unitWidth))/unitWidth));
   val.y = (int)(ceil((plyLoc.y +(1-unitWidth))/unitWidth));

   return val;

}

loc Player::converter(int x, int y)
{
    loc val;
    x+=1;
    y+=1;

    val.x = -1-unitWidth/2+(unitWidth)*x;
    val.y = -1-unitWidth/2+(unitWidth)*y;
    return val;
}
