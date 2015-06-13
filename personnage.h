#ifndef PERSONNAGE_H_INCLUDED
#define PERSONNAGE_H_INCLUDED

#include <string>
#include "meshobj.h"

class Personnage
{
    private:
        MeshObj *model;

    public:
        double x;
        double y;
        double z;

        Personnage(std::string name);
        void setPos(double px, double py, double pz);
        void draw(double angle);
        ~Personnage();
};
        void dessiner_repere();

#endif // PERSONNAGE_H_INCLUDED
