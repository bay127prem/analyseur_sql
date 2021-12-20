#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analyseur_sql.h"

/*********************************************
*          Définition    de  la pile
*          d'analyse syntaxique LL(1)
**********************************************/

typedef struct cellule Cellule;
typedef Cellule* Pointeur_cellule;
typedef Pointeur_cellule Pile;

struct cellule {
    int valeur;
    Pointeur_cellule suivant;
};

void allouer(Pointeur_cellule* P);
void liberer(Pointeur_cellule P);
int valeur(Pointeur_cellule P);
Pointeur_cellule suivant(Pointeur_cellule P);
void aff_adr(Pointeur_cellule P, Pointeur_cellule Q);
void aff_val(Pointeur_cellule P, int valeur);
void init_pile(Pile* L);
void empiler(Pile* L, int val);
int depiler(Pile* L, int* val);
int pile_vide(Pile P);
void afficher_pile(Pile L);
void print_nul_ptr_err();


/*********************************************
*          Définition  des fonctions
*          d'analyse syntaxique LL(1)
**********************************************/

RP get_analyse_table(VNTer A, XTer x);
int get_RP_element(int num, int num_element);

int regles_production[MAX_RP][MAX_SIZE_RP] = {
    {VN_S, SEPARATEUR_POINT_VIRGULE, VN_D, VN_C, FROM_KW, VN_A, SELECT_KW, RP_END}, /// S -> SELECT A FROM C D SEP_;
    {PARENTHESE_FERMANTE, VN_B, PARENTHESE_OUVRANTE, COUNT_KW, RP_END, RP_END, RP_END, RP_END}, /// A-> COUNT PAR_( B PAR_)
    {VN_B, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END}, /// A-> B
    {VN_C, DISTINCT_KW, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END}, /// B-> DISTINCT C
    {VN_C, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END}, /// B-> C
    {VN_C1, IDETIFIANT_UL, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END},    /// C-> IDENT C1
    {VN_B, SEPARATEUR_VIRGULE, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END},    /// C1-> SEP_, B
    {EPSILON, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END},    /// C1-> EPSILONE
    {VN_E, WHERE_KW, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END},   /// D-> WHERE E
    {EPSILON, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END},  /// D-> EPSILONE
    {VN_E1, VN_F, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END},   /// E-> F E1
    {VN_E, OPERATEUR_LOGIQUE, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END},   /// E1-> OP_LOG E
    {EPSILON, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END},   /// E1 -> EPSILONE
    {VN_G, OPERATEUR_RELATIONNEL, VN_G, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END},   /// F-> G OP_REL G
    {VN_G1, PARENTHESE_FERMANTE, VN_G, PARENTHESE_OUVRANTE,  RP_END, RP_END, RP_END, RP_END},   /// G-> PAR_( G PAR_) G1
    {VN_G1, IDETIFIANT_UL, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END},   /// G-> IDENT G1
    {VN_G1, ENTIER_UL, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END},   /// G-> ENTIER G1
    {VN_G1, FLOTTANT_UL, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END},   /// G-> FLOTTANT_UL G1
    {VN_G, OPERATEUR_ARITHMETIQUE, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END},  /// G1-> OP_ARTH G
    {EPSILON, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END},  ///G1-> EPSILONE
    {EPSILON, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END, RP_END}
    /// S-> EPSILONE
};


/*********************************************
*          Implementation des fonctions
*          d'analyse syntaxique LL(1)
**********************************************/

/// retourne un XTer ou VNTer (X union V)
int get_RP_element(int num, int num_element) {
    return regles_production[num][num_element];
}

RP get_analyse_table(VNTer A, XTer x) {
    switch (A) {
        case VN_S:
            if (x == SELECT_KW)
                return 0;
            else if (x == FIN_SUITE_UL)
                return 20;
            else
                return RP_NULL;
            break;
        case VN_A:
            if (x == DISTINCT_KW)
                return 2;
            else if (x == COUNT_KW)
                return 1;
            else if (x == IDETIFIANT_UL)
                return 2;
            else
                return RP_NULL;
            break;
        case VN_B:
            if (x == DISTINCT_KW)
                return 3;
            else if (x == IDETIFIANT_UL)
                return 4;
            else
                return RP_NULL;
            break;
        case VN_C:
            if (x == IDETIFIANT_UL)
                return 5;
            else
                return RP_NULL;
            break;
        case VN_C1:
            if (x == FROM_KW)
                return 7;
            else if (x == WHERE_KW)
                return 7;
            else if (x == SEPARATEUR_VIRGULE)
                return 6;
            else if (x == SEPARATEUR_POINT_VIRGULE)
                return 7;
            else if (x == PARENTHESE_FERMANTE)
                return 7;
            else
                return RP_NULL;
            break;
        case VN_D:
            if (x == WHERE_KW)
                return 8;
            else if (x == SEPARATEUR_POINT_VIRGULE)
                return 9;
            else
                return RP_NULL;
            break;
        case VN_E:
            if (x == IDETIFIANT_UL)
                return 10;
            else if (x == ENTIER_UL)
                return 10;
            else if (x == FLOTTANT_UL)
                return 10;
            else if (x == PARENTHESE_OUVRANTE)
                return 10;
            else
                return RP_NULL;
            break;
        case VN_E1:
            if (x == SEPARATEUR_POINT_VIRGULE)
                return 12;
            else if (x == OPERATEUR_LOGIQUE)
                return 11;
            else
                return RP_NULL;
            break;
        case VN_F:
            if (x == IDETIFIANT_UL)
                return 13;
            else if (x == ENTIER_UL)
                return 13;
            else if (x == FLOTTANT_UL)
                return 13;
            else if (x == PARENTHESE_OUVRANTE)
                return 13;
            else
                return RP_NULL;
            break;
        case VN_G:
            if (x == IDETIFIANT_UL)
                return 15;
            else if (x == ENTIER_UL)
                return 16;
            else if (x == FLOTTANT_UL)
                return 16;
            else if (x == PARENTHESE_OUVRANTE)
                return 14;
            else
                return RP_NULL;
            break;
        case VN_G1:
            if (x == OPERATEUR_ARITHMETIQUE)
                return 18;
            else if (x == SEPARATEUR_POINT_VIRGULE)
                return 19;
            else if (x == OPERATEUR_RELATIONNEL)
                return 19;
            else if (x == OPERATEUR_LOGIQUE)
                return 19;
            else if (x == PARENTHESE_FERMANTE)
                return 19;
            else
                return RP_NULL;
            break;
        default:
            return RP_NULL;
            break;
    }
}

