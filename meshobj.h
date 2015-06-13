#ifndef MESHOBJ_H_INCLUDED
#define MESHOBJ_H_INCLUDED

#include "material.h"
#include "vertexbuffer.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <map>

class MeshObj
{
    private:

        std::map<std::string, Material*> materiaux;
        std::vector<VertexBuffer*> vbo;

    public:

        MeshObj(std::string name, MeshObj *first=NULL);
        void charger_obj(std::string name, MeshObj *first=NULL);
        void charger_mtl(std::string name);
        void draw_model();
        void giveMaterialsAndTex(MeshObj *target);
        ~MeshObj();
};

std::vector<std::string> splitSpace(std::string s);
std::string remplacerSlash(std::string s);
std::string doubleSlash(std::string s);
std::string get_directory(std::string s);
float* vector2float(std::vector<float>& tableau);

#endif // MESHOBJ_H_INCLUDED
