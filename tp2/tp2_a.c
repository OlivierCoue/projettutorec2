/**
 * \file tp2_a.c
 * \brief analyseur lexical pour le langage JSON
 * \author NM
 * \version 0.1
 * \date 25/11/2015
 *
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#define JSON_LEX_ERROR -1 /**< code d'erreur lexicale */
#define JSON_TRUE 1 /**< entite lexicale true */
#define JSON_FALSE 2 /**< entite lexicale false */
#define JSON_NULL 3 /**< entite lexicale null */
#define JSON_LCB 4 /**< entite lexicale { */
#define JSON_RCB 5 /**< entite lexicale } */
#define JSON_LB 6 /**< entite lexicale [ */
#define JSON_RB 7 /**< entite lexicale ] */
#define JSON_COMMA 8 /**< entite lexicale , */
#define JSON_COLON 9 /**< entite lexicale : */
#define JSON_STRING 10 /**<entite lexicale chaine de caracteres */
#define JSON_INT_NUMBER 11 /**< entite lexicale nombre entier */
#define JSON_REAL_NUMBER 12 /**< entite lexicale nombre reel */

/**
 * \union TSymbole
 * \brief union permettant de  manipuler un entier/reel/chaine pour la table
 * des symboles
 */
typedef struct {
  int type; /**< l'un des 3 types suivants : JSON_STRING/JSON_INT_NUMBER/JSON_REAL_NUMBER */
  union {
        int entier;
        float reel;
        char * chaine;
    } val; /**< valeur associer a un element de la table des symboles */
} TSymbole;

/**
 * \struct TLex
 * \brief structure contenant tous les parametres/donnees pour
 * l'analyse lexicale
 */
typedef struct{
  char * data; /**< chaine a parcourir */
  char * startPos; /**< position de depart pour la prochaine analyse */
  int nbLignes; /**< nb de lignes analysees */
  TSymbole * tableSymboles; /**< tableau des symboles : chaines/entier/reel */
  int nbSymboles; /**< taille du tableau tableSymboles */
} TLex;

/**
 * \fn int isSep(char _symb)
 * \brief fonction qui teste si un symbole fait partie des separateurs
 *
 * \param[in] _symb symbole a analyser
 * \return 1 (vrai) si _symb est un separateur, 0 (faux) sinon
 */
int isSep(const char _symb) {
  int res = (_symb=='{' | _symb=='}' | _symb=='[' | _symb==']' | _symb==',' | _symb==':') ? 1 : 0;
  return res;
}

/**
 * \fn TLex * initLexData(char * _data)
 * \brief fonction qui reserve la memoire et initialise les
 * donnees pour l'analyseur lexical
 *
 * \param[in] _data chaine a analyser
 * \return pointeur sur la structure de donnees creee
 */
TLex * initLexData(char * _data) {
  TLex *lex;
  lex = (TLex*)malloc(sizeof(TLex));
  if(lex==NULL){
    printf("Allocation error");
    exit(-1);
  }
  lex->data = strdup(_data);
  lex->startPos = lex->data;
  lex->nbLignes = 1;
  lex->tableSymboles = NULL;
  lex->nbSymboles = 0;
  return lex;
}

/**
 * \fn void deleteLexData(TLex ** _lexData)
 * \brief fonction qui supprime de la memoire les donnees pour
 * l'analyseur lexical
 *
 * \param[in/out] _lexData donnees de l'analyseur lexical
 * \return neant
 */
void deleteLexData(TLex ** _lexData) {
  int i;
  free((*_lexData)->data);  
  for(i=0;i<(*_lexData)->nbSymboles;i++){
    if((*_lexData)->tableSymboles[i].type==JSON_STRING)
      free((*_lexData)->tableSymboles[i].val.chaine);
  }
  free((*_lexData)->tableSymboles);
  free((*_lexData));
}

/**
 * \fn void printLexData(TLex * _lexData)
 * \brief fonction qui affiche les donnees pour
 * l'analyseur lexical
 *
 * \param[in] _lexData donn�es de l'analyseur lexical
 * \return neant
 */
void printLexData(TLex * _lexData) {
  int i;
  printf("\n\nTable Symboles:\n");
  for(i=0;i<_lexData->nbSymboles;i++){
    printf("n°%d\n", i);
    switch(_lexData->tableSymboles[i].type){
      case JSON_STRING:
        printf("type: STRING\nval: %s\n\n",_lexData->tableSymboles[i].val.chaine);
        break;
      case JSON_INT_NUMBER:
        printf("type: INT\nval: %d\n\n",_lexData->tableSymboles[i].val.entier);
        break;
      case JSON_REAL_NUMBER:
        printf("type: REAL\nval: %f\n\n",_lexData->tableSymboles[i].val.reel);
        break;
    }
  }
}


/**
 * \fn void addIntSymbolToLexData(TLex * _lexData, const int _val)
 * \brief fonction qui ajoute un symbole entier a la table des symboles
 *
 * \param[in/out] _lexData donnees de l'analyseur lexical
 * \param[in] _val valeur entiere e ajouter
 * \return neant
 */
void addIntSymbolToLexData(TLex * _lexData, const int _val) {
  _lexData->tableSymboles = (TSymbole*)realloc(_lexData->tableSymboles, (_lexData->nbSymboles+1)*sizeof(TSymbole));  
  _lexData->tableSymboles[_lexData->nbSymboles].type = JSON_INT_NUMBER;
  _lexData->tableSymboles[_lexData->nbSymboles].val.entier = _val;
  _lexData->nbSymboles++;
}

