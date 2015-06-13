#include "meshobj.h"
#include "material.h"
#include "vertexbuffer.h"
#include <stdlib.h>
#include <iostream>
#include "sdlglutils.h"
#include <fstream>
#include <vector>
#include <map>
#include <iterator>

std::ofstream fluxSortant("TEST.txt", std::ios::out);

MeshObj::MeshObj(std::string name, MeshObj *first)
{
    charger_obj(name, first);
}

// Convertisseur : vector<float> => float*
float* vector2float(std::vector<float>& tableau)
{
    float* t = NULL;
    t = (float*) malloc(tableau.size()*sizeof(float));

    if(t == NULL || tableau.empty()) // Si tableau en param est vide (2 test)
    {
        float *t1 = (float*) malloc(sizeof(float)*3); // On envoie
        for(int i(0); i < 3; i++)                     // un tab
            t1[i] = 0;                                // avec trois 0
        return t1;
    }

    for(unsigned int i(0); i < tableau.size(); i++)
        t[i] = tableau[i];

    return t;
}

// Recupere le dossier où se trouve le .obj
std::string get_directory(std::string s)
{
    std::string res = "", s2 = "";

    for(unsigned int i(0); i < s.size(); i++)
    {
        if(s[i] == '/' || s[i] == '//')
        {
            res += s2+"/";
            s2 = "";
        }
        else
            s2 += s[i];
    }

    fluxSortant << std::endl;
    fluxSortant << std::endl;
    fluxSortant << std::endl;
    fluxSortant << "**** Fonction get_directory() **** " << std::endl;
    fluxSortant << "**** Valeur envoyée en paramètre : " << s << std::endl;
    fluxSortant << "**** Valeur retourné : " << res << std::endl << std::endl << std::endl;

    return res;
}

// Eclate la chaine de caractère là où il y a des espaces
std::vector<std::string> splitSpace(std::string s)
{
    std::vector<std::string> res;
    std::string s1 = "";

    for(unsigned int i(0); i < s.size(); i++)
    {
        if(s[i] == ' ' || i == s.size() - 1)
        {
            if(i == s.size() - 1)
                s1 += s[i];

            res.push_back(s1);
            s1 = "";
        }
        else
        {
            s1 += s[i];
        }
    }

    return res;
}

// Remplace les slashs par les espaces
std::string remplacerSlash(std::string s)
{
    std::string res = "";

    for(unsigned int i(0); i < s.size(); i++)
    {
        if(s[i] == '/')
            res += ' ';
        else
            res += s[i];
    }

    return res;
}

// Remplace les "//" par des "/1/"
std::string doubleSlash(std::string s)
{
    std::string res = "";

    for(unsigned int i(0); i < s.size(); i++)
    {
        if(s[i] == '/' && s[i+1] == '/' && i < s.size() - 1)
        {
            res += "/1/";
            i++; // sinon ça rajoute un "/" à l'itération suivante
        }
        else
        {
            res += s[i];
        }
    }

    return res;
}

