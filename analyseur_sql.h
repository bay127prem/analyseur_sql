#ifndef ANALYSEUR_SQL_H_INCLUDED
#define ANALYSEUR_SQL_H_INCLUDED

int debug_analyse_sql();

/*********************************************
*                Analyseur lexical
**********************************************/

#define MAX_SIZE_WORD 25
#define MAX_SYMB 11

// on accepte seulement de 0 a 99 comme valeurs pour les non terminaux, sinon ils sont des terminaux
enum terminal {
    FLOTTANT_UL = 259,
    IDETIFIANT_UL = 260,
    ENTIER_UL = 261,
    OPERATEUR_ARITHMETIQUE = 262,
    SEPARATEUR_VIRGULE = 263,
    SEPARATEUR_POINT_VIRGULE = 264,
    PARENTHESE_OUVRANTE = 265,
    PARENTHESE_FERMANTE = 266,
    OPERATEUR_RELATIONNEL = 267,
    OPERATEUR_LOGIQUE = 268,
    SELECT_KW = 269,
    FROM_KW = 270,
    WHERE_KW = 271,
    COUNT_KW = 272,
    DISTINCT_KW = 273,
    EPSILON = 111,
    FIN_SUITE_UL = 333,
    ERREUR = -99
};

typedef enum terminal XTer;    ///Terminaux (X)


typedef struct unite Unite_lexicale;
typedef Unite_lexicale* Pointeur_unite;
typedef Pointeur_unite ListeUL;

struct unite {
    char* lexeme;
    int code;
    int ligne;
    Pointeur_unite suivant_ul;
};

/***Lecture des UL***/
char* lexeme(Pointeur_unite P);
int code(Pointeur_unite P);
int ligne(Pointeur_unite P);
Pointeur_unite suivant_ul(Pointeur_unite P);

/***Lecture de la liste UL***/
void init_liste(ListeUL* L);
void afficher_liste(ListeUL L);
void afficher_liste_line(ListeUL L);
void print_unite(Pointeur_unite P);

int automate_sql(char* ch, ListeUL* liste_ul);
int automate_sql_file(char* path, ListeUL* liste_ul);

typedef struct unite Unite_lexicale;
typedef Unite_lexicale* Pointeur_unite;
typedef Pointeur_unite ListeUL;

/*********************************************
*            Analyseur syntaxique
**********************************************/

typedef int RP; ///Regle de Production
#define MAX_RP 21
#define MAX_SIZE_RP 8
#define RP_END -1
#define RP_NULL -999

/*****Les valeurs des terminaux et non terminaux sont differents*****/

// on accepte seulement de 0 a 99 comme valeurs pour les non terminaux, sinon ils sont des terminaux
enum non_terminal {
    VN_S = 0,
    VN_A = 1,
    VN_B = 2,
    VN_C = 3,
    VN_C1 = 4,
    VN_D = 5,
    VN_E = 6,
    VN_E1 = 7,
    VN_F = 8,
    VN_G = 9,
    VN_G1 = 10,
    VN_MAX = 11
};

typedef enum terminal VNTer;   ///Non terminaux (V)

int est_terminal(int val);
int syntax_sql(ListeUL liste_ul);

#endif // ANALYSEUR_SQL_H_INCLUDED
