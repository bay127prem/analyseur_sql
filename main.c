#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyseur_sql.h"

int debug = 0;

int debug_analyse_sql() {
    return debug;
}

int main(int argc, char** argv) {
    if ((argc < 2) || (argc > 3)) return 2;

    if ((argc == 3) && (strcmp(argv[2], "-d") == 0)) {
        debug = 1;
    }

    ListeUL liste_ul;
    init_liste(&liste_ul);

    int analyse_lex = automate_sql_file(argv[1], &liste_ul);

    if (analyse_lex == 1) {
        printf("SQL request is lexically valid\n");
        if (syntax_sql(liste_ul)) {
            printf("SQL request is syntaxically valid\n");
        }
        else {
            printf("Syntaxical Error SQL\n");
        }
    }
    else if (analyse_lex == 0) {
        printf("Lexical Error SQL\n");
    }
    else {
        fputs("Error reading file", stderr);
        return 2;
    }

    return 0;
}