int est_terminal(int val) {
    if (val == ERREUR) return 1;
    else if (val > 99) return 1;
    else return 0;
}

int syntax_sql(ListeUL liste_ul) {
    Pile pile;
    init_pile(&pile);

    empiler(&pile, FIN_SUITE_UL);
    empiler(&pile, VN_S);

    if (debug_analyse_sql()) {
        printf("\n");
        afficher_liste_line(liste_ul);
        afficher_pile(pile);
    }

    int sommet, rp;
    XTer a;

    while (!pile_vide(pile)) {
        a = code(liste_ul);
        depiler(&pile, &sommet);

        if (!est_terminal(sommet)) {
            rp = get_analyse_table(sommet, a);
            if (rp == RP_NULL) {
                return 0;
            }
            else {
                for (int i=0; (get_RP_element(rp, i)!=RP_END) && i < MAX_SIZE_RP; ++i) {
                    int element = get_RP_element(rp, i);
                    if (element != EPSILON) empiler(&pile, element);
                }
            }
            if (debug_analyse_sql()) {
                printf("table_analyse[%d, %d] = %d\n", sommet, a, rp);
            }
        }
        else {
            if ((sommet == a) && (sommet != FIN_SUITE_UL)) {
                if (debug_analyse_sql())
                    printf("%d == %d\n", sommet, a);
                liste_ul = suivant_ul(liste_ul);
            }
            else if ((sommet == FIN_SUITE_UL) && (a == FIN_SUITE_UL)) {
                if (debug_analyse_sql())
                    printf("%d == %d\n", sommet, a);
                return 1;
            }
            else {
                if (debug_analyse_sql())
                    printf("%d != %d\n", sommet, a);
                return 0;
            }
        }
        if (debug_analyse_sql()) {
            printf("\n");
            afficher_liste_line(liste_ul);
            afficher_pile(pile);
        }
    }
    return 0;
}

/*********************************************
*          Implementation de la pile
*          d'analyse syntaxique LL(1)
**********************************************/

void print_nul_ptr_err() {
    printf("!***** Erreur: Pointeur Nul hhh *****!\n");
}

void allouer(Pointeur_cellule* P) {
    *P = (Pointeur_cellule) malloc(sizeof(Cellule));
    aff_val(*P, NULL);
    aff_adr(*P, NULL);
}

void liberer(Pointeur_cellule P) {
    if (P == NULL) {
        print_nul_ptr_err();
        return;
    }
    free(P);
}

int valeur(Pointeur_cellule P) {
    if (P == NULL) {
        print_nul_ptr_err();
        return;
    }
    return P->valeur;
}

Pointeur_cellule suivant(Pointeur_cellule P) {
    if (P == NULL) {
        print_nul_ptr_err();
        return;
    }
    return P->suivant;
}

void aff_adr(Pointeur_cellule P, Pointeur_cellule Q) {
    if (P == NULL) {
        print_nul_ptr_err();
        return;
    }
    P->suivant = Q;
}

void aff_val(Pointeur_cellule P, int valeur) {
    if (P == NULL) {
        print_nul_ptr_err();
        return;
    }
    P->valeur = valeur;
}

void init_pile(Pile* L) {
    (*L) = NULL;
}

void empiler(Pile* L, int val) {
    Pointeur_cellule P;
    allouer(&P);
    aff_val(P, val);
    aff_adr(P, *L);
    (*L) = P;
}

int depiler(Pile* L, int* val) {
    if ((*L) != NULL) {
        Pointeur_cellule P = (*L);
        *val = valeur(*L);
        (*L) = suivant(*L);
        liberer(P);
        return 1;
    }
    else
        return 0;
}

void afficher_pile(Pile L) {
    printf("PileNVT: ");
    if (L == NULL) {
        printf("La pile est vide");
    }
    else {
        while (L != NULL) {
            printf("%d\t", valeur(L));
            L = suivant(L);
        }
    }
    printf("\n");
}

int pile_vide(Pile P) {
    return (P == NULL);
}
