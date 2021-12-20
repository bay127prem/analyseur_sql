#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analyseur_sql.h"

void aff_adr_ul(Pointeur_unite P, Pointeur_unite Q);
void aff_lexeme(Pointeur_unite P, char* lexeme);
void aff_code(Pointeur_unite P, int code);
void aff_ligne(Pointeur_unite P, int ligne);
void print_nul_ptr_err_ul();
void allouer_ul(Pointeur_unite* P);
void liberer_ul(Pointeur_unite P);
void ajouter_debut(ListeUL* L, char* lexeme, int code, int ligne);
Pointeur_unite ajouter_fin(ListeUL* L, char* lexeme, int code, int ligne);

int SYMB_TAB[MAX_SYMB][2] = {
    {'+', OPERATEUR_ARITHMETIQUE},
    {'/', OPERATEUR_ARITHMETIQUE},
    {'-', OPERATEUR_ARITHMETIQUE},
    {'*', OPERATEUR_ARITHMETIQUE},
    {'=', OPERATEUR_RELATIONNEL},
    {'(', PARENTHESE_OUVRANTE},
    {')', PARENTHESE_FERMANTE},
    {',', SEPARATEUR_VIRGULE},
    {';', SEPARATEUR_POINT_VIRGULE},
    {'>', OPERATEUR_RELATIONNEL},
    {'<', OPERATEUR_RELATIONNEL}
};

int get_code_symb(char* str, int etat) {
    if (etat == 0) {
        for (int i=0; i<MAX_SYMB; ++i) {
            if (str[0] == SYMB_TAB[i][0])
                return SYMB_TAB[i][1];
        }
        return EPSILON;
    }
    else if ((etat == 1) || (etat == 5)) {
        if (strcasecmp(str, "SELECT") == 0) {
            return SELECT_KW;
        }
        else if (strcasecmp(str, "WHERE") == 0) {
            return WHERE_KW;
        }
        else if (strcasecmp(str, "COUNT") == 0) {
            return COUNT_KW;
        }
        else if (strcasecmp(str, "DISTINCT") == 0) {
            return DISTINCT_KW;
        }
        else if (strcasecmp(str, "FROM") == 0) {
            return FROM_KW;
        }
        else if (strcasecmp(str, "AND") == 0) {
            return OPERATEUR_LOGIQUE;
        }
        else if (strcasecmp(str, "OR") == 0) {
            return OPERATEUR_LOGIQUE;
        }
        else {
            return IDETIFIANT_UL;
        }
    }
    else if (etat == 2) {
        return ENTIER_UL;
    }
    else if (etat == 4) {
        return FLOTTANT_UL;
    }
    else
        return ERREUR;  /// non final
}

void allouer_ul(Pointeur_unite* P) {
    *P = (Pointeur_unite) malloc(sizeof(Unite_lexicale));
    aff_lexeme(*P, NULL);
    aff_code(*P, NULL);
    aff_ligne(*P, NULL);
    aff_adr_ul(*P, NULL);
}

void liberer_ul(Pointeur_unite P) {
    if (P == NULL) {
        print_nul_ptr_err_ul();
        return;
    }
    free(P);
}

char* lexeme(Pointeur_unite P) {
    if (P == NULL) {
        print_nul_ptr_err_ul();
        return;
    }
    return P->lexeme;
}

int code(Pointeur_unite P) {
    if (P == NULL) {
        print_nul_ptr_err_ul();
        return;
    }
    return P->code;
}

int ligne(Pointeur_unite P) {
    if (P == NULL) {
        print_nul_ptr_err_ul();
        return;
    }
    return P->ligne;
}

Pointeur_unite suivant_ul(Pointeur_unite P) {
    if (P == NULL) {
        print_nul_ptr_err_ul();
        return;
    }
    return P->suivant_ul;
}

void aff_adr_ul(Pointeur_unite P, Pointeur_unite Q) {
    if (P == NULL) {
        print_nul_ptr_err_ul();
        return;
    }
    P->suivant_ul = Q;
}

