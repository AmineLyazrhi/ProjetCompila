ALGO-COMPILER

## **Remerciements**

	Avant d’aborder la description des parties importantes du projet,nous aimerons tout d’abord exprimer notre gratitude intense à toute personne qui a
contribué énormément dans l’élaboration et la réalisation de ce travail. on commence ainsi par offrir nos remerciements à l’intégralité des personnes travaillant au sein de l’école Nationale supérieure d’informatique et d’analyse des systèmes. Toute nos gratitude et profonde reconnaissance s’adressent à notre encadrant de projet TABII Younes, et à toute personne qui a contribué dans la réalisation de ce projet. on vous remercie énormément pour l’encadrement de qualité garanti pour bien mener et assurer la réalisation de ce travail dans les meilleures des conditions.

## **Description**

	Ce rapport présente le projet que nous avons réalisé en groupe pour le module de compilation de la deuxième année à l’ENSIAS.
Le but de ce projet est de faire la conception d’un langage de programmation, qu’on a nommé ALGO, et la réalisation d’un simple
compilateur (analyse lexicale et syntaxique) pour ce langage. Dans ce rapport, nous allons présenter dans un premier temps le langage
de programmation et les cas d’utilisation potentiels. Ensuite, nous allons nous intéresser à la présentation de notre grammaire LL (1).
et après on va présenter le fonctionnement de l’analyseur lexicale et l’analyseur syntaxique.


execution:
==========

le programme peut être compilé et executé dans sa totalité par la commande:
----------------------------------------------------------------------------

     gcc main.c -o main
puis
----
    ./main
