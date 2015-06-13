#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_Image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>

#include <iterator>
#include <fstream>
#include <map>
#include <sstream>

#include "sdlglutils.h"
#include "cam_trackball.h"
#include "systeme.h"
#include "personnage.h"
#include "meshobj.h"
#include "camera.h"

int tmp = 0;
int fps = 1;

bool vueArriere = false;
bool continuer = true;

GLuint tab [10];
GLuint text1;

Personnage *perso = NULL;
SDL_Joystick *joypad = NULL;
Camera *camera;

SDL_Event event;
std::ofstream flu("85ST.txt", std::ios::out);

std::map<SDLKey, bool> gp_evenement;
std::map<SDLKey, bool>::iterator it_ev;
std::map<Uint8, bool> joy_evenement;
std::map<Uint8, bool>::iterator it_joy;

void dessiner(int fps);
void stop();
void loadNumTexture();
void gestion_evenement();
void clavier();
void manette_gamecube();

int main (int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    SDL_WM_SetIcon(IMG_Load("contents/icone/fire.png"),NULL);
    init_sdl_opengl();
    loadNumTexture();

        if(SDL_NumJoysticks() != 0)
        {
            joypad = SDL_JoystickOpen(0);
            SDL_JoystickEventState(SDL_ENABLE);
        }

        text1 = loadTexture("195.gif");
        perso = new Personnage("models/mario.obj");
        camera = new Camera();

        int tmpfps = 0;
        Uint32 last_time = SDL_GetTicks();
        Uint32 current_time;
        Uint32 ellapsed_time;
        //Uint32 start_time;

        std::ostringstream oss;

        atexit(stop);

    dessiner(fps);

    while(continuer)
    {
        //start_time = SDL_GetTicks();
            gestion_evenement();

        current_time = SDL_GetTicks();
        ellapsed_time = current_time - last_time;
        last_time = current_time;

        if(tmpfps%10 == 0)
        {
            fps = 1000/ellapsed_time;
            oss.str(std::string());
            oss << "FPS : " << fps;
            SDL_WM_SetCaption(oss.str().c_str(), NULL);
        }

        if(tmpfps > 60)
            tmpfps = 0;
        else
            tmpfps++;

            dessiner(fps);

        /* Limitation des FPS
        ellapsed_time = SDL_GetTicks() - start_time;
            if(ellapsed_time < 20)
                SDL_Delay(20 - ellapsed_time);
        */
    }
    return 0;
}

// GESTION EVENEMENT
void gestion_evenement()
{
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
                        case SDL_QUIT:
                                continuer = false;
                                gp_evenement.clear();
                                joy_evenement.clear();
                                exit(0);
                            break;
                        case SDL_KEYDOWN:
                                switch(event.key.keysym.sym)
                                {
                                    case SDLK_UP:
                                            gp_evenement[SDLK_UP] = true;
                                        break;
                                    case SDLK_DOWN:
                                            gp_evenement[SDLK_DOWN] = true;
                                        break;
                                    case SDLK_LEFT:
                                            gp_evenement[SDLK_LEFT] = true;
                                        break;
                                    case SDLK_RIGHT:
                                            gp_evenement[SDLK_RIGHT] = true;
                                        break;
                                    case SDLK_d:
                                            while(tmp <= 4)
                                            {
                                                tmp++;
                                                perso->z += 0.4;
                                                dessiner(fps);
                                            }
                                            while(tmp > 0)
                                            {
                                                tmp--;
                                                perso->z -= 0.4;
                                                dessiner(fps);
                                            }
                                        break;
                                    case SDLK_p:
                                            takeScreenshot("screen.bmp");
                                        break;
                                    case SDLK_v:
                                            vueArriere = true;
                                        break;
                                    case SDLK_ESCAPE:
                                            continuer = false;
                                        break;
                                    default:
                                        break;
                                        //cam->OnKeyboard(event.key);*/
                                }
                            break;
                        /*case SDL_JOYAXISMOTION:
                                if(event.jaxis.axis == 1)
                                    perso->x += event.jaxis.value/10000;
                                if(event.jaxis.axis == 0)
                                    perso->y += event.jaxis.value/10000;
                            break;*/
                        case SDL_KEYUP:
                                switch(event.key.keysym.sym)
                                {
                                    case SDLK_v:
                                            vueArriere = false;
                                        break;
                                    case SDLK_UP:
                                            gp_evenement[SDLK_UP] = false;
                                        break;
                                    case SDLK_DOWN:
                                            gp_evenement[SDLK_DOWN] = false;
                                        break;
                                    case SDLK_LEFT:
                                            gp_evenement[SDLK_LEFT] = false;
                                        break;
                                    case SDLK_RIGHT:
                                            gp_evenement[SDLK_RIGHT] = false;
                                        break;
                                    default:
                                        break;
                                }
                            break;
                        /*case SDL_MOUSEMOTION:
                                cam->OnMouseMotion(event.motion);
                            break;
                        case SDL_MOUSEBUTTONUP:
                                cam->OnMouseButton(event.button);
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                                cam->OnMouseButton(event.button);
                            break;*/
                        case SDL_JOYBUTTONDOWN:
                            switch(event.jbutton.button)
                            {
                                case 3:
                                        perso->x = 0;
                                        perso->y = 0;
                                        camera->init_pos();
                                    break;
                                case 4:
                                        vueArriere = true;
                                    break;
                                case 1:
                                        joy_evenement[1] = true;
                                    break;
                                default:
                                    break;
                            }
                            break;
                        case SDL_JOYBUTTONUP:
                            switch(event.jbutton.button)
                            {
                                case 4:
                                        vueArriere = false;
                                    break;
                                case 1:
                                        joy_evenement[1] = false;
                                    break;
                                default:
                                    break;
                            }
                            break;
                        default:
                            break;
        }
    }

    //Gestion périphérique
    //clavier();
    manette_gamecube();
}

