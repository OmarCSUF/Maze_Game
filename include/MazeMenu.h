/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */
#ifndef MAZE_MENU_H
#define MAZE_MENU_H

#include<CommonThings.h>
#include<string.h>
#include<gl/gl.h>
#include<GL/glut.h>
#include<SOIL.h>
#include<iostream>
#include <Timer.h>

using namespace std;

class MazeMenu
{
    public:
        MazeMenu();                           // Constructor
        virtual ~MazeMenu();                      // Deconstructor

        void init();
        void loadMenuImage();   // set an image for the background
        void drawMenu();                   // Draw Chest
        void drawMenuItems();                   // Draw Chest
        void drawBackground();                   // Draw Chest

        int  getMenuType();
        bool liveMenu;
        int  nPointPos;
        int  nItemCnt;
        int  getMenuItemCnt();

    private:
        GLuint menuBackTex;                     // Handler for Chest texture
        GLuint level1Tex;                     // Handler for Chest texture
        GLuint level2Tex;                     // Handler for Chest texture
        GLuint level3Tex;                     // Handler for Chest texture
        GLuint nextTex;                     // Handler for Chest texture
        GLuint replayTex;                     // Handler for Chest texture
        GLuint gameoverTex;                     // Handler for Chest texture
        GLuint gamewinTex;                     // Handler for Chest texture
        GLuint exitTex;                     // Handler for Chest texture
        GLuint menuTex;                     // Handler for Chest texture
        GLuint mazeTex;                     // Handler for Chest texture
};

#endif // MAZE_MENU_H
