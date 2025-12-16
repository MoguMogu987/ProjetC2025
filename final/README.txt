# Projet de BADGER Maëlys, CAUET Garris et RICHARD Thomas.

### Fonctions de base implémentées sont :
BigBinary creerBigBinaryDepuisChaine(const char *chaine)

* void divisePar2(BigBinary *nb) : implémtée mais pas utilisé.

* BigBinary Addition(BigBinary a, BigBinary b) : utilise 2 sous fonction BB_Add et BB_Add2
BB_Add fais l'addition de 2 nombre de même signe
BB_Add2 fais l'addition de 2 nombre de signe différend

* BigBinary Soustraction(BigBinary a, BigBinary b) : utilise Addition car A - B = A + (-B)

* bool Egal(BigBinary a,BigBinary b)

* bool Inferieur(BigBinary a,BigBinary b)

* BigBinary minBB(BigBinary a, BigBinary b)

* BigBinary maxBB(BigBinary a,BigBinary b)

* BigBinary PGCD(BigBinary a,BigBinary b)

* BigBinary Modulo(BigBinary a,BigBinary b)

* BigBinary BigBinary_Egypt(BigBinary A, BigBinary B)

### Fonction avancé implémenté :

* BigBinary BBexpMod(BigBinary M,int E,BigBinary N) : exponentiation modulaire

* RSA_Encrypt et RSA_Decrypt sont identique, elle font simplement une exponentaition modulaire
BigBinary RSA_Encrypt(BigBinary M, int E, BigBinary N)
BigBinary RSA_Decrypt(BigBinary C, int D, BigBinary N)

### Autre fonction utilisé :

* BigBinary setBB(int a) : créer un BB a partir d'un int

* void normalizeBB(BigBinary *bb) : réduit la taille des BB pour éviter les 0 inutiles

* bool Inferieur_Taille(BigBinary A, BigBinary B) : contrairement à inférieur, ne compare pas la valeur mais la taille

* int puissance(int base,int e) : calcul la puissance d'un int

* int BBtoInt(BigBinary a) : transforme un BB en int

* BigBinary Shift(BigBinary a) : contrairement à divisePar2, Shift décale vers la gauche et donc multiplie par 2

### Les fonction de test
Les fonction de test sont à la fin à partir de la ligne 440.