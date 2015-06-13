#include "material.h"
#include "floatvector.h"
#include <string>

Material::Material()
{

}

Material::Material(float r, float g, float b, std::string n)
{
    name = n;
    coul.x = r;
    coul.y = g;
    coul.z = b;
}

Material::~Material()
{

}
