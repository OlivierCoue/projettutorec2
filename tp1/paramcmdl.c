/**
 * \file paramcmdl.c
 * \brief 3ieme etape du TP IL
 * \author OC & JT
 * \version 0.1
 * \date 06/12/2016
 *
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "paramcmdl.h"

/** constante chaine de caracteres pour l'affichage des types */
char * ParamTypeChaine[] = {"entier","reel","chaine"};

/**
 * \fn char * ValeurParamToString(TParamDef * _tabParam, const int _index)
 * \brief fonction qui transforme la valeur d'un parametre en chaine de caractere
 *
 * \param[in] _tabParam tableau des parametres de la ligne de commande
 * \param[in] _index indice du parametre a considerer dans le tableau
 * \return une nouvelle chaine (qu'il faudra libérer par la suite)
*/
char * ValeurParamToString(TParamDef * _tabParam, const int _index) {
    char *out = (char *) malloc(100 * sizeof(char));    
    switch(_tabParam[_index].type){
      case PTentier:
        snprintf(out, 100, "-%c %s (%s) [%d]\n", _tabParam[_index].lettre, _tabParam[_index].nom, ParamTypeChaine[0], _tabParam[_index].valeur.entier);
        break;
      case PTreel:      
        snprintf(out, 100, "-%c %s (%s) [%f]\n", _tabParam[_index].lettre, _tabParam[_index].nom, ParamTypeChaine[1], _tabParam[_index].valeur.reel);
        break;
      case PTchaine:
        snprintf(out, 100, "-%c %s (%s) [%s]\n", _tabParam[_index].lettre, _tabParam[_index].nom, ParamTypeChaine[2], _tabParam[_index].valeur.chaine);      
        break;
    }
    return out;
}

/**
 * \fn PrintParam(TParamDef * _tabParam, const int _nbParam)
 * \brief fonction qui affiche a l'ecran les parametre, leur type et leur valeur
 *
 * \param[in] _tabParam tableau des parametres de la ligne de commande
 * \param[in] _nbParam taille du tableau
 * \return neant
*/
void PrintParam(TParamDef * _tabParam, const int _nbParam) {
  int i;
  for(i=0; i<_nbParam; i++){
    printf("%s",ValeurParamToString(_tabParam, i));
  }
}

/**
 * \fn int ReadParamFromCommandLine(TParamDef * _tabParam, const int _nbParam, const int _argc, const char * _argv[])
 * \brief fonction qui analyse la ligne de commande pour en extraire des valeurs pour les parametres
 *
 * \param[out] _tabParam tableau des parametres de la ligne de commande
 * \param[in] _nbParam taille du tableau
 * \param[in] _argc nombre d'arguments passes sur la ligne de commande
 * \param[in] _argv tableau qui contient les chaines de caracteres passees en arguments du programme
 * \return >=0 : nombre de parametres mis a jour, <0 : erreur
*/
int ReadParamFromCommandLine(TParamDef * _tabParam, const int _nbParam, const int _argc, const char * _argv[]) {
  int i, j, parametersSettedNb = 0;

  for(i=1; i<_argc; i++){
    for(j=0; j<_nbParam; j++){
        if(_argv[i][1] == _tabParam[j].lettre){
            switch(_tabParam[j].type){
                case PTentier:
                    _tabParam[j].valeur.entier = strtol(_argv[i+1], (char **)NULL, 10);
                    break;
                case PTreel:
                    _tabParam[j].valeur.reel = atof(_argv[i+1]);              
                    break;
                case PTchaine:
                    _tabParam[j].valeur.chaine = _argv[i+1];              
                    break;
            }
            parametersSettedNb++;        
        }
    }    
    i++;
  }
  return parametersSettedNb;
}