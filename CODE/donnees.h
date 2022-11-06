#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    //MOTS CLES
    DEBUT, FIN, FONCTION, MAIN, FAIRE, SI, SINON, SIAUTRE, ALORS, OPTION,
    CAS, STOP, DEFAULT, TANTQUE, POUR,DANS, ECRIRE, LIRE, DONNER,TYPE,
    BIT, REEL, ENTIER, CARACT, CHAINE,
    //MOTS SPECIAUX
    PV, PT, PLUS, MOINS, MULT, DIV, VIR, EG, AFF, INF, INFEG, SUP, SUPEG,
    DIFF, PO, PF, PP, ET, OR,NON,
    //AUTRES
    COMMENT, ESPACE, SAUT, ERREUR, ID, NUM, EOF_TOKEN,
} TOKENS;
const char* keywords_list[] =
{ "debut", "fin", "fonction", "main", "faire", "si" , "sinon", "si_autre", "alors", "option",
    "cas", "stop", "default", "tantque", "pour","dans", "ecrire", "lire", "donner","type",
    "bit", "reel", "entier", "caract", "chaine" };

const char* specialTokens_list[] =
{ "PV",  "PT",  "PLUS", "MOINS", "MULT", "DIV", "VIR",  "EG", "AFF", "INF",  "INFEG", "SUP", "SUPEG", "DIFF", "PO",  "PF","PP" ,"ET","OR","NON" };
const char* specialTokens_symb[] =
{ ";",    ".",    "+",    "-",     "*",    "/",   "," ,   "=",  "<-",  "<",    "<=",    ">",    ">=",  "<>",     "(",   ")", ":" ,"&","|","!" };

int keywords_list_size = sizeof(keywords_list) / sizeof(keywords_list[0]);
int specialTokens_list_size = sizeof(specialTokens_list) / sizeof(specialTokens_list[0]);

#define NBRKEYWORDS         keywords_list_size
#define NBRSPECIALTOKENS    specialTokens_list_size

typedef struct Structure_TOKEN
{
    TOKENS token;
    char* nom;
    int val;
    struct Structure_TOKEN* next;
} Structure_TOKEN;

Structure_TOKEN* head;
Structure_TOKEN* tempo;
char* mot;
char Car_Cour;
FILE* fichier;
int word;
int syntax;
int ligne;



/*grammaire
DEBUT<-debut VAR FONCTIONS MAIN fin
VAR<- TYPE ID S' | epsilon
TYPE<- entier|caract|chaine|bit|reel
ID <- LETTRE ID'
ID'<-LETTRE|CHIFFRE|epsilon
LETTRE<-a...z|A...Z
NUM<-CHIFFRE|CHIFFRE NUM
CHIFFRE<- 0...9
S'<- ;VAR | ,ID S' | <-NUM S"|<-CHAINE S"|epsilon
S" <- ;VAR| ,ID S'
FONCTIONS<-FONCTION FONCTIONS |epsilon
FONCTION <- debut TYPE ID (ARG):INSTS fin ID
MAIN <-debut main():INSTS fin main
ARG<- TYPE ID,ARG | ARG |epsilon
INSTS<-INST INSTS
INST<-AFFEC | SI | TANTQUE | ECRIRE | LIRE | epsilon
AFFEC<- ID <- EXPR
SI<-si COND alors :INSTS CHOIX fin si
CHOIX<-AUTRE CHOIX |sinon : INSTS | epsilon
AUTRES<-si_autre COND alors : INSTS
TANTQUE<-tantque COND faire : INSTS
ECRIRE<-ecrire(EXPRS)
EXPRS<-EXPR EXPRS
LIRE<-lire(IDS)
IDS<- ID ,IDS|ID
COND<-EXPR RELOP EXPR
RELOP<-= | <> | < | > | <= | >=
EXPR<-TERM { ADDOP TERM }
ADDOP<-+ | -
TERM<-FACT { MULOP FACT }
MULOP<-* | /
FACT  <-ID | NUM | ( EXPR )

*/