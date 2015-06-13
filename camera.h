#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

class Camera
{
    private:
        // Ancienne position du personnage
        double old_x;
        double old_y;
        double old_z;

        // Position du cadre de la camera
        double pos [3];

        // Point fixe du centre de l'écran
        double center [3];


        bool retro;

        double angle;
        double angleCam;
    public:
        Camera();
        double changement_base_x(double axe_x, double axe_y);
        double changement_base_y(double axe_x, double axe_y);
        double getAngle();
        void setPos(double x, double y, double z);
        void setCenter(double x, double y, double z);
        void setRetro(bool wat);
        void init_pos();
        void look();
        ~Camera();
};

#endif // CAMERA_H_INCLUDED
