#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "donnees.h"

//la liste des fonctions

int analyse_lexical();
char Lire_Car();
TOKENS lire_mot();
TOKENS lire_nombre();
TOKENS lire_Symbole();
TOKENS analyse();
void openfile(char* filename);
void resultfile(char* filename, char* chaine);
void ajouter();
void AfficherListe();
char* conversion(TOKENS token);

/* fonction d'analayse lexicale */
int analyse_lexicale() {
    /* allocation mémoire de tempo de type la structure Structure_TOKEN */
    tempo = (Structure_TOKEN*)malloc(sizeof(Structure_TOKEN));
    /* lecture du premier caractère */
    Lire_Car();
    /* vérification de mot par mot jusqu'à la fin du fichier */
    while (Car_Cour != EOF) {
        analyse();
        Lire_Car();
    }
    tempo->token = EOF_TOKEN;
    tempo->nom = "EOF";
    /* ajouter le EOF_TOKEN pour marquer la fin de vérification */
    ajouter();
    /* afficher la liste chainée et ecriture des résultats des tokens sur un fichier txt */
    AfficherListe();
    /* initialisation du pointeur tempo sur la tete de la lise chainée */
    tempo = head;
    int ligne = 0;
    printf("=====================================================================\n");
    printf("     ===================FIN ANALYSE LEXICALE===================\n");
    /* chercher l'EREEUR sur a liste en parcourant la liste chainée créée */
    while ((strcmp(conversion(tempo->token), "ERREUR") != 0) && (tempo->next != NULL))
    {
        tempo = tempo->next;
        ligne = ligne + 1;
    }
    //verifier les resultats
    if (tempo != NULL && tempo->token != EOF_TOKEN) {
        printf("     lexique incorrecte!!!!!!!erreur sur %s de la ligne %d\n", conversion(tempo->token), ligne);
        printf("   ===============================================\n");
        return 1;
    }
    else {
        printf("    ===============LEXIQUE CORRECTE========================        \n");
        printf("====================================================================\n");
        return 0;
    }
}
/* lecture des caractères à partir du fichier */
char Lire_Car() {
    Car_Cour = fgetc(fichier);
    return Car_Cour;
}
/* verificaion d'un mot */
TOKENS lire_mot() {

    /* constitution de mot à vérifier */
    while (((Car_Cour >= 'a' && Car_Cour <= 'z') || (Car_Cour >= 'A' && Car_Cour <= 'Z') || (Car_Cour >= '0' && Car_Cour <= '9') || (Car_Cour == '_')) && (Car_Cour != ' ')) {
        strncat(mot, &Car_Cour, 1);
        Lire_Car();
    }
    int j = 0, cmp = 0;
    // Verify this word match a keyword in the list of keywords
    while ((cmp = strcmp(mot, keywords_list[j++])) != 0 && j < NBRKEYWORDS);

    if (cmp == 0) { // is keyword
        if (j >= NBRKEYWORDS - 4) {
            tempo->token = TYPE;
            tempo->nom = mot;
            //printf("%s ===> TYPE_TOKEN\n", mot);
        }
        else {
            tempo->token = (TOKENS)(j - 1);
            tempo->nom = mot;
            //printf("%s ===> %s_TOKEN\n", mot, keywords_list[j - 1]);
        }
    }

    else { // si n'est pas un keyword, donc ID
        tempo->token = ID;
        tempo->nom = mot;
        //printf("%s ===> ID_TOKEN\n", mot);
    }
    ajouter();
    return tempo->token;
}
/* test d'un nombre */
TOKENS lire_nombre() {
    int type = 0;
    while (Car_Cour >= '0' && Car_Cour <= '9' && Car_Cour != ' ') {
        strncat(mot, &Car_Cour, 1);
        if (Car_Cour != '0' && Car_Cour != '1') {
            type = 1;
        }
        Lire_Car();
    }
    if (Car_Cour == '.') {
        type = 2;
        strncat(mot, &Car_Cour, 1);
        Lire_Car();
        if (!(Car_Cour >= '0' && Car_Cour <= '9' && Car_Cour != ' ')) {
            tempo->token = ERREUR;
        }
        else {
            while (Car_Cour >= '0' && Car_Cour <= '9' && Car_Cour != ' ') {
                strncat(mot, &Car_Cour, 1);
                if (Car_Cour != '0' || Car_Cour != '1') {
                    type = 2;
                }
                Lire_Car();
            }
        }
    }
    if (type == 2) {
        //printf("%s ===> REEL_TOKEN\n", mot);
        tempo->token = REEL;
        tempo->nom = mot;
    }
    else if (strlen(mot) > 11) {
        tempo->token = ERREUR;
    }
    else {
        if (type == 1)
        {
            //printf("%s ===> ENTIER_TOKEN\n", mot);
            tempo->val = atoi(mot);
            tempo->token = ENTIER;
        }
        else {
            //printf("%s ===> BIT_TOKEN\n", mot);
            tempo->token = BIT;
        }
        tempo->nom = mot;
    }
    ajouter();
    return tempo->token;
}
/* test d'un symbole */
TOKENS lire_Symbole() {

    int i = 0, cmp = 0;
    char op[3];
    memset(op, '\0', sizeof(op));

    op[0] = Car_Cour;
    if (Car_Cour == '<')
    {
        Lire_Car();
        char c = Car_Cour;
        if (c == '-' || c == '=' || c == '>') {
            op[1] = c;
        }
    }
    else if (Car_Cour == '>') {
        Lire_Car();
        char c = Car_Cour;
        if (c == '=') {
            op[1] = c;
        }
    }
    //printf("%s\n", op);
    while ((cmp = strcmp(op, specialTokens_symb[i++]) != 0) && i < NBRSPECIALTOKENS);

    if (cmp == 0)
        tempo->token = (TOKENS)(NBRKEYWORDS + i - 1);
    else return ERREUR;
    //printf("%s ===> %s_TOKEN\n", op, specialTokens_list[i - 1]);
    tempo->nom = (char*)(specialTokens_symb[i - 1]);
    ajouter();
    return tempo->token;
}
/* test du type du mot à analyser*/
TOKENS analyse() {
    mot = (char*)malloc(sizeof(char) * 30);
    TOKENS b;
    if ((Car_Cour >= 'A' && Car_Cour <= 'Z') || (Car_Cour >= 'a' && Car_Cour <= 'z'))
    {
        b = lire_mot();
    }

    if (Car_Cour >= '0' && Car_Cour <= '9')
    {
        b = lire_nombre();
    }

    if ((Car_Cour == '=') || (Car_Cour == '{') || (Car_Cour == '}') || (Car_Cour == '(') || (Car_Cour == ')') || (Car_Cour == '<') || (Car_Cour == '>') || (Car_Cour == ':') || (Car_Cour == ',') || (Car_Cour == '/') || (Car_Cour == '*') || (Car_Cour == '-') || (Car_Cour == '+') || (Car_Cour == ';') || (Car_Cour == '.') || (Car_Cour == '&') || Car_Cour == '|' || Car_Cour == '!')
    {
        b = lire_Symbole();
    }

    if (Car_Cour == '"')
    {
        Lire_Car();
        while ((Car_Cour != EOF) && (Car_Cour != '"') && (strlen(mot) < 30)) {
            strncat(mot, &Car_Cour, 1);
            Lire_Car();
        }
        if (Car_Cour == '"')
        {
            strncat(mot, &Car_Cour, 1);
            //printf("%s====>CHAINE_TOKEN\n", mot);
            tempo->token = CHAINE;
            strcmp(tempo->nom, mot);
            ajouter();
        }
        else b = ERREUR;
    }

    if (Car_Cour == '#')
    {
        Lire_Car();
        while (Car_Cour != '#') {
            strncat(mot, &Car_Cour, 1);
            Lire_Car();
        }
        if (Car_Cour == '#')
        {
            strncat(mot, &Car_Cour, 1);
            //printf("=============>COMMENTAIRE_TOKEN\n");
            tempo->token = COMMENT;
            tempo->nom = mot;
            ajouter();
        }
        else b = ERREUR;
    }
    if (Car_Cour == ' ') {
        //printf("=============>ESPACE_TOKEN\n");
        tempo->token = ESPACE;
        tempo->nom = "espace";
    }
    else if (Car_Cour == '\n') {
        //printf("=============>SAUT_TOKEN\n");
        tempo->token = SAUT;
        tempo->nom = "retour ligne";
        ajouter();
    }
    else b = ERREUR;

    return b;
}
/* ouverture du fichier */
void openfile(char* filename) {
    fichier = fopen(filename, "r");
    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier en lecture\n");
        exit(1);
    }
}
/* ajout des resultats de test dans la liste chainée */
void ajouter() {

    Structure_TOKEN* last = head;
    Structure_TOKEN* nouveau_symb = (Structure_TOKEN*)malloc(sizeof(Structure_TOKEN));
    nouveau_symb->token = tempo->token;
    nouveau_symb->nom = tempo->nom;
    nouveau_symb->val = tempo->val;
    nouveau_symb->next = NULL;
    if (head == NULL)
        head = nouveau_symb;
    else {
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = nouveau_symb;
    }
    return;
}
/* affichage de la liste chainée et écriture sur le fichier résultat */
void AfficherListe() {
    printf("======================================================================\n");
    printf("===========================LISTE DES TOKENS===========================\n");
    printf("======================================================================\n");
    /* Tant que l'on n'est pas au bout de la liste */
    Structure_TOKEN* tok;
    tok = (Structure_TOKEN*)malloc(sizeof(Structure_TOKEN));
    tok = head;
    int i = 0;
    //parcourir liste chainée
    while (tok != NULL)
    {
        /* On affiche */
        printf("TOKEN %d :  %s ==> %s\n", i,tok->nom, conversion(tok->token));
        /* On avance d'une case */
        tok = tok->next;
        i = i + 1;
    }
}
/* conversion des valeurs du token créé en chaine de caractère */
char* conversion(TOKENS token) {
    int corresp = (int)token;
    char* wordc = mot = (char*)malloc(sizeof(char));;
    //printf("%d\n", corresp);
    if (corresp < NBRKEYWORDS) {
        strcpy(wordc, keywords_list[corresp]);
    }
    else if (corresp < NBRSPECIALTOKENS + NBRKEYWORDS) {
        strcpy(wordc, specialTokens_list[corresp - NBRKEYWORDS]);
    }
    else {
        switch (token) {
        case COMMENT:return "COMMENT";break;
        case ESPACE:return"ESPACE";break;
        case SAUT:return "SAUT";break;
        case ERREUR:return "ERREUR";break;
        case ID:return "ID";break;
        case NUM:return"NUM";break;
        case EOF_TOKEN:return "EOF_TOKEN";break;
        default:return "";
        }
    }
    return wordc;
}
