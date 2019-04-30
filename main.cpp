/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <string.h>
#include <CommonThings.h>
#include <iostream>
#include <Timer.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <GL/global.h>

#include <wall.h>
#include <iostream>
#include <fstream>
#include <cmath>

/* GLUT callback Handlers */

Maze        *g_Maze = new Maze(10);                 // Set Maze grid size
Player      *g_Player = new Player();               // create player
MazeMenu    *g_menu = new MazeMenu();
Enemies     g_Enemy[10];                            // create number of enemies
int         g_nEnemies;

wall        g_wall[100];                                 // wall with number of bricks
int         g_nWalls;
Timer       *T0 = new Timer();                      // animation timer

float wWidth, wHeight;                          // display window width and Height
float xPos,yPos;                                // Viewpoar mapping

void display(void);                             // Main Display : this runs in a loop
void loadMazeFromFile();
void onBtnExit();
void onBtnLevel( int nLevel);
void onBtnMenu();
void onBtnEnter();

int  g_nGameState = GAME_STATE_MENU;
int  g_nLevel = 0;

void resize(int width, int height)              // resizing case on the window
{
    wWidth = width;
    wHeight = height;

    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}
void initMaze() {
    g_menu->init();

    int i = 0;
    for (i = 0; i < g_nEnemies; i++ ) {
        g_Enemy[i].freeEnum();
    }
    g_nEnemies = 0;
    g_Player->placePlayer(9,9);

    for( i = 0; i < g_nWalls; i++ )
        g_wall[i].liveWall = false;
    g_nWalls = 0;
}

void startGame() {
    if( g_nGameState == GAME_STATE_PLAYING ) {
        loadMazeFromFile();
        g_Player->livePlayer = true;
    } else
        g_menu->init();
}

void init()
{
    glEnable(GL_COLOR_MATERIAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0, wWidth, 0, wHeight);

    T0->Start();                                        // set timer to 0

    glEnable(GL_BLEND);                                 //display images with transparent
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    g_Maze->loadBackgroundImage("images/bak.jpg");           // Load maze background image
    g_Maze->loadChestImage("images/chest.png");              // load chest image
    g_Maze->loadSetOfArrowsImage("images/arrwset.png");      // load set of arrows image
    g_Player->initPlayer(g_Maze->getGridSize(),6,"images/p.png");   // initialize player pass grid size,image and number of frames
    g_Player->loadArrowImage("images/arr.png");                // Load arrow imageg
    g_Maze->loadMenuImage("images/menuback.png");
    g_menu->loadMenuImage();

    g_nGameState = GAME_STATE_MENU;
    startGame();
}

char szLevelName[32];
void loadMazeFromFile() {
    initMaze();
    int  nLine = 0;
    int i, j;

    sprintf(szLevelName, "maze%d.txt", g_nLevel+1);
    ifstream file(szLevelName);
    string  line;
    char    ch;
    int     st;

    while (getline(file, line) && nLine < 10 )
    {
        st = 0; j = 0;
        if( nLine == 0 )    st = 3;

        for( i = st; i < st+10; i++, j++ ) {
            ch = line.at(i);
            g_Maze->pMazeData[nLine][j] = atoi(&ch);
        }
        nLine++;
    }

    int nAdr = 0;
    g_nEnemies = 0;
    bool  bChest = false, bStArrw = false;
    for( i = 0; i < 10; i++ ) {
        for( j = 0; j < 10; j++, nAdr++) {
            if( g_Maze->pMazeData[i][j] == CELL_STATE_EMPTY )
                continue;

            if( g_Maze->pMazeData[i][j] == CELL_STATE_ENEMY && g_nEnemies < 10) {
                g_Enemy[g_nEnemies].initEnm(g_Maze->getGridSize(), 4, "images/e.png");
                g_Enemy[g_nEnemies].placeEnemy( j, i );
                g_nEnemies++;
            } else if ( g_Maze->pMazeData[i][j] == CELL_STATE_WALL && g_nWalls < 100 ) {
                g_wall[g_nWalls].wallInit(g_Maze->getGridSize(),"images/wall.png");// Load walls
                g_wall[g_nWalls].placeWall(j, i);
                g_nWalls++;
            } else if ( g_Maze->pMazeData[i][j] == CELL_STATE_ARROW && !bStArrw ) {
                g_Maze->placeStArrws(j, i);
                bStArrw = true;                             // place each brick
            } else if ( g_Maze->pMazeData[i][j] == CELL_STATE_CHEST && !bChest ) {
                g_Maze->placeChest(j, i);
                bChest = true;                             // place each brick
            } else {
                g_Maze->pMazeData[i][j] = CELL_STATE_EMPTY;
            }
        }
    }
}

float distTwoGridLoc( loc loc1, loc loc2 ) {
    return abs(loc1.x-loc2.x) + abs(loc1.y - loc2.y);
}

bool isGameOver() {
    if( !g_Player->livePlayer )
        return false;
    int i;
    GridLoc enmPos, playerPos = g_Player->getPlayerLoc();
    for( i = 0; i < g_nEnemies; i++ ) {
        if( distTwoGridLoc( g_Player->plyLoc, g_Enemy[i].enmLoc) < g_Maze->unitWidth / 2 ) {
            g_Player->livePlayer = false;
            g_nGameState = GAME_STATE_OVER;
            g_menu->init();
            return true;
        }
    }

    return false;
}