// CHARGEMENT FICHIER OBJ
void MeshObj::charger_obj(std::string name, MeshObj *first)
{
    /* Parsage du fichier OBJ */

    /* On créé les Vector de FloatVector
    * afin d'y mettre les données des
    * couleurs, textures, normales, sommets
    */

    std::vector<FloatVector> ver, nor, tex, col;
    std::vector<unsigned int> iv, in, it; // tableau d'indice
    std::vector<float> tv(0), tc(0), tn(0), tt(0);

    std::string cur_material = "";
    std::string ligne;

    std::ifstream fluxToFile(name.c_str(), std::ios::in);

    int i = 0;
    bool deuxiemeFois = false;

    if(fluxToFile) // c'est ok
    {
    fluxSortant << "Ca veut bien rentrer dans le fichier" << std::endl;
        while(getline(fluxToFile,ligne))
        {
            i++;
            if(ligne[0] == 'v')
            {
                if(ligne[1] == ' ') //Vertex
                {
                    char x[255],y[255],z[255];
                    sscanf(ligne.c_str(),"v %s %s %s",x,y,z);
                    ver.push_back(FloatVector(strtod(x,NULL),strtod(y,NULL),strtod(z,NULL)));
                }
                else if(ligne[1] == 't') //Texture
                {
                    char x[255],y[255];
                    sscanf(ligne.c_str(),"vt %s %s",x,y);
                    tex.push_back(FloatVector(strtod(x,NULL),strtod(y,NULL)));
                }
                else if(ligne[1] == 'n') //Normale
                {
                    char x[255],y[255],z[255];
                    sscanf(ligne.c_str(),"vn %s %s %s",x,y,z);
                    nor.push_back(FloatVector(strtod(x,NULL),strtod(y,NULL),strtod(z,NULL)));
                }
            }
            else if(ligne[0] == 'f')
            {
                ligne = doubleSlash(ligne);
                ligne = remplacerSlash(ligne);

                std::vector<std::string> termes = splitSpace(ligne.substr(2)); // Permet d'enlever le "f "
                int ndonnees = (int)termes.size()/3;

                for(int i(0); i < (ndonnees==3?3:4); i++)
                {
                    iv.push_back(strtol(termes[i*3].c_str(), NULL,10)-1);
                    it.push_back(strtol(termes[i*3+1].c_str(), NULL,10)-1);
                    in.push_back(strtol(termes[i*3+2].c_str(), NULL,10)-1);
                }
                if(ndonnees==3) //S'il n'y a que 3 sommets on duplique le dernier pour faire un quad ayant l'apparence d'un triangle
                {
                    iv.push_back(strtol(termes[0].c_str(),NULL,10)-1);
                    it.push_back(strtol(termes[1].c_str(),NULL,10)-1);
                    in.push_back(strtol(termes[2].c_str(),NULL,10)-1);
                }

                    for(int j=0; j < 4; j++) // pour les 4 sommets
                        col.push_back(materiaux[cur_material]->coul);
            }
            else if(ligne[0] == 'm' && first == NULL)
            {
                fluxSortant << "*********Appel du fichier MTL" << std::endl;
                charger_mtl(get_directory(name)+ligne.substr((7)));
            }
            else if(ligne[0] == 'u') // utiliser un mtl : "usemtl"
            {
                    if(deuxiemeFois) // Si c'est pas la première fois qu'on rencontre le token usemtl
                    {
                                            // APPLICATION DES INDICES
                                                // Pour les vertex et les couleurs
                                                for(unsigned int i(0); i < iv.size(); i++)
                                                    if(iv[i] < ver.size())
                                                    {
                                                        tv.push_back(ver[iv[i]].x);
                                                        tv.push_back(ver[iv[i]].y);
                                                        tv.push_back(ver[iv[i]].z);

                                                        tc.push_back(col[iv[i]].x);
                                                        tc.push_back(col[iv[i]].y);
                                                        tc.push_back(col[iv[i]].z);
                                                        tc.push_back(col[iv[i]].a);
                                                    }

                                                // Pour les normales
                                                for(unsigned int i(0); i < in.size(); i++)
                                                    if(in[i] < nor.size())
                                                    {
                                                        tn.push_back(nor[in[i]].x);
                                                        tn.push_back(nor[in[i]].y);
                                                        tn.push_back(nor[in[i]].z);
                                                    }

                                                // Pour les textures
                                                for(unsigned int i(0); i < it.size(); i++)
                                                    if(it[i] < tex.size())
                                                    {
                                                        tt.push_back(tex[it[i]].x);
                                                        tt.push_back(tex[it[i]].y);
                                                    }
                                            // FIN DE L'APPLICATION

                        // TRAITEMENT DU NOUVEAU Vertexbuffer
                            // Création du nouveau Vertexbuffer
                            vbo.push_back(new VertexBuffer());

                            // Récupération du nombre de vertex à afficher pour glDrawArray()
                            vbo[vbo.size()-1]->n_data = iv.size();

                            // Chargement de texture
                            vbo[vbo.size()-1]->material.texture = materiaux[cur_material]->texture;

                            // Chargement tableau float de coordonnées
                            vbo[vbo.size()-1]->coord_vertex = vector2float(tv);
                            vbo[vbo.size()-1]->coord_normal = vector2float(tn);
                            vbo[vbo.size()-1]->coord_texture = vector2float(tt);
                            vbo[vbo.size()-1]->coord_couleur = vector2float(tc);
                        // FIN TRAITEMENT DU NOUVEAU Vertexbuffer

                        // PREPARATION DE LA MEMOIRE POUR LE PROCHAIN Vertexbuffer
                            // Suppresion du contenu des tableaux d'indice (fait lorsqu'il y a le token "f ..."
                                iv.clear();
                                in.clear();
                                it.clear();
                            // Suppresion du contenu des tableaux de vertex
                                tv.clear();
                                tn.clear();
                                tt.clear();
                                tc.clear();
                            // Suppresion du contenu du tableau couleur
                                col.clear();
                        // FIN DE LA PREPARATION MEMOIRE
                    }
                    deuxiemeFois = true;
                    cur_material = ligne.substr(7);
            }
        }
    }
    else
    {
        fluxSortant << "ERREUR AVEC OUVERTURE DE FICHIER .OBJ" << std::endl;
    }

    /* Libération de mémoire */

        ver.clear();
        nor.clear();
        tex.clear();
        col.clear();

        iv.clear();
        it.clear();
        in.clear();
    fluxSortant << "Libération de la mémoire" << std::endl;
    /* Fin de libération de mémoire */
}

