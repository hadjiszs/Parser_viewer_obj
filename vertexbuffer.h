#ifndef VERTEXBUFFER_H_INCLUDED
#define VERTEXBUFFER_H_INCLUDED

#include "material.h"

class VertexBuffer
{
    public:

        VertexBuffer();
        ~VertexBuffer();

        int n_data;
        float *coord_vertex;
        float *coord_normal;
        float *coord_texture;
        float *coord_couleur;

        Material material;
};

#endif // VERTEXBUFFER_H_INCLUDED