void aff_lexeme(Pointeur_unite P, char* lexeme) {
    if (P == NULL) {
        print_nul_ptr_err_ul();
        return;
    }
    P->lexeme = lexeme;
}

void aff_code(Pointeur_unite P, int code) {
    if (P == NULL) {
        print_nul_ptr_err_ul();
        return;
    }
    P->code = code;
}

void aff_ligne(Pointeur_unite P, int ligne) {
    if (P == NULL) {
        print_nul_ptr_err_ul();
        return;
    }
    P->ligne = ligne;
}

void print_nul_ptr_err_ul() {
    printf("!***** Erreur: Pointeur Nul *****!\n");
}

void init_liste(ListeUL* L) {
    (*L) = NULL;
}

void ajouter_debut(ListeUL* L, char* lexeme, int code, int ligne) {
    Pointeur_unite P;
    allouer_ul(&P);
    aff_lexeme(P, lexeme);
    aff_code(P, code);
    aff_ligne(P, ligne);
    aff_adr_ul(P, *L);
    (*L) = P;
}

Pointeur_unite ajouter_fin(ListeUL* L, char* lexeme, int code, int ligne) {
    Pointeur_unite P;
    allouer_ul(&P);
    aff_lexeme(P, lexeme);
    aff_code(P, code);
    aff_ligne(P, ligne);
    aff_adr_ul(P, NULL);
    if ((*L) == NULL) {
        (*L) = P;
    }
    else {
        Pointeur_unite Q = (*L);
        while (suivant_ul(Q) != NULL) {
            Q = suivant_ul(Q);
        }
        aff_adr_ul(Q, P);
    }
    return P;
}

void ajouter_fin_rapide(ListeUL* L, Pointeur_unite* P, char* lexeme, int code, int ligne) {
    if ((*P) == NULL)
        ajouter_fin(L, lexeme, code, ligne);
    else
        (*P) = ajouter_fin(P, lexeme, code, ligne);
}

void afficher_liste(ListeUL L) {
    if (L == NULL) {
        printf("La liste est vide\n");
    }
    else {
        printf("Structure d'une unite : (lexeme | code | ligne)\n");
        while (L != NULL) {
            print_unite(L);
            L = suivant_ul(L);
        }
    }
}

void afficher_liste_line(ListeUL L) {
    printf("SuiteUL: ");
    if (L == NULL) {
        printf("La pile est vide");
    }
    else {
        while (L != NULL) {
            printf("%d\t", code(L));
            L = suivant_ul(L);
        }
    }
    printf("\n");
}

void print_unite(Pointeur_unite P) {
    if (P == NULL)
        printf("Pas de unite\n");
    else
        printf("%x : (%s | %d | %d) | %x\n", P, lexeme(P), code(P), ligne(P), suivant_ul(P));
}

int mot_vide(char* str) {
    return (str[0] == '\0');
}

void init_mot(char** mot, int taille) {
    (*mot) = malloc(taille+1);
    (*mot)[0] = '\0';
}

int isdelim(char c) {

    return ((c == '\n') || (c == ' ') || (c == '+') || (c == '-') || (c == '/') || (c == '*') || (c == '(') || (c == ')') || (c == '=') || (c == '>') || (c == '<') || (c == ',') || (c == ';'));
}

int isdot(char c) {
    return (c == '.');
}

