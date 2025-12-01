#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BASE 2

typedef struct{
    int *Tdigits;
    int Taille;
    int Signe;
} BigBinary;

BigBinary createBigBinary(int size){
    BigBinary bb;
    bb.Tdigits=malloc(sizeof(int)*size);
    bb.Taille=size;
    bb.Signe=0;
    return bb;
}

BigBinary initBigBinary (int taille,int signe){
    BigBinary nb;
    nb.Taille=taille;
    nb.Signe=signe;
    nb.Tdigits=malloc(sizeof(int)*taille);
    for (int i=0;i<taille;i++){
        nb.Tdigits[i]=0;
    }
    return nb;
}

void afficheBigBinary(BigBinary nb){
    if (nb.Signe==-1) printf("-") ;
    if (nb.Signe==0||nb.Taille==0) {
        printf("0\n");
        return;
    }
    for (int i=0;i < nb.Taille;i++){
        printf("%d",nb.Tdigits[i]);
    }
    printf("\n");
}

void libereBigBinary(BigBinary *nb){
    free(nb->Tdigits);
    nb->Tdigits=NULL;
    nb->Taille=0;
    nb->Signe=0;
}

BigBinary setBB(int a){
    if (a==0) return initBigBinary(1,0);
    int temp=abs(a);
    int t=0;
    while (temp!=0) {
        t+=1;
        temp/=2;
    }

    BigBinary A = createBigBinary(t);
    if (a == 0) {
        return A;
    }
    if (a < 0) {
        A.Signe = -1;
    } else {
        A.Signe = 1;
    }

    temp = abs(a);
    while (t >= 0) {
        A.Tdigits[--t] = temp%2;
        temp /= 2;
    }
    return A;
}

void normalizeBB(BigBinary *bb){
    if (bb->Taille==0){
        bb->Signe=0;
        return;
    }
    int index=-1;
    for (int i=0;i<bb->Taille;i++){
        if (bb->Tdigits[i]==1){
            index=i;
            break;
        }
    }
    if (index == -1) {
        if (bb->Tdigits != NULL) free(bb->Tdigits);
        bb->Tdigits = (int *)malloc(sizeof(int));
        bb->Tdigits[0] = 0;
        bb->Taille = 1;
        bb->Signe = 0;
    } else if (index > 0) {
        int new_taille = bb->Taille - index;
        int *new_digits = (int *)malloc(sizeof(int) * new_taille);
        if (new_digits == NULL) {
             fprintf(stderr, "Erreur d'allocation mémoire.\n");
             exit(EXIT_FAILURE);
        }

        for (int i = 0; i < new_taille; ++i) {
            new_digits[i] = bb->Tdigits[index + i];
        }
        free(bb->Tdigits);
        bb->Tdigits = new_digits;
        bb->Taille = new_taille;
        if (bb->Signe == 0) {
            bb->Signe = 1;
        }
    }
}

BigBinary Addition(BigBinary a, BigBinary b) {      // Différents cas de base
    if (a.Taille<b.Taille) {                // Assure que a est plus grand que b
        return Addition(b,a);
    }
    if (a.Signe == 0) {
        return b;
    }
    if (b.Signe == 0) {
        return a;
    }
    BigBinary s;
    s = createBigBinary(a.Taille+1);
    s.Signe = 1;
    int sui = 0;        // Garde en mémoire l'incrément (de l'étape suivante) et la valeur à ajouter
    int i = 1;
    while (i<=b.Taille) {
        sui = sui+a.Tdigits[a.Taille-i]+b.Tdigits[b.Taille-i];
        s.Tdigits[s.Taille-i] = sui%2;
        sui /= 2;
        i++;
    }
    while (sui!= 0 && i<=a.Taille) {
        sui = sui+a.Tdigits[a.Taille-i];
        s.Tdigits[s.Taille-i] = sui%2;
        sui /= 2;
        i++;
    }
    while (i <= a.Taille) {
        s.Tdigits[s.Taille-i]=a.Tdigits[a.Taille-i];
        i++;
    }
    if (sui != 0) {
        s.Tdigits[s.Taille-i] = sui;
    }
    normalizeBB(&s);
    return s;
}

BigBinary Soustraction(BigBinary a, BigBinary b) {
    if (b.Signe == 0) {
        return a;
    }
    BigBinary s;
    s = createBigBinary(a.Taille);
    s.Signe = 1;
    int sui = 0;        // Garde en mémoire l'incrément (de l'étape suivante) et la valeur à ajouter
    int i = 1;
    while (i<=b.Taille) {
        sui = a.Tdigits[a.Taille-i]-b.Tdigits[b.Taille-i]-sui;
        if (sui < 0) {
            s.Tdigits[s.Taille-i] = 1;
            sui = 1;
        }
        else {
            s.Tdigits[s.Taille-i] = sui;
            sui = 0;
        }
        i++;
    }
    while (sui!= 0 && i<=a.Taille) {
        sui = a.Tdigits[a.Taille-i]-sui;
        if (sui < 0) {
            s.Tdigits[s.Taille-i] = 1;
            sui = 1;
        }
        else {
            s.Tdigits[s.Taille-i] = sui;
            sui = 0;
        }
        i++;
    }
    while (i <= a.Taille) {
        s.Tdigits[s.Taille-i]=a.Tdigits[a.Taille-i];
        i++;
    }
    normalizeBB(&s);
    return s;
}