/**
 * \fn void addRealSymbolToLexData(TLex * _lexData, const float _val)
 * \brief fonction qui ajoute un symbole reel a la table des symboles
 *
 * \param[in/out] _lexData donnees de l'analyseur lexical
 * \param[in] _val valeur reelle a ajouter
 */
void addRealSymbolToLexData(TLex * _lexData, const float _val) {
  _lexData->tableSymboles = (TSymbole*)realloc(_lexData->tableSymboles, (_lexData->nbSymboles+1)*sizeof(TSymbole));  
  _lexData->tableSymboles[_lexData->nbSymboles].type = JSON_REAL_NUMBER;
  _lexData->tableSymboles[_lexData->nbSymboles].val.reel = _val;
  _lexData->nbSymboles++;
}

 /**
 * \fn void addStringSymbolToLexData(TLex * _lexData, char * _val)
 * \brief fonction qui ajoute une chaine de caracteres a la table des symboles
 *
 * \param[in/out] _lexData donnees de l'analyseur lexical
 * \param[in] _val chaine a ajouter
 */
void addStringSymbolToLexData(TLex * _lexData, char * _val) {
  _lexData->tableSymboles = (TSymbole*)realloc(_lexData->tableSymboles, (_lexData->nbSymboles+1)*sizeof(TSymbole));  
  _lexData->tableSymboles[_lexData->nbSymboles].type = JSON_STRING;
  _lexData->tableSymboles[_lexData->nbSymboles].val.chaine = strdup(_val);
  _lexData->nbSymboles++;
}


 /**
 * \fn void parseString(TLex * _lexData)
 * \brief fonction qui parse une chaine de charactère et l'ajoute à la table des symbole
 *
 * \param[in/out] _lexData donnees de l'analyseur lexical
 */
void parseString(TLex * _lexData){
  int len;
  char *buffer = strdup("");
  _lexData->startPos++;
  while(_lexData->startPos[0]!='\"' || (_lexData->startPos-1)[0]=='\\'){            
    len = strlen(buffer);
    buffer = (char*)realloc(buffer,len+2);
    buffer[len]=_lexData->startPos[0];
    buffer[len+1]='\0';
    _lexData->startPos++;
  }
  addStringSymbolToLexData(_lexData, buffer);
  free(buffer);
}

 /**
 * \fn int parseNumber(TLex * _lexData)
 * \brief fonction qui parse un nombre (entier ou réel) et l'ajoute à la table des symbole
 *
 * \param[in/out] _lexData donnees de l'analyseur lexical
  * \return un entier corespondant au code entier ou réel
 */
int parseNumber(TLex * _lexData){
  int len;
  char *buffer = strdup("");
  char *ptr;
  while(isSep(_lexData->startPos[0])==0){    
    len = strlen(buffer);
    buffer = (char*)realloc(buffer,len+2);
    buffer[len]=_lexData->startPos[0];
    buffer[len+1]='\0';
    _lexData->startPos++;
  }

  ptr = strchr(buffer,'.');
  if(ptr==NULL){
    addIntSymbolToLexData(_lexData, atoi(buffer));
    free(buffer);
    return JSON_INT_NUMBER;
  }else{
    addRealSymbolToLexData(_lexData, atof(buffer));
    free(buffer);
    return JSON_REAL_NUMBER;
  }
}

/**
 * \fn int lex(const char * _entree, TLex * _lexData)
 * \brief fonction qui effectue l'analyse lexicale (contient le code l'automate fini)
 *
 * \param[in/out] _lexData donn�es de suivi de l'analyse lexicale
 * \return code d'identification de l'entite lexicale trouvee
*/
int lex(TLex * _lexData) {  
  int res;
  /* check if its the end of the string */
  if(_lexData->startPos[0]=='\0')
    return JSON_LEX_ERROR;

  while(_lexData->startPos[0]==' ')
    _lexData->startPos++;

  if(_lexData->startPos[0]=='\"'){
    parseString(_lexData); 
    res = JSON_STRING;
  }else if(_lexData->startPos[0]>='0'&&_lexData->startPos[0]<='9'){
    res = parseNumber(_lexData);
  }else if(_lexData->startPos[0]=='{')
    res = JSON_LCB;
  else if(_lexData->startPos[0]=='}')
    res = JSON_RCB;
  else if(_lexData->startPos[0]=='[')
    res = JSON_LB;
  else if(_lexData->startPos[0]==']')
    res = JSON_RB;
  else if(_lexData->startPos[0]==',')
    res = JSON_COMMA;
  else if(_lexData->startPos[0]==':')
    res = JSON_COLON;
  else if(_lexData->startPos[0]=='t'){
    _lexData->startPos+=4;
    res = JSON_TRUE;
  }else if(_lexData->startPos[0]=='f'){
    _lexData->startPos+=5;
    res = JSON_FALSE;
  }

  _lexData->startPos++;
  return res;
}


/**
 * \fn int main()
 * \brief fonction principale
 */
int main() {
  char * test;
  int i;
  TLex * lex_data;

  test = strdup("{\"obj1\": [ {\"obj2\": 12, \"obj3\":\"text1 \\\"and\\\" text2\"},\n {\"obj4\":314.32} ], \"obj5\": true }");
  printf("%s",test);
  printf("\n");

  lex_data = initLexData(test);
  i = lex(lex_data);
  while (i!=JSON_LEX_ERROR) {
    printf("lex()=%d\n",i);
    i = lex(lex_data);
  }
  printLexData(lex_data);
  deleteLexData(&lex_data);
  free(test);
  return 0;
}
