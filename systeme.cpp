#include "systeme.h"

void init_sdl_opengl()
{
    SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_OPENGL);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    gluPerspective(70, LARGEUR/HAUTEUR, 1, 1000);

    SDL_EnableKeyRepeat(1,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glClearColor(0,0,0,0);
}