// CHARGEMENT FICHIER MTL
void MeshObj::charger_mtl(std::string name)
{
    /* Parsage du fichier MTL */
    fluxSortant << "************** ON EST DANS LA FONCTION charger_mtl() ***************" << std::endl;
    std::ifstream fluxToFile(name.c_str(), std::ios::in);

    std::string cur_material = "";
    std::string ligne = "";

    if(fluxToFile)
    {
        fluxSortant << "oooooooooo CHARGEMENT DU FICHIER MTL : SUCCESS oooooooooo" << std::endl;
        while(getline(fluxToFile, ligne))
        {
            if(ligne[0] == 'n') // newmtl = nouveau materiau
            {
                cur_material = ligne.substr(7);
            }
            else if(ligne[0] == 'K' && ligne[1] == 'd') // Kd = couleur
            {
                std::vector<std::string> termes = splitSpace(ligne.substr(3));
                materiaux[cur_material] = new Material((float)strtod(termes[0].c_str(), NULL), (float)strtod(termes[1].c_str(), NULL), (float)strtod(termes[2].c_str(), NULL), cur_material);
            }
            else if(ligne[0] == 'm' && ligne[5] == 'd') // map_Kd = texture
            {
                fluxSortant << "!!!!!!!!!! CHARGEMENT DE TEXTURE !!!!!!!!!!" << std::endl;
                std::string path = get_directory(name) + ligne.substr(7);
                materiaux[cur_material]->texture = loadTexture(path.c_str());
                fluxSortant << "PATH =========>>> : " << path << std::endl;
                fluxSortant << "!!!!!!!!!! FIN DE CHARGEMENT DE TEXTURE !!!!!!!!!!" << std::endl;

            }
            else if(ligne[0] == 'd') // d = opacité
            {
                std::string n = ligne.substr(2);
                materiaux[cur_material]->coul.a = strtod(n.c_str(), NULL);
            }
        }

        fluxSortant << "oooooooooo FIN DU CHARGEMENT DU FICHIER MTL : SUCCESS oooooooooo" << std::endl;
    }
    else
    {
        fluxSortant << "ERREUR DE PUTAIN DE MERDE AVEC OUVERTURE DE FICHIER .MTL" << std::endl;
    }

    fluxSortant << "************** ON N'EST PLUS DANS LA FONCTION charger_mtl() ***************" << std::endl;
    /* Fin du parsage du fichier MTL */
}

// AFFICHAGE DU MODELE
void MeshObj::draw_model()
{
    glColor3ub(255,255,255);
    // On active les listes de sommets
    glEnableClientState(GL_VERTEX_ARRAY);

    // On active les listes de normales
    glEnableClientState(GL_NORMAL_ARRAY);

    // On active les listes de coord de textures
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // On active les listes de couleurs
    //glEnableClientState(GL_COLOR_ARRAY);

    /* On dessine les listes correspondant
    * aux différents élements
    */

    for(unsigned int i(0); i < vbo.size(); i++)
    {
        glBindTexture(GL_TEXTURE_2D, vbo[i]->material.texture);

            glVertexPointer(3, GL_FLOAT, 0, vbo[i]->coord_vertex);
            glNormalPointer(GL_FLOAT, 0, vbo[i]->coord_normal);
            glTexCoordPointer(2, GL_FLOAT, 0, vbo[i]->coord_texture);
            //glColorPointer(4, GL_FLOAT, 0, vbo[i]->coord_couleur);

        glDrawArrays(GL_QUADS, 0, vbo[i]->n_data);
    }

    //glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void MeshObj::giveMaterialsAndTex(MeshObj *target)
{

}

MeshObj::~MeshObj()
{
// Suppression de la map materiaux
    std::map<std::string, Material*>::iterator it;
    for(it = materiaux.begin(); it != materiaux.end(); it++)
        delete (*it).second;
    materiaux.clear();

    for(unsigned int i(0); i < vbo.size(); i++)
        delete vbo[i];
    vbo.clear();
}
