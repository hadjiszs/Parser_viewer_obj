#include "personnage.h"
#include "meshobj.h"
#include <GL/gl.h>
#include <GL/glu.h>

Personnage::Personnage(std::string name)
{
    model = new MeshObj(name);

    x = 0;
    y = 0;
    z = 0;
}

void Personnage::setPos(double px, double py, double pz)
{
    x = px;
    y = py;
    z = pz;
}

void Personnage::draw(double angle)
{
    glPushMatrix();
        glTranslated(x,y,z);
    glPushMatrix();
    /*if(angle <= 0 || angle > 360)
        glRotated(2, 0,0,1);
    else*/
        glRotated(angle, 0,0,1);
            dessiner_repere();
    glPopMatrix();
            model->draw_model();
    glPopMatrix();
}

// DESSINER REPERE
void dessiner_repere()
{
    glLineWidth(6);
    glBegin(GL_LINES);
            glColor3ub(255,0,0); // ROUGE : X
        glVertex3d(0,0,0);
        glVertex3d(10,0,0);
            glColor3ub(0,255,0); // VERT : Y
        glVertex3d(0,0,0);
        glVertex3d(0,10,0);
            glColor3ub(0,0,255); // BLEU : Z
        glVertex3d(0,0,0);
        glVertex3d(0,0,10);
    glEnd();
}

Personnage::~Personnage()
{
    delete model;
}
