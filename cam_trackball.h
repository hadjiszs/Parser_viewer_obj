#ifndef CAM_TRACKBALL_H_INCLUDED
#define CAM_TRACKBALL_H_INCLUDED

#include <SDL/SDL.h>

class trackBall
{
    public:
        trackBall();

        virtual void OnMouseMotion(const SDL_MouseMotionEvent & event);
        virtual void OnMouseButton(const SDL_MouseButtonEvent & event);
        virtual void OnKeyboard(const SDL_KeyboardEvent & event);

        virtual void look();
        virtual void setMotionSensivity(double sensivity);
        virtual void setScrollSensivity(double sensivity);

        virtual ~trackBall();

    protected:
        double _motionSensivity;
        double _scrollSensivity;
        bool _hold;
        double _distance;
        double _angleY;
        double _angleZ;
        SDL_Cursor * _hand1; // temps normal
        SDL_Cursor * _hand2; // quand bouton gauche enfoncé
};

#endif // CAM_TRACKBALL_H_INCLUDED
