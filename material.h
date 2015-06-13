#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#include <string>
#include "floatvector.h"
#include <GL/gl.h>
#include <GL/glu.h>

class Material
{
    public:

        Material();
        Material(float r, float g, float b, std::string n);
        ~Material();

        GLuint texture;
        FloatVector coul;
        std::string name;
};

#endif // MATERIAL_H_INCLUDED
