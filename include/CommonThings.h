/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#ifndef COMMONTHINGS_H
#define COMMONTHINGS_H

#include<GL/gl.h>
#include<SOIL.h>

#include<iostream>
#include <Timer.h>

using namespace std;

#define MOVE_DIRECTION_NONE     0  // stand
#define MOVE_DIRECTION_DOWN     1  // down
#define MOVE_DIRECTION_UP       2  // up
#define MOVE_DIRECTION_RIGHT    3  // right
#define MOVE_DIRECTION_LEFT     4  // left

#define CELL_STATE_EMPTY        0  // stand
#define CELL_STATE_ENEMY        1  // up
#define CELL_STATE_WALL         2  // up
#define CELL_STATE_CHEST        3  // up
#define CELL_STATE_ARROW        4  // up

#define GAME_STATE_MENU         0  // menu
#define GAME_STATE_PLAYING      1  // play
#define GAME_STATE_WIN          2  // game win
#define GAME_STATE_OVER         3  // game over

typedef struct
{
  float x;
  float y;
}loc;

typedef struct
{
  int x;
  int y;
}GridLoc;

static GLuint TextureLoader(char* FileName)
{
   GLuint tex;
   unsigned char* image;
   int width, height;

   glGenTextures(1, &tex);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, tex);
    image = SOIL_load_image(FileName, &width, &height, 0, SOIL_LOAD_RGBA);

    if(!image) {cout<<" Cannot Find "<<FileName<<" image:<<endl";}
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    return tex;
}

#endif // COMMONTHINGS_H