bool isGameWin() {
    if( !g_Player->livePlayer )
        return false;
    if( distTwoGridLoc( g_Player->plyLoc, g_Maze->chestLoc ) < g_Maze->unitWidth / 4 )
    {
        g_Player->livePlayer = false;
        g_nGameState = GAME_STATE_WIN;
        g_menu->init();
        return true;
    }

    return false;
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);        // clear display screen

    glPushMatrix();
        g_Maze->drawBackground();
    glPopMatrix();

    for(int i=0; i<g_nWalls;i++)
       g_wall[i].drawWall();

    glPushMatrix();
        g_Maze->drawGrid();
    glPopMatrix();

    glPushMatrix();
        g_Player->drawplayer();
    glPopMatrix();

    for(int i=0; i<g_nEnemies;i++)
    {
        g_Enemy[i].drawEnemy();
    }

    glPushMatrix();
        g_Player->drawArrow();
    glPopMatrix();

    glPushMatrix();
        g_Maze->drawChest();
    glPopMatrix();

    glPushMatrix();
        g_Maze->drawArrows();
    glPopMatrix();

    //glPushMatrix();
        g_menu->drawMenu();
    //glPopMatrix();

    glutSwapBuffers();

    // ----
    if( isGameWin() ){
        // show win dialog
        //MessageBox(*(HWND *) KeyloggerFrame::GetHandle(), _("Text"), _("Title"), MB_OK);
        //wxMessageBox(_("Text"), _("Title"));
    }
    if( isGameOver() ) {
        // show gameover dialog
    }
}

void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ' ':
            // if(!g_Maze->liveSetOfArrws)      // if setof arrows were picked by player
            g_Player->shootArrow();
        break;
        case 27 :                       // esc key to exit
        case 'q':
            onBtnExit();
            break;
        case 13:                                // enter key to start from menu
             onBtnEnter();
             break;
   }

    glutPostRedisplay();
}

void GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    xPos = posX ;
    yPos = posY ;
}

void idle(void)
{
    //Your Code here
    glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y){

    switch(btn){
    case GLUT_LEFT_BUTTON:
        if(state==GLUT_DOWN){
          GetOGLPos(x,y);
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if(state==GLUT_DOWN){
            GetOGLPos(x,y);
        }
        break;
    }
    glutPostRedisplay();
};

void onBtnMenu() {
    g_nGameState = GAME_STATE_MENU;
    g_menu->init();
}

void onBtnExit() {
    exit(0);
}

void onBtnLevel( int nLevel ) {
    g_nLevel = nLevel;
    g_nGameState = GAME_STATE_PLAYING;
    startGame();
}

void onBtnEnter() {
    if( g_nGameState == GAME_STATE_PLAYING )
        return;

    int cnt = g_menu->getMenuItemCnt(), nPos;
    if( cnt < 1 )
        return;

    if( g_nGameState == GAME_STATE_MENU ) {
        if( g_menu->nPointPos < g_menu->getMenuItemCnt()-1 ) {
            onBtnLevel(g_menu->nPointPos);
        } else {
            onBtnExit();
        }
    } else if ( g_nGameState == GAME_STATE_WIN ) {
        nPos = g_menu->nPointPos;
        if( g_nLevel == 2 ) {
            nPos++;
        }

        if( nPos == 0 ) {
            onBtnLevel(g_nLevel+1);
        } else if ( nPos == 1 ) {
            onBtnLevel(g_nLevel);
        } else {
            onBtnMenu();
        }
    } else if ( g_nGameState == GAME_STATE_OVER ) {
        nPos = g_menu->nPointPos;
        if( nPos == 0 ) {
            onBtnLevel(g_nLevel);
        } else {
            onBtnMenu();
        }
    }
}

void Specialkeys(int key, int x, int y)
{
    // Your Code here
    if( g_nGameState == GAME_STATE_PLAYING ) {
        switch(key)
        {
        case GLUT_KEY_UP:
             cout<< g_Player->getPlayerLoc().x<< "    "<<g_Player->getPlayerLoc().y<<endl;
             g_Player->movePlayer(MOVE_DIRECTION_UP);
             break;

        case GLUT_KEY_DOWN:
             cout<< g_Player->getPlayerLoc().x<< "    "<<g_Player->getPlayerLoc().y<<endl;
             g_Player->movePlayer(MOVE_DIRECTION_DOWN);
             break;

        case GLUT_KEY_LEFT:
             cout<< g_Player->getPlayerLoc().x<< "    "<<g_Player->getPlayerLoc().y<<endl;
             g_Player->movePlayer(MOVE_DIRECTION_LEFT);
             break;

        case GLUT_KEY_RIGHT:
             cout<< g_Player->getPlayerLoc().x<< "    "<<g_Player->getPlayerLoc().y<<endl;
             g_Player->movePlayer(MOVE_DIRECTION_RIGHT);
             break;
       }
    } else {
        int nPos = g_menu->nPointPos;
        switch(key)
        {
        case GLUT_KEY_DOWN:
             g_menu->nPointPos = ( nPos == g_menu->getMenuItemCnt()-1 )? 0: nPos+1;
             break;

        case GLUT_KEY_UP:
             g_menu->nPointPos = ( nPos == 0 )? g_menu->getMenuItemCnt()-1: nPos-1;
             break;
      }
    }

   glutPostRedisplay();
}

/* Program entry point */
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (800, 800);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Maze");                    //program title

   init();

   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutSpecialFunc(Specialkeys);
   glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glutMainLoop();

   return EXIT_SUCCESS;
}
