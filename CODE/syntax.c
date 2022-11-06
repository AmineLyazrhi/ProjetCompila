#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexical.c"
int analyse_syntaxique();
void Sym_Suiv();
void Test_Symbole(TOKENS COD_ERR);
void PROGRAM();
void CHOIX();
void FONCTIONS();
void INSTS();
void INST();
void AFFEC();
void EXPR();
void TERM();
void FACT();
void si();
void COND();
void tantque();
void lire();
void ecrire();
void fonction();
void pour();
void option();
void VARS();

//fonction principale de analyse syntaxique
int analyse_syntaxique() {
    /* initialisation*/
    word = 0;syntax = 0;ligne = 1;
    /* initialisation du pointeur tempo sur la tete de la liste */
    tempo = head;
    printf("\n");
    printf("========================DEBUT ANALYSE SYNTAXIQUE============\n");
    printf("===============================================================\n");
    /* analyse du programme proposé */
    PROGRAM();
    /* affichage du resultat d'analyse */
    printf("==========================================================\n");
    printf("      ===============FIN ANALYSE SYNTAXIQUE===========\n");

    if (syntax == 0)
    {
        printf(" ********   SYNTAXE CORRECTE   ******\n");
        printf("  ====================================\n");
        return 0;
    }
    else {
        printf(" ********  !!!!SYNTAXE INCORRECTE!! %d ERREUR(S)  ******\n", syntax);

        printf("    =====================================\n");
        printf(" !!!!!!!!!!!!!!!!!VERIFIER VOTRE SYNTAXE!!!!!!!!!!!!!!!!!\n");
        return 1;
    }

}
/* analyse de la liste des tokens */
void PROGRAM() {
    Test_Symbole(DEBUT);
    while (tempo->token == SAUT || tempo->token == COMMENT) {
        ligne += 1;
        Sym_Suiv();
    }
    VARS();
    FONCTIONS();
    Test_Symbole(FIN);
}
/* passage d'un symbole à l'autre */
void Sym_Suiv() {
    tempo = tempo->next;
}
/* arreter le programme*/
void Erreur(TOKENS COD_ERR, int word) {
    syntax += 1;
    printf("%d-ligne %d ERREUR-->%s_ERREUR\n", word, ligne - 1, conversion(COD_ERR));
    exit(1);
}
/* test de symobole passer ne paramètre avec les elts de la liste chainée*/
void Test_Symbole(TOKENS tok) {

    if (tempo->token == tok)
    {
        printf("%d- %s ok \n", word, conversion(tempo->token));
        word = word + 1;
        Sym_Suiv();
    }
    else {
        Erreur(tok, word);
        exit(1);
    }
}
/* verification des VAR */
void VARS() {
    while (tempo->token == TYPE) {
        char* word = tempo->nom;
        Test_Symbole(TYPE);
        Test_Symbole(ID);
        //printf("%d\n", (int)tempo->token);
        if (tempo->token == AFF) {
            Test_Symbole(AFF);
            if (!strcmp(word, "entier"))Test_Symbole(ENTIER);
            else if (!strcmp(word, "reel"))Test_Symbole(REEL);
            else if (!strcmp(word, "bit"))Test_Symbole(BIT);
            else if (!strcmp(word, "caract"))Test_Symbole(ID);
            else Test_Symbole(CHAINE);
            while (tempo->token == VIR) {
                Test_Symbole(VIR);
                while (tempo->token == SAUT || tempo->token == COMMENT)
                {
                    ligne += 1;
                    Sym_Suiv();
                }
                Test_Symbole(ID);
                if (tempo->token == AFF) {
                    Test_Symbole(AFF);
                    if (!strcmp(word, "entier"))Test_Symbole(ENTIER);
                    else if (!strcmp(word, "reel"))Test_Symbole(REEL);
                    else if (!strcmp(word, "bit"))Test_Symbole(BIT);
                    else if (!strcmp(word, "caract"))Test_Symbole(CARACT);
                    else Test_Symbole(CHAINE);

                }
            }
        }
        else {
            while (tempo->token == VIR) {
                Sym_Suiv();
                while (tempo->token == SAUT || tempo->token == COMMENT)
                {
                    ligne += 1;
                    Sym_Suiv();
                }
                Test_Symbole(ID);
                if (tempo->token == AFF) {
                    if (!strcmp(word, "entier"))Test_Symbole(ENTIER);
                    else if (!strcmp(word, "reel"))Test_Symbole(REEL);
                    else if (!strcmp(word, "bit"))Test_Symbole(BIT);
                    else if (!strcmp(word, "caract"))Test_Symbole(CARACT);
                    else Test_Symbole(CHAINE);
                }
            }
        }
        Test_Symbole(PV);
        while (tempo->token == SAUT || tempo->token == COMMENT)
        {
            ligne += 1;
            Sym_Suiv();
        }
    }
}
/* analyse du corps du programme */
void FONCTIONS() {
    while (tempo->token == DEBUT) {
        switch (tempo->token) {
        case DEBUT: Test_Symbole(DEBUT);
            if (tempo->token == TYPE)
            {
                Sym_Suiv();
            }
            else if (tempo->token == ID) { ; }
            else if (tempo->token == MAIN) { ; }
            else {
                Erreur(TYPE, word);
            }
            word = word + 1;
            switch (tempo->token) {
            case FONCTION: Test_Symbole(FONCTION);
                Test_Symbole(ID);
                Test_Symbole(PO);
                if (tempo->token == TYPE) {
                    Test_Symbole(TYPE);
                    Test_Symbole(ID);
                    while (tempo->token == VIR) {
                        Sym_Suiv();
                        Test_Symbole(TYPE);
                        Test_Symbole(ID);
                    }
                }
                Test_Symbole(PF);
                Test_Symbole(PP);
                while (tempo->token == SAUT || tempo->token == COMMENT)
                {
                    ligne += 1;
                    Sym_Suiv();
                }
                INSTS();
                Test_Symbole(FIN);
                Test_Symbole(FONCTION);
                Test_Symbole(ID);break;
            case MAIN: Test_Symbole(MAIN);
                Test_Symbole(PO);
                Test_Symbole(PF);
                Test_Symbole(PP);
                while (tempo->token == SAUT || tempo->token == COMMENT)
                {
                    ligne += 1;
                    Sym_Suiv();
                }
                INSTS();
                Test_Symbole(FIN);
                Test_Symbole(MAIN);break;
            default: Erreur(FONCTION, word);break;
            }break;
        default: Erreur(DEBUT, word);break;
        }
        while (tempo->token == SAUT)
        {
            ligne += 1;
            Sym_Suiv();
        }
    }
}
/* verification des instruction */
void INSTS() {
    INST();
    while (tempo->token != FIN)
        INST();
}
/* verification des AFF,OPTION ,SI ,POUR ,TANTQUE.....*/
void INST() {
    while (tempo->token == SAUT)
    {
        ligne += 1;
        Sym_Suiv();
    }
    switch (tempo->token) {
    case FONCTION:Test_Symbole(FONCTION);fonction();Test_Symbole(PV);
        while (tempo->token == SAUT)
        {
            ligne += 1;
            Sym_Suiv();
        }break;
    case OPTION: option();break;
    case POUR: pour(); break;
    case ID: Test_Symbole(ID);
        if (tempo->token == PV) Test_Symbole(PV);
        else if (tempo->token == PO) {
            fonction();Test_Symbole(PV);
        }
        else  AFFEC();
        while (tempo->token == SAUT)
        {
            ligne += 1;
            Sym_Suiv();
        }break;
    case SI: si(); break;
    case TANTQUE: tantque();break;
    case ECRIRE: ecrire();Test_Symbole(PV);

        while (tempo->token == SAUT)
        {
            ligne += 1;
            Sym_Suiv();
        }break;
    case LIRE: lire();Test_Symbole(PV); break;
    case DONNER: Test_Symbole(DONNER);
        EXPR();
        Test_Symbole(PV);
        while (tempo->token == SAUT)
        {
            ligne += 1;
            Sym_Suiv();
        }break;
    case FIN: break;
    case COMMENT: Test_Symbole(COMMENT);break;
    default: printf("%d-ligne %d ERREUR-->OPERATION_ERREUR\n", word, ligne);
        exit(1);
        break;
    }
}
/* verification des AFFEC (affectation) */
void AFFEC() {
    Test_Symbole(AFF);
    EXPR();
    Test_Symbole(PV);
}
/* verification des expresions + ou - */
void EXPR() {
    printf("%s\n", conversion(tempo->token));
    TERM();
    while (tempo->token == PLUS || tempo->token == MOINS || tempo->token == OR || tempo->token == ET ) {
        Test_Symbole(tempo->token);
        TERM();
    }
}
/* verification des termes */
void TERM() {

    FACT();
    /* verification des expresions multiplication ou division */
    while (tempo->token == MULT || tempo->token == DIV) {
        Test_Symbole(tempo->token);
        FACT();
    }
}
/* verification des facteurs */
void FACT() {

    switch (tempo->token) {
    case FONCTION:Test_Symbole(FONCTION);FACT();break;
    case ID: Test_Symbole(ID);
        if (tempo->token == PO)
            fonction(); break;
    case PO: Test_Symbole(PO);EXPR(); Test_Symbole(PF); break;
    case ENTIER: Test_Symbole(ENTIER); break;
    case BIT: Test_Symbole(BIT); break;
    case CARACT: Test_Symbole(CARACT); break;
    case REEL: Test_Symbole(REEL); break;
    case NON :Test_Symbole(NON);FACT();break;
    default:
        printf("%d-ligne %d ERREUR-->OPERATION_ERREUR\n", word, ligne);
        exit(1);
        break;
    }
}
/* verification fonction */
void fonction() {
    Test_Symbole(PO);
    if (tempo->token != PF)
        EXPR();
    while (tempo->token == VIR) {
        Sym_Suiv();
        EXPR();
    }
    Test_Symbole(PF);
}
/* verification POUR */
void pour() {
    Test_Symbole(POUR);
    Test_Symbole(ID);
    Test_Symbole(DANS);
    Test_Symbole(INF);
    FACT();
    Test_Symbole(PV);
    FACT();
    Test_Symbole(PV);
    if (tempo->token == ID)Test_Symbole(ID);
    else Test_Symbole(ENTIER);
    Test_Symbole(SUP);
    Test_Symbole(PP);
    INSTS();
    Test_Symbole(FIN);
    Test_Symbole(POUR);
}
/* verification OPTION */
void option() {
    Test_Symbole(OPTION);
    Test_Symbole(PO);
    Test_Symbole(ID);
    Test_Symbole(PF);
    while (tempo->token == SAUT)
    {
        ligne += 1;
        Sym_Suiv();
    }
    if (!(tempo->token == CAS))
    {
        Erreur(tempo->token, word);
        exit(1);
    }
    while (tempo->token == CAS) {
        Test_Symbole(CAS);
        FACT();
        Test_Symbole(PP);
        while (tempo->token != STOP)
            INST();
        Test_Symbole(STOP);
        Test_Symbole(PV);
        while (tempo->token == SAUT) {
            ligne += 1;
            Sym_Suiv();
        }
    }
    if (tempo->token == DEFAULT) {
        Test_Symbole(DEFAULT);
        Test_Symbole(PP);
        INST();
        while (tempo->token == SAUT)
        {
            ligne += 1;
            Sym_Suiv();
        }
    }
    Test_Symbole(FIN);
    Test_Symbole(OPTION);
}
/* verification de SI */
void si() {

    Test_Symbole(SI);
    if (tempo->token == PO)
    {
        Test_Symbole(PO);
        COND();
        Test_Symbole(PF);
    }
    else COND();
    while (tempo->token == SAUT)
    {
        ligne += 1;
        Sym_Suiv();
    }
    Test_Symbole(ALORS);
    Test_Symbole(PP);
    while (tempo->token != SIAUTRE && tempo->token != SINON)
    {
        printf("hna\n");
        INST();
    }
    while (tempo->token == SIAUTRE) { CHOIX(); }
    if (tempo->token == SINON) {
        CHOIX();
        Test_Symbole(FIN);
        Test_Symbole(SI);
    }
    else if (tempo->token == FIN)
    {
        Test_Symbole(FIN);
        Test_Symbole(SI);
    }
    else {
        printf("%d-ligne %d ERREUR-->CONDITION_ERREUR\n", word, ligne - 1);exit(1);
    }
}
void CHOIX() {
    switch (tempo->token) {
    case SIAUTRE: Test_Symbole(SIAUTRE);
        COND();
        while (tempo->token == SAUT)
        {
            ligne += 1;
            Sym_Suiv();
        }
        Test_Symbole(ALORS);
        Test_Symbole(PP);
        while (tempo->token != SIAUTRE && tempo->token != SINON)
        {
            INST();
        }
    case SINON: Test_Symbole(SINON);
        while (tempo->token == SAUT)
        {
            ligne += 1;
            Sym_Suiv();
        }
        Test_Symbole(PP);
        INSTS();break;
    default: break;
    }

}
/* verification de la condition apres SI */
void COND() {
    if (tempo->token == PO) {
        Test_Symbole(PO);
        COND();
        Test_Symbole(PF);
    }
    else
    {
        EXPR();
        printf("%s\n", conversion(tempo->token));
        switch (tempo->token) {
        case EG:Test_Symbole(tempo->token);EXPR();break;
        case DIFF:Test_Symbole(tempo->token);EXPR();break;
        case INF:Test_Symbole(tempo->token);EXPR();break;
        case SUP:Test_Symbole(tempo->token);EXPR();break;
        case INFEG:Test_Symbole(tempo->token);EXPR();break;
        case SUPEG:Test_Symbole(tempo->token);EXPR();break;
        case ET:Test_Symbole(tempo->token);EXPR();break;
        case OR:Test_Symbole(tempo->token);EXPR();break;
        default:printf("%d-ligne %d ERREUR-->CONDITION_ERREUR\n", word, ligne - 1);exit(1);break;
        }
    }
}
/* verification de ce qui est après le  TANTQUE*/
void tantque() {
    Test_Symbole(TANTQUE);
    if (tempo->token == PO) {
        COND();
        Test_Symbole(PF);
    }
    else COND();
    while (tempo->token == SAUT)
    {
        ligne += 1;
        Sym_Suiv();
    }
    Test_Symbole(FAIRE);
    Test_Symbole(PP);
    INSTS();
    Test_Symbole(FIN);
    Test_Symbole(TANTQUE);
}
/* verification de ce qui est après l'ecriture et lecture */
void ecrire() {
    Test_Symbole(ECRIRE);
    Test_Symbole(PO);
    while (tempo->token != PF)
    {
        printf("%s\n", conversion(tempo->token));
        switch (tempo->token) {
        case CHAINE: Test_Symbole(CHAINE);break;
        case ID: EXPR();break;
        case ENTIER: EXPR();break;
        case REEL: EXPR();break;
        case BIT: EXPR();break;
        case VIR: Test_Symbole(VIR);break;
        default:printf("%d-ligne %d ERREUR-->ECRIRE_ERREUR\n", word, ligne - 1);exit(1);break;
        }
    }
    Test_Symbole(PF);
}
void lire() {

    Test_Symbole(LIRE);
    Test_Symbole(PO);
    Test_Symbole(ID);
    while (tempo->token == VIR) {
        Sym_Suiv();
        Test_Symbole(ID);
    }
    Test_Symbole(PF);
}