// CLAVIER
void clavier()
{
    for(it_ev = gp_evenement.begin(); it_ev != gp_evenement.end(); it_ev++)
    {
        if((*it_ev).second == true)
            switch((*it_ev).first)
            {
                case SDLK_RIGHT:
                        if(perso->x > -80)
                            perso->x += 0.5;
                    break;
                case SDLK_LEFT:
                        if(perso->x < 80)
                            perso->x -= 0.5;
                    break;
                case SDLK_UP:
                        if(perso->y > -80)
                            perso->y += 0.5;
                    break;
                case SDLK_DOWN:
                        if(perso->y < 80)
                            perso->y -= 0.5;
                    break;
                default:
                    break;
            }
    }
}

// MANETTE GC
void manette_gamecube()
{
        for(it_joy = joy_evenement.begin(); it_joy != joy_evenement.end(); it_joy++)
        {
            if((*it_joy).second == true)
            {
                if((*it_joy).first == 1)
                    if(perso->z < 5)
                        perso->z++;
            }
            else
            {
                if((*it_joy).first == 1)
                    if(perso->z > 0)
                        perso->z--;
            }
        }

        perso->y -= camera->changement_base_y(SDL_JoystickGetAxis(joypad, 0)/15550, SDL_JoystickGetAxis(joypad, 1)/15550);
        perso->x += camera->changement_base_x(SDL_JoystickGetAxis(joypad, 0)/15550, SDL_JoystickGetAxis(joypad, 1)/15550);
        flu << camera->getAngle() << std::endl;
}

// AFFICHAGE FPS
void affichage_fps(int fps)
{
    glPushMatrix();

        if(!vueArriere)
        {
            glTranslated(perso->x-0.5,perso->y-12.7,5);
            glRotated(-18,1,0,0);
            glRotated(-90,0,0,1);
        }
        else
        {
            glTranslated(perso->x-0.5,perso->y+12.7,5);
            glRotated(18,1,0,0);
            glRotated(90,0,0,1);
        }

        glScaled(0.3,0.3,0.3);

            glBindTexture(GL_TEXTURE_2D, tab[fps/100]); // chiffre des centaines
                glBegin(GL_QUADS);
                    glColor3ub(255,0,0); //face rouge
                    glTexCoord2d(0,1); glVertex3d(1,-1,2);
                    glTexCoord2d(0,0); glVertex3d(1,-1,0);
                    glTexCoord2d(1,0); glVertex3d(1,1,0);
                    glTexCoord2d(1,1); glVertex3d(1,1,2);
                glEnd();

            glBindTexture(GL_TEXTURE_2D, tab[((int)fps/10)%10]); // chiffre des dizaines
                glBegin(GL_QUADS);
                    glColor3ub(0,255,0); //face verte
                    glTexCoord2d(0,1); glVertex3d(1,1,2);
                    glTexCoord2d(0,0); glVertex3d(1,1,0);
                    glTexCoord2d(1,0); glVertex3d(1,3,0);
                    glTexCoord2d(1,1); glVertex3d(1,3,2);
                glEnd();

            glBindTexture(GL_TEXTURE_2D, tab[(fps%100)%10]); // chiffre des unités
                glBegin(GL_QUADS);
                    glColor3ub(0,0,255); //face bleue
                    glTexCoord2d(0,1); glVertex3d(1,3,2);
                    glTexCoord2d(0,0); glVertex3d(1,3,0);
                    glTexCoord2d(1,0); glVertex3d(1,5,0);
                    glTexCoord2d(1,1); glVertex3d(1,5,2);
                glEnd();
    glPopMatrix();
}