int automate_sql(char* ch, ListeUL* liste_ul) {
    int qc = 0; /// etat courant
    int qp = -1; /// etat precedent
    int error = 0;
    int ligne_c = 0;
    char* mot_c;
    char* mot;
    int cpt = 0;    /// compteur des caracteres des mots (maximum)
    Pointeur_unite P = NULL;

    init_mot(&mot_c, MAX_SIZE_WORD);
    init_liste(liste_ul);

    for (int i=0; (ch[i] != '\0') && (!error) && (cpt <= MAX_SIZE_WORD); ++i) {
        qp = qc;
        switch (qc) {
            case 0:
                if (isalpha(ch[i])) qc=1;
                else if (isdigit(ch[i])) qc=2;
                else if (isdot(ch[i])) qc=3;
                else if (isdelim(ch[i])) qc=0;
                else error=1;
                break;
            case 1:
                if ((isalpha(ch[i])) || (isdigit(ch[i])) || (ch[i] == '_')) qc=1;
                else if (isdelim(ch[i])) qc=0;
                else error=1;
                break;
            case 2:
                if (isdigit(ch[i])) qc=2;
                else if (isdot(ch[i])) qc=3;
                else if (isdelim(ch[i])) qc=0;
                else error=1;
                break;
            case 3:
                if (isdigit(ch[i])) qc=4;
                else error=1;
                break;
            case 4:
                if (isdigit(ch[i])) qc=4;
                else if (isdelim(ch[i])) qc=0;
                else error=1;
                break;
            case 5:
                if (isdigit(ch[i])) qc=2;
                else if (isdelim(ch[i])) qc=0;
                else if (isalpha(ch[i])) qc=1;
                else error=1;
                break;
            default:
                error=1;
                break;
        }

        if (error) qc = -1;

        if (qc == 0) {      /// Il y a un delimiteur
            char* sep_mot;
            sep_mot = malloc(2);
            sep_mot[0] = ch[i]; sep_mot[1] = '\0';

            if (!mot_vide(mot_c)) {
                init_mot(&mot, cpt);
                strcpy(mot, mot_c);
                ajouter_fin_rapide(liste_ul, &P, mot, get_code_symb(mot_c, qp), ligne_c);
            }

            if (ch[i] == '\n') ++ligne_c;
            int code_symb = get_code_symb(sep_mot, qc);

            if (code_symb != EPSILON) {
                if (!mot_vide(sep_mot)) ajouter_fin_rapide(liste_ul, &P, sep_mot, code_symb, ligne_c);
            }
            if (!mot_vide(mot_c)) {
                mot_c[0] = '\0';
                cpt = 0;
            }
        }
        else {  /// Pour ne pas concatener les delimiteurs
            strncat(mot_c, &ch[i], 1);
            ++cpt;
        }
    }

    if (error || ((cpt > MAX_SIZE_WORD) && printf("MAX_SIZE_WORD = %d\n", MAX_SIZE_WORD)) || ((qc != 0) && (qc != 1) && (qc != 2) && (qc != 4) && (qc != 5))) {
        if (!mot_vide(mot_c)) ajouter_fin_rapide(liste_ul, &P, mot_c, ERREUR, ligne_c);
        if (debug_analyse_sql()) afficher_liste(*liste_ul);
        return 0;
    }
    else {
        if (!mot_vide(mot_c))
            ajouter_fin_rapide(liste_ul, &P, mot_c, get_code_symb(mot_c, qp), ligne_c);

        if (!mot_vide(mot_c)) {
            init_mot(&mot_c, 1);
        }
        mot_c[0]='#'; mot_c[1]='\0';
        ajouter_fin_rapide(liste_ul, &P, mot_c, FIN_SUITE_UL, ligne_c);
        if (debug_analyse_sql()) afficher_liste(*liste_ul);
        return 1;
    }
}

int automate_sql_file(char* path, ListeUL* liste_ul) {
    char* source = NULL;
    FILE* fp = fopen(path, "r");
    if (fp != NULL) {
        /* Go to the end of the file. */
        if (fseek(fp, 0L, SEEK_END) == 0) {
            /* Get the size of the file. */
            long bufsize = ftell(fp);
            if (bufsize == -1) {
                fclose(fp);
                return 2;
            }

            /* Allocate our buffer to that size. */
            source = malloc(sizeof(char) * (bufsize + 1));

            /* Go back to the start of the file. */
            if (fseek(fp, 0L, SEEK_SET) != 0) {
                fclose(fp);
                return 2;
            }

            /* Read the entire file into memory. */
            size_t newLen = fread(source, sizeof(char), bufsize, fp);
            if (ferror(fp) != 0) {
                fclose(fp);
                return 2;
            } else {
                source[newLen++] = '\0'; /* Just to be safe. */
                return automate_sql(source, liste_ul);
            }
            free(source);
        }
        fclose(fp);
    }
    else
        return 2;
}
