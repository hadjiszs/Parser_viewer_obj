#include "cam_trackball.h"
#include <GL/glu.h>
#include <cmath>
#include "sdlglutils.h"

trackBall::trackBall()
{
    const char *hand1[] =
        {
            /* width height num_colors chars_per_pixel */
            " 16 16 3 1 ",
            /* colors */
            "X c #000000",
            ". c #ffffff",
            "  c None",
            /* pixels */
            "       XX       ",
            "   XX X..XXX    ",
            "  X..XX..X..X   ",
            "  X..XX..X..X X ",
            "   X..X..X..XX.X",
            "   X..X..X..X..X",
            " XX X.......X..X",
            "X..XX..........X",
            "X...X.........X ",
            " X............X ",
            "  X...........X ",
            "  X..........X  ",
            "   X.........X  ",
            "    X.......X   ",
            "     X......X   ",
            "     X......X   ",
            "0,0"
        };

    const char *hand2[] =
        {
            /* width height num_colors chars_per_pixel */
            " 16 16 3 1 ",
            /* colors */
            "X c #000000",
            ". c #ffffff",
            "  c None",
            /* pixels */
            "                ",
            "                ",
            "                ",
            "                ",
            "    XX XX XX    ",
            "   X..X..X..XX  ",
            "   X........X.X ",
            "    X.........X ",
            "   XX.........X ",
            "  X...........X ",
            "  X...........X ",
            "  X..........X  ",
            "   X.........X  ",
            "    X.......X   ",
            "     X......X   ",
            "     X......X   ",
            "0,0"
        };
    _hand1 = cursorFromXPM(hand1); //création du curseur normal
    _hand2 = cursorFromXPM(hand2); //création du curseur utilisé quand le bouton est enfoncé
    SDL_SetCursor(_hand1); //activation du curseur normal
    _hold = false; //au départ on part du principe que le bouton n'est pas maintenu
    _angleY = 0;
    _angleZ = 0;
    _distance = 5; //distance initiale de la caméra avec le centre de la scène
    _motionSensivity = 0.3;
    _scrollSensivity = 1;
}

void trackBall::OnMouseMotion(const SDL_MouseMotionEvent & event)
{
    if(_hold == true)
    {
        _angleZ += event.xrel*_motionSensivity;
        _angleY += event.yrel*_motionSensivity;

        if(_angleY >=90)
            _angleY = 90;
        else if(_angleY <= -90)
            _angleZ = -90;
    }
}

void trackBall::OnMouseButton(const SDL_MouseButtonEvent & event)
{
    if(event.button == SDL_BUTTON_LEFT)
    {
        if((_hold) && (event.type == SDL_MOUSEBUTTONUP))
        {
            _hold = false;
            SDL_SetCursor(_hand1);
        }
        else if((!_hold) && (event.type == SDL_MOUSEBUTTONDOWN))
        {
            _hold = true;
            SDL_SetCursor(_hand2);
        }
    }
    else if(event.button == SDL_BUTTON_WHEELUP)
    {
        _distance += 1*_scrollSensivity;
    }
    else if(event.button == SDL_BUTTON_WHEELDOWN)
    {
        _distance -= 1*_scrollSensivity;
        if(_distance < 0.1)
            _distance = 0.1;
    }
}

void trackBall::OnKeyboard(const SDL_KeyboardEvent & event)
{
    if((event.type == SDL_KEYDOWN)&&(event.keysym.sym == SDLK_SPACE))
    {
        _angleY = 0;
        _angleZ = 0;
    }
}

void trackBall::setMotionSensivity(double sensivity)
{
    _motionSensivity = sensivity;
}

void trackBall::setScrollSensivity(double sensivity)
{
    _scrollSensivity = sensivity;
}

void trackBall::look()
{
    gluLookAt(_distance, 0, 0,
              0, 0, 0,
              0, 0, 1);
    glRotated(_angleY, 0, 1, 0);
    glRotated(_angleZ, 0, 0, 1);
}

trackBall::~trackBall()
{
    SDL_FreeCursor(_hand1);
    SDL_FreeCursor(_hand2);
    SDL_SetCursor(NULL);
}