// DESSINER REPERE
/*void dessiner_repere()
{
    glBegin(GL_LINES);
            glColor3ub(255,0,0); // ROUGE : X
        glVertex3d(0,0,0);
        glVertex3d(7,0,0);
            glColor3ub(0,255,0); // VERT : Y
        glVertex3d(0,0,0);
        glVertex3d(0,7,0);
            glColor3ub(0,0,255); // BLEU : Z
        glVertex3d(0,0,0);
        glVertex3d(0,0,7);
    glEnd();
}*/

// DESSINER CUBE
void dessiner_cube()
{
    glPushMatrix();
        glScaled(900,900,500);
            glBindTexture(GL_TEXTURE_2D, text1);

    glBegin(GL_QUADS);
            glColor3ub(255,0,0); //face rouge
                glTexCoord2d(0,1); glVertex3d(1,1,1);
                glTexCoord2d(0,0); glVertex3d(1,1,-1);
                glTexCoord2d(1,0); glVertex3d(-1,1,-1);
                glTexCoord2d(1,1); glVertex3d(-1,1,1);

            glColor3ub(0,255,0); //face verte
                glTexCoord2d(0,10); glVertex3d(1,-1,1);
                glTexCoord2d(0,0); glVertex3d(1,-1,-1);
                glTexCoord2d(10,0); glVertex3d(1,1,-1);
                glTexCoord2d(10,10); glVertex3d(1,1,1);

            glColor3ub(0,0,255); //face bleue
                glTexCoord2d(0,10); glVertex3d(-1,-1,1);
                glTexCoord2d(0,0); glVertex3d(-1,-1,-1);
                glTexCoord2d(10,0); glVertex3d(1,-1,-1);
                glTexCoord2d(10,10); glVertex3d(1,-1,1);

            glColor3ub(255,255,0); //face jaune
                glTexCoord2d(0,10); glVertex3d(-1,1,1);
                glTexCoord2d(0,0); glVertex3d(-1,1,-1);
                glTexCoord2d(10,0); glVertex3d(-1,-1,-1);
                glTexCoord2d(10,10); glVertex3d(-1,-1,1);

            glColor3ub(0,255,255); //face cyan
                glTexCoord2d(0,10); glVertex3d(1,1,0);
                glTexCoord2d(0,0); glVertex3d(1,-1,0);
                glTexCoord2d(10,0); glVertex3d(-1,-1,0);
                glTexCoord2d(10,10); glVertex3d(-1,1,0);

            glColor3ub(255,0,255); //face magenta
                glTexCoord2d(0,10); glVertex3d(1,-1,1);
                glTexCoord2d(0,0); glVertex3d(1,1,1);
                glTexCoord2d(10,0); glVertex3d(-1,1,1);
                glTexCoord2d(10,10); glVertex3d(-1,-1,1);
        glEnd();

    glPopMatrix();
}

// DESSINER
void dessiner(int fps)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Load matrice identité, réinitilisation

    camera->setPos(perso->x, perso->y, 8);
    camera->setRetro(vueArriere);
        camera->look();

        perso->draw(camera->getAngle());

    dessiner_cube();
    //affichage_fps(fps);

    glFlush();
    SDL_GL_SwapBuffers();
}

void loadNumTexture(void)
{
    tab[0] = loadTexture("contents/fps/0.png");
    tab[1] = loadTexture("contents/fps/1.png");
    tab[2] = loadTexture("contents/fps/2.png");
    tab[3] = loadTexture("contents/fps/3.png");
    tab[4] = loadTexture("contents/fps/4.png");
    tab[5] = loadTexture("contents/fps/5.png");
    tab[6] = loadTexture("contents/fps/6.png");
    tab[7] = loadTexture("contents/fps/7.png");
    tab[8] = loadTexture("contents/fps/8.png");
    tab[9] = loadTexture("contents/fps/9.png");
}

void stop()
{
    SDL_JoystickClose(joypad);

    delete perso;
    delete camera;
    //delete [] tab;

    SDL_Quit();
}