bool Egal(BigBinary a,BigBinary b){
    if ((a.Signe!=b.Signe)&&(a.Taille!=b.Taille))return false;
    for (int i=0;i<a.Taille;i++){
        if(a.Tdigits[i]!=b.Tdigits[i])return false;
    }
    return true;
}

bool Inferieur(BigBinary a,BigBinary b){
    if (a.Signe!=b.Signe||((a.Signe==0)&&(b.Signe==0))) return a.Signe<b.Signe;
    if (a.Taille!=b.Taille) return a.Taille<b.Taille; //il faut normaliser avant
    for (int i=1; i<a.Taille;i++){
        if(a.Tdigits[i]!=b.Tdigits[i]) return a.Tdigits!=1; //si a=1 alors b=0 donc a>b
    }
    return false;//==
}

BigBinary Multiplication(BigBinary a,int n){
    BigBinary res = initBigBinary(a.Taille,a.Signe);
    for (int i=0;i<n;i++) res=Addition(res,a);
    return res;
}

BigBinary minBB(BigBinary a, BigBinary b){
    if (Inferieur(a,b)) return a;
    return b;
}

BigBinary maxBB(BigBinary a,BigBinary b){
    if (Inferieur(a,b))return b;
    return a;
}

BigBinary PGCD(BigBinary a,BigBinary b){
    BigBinary u=maxBB(a,b);
    BigBinary v=minBB(a,b);
    while (!Egal(u,v)){
        if(Inferieur(v,u))u=Soustraction(u,v);
        else v=Soustraction(v,u);
    }
    return u;
}

int puissance(int a,int b){// pas pour les BB
    for (int i=0;i<b;i++) a+=a;
    return a;
}

int BBtoInt(BigBinary a){
    int res=0;
    for (int i=a.Taille-1;i>0;i--){
        if(a.Tdigits[i]==1)res+=puissance(2,a.Taille-i);
    }
    return res;
}

BigBinary Modulo(BigBinary a,BigBinary b){
    //a et b positif
    if (Egal(a,b)) return initBigBinary(0,0);
    if (!Inferieur(a,b))return Modulo(Soustraction(a,b),b);
    BigBinary p = PGCD(a,b);
    if (!Inferieur(a,Multiplication(b,2)))return Modulo(Soustraction(a,Multiplication(b,2)),b);
    if (!Inferieur(a,Multiplication(b,3)))return Modulo(Soustraction(a,Multiplication(b,3)),b);
    if (Inferieur(a,b))return a;
}

BigBinary MultiplicationEgyptienne(BigBinary a,BigBinary b){
    BigBinary somme = initBigBinary(a.Taille,0);
    BigBinary tmp = a;
    for (int i=a.Taille-1;i>0;i++){
        if (b.Tdigits[i]==1){
            Addition(somme,tmp);
        }
        Multiplication(tmp,2);
    }
    return somme;
}

