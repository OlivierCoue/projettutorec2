/**
 * \file tp1_b.c
 * \brief 2ieme etape du TP IL
 * \author NM
 * \version 0.1
 * \date 17/11/2014
 *
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * \enum TParamType
 * \brief Constantes pour le type des parametres de la ligne de commande
 */
typedef enum {
    PTentier, /**< un nombre entier */
    PTreel, /**< un nombre reel */
    PTchaine /**< une chaine de caracteres */
  } TParamType;

/** constante chaine de caracteres pour l'affichage des types */
char * ParamTypeChaine[] = {"entier","reel","chaine"};

/**
 * \union TParamValue
 * \brief union permettant de  manipuler un entier/reel/chaine
 */
typedef union {
    int entier;
    float reel;
    const char * chaine;
  } TParamValue;

/**
 * \struct TParamDef
 * \brief represente un parametre de la ligne de commande (nom,type,valeur...)
 */
typedef struct {
    char * nom; /**< nom du parametre */
    TParamType type; /**< type (entier,reel,chaine) */
    char lettre; /**< lettre a utiliser sur la ligne de commande */
    TParamValue valeur; /**< valeur a affecter au parametre */
  } TParamDef;

/**
 * \fn char * ValeurParamToString(TParamDef * _tabParam, const int _index)
 * \brief fonction qui transforme la valeur d'un parametre en chaine de caractere
 *
 * \param[in] _tabParam tableau des parametres de la ligne de commande
 * \param[in] _index indice du parametre a considerer dans le tableau
 * \return une nouvelle chaine (qu'il faudra libérer par la suite)
*/
char * ValeurParamToString(TParamDef * _tabParam, const int _index) {
/***** A ECRIRE *****/
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
    switch(_tabParam[i].type){
      case PTentier:
        printf("-%c %s (%s) [%d]\n", _tabParam[i].lettre, _tabParam[i].nom, ParamTypeChaine[0], _tabParam[i].valeur.entier);
        break;
      case PTreel:      
        printf("-%c %s (%s) [%f]\n", _tabParam[i].lettre, _tabParam[i].nom, ParamTypeChaine[1], _tabParam[i].valeur.reel);
        break;
      case PTchaine:
        printf("-%c %s (%s) [%s]\n", _tabParam[i].lettre, _tabParam[i].nom, ParamTypeChaine[2], _tabParam[i].valeur.chaine);      
        break;
    } 
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
  int i, parametersSettedNb = 0;

  for(i=1; i<_argc; i++){

    if(strcmp(_argv[i], "-s")==0){
      _tabParam[0].valeur.chaine = _argv[i+1];
      parametersSettedNb++;
    }else if(strcmp(_argv[i], "-a")==0){
      _tabParam[1].valeur.chaine = _argv[i+1];
      parametersSettedNb++;
    }else if(strcmp(_argv[i], "-t")==0){
      _tabParam[2].valeur.entier = strtol(_argv[i+1], (char **)NULL, 10);
      parametersSettedNb++;
    }

    i++;
  }
  return (parametersSettedNb==3) ? 1 : -1;
}

/**
 * \fn int main (const int _argc, const char * _argv[])
 * \brief fonction principale
 *
 * \param[in] _argc : nombre d'arguments passes sur la ligne de commande
 * \param[in] _argv : tableau qui contient les chaines de caracteres passes en arguments du programme
 * \return 0 si terminaison normale
*/
int main (const int _argc, const char * _argv[]) {
  /* declaration des paramtres avec leur type+valeurs par defaut */
  TParamDef tab_param[] = {
    {"serveur",PTchaine,'s',.valeur.chaine="??"},
    {"appli",PTchaine,'a',.valeur.chaine=""},
    {"tours",PTentier,'t',.valeur.entier=200}};
  int nb_param = 3;
  int result_arg;

  /* affichage des formats de parametre */
  printf("Valeurs par defaut :\n");
  PrintParam(tab_param,nb_param);
  /* analyse de la ligne de commande */
  result_arg = ReadParamFromCommandLine(tab_param,nb_param,_argc,_argv);
  /* affichage des nouveaux parametres */
  printf("Valeurs des parametres :\n");
  PrintParam(tab_param,nb_param);


  return 0;
}
