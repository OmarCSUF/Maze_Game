/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <MazeMenu.h>
#include <GL/global.h>

int     typeArray[] = { GAME_STATE_MENU, GAME_STATE_WIN, GAME_STATE_OVER };
GLuint  texArray[3][10];

MazeMenu::MazeMenu(){
    liveMenu = false;
    nPointPos = 0;
    nItemCnt = 0;
}

MazeMenu::~MazeMenu(){
    //dtor
}

void MazeMenu::init() {
    nPointPos = 0;
    nItemCnt = getMenuItemCnt();

    if( g_nGameState == GAME_STATE_PLAYING )
        liveMenu = false;
    else
        liveMenu = true;
}

void MazeMenu::loadMenuImage() {
    menuBackTex = TextureLoader("images/menuback.png");
    menuTex = TextureLoader("images/menu.png");
    level1Tex = TextureLoader("images/level1.png");
    level2Tex = TextureLoader("images/level2.png");
    level3Tex = TextureLoader("images/level3.png");
    nextTex = TextureLoader("images/next.png");
    replayTex = TextureLoader("images/replay.png");
    gameoverTex = TextureLoader("images/gameover.png");
    gamewinTex = TextureLoader("images/gamewin.png");
    exitTex = TextureLoader("images/exit.png");
    mazeTex = TextureLoader("images/maze.png");

    texArray[0][0] = level1Tex;
    texArray[0][1] = level2Tex;
    texArray[0][2] = level3Tex;
    texArray[0][3] = exitTex;

    texArray[1][0] = nextTex;
    texArray[1][1] = replayTex;
    texArray[1][2] = menuTex;

    texArray[2][0] = replayTex;
    texArray[2][1] = menuTex;
}

void MazeMenu::drawBackground() {
    float     fGreedSize = (float)g_Maze->getGridSize();

    glPushMatrix();
        glColor3f(1.0,1.0,1.0);
        glTranslatef(0,0,0.0);
        glBindTexture(GL_TEXTURE_2D, menuBackTex);
        glScaled(4.0/fGreedSize,(nItemCnt+1)/fGreedSize,1.0);

        glBegin(GL_QUADS);
            glTexCoord2f(0,1);
            glVertex3f(1,-1,0.0f);

            glTexCoord2f(0,0);
            glVertex3f(1,1,0.0f);

            glTexCoord2f(1,0);
            glVertex3f(-1,1,0.0f);

            glTexCoord2f(1,1);
            glVertex3f(-1,-1,0.0f);
        glEnd();
    glPopMatrix();
}

int MazeMenu::getMenuType() {
    int i;
    for( i = 0; i < 3; i++ ) {
        if( typeArray[i] == g_nGameState )
            return i;
    }
    return 0;
}

void MazeMenu::drawMenuItems() {
    if( nItemCnt < 1 )
        return;

    float     fGreedSize = (float)g_Maze->getGridSize();

    int pos = 0;
    int type = getMenuType();
    float fOneHeight = 1.0/fGreedSize;

    // draw Title -----------------------
    glPushMatrix();
        glColor3f(1.0,1.0,1.0);
        glTranslatef(0, fOneHeight*(nItemCnt+4)/2,1.0);

        if( g_nGameState == GAME_STATE_MENU ){
            glBindTexture(GL_TEXTURE_2D,mazeTex);
        } else if (g_nGameState == GAME_STATE_WIN ) {
            glBindTexture(GL_TEXTURE_2D,gamewinTex);
        } else if (g_nGameState == GAME_STATE_OVER ) {
            glBindTexture(GL_TEXTURE_2D,gameoverTex);
        }

        glScaled(4.0/fGreedSize,fOneHeight,0.0);

        glBegin(GL_QUADS);
            glTexCoord2f(0,1);
            glVertex3f(-1,-1,0.0f);

            glTexCoord2f(0,0);
            glVertex3f(-1,1,0.0f);

            glTexCoord2f(1,0);
            glVertex3f(1,1,0.0f);

            glTexCoord2f(1,1);
            glVertex3f(1,-1,0.0f);
        glEnd();
    glPopMatrix();
    // ----------------------------------

    for( pos = 0; pos < nItemCnt; pos++ ) {
    glPushMatrix();
        glColor3f(1.0,1.0,1.0);
        glTranslatef(0, fOneHeight*(nItemCnt-2*pos-2),1.0);

        int n;

        if( g_nGameState == GAME_STATE_WIN && g_nLevel == 2 ){
            glBindTexture(GL_TEXTURE_2D,texArray[type][pos+1]);
        } else {
            glBindTexture(GL_TEXTURE_2D,texArray[type][pos]);
        }

        n = (int)(pos==nPointPos);
        glScaled(3.0/fGreedSize,fOneHeight,0.0);

        float ymin = (float)n * 0.5;
        float ymax = ymin + 0.5;

        glBegin(GL_QUADS);
            glTexCoord2f(0,ymax);
            glVertex3f(-1,-1,0.0f);

            glTexCoord2f(0,ymin);
            glVertex3f(-1,1,0.0f);

            glTexCoord2f(1,ymin);
            glVertex3f(1,1,0.0f);

            glTexCoord2f(1,ymax);
            glVertex3f(1,-1,0.0f);
        glEnd();
    glPopMatrix();
    }
}

void MazeMenu::drawMenu() {
    if( g_nGameState == GAME_STATE_PLAYING || nItemCnt < 1 )
        return;

    drawBackground();
    drawMenuItems();
}

int MazeMenu::getMenuItemCnt() {
    if( g_nGameState == GAME_STATE_MENU )
        return 4;

    if( g_nGameState == GAME_STATE_OVER )
        return 2;

    if( g_nGameState == GAME_STATE_WIN )
    {
        if( g_nLevel == 2 ) {
            return 2;
        }
        return 3;
    }

    return 0;
}
