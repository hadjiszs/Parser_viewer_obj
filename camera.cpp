#include <GL/glu.h>
//#include <stdlib.h>
#include <cmath>
#include "camera.h"


Camera::Camera()
{
    for(unsigned int i(0); i < 3; i++)
        center[i] = 0;

    angle = 0;
    angleCam = 0;
    old_x = 0;
    old_y = 0;
    old_z = 0;

    init_pos();
}

double Camera::changement_base_x(double axe_x, double axe_y)
{
    //double res = 0;

    axe_y *= -1;

    if(axe_x*axe_y >= 0)
    {
        if(axe_x > 0) // Quadran 1
        {
            angle = acos(axe_x/(sqrt(pow(axe_x,2)+pow(axe_y,2))))*180.0 / M_PI;
        }
        else // Quadran 3
        {
            angle = acos(-axe_x/(sqrt(pow(axe_x,2)+pow(axe_y,2))))*180.0 / M_PI;
            angle += 180;
        }
    }
    else
    {
        if(axe_x < 0) // Quadran 2
        {
            angle = acos(-axe_x/(sqrt(pow(axe_x,2)+pow(axe_y,2))))*180.0 / M_PI;
            angle += 90;
        }
        else // Quadran 4
        {
            angle = acos(axe_x/(sqrt(pow(axe_x,2)+pow(axe_y,2))))*180.0 / M_PI;
            angle += 270;
        }
    }

    angleCam = acos(old_x-pos[0]/sqrt(pow(old_x-pos[0],2)+pow(old_y-pos[1],2)))*180.0 / M_PI;

    angle = fmod(angle,360.0);
    angleCam = fmod(angleCam,360.0);
    /*
        res = center[0] - pos[0];
        res *= axe_x;
        */
    return axe_x;
}

double Camera::changement_base_y(double axe_x, double axe_y)
{
    double res = 0;

        /*
        res = center[1] - pos[1];
        res *= axe_y;
        */
    return axe_y;
}

double Camera::getAngle()
{
    //double res = 0;



    return angleCam;
}

void Camera::setPos(double x, double y, double z) // En parametre : position xyz du personnage
{
    double norme = sqrt(pow(std::abs(x-pos[0]),2.0) + pow(std::abs(y-pos[1]), 2.0)); // distance entre le personnage et le placement de l'oeil

    if(norme >= 30)
    {
        pos[0] += x-old_x;
        pos[1] += y-old_y;
    }

    pos[2] = z;

    old_x = x;
    old_y = y;
    old_z = z;

    setCenter(x, y, 2);
}

void Camera::init_pos()
{
    pos[0] = 0;
    pos[1] = -15;
    pos[2] = 3;
}

void Camera::setCenter(double x, double y, double z)
{
    center[0] = x;
    center[1] = y;
    center[2] = z;
}

void Camera::setRetro(bool wat)
{
    retro = false;
}

void Camera::look()
{
    if(retro)
        pos[0] = center[0] - 15;

    gluLookAt(pos[0],pos[1],pos[2],
              center[0],center[1],center[2],
              0,0,1);
}

Camera::~Camera()
{
    delete [] pos;
    delete [] center;
}
