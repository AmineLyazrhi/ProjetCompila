#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax.c"

int main(int argc, const char* argv[]) {
  /* ouverture de fichier à programme */
  openfile("programme.txt");
  /* analyse lexicale */
  if (analyse_lexicale())return 1;
  /* analyse syntaxique */
  analyse_syntaxique();
  return 0;
}