//fonction de test
void testAfficheBigBinary(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

    printf("A (83) : "); printf("(1010011)" );afficheBigBinary(A);
    printf("B (57) : "); printf("(111001)" );afficheBigBinary(B);
    printf("C (26) : "); printf("(11010)" );afficheBigBinary(C);
    printf("D (83) : "); printf("(1010011)" );afficheBigBinary(D);
    printf("Z (0) : "); printf("(0)" );afficheBigBinary(Z);
    printf("E (140) : "); printf("(10001100)" );afficheBigBinary(E);
    printf("N (-13) : "); printf("(-1101)" );afficheBigBinary(N);
    printf("M (100) : "); printf("(1100100)" );afficheBigBinary(M);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

void testEgalInf(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

    printf("Egal(A, D) (83 == 83) : %s\n", Egal(A, D) ? "true" : "false"); // true
    printf("Egal(A, B) (83 == 57) : %s\n", Egal(A, B) ? "true" : "false"); // false
    printf("Inferieur(B, A) (57 < 83) : %s\n", Inferieur(B, A) ? "true" : "false"); // true
    printf("Inferieur(A, D) (83 < 83) : %s\n", Inferieur(A, D) ? "true" : "false"); // false
    printf("Inferieur(Z, A) (0 < 83) : %s\n", Inferieur(Z, A) ? "true" : "false"); // true
    printf("Inferieur(N, A) (-13 < 83) : %s\n", Inferieur(N, A) ? "true" : "false"); // true

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

void testAddSub(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

    BigBinary Sum = Addition(A, B);
    printf("A + B (83 + 57 = 140): "); printf("(10001100) "); afficheBigBinary(Sum);
    libereBigBinary(&Sum);

    BigBinary Sum2 = Addition(A, D);
    printf("A + D (83 + 83 = 166): "); printf("(10100110) ");afficheBigBinary(Sum2);
    libereBigBinary(&Sum2);

    BigBinary Diff = Soustraction(A, B);
    printf("A - B (83 - 57 = 26): ");printf("(11010) ");afficheBigBinary(Diff);
    libereBigBinary(&Diff);

    BigBinary Diff2 = Soustraction(A, D);
    printf("A - D (83 - 83 = 0): ");printf("(0) "); afficheBigBinary(Diff2); // 0
    libereBigBinary(&Diff2);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

void testMinMaxPGCD(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

    BigBinary min_ab = minBB(A, B);
    BigBinary max_ab = maxBB(A, B);
    BigBinary min_an = minBB(A, N); // min(83, -13) = -13
    
    printf("min(A, B) (min(83, 57) = 57): "); afficheBigBinary(min_ab);
    printf("max(A, B) (max(83, 57) = 83): "); afficheBigBinary(max_ab);
    printf("min(A, N) (min(83, -13) = -13): "); afficheBigBinary(min_an);
    
    libereBigBinary(&min_ab);
    libereBigBinary(&max_ab);
    libereBigBinary(&min_an);

    // PGCD(83, 57) = 1
    BigBinary pgcd_ab = PGCD(A, B);
    printf("PGCD(A, B) (PGCD(83, 57) = 1): "); afficheBigBinary(pgcd_ab); 
    libereBigBinary(&pgcd_ab);

    // PGCD(100, 26) = 2
    BigBinary pgcd_mc = PGCD(M, C);
    printf("PGCD(M, C) (PGCD(100, 26) = 2): "); afficheBigBinary(pgcd_mc);
    libereBigBinary(&pgcd_mc);
    
    // PGCD(140, 26) = 2
    BigBinary pgcd_dc = PGCD(D, C);
    printf("PGCD(D, C) (PGCD(140, 26) = 2): "); afficheBigBinary(pgcd_dc);
    libereBigBinary(&pgcd_dc);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

void testBBtoInt(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

     int int_a = BBtoInt(A);
    int int_b = BBtoInt(B);
    int int_n = BBtoInt(N);
    int int_z = BBtoInt(Z);

    printf("BBtoInt(A) (83) : %d\n", int_a);
    printf("BBtoInt(B) (57) : %d\n", int_b);
    printf("BBtoInt(N) (-13) : %d\n", int_n);
    printf("BBtoInt(Z) (0) : %d\n", int_z);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

void testModulo(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

    // Modulo(83, 57) = 26
    BigBinary mod_ab = Modulo(A, B);
    printf("Modulo(A, B) (83 %% 57 = 26): "); afficheBigBinary(mod_ab);
    libereBigBinary(&mod_ab);
    
    // Modulo(100, 26) = 22
    BigBinary mod_mc = Modulo(M, C);
    printf("Modulo(M, C) (100 %% 26 = 22): "); afficheBigBinary(mod_mc);
    libereBigBinary(&mod_mc);

    // Modulo(26, 83) = 26
    BigBinary mod_ca = Modulo(C, A);
    printf("Modulo(C, A) (26 %% 83 = 26): "); afficheBigBinary(mod_ca);
    libereBigBinary(&mod_ca);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

void testEgypt(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

    // 5 * 13 = 65
    BigBinary T13 = setBB(13);
    BigBinary T5 = setBB(5);
    BigBinary mult_13_5 = MultiplicationEgyptienne(T13, T5);
    printf("13 * 5 = 65: "); afficheBigBinary(mult_13_5);
    libereBigBinary(&mult_13_5);
    
    // 83 * 57 = 4731
    BigBinary mult_ab = MultiplicationEgyptienne(A, B);
    printf("A * B (83 * 57 = 4731): "); afficheBigBinary(mult_ab);
    libereBigBinary(&mult_ab);

    // -13 * 5 = -65
    BigBinary mult_n_5 = MultiplicationEgyptienne(N, T5);
    printf("N * 5 (-13 * 5 = -65): "); afficheBigBinary(mult_n_5);
    libereBigBinary(&mult_n_5);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

int main () {

    //testAfficheBigBinary();
    //testEgalInf();
    //testAddSub();
    //testMinMaxPGCD(); //erreur
    //testBBtoInt(); //erreur
    //testModulo();
    testEgypt();

    return 0;
}